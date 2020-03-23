#include "core/dal/chartdata.h"

namespace ezechiel {
namespace core {

Q_AUTO_PROPERTY_IMPL(ChartData, PredictionResult*, popPred, PopPred)
Q_AUTO_PROPERTY_IMPL(ChartData, PredictionResult*, aprPred, AprPred)
Q_AUTO_PROPERTY_IMPL(ChartData, PredictionResult*, apoPred, ApoPred)
Q_AUTO_PROPERTY_IMPL(ChartData, PredictionResult*, revPred, RevPred)
Q_AUTO_PROPERTY_IMPL(ChartData, PredictionResult*, adjPred, AdjPred)

ChartData::ChartData(AbstractRepository *repository,QObject *parent)
    : Entity(repository, parent),
      _popPred(CoreFactory::createEntity<PredictionResult>(repository, this)),
      _aprPred(CoreFactory::createEntity<PredictionResult>(repository, this)),
      _apoPred(CoreFactory::createEntity<PredictionResult>(repository, this)),
      _revPred(CoreFactory::createEntity<PredictionResult>(repository, this)),
      _adjPred(CoreFactory::createEntity<PredictionResult>(repository, this))
{

}

void ChartData::initialize()
{
    if (_popPred)
        delete _popPred;
    if (_aprPred)
        delete _aprPred;
    if (_apoPred)
        delete _apoPred;
    if (_revPred)
        delete _revPred;
    if (_adjPred)
        delete _adjPred;

    _popPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _aprPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _apoPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _revPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _adjPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
}

}
}
