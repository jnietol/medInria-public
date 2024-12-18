#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medDataHub.h>

#include <medSourceContextMenu.h>

#include <QWidget>
#include <QObject>

#include <QModelIndex>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>

#include <medDataIndex.h>

class medSourceModelPresenter;

class medSourcesWidget : public QWidget
{
    Q_OBJECT

public:
    medSourcesWidget();
    ~medSourcesWidget();

    void addSources(medDataHub *dataHub);

public slots:
    void addSource(medDataHub *dataHub, QString sourceInstanceId);
    void removeSource(QString sourceInstanceId);
    void setSourceVisible(QString sourceInstanceId, bool checked);
    void expandSourceTree(QAction *action);
    void filter(const QString &);

    void onDoubleClick(QModelIndex const & index);

signals:
    void openOnDoubleClick(medDataIndex);

private:
    void onCustomContextMenu(QPoint const &point, QMenu *pi_pMenu);
    QModelIndex indexFromMenu(QMenu *pi_pMenu);
    

private:
    QVBoxLayout m_layout;
    QMap<QString, QWidget*>   m_treeMap; // sourceInstanceId, SubTreeWidget
    QMap<QString, QLayout*>   m_titleMap;  // sourceInstanceId, source instance names
    QMap<QMenu*,  QTreeView*> m_TreeviewByMenuMap;
};
