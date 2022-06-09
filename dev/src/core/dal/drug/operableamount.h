//@@license@@

#ifndef OPERABLEAMOUNT_H
#define OPERABLEAMOUNT_H

#include "core/dal/entity.h"
#include "core/dal/amount.h"
#include "core/utils/ezutils.h"
#include "core/dal/drug/operation.h"

namespace ezechiel {
namespace GuiCore {

class IdentifiableAmount : public Entity, public Amount
{
    Q_OBJECT

    ENTITY_UTILS(IdentifiableAmount)
    Q_PROPERTY(double dbvalue READ getDbvalue WRITE setDbvalue NOTIFY dbvalueChanged)
    Q_PROPERTY(double multiplier READ getMultiplier NOTIFY multiplierChanged)
    Q_PROPERTY(QString unitstring READ getUnitstring WRITE setUnitstring NOTIFY unitstringChanged)
    AUTO_PROPERTY_DECL(QString, amountId, AmountId)
public:
    QString getUnitstring() {
        return this->unit().name();
    }//toString(); }
    void setUnitstring(QString& unit) {
        Unit u;
        u.fromString("["+unit+"]");
        if (u != this->unit()) {
            _unit.fromString("["+unit+"]");
            emit unitstringChanged(unit);
            emit multiplierChanged(getMultiplier());
        }
    }
    double getDbvalue() const { return this->value(); }
    void setDbvalue(double val) {
        if (this->value()!= val) {
            this->setValue(val);
            emit dbvalueChanged(val);
        }
    }
    double getMultiplier() { return unit().multiplier(Unit("ug/l")); }

signals:
    void dbvalueChanged(double);
    void multiplierChanged(double);
    void unitstringChanged(QString);

protected:
    Q_INVOKABLE IdentifiableAmount(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent), _amountId("default"), Amount() {}
};

//class OperationList;

class OperableAmount : public IdentifiableAmount
{
    Q_OBJECT

    ENTITY_UTILS(OperableAmount)

    AUTO_PROPERTY_DECL(OperationList*, operations, Operations)

    // This field should be used for PatientVariate, for instance.
    // It will allow to define if the user did choose a value or did let the operation be applied
    // Should not be used for DrugVariate
    AUTO_PROPERTY_DECL(bool, fixed, Fixed)

    protected:
        Q_INVOKABLE OperableAmount(AbstractRepository *repository, QObject *parent = nullptr)
      : IdentifiableAmount(repository, parent),
       _operations(CoreFactory::createEntity<OperationList>(repository, parent)),
       _fixed(false)
    {}
};

class OperableValue : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(OperableValue)

    AUTO_PROPERTY_DECL(QString, valueId, ValueId)
    AUTO_PROPERTY_DECL(QVariant, value, Value)
    AUTO_PROPERTY_DECL(OperationList*, operations, Operations)

    protected:
        Q_INVOKABLE OperableValue(AbstractRepository *repository, QObject *parent = nullptr)
      : Entity(repository, parent),
       _operations(CoreFactory::createEntity<OperationList>(repository, parent))
        {}
};

POINTERLIST_CLASS_DECL(OperableValueList, OperableValue)


} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::GuiCore::IdentifiableAmount*)
Q_DECLARE_METATYPE(QList<ezechiel::GuiCore::IdentifiableAmount*>)
Q_DECLARE_METATYPE(ezechiel::GuiCore::OperableAmount*)
Q_DECLARE_METATYPE(QList<ezechiel::GuiCore::OperableAmount*>)
Q_DECLARE_METATYPE(ezechiel::GuiCore::OperableValue*)
Q_DECLARE_METATYPE(QList<ezechiel::GuiCore::OperableValue*>)

#endif // OPERABLEAMOUNT_H
