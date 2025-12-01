/*
 * Tucuxi - Tucuxi-gui software.
 * This software is able to perform prediction of drug concentration in blood
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


#ifndef GRAPHINFORMATIONSELECTION_H
#define GRAPHINFORMATIONSELECTION_H

#include "core/dal/entity.h"
#include "steptypes.h"
#include "curvetypes.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

struct GraphInfo
{
    bool available;
    bool visible;
};

typedef enum{
    Perc5_95 = 0,
    Perc10_90,
    Perc25_75,
    Perc50
}PercentileRangeEnum;

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::PercentileRangeEnum)

namespace Tucuxi {
namespace Gui {
namespace Admin {

class GraphInformationSelection : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(GraphInformationSelection)

    AUTO_PROPERTY_DECL(bool, displayPopulationPrediction, DisplayPopulationPrediction)

    AUTO_PROPERTY_DECL(bool, displayAprioriPrediction, DisplayAprioriPrediction)

    AUTO_PROPERTY_DECL(bool, displayAposterioriPrediction, DisplayAposterioriPrediction)

    AUTO_PROPERTY_DECL(bool, displayPopulationPercentiles, DisplayPopulationPercentiles)

    AUTO_PROPERTY_DECL(bool, displayAprioriPercentiles, DisplayAprioriPercentiles)

    AUTO_PROPERTY_DECL(bool, displayAposterioriPercentiles, DisplayAposterioriPercentiles)

    AUTO_PROPERTY_DECL(bool, displayPossibleAdjustments, DisplayPossibleAdjustments)

    AUTO_PROPERTY_DECL(bool, displaySelectedAdjustment, DisplaySelectedAdjustment)

    AUTO_PROPERTY_DECL(bool, displayMeasures, DisplayMeasures)

    AUTO_PROPERTY_DECL(bool, displayTargets, DisplayTargets)

    AUTO_PROPERTY_DECL(bool, perc5_95, Perc5_95)
    AUTO_PROPERTY_DECL(bool, perc10_90, Perc10_90)
    AUTO_PROPERTY_DECL(bool, perc25_75, Perc25_75)
    AUTO_PROPERTY_DECL(bool, perc50, Perc50)

    AUTO_PROPERTY_DECL(bool, displayCurrentTime, DisplayCurrentTime)
    AUTO_PROPERTY_DECL(bool, displayCovariateChange, DisplayCovariateChange)
    AUTO_PROPERTY_DECL(bool, displayLiveAnnotations, DisplayLiveAnnotations)


    public:
        void setCurrentTab(StepType::Enum step);
        Q_INVOKABLE void setAvailable(Tucuxi::Gui::Admin::CurveType::Enum curveType, bool isAvailable);
        Q_INVOKABLE void setAvailable(Tucuxi::Gui::Admin::StepType::Enum stepType, CurveType::Enum curveType, bool isAvailable);
        Q_INVOKABLE void saveSettings();
        Q_INVOKABLE void setPercentile(Tucuxi::Gui::Admin::PercentileRangeEnum percentileRange, bool isAvailable);
        Q_INVOKABLE QString getCurrentTab() const;

    protected:
        Q_INVOKABLE GraphInformationSelection(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);

    private:
        void initStep(Tucuxi::Gui::Admin::StepType::Enum stepType, std::array<bool, CurveType::size> curveInfo);
        void updateProperties();
        void loadDisplayParametersSettings();

	private:
        std::array<std::array<GraphInfo, CurveType::size>, StepType::size> _graphInfo;
        StepType::Enum _currentStep;
        QMap<QString, QVariant> _parametersSettingsMap;
};

}
}
}

#endif // GRAPHINFORMATIONSELECTION_H
