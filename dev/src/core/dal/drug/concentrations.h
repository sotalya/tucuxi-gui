//@@license@@

#ifndef CONCENTRATIONS_H
#define CONCENTRATIONS_H

#include "core/utils/units.h"
#include "core/dal/entity.h"
#include "core/dal/amount.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class Concentrations : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Concentrations)

    AMOUNT_PROPERTY_DECL(Amount, quantity, Quantity)

protected:

    Q_INVOKABLE explicit Concentrations(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) { _quantity.setValue(-1.0);}
    Q_INVOKABLE Concentrations(AbstractRepository *repository, const Unit &unit, double factor) : Entity(repository), _quantity(Amount(factor, unit)) {}

};


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_OPAQUE_POINTER(Tucuxi::Gui::Core::Concentrations*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Concentrations*>)

#endif // CONCENTRATIONS_H
