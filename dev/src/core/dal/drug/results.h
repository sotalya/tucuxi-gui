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


#ifndef RESULTS_H
#define RESULTS_H

#include "core/utils/units.h"
#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class Results : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Results)

    AUTO_PROPERTY_DECL(Unit, unit, Unit)
    AUTO_PROPERTY_DECL(double, factor, Factor)

protected:

    Q_INVOKABLE explicit Results(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent), _factor(-1.0) {}
    Q_INVOKABLE Results(AbstractRepository *repository, QObject *parent, const Unit &unit, double factor) : Entity(repository, parent), _unit(unit), _factor(factor) {}

};

typedef Results* SharedResults;

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // RESULTS_H
