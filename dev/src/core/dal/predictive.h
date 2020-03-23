#ifndef PREDICTIVE_H
#define PREDICTIVE_H

#include <QObject>
#include "predictiondata.h"
#include "core/utils/autoproperty.h"
#include "percentilepair.h"
#include "core/dal/entity.h"

namespace ezechiel {
namespace core {

class Predictive : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(Predictive)
    AUTO_PROPERTY_DECL(PredictionData*, predictionData, PredictionData)
    AUTO_PROPERTY_DECL(PercentileDataList*, percentilePairs, PercentileDataList)

    AUTO_PROPERTY_DECL(FancyPoints*, pointsAtMeasures, PointsAtMeasures)
public:
    explicit Predictive(AbstractRepository *repository, QObject *parent = nullptr);

signals:

public slots:
};

}
}
Q_DECLARE_METATYPE(ezechiel::core::Predictive*)
Q_DECLARE_METATYPE(QList<ezechiel::core::Predictive*>)
#endif // PREDICTIVE_H
