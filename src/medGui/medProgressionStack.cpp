/* medProgressionStack.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Jan 19 13:39:47 2010 (+0100)
 * Version: $Id$
 * Last-Updated: Tue Jun 15 20:01:36 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 71
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "medProgressionStack.h"

#include <QtCore>
#include <QtGui>

#include <medCore/medJobItem.h>
#include <medCore/medMessageController.h>

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

QString medChop(const QString& string)
{
    QString result = string;

    if(string.size() > 15) {
        result.resize(12);
        result.append("...");
    }
    
    return result;
}

// /////////////////////////////////////////////////////////////////
// medProgressionStack
// /////////////////////////////////////////////////////////////////

class medProgressionStackPrivate
{
public:
    QVBoxLayout *layout;

    QHash<QObject *, QProgressBar *> bars;
    QHash<QObject *, QPushButton*> buttons;
    QHash<QObject *, QObject *> buttonsSender;
    QHash<QObject *, QWidget *> widgets;
    QQueue<QObject *> itemstoBeRemoved;
};

medProgressionStack::medProgressionStack(QWidget *parent) : QWidget(parent), d(new medProgressionStackPrivate)
{
    d->layout = new QVBoxLayout(this);
    d->layout->setContentsMargins(0, 0, 0, 0);
    d->layout->setSpacing(4);
}

medProgressionStack::~medProgressionStack(void)
{
    delete d;

    d = NULL;
}

QSize medProgressionStack::sizeHint(void) const
{
    if(d->bars.count())
        return QSize(d->bars.values().first()->width(), (d->bars.values().first()->height()+4) * d->bars.values().count());
    else
        return QSize();
}

void medProgressionStack::setLabel(QObject *sender, QString label)
{
    if(d->bars.count() == 0)
        emit(shown());

    if (d->bars.contains (sender)) {
        qWarning () << "progression stack already has a label assigned to process " << sender << ", not assigning a new one";
	return;
    }

    QWidget *widget = new QWidget(this);

    QLabel *ilabel = new QLabel(medChop(label), widget);

    QProgressBar *bar = new QProgressBar(widget);
    bar->setRange(0, 100);
    d->bars.insert(sender, bar);

    QPushButton *button= new QPushButton(widget);
    button->setText("C");
    connect(button,SIGNAL(clicked()), this,SLOT(sendCancelRequest()));
    d->buttons.insert(sender,button);
    d->buttonsSender.insert(button, sender);

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(ilabel);
    layout->addWidget(bar);
    layout->addWidget(button);
    d->widgets.insert(sender, widget);

    d->layout->addWidget(widget);
}

void medProgressionStack::setProgress(QObject* sender, int progress)
{
    if (d->bars.contains(sender)) {

        if(!d->bars.value(sender)->isHidden())
            d->bars.value(sender)->setValue(progress);
    }
}

void medProgressionStack::onSuccess (QObject* sender)
{
    completeNotification(sender, tr("Successful"));
}

void medProgressionStack::onFailure (QObject* sender)
{
    completeNotification(sender, tr("Failure"));
}

void medProgressionStack::onCancel(QObject* sender)
{
    completeNotification(sender, tr("Cancelled"));
}

void medProgressionStack::removeItem(){
    try
    {
        if(!d->itemstoBeRemoved.isEmpty()){
            QObject* object = d->itemstoBeRemoved.dequeue();
            QWidget *widget = d->widgets.value(object);
            delete d->bars.value(object);
            d->bars.remove (object);
            delete d->buttons.value(object);
            d->buttons.remove(object);
            d->layout->removeWidget(widget);
            d->widgets.remove(object);
            delete widget;
            if(d->bars.count() == 0)
                emit(hidden());
        }
    }
    catch (...)
    {
        qWarning() << "exception caught while removing item from progression stack";
    }

}

void medProgressionStack::completeNotification(QObject* sender, QString label )
{
    // check if the sender is in the list of stored senders
    if (d->bars.contains(sender)) {

        QWidget *widget = d->widgets.value(sender);

        if(!d->bars.value(sender)->isHidden())
        {
            //Completed notification
            QLabel *completeLabel = new QLabel(label,widget);
            widget->layout()->removeWidget(d->bars.value(sender));
            d->bars.value(sender)->hide();
            widget->layout()->removeWidget(d->buttons.value(sender));
            d->buttons.value(sender)->hide();
            widget->layout()->addWidget(completeLabel);
            d->itemstoBeRemoved.enqueue(sender);
            QTimer::singleShot(3000, this, SLOT(removeItem()));
        }

    }
}

void medProgressionStack::sendCancelRequest()
{
    QObject* sender = this->sender();
    emit cancelRequest(d->buttonsSender.value(sender));
}

void medProgressionStack::AddJobItem(medJobItem* job, QString label)
{
    connect(job, SIGNAL(progressed(QObject*, int)), this, SLOT(setProgress(QObject*, int)), Qt::QueuedConnection);
    connect(job, SIGNAL(success(QObject*)), this, SLOT(onSuccess(QObject*)), Qt::QueuedConnection);
    connect(job, SIGNAL(failure(QObject*)), this, SLOT(onFailure(QObject*)), Qt::QueuedConnection);
    connect(job, SIGNAL(showError(QObject*,const QString&,unsigned int)), 
        medMessageController::instance(),SLOT(showError (QObject*,const QString&,unsigned int)), Qt::QueuedConnection);
    connect(job, SIGNAL(cancelled(QObject*)), this,SLOT(onCancel(QObject*)), Qt::QueuedConnection);
    connect(this, SIGNAL(cancelRequest(QObject*)),job, SLOT(onCancel(QObject*)), Qt::QueuedConnection);
    
    this->setLabel(job, label);  
}
