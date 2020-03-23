/**
 * @file qbitarrayutility.h
 * @author Yvan Da Silva <yvan.m.silva AT gmail.com>
 * @author IICT Institute for Information and Communication<www.iict.ch>
 * @author HEIG-VD (Haute école d'inénierie et de gestion) <www.heig-vd.ch>
 * @date 06.2012
 * @section LICENSE
 *
 * This application is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @class QBitArrayUtility
 * @brief Tools to manage and add functionalities to QBitArray objects.
 *
 */
#ifndef QBITARRAYUTILITY_H
#define QBITARRAYUTILITY_H

#include "QBitArray"
#include "QString"

class QBitArrayUtility
{
public:
    QBitArrayUtility();
    static uint bitArray2Uint(QBitArray *array);
    static QString bitArray2String(QBitArray *array);
};

#endif // QBITARRAYUTILITY_H
