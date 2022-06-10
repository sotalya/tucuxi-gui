//@@license@@

#include <QString>

#include "core/dal/covariate.h"

#include "core/core.h"
#include "core/definitionmanager.h"
#include "core/corefactory.h"
#include "core/dal/uncastedvalue.h"

namespace Tucuxi {
namespace GuiCore {

AUTO_PROPERTY_IMPL(PatientVariate, QDateTime, date, Date)
AUTO_PROPERTY_IMPL(PatientVariate, QString, valueAsString, ValueAsString)
AUTO_PROPERTY_IMPL(PatientVariate, OperableAmount*, quantity, Quantity)
AUTO_PROPERTY_IMPL(PatientVariate, QString, description, Description)
AUTO_PROPERTY_IMPL(PatientVariate, Descriptor, descriptor, Descriptor)
AUTO_PROPERTY_IMPL(PatientVariate, QMetaType::Type, type, Type)
AUTO_PROPERTY_IMPL(PatientVariate, bool, forced, Forced)
AUTO_PROPERTY_IMPL(PatientVariate, UncastedValueList*, uncastedValues, UncastedValues)

//Contructor
PatientVariate::PatientVariate(AbstractRepository *repository, QObject* parent, const QString &name, const QDateTime &date) :
    Entity(repository, parent), _date(date), _name(name), _type(QMetaType::Int), _forced(false),
    _uncastedValues(CoreFactory::createEntity<UncastedValueList>(repository, this))
{
    setId(-1);
    _quantity = Tucuxi::GuiCore::CoreFactory::createEntity<OperableAmount>(repository, this);
    _quantity->setValue(0);
}

//Constructor
PatientVariate::PatientVariate(AbstractRepository *repository, QObject* parent, const QString& name, const QDateTime &date, double value) :
    Entity(repository, parent), _name(name), _date(date), _forced(true),
    _uncastedValues(CoreFactory::createEntity<UncastedValueList>(repository, this))
{
    // YJE: This code is just not working :-( To be cleaned up...
    // _valid = CORE->definitionManager()->covariateExists(name);

    setId(-1);
    _quantity->setValue(value);
}

//Sets the covariate ID
bool PatientVariate::setCovariateId(const QString covariateId)
{
    _covariateId = covariateId;

    // YJE: This code is just not working :-( To be cleaned up...
    //_valid = CORE->definitionManager()->covariateExists(covariateId);
    //return _valid;
    return true;
}


//Sets the covariate ID
bool PatientVariate::setName(const QString name)
{
    _name = name;

    // YJE: This code is just not working :-( To be cleaned up...
    //_valid = CORE->definitionManager()->covariateExists(name);
    //return _valid;
    return true;
}

//Returns a string description of the covariate
QString PatientVariate::toString() const
{
//    if (!isValid())
//        return QString("Invalid covariate");

    if (getQuantity()->unit().toString().isEmpty())
        return QString("%1: %2").arg(getCovariateId()).arg(_quantity->value());
    else
        return QString("%1: %2 %3").arg(getCovariateId()).arg(_quantity->value()).arg(getQuantity()->unit().toString());
}

//Checks if the covariate is valid
bool PatientVariate::isValid() const
{
    return true;
//    return _valid; // YJE: This code is just not working :-( To be cleaned up...
}

//Assigns the value of the covariate
void PatientVariate::operator=(double value)
{
    _quantity->setValue(value);
}

//Compares the covariate with another
bool PatientVariate::operator ==(const PatientVariate &other) const
{
    return _id == other._id &&
            _date == other._date &&
            _quantity->value() == other.getQuantity()->value() &&
            _valid == other._valid;
}

//Compares the covariate with another
bool PatientVariate::operator !=(const PatientVariate &other) const
{
    return !(*this == other);
}

QML_POINTERLIST_CLASS_IMPL(PatientVariateList, PatientVariate)

} // namespace core
} // namespace Tucuxi

