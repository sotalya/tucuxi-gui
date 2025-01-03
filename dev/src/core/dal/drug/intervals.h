/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef INTERVALS_H
#define INTERVALS_H

#include "core/dal/entity.h"
#include "core/dal/drug/operableamount.h"
#include "core/utils/units.h"
#include "core/utils/duration.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


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
            ValidInterval* _vi = Tucuxi::Gui::Core::CoreFactory::createEntity<ValidInterval>(repository);
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
} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ValidInterval*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::ValidInterval*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ValidIntervals*)

#endif // INTERVALS_H
