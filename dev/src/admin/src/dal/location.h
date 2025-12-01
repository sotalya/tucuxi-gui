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


#ifndef LOCATION_H
#define LOCATION_H

#include "core/dal/entity.h"
#include "core/utils/autoproperty.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {


class Location : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT
    ADMIN_ENTITY_UTILS(Location)
    AUTO_PROPERTY(QString, address)
    AUTO_PROPERTY(QString, postcode)
    AUTO_PROPERTY(QString, city)
    AUTO_PROPERTY(QString, state)
    AUTO_PROPERTY(QString, country)

public:
    Q_INVOKABLE Location(Tucuxi::Gui::Core::AbstractRepository *repository = nullptr, QObject *parent = nullptr);
    Q_INVOKABLE Location(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

    [[nodiscard]] QVariant toQVariant() const;
};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::Location*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Location*>)

#endif // LOCATION_H
