#ifndef DOSES_H
#define DOSES_H

#include "core/utils/duration.h"
#include "core/utils/units.h"
#include "core/dal/drug/operableamount.h"
#include "core/dal/entity.h"
#include "core/dal/drug/adme.h"

namespace ezechiel {
namespace core {

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
} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::core::ValidDose*)
Q_DECLARE_METATYPE(QList<ezechiel::core::ValidDose*>)
Q_DECLARE_METATYPE(ezechiel::core::ValidDoses*)

#endif // DOSES_H
