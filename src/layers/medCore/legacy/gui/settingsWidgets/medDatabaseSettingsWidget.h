#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medSettingsWidget.h>
#include <QWidget>

class medDatabaseSettingsWidgetPrivate;

class MEDCORE_EXPORT medDatabaseSettingsWidget : public medSettingsWidget
{
    Q_OBJECT
    MED_SETTINGS_INTERFACE("Database","Database Settings")
public:
    medDatabaseSettingsWidget(QWidget *parent = nullptr);

public slots:
    void read();

private slots:
    void selectDbDirectory();

protected:
    bool validate();

    void write();
private:
    medDatabaseSettingsWidgetPrivate *d;

};
