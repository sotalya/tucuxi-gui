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


#include "routetranslator.h"


namespace Tucuxi {
namespace Gui {
namespace Rest {


RouteTranslator::RouteTranslator()
{
}


DummyRouteTranslator::DummyRouteTranslator()
{
}

Tucuxi::Gui::Core::Admin::Route DummyRouteTranslator::restToInternalRoute(QString restRoute)
{
    return Tucuxi::Gui::Core::Admin::UNVALID;
}

ChuvRouteTranslator::ChuvRouteTranslator()
{
    map.insert("", Tucuxi::Gui::Core::Admin::DEFAULT);
    // Internal values
    map.insert("BOLUS", Tucuxi::Gui::Core::Admin::BOLUS);
    map.insert("INFUSION", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("EXTRA", Tucuxi::Gui::Core::Admin::EXTRA);

    // External values
    map.insert("i.v", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("i.v.", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("injection iv", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("perfusion i.v. continue", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("perfusion iv continue", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("orale", Tucuxi::Gui::Core::Admin::EXTRA);
    map.insert("per os", Tucuxi::Gui::Core::Admin::EXTRA);
    map.insert("p.o", Tucuxi::Gui::Core::Admin::EXTRA);
    map.insert("intravenousDrip", Tucuxi::Gui::Core::Admin::INFUSION);
}

Tucuxi::Gui::Core::Admin::Route ChuvRouteTranslator::restToInternalRoute(QString restRoute)
{
    return map.value(restRoute,Tucuxi::Gui::Core::Admin::UNVALID);
}

}
}
}
