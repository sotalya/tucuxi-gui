//@@license@@

#include "percentilepair.h"

namespace Tucuxi {
namespace GuiCore {

QML_POINTERLIST_CLASS_IMPL(PercentileDataList, PercentileData)

AUTO_PROPERTY_IMPL(PercentileData, double, percentile, Percentile)
Q_AUTO_PROPERTY_IMPL(PercentileData, PredictionData*, predictionData, PredictionData)

PercentileData::PercentileData(AbstractRepository *repository, QObject *parent)
    : Entity(repository, parent),
      _percentile(0.0),
      _predictionData(CoreFactory::createEntity<PredictionData>(ABSTRACTREPO, this))
{

}

}
}
