//@@license@@

#ifndef INTERVALS_H
#define INTERVALS_H

#include "core/dal/entity.h"
#include "core/dal/drug/operableamount.h"
#include "core/utils/units.h"
#include "core/utils/duration.h"

namespace ezechiel {
namespace GuiCore {


class ValidInterval : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(ValidInterval)

    AUTO_PROPERTY_DECL(IdentifiableAmount*, quantity, Quantity)

protected:

    Q_INVOKABLE explicit ValidInterval(AbstractRepository *repository, QObject *parent = nullptr)
      : Entity(repository, parent),
        _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this))
    {
        _quantity->setValue(0);
        _quantity->setUnit(Unit(""));
    }

    Q_INVOKABLE ValidInterval(AbstractRepository *repository, QObject *parent, const double &interval, const QString &unit)
        : Entity(repository, parent), _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this)) {
        _quantity->setValue(interval);
        _quantity->setUnit(unit);
    }

};

class ValidIntervals : public Entity, public QList<ValidInterval*>
{
    Q_OBJECT

    ENTITY_UTILS(ValidIntervals)


    AUTO_PROPERTY_DECL(IdentifiableAmount*, quantity, Quantity)
    AUTO_PROPERTY_DECL(bool, any, Any)
    AUTO_PROPERTY_DECL(QList<double>, intervalsList, IntervalsList)

public:
    explicit ValidIntervals(AbstractRepository *repository, QObject *parent = nullptr)
        : Entity(repository, parent),
        _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this)),
        _any(false)
    {
        _quantity->setValue(0);
        _quantity->setUnit(Unit(""));
    }

    ValidIntervals(AbstractRepository *repository, QObject *parent, const QList<double> &intervals, const Unit &unit, double defaultinterval, bool any, int did)
        : Entity(repository, parent), _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this)), _any(any) {
        _quantity->setValue(defaultinterval);
        _quantity->setUnit(unit);
        foreach( double _i, intervals) {
            ValidInterval* _vi = ezechiel::GuiCore::CoreFactory::createEntity<ValidInterval>(repository);
            _vi->getQuantity()->setUnit(Unit(unit.toString()));
            this->append(_vi);
        }
    }

    QList<Duration> durations() const {
        QList<Duration> _durations;

         for (int i = 0; i < this->size(); i++)
             _durations.append(Duration(this->at(i)->getQuantity()->value()));

         return _durations;
    }

    QList<double> intervals() const {
        QList<double> _intervals;

         for (int i = 0; i < this->size(); i++)
             _intervals.append(this->at(i)->getQuantity()->value());

         return _intervals;
    }

private:

};
QML_POINTERLIST_CLASS_DECL(IntervalList,ValidInterval)
} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::GuiCore::ValidInterval*)
Q_DECLARE_METATYPE(QList<ezechiel::GuiCore::ValidInterval*>)
Q_DECLARE_METATYPE(ezechiel::GuiCore::ValidIntervals*)

#endif // INTERVALS_H
