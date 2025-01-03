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


#include "core/dal/drug/operation.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


AUTO_PROPERTY_IMPL(Operation, OperationType, type, Type)
AUTO_PROPERTY_IMPL(Operation, QString, formula, Formula)

AUTO_PROPERTY_IMPL(Operation, TranslatableString*, comments, Comments)

Operation::Operation(AbstractRepository *repository, QObject *parent) :
    Entity(repository, parent),
    _type(NOOP),
    _comments(nullptr)
{

}



QML_POINTERLIST_CLASS_IMPL(OperationList, Operation)


} // namespace Core
} // namespace Gui
} // namespace Tucuxi





