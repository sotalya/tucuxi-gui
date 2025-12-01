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


#ifndef PRACTICIAN_H
#define PRACTICIAN_H

#include "core/dal/entity.h"
#include "person.h"
#include "institute.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

class Practician : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT
    ADMIN_ENTITY_UTILS(Practician)
    AUTO_PROPERTY(QString, externalId)
    AUTO_PROPERTY(QString, title)
    AUTO_PROPERTY(QString, role)
    AUTO_PROPERTY(Person*, person)
    AUTO_PROPERTY(int, person_id)
    AUTO_PROPERTY(Institute*, institute)
    AUTO_PROPERTY(int, institute_id)

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

//    void copyTo(Practician *other);

public:
    Q_INVOKABLE Practician(Tucuxi::Gui::Core::AbstractRepository *repository = nullptr, QObject *parent = nullptr);
    Q_INVOKABLE Practician(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::Practician*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Practician*>)

namespace Tucuxi {
namespace Gui {
namespace Admin {

class PracticianSet : public QList<Practician*>
{
public :
    PracticianSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent = nullptr) {}
    PracticianSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent, const PracticianSet* &other);
};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::PracticianSet*)

typedef Tucuxi::Gui::Admin::Practician* SharedPractician;
typedef Tucuxi::Gui::Admin::PracticianSet* SharedPracticianSet;

#endif // PRACTICIAN_H
