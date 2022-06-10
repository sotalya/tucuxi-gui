//@@license@@

#ifndef RESULTS_H
#define RESULTS_H

#include "core/utils/units.h"
#include "core/dal/entity.h"

namespace Tucuxi {
namespace GuiCore {


class Results : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Results)

    AUTO_PROPERTY_DECL(Unit, unit, Unit)
    AUTO_PROPERTY_DECL(double, factor, Factor)

protected:

    Q_INVOKABLE explicit Results(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent), _factor(-1.0) {}
    Q_INVOKABLE Results(AbstractRepository *repository, QObject *parent, const Unit &unit, double factor) : Entity(repository, parent), _unit(unit), _factor(factor) {}

};

typedef Results* SharedResults;

} // namespace core
} // namespace Tucuxi

#endif // RESULTS_H
