/*==============================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include "medPythonInit.h"

#include <QApplication>

#include "medPythonCore.h"

namespace med::python
{

bool setup()
{
    bool success = core::setup();

    if (success)
    {
        QApplication::connect(qApp, &QApplication::aboutToQuit, &teardown);
    }
    else
    {
        core::teardown();
    }

    return success;
}

bool teardown()
{
    return core::teardown();
}

} // namespace med::python
