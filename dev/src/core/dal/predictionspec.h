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


#ifndef PREDICTIONSPEC_H
#define PREDICTIONSPEC_H

#include <QDateTime>

#include "core/dal/entity.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drug/parameters.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



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
        //  _analysis = Tucuxi::Gui::Core::CoreFactory::createEntity<DrugResponseAnalysis>(repository);
    }
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::PredictionSpec*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::PredictionSpec*>)

#endif // PREDICTIONSPEC_H
