//@@license@@

#include "core/dal/predictionresult.h"
#include "core/dal/entity.h"

namespace ezechiel {
namespace GuiCore {

//AUTO_PROPERTY_IMPL(PredictionResult, ParameterType, paramsType, ParamsType)
Q_AUTO_PROPERTY_IMPL(PredictionResult, Predictive*, predictive, Predictive)
Q_AUTO_PROPERTY_IMPL(PredictionResult, Adjustments*, adjustments, Adjustments)
AUTO_PROPERTY_IMPL(PredictionResult, bool, isValidDomain, IsValidDomain)
AUTO_PROPERTY_IMPL(PredictionResult, QString, domainMessage, DomainMessage)

PredictionResult::PredictionResult(AbstractRepository *repository,QObject *parent)
    : Entity(repository, parent),
      _predictive(CoreFactory::createEntity<Predictive>(repository, this)),
      _adjustments(CoreFactory::createEntity<Adjustments>(repository, this)),
      _isValidDomain(true),
      _domainMessage(QString("Init"))
//      _paramsType(ParameterType::APRIORI)
{

}

}
}
