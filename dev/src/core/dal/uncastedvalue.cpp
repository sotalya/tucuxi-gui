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


#include "core/dal/uncastedvalue.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



Q_INVOKABLE UncastedValue::UncastedValue(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent)
    : Entity(repository, parent), _field(""), _text(""), _comment(""), _status(UncastedStatus::Uncasted), _validated(false)
{

}

bool UncastedValue::isValid()
{
    return true;
}

AUTO_PROPERTY_IMPL(UncastedValue, QString, field, Field)
AUTO_PROPERTY_IMPL(UncastedValue, QString, text, Text)
AUTO_PROPERTY_IMPL(UncastedValue, QString, comment, Comment)
AUTO_PROPERTY_IMPL(UncastedValue, UncastedStatus, status, Status)
AUTO_PROPERTY_IMPL(UncastedValue, bool, validated, Validated)


QML_POINTERLIST_CLASS_IMPL(UncastedValueList, UncastedValue)


} // namespace Core
} // namespace Gui
} // namespace Tucuxi
