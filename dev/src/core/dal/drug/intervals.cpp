#include "core/dal/drug/intervals.h"

namespace ezechiel {
namespace core {

AUTO_PROPERTY_IMPL(ValidInterval, IdentifiableAmount*, quantity, Quantity)

AUTO_PROPERTY_IMPL(ValidIntervals, IdentifiableAmount*, quantity, Quantity)
AUTO_PROPERTY_IMPL(ValidIntervals, bool, any, Any)

QML_POINTERLIST_CLASS_IMPL(IntervalList,ValidInterval)
} // namespace core
} // namespace ezechiel






