//@@license@@

#include "core/dal/drug/errormodel.h"
#include "core/dal/drug/translatablestring.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


//! The parameter's value standard variability
AUTO_PROPERTY_IMPL(ErrorModel, double, additive, Additive)
//! The parameter's value proportional variability
AUTO_PROPERTY_IMPL(ErrorModel, double, proportional, Proportional)

AUTO_PROPERTY_IMPL(ErrorModel, TranslatableString*, comments, Comments)

} // namespace Core
} // namespace Gui
} // namespace Tucuxi






