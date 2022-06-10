//@@license@@

#ifndef CONCENTRATIONS_H
#define CONCENTRATIONS_H

#include "core/utils/units.h"
#include "core/dal/entity.h"
#include "core/dal/amount.h"

namespace Tucuxi {
namespace GuiCore {


class Concentrations : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Concentrations)

    AMOUNT_PROPERTY_DECL(Amount, quantity, Quantity)

protected:

    Q_INVOKABLE explicit Concentrations(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) { _quantity.setValue(-1.0);}
    Q_INVOKABLE Concentrations(AbstractRepository *repository, const Unit &unit, double factor) : Entity(repository), _quantity(Amount(factor, unit)) {}

};


} // namespace core
} // namespace Tucuxi

Q_DECLARE_OPAQUE_POINTER(Tucuxi::GuiCore::Concentrations*)
Q_DECLARE_METATYPE(QList<Tucuxi::GuiCore::Concentrations*>)

#endif // CONCENTRATIONS_H
