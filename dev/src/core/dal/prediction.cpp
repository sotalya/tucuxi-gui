#include "core/dal/prediction.h"
//#include "core/corerepository.h"
#include "core/core.h"
#include "core/dal/drug/parameters.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/predictionspec.h"
#include "core/dal/dosage.h"

namespace ezechiel {
namespace core {

AUTO_PROPERTY_IMPL(Prediction, QString, name, Name)
AUTO_PROPERTY_IMPL(Prediction, QString, comment, Comment)
AUTO_PROPERTY_IMPL(Prediction, CurveType, curveType, CurveType)
AUTO_PROPERTY_IMPL(Prediction, ParameterType, paramsType, ParamsType)
AUTO_PROPERTY_IMPL(Prediction, DrugResponseAnalysis*, analysis, Analysis)
AUTO_PROPERTY_IMPL(Prediction, PredictionSpec*, spec, Spec)

Prediction::Prediction(AbstractRepository *repository, QObject* parent, DrugResponseAnalysis* analysis)
    : Entity(repository, parent), _name(""), _comment(""), _curveType(CurveType::Continous),
      _paramsType(ParameterType::POPULATION),
    _analysis(analysis),
    _spec(ezechiel::core::CoreFactory::createEntity<PredictionSpec>(repository))
{
}


Prediction::Prediction(AbstractRepository *repository, QObject* parent)
    : Entity(repository, parent), _name(""), _comment(""), _curveType(CurveType::Continous),
      _paramsType(ParameterType::POPULATION) //,
      //_spec(ezechiel::core::CoreFactory::createEntity<PredictionSpec>(repository)),
      //_analysis(ezechiel::core::CoreFactory::createEntity<DrugResponseAnalysis>(repository))
{

          _spec=(ezechiel::core::CoreFactory::createEntity<PredictionSpec>(repository));
          _analysis=(ezechiel::core::CoreFactory::createEntity<DrugResponseAnalysis>(repository));
//    LOG(QtDebugMsg, NOEZERROR, "Prediction Constructor");
}

DrugTreatment *Prediction::getTreatment() const
{
    return getAnalysis()->getTreatment();
}

//QDateTime Prediction::firsttake() const
//{
//    return getAnalysis()->getTreatment()->getDosages()->firsttake();
//}

void Prediction::setFirsttake(const QDateTime &firsttake)
{
    getAnalysis()->getTreatment()->getDosages()->setFirsttake(firsttake);
}

} // namespace core
} // namespace ezechiel
