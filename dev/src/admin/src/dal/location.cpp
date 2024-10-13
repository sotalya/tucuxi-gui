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


#include "location.h"

using namespace Tucuxi::Gui::Admin;

Location::Location(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _address(),
    _postcode(),
    _city(),
    _state(),
    _country()
{
}

Location::Location(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository, id, parent),
    _address(),
    _postcode(),
    _city(),
    _state(),
    _country()
{
}

QVariant Location::toQVariant() const{
    QMap<QString, QVariant> locationMap;

    locationMap["city"] = _city;
    locationMap["postcode"] = _postcode;
    locationMap["state"] = _state;
    locationMap["country"] = _country;

    return locationMap;
}

//QString Location::address() const
//{
//    return _address;
//}

//void Location::setAddress(const QString &address)
//{
//    _address = address;
//    emit addressChanged(address);
//}

//QString Location::postcode() const
//{
//    return _postcode;
//}

//void Location::setPostcode(const QString &postcode)
//{
//    _postcode = postcode;
//    emit postcodeChanged(postcode);
//}

//QString Location::city() const
//{
//    return _city;
//}

//void Location::setCity(const QString &city)
//{
//    _city = city;
//    emit cityChanged(city);
//}

//QString Location::state() const
//{
//    return _state;
//}

//void Location::setState(const QString &state)
//{
//    _state = state;
//    emit stateChanged(state);
//}

//QString Location::country() const
//{
//    return _country;
//}

//void Location::setCountry(const QString &country)
//{
//    _country = country;
//    emit countryChanged(country);
//}
