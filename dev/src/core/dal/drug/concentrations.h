#ifndef CONCENTRATIONS_H
#define CONCENTRATIONS_H

#include "core/utils/units.h"
#include "core/dal/entity.h"
#include "core/dal/amount.h"

namespace ezechiel {
namespace core {


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
} // namespace ezechiel

Q_DECLARE_OPAQUE_POINTER(ezechiel::core::Concentrations*)
Q_DECLARE_METATYPE(QList<ezechiel::core::Concentrations*>)

#endif // CONCENTRATIONS_H
