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


#include "core/dal/drug/adme.h"

#include <QObject>

#include "core/definitionmanager.h"
#include "core/core.h"
#include "core/dal/drug/translatablestring.h"


namespace Tucuxi {
namespace Gui {
namespace Core {


AUTO_PROPERTY_IMPL(ADME, Admin*, defaultIntake, DefaultIntake)
AUTO_PROPERTY_IMPL(ADME, QString, distribution, Distribution)
AUTO_PROPERTY_IMPL(ADME, QString, elimination, Elimination)
AUTO_PROPERTY_IMPL(ADME, AdminList*, intakes, Intakes)

AUTO_PROPERTY_IMPL(ADME, TranslatableString*, comments, Comments)

//QString ADME::toString(Route route)
//{
//    switch(route) {
//    case BOLUS    : return tr("Bolus");
//    case INFUSION : return tr("Infusion");
//    case EXTRA    : return tr("Extravascular");
//    default       : return tr("Unknown route of administration");
//    }
//}

bool ADME::isValid()
{
//    if (CORE->definitionManager()->validateIntake(_defaultIntake) && !_distribution.isEmpty() && !_elimination.isEmpty())
//        return false;
//    for(int i=0;i<_intakes.size();i++)
//        if (CORE->definitionManager()->validateIntake(_intakes.at(i)) && !_distribution.isEmpty() && !_elimination.isEmpty())
//            return false;
    return true;
}


QString formulationAndRoutedescription(const Tucuxi::Core::DMFormulationAndRoute& _formulationAndRoute)
{
    QString result;
    switch(_formulationAndRoute.getAdministrationRoute()) {
    case Tucuxi::Core::AdministrationRoute::Intramuscular : result = "Intramuscular"; break;
    case Tucuxi::Core::AdministrationRoute::IntravenousBolus : result = "Intravenous bolus"; break;
    case Tucuxi::Core::AdministrationRoute::IntravenousDrip : result = "Intravenous drip"; break;
    case Tucuxi::Core::AdministrationRoute::Nasal : result = "Nasal"; break;
    case Tucuxi::Core::AdministrationRoute::Oral : result = "Oral"; break;
    case Tucuxi::Core::AdministrationRoute::Rectal : result = "Rectal"; break;
    case Tucuxi::Core::AdministrationRoute::Subcutaneous : result = "Subcutaneous"; break;
    case Tucuxi::Core::AdministrationRoute::Sublingual : result = "Sublingual"; break;
    case Tucuxi::Core::AdministrationRoute::Transdermal : result = "Transdermal"; break;
    default: result = "Undefined"; break;
    }
    return result;
}


void Admin::setFormulationAndRoute(Tucuxi::Core::DMFormulationAndRoute formulationAndRoute) {
    _formulationAndRoute = formulationAndRoute;

    // Here we update the route. Important for a correct display by the QML DialogTab
    switch (formulationAndRoute.getAbsorptionModel()) {
    case Tucuxi::Core::AbsorptionModel::Extravascular : {
        setRoute(Route::EXTRA);
    } break;
    case Tucuxi::Core::AbsorptionModel::ExtravascularLag : {
        setRoute(Route::EXTRALAG);
    } break;
    case Tucuxi::Core::AbsorptionModel::Infusion : {
        setRoute(Route::INFUSION);
    } break;
    case Tucuxi::Core::AbsorptionModel::Intravascular : {
        setRoute(Route::BOLUS);
    } break;
    case Tucuxi::Core::AbsorptionModel::Undefined : {
        setRoute(Route::UNVALID);
    }
    }

    setDescription(formulationAndRoutedescription(_formulationAndRoute));
}

QML_POINTERLIST_CLASS_IMPL(AdminList, Admin)

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
