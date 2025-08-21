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


#ifndef PATIENT_H
#define PATIENT_H

#include "core/dal/corepatient.h"
#include "person.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

class Patient : public Tucuxi::Gui::Core::CorePatient
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(Patient)

    AUTO_PROPERTY(QString, externalId)
    AUTO_PROPERTY(QString, stayNumber)

    // Do not know what is this statOk about
    AUTO_PROPERTY(bool, statOk)
    AUTO_PROPERTY(Person*, person)
    AUTO_PROPERTY(int, person_id)

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

public:
    Q_INVOKABLE Patient(Tucuxi::Gui::Core::AbstractRepository *repository = nullptr, QObject *parent = nullptr);
    Q_INVOKABLE Patient(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::Patient*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Patient*>)

//QML_POINTERLIST_CLASS_DECL(PatientList, Patient)

typedef Tucuxi::Gui::Core::CorePatient* SharedPatient;

#endif // PATIENT_H
