#include "medSourcesWidget.h"

#include <medDataHub.h>
#include <medSourceModel.h>
#include <medSourceModelItem.h>
#include <medSourceModelPresenter.h>

#include <medDataInfoWidget.h>

#include <QPushButton>
#include <QToolButton>
#include <QTreeView>
#include <QAction>
#include <QLabel>

class medSortFilterProxyModel : public QSortFilterProxyModel
{
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override
    {
        return source_column == 0;
    }

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override
    {
        bool bRes = false;
        for (int col = 0; col < sourceModel()->columnCount(source_parent); col++)
        {
            QModelIndex index1 = sourceModel()->index(source_row, col, source_parent);
            if (sourceModel()->data(index1).toString().contains(filterRegExp()))
            {
                bRes = true;
            }
        }
        return bRes;
    }
};

medSourcesWidget::medSourcesWidget()
{
     this->setLayout(&m_layout);
     m_layout.setAlignment(Qt::AlignTop);
     m_layout.setContentsMargins(0, 0, 0, 0);
}

medSourcesWidget::~medSourcesWidget()
{
}

void medSourcesWidget::addSources(medDataHub *dataHub)
{
    auto  sourceModels = dataHub->models();
    for (auto sourceModel : sourceModels)
    {
        addSource(dataHub, sourceModel->getSourceIntanceId());
    }
}

void medSourcesWidget::addSource(medDataHub *dataHub, QString sourceInstanceId)
{
    auto                        *sourceModel = dataHub->getModel(sourceInstanceId);
    QString                      instanceName = medSourceHandler::instance()->getInstanceName(sourceInstanceId);
    bool isOnline, isLocal, isWritable, isCached;
    medSourceHandler::instance()->sourceGlobalInfo(sourceInstanceId,isOnline, isLocal, isWritable, isCached);
    medSourceModelPresenter *sourcePresenter = new medSourceModelPresenter(sourceModel);

    QToolButton *sourceTreeBttn = new QToolButton();
    auto *treeButtonAction = new QAction(QIcon::fromTheme("arrow-bot"), QString("arrow"));
    treeButtonAction->setData(sourceInstanceId);
    sourceTreeBttn->setDefaultAction(treeButtonAction);
    QLabel      *sourceTreeTitle = new QLabel(instanceName);
    QPushButton *plusButton = new QPushButton(QIcon::fromTheme("plus"), "");

    sourceTreeBttn->setToolTip("Show/Hide Source");
    sourceTreeBttn->setMaximumSize(QSize(20,20));
 
    plusButton->setToolTip("Expand All");
    plusButton->setMaximumSize(QSize(20,20));
    plusButton->setCheckable(true);
    plusButton->setFlat(true);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(sourceTreeBttn);
    hLayout->addWidget(sourceTreeTitle);
    hLayout->addWidget(plusButton, 0, Qt::AlignRight);


    QTreeView   *sourceTreeView  = sourcePresenter->buildTree(new medSortFilterProxyModel());
    connect(sourceTreeView, &QTreeView::doubleClicked, this, &medSourcesWidget::onDoubleClick);
    
    connect(sourceTreeBttn, &QToolButton::triggered, this, &medSourcesWidget::expandSourceTree);
    connect(plusButton, &QPushButton::toggled, [=](bool checked) {
        if (checked)
        {
            plusButton->setIcon(QIcon::fromTheme("minus"));
            dataHub->expandAll(sourceInstanceId);
            sourceTreeView->expandAll();
        }
        else
        {
            plusButton->setIcon(QIcon::fromTheme("plus"));
            sourceTreeView->collapseAll();
        }
    });

    sourceTreeView->setDragEnabled(true);
    sourceTreeView->viewport()->setAcceptDrops(true);
    sourceTreeView->setAcceptDrops(true);
    //sourceTreeView->setDropIndicatorShown(true);
    sourceTreeView->setDragDropMode(QAbstractItemView::DragDrop);

    //context menu code
    auto pMenu = new medSourceContextMenu(sourceTreeView);
    sourceTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_TreeviewByMenuMap[pMenu] = sourceTreeView;
    QAction *pushAction    = new QAction(tr("Push"),          pMenu);
    QAction *refreshAction = new QAction(tr("Refresh"),       pMenu);
    QAction *saveAction    = new QAction(tr("Save on disk"),  pMenu);
    QAction *removeAction  = new QAction(tr("Remove"),        pMenu);
    QAction *fetchAction   = new QAction(tr("Fetch"),         pMenu);
//    QAction *preloadAction = new QAction(tr("Pre-Load"),      pMenu);
    QAction *readerAction  = new QAction(tr("Change reader"), pMenu);
    QAction *unloadAction  = new QAction(tr("Unload"),        pMenu);
    QAction *infoAction    = new QAction(tr("Information"),   pMenu);
	QAction *createFolderAction = new QAction(tr("Create Folder"), pMenu);
    pMenu->addAction(pushAction);
    pMenu->addAction(refreshAction);
    pMenu->addAction(saveAction);
    pMenu->addAction(removeAction);
    pMenu->addAction(fetchAction);
    if (isLocal)
    {
        fetchAction->setDisabled(true);
    }
//    pMenu->addAction(preloadAction);
    pMenu->addAction(readerAction);
    pMenu->addAction(unloadAction);
	pMenu->addAction(infoAction);
	pMenu->addAction(createFolderAction);

    //connect(pushAction,    &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    //connect(refreshAction, &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    //connect(saveAction,    &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    //connect(removeAction,  &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    //connect(fetchAction,   &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    //connect(preloadAction, &QAction::triggered, [=]() {
    //    QModelIndex index = this->indexFromMenu(pMenu);
    //    if (index.isValid())
    //    {
    //        const_cast<medSourceModel*>(static_cast<const medSourceModel*>(index.model()))->fetchData(index);
    //    }
    //});
    connect(fetchAction, &QAction::triggered, [=]() {
        QModelIndex index = this->indexFromMenu(pMenu);
        if (index.isValid())
        {
            const_cast<medSourceModel*>(static_cast<const medSourceModel*>(index.model()))->fetchData(index);
        }
    });

    //connect(readerAction,  &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    //connect(unloadAction,  &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    connect(infoAction,    &QAction::triggered, [=]() {
        medSourceHandler::datasetAttributes mandatoriesAttributes;

        QModelIndex index = this->indexFromMenu(pMenu);
        if (index.isValid())
        {
            mandatoriesAttributes = sourceModel->getMendatoriesMetaData(index);
            QMap<QString, QString> dataAttributes;
            for (auto & key : mandatoriesAttributes.values.keys())
            {
                dataAttributes[key] = mandatoriesAttributes.values[key];
            }
            
            medDataIndex medIndex = static_cast<const medSourceModel*>(index.model())->dataIndexFromModelIndex(index);
            medSourceHandler::datasetAttributes optionalAttr = dataHub->getOptionalMetaData(medIndex);
            QMultiMap<QString, QString> optionalMultiMap;

            for (auto key: optionalAttr.values.keys())
            {
                if (optionalAttr.tags.contains(key))
                {
                    auto tag = optionalAttr.tags.take(key);
                    optionalMultiMap.insert(key, tag);
                }
                optionalMultiMap.insert(key, optionalAttr.values[key]);
            }

            for (auto key: optionalAttr.tags.keys())
            {
                optionalMultiMap.insert(key, optionalAttr.tags[key]);
                optionalMultiMap.insert(key, " ");
            }

            auto popupDataInfo = new medDataInfoWidget(dataAttributes, optionalMultiMap);
            popupDataInfo->show();
        }

    });



    connect(sourceTreeView, &QTreeView::customContextMenuRequested, [=](QPoint const& point) { onCustomContextMenu(point, pMenu); });

    m_layout.addLayout(hLayout);
    m_layout.addWidget(sourceTreeView);

    connect(sourceTreeView, &QTreeView::clicked, [=]() {
        if (!sourceTreeView->isEnabled())
        {
            dataHub->sourceIsOnline(sourceInstanceId);
        }
    });
    
    connect(sourceModel, &medSourceModel::online, sourceTreeView, &QTreeView::setEnabled);
 
	connect(createFolderAction, &QAction::triggered, [=]() {
		QModelIndex index = this->indexFromMenu(pMenu);
		if (index.isValid())
		{
			auto model = const_cast<medSourceModel*>(static_cast<const medSourceModel*>(index.model()));
			//TODO create 

		}
	});
    
    m_treeMap  [sourceInstanceId] = sourceTreeView;
    m_titleMap [sourceInstanceId] = hLayout;
    delete sourcePresenter;
}

void medSourcesWidget::removeSource(QString sourceInstanceId)
{
    if (m_treeMap.contains(sourceInstanceId))
    {
        auto widget = m_treeMap.take(sourceInstanceId);
        auto titleLayout = m_titleMap.take(sourceInstanceId);
        
        QLayoutItem *child;
        while ((child = titleLayout->takeAt(0)) != nullptr)
        {
            delete child->widget(); // delete the widget
            delete child;   // delete the layout item
        }

        //m_layout.removeWidget(widget);
        //m_layout.removeWidget(label);

        //TODO made disconnection here
        
        delete widget;
        delete titleLayout;
    }
}

void medSourcesWidget::setSourceVisible(QString sourceInstanceId, bool checked)
{
    if (m_titleMap.contains(sourceInstanceId) && m_treeMap.contains(sourceInstanceId))
    {
        auto widget = m_treeMap.value(sourceInstanceId);
        auto title = m_titleMap.value(sourceInstanceId);
        
        for (int i=0; i<title->count(); i++)
        {
            QLayoutItem *child = title->itemAt(i);
            QWidget *wgt = child->widget();
            wgt->setVisible(checked);
        }
        widget->setVisible(checked); 
        auto button = dynamic_cast<QToolButton *>(title->itemAt(0)->widget());
        if (widget->isVisible())
        {
            button->setIcon(QIcon::fromTheme("arrow-bot"));
        }
        else
        {
            button->setIcon(QIcon::fromTheme("arrow-right"));
        }
 
    }

}

void medSourcesWidget::expandSourceTree(QAction *action)
{
    QString sourceInstanceId = action->data().toString();
    if (m_titleMap.contains(sourceInstanceId) && m_treeMap.contains(sourceInstanceId))
    {
        auto widget = m_treeMap.value(sourceInstanceId);
        auto layout = m_titleMap.value(sourceInstanceId);
        auto button = dynamic_cast<QToolButton *>(layout->itemAt(0)->widget());
        if (widget->isVisible())
        {
            button->setIcon(QIcon::fromTheme("arrow-right"));
            
        }
        else
        {
            button->setIcon(QIcon::fromTheme("arrow-bot"));
        }
        widget->setVisible(!widget->isVisible()); 
    }
}

void medSourcesWidget::filter(const QString &text)
{
    for ( auto sourceTreeView : m_treeMap.values())
    {
        auto tree = dynamic_cast<QTreeView*>(sourceTreeView);
        auto proxy = dynamic_cast<medSortFilterProxyModel *>(tree->model());
        QRegExp regExp(text, Qt::CaseInsensitive, QRegExp::Wildcard);
        proxy->setFilterRegExp(regExp);
        proxy->setRecursiveFilteringEnabled(true);
    }
}

void medSourcesWidget::onDoubleClick(QModelIndex const & index)
{
    auto proxy = static_cast<const medSortFilterProxyModel*>(index.model());
    auto sourceIndex = proxy->mapToSource(index);
    medDataIndex medIndex = static_cast<const medSourceModel*>(sourceIndex.model())->dataIndexFromModelIndex(sourceIndex);
    emit openOnDoubleClick(medIndex);
}

void medSourcesWidget::onCustomContextMenu(QPoint const &point, QMenu *pi_pMenu)
{
    auto pTreeView = m_TreeviewByMenuMap[pi_pMenu];
    QModelIndex index = pTreeView->indexAt(point);
    if (index.isValid())
    {
        QPoint pos = pTreeView->viewport()->mapToGlobal(point);
		//TODO Here Hide invalide action
        pi_pMenu->exec(pos);
    }
}

QModelIndex medSourcesWidget::indexFromMenu(QMenu * pi_pMenu)
{
    QModelIndex indexRes;

    auto sourceTreeView = m_TreeviewByMenuMap[pi_pMenu];    
    auto proxy = static_cast<medSortFilterProxyModel*>(sourceTreeView->model());
    auto pos = sourceTreeView->viewport()->mapFromGlobal(pi_pMenu->pos());
    indexRes = proxy->mapToSource(sourceTreeView->indexAt(pos));

    return indexRes;
}


