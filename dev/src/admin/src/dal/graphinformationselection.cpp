
#include "graphinformationselection.h"
#include "core/settings.h"

GraphInformationSelection::GraphInformationSelection(ezechiel::GuiCore::AbstractRepository *repository, QObject *parent) :
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

    _displayCurrentTime = true;
    _displayCovariateChange = true;

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


    QMap<QString, QVariant> displayParameters = SETTINGS.get(ezechiel::GuiCore::Module::GUI, "GraphDisplayParameters").toMap();
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

    QList<QVariant> percentile = SETTINGS.get(ezechiel::GuiCore::Module::GUI, "PercentilesDisplayParameters").toList();
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

    QMap<QString, QVariant> generalParameters = SETTINGS.get(ezechiel::GuiCore::Module::GUI, "GeneralDisplayParameters").toMap();
    _displayCurrentTime = generalParameters.value("displayCurrentTime", true).toBool();
    _displayCovariateChange = generalParameters.value("displayCovariateChange", true).toBool();
    _displayLiveAnnotations = generalParameters.value("displayLiveAnnotations", true).toBool();

}

void GraphInformationSelection::setCurrentTab(StepType::Enum step)
{
    _currentStep = step;
	updateProperties();
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
    QMap<QString, QVariant> general;
    general["displayCurrentTime"] = _displayCurrentTime;
    general["displayCovariateChange"] = _displayCovariateChange;
    general["displayLiveAnnotations"] = _displayLiveAnnotations;


    percentiles.append(_perc50);
    percentiles.append(_perc25_75);
    percentiles.append(_perc10_90);
    percentiles.append(_perc5_95);

    for (int i=CurveType::first; i<=CurveType::last; i++)
    {
        parametersTypeList.append(_graphInfo[_currentStep][i].available);
    }

    _parametersSettingsMap[currentTab] = parametersTypeList;


    SETTINGS.set(ezechiel::GuiCore::Module::GUI, "GraphDisplayParameters" ,_parametersSettingsMap);

    SETTINGS.set(ezechiel::GuiCore::Module::GUI, "PercentilesDisplayParameters" ,percentiles);

    SETTINGS.set(ezechiel::GuiCore::Module::GUI, "GeneralDisplayParameters", general);
}

void GraphInformationSelection::setPercentile(PercentileRangeEnum percentileRange, bool isAvailable){

    switch (percentileRange) {

    case Perc50:
        _perc50 = isAvailable;
        break;

    case Perc5_95:
        _perc5_95 = isAvailable;
        break;

    case Perc10_90:
        _perc10_90 = isAvailable;
        break;

    case Perc25_75:
        _perc25_75 = isAvailable;
        break;
    }

    updateProperties();
}

void GraphInformationSelection::updateProperties()
{
    _displayPopulationPrediction = _graphInfo[_currentStep][CurveType::PopulationPrediction].available;

    _displayPopulationPercentiles = _graphInfo[_currentStep][CurveType::PopulationPercentiles].available;

    _displayAprioriPrediction = _graphInfo[_currentStep][CurveType::APrioriPrediction].available;

    _displayAprioriPercentiles = _graphInfo[_currentStep][CurveType::AprioriPercentiles].available;

    _displayAposterioriPrediction = _graphInfo[_currentStep][CurveType::APosterioriPrediction].available;

    _displayAposterioriPercentiles = _graphInfo[_currentStep][CurveType::APosterioriPercentiles].available;

    _displayMeasures = _graphInfo[_currentStep][CurveType::Measure].available;

    _displayPossibleAdjustments = _graphInfo[_currentStep][CurveType::PossibleAdjustments].available;

    _displaySelectedAdjustment = _graphInfo[_currentStep][CurveType::SelectedAdjustments].available;

    _displayTargets = _graphInfo[_currentStep][CurveType::Target].available;

    setModified(true);
}

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayPopulationPrediction, DisplayPopulationPrediction)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayAprioriPrediction, DisplayAprioriPrediction)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayAposterioriPrediction, DisplayAposterioriPrediction)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayPopulationPercentiles, DisplayPopulationPercentiles)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayAprioriPercentiles, DisplayAprioriPercentiles)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayAposterioriPercentiles, DisplayAposterioriPercentiles)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayPossibleAdjustments, DisplayPossibleAdjustments)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displaySelectedAdjustment, DisplaySelectedAdjustment)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayMeasures, DisplayMeasures)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayTargets, DisplayTargets)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, perc5_95, Perc5_95)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, perc10_90, Perc10_90)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, perc25_75, Perc25_75)
AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, perc50, Perc50)

AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayLiveAnnotations, DisplayLiveAnnotations)


//AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayCurrentTime, DisplayCurrentTime)

// We write the two functions here instead of using the macros, to add
// updateProperties() in case of modification
bool GraphInformationSelection::getDisplayCurrentTime() const { return _displayCurrentTime; }
void GraphInformationSelection::setDisplayCurrentTime(bool value) {
    if (_displayCurrentTime == value) return;
    _displayCurrentTime = value;
    _isModified = true;
    emit displayCurrentTimeChanged(value);
    updateProperties();
}

//AUTO_PROPERTY_IMPL(GraphInformationSelection, bool, displayCovariateChange, DisplayCovariateChange)

// We write the two functions here instead of using the macros, to add
// updateProperties() in case of modification
bool GraphInformationSelection::getDisplayCovariateChange() const { return _displayCovariateChange; }
void GraphInformationSelection::setDisplayCovariateChange(bool value) {
    if (_displayCovariateChange == value) return;
    _displayCovariateChange = value;
    _isModified = true;
    emit displayCovariateChangeChanged(value);
    updateProperties();
}
