//@@license@@

#include "predictive.h"

namespace Tucuxi {
namespace GuiCore {

Q_AUTO_PROPERTY_IMPL(Predictive, PredictionData*, predictionData, PredictionData)
Q_AUTO_PROPERTY_IMPL(Predictive, PercentileDataList*, percentilePairs, PercentileDataList)

Q_AUTO_PROPERTY_IMPL(Predictive, FancyPoints*, pointsAtMeasures, PointsAtMeasures)

Predictive::Predictive(AbstractRepository *repository, QObject *parent)
    : Entity(repository, parent),
      _predictionData(CoreFactory::createEntity<PredictionData>(repository, this)),
      _percentilePairs(CoreFactory::createEntity<PercentileDataList>(repository, this)),
      _pointsAtMeasures(CoreFactory::createEntity<FancyPoints>(repository, this))
{

}

}
}
