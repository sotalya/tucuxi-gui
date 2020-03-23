#ifndef GRAPHINFORMATIONSELECTION_H
#define GRAPHINFORMATIONSELECTION_H

#include "core/dal/entity.h"
#include "steptypes.h"
#include "curvetypes.h"

struct GraphInfo
{
    bool available;
    bool visible;
};

class GraphInformationSelection : public ezechiel::core::Entity
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(GraphInformationSelection)

    AUTO_PROPERTY_DECL(bool, presentPopulationPrediction, PresentPopulationPrediction)
    AUTO_PROPERTY_DECL(bool, displayPopulationPrediction, DisplayPopulationPrediction)

    AUTO_PROPERTY_DECL(bool, presentAprioriPrediction, PresentAprioriPrediction)
    AUTO_PROPERTY_DECL(bool, displayAprioriPrediction, DisplayAprioriPrediction)

    AUTO_PROPERTY_DECL(bool, presentAposterioriPrediction, PresentAposterioriPrediction)
    AUTO_PROPERTY_DECL(bool, displayAposterioriPrediction, DisplayAposterioriPrediction)

    AUTO_PROPERTY_DECL(bool, presentPopulationPercentiles, PresentPopulationPercentiles)
    AUTO_PROPERTY_DECL(bool, displayPopulationPercentiles, DisplayPopulationPercentiles)

    AUTO_PROPERTY_DECL(bool, presentAprioriPercentiles, PresentAprioriPercentiles)
    AUTO_PROPERTY_DECL(bool, displayAprioriPercentiles, DisplayAprioriPercentiles)

    AUTO_PROPERTY_DECL(bool, presentAposterioriPercentiles, PresentAposterioriPercentiles)
    AUTO_PROPERTY_DECL(bool, displayAposterioriPercentiles, DisplayAposterioriPercentiles)

    AUTO_PROPERTY_DECL(bool, presentPossibleAdjustments, PresentPossibleAdjustments)
    AUTO_PROPERTY_DECL(bool, displayPossibleAdjustments, DisplayPossibleAdjustments)

    AUTO_PROPERTY_DECL(bool, presentSelectedAdjustment, PresentSelectedAdjustment)
    AUTO_PROPERTY_DECL(bool, displaySelectedAdjustment, DisplaySelectedAdjustment)

    AUTO_PROPERTY_DECL(bool, presentMeasures, PresentMeasures)
    AUTO_PROPERTY_DECL(bool, displayMeasures, DisplayMeasures)

    AUTO_PROPERTY_DECL(bool, presentTargets, PresentTargets)
    AUTO_PROPERTY_DECL(bool, displayTargets, DisplayTargets)

    public:
        void setCurrentTab(StepType::Enum step);
        Q_INVOKABLE void setVisible(CurveType::Enum curveType, bool isVisible);
        Q_INVOKABLE void setVisible(StepType::Enum stepType, CurveType::Enum curveType, bool isVisible);

    protected:
        Q_INVOKABLE GraphInformationSelection(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr);

    private:
        void initStep(StepType::Enum stepType, bool curveInfo[CurveType::size]);
		void updateProperties();

	private:
        GraphInfo _graphInfo[StepType::size][CurveType::size];
        StepType::Enum _currentStep;
};

#endif // GRAPHINFORMATIONSELECTION_H
