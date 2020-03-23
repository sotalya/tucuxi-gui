#include "core/dal/predictionspec.h"

namespace ezechiel {
namespace core {

AUTO_PROPERTY_IMPL(PredictionSpec, QDateTime, startDate, StartDate)
AUTO_PROPERTY_IMPL(PredictionSpec, QDateTime, endDate, EndDate)
AUTO_PROPERTY_IMPL(PredictionSpec, bool, steadyState, SteadyState)
AUTO_PROPERTY_IMPL(PredictionSpec, int, nbPoints, NbPoints)
AUTO_PROPERTY_IMPL(PredictionSpec, ParameterType, paramsType, ParamsType)
AUTO_PROPERTY_IMPL(PredictionSpec, DrugResponseAnalysis*, analysis, Analysis)
AUTO_PROPERTY_IMPL(PredictionSpec, std::vector<double>, percentileList, PercentileList)
AUTO_PROPERTY_IMPL(PredictionSpec, QDateTime, adjustmentDate, AdjustmentDate)
AUTO_PROPERTY_IMPL(PredictionSpec, bool, adjustmentWithLoadingDose, AdjustmentWithLoadingDose)
AUTO_PROPERTY_IMPL(PredictionSpec, bool, adjustmentWithRestPeriod, AdjustmentWithRestPeriod)
AUTO_PROPERTY_IMPL(PredictionSpec, PredictionSpec::CalculationType, calculationType, CalculationType)



QMap<PredictionSpec::CalculationType, QString> PredictionSpec::typeStringMap;
bool PredictionSpec::mapFilled = false;

void PredictionSpec::checkMap()
{
    if (mapFilled)
        return;
    typeStringMap.insert(CalculationType::PopPred, "PopPred");
    typeStringMap.insert(CalculationType::PopPerc, "PopPerc");
    typeStringMap.insert(CalculationType::ApoPred, "ApoPred");
    typeStringMap.insert(CalculationType::ApoPerc, "ApoPerc");
    typeStringMap.insert(CalculationType::AprPred, "AprPred");
    typeStringMap.insert(CalculationType::AprPerc, "AprPerc");
    typeStringMap.insert(CalculationType::RevPred, "RevPred");
    typeStringMap.insert(CalculationType::AdjPred, "AdjPred");
    typeStringMap.insert(CalculationType::AdjPerc, "AdjPerc");
    typeStringMap.insert(CalculationType::Quit, "Quit");
    typeStringMap.insert(CalculationType::Undefined, "Undefined");
}

QString PredictionSpec::calculationTypeToString(CalculationType type)
{
    checkMap();
    return typeStringMap.value(type, "UndefinedCalculation");
}

PredictionSpec::CalculationType PredictionSpec::stringToCalculationType(const QString &string)
{
    checkMap();
    return typeStringMap.key(string, CalculationType::Undefined);
}

} // namespace core
} // namespace ezechiel






