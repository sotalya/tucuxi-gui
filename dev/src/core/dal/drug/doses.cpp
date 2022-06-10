//@@license@@

#include "core/dal/drug/doses.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


AUTO_PROPERTY_IMPL(ValidDose, IdentifiableAmount*, quantity, Quantity)
AUTO_PROPERTY_IMPL(ValidDose, Admin*, route, Route)


AUTO_PROPERTY_IMPL(ValidDoses, IdentifiableAmount*, quantity, Quantity)
AUTO_PROPERTY_IMPL(ValidDoses, bool, anyDose, AnyDose)

AUTO_PROPERTY_IMPL(ValidDoses, IdentifiableAmount*, fromDose, FromDose)
AUTO_PROPERTY_IMPL(ValidDoses, IdentifiableAmount*, toDose, ToDose)
AUTO_PROPERTY_IMPL(ValidDoses, IdentifiableAmount*, stepDose, StepDose)

AUTO_PROPERTY_IMPL(ValidDoses, TranslatableString*, comments,Comments)

AUTO_PROPERTY_IMPL(ValidDoses, QList<double>, dosesList, DosesList)


QML_POINTERLIST_CLASS_IMPL(DoseList, ValidDose)

} // namespace Core
} // namespace Gui
} // namespace Tucuxi





