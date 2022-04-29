//@@license@@

#include "adjustmenttabcontroller.h"
#include "interpretationcontroller.h"
#include "core/dal/dosage.h"
#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drug/drug.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/predictionspec.h"
#include "core/dal/chartdata.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/standardtreatment.h"

STD_PROPERTY_IMPL(AdjustmentTabController, ezechiel::core::DosageHistory*, adjustments, Adjustments)
STD_PROPERTY_IMPL(AdjustmentTabController, bool, isManualAdjustmentDefined, IsManualAdjustmentDefined)

static const QString MANUAL = "Manual";
static const QString SUGGESTED = "Suggested";

AdjustmentTabController::AdjustmentTabController(QObject *parent) : AbstractViewController(parent),
    _isManualAdjustmentDefined(false)
{
    _adjustments = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DosageHistory>(ABSTRACTREPO, this);
}

QDateTime AdjustmentTabController::getAdjustmentDate() const
{
    return masterController->getPredictionSpec()->getAdjustmentDate();
}


void AdjustmentTabController::setAdjustmentDate(QDateTime date)
{
    // Round it at the minute
    date = date.fromMSecsSinceEpoch(60000*(date.toMSecsSinceEpoch()/60000));

    if (date == getAdjustmentDate()) return;

    masterController->getPredictionSpec()->setAdjustmentDate(date);
    masterController->getInterpretation()->setAdjustmentDate(date);
    masterController->getInterpretation()->setAdjustmentDateIsSet(true);
    emit adjustmentDateChanged(date);

    masterController->adjustmentDateUpdated();
}

bool AdjustmentTabController::getAdjustmentWithLoadingDose() const
{
    return masterController->getInterpretation()->getAdjustmentWithLoadingDose();
}

void AdjustmentTabController::setAdjustmentWithLoadingDose(bool value)
{
    if (value == masterController->getInterpretation()->getAdjustmentWithLoadingDose()) {
        return;
    }
    masterController->getPredictionSpec()->setAdjustmentWithLoadingDose(value);
    masterController->getInterpretation()->setAdjustmentWithLoadingDose(value);
    masterController->adjustmentSettingsUpdated();
}

bool AdjustmentTabController::getAdjustmentWithRestPeriod() const
{
    return masterController->getInterpretation()->getAdjustmentWithRestPeriod();
}

void AdjustmentTabController::setAdjustmentWithRestPeriod(bool value)
{
    if (value == masterController->getInterpretation()->getAdjustmentWithRestPeriod()) {
        return;
    }
    masterController->getPredictionSpec()->setAdjustmentWithRestPeriod(value);
    masterController->getInterpretation()->setAdjustmentWithRestPeriod(value);
    masterController->adjustmentSettingsUpdated();
}

void AdjustmentTabController::setAdjustmentDateFromMaster(QDateTime date)
{
    // Round it at the minute
    date = date.fromMSecsSinceEpoch(60000*(date.toMSecsSinceEpoch()/60000));

    if (date == getAdjustmentDate()) return;

    masterController->getPredictionSpec()->setAdjustmentDate(date);
    masterController->getInterpretation()->setAdjustmentDate(date);
    masterController->getInterpretation()->setAdjustmentDateIsSet(true);
    emit adjustmentDateChanged(date);
}

void AdjustmentTabController::setInitialAdjustmentDateFromMaster(QDateTime date)
{
    // Round it at the minute
    date = date.fromMSecsSinceEpoch(60000*(date.toMSecsSinceEpoch()/60000));

    masterController->getPredictionSpec()->setAdjustmentDate(date);
    masterController->getInterpretation()->setAdjustmentDate(date);
    masterController->getInterpretation()->setAdjustmentDateIsSet(true);
    emit adjustmentDateChanged(date);
}

void AdjustmentTabController::setAppliedTime(int index, QDateTime time)
{
    if (!isIndexValid(index)) {
        return;
    }

    ezechiel::core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getApplied() == time) {
        return;
    }

    adjustment->setApplied(time);
}

void AdjustmentTabController::setEndTime(int index, QDateTime time)
{
    if (!isIndexValid(index)) {
        return;
    }

    ezechiel::core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getEndTime() == time) {
        return;
    }

    adjustment->setEndTime(time);
}

void AdjustmentTabController::setDbValue(int index, double value)
{
    if (!isIndexValid(index)) {
        return;
    }

    ezechiel::core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getQuantity()->getDbvalue() == value) {
        return;
    }

    adjustment->getQuantity()->setDbvalue(value);
}

void AdjustmentTabController::setDbTinf(int index, double value)
{
    if (!isIndexValid(index)) {
        return;
    }

    // We set the infusion time in minutes
    ezechiel::core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getDbtinf() == value) {
        return;
    }

    adjustment->setDbtinf(value);
}

void AdjustmentTabController::setDbInterval(int index, double interval)
{
    if (!isIndexValid(index)) return;

    // We set the interval in hours
    ezechiel::core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getInterval() == ezechiel::core::Duration(interval)) {
        return;
    }

    adjustment->setDbinterval(interval);
}

void AdjustmentTabController::setRouteValue(int index, int routeValue)
{
    if (!isIndexValid(index)) {
        return;
    }
    ezechiel::core::ADME* adme = masterController->getInterpretation()->getDrugResponseAnalysis()->getDrugModel()->getAdme();
    ezechiel::core::Admin::Route route = adme->getIntakes()->at(routeValue)->getRoute();

    ezechiel::core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getRoute()->getRoute() == route) {
        return;
    }

    adjustment->getRoute()->setRoute(route);
    adjustment->getRoute()->setFormulationAndRoute(adme->getIntakes()->at(routeValue)->getFormulationAndRoute());
}

void AdjustmentTabController::addAdjustment()
{    
    ezechiel::core::DrugModel *drugModel;
    drugModel = masterController->getInterpretation()->getDrugResponseAnalysis()->getDrugModel();

    ezechiel::core::AdjustmentDosage* adjustment = ezechiel::core::CoreFactory::createEntity<ezechiel::core::AdjustmentDosage>(ABSTRACTREPO, _adjustments);
    adjustment->setApplied(getAdjustmentDate());
    adjustment->setEndTime(getAdjustmentDate().addDays(1));
    adjustment->getRoute()->setRoute(drugModel->getAdme()->getDefaultIntake()->getRoute());
    adjustment->getRoute()->setFormulationAndRoute(drugModel->getAdme()->getDefaultIntake()->getFormulationAndRoute());
    adjustment->getRoute()->setDescription(drugModel->getAdme()->getDefaultIntake()->getDescription());
    adjustment->getQuantity()->setDbvalue(drugModel->getDoses()->getQuantity()->value());
    adjustment->getQuantity()->setUnit(ezechiel::core::Unit("mg"));
    adjustment->setDbinterval(drugModel->getIntervals()->getQuantity()->value());

    if (drugModel->getInfusions()->getQuantity()->value() > 0.0)
    {
        adjustment->setDbtinf(drugModel->getInfusions()->getQuantity()->value());
    }

    adjustment->setType(MANUAL);

    // An adjustment can not be at steady state
    adjustment->setIsAtSteadyState(false);

    removeFromTreatement(MANUAL);
    setIsManualAdjustmentDefined(true);
    _adjustments->append(adjustment);
    forceRefresh();
}

void AdjustmentTabController::removeAdjustment(int index)
{
    if (!isIndexValid(index)) {
        return;
    }

    if (getIsManualAdjustmentDefined()) {
        if (static_cast<ezechiel::core::AdjustmentDosage*>(_adjustments->at(index))->getType() == MANUAL) {
            setIsManualAdjustmentDefined(false);
        }
    }

    _adjustments->remove(index);
    forceRefresh();
}

void AdjustmentTabController::selectAdjustment(int index)
{
    ezechiel::core::Adjustments *adjustments = _chartData->getRevPred()->getAdjustments();
    Q_ASSERT(adjustments->size() > index);
    if (adjustments->size() <= index) {
        return;
    }    


    removeFromTreatement(SUGGESTED);

    for (int i = 0; i < adjustments->at(index)->getDosageHistory()->size(); i++) {
        ezechiel::core::Dosage* suggestedAdjustment = adjustments->at(index)->getDosageHistory()->at(i);

        ezechiel::core::AdjustmentDosage* adjustment = ezechiel::core::CoreFactory::createEntity<ezechiel::core::AdjustmentDosage>(ABSTRACTREPO, _adjustments);
        adjustment->setApplied(suggestedAdjustment->getApplied());
        adjustment->setEndTime(suggestedAdjustment->getEndTime());
        adjustment->getRoute()->setRoute(suggestedAdjustment->getRoute()->getRoute());
        adjustment->getRoute()->setFormulationAndRoute(suggestedAdjustment->getRoute()->getFormulationAndRoute());
        adjustment->getRoute()->setDescription(suggestedAdjustment->getRoute()->getDescription());
        adjustment->getQuantity()->setDbvalue(suggestedAdjustment->getQuantity()->value());
        adjustment->getQuantity()->setUnit(suggestedAdjustment->getQuantity()->unit());
        adjustment->setDbinterval(suggestedAdjustment->getDbinterval());
        adjustment->setDbtinf(suggestedAdjustment->getDbtinf());
        adjustment->setType(SUGGESTED);

        // An adjustment can not be at steady state
        adjustment->setIsAtSteadyState(false);

        _adjustments->append(adjustment);
    }
    forceRefresh();
}

void AdjustmentTabController::forceRefresh()
{
    adaptDates(getAdjustment(MANUAL), getLastAdjustment(SUGGESTED));
    _adjustments->sort(compareAdjustment);
    masterController->adjustmentUpdated();
}

void AdjustmentTabController::setChartData(ezechiel::core::ChartData *chartData)
{
    _chartData = chartData;
}

void AdjustmentTabController::onDosageUpdated()
{
    // Check if we need to update the adjustment date
    QDateTime newAdjustmentDate;
    ezechiel::core::DosageHistory* dosageHistory = masterController->getInterpretation()->getDrugResponseAnalysis()->getTreatment()->getDosages();
    if (dosageHistory->getNextIntake(QDateTime::currentDateTime(), newAdjustmentDate)) {
        setAdjustmentDate(newAdjustmentDate);
    }
    reset();
}

void AdjustmentTabController::reset()
{
    _adjustments->clear();
    setIsManualAdjustmentDefined(false);
}

bool AdjustmentTabController::isIndexValid(int index)
{
    return (index >= 0) && (index < _adjustments->size());
}

void AdjustmentTabController::removeFromTreatement(const QString &type)
{
    for (int i=0; i<_adjustments->size(); i++){
        if (static_cast<ezechiel::core::AdjustmentDosage*>(_adjustments->at(i))->getType() == type) {
            _adjustments->remove(i);
            if (i >= 0) {
                i --;
            }
        }
    }
}

bool AdjustmentTabController::compareAdjustment(const ezechiel::core::Dosage* a, const ezechiel::core::Dosage* b)
{
    return (a->getApplied() < b->getApplied());
}

ezechiel::core::AdjustmentDosage* AdjustmentTabController::getAdjustment(const QString &type)
{
    for (int i=0; i<_adjustments->size(); i++) {
        ezechiel::core::AdjustmentDosage* adj = static_cast<ezechiel::core::AdjustmentDosage*>(_adjustments->at(i));
        if (adj->getType() == type) {
            return adj;
        }
    }
    return nullptr;
}


ezechiel::core::AdjustmentDosage* AdjustmentTabController::getLastAdjustment(const QString &type)
{
    for (int i=_adjustments->size() - 1; i >= 0; i--) {
        ezechiel::core::AdjustmentDosage* adj = static_cast<ezechiel::core::AdjustmentDosage*>(_adjustments->at(i));
        if (adj->getType() == type) {
            return adj;
        }
    }
    return nullptr;
}


void AdjustmentTabController::adaptDates(const ezechiel::core::AdjustmentDosage *manual, ezechiel::core::AdjustmentDosage *suggested)
{
    if (suggested != nullptr) {
        QDateTime appliedDate = suggested->getApplied();
        if ((manual != nullptr) && (appliedDate < manual->getEndTime())) {
            appliedDate = manual->getEndTime();
        }
        suggested->setApplied(appliedDate);
        if (!masterController->getInterpretation()->getDrugResponseAnalysis()->getDrugModel()->getStandardTreatment()->getIsFixedDuration()) {
            suggested->setEndTime(appliedDate.addMonths(1));
        }
    }
}
