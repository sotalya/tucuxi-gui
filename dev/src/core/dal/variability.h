//@@license@@

#ifndef VARIABILITY_H
#define VARIABILITY_H

#include "core/dal/entity.h"
#include "core/dal/amount.h"
#include "core/utils/ezutils.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class OperationList;
class TranslatableString;

class OperableDouble : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(OperableDouble)

    AUTO_PROPERTY_DECL(QString, valueId, ValueId)
    AUTO_PROPERTY_DECL(double, value, Value)
    AUTO_PROPERTY_DECL(OperationList*, operations, Operations)

    protected:
        Q_INVOKABLE OperableDouble(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) {}
};

POINTERLIST_CLASS_DECL(OperableDoubleList, OperableDouble)

class Variability : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Variability)

    AUTO_PROPERTY_DECL(QString, distributionId, DistributionId)
    AUTO_PROPERTY_DECL(OperableDoubleList*, stdDeviations, StdDeviations)
    AUTO_PROPERTY_DECL(TranslatableString*,comments,Comments)


protected:
    Q_INVOKABLE Variability(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) {}
};


class VariableValue : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(VariableValue)

    AUTO_PROPERTY_DECL(QVariant, value, Value)
    AUTO_PROPERTY_DECL(Variability*, variability, Variability)

protected:
    Q_INVOKABLE VariableValue(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) {}
};


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Variability*)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::VariableValue*)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::OperableDouble*)

#endif // VARIABILITY_H
