#include "core/dal/drug/adme.h"

#include <QObject>

#include "core/definitionmanager.h"
#include "core/core.h"
#include "core/dal/drug/translatablestring.h"


namespace ezechiel {
namespace core {

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


QString formulationAndRoutedescription(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute)
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


void Admin::setFormulationAndRoute(Tucuxi::Core::FormulationAndRoute formulationAndRoute) {
    _formulationAndRoute = formulationAndRoute;
    setDescription(formulationAndRoutedescription(_formulationAndRoute));
}

QML_POINTERLIST_CLASS_IMPL(AdminList, Admin)

} // namespace core
} // namespace ezechiel
