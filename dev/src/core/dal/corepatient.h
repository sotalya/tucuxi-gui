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


#ifndef COREPATIENT_H
#define COREPATIENT_H

#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class CorePatient : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(CorePatient)

protected:
    Q_INVOKABLE CorePatient(AbstractRepository *repository = nullptr, QObject *parent = nullptr) : Entity(repository, parent) {}
    Q_INVOKABLE CorePatient(AbstractRepository *repository, const int &id, QObject *parent = nullptr) : Entity(repository, parent) {setId(id);}
};

class CorePatientSet : public QList<CorePatient*>
{
public :
    CorePatientSet(AbstractRepository *repository = nullptr, QObject* parent = 0) {}
    CorePatientSet(AbstractRepository *repository, QObject* parent, const CorePatientSet* &other);
};

QML_POINTERLIST_CLASS_DECL(CorePatientList, CorePatient)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::CorePatient*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::CorePatient*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::CorePatientList*)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::CorePatientSet*)


#endif // COREPATIENT_H
