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


#include <QString>

#include "core/dal/covariate.h"

#include "core/core.h"
#include "core/definitionmanager.h"
#include "core/corefactory.h"
#include "core/dal/uncastedvalue.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


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
    _quantity = Tucuxi::Gui::Core::CoreFactory::createEntity<OperableAmount>(repository, this);
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

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

