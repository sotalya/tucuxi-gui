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


#ifndef PHONE_H
#define PHONE_H

#include "core/dal/entity.h"
#include "core/utils/autoproperty.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

enum class PhoneType {
    Private,
    Professional,
    Cell,
    UnknownPhoneType
};

class Phone : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(Phone)

    AUTO_PROPERTY_DECL(QString, number, Number)
    AUTO_PROPERTY_DECL(PhoneType, type, Type)

public:

    Q_INVOKABLE Phone(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);
    Q_INVOKABLE Phone(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    void setTypeFromString(const QString &type);
    QString typeToString() const;

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

};

QML_POINTERLIST_CLASS_DECL(PhoneList, Phone)

}
}
}

typedef Tucuxi::Gui::Admin::Phone* SharedPhone;
Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::PhoneType)
Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::Phone*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Phone*>)
#endif // PHONE_H
