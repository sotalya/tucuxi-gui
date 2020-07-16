#ifndef PREDICTIONRESULT_H
#define PREDICTIONRESULT_H

#include <QObject>
#include "core/dal/drug/parameters.h"
#include "predictive.h"
#include "adjustment.h"
#include "core/dal/entity.h"

namespace ezechiel {
namespace core {

class PredictionResult : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(PredictionResult)
//    AUTO_PROPERTY_DECL(ParameterType, paramsType, ParamsType)
    AUTO_PROPERTY_DECL(Predictive*, predictive, Predictive)
    AUTO_PROPERTY_DECL(Adjustments*, adjustments, Adjustments)
    AUTO_PROPERTY_DECL(bool, isValidDomain, IsValidDomain)
    AUTO_PROPERTY_DECL(QString, domainMessage, DomainMessage)
    public:
    explicit PredictionResult(AbstractRepository *repository,QObject *parent = nullptr);

};

}
}
Q_DECLARE_METATYPE(ezechiel::core::PredictionResult*)
Q_DECLARE_METATYPE(QList<ezechiel::core::PredictionResult*>)
#endif // PREDICTIONRESULT_H
