/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractArithmeticOperationProcess.h>

#include <dtkCore>

#include <medInriaExport.h>

class MEDINRIA_EXPORT medAbstractMultiplyImageProcess: public medAbstractArithmeticOperationProcess
{
    Q_OBJECT
public:
    medAbstractMultiplyImageProcess(QObject *parent): medAbstractArithmeticOperationProcess(parent) {}
};

DTK_DECLARE_OBJECT        (medAbstractMultiplyImageProcess*)
DTK_DECLARE_PLUGIN        (medAbstractMultiplyImageProcess, MEDINRIA_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(medAbstractMultiplyImageProcess, MEDINRIA_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(medAbstractMultiplyImageProcess, MEDINRIA_EXPORT)