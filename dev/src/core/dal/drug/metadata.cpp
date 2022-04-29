//@@license@@

#include "core/dal/drug/metadata.h"
#include "core/dal/drug/editors.h"
#include "references.h"

namespace ezechiel {
namespace core {


AUTO_PROPERTY_IMPL(MetaData,EditorList*,editors,Editors)
AUTO_PROPERTY_IMPL(MetaData,ReferenceList*,references,References)



} // namespace core
} // namespace ezechiel



