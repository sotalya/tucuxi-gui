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


#include "phone.h"

using namespace Tucuxi::Gui::Admin;

Phone::Phone(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _number(),
    _type(PhoneType::Professional)
{
    setId(-1);
}

Phone::Phone(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository,id, parent),
    _number(),
    _type(PhoneType::Professional)
{
    setId(id);
}

AUTO_PROPERTY_IMPL(Phone, QString, number, Number)
AUTO_PROPERTY_IMPL(Phone, PhoneType, type, Type)

QML_POINTERLIST_CLASS_IMPL(PhoneList, Phone)

//QString Phone::number() const
//{
//    return _number;
//}

//void Phone::setNumber(const QString &number)
//{
//    _number = number;
//    emit numberChanged(number);
//}

//Phone::Type Phone::type() const
//{
//    return _type;
//}

//void Phone::setType(const Phone::Type &type)
//{
//    _type = type;
//    emit typeChanged(type);
//}


void Phone::setTypeFromString(const QString &stype)
{
    if (stype.compare("private") == 0)
        setType(PhoneType::Private);
    else if (stype.compare("prof") == 0)
        setType(PhoneType::Professional);
    else if (stype.compare("cell") == 0)
        setType(PhoneType::Cell);
    else setType(PhoneType::UnknownPhoneType);
}

QString Phone::typeToString() const
{
    switch (_type) {
    case PhoneType::Private          : return "private";
    case PhoneType::Professional     : return "prof";
    case PhoneType::Cell             : return "cell";
    case PhoneType::UnknownPhoneType : return "unknown";
    }
    return "unknown";
}
