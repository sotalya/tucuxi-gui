#include "core/dal/drug/doses.h"

namespace ezechiel {
namespace core {

AUTO_PROPERTY_IMPL(ValidDose, IdentifiableAmount*, quantity, Quantity)
AUTO_PROPERTY_IMPL(ValidDose, Admin*, route, Route)


AUTO_PROPERTY_IMPL(ValidDoses, IdentifiableAmount*, quantity, Quantity)
AUTO_PROPERTY_IMPL(ValidDoses, bool, anyDose, AnyDose)

AUTO_PROPERTY_IMPL(ValidDoses, IdentifiableAmount*, fromDose, FromDose)
AUTO_PROPERTY_IMPL(ValidDoses, IdentifiableAmount*, toDose, ToDose)
AUTO_PROPERTY_IMPL(ValidDoses, IdentifiableAmount*, stepDose, StepDose)

AUTO_PROPERTY_IMPL(ValidDoses, TranslatableString*, comments,Comments)

//POINTERLIST_CLASS_IMPL(dosesList, ValidDose)


QML_POINTERLIST_CLASS_IMPL(DoseList, ValidDose)
} // namespace core
} // namespace ezechiel




