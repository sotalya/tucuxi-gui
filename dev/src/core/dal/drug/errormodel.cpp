#include "core/dal/drug/errormodel.h"
#include "core/dal/drug/translatablestring.h"

namespace ezechiel {
namespace core {

//! The parameter's value standard variability
AUTO_PROPERTY_IMPL(ErrorModel, double, additive, Additive)
//! The parameter's value proportional variability
AUTO_PROPERTY_IMPL(ErrorModel, double, proportional, Proportional)

AUTO_PROPERTY_IMPL(ErrorModel, TranslatableString*, comments, Comments)

} // namespace core
} // namespace ezechiel






