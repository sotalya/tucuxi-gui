/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "core/dal/predictionspec.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


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
AUTO_PROPERTY_IMPL(PredictionSpec, bool, clearCache, ClearCache)



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

} // namespace Core
} // namespace Gui
} // namespace Tucuxi






