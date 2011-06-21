/* medDatabaseRemover.h --- 
 * 
 * Author: John Stark
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Jun 29 15:25:31 2010 (+0200)
 * Version: $Id$
 * Last-Updated: Tue Jun 29 15:32:21 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 12
 */

/* Commentary: 
 * 
 */

#ifndef MEDDATABASEREMOVER_H
#define MEDDATABASEREMOVER_H

#include <QtCore/QObject>

#include <medCore/medDataIndex.h>

class medDatabaseRemoverPrivate;

/**
 * @class medDatabaseRemover
 * @brief Removes given data from the database.
 */
class medDatabaseRemover : public QObject
{
    Q_OBJECT;

public:
     medDatabaseRemover(const medDataIndex &index);
    ~medDatabaseRemover();

    void run();

signals:
    void success(QObject *);
    void failure(QObject *);
    void progressed(int);
    void removed(const medDataIndex &);

protected:
    void removeImage( int imageId);


    bool isSeriesEmpty( int seriesId );
    void removeSeries( int seriesId );
    bool isStudyEmpty( int studyId );
    void removeStudy( int studyId );
    bool isPatientEmpty( int patientId );
    void removePatient( int patientId );

    void removeFile( const QString & filename );
    void removeTableRow( const QString &table, int id );

private:
    medDatabaseRemoverPrivate *d;
};

#endif
