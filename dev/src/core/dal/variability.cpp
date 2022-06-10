//@@license@@

#include "variability.h"

#include "core/dal/drug/translatablestring.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


AUTO_PROPERTY_IMPL(Variability, QString, distributionId, DistributionId)
AUTO_PROPERTY_IMPL(Variability, OperableDoubleList*, stdDeviations, StdDeviations)
AUTO_PROPERTY_IMPL(Variability, TranslatableString*, comments,Comments)

AUTO_PROPERTY_IMPL(VariableValue, QVariant, value, Value)
AUTO_PROPERTY_IMPL(VariableValue, Variability*, variability, Variability)

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
