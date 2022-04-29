//@@license@@

#include "core/dal/drug/halflife.h"
#include "core/dal/drug/translatablestring.h"

namespace ezechiel {
namespace core {

AUTO_PROPERTY_IMPL(Halflife, Unit, unit, Unit)
AUTO_PROPERTY_IMPL(Halflife, double, value, Value)
AUTO_PROPERTY_IMPL(Halflife, int, multiplier, Multiplier)
//AUTO_PROPERTY_IMPL(Halflife, CommentSet, commentSet, CommentSet)

AUTO_PROPERTY_IMPL(Halflife, TranslatableString*, comments, Comments)

} // namespace core
} // namespace ezechiel



