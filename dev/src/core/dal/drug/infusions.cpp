//@@license@@

#include "core/dal/drug/infusions.h"

namespace ezechiel {
namespace core {

AUTO_PROPERTY_IMPL(ValidInfusion, IdentifiableAmount*, quantity, Quantity)

AUTO_PROPERTY_IMPL(ValidInfusions, IdentifiableAmount*, quantity, Quantity)
AUTO_PROPERTY_IMPL(ValidInfusions, bool, any, Any)
AUTO_PROPERTY_IMPL(ValidInfusions, InfusionsList*, list, List)

AUTO_PROPERTY_IMPL(ValidInfusions, QList<double>, infusionsList, InfusionsList)

POINTERLIST_CLASS_IMPL(InfusionsList, ValidInfusion)
QML_POINTERLIST_CLASS_IMPL(InfusionList, ValidInfusion)
} // namespace core
} // namespace ezechiel


