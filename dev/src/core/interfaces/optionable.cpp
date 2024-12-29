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


#include "optionable.h"
//#include "core_errors.h"
//#include "coremessagehandler.h"
#include "core/core.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



//Destructor
Optionable::~Optionable()
{

}

//Get the list of options
QList<Option> Optionable::getOptions() const
{
   return _options.values();
}

//Get a specific option
Option Optionable::option(const QString &oid) const
{
   return _options.value(oid);
}

//Set the option value
bool Optionable::setOptionValue (const QString &oid, const QVariant &value)
{
   //Check if the option exists
   if (!_options.contains(oid))
      return false;

   //Set the option value
   _options[oid].value = value;

   return true;
}

//Set the option DB ID
bool Optionable::setOptionDbId(const QString &oid, const ident id)
{
    //Check if the option exists
    if (!_options.contains(oid))
       return false;

    //Set the option database ID
    _options[oid].sqlId = id;

    return true;
}

//Copy the options
void Optionable::copyOptions(Optionable *other)
{
   _options.clear();
   foreach(Option o, other->_options)
      _options.insert(o.oid,o);
}

//Check if an option exits
bool Optionable::optionExists(const QString &oid) const
{
   return _options.contains(oid);
}

//Add an option to the map
void Optionable::addOption(const QString &oid, const QString &name, const QString &desc, int type, const QVariant &value, const QString &group)
{
   _options.insert(oid,Option(Descriptor(oid,name,desc),type, value, oid, group, false));
}

//Remove an option from the map
void Optionable::removeOption(const QString &oid)
{
   _options.remove(oid);
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

