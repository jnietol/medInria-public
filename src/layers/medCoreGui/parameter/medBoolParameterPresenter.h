#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractParameterPresenter.h>
#include <medBoolParameter.h>

class QWidget;
class QCheckBox;
class QRadioButton;
class QPushButton;
class QAbstractButton;
class medBoolParameterPresenterPrivate;


class MEDCOREGUI_EXPORT medBoolParameterPresenter : public medAbstractParameterPresenter
{
    Q_OBJECT

public:
    medBoolParameterPresenter(medBoolParameter *parent);
    medBoolParameterPresenter(const QString & newParameterId);
    virtual ~medBoolParameterPresenter();

    virtual medBoolParameter *parameter() const;

    void setIcon(QIcon icon);
    void setIconSize(QSize const & size);

    virtual QWidget * buildWidget();
    QPushButton *     buildPushButton();
    QRadioButton *    buildRadioButton();
    QCheckBox *       buildCheckBox();


private:
    void _connectButton(QAbstractButton *button);

private:
    const QScopedPointer<medBoolParameterPresenterPrivate> d;
};
