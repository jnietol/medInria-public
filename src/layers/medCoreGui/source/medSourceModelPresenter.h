#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <QObject>
#include <medCoreGuiExport.h>

class QWidget;
class QTreeView;
class QSortFilterProxyModel;
class medSourceModel;
class medSourceView;
class medSourceModelPresenterPrivate;

class MEDCOREGUI_EXPORT medSourceModelPresenter : public QObject
{
    Q_OBJECT
public:
    medSourceModelPresenter(medSourceModel *parent = nullptr);
    virtual ~medSourceModelPresenter();

    virtual QWidget* buildWidget();

    virtual QTreeView* buildTree(QSortFilterProxyModel *proxy = nullptr);

    virtual medSourceModel *sourceModel() const;


private:
    const QScopedPointer<medSourceModelPresenterPrivate> d;
};
