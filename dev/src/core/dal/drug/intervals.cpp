//@@license@@

#include "core/dal/drug/intervals.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


AUTO_PROPERTY_IMPL(ValidInterval, IdentifiableAmount*, quantity, Quantity)

AUTO_PROPERTY_IMPL(ValidIntervals, IdentifiableAmount*, quantity, Quantity)
AUTO_PROPERTY_IMPL(ValidIntervals, bool, any, Any)
AUTO_PROPERTY_IMPL(ValidIntervals, QList<double>, intervalsList, IntervalsList)

QML_POINTERLIST_CLASS_IMPL(IntervalList,ValidInterval)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi






