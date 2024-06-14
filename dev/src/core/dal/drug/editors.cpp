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


#include "core/dal/drug/editors.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


AUTO_PROPERTY_IMPL(Editor,QString,name,Name)
AUTO_PROPERTY_IMPL(Editor,QString, institution,Institution)
AUTO_PROPERTY_IMPL(Editor,QString, email, Email)
AUTO_PROPERTY_IMPL(Editor,QDateTime,date,Date)

AUTO_PROPERTY_IMPL(Editor,TranslatableString*,comments,Comments)

QML_POINTERLIST_CLASS_IMPL(EditorList,Editor)

/*
// Because an EditorSet is not an Entity but a QList we need to delete manually the objects
// YTA : I think we could use EditorList instead of EditorSet
EditorSet::~EditorSet()
{
    while (!this->isEmpty()) {
        Editor *e = this->at(this->size()-1);
        this->pop_back();
        delete e;
    }
}
*/

} // namespace Core
} // namespace Gui
} // namespace Tucuxi



