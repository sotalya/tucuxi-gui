//@@license@@

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
{/*
    if (_popPred)
        _popPred->deleteLater();
    if (_aprPred)
        _aprPred->deleteLater();
    if (_apoPred)
        _apoPred->deleteLater();
    if (_revPred)
        _revPred->deleteLater();
    if (_adjPred)
        _adjPred->deleteLater();
*/
    _popPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _aprPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _apoPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _revPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _adjPred = CoreFactory::createEntity<PredictionResult>(_repository, this);

}

}
}
