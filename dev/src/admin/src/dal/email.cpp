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


#include "email.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

AUTO_PROPERTY_IMPL(Email, QString, email, Email)
AUTO_PROPERTY_IMPL(Email, Type, type, Type)

Email::Email(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _email(),
    _type(Type::Professional)
{
    setId(-1);
}

Email::Email(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository,id, parent),
    _email(),
    _type(Type::Professional)
{
    setId(-1);
}

//QString Email::email() const
//{
//    return _email;
//}

//void Email::setEmail(const QString &email)
//{
//    _email = email;
//    emit emailChanged(email);
//}

//Email::Type Email::type() const
//{
//    return _type;
//}

//void Email::setType(const Email::Type &type)
//{
//    _type = type;
//    emit typeChanged(type);
//}

}
}
}
