#include "medToolBoxActions.h"

#include <QtGui>

class medToolBoxActionsPrivate
{
public:

    QWidget *page;

    QPushButton* removeBt;
    QPushButton* viewBt;
    QPushButton* exportBt;
    QPushButton* bookmarkBt;
    QPushButton* importBt;
    QPushButton* loadBt;
    QPushButton* indexBt;
    QLabel* placeholder;

    QList<QPushButton*> btList;
    QMultiMap<QString, QString> itemToActions;
};

medToolBoxActions::medToolBoxActions( QWidget *parent /*= 0*/ ) : medToolBox(parent), d(new medToolBoxActionsPrivate)
{
    d->page = new QWidget(this);

    /* Begin Initialization of itemsToActions map */

    d->itemToActions = *(new QMultiMap<QString, QString>());

    d->itemToActions.insert("Patient", "Remove");

    d->itemToActions.insert("Series", "View");
    d->itemToActions.insert("Series", "Export");
    d->itemToActions.insert("Series", "Remove");

    d->itemToActions.insert("Folder", "Bookmark");
    d->itemToActions.insert("Folder", "Import");
    d->itemToActions.insert("Folder", "Index");
    d->itemToActions.insert("Folder", "Load");
    d->itemToActions.insert("Folder", "View");

    d->itemToActions.insert("File", "Import");
    d->itemToActions.insert("File", "Index");
    d->itemToActions.insert("File", "Load");
    d->itemToActions.insert("File", "View");

    /* End Initialization of itemsToActions map */

    d->removeBt = new QPushButton(tr("Remove"));
    d->removeBt->setAccessibleName("Remove");
    d->removeBt->setToolTip(tr("Remove selected item from the database."));
    d->viewBt = new QPushButton(tr("View"));
    d->viewBt->setAccessibleName("View");
    d->viewBt->setToolTip(tr("Load and visualize the currently selected item."));
    d->exportBt = new QPushButton(tr("Export"));
    d->exportBt->setAccessibleName("Export");
    d->exportBt->setToolTip(tr("Export the series."));
    d->importBt = new QPushButton(tr("Import"));
    d->importBt->setAccessibleName("Import");
    d->importBt->setToolTip(tr("Import (copy) item(s) into medInria's database."));
    d->loadBt = new QPushButton(tr("Load"));
    d->loadBt->setAccessibleName("Load");
    d->loadBt->setToolTip(tr("Temporary load the item(s) so as they can be used inside medInria,\nbut do not include them in the database."));
    d->indexBt = new QPushButton(tr("Index"));
    d->indexBt->setAccessibleName("Index");
    d->indexBt->setToolTip(tr("Include the item(s) into medInria's database but do not import (copy) them."));
    d->bookmarkBt = new QPushButton(tr("Bookmark"));
    d->bookmarkBt->setAccessibleName("Bookmark");
    d->bookmarkBt->setToolTip(tr("Bookmark selected folder/resource."));

    d->btList = *(new QList<QPushButton*>());
    d->btList << d->removeBt << d->viewBt << d->exportBt << d->bookmarkBt << d->importBt << d->loadBt << d->indexBt;

    foreach(QPushButton* bt, d->btList)
    {
        bt->setVisible(false);
        bt->setObjectName("actionToolBoxButton"); // set for style sheet medInria.qss
    }

    d->placeholder = new QLabel(tr("Select an item to see possible actions."));
    d->placeholder->setObjectName("actionToolBoxLabel");

    QGridLayout *glayout = new QGridLayout();
    glayout->addWidget(d->removeBt, 0, 0, Qt::AlignCenter);
    glayout->addWidget(d->viewBt, 0, 1, Qt::AlignCenter);
    glayout->addWidget(d->exportBt, 0, 2, Qt::AlignCenter);
    glayout->addWidget(d->importBt, 1, 0, Qt::AlignCenter);
    glayout->addWidget(d->loadBt, 1, 1, Qt::AlignCenter);
    glayout->addWidget(d->indexBt, 1, 2, Qt::AlignCenter);
    glayout->addWidget(d->bookmarkBt, 2, 1, Qt::AlignCenter);

    glayout->addWidget(d->placeholder, 3, 1, 1, 3, Qt::AlignCenter);

    d->page->setLayout(glayout);
    this->addWidget(d->page);

    connect(d->removeBt, SIGNAL(clicked()), this, SIGNAL(removeClicked()));
    connect(d->viewBt, SIGNAL(clicked()), this, SIGNAL(viewClicked()));
    connect(d->exportBt, SIGNAL(clicked()), this, SIGNAL(exportClicked()));
    connect(d->bookmarkBt, SIGNAL(clicked()), this, SIGNAL(bookmarkClicked()));
    connect(d->importBt, SIGNAL(clicked()), this, SIGNAL(importClicked()));
    connect(d->loadBt, SIGNAL(clicked()), this, SIGNAL(loadClicked()));
    connect(d->indexBt, SIGNAL(clicked()), this, SIGNAL(indexClicked()));

    this->setTitle(tr("Actions"));
}

medToolBoxActions::~medToolBoxActions()
{
    delete d;
    d = NULL;
}

void medToolBoxActions::patientSelected()
{
    updateButtons("Patient");
}

void medToolBoxActions::seriesSelected()
{
    updateButtons("Series");
}

void medToolBoxActions::pathSelected(QString path)
{
    QFileInfo fi(path);

    if (fi.isDir())
        updateButtons("Folder");
    else
        updateButtons("File");
}

void medToolBoxActions::updateButtons(QString item)
{
    QList<QString> actions = d->itemToActions.values(item);

    foreach(QPushButton* bt, d->btList)
    {
        if( actions.contains( bt->accessibleName()) )
        {
            bt->setVisible(true);
        }
        else
        {
           bt->setVisible(false);
        }
    }

    // insert a placeholder if no button is being displayed
    if(actions.size() == 0)
        d->placeholder->setVisible(true);
    else
        d->placeholder->setVisible(false);
}
