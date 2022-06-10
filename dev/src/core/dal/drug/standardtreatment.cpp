//@@license@@

#include "standardtreatment.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


AUTO_PROPERTY_IMPL(StandardTreatment, bool, isFixedDuration, IsFixedDuration)
AUTO_PROPERTY_IMPL(StandardTreatment, Duration, duration, Duration)



StandardTreatment::StandardTreatment(AbstractRepository *repository, QObject *parent)
  : Entity(repository, parent) , _isFixedDuration(false), _duration(0) {}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
