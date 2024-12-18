#include "AsyncNetwork.h"

#include <QDir>
#include <QHttpPart>

#include <medShanoir.h>
#include <medNotif.h>

#include <LocalInfo.h>
#include <Authenticator.h>
#include <RequestManager.h>
#include <SyncNetwork.h>
#include <RequestPreparation.h>

#include <FileHelper.h>
#include <JsonHelper.h>


AsyncNetwork::AsyncNetwork(medShanoir * parent, LocalInfo *info, Authenticator * authent, SyncNetwork *syncNet, RequestManager * requester): 
	QObject(parent), m_parent(parent), m_info(info), m_authent(authent), m_syncNet(syncNet), m_medReqId(0), m_rootPath(m_info->getStoragePath() + "default")
{
	// connecting the AsyncNetwork object to the RequestManager object
	QObject::connect(this, &AsyncNetwork::asyncGet, requester, &RequestManager::httpGet, Qt::ConnectionType::QueuedConnection);
	QObject::connect(this, &AsyncNetwork::asyncPost, requester, &RequestManager::httpPost, Qt::ConnectionType::QueuedConnection);
	QObject::connect(this, &AsyncNetwork::asyncPostMulti, requester, &RequestManager::httpPostMulti, Qt::ConnectionType::QueuedConnection);
	QObject::connect(this, &AsyncNetwork::asyncPut, requester, &RequestManager::httpPut, Qt::ConnectionType::QueuedConnection);

	QObject::connect(this, &AsyncNetwork::asyncAbort, requester, &RequestManager::abort, Qt::ConnectionType::QueuedConnection);

}

AsyncNetwork::~AsyncNetwork()
{
	// deletion of all the folders that contain files that were created by async network
	deleteAllFolders(m_filesToRemove);
	// deletion of the folder that contains all those folders 
	deleteAllFolders(QStringList() << m_rootPath);
}

/**
 * 
 * SLOTS
 * 
*/

void AsyncNetwork::asyncGetSlot(QUuid netReqId, QByteArray payload, QJsonObject headers, int statusOrHttpCode)
{
	if (m_requestIdMap.contains(netReqId))
	{
		getAsyncDataInterpretation(netReqId , { statusOrHttpCode, headers, payload });
	}
}


void AsyncNetwork::asyncPostSlot(QUuid netReqId, QByteArray payload, QJsonObject headers, int statusOrHttpCode)
{
	if(m_requestIdMap.contains(netReqId))
	{
		addAsyncDataInterpretation(netReqId , { statusOrHttpCode, headers, payload });
	}
}

void AsyncNetwork::asyncPutSlot(QUuid netReqId, QByteArray payload, QJsonObject headers, int statusOrHttpCode)
{
	if(m_requestIdMap.contains(netReqId))
	{
		// HERE call the interpretation(s) function that is/are based on PUT request 
		// for now, none of them exist
	}
}


/**
 * 
 * GET DATA methods
 * 
*/

int AsyncNetwork::getAssyncData(unsigned int pi_uiLevel, QString key)
{
 	int medId = -1;

	// checking that the level is appropriate for a processed dataset download
 	QStringList parts = key.split('.');
 	bool dataset_level = pi_uiLevel == 4 && parts.size() == 5;
 	bool ps_dataset_level = pi_uiLevel == 6 && parts.size() == 7;
 	if (dataset_level || ps_dataset_level) // dataset level or processed dataset
 	{
 		int id_ds = parts[4].toInt();
 		if (ps_dataset_level)
 		{
 			id_ds = parts[6].toInt();
 		}
		int medIdRequest = ++m_medReqId;
		QUuid netReqId = getDataset(medIdRequest, id_ds, false);
		medId = m_requestIdMap[netReqId].first;
 	}

 	return medId;
}


QUuid AsyncNetwork::getDataset(int medId, int id_ds, bool conversion)
{
	// generation of the requests id
	QUuid netReqId = QUuid::createUuid();

	m_requestIdMap[netReqId].first = medId;
	m_requestIdMap[netReqId].second = getData;
	// saving temporarily the information about the request in the result map 
	// format idDataset.infoConversion 
	// infoConversion worth 0 if conversion is false, and 1 if conversion is true
	m_idResultMap[medId] = QString::number(id_ds)+"."+ QString::number(conversion);

	// sending the request
	QNetworkRequest req;
	writeNiftiDatasetRetrievingRequest(req, m_info->getBaseURL(), m_authent->getCurrentAccessToken(), id_ds, conversion);

	emit asyncGet(netReqId, req);

	return netReqId;
}

void AsyncNetwork::getAsyncDataInterpretation(QUuid netReqId, RequestResponse res)
{
	int medId = m_requestIdMap[netReqId].first;
	if (res.code == SUCCESS_CODE) // finished with success
	{
		// successCode is about success of the conversion of the retrieved data into a file
		int successCode = dataToFile(netReqId, res);
		QString tmpResult = m_idResultMap[medId].toString();
		if(successCode == 1 && tmpResult.size()>0)
		{
			// successCode worth 1 : a retry is necessary
			// the temporary saved information is retrieved (format idDataset.infoConversion)
			QStringList parts = tmpResult.split('.');
			if(parts.size()==2 && parts[1].toInt() == 0)
			{
				// no conversion was done, therefore a retry of the request is made,
				// asking for shanoir to make the convertion of the dataset to a niftii file
				getDataset(medId, parts[0].toInt(), true);
			}
			else
			{
				// no retry will be done, we consider the request is finished
				successCode = 0;
			} 
		}
		else if(successCode == -1) 
		{
			emit m_parent->progress(medId, eRequestStatus::faild);
		}
		if(successCode == 0)
		{
			emit m_parent->progress(medId, eRequestStatus::finish);
		}
		m_requestIdMap.remove(netReqId);
	}
	else if (res.code == UPLOAD_CODE || res.code == DOWNLOAD_CODE) // request in progress
	{
		int bytesSent = res.headers["bytesSent"].toInt();
		int bytesTotal = res.headers["bytesTotal"].toInt();
		emit m_parent->progress(medId, eRequestStatus::pending);
	}
	else // an error occured 
	{
		// notify
		QString errorMessage = "ERROR WHILE GETTING DATA \n NETWORK ERROR CODE :" + QString::number(res.code);
		medNotif::createNotif(notifLevel::error, "SHANOIR PLUGIN", errorMessage);
		// trace
		qDebug() << "\nNETWORKERROR (code = " << res.code << ") WHILE GETTING DATA ( request number ="<< medId << ")\nLOOK AT https://doc.qt.io/qt-5/qnetworkreply.html#NetworkError-enum for more information\n";
		// handling the error
		emit m_parent->progress(medId, eRequestStatus::faild);
		m_requestIdMap.remove(netReqId);
	}
}


QVariant AsyncNetwork::getAsyncResults(int pi_iRequest)
{
	QVariant qRes;
	if (m_idResultMap.contains(pi_iRequest)) 
	{
		qRes = m_idResultMap[pi_iRequest];
	}
	return qRes;
}

int AsyncNetwork::dataToFile(QUuid netReqId, RequestResponse res)
{
	int successCode = (res.payload.size() < 100) ? 1 : -1;
	int medId = m_requestIdMap[netReqId].first;
	if (successCode != 1)
	{
		// updating the root path with the instance id
		m_rootPath = m_info->getStoragePath() + m_info->getInstanceId() + "/";
		// creation of the nifti file
		QVariant pathRes = decompressNiftiiFromRequest(m_rootPath + QString::number(medId) + "/", res.headers, res.payload, m_filesToRemove, 60000);//1 minute before deletion
		if (pathRes.type() == QVariant::String)
		{ // everything went well, we receive the corresponding path
			m_idResultMap[medId] = pathRes;
			m_requestIdMap.remove(netReqId);
			successCode = 0;
		}
		else if (pathRes.type() == QVariant::Int)
		{
			qDebug() << "DECOMPRESSION ERROR " << pathRes.toInt();
			m_idResultMap[medId] = QVariant();
		}
	}
	return successCode;
}

/**
 * 
 * ADD DATA methods
 * 
*/

int AsyncNetwork::addAssyncData(QVariant data, levelMinimalEntries & pio_minimalEntries, unsigned int pi_uiLevel, QString parentKey)
{
	int medId = -1;
	QString path = data.toString(); 

	// checking that the level is appropriate for a processed dataset creation
	int parent_level = pi_uiLevel - 1; // working with parent of the data to create (easier for understanding)
	QStringList parts = parentKey.split('.');
	bool psing_dataset_level = parent_level == 5 && parts.size() == 6;
	if(psing_dataset_level && path.endsWith(".nii.gz"))
	{
		QVariant sending = sendProcessedDataset(path, pio_minimalEntries.name, parts[4].toInt(), parts[5].toInt());
		// sending is wether a boolean with the false, or a QUuid
		if(sending.isValid() && sending.canConvert<QUuid>())
		{
			QUuid netReqId = sending.value<QUuid>();
			medId =  m_requestIdMap[netReqId].first;
			// storing temporarily the parentKey, in order to help the next request-treating functions
			QJsonObject tmpResult = m_idResultMap[medId].toJsonObject();
			tmpResult["parentKey"] = parentKey;
			m_idResultMap[medId] = tmpResult;

		}
	}
	return medId;
}

QVariant AsyncNetwork::sendProcessedDataset(QString &filepath, QString name, int idDataset, int idProcessing)
{
	QString distant_path;

	// construction of the processed dataset to upload
	QFile *file = new QFile(filepath);
	file->open(QIODevice::ReadOnly);
	QFileInfo fileInfo(filepath);

	if (fileInfo.exists())
	{
		// generation of the requests ids 
		int medId = ++m_medReqId;
		QUuid netReqId = QUuid::createUuid();

		m_requestIdMap[netReqId].first = medId;
		m_requestIdMap[netReqId].second = addDataFile;

		// saving temporarily the informations about the request in the result map 
		QJsonObject tmpResult; 
		tmpResult.insert("idDataset", idDataset);
		tmpResult.insert("idProcessing", idProcessing);
		tmpResult.insert("name", name);
		m_idResultMap[medId] = tmpResult;

		// sending the request
		QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
		QNetworkRequest req;
		writeProcessedDatasetUploadRequest(req, multiPart, m_info->getBaseURL(), m_authent->getCurrentAccessToken(), file, fileInfo);

		emit asyncPostMulti(netReqId, req, multiPart);

		return netReqId;
	}
	else 
	{
		return false;
	}
}

QVariant AsyncNetwork::sentDatasetFileInterpretation(QUuid netReqIdFile, RequestResponse res)
{
	int medId = m_requestIdMap[netReqIdFile].first;

	QString distant_path;
	if (res.code == SUCCESS_CODE && !res.payload.isNull())
	{
		// retrieving the path returned by shanoir
		// it has a format of "/tmp/.../filecreated.nii"
		distant_path = QString::fromUtf8(res.payload);
	}
	QJsonObject tmpResult = m_idResultMap[medId].toJsonObject();

	if(distant_path.length()>0)
	{
		// generation of the request id
		QUuid netReqIdContext = QUuid::createUuid();

		m_requestIdMap[netReqIdContext].first = medId;
		m_requestIdMap[netReqIdContext].second = addDataContext;

		// preparation of the request
		int dsId = tmpResult["idDataset"].toInt();
		int psingId = tmpResult["idProcessing"].toInt();
		QString name = tmpResult["name"].toString();

		// building the context of the processed dataset
		DatasetDetails ds_details = m_syncNet->getDatasetDetails(dsId);
		StudyDetails s = m_syncNet->getStudyDetails(ds_details.study_id);
		StudyOverview study = {s.id, s.name};
		QString processedDatasetType = "RECONSTRUCTEDDATASET";
		QString processedDatasetName = name;
		QJsonObject parentDatasetProcessing = m_syncNet->getDatasetProcessing(psingId);

		// update the temporary result by saving the current parent dataset Processing in order to compare it after the upload of the new dataset
		tmpResult["parentDatasetProcessing"] = parentDatasetProcessing;
		m_idResultMap[medId] = tmpResult;
			
		// sending the request
		QNetworkRequest req;
		QByteArray postData;
		writeProcessedDatasetUploadContextRequest(req, postData, m_info->getBaseURL(), m_authent->getCurrentAccessToken(), ds_details, study, distant_path, processedDatasetType, processedDatasetName, parentDatasetProcessing);

		emit asyncPost(netReqIdContext, req, postData);

		return netReqIdContext;
	}
	return false;
}

void AsyncNetwork::sentDatasetContextInterpretation(QUuid netReqId, RequestResponse res)
{
	// check that the upload is successful and creation 
	// if so, the tree-view key for the new processed dataset is created

	int medId = m_requestIdMap[netReqId].first;
	QJsonObject tmpResult = m_idResultMap[medId].toJsonObject();

	// retrieving the metadata about this dataset with the temporary results
	QString parentKey = tmpResult["parentKey"].toString();
	int dsId = tmpResult["idDataset"].toInt();
	int psingId = tmpResult["idProcessing"].toInt();
	QString name = tmpResult["name"].toString();
	QJsonObject prevPsing = tmpResult["parentDatasetProcessing"].toObject();
	QJsonObject currentPsing = m_syncNet->getDatasetProcessing(psingId);

	int prevPsingSize = prevPsing.value("outputDatasets").toArray().size();
	int currPsingSize = m_syncNet->getDatasetProcessing(psingId).value("outputDatasets").toArray().size();

	// the state of the parent (processing dataset) is compared
	// we check if the creation of the processed dataset has been done
	if (prevPsingSize + 1 == currPsingSize)
	{
		int newId = currentPsing.value("outputDatasets").toArray().last().toObject().value("id").toInt();
		medNotif::createNotif(notifLevel::info, "SHANOIR PLUGIN", "dataset "+name+" successfuly uploaded");	
		m_idResultMap[medId] = parentKey +"."+  QString::number(newId);
	}
	else
	{
		// we suppose it worked, but not in the usual way.
		// therefore we do not send notification.
		m_idResultMap[medId] = true;
	}
	
	emit m_parent->progress(medId, eRequestStatus::finish);
}


void AsyncNetwork::addAsyncDataInterpretation(QUuid netReqId, RequestResponse res)
{
	int medId = m_requestIdMap[netReqId].first;
	
	if (res.code == SUCCESS_CODE) // finished with success
	{
		// checking the add data step 
		AsyncRequestType step = m_requestIdMap[netReqId].second;
		if(step == addDataFile)
		{
			sentDatasetFileInterpretation(netReqId, res);
		}
		else if(step == addDataContext)
		{
			sentDatasetContextInterpretation(netReqId, res);
		}
	}
	else if (res.code == UPLOAD_CODE || res.code == DOWNLOAD_CODE) //request in progress
	{
		int bytesSent = res.headers["bytesSent"].toInt();
		int bytesTotal = res.headers["bytesTotal"].toInt();
		emit m_parent->progress(medId, eRequestStatus::pending);
	}
	else // an error occured during the request sending (http error)
	{
		// trace
		qDebug() << "\nNETWORKERROR (code = " << res.code << ") WHILE ADDING DATA ( request number ="<< medId << ")\nLOOK AT https://doc.qt.io/qt-5/qnetworkreply.html#NetworkError-enum for more information\n";
		// notify
		QString activity;
		if(m_requestIdMap[netReqId].second == addDataFile)
		{
			activity = "UPLOADING A DATASET";
		}
		else 
		{
			activity = "UPLOADING A DATASET CONTEXT";
		}
		QString errorMessage =  "ERROR WHILE "+ activity +"\n NETWORK ERROR CODE :"+QString::number(res.code);
		medNotif::createNotif(notifLevel::error, "SHANOIR PLUGIN", errorMessage);
		// handling the error
		emit m_parent->progress(medId, eRequestStatus::faild);
		m_requestIdMap.remove(netReqId);
		m_idResultMap[medId] = false;
	}
}


void AsyncNetwork::abort(int medId)
{
	//m_requestIdMap associates Uuid to medIds
	// here we need to retrieve a Uuid from a medId
	for (auto it = m_requestIdMap.begin(); it != m_requestIdMap.end(); ++it)
	{
		if (it.value().first == medId)
		{
			QUuid netReqId = it.key();
			emit asyncAbort(netReqId);
			return;
		}
	}
}