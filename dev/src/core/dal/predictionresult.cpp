#include "core/dal/predictionresult.h"
#include "core/dal/entity.h"

namespace ezechiel {
namespace core {

//AUTO_PROPERTY_IMPL(PredictionResult, ParameterType, paramsType, ParamsType)
Q_AUTO_PROPERTY_IMPL(PredictionResult, Predictive*, predictive, Predictive)
Q_AUTO_PROPERTY_IMPL(PredictionResult, Adjustments*, adjustments, Adjustments)

PredictionResult::PredictionResult(AbstractRepository *repository,QObject *parent)
    : Entity(repository, parent),
      _predictive(CoreFactory::createEntity<Predictive>(repository, this)),
      _adjustments(CoreFactory::createEntity<Adjustments>(repository, this))
//      _paramsType(ParameterType::APRIORI)
{

}

}
}
