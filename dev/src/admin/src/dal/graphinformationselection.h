#ifndef GRAPHINFORMATIONSELECTION_H
#define GRAPHINFORMATIONSELECTION_H

#include "core/dal/entity.h"
#include "steptypes.h"
#include "curvetypes.h"
#include "core/settings.h"

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
        void initStep(Tucuxi::Gui::Admin::StepType::Enum stepType, bool curveInfo[CurveType::size]);
        void updateProperties();
        void loadDisplayParametersSettings();

	private:
        GraphInfo _graphInfo[StepType::size][CurveType::size];
        StepType::Enum _currentStep;
        QMap<QString, QVariant> _parametersSettingsMap;
};

}
}
}

#endif // GRAPHINFORMATIONSELECTION_H
