/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef FILE_CONNECT_H
#define FILE_CONNECT_H

#include <QMessageBox>

#define COUCOU(c) c

#define CHECK_INVOKEMETHOD(invocation) \
    if (!(invocation)) \
    { \
        QString mess;\
        mess =QString("InvokeMethod error in file %1, line %2.").arg(__FILE__).arg(__LINE__);\
        QMessageBox::warning(nullptr, "Tucuxi",mess);\
    }

#define CONNECT(a,b,c,d) \
   if (!connect(a,b,c,d))\
           {\
               QString mess;\
               mess =QString("Signal connection error in file %1, line %2. %3: %4::%5").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__).arg(#a).arg(#b);\
               QMessageBox::warning(nullptr, "Tucuxi",\
                   mess);\
           }


#endif // FILE_CONNECT_H
