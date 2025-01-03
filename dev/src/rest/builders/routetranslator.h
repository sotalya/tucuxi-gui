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


#ifndef ROUTETRANSLATOR_H
#define ROUTETRANSLATOR_H



#include <QString>
#include <QMap>

#include "core/dal/drug/adme.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class RouteTranslator
{
public:
    RouteTranslator();
    virtual ~RouteTranslator() = default;

    virtual Tucuxi::Gui::Core::Admin::Route restToInternalRoute(QString restRoute) = 0;
};

class DummyRouteTranslator : public RouteTranslator
{
public:
    DummyRouteTranslator();

    virtual Tucuxi::Gui::Core::Admin::Route restToInternalRoute(QString restRoute) ;
};

class ChuvRouteTranslator : public RouteTranslator
{
public:
    ChuvRouteTranslator();

    virtual Tucuxi::Gui::Core::Admin::Route restToInternalRoute(QString restRoute) ;

private:
    QMap<QString,Tucuxi::Gui::Core::Admin::Route> map;
};

}
}
}

#endif // ROUTETRANSLATOR_H
