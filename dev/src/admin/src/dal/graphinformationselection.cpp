
#include "graphinformationselection.h"
#include "core/settings.h"

GraphInformationSelection::GraphInformationSelection(ezechiel::core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _currentStep(StepType::Patient)
{
	// Initialize what curve is visible for each activity
    //                                        Population      Apriori    Aposteriori  Measure Target   Adjustment
    //                                       Pred   Perc   Pred   Perc   Pred   Perc                 List  Selected
    bool patientInfo[CurveType::size] =    { false, false, false, false, false, false, false, false, false, false };
    bool drugInfo[CurveType::size] =       { false, false, false, false, false, false, false, false, false, false };
    bool dosageInfo[CurveType::size] =     { true,  true,  false, false, false, false, false, false, false, false };
    bool covariateInfo[CurveType::size] =  { false, false, true,  true,  false, false, false, false, false, false };
    bool measureInfo[CurveType::size] =    { false, false, false, false, true,  true,  true,  false, false, false };
    bool targetInfo[CurveType::size] =     { false, false, false, false, true,  true,  true,  true,  false, false };
    bool adjustmentInfo[CurveType::size] = { false, false, false, false, true,  true,  true,  true,  true,  true  };
    bool validationInfo[CurveType::size] = { true,  true,  true,  true,  true,  true,  true,  true,  false, true  };
    bool reportInfo[CurveType::size] =     { true,  true,  true,  true,  true,  true,  true,  true,  false, true  };

    initStep(StepType::Patient, patientInfo);
    initStep(StepType::Drug, drugInfo);
    initStep(StepType::Dosage, dosageInfo);
    initStep(StepType::Covariate, covariateInfo);
    initStep(StepType::Measure, measureInfo);
    initStep(StepType::Target, targetInfo);
    initStep(StepType::Adjustment, adjustmentInfo);
    initStep(StepType::Validation, validationInfo);
    initStep(StepType::Report, reportInfo);

    loadDisplayParametersSettings();

}

QString GraphInformationSelection::getCurrentTab() const{

    static std::map<StepType::Enum, std::string> stringMap =
    {
        {StepType::Enum::Patient, "Patient"},
        {StepType::Enum::Drug, "Drug"},
        {StepType::Enum::Dosage, "Dosage"},
        {StepType::Enum::Covariate, "Covariate"},
        {StepType::Enum::Measure, "Measure"},
        {StepType::Enum::Target, "Target"},
        {StepType::Enum::Adjustment, "Adjustement"},
        {StepType::Enum::Validation, "Validation"},
        {StepType::Enum::Report, "Report"}
    };

    auto it = stringMap.find(_currentStep);
    if (it != stringMap.end()) {
        return QString::fromStdString(it->second);
    }

    return 0; // SHOULDN'T GO HERE
}

void GraphInformationSelection::loadDisplayParametersSettings(){


    QMap<QString, QVariant> displayParameters = SETTINGS.get(ezechiel::core::Module::GUI, "GraphDisplayParameters").toMap();
    QList<QVariant> listParametersValues;
    int curveIndex = 0;

    for (int stepIndex=0; stepIndex<=StepType::last; stepIndex++){
        QString qIndex = QString::fromStdString(std::to_string(stepIndex));

        if (displayParameters.contains(qIndex)){
            listParametersValues = displayParameters[qIndex].toList();
            for (const QVariant &value : listParametersValues){
                setAvailable(StepType::Enum(stepIndex),CurveType::Enum(curveIndex), value.toBool());
                curveIndex++;
            }
            curveIndex = 0;
        }
    }

    QList<QVariant> percentile = SETTINGS.get(ezechiel::core::Module::GUI, "GeneralDisplayParameters").toList();
    if (percentile.length() != 0){
        _perc50 = percentile[0].toBool();
        _perc25_75 = percentile[1].toBool();
        _perc10_90 = percentile[2].toBool();
        _perc5_95 = percentile[3].toBool();
    }
    else{
        _perc50 = true;
        _perc25_75 = true;
        _perc10_90 = true;
        _perc5_95 = true;
    }
}

void GraphInformationSelection::setCurrentTab(StepType::Enum step)
{
    _currentStep = step;
	updateProperties();
}

void GraphInformationSelection::setVisible(CurveType::Enum curveType, bool isVisible)
{
    setVisible(_currentStep, curveType, isVisible);
}

void GraphInformationSelection::setVisible(StepType::Enum stepType, CurveType::Enum curveType, bool isVisible)
{
    if (_graphInfo[stepType][curveType].visible != isVisible)
    {
        _graphInfo[stepType][curveType].visible = isVisible;
        if (stepType == _currentStep)
        {
            updateProperties();
        }
    }
}

void GraphInformationSelection::setAvailable(CurveType::Enum curveType, bool isAvailable)
{
    setAvailable(_currentStep, curveType, isAvailable);
}

void GraphInformationSelection::setAvailable(StepType::Enum stepType, CurveType::Enum curveType, bool isAvailable)
{
    if (_graphInfo[stepType][curveType].available != isAvailable)
    {
        _graphInfo[stepType][curveType].available = isAvailable;
        if (stepType == _currentStep)
        {
            updateProperties();
        }
    }
}

void GraphInformationSelection::initStep(StepType::Enum stepType, bool curveInfo[CurveType::size])
{
    for (int i=CurveType::first; i<=CurveType::last; i++)
    {
        _graphInfo[stepType][i].available = curveInfo[i];
        _graphInfo[stepType][i].visible = true;
    }
}

void GraphInformationSelection::saveSettings(){

    QString currentTab = QString::fromStdString(std::to_string(_currentStep));
    QList<QVariant> parametersTypeList;
    QList<QVariant> percentiles;

    percentiles.append(_perc50);
    percentiles.append(_perc25_75);
    percentiles.append(_perc10_90);
    percentiles.append(_perc5_95);

    for (int i=CurveType::first; i<=CurveType::last; i++)
    {
        parametersTypeList.append(_graphInfo[_currentStep][i].available);
    }

    _parametersSettingsMap[currentTab] = parametersTypeList;


    SETTINGS.set(ezechiel::core::Module::GUI, "GraphDisplayParameters" ,_parametersSettingsMap);

    SETTINGS.set(ezechiel::core::Module::GUI, "GeneralDisplayParameters" ,percentiles);
}

void GraphInformationSelection::setPercentile(bool percentile, PercentileRangeEnum percentileRange){

    switch (percentileRange) {

    case Perc50:
        _perc50 = percentile;
        break;

    case Perc5_95:
        _perc5_95 = percentile;
        break;

    case Perc10_90:
        _perc10_90 = percentile;
        break;

    case Perc25_75:
        _perc25_75 = percentile;
        break;
    }

    updateProperties();
}

void GraphInformationSelection::updateProperties()
{
    _presentPopulationPrediction = _graphInfo[_currentStep][CurveType::PopulationPrediction].available;
    _displayPopulationPrediction = _graphInfo[_currentStep][CurveType::PopulationPrediction].visible;

    _presentPopulationPercentiles = _graphInfo[_currentStep][CurveType::PopulationPercentiles].available;
    _displayPopulationPercentiles = _graphInfo[_currentStep][CurveType::PopulationPercentiles].visible;

    _presentAprioriPrediction = _graphInfo[_currentStep][CurveType::APrioriPrediction].available;
    _displayAprioriPrediction = _graphInfo[_currentStep][CurveType::APrioriPrediction].visible;

    _presentAprioriPercentiles = _graphInfo[_currentStep][CurveType::AprioriPercentiles].available;
    _displayAprioriPercentiles = _graphInfo[_currentStep][CurveType::AprioriPercentiles].visible;

    _presentAposterioriPrediction = _graphInfo[_currentStep][CurveType::APosterioriPrediction].available;
    _displayAposterioriPrediction = _graphInfo[_currentStep][CurveType::APosterioriPrediction].visible;

    _presentAposterioriPercentiles = _graphInfo[_currentStep][CurveType::APosterioriPercentiles].available;
    _displayAposterioriPercentiles = _graphInfo[_currentStep][CurveType::APosterioriPercentiles].visible;

    _presentMeasures = _graphInfo[_currentStep][CurveType::Measure].available;
    _displayMeasures = _graphInfo[_currentStep][CurveType::Measure].visible;

    _presentPossibleAdjustments = _graphInfo[_currentStep][CurveType::PossibleAdjustments].available;
    _displayPossibleAdjustments = _graphInfo[_currentStep][CurveType::PossibleAdjustments].visible;

    _presentSelectedAdjustment = _graphInfo[_currentStep][CurveType::SelectedAdjustments].available;
    _displaySelectedAdjustment = _graphInfo[_currentStep][CurveType::SelectedAdjustments].visible;

    _presentTargets = _graphInfo[_currentStep][CurveType::Target].available;
    _displayTargets = _graphInfo[_currentStep][CurveType::Target].visible;

    setModified(true);
}

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, presentPopulationPrediction, PresentPopulationPrediction)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayPopulationPrediction, DisplayPopulationPrediction)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, presentAprioriPrediction, PresentAprioriPrediction)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayAprioriPrediction, DisplayAprioriPrediction)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, presentAposterioriPrediction, PresentAposterioriPrediction)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayAposterioriPrediction, DisplayAposterioriPrediction)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, presentPopulationPercentiles, PresentPopulationPercentiles)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayPopulationPercentiles, DisplayPopulationPercentiles)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, presentAprioriPercentiles, PresentAprioriPercentiles)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayAprioriPercentiles, DisplayAprioriPercentiles)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, presentAposterioriPercentiles, PresentAposterioriPercentiles)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayAposterioriPercentiles, DisplayAposterioriPercentiles)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, presentPossibleAdjustments, PresentPossibleAdjustments)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayPossibleAdjustments, DisplayPossibleAdjustments)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, presentSelectedAdjustment, PresentSelectedAdjustment)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displaySelectedAdjustment, DisplaySelectedAdjustment)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, presentMeasures, PresentMeasures)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayMeasures, DisplayMeasures)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, presentTargets, PresentTargets)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayTargets, DisplayTargets)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, perc5_95, Perc5_95)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, perc10_90, Perc10_90)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, perc25_75, Perc25_75)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, perc50, Perc50)
