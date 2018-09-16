/****************************************************************************************
 ** Server is an application to manage several clients inside a thread.
 ** Copyright (C) 2013  Francesc Martinez <es.linkedin.com/in/cescmm/en>
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2.1 of the License, or (at your option) any later version.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/
#ifndef VERION_H
#define VERION_H
 
#include "QLogger.h"

using namespace QLogger;

#define VERSION_VS "0.1"

#ifdef Q_CC_MSVC
    #ifndef and
        #define and &&
    #endif

    #ifndef or
        #define or ||
    #endif
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <qmath.h>
#else
#include <QtMath>
#endif

#endif // VERION_H
