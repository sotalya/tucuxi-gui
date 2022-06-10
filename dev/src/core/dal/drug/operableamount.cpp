//@@license@@

#include "core/dal/drug/operableamount.h"
#include "core/dal/drug/operation.h"


namespace Tucuxi {
namespace GuiCore {

AUTO_PROPERTY_IMPL(IdentifiableAmount, QString, amountId, AmountId)

AUTO_PROPERTY_IMPL(OperableAmount, OperationList*, operations, Operations)
AUTO_PROPERTY_IMPL(OperableAmount, bool, fixed, Fixed)

AUTO_PROPERTY_IMPL(OperableValue, QString, valueId, ValueId)
AUTO_PROPERTY_IMPL(OperableValue, QVariant, value, Value)
AUTO_PROPERTY_IMPL(OperableValue, OperationList*, operations, Operations)

POINTERLIST_CLASS_IMPL(OperableValueList, OperableValue)

} // namespace core
} // namespace Tucuxi
