//@@license@@

#ifndef ERRORMODEL_H
#define ERRORMODEL_H

#include "core/dal/entity.h"
#include "core/utils/ezutils.h"

namespace Tucuxi {
namespace GuiCore {

class TranslatableString;

class ErrorModel : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(ErrorModel)

    //! The parameter's value standard variability
    AUTO_PROPERTY_DECL(double, additive, Additive)
    //! The parameter's value proportional variability
    AUTO_PROPERTY_DECL(double, proportional, Proportional)

    AUTO_PROPERTY_DECL(TranslatableString*, comments, Comments)

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

protected:

    Q_INVOKABLE ErrorModel(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent), _additive(0.0), _proportional(0.0), _comments(0) {}
    Q_INVOKABLE ErrorModel(AbstractRepository *repository, QObject * parent, double additive, double proportional)
        : Entity(repository, parent), _additive(additive), _proportional(proportional) {}
};

} // namespace core
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::GuiCore::ErrorModel*)
Q_DECLARE_METATYPE(QList<Tucuxi::GuiCore::ErrorModel*>)

#endif // ERRORMODEL_H
