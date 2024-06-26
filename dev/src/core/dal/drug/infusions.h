/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef INFUSIONS_H
#define INFUSIONS_H

#include "core/dal/entity.h"
#include "core/utils/units.h"
#include "core/utils/duration.h"
#include "core/dal/drug/operableamount.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class ValidInfusion : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(ValidInfusion)

    AUTO_PROPERTY_DECL(IdentifiableAmount*, quantity, Quantity)

protected:

    Q_INVOKABLE explicit ValidInfusion(AbstractRepository *repository, QObject *parent = nullptr)
      : Entity(repository, parent),
        _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this))
    {
        _quantity->setValue(0);
        _quantity->setUnit(Unit(""));
    }

    Q_INVOKABLE ValidInfusion(AbstractRepository *repository, const double &infusion, const QString &unit)
        : Entity(repository),
        _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this))
    {
        _quantity->setValue(infusion);
        _quantity->setUnit(Unit(unit));
    }
};

POINTERLIST_CLASS_DECL(InfusionsList, ValidInfusion)

class ValidInfusions : public Entity, public QList<ValidInfusion*>

{

    Q_OBJECT

    ENTITY_UTILS(ValidInfusions)

    AUTO_PROPERTY_DECL(IdentifiableAmount*, quantity, Quantity)
    AUTO_PROPERTY_DECL(bool, any, Any)
    AUTO_PROPERTY_DECL(InfusionsList*, list, List)
    AUTO_PROPERTY_DECL(QList<double>, infusionsList, InfusionsList)

public:
    Q_INVOKABLE ValidInfusions(AbstractRepository *repository, QObject *parent = nullptr)
        : Entity(repository, parent),
        _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this)),
        _any(false),
        _list(CoreFactory::createEntity<InfusionsList>(repository))
    {
        _quantity->setValue(0);
        _quantity->setUnit(Unit(""));
    }

    ~ValidInfusions()
    {
        for (int i = 0; i < _list->size(); i++) {
            ValidInfusion *infusion = _list->at(i);
            delete infusion;
        }
    }
/*
 * This constructor is commented, as it was written badly (error in the foreach loop)
 *
    Q_INVOKABLE ValidInfusions(AbstractRepository *repository, QObject *parent, const QList<double> &infusions, const Unit &unit, double defaultInfusion, bool any, int did)
        :  Entity(repository, parent),
            _any(any),
            _list(CoreFactory::createEntity<InfusionsList>(repository)),
        _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this))
    {
        _quantity->setValue(0);
        _quantity->setUnit(Unit(""));
        foreach( double _i, infusions) {
            ValidInfusion* _vi = CoreFactory::createEntity<ValidInfusion>(repository);
            _vi->getQuantity()->setUnit(unit.toString());
            _vi->setId(did);
            _list->append(_vi);
        }
    }
*/
    // The 4 following functions are just facilitators
    int size() const { return _list->size();}
    ValidInfusion *at(int i) const { return _list->at(i);}
    bool isEmpty() const { return _list->isEmpty();}
    void append(ValidInfusion *infusion) { _list->append(infusion);}

    QList<Duration> durations() const {
        QList<Duration> infusions;

        //Construct the durations
        for (int i = 0; i < _list->size(); i++)
            infusions.append(Duration(int(_list->at(i)->getQuantity()->getDbvalue()) / 60, int(_list->at(i)->getQuantity()->getDbvalue()) % 60));

        //Return the infusions
        return infusions;
    }

    QList<double> infusions() {
        QList<double> infusions;

        //Construct the durations
        for (int i = 0; i < _list->size(); i++)
            infusions.append(_list->at(i)->getQuantity()->getDbvalue());

        //Return the infusions
        return infusions;
    }

private:

};
QML_POINTERLIST_CLASS_DECL(InfusionList,ValidInfusion)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ValidInfusion*)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ValidInfusions*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::ValidInfusion*>)

#endif // INFUSIONS_H
