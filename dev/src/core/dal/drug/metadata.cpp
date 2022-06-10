//@@license@@

#include "core/dal/drug/metadata.h"
#include "core/dal/drug/editors.h"
#include "references.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



AUTO_PROPERTY_IMPL(MetaData,EditorList*,editors,Editors)
AUTO_PROPERTY_IMPL(MetaData,ReferenceList*,references,References)



} // namespace Core
} // namespace Gui
} // namespace Tucuxi



