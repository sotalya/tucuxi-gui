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


#ifndef DOSES_H
#define DOSES_H

#include "core/utils/duration.h"
#include "core/utils/units.h"
#include "core/dal/drug/operableamount.h"
#include "core/dal/entity.h"
#include "core/dal/drug/adme.h"

Q_MOC_INCLUDE("core/dal/drug/translatablestring.h")

namespace Tucuxi {
namespace Gui {
namespace Core {

class TranslatableString;

class ValidDose : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(ValidDose)

    AUTO_PROPERTY_DECL(IdentifiableAmount*, quantity, Quantity)
    AUTO_PROPERTY_DECL(Admin*, route, Route) // YTA: This Route should be declared outside dosage.h


protected:

    Q_INVOKABLE explicit ValidDose(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent),
        _route(CoreFactory::createEntity<Admin>(repository, this)),
        _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this)) {
        _quantity->setValue(0);
        _quantity->setUnit(Unit(""));
    }
    Q_INVOKABLE ValidDose(AbstractRepository *repository, const double &dose, const QString &unit)
        : Entity(repository),
        _route(CoreFactory::createEntity<Admin>(repository, this)),
          _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this)) {
        _quantity->setValue(dose);
        _quantity->setUnit(unit);
    }

public:

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

    ~ValidDose() {}
};



class ValidDoses : public Entity, public QList<ValidDose*>
{

    Q_OBJECT

    ENTITY_UTILS(ValidDoses)

    AUTO_PROPERTY_DECL(IdentifiableAmount*, quantity, Quantity)
    AUTO_PROPERTY_DECL(bool, anyDose, AnyDose)

    AUTO_PROPERTY_DECL(IdentifiableAmount*, fromDose, FromDose)
    AUTO_PROPERTY_DECL(IdentifiableAmount*, toDose, ToDose)
    AUTO_PROPERTY_DECL(IdentifiableAmount*, stepDose, StepDose)

    AUTO_PROPERTY_DECL(TranslatableString*, comments,Comments)

    AUTO_PROPERTY_DECL(QList<double>, dosesList, DosesList)


public:


    explicit ValidDoses(AbstractRepository *repository, QObject *parent = nullptr) :
        Entity(repository, parent),
        _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this)),
        _anyDose(false),
        _fromDose(CoreFactory::createEntity<IdentifiableAmount>(repository, this)),
        _toDose(CoreFactory::createEntity<IdentifiableAmount>(repository, this)),
        _stepDose(CoreFactory::createEntity<IdentifiableAmount>(repository, this)),
        _comments(nullptr)
    {
        _quantity->setValue(0);
        _quantity->setUnit(Unit(""));
    }
    ValidDoses(AbstractRepository *repository, QObject *parent, const QList<double> &doses, const Unit &unit, double value, bool any, double from, double to, double step, int _drug_id)
        : Entity(repository, parent), _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this)), _anyDose(any) {
        _quantity->setValue(value);
        _quantity->setUnit(unit);
        _toDose = CoreFactory::createEntity<IdentifiableAmount>(repository, this);
        _toDose->setValue(to);
        _toDose->setUnit(unit);
        _fromDose = CoreFactory::createEntity<IdentifiableAmount>(repository, this);
        _fromDose->setValue(from);
        _fromDose->setUnit(unit);
        _stepDose = CoreFactory::createEntity<IdentifiableAmount>(repository, this);
        _stepDose->setValue(step);
        _stepDose->setUnit(unit);
        foreach( double _d, doses) {
            ValidDose* _vd = CoreFactory::createEntity<ValidDose>(repository);
            _vd->getQuantity()->setDbvalue(_d);
            _vd->getQuantity()->setUnit(Unit(unit.toString()));
            this->append(_vd);
        }
    }

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

    /** \brief Construct and returns the drug's supported doses amounts.
     * @return The doses amounts.
     */
    QList<Amount*> amounts() const {
        QList<Amount*> amounts;

        //Check the doses unit
        if (!getQuantity()->unit().isValid())
            return amounts;

        //Constructs the amounts
        for (int i = 0; i < this->size(); i++)
            amounts.append(this->at(i)->getQuantity());

        //Return the amounts
        return amounts;
    }

  Q_INVOKABLE  QList<double> doses() const {
        QList<double> _doses;

         for (int i = 0; i < this->size(); i++)
             _doses.append(this->at(i)->getQuantity()->value());

         return _doses;
    }

};
QML_POINTERLIST_CLASS_DECL(DoseList,ValidDose)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ValidDose*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::ValidDose*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ValidDoses*)

#endif // DOSES_H
