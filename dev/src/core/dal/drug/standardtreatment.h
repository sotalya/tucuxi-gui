//@@license@@

#ifndef STANDARDTREATMENT_H
#define STANDARDTREATMENT_H


#include "core/dal/entity.h"
#include "core/utils/duration.h"

namespace ezechiel {
namespace GuiCore {



class StandardTreatment : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(StandardTreatment)

    AUTO_PROPERTY_DECL(bool, isFixedDuration, IsFixedDuration)
    AUTO_PROPERTY_DECL(Duration, duration, Duration)

protected:

    Q_INVOKABLE explicit StandardTreatment(AbstractRepository *repository, QObject *parent = nullptr);

};

} // namespace core
} // namespace ezechiel

#endif // STANDARDTREATMENT_H
