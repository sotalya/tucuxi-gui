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


#include "core/dal/drug/drugvariate.h"
#include "core/core.h"
#include "core/definitionmanager.h"
#include "core/dal/drug/operation.h"
#include "core/dal/drug/translatablestring.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


AUTO_PROPERTY_IMPL(DrugVariate, QString, covariateId, CovariateId)
//AUTO_PROPERTY_IMPL(DrugVariate, QDateTime, date, Date)
AUTO_PROPERTY_IMPL(DrugVariate, OperableAmount*, quantity, Quantity)
AUTO_PROPERTY_IMPL(DrugVariate, Descriptor, descriptor, Descriptor)
AUTO_PROPERTY_IMPL(DrugVariate, QString, description, Description)
AUTO_PROPERTY_IMPL(DrugVariate, QMetaType::Type, type, Type)
AUTO_PROPERTY_IMPL(DrugVariate, bool, automatic, Automatic)
AUTO_PROPERTY_IMPL(DrugVariate, CovariateType, covariateType, CovariateType)

AUTO_PROPERTY_IMPL(DrugVariate, TranslatableString*, visualNameTranslation, VisualNameTranslation)
AUTO_PROPERTY_IMPL(DrugVariate, TranslatableString*, descriptionTranslation, DescriptionTranslation)
AUTO_PROPERTY_IMPL(DrugVariate, TranslatableString*, comments, Comments)


//Contructor
DrugVariate::DrugVariate(AbstractRepository *repository, QObject* parent, const QString &covariateId, const QDateTime &date) :
   Entity(repository, parent),
//   _date(date),
    _covariateType(CovariateType::Standard),
   _quantity(CoreFactory::createEntity<OperableAmount>(repository, this)),
   _description(""),
   _type(QMetaType::Int),
   _automatic(false),
   _visualNameTranslation(CoreFactory::createEntity<TranslatableString>(repository,this)),
   _descriptionTranslation(CoreFactory::createEntity<TranslatableString>(repository,this)),
   _comments(CoreFactory::createEntity<TranslatableString>(repository,this))
{
    setCovariateId(covariateId);
    _quantity->setValue(0);
}

//Constructor
DrugVariate::DrugVariate(AbstractRepository *repository, QObject* parent, const QString& covariateId, const QDateTime &date, double value) :
    Entity(repository, parent),
    _covariateId(covariateId),
//    _date(date),
    _quantity(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _type(QMetaType::Int),
    _automatic(false),
    _visualNameTranslation(CoreFactory::createEntity<TranslatableString>(repository,this)),
    _descriptionTranslation(CoreFactory::createEntity<TranslatableString>(repository,this)),
    _comments(CoreFactory::createEntity<TranslatableString>(repository,this))

{
    _quantity->setValue(value);
}


//Returns a string description of the covariate
QString DrugVariate::toString() const
{
//    if (!isValid())
//        return QString("Invalid covariate");

    if (_quantity->unit().name().isEmpty())
        return QString("%1: %2").arg(getCovariateId()).arg(_quantity->value());
    else
        return QString("%1: %2 %3").arg(getCovariateId()).arg(_quantity->value()).arg(_quantity->unit().name());
}


//Assigns the value of the covariate
void DrugVariate::operator=(double value)
{
    _quantity->setValue(value);
}

//Compares the covariate with another
bool DrugVariate::operator ==(const DrugVariate &other) const
{
    return _id == other._id &&
//            _date == other._date &&
            _quantity->value() == other.getQuantity()->value();
}

//Compares the covariate with another
bool DrugVariate::operator !=(const DrugVariate &other) const
{
    return !(*this == other);
}

QML_POINTERLIST_CLASS_IMPL(DrugVariateList, DrugVariate)

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
