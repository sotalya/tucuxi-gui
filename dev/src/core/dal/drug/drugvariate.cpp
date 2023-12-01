//@@license@@

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
