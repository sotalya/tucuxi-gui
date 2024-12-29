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


#ifndef EMAIL_H
#define EMAIL_H

#include "core/dal/entity.h"
#include "core/utils/autoproperty.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

enum class Type {
    Private,
    Professional
};

class Email : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(QString, email, Email)
    AUTO_PROPERTY_DECL(Type, type, Type)

public:


    Q_INVOKABLE Email(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);
    Q_INVOKABLE Email(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);
};

}
}
}

Q_DECLARE_OPAQUE_POINTER(Tucuxi::Gui::Admin::Email*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Email*>)
#endif // EMAIL_H
