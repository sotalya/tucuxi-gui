//@@license@@

#ifndef OPERATION_H
#define OPERATION_H

#include "core/dal/entity.h"
#include "core/utils/ezutils.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class TranslatableString;

enum OperationType {
    //! Empty operation
    NOOP,
    //! The operation is hardcoded in the software. So use formulaIdList
    HARDCODED,
    //! The operation is coded in formula field
    IMPORTED
};

class Operation : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Operation)

    AUTO_PROPERTY_DECL(OperationType, type, Type)
    // If hardcoded, then the formula represents the ID of the operation to be used
    // If imported, then the formula is the QScript representing the operation
    AUTO_PROPERTY_DECL(QString, formula, Formula)

    AUTO_PROPERTY_DECL(TranslatableString*, comments, Comments)

protected:

    Q_INVOKABLE explicit Operation(AbstractRepository *repository, QObject *parent = nullptr);

public:

};


QML_POINTERLIST_CLASS_DECL(OperationList, Operation)


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::OperationType)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::OperationType*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Operation*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Operation*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::OperationList*)

#endif // OPERATION_H
