//@@license@@

#ifndef PERCENTILEPAIR_H
#define PERCENTILEPAIR_H

#include <QObject>
#include "core/utils/autoproperty.h"
#include "predictiondata.h"
#include "core/dal/entity.h"

namespace ezechiel {
namespace GuiCore {

class PercentileData : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(PercentileData)
    AUTO_PROPERTY_DECL(double, percentile, Percentile)
    AUTO_PROPERTY_DECL(PredictionData*, predictionData, PredictionData)
public:
    explicit PercentileData(AbstractRepository *repository, QObject *parent = nullptr);
};

QML_POINTERLIST_CLASS_DECL(PercentileDataList, PercentileData)
}
}
Q_DECLARE_METATYPE(ezechiel::GuiCore::PercentileData*)
Q_DECLARE_METATYPE(QList<ezechiel::GuiCore::PercentileData*>)
Q_DECLARE_METATYPE(ezechiel::GuiCore::PercentileDataList*)
#endif // PERCENTILEPAIR_H
