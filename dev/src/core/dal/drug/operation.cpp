//@@license@@

#include "core/dal/drug/operation.h"

namespace ezechiel {
namespace GuiCore {

AUTO_PROPERTY_IMPL(Operation, OperationType, type, Type)
AUTO_PROPERTY_IMPL(Operation, QString, formula, Formula)

AUTO_PROPERTY_IMPL(Operation, TranslatableString*, comments, Comments)

Operation::Operation(AbstractRepository *repository, QObject *parent) :
    Entity(repository, parent),
    _type(NOOP),
    _comments(nullptr)
{

}



QML_POINTERLIST_CLASS_IMPL(OperationList, Operation)


} // namespace core
} // namespace ezechiel





