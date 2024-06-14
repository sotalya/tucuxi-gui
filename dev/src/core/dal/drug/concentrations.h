/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef CONCENTRATIONS_H
#define CONCENTRATIONS_H

#include "core/utils/units.h"
#include "core/dal/entity.h"
#include "core/dal/amount.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class Concentrations : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Concentrations)

    AMOUNT_PROPERTY_DECL(Amount, quantity, Quantity)

protected:

    Q_INVOKABLE explicit Concentrations(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) { _quantity.setValue(-1.0);}
    Q_INVOKABLE Concentrations(AbstractRepository *repository, const Unit &unit, double factor) : Entity(repository), _quantity(Amount(factor, unit)) {}

};


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_OPAQUE_POINTER(Tucuxi::Gui::Core::Concentrations*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Concentrations*>)

#endif // CONCENTRATIONS_H
