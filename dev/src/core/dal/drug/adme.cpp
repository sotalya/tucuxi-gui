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

QML_POINTERLIST_CLASS_IMPL(AdminList, Admin)

} // namespace core
} // namespace ezechiel
