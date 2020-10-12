#ifndef PREDICTIONSPEC_H
#define PREDICTIONSPEC_H

#include <QDateTime>

#include "core/dal/entity.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drug/parameters.h"

namespace ezechiel {
namespace core {


class PredictionSpec : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(PredictionSpec)

public:

    enum CalculationType {
         PopPred,
         PopPerc,
         AprPred,
         AprPerc,
         ApoPred,
         ApoPerc,
         RevPred,
         AdjPred,
         AdjPerc,
         Quit,
         Undefined
    };

    static QString calculationTypeToString(CalculationType type);
    static CalculationType stringToCalculationType(const QString &string);

protected:
    static QMap<CalculationType, QString> typeStringMap;
    static bool mapFilled;
    static void checkMap();

    AUTO_PROPERTY_DECL(QDateTime, startDate, StartDate)
    AUTO_PROPERTY_DECL(QDateTime, endDate, EndDate)
    AUTO_PROPERTY_DECL(bool, steadyState, SteadyState)
    AUTO_PROPERTY_DECL(int, nbPoints, NbPoints)
    AUTO_PROPERTY_DECL(DrugResponseAnalysis*, analysis, Analysis)
    AUTO_PROPERTY_DECL(ParameterType, paramsType, ParamsType)
    AUTO_PROPERTY_DECL(std::vector<double>, percentileList, PercentileList)
    AUTO_PROPERTY_DECL(QDateTime, adjustmentDate, AdjustmentDate)
    AUTO_PROPERTY_DECL(bool, adjustmentWithLoadingDose, AdjustmentWithLoadingDose)
    AUTO_PROPERTY_DECL(bool, adjustmentWithRestPeriod, AdjustmentWithRestPeriod)
    AUTO_PROPERTY_DECL(CalculationType, calculationType, CalculationType)
    AUTO_PROPERTY_DECL(bool, clearCache, ClearCache)

    protected:

        Q_INVOKABLE PredictionSpec(AbstractRepository *repository = nullptr, QObject* parent = nullptr)
      : Entity(repository),
        _steadyState(false),
        _nbPoints(250),
        _analysis(nullptr),
        _paramsType(POPULATION),
        _calculationType(Undefined),
        _adjustmentWithLoadingDose(false),
        _adjustmentWithRestPeriod(false),
        _clearCache(true)
    {
        //  _analysis = ezechiel::core::CoreFactory::createEntity<DrugResponseAnalysis>(repository);
    }
};

} // namespace core
} // namespace ezechiel


Q_DECLARE_METATYPE(ezechiel::core::PredictionSpec*)
Q_DECLARE_METATYPE(QList<ezechiel::core::PredictionSpec*>)

#endif // PREDICTIONSPEC_H
