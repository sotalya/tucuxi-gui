//@@license@@

#include "references.h"

namespace Tucuxi {
namespace GuiCore {

AUTO_PROPERTY_IMPL(Reference,QString, text, Text)
AUTO_PROPERTY_IMPL(Reference,QString, type, Type)

POINTERLIST_CLASS_IMPL(ReferenceList, Reference)

} // namespace core
} // namespace Tucuxi

