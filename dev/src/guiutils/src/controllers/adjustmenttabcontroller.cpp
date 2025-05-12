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

using namespace Tucuxi::Gui::GuiUtils;

STD_PROPERTY_IMPL(Tucuxi::Gui::GuiUtils::AdjustmentTabController, Tucuxi::Gui::Core::DosageHistory*, adjustments, Adjustments)
STD_PROPERTY_IMPL(Tucuxi::Gui::GuiUtils::AdjustmentTabController, bool, isManualAdjustmentDefined, IsManualAdjustmentDefined)

static const QString MANUAL = "Manual";
static const QString SUGGESTED = "Suggested";

Tucuxi::Gui::GuiUtils::AdjustmentTabController::AdjustmentTabController(QObject *parent) : AbstractViewController(parent),
    _isManualAdjustmentDefined(false)
{
    _adjustments = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DosageHistory>(ABSTRACTREPO, this);
}

QDateTime Tucuxi::Gui::GuiUtils::AdjustmentTabController::getAdjustmentDate() const
{
    return masterController->getPredictionSpec()->getAdjustmentDate();
}


void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setAdjustmentDate(QDateTime date)
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

bool Tucuxi::Gui::GuiUtils::AdjustmentTabController::getAdjustmentWithLoadingDose() const
{
    return masterController->getInterpretation()->getAdjustmentWithLoadingDose();
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setAdjustmentWithLoadingDose(bool value)
{
    if (value == masterController->getInterpretation()->getAdjustmentWithLoadingDose()) {
        return;
    }
    masterController->getPredictionSpec()->setAdjustmentWithLoadingDose(value);
    masterController->getInterpretation()->setAdjustmentWithLoadingDose(value);
    masterController->adjustmentSettingsUpdated();
}

bool Tucuxi::Gui::GuiUtils::AdjustmentTabController::getAdjustmentWithRestPeriod() const
{
    return masterController->getInterpretation()->getAdjustmentWithRestPeriod();
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setAdjustmentWithRestPeriod(bool value)
{
    if (value == masterController->getInterpretation()->getAdjustmentWithRestPeriod()) {
        return;
    }
    masterController->getPredictionSpec()->setAdjustmentWithRestPeriod(value);
    masterController->getInterpretation()->setAdjustmentWithRestPeriod(value);
    masterController->adjustmentSettingsUpdated();
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setAdjustmentDateFromMaster(QDateTime date)
{
    // Round it at the minute
    date = date.fromMSecsSinceEpoch(60000*(date.toMSecsSinceEpoch()/60000));

    if (date == getAdjustmentDate()) return;

    masterController->getPredictionSpec()->setAdjustmentDate(date);
    masterController->getInterpretation()->setAdjustmentDate(date);
    masterController->getInterpretation()->setAdjustmentDateIsSet(true);
    emit adjustmentDateChanged(date);
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setInitialAdjustmentDateFromMaster(QDateTime date)
{
    // Round it at the minute
    date = date.fromMSecsSinceEpoch(60000*(date.toMSecsSinceEpoch()/60000));

    masterController->getPredictionSpec()->setAdjustmentDate(date);
    masterController->getInterpretation()->setAdjustmentDate(date);
    masterController->getInterpretation()->setAdjustmentDateIsSet(true);
    emit adjustmentDateChanged(date);
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setAppliedTime(int index, QDateTime time)
{
    if (!isIndexValid(index)) {
        return;
    }

    Tucuxi::Gui::Core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getApplied() == time) {
        return;
    }

    adjustment->setApplied(time);
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setEndTime(int index, QDateTime time)
{
    if (!isIndexValid(index)) {
        return;
    }

    Tucuxi::Gui::Core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getEndTime() == time) {
        return;
    }

    adjustment->setEndTime(time);
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setDbValue(int index, double value)
{
    if (!isIndexValid(index)) {
        return;
    }

    Tucuxi::Gui::Core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getQuantity()->getDbvalue() == value) {
        return;
    }

    adjustment->getQuantity()->setDbvalue(value);
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setDbTinf(int index, double value)
{
    if (!isIndexValid(index)) {
        return;
    }

    // We set the infusion time in minutes
    Tucuxi::Gui::Core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getDbtinf() == value) {
        return;
    }

    adjustment->setDbtinf(value);
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setDbInterval(int index, double interval)
{
    if (!isIndexValid(index)) return;

    // We set the interval in hours
    Tucuxi::Gui::Core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getInterval() == Tucuxi::Gui::Core::Duration(interval)) {
        return;
    }

    adjustment->setDbinterval(interval);
}

/*
void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setRouteValue(int index, int routeValue)
{
    if (!isIndexValid(index)) {
        return;
    }
    Tucuxi::Gui::Core::ADME* adme = masterController->getInterpretation()->getDrugResponseAnalysis()->getDrugModel()->getAdme();
    Tucuxi::Gui::Core::DMAdmin::Route route = adme->getIntakes()->at(routeValue)->getRoute();

    Tucuxi::Gui::Core::Dosage* adjustment = _adjustments->at(index);
    if (adjustment->getRoute()->getRoute() == route) {
        return;
    }

    adjustment->getRoute()->setRoute(route);
    adjustment->getRoute()->setFormulationAndRoute(adme->getIntakes()->at(routeValue)->getFormulationAndRoute());
}
*/

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::addAdjustment()
{    
    Tucuxi::Gui::Core::DrugModel *drugModel;
    drugModel = masterController->getInterpretation()->getDrugResponseAnalysis()->getDrugModel();

    Tucuxi::Gui::Core::AdjustmentDosage* adjustment = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::AdjustmentDosage>(ABSTRACTREPO, _adjustments);
    adjustment->setApplied(getAdjustmentDate());
    adjustment->setEndTime(getAdjustmentDate().addDays(1));
//    adjustment->getRoute()->setRoute(drugModel->getAdme()->getDefaultIntake()->getRoute());
    adjustment->getRoute()->setFormulationAndRoute(drugModel->getAdme()->getDefaultIntake()->getFormulationAndRoute().getTreatmentFormulationAndRoute());
    adjustment->getRoute()->setDescription(drugModel->getAdme()->getDefaultIntake()->getDescription());
    adjustment->getQuantity()->setDbvalue(drugModel->getDoses()->getQuantity()->value());
    adjustment->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit("mg"));
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

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::removeAdjustment(int index)
{
    if (!isIndexValid(index)) {
        return;
    }

    if (getIsManualAdjustmentDefined()) {
        if (static_cast<Tucuxi::Gui::Core::AdjustmentDosage*>(_adjustments->at(index))->getType() == MANUAL) {
            setIsManualAdjustmentDefined(false);
        }
    }

    _adjustments->remove(index);
    forceRefresh();
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::selectAdjustment(int index)
{
    Tucuxi::Gui::Core::Adjustments *adjustments = _chartData->getRevPred()->getAdjustments();
    Q_ASSERT(adjustments->size() > index);
    if (adjustments->size() <= index) {
        return;
    }    


    removeFromTreatement(SUGGESTED);

    for (int i = 0; i < adjustments->at(index)->getDosageHistory()->size(); i++) {
        Tucuxi::Gui::Core::Dosage* suggestedAdjustment = adjustments->at(index)->getDosageHistory()->at(i);

        Tucuxi::Gui::Core::AdjustmentDosage* adjustment = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::AdjustmentDosage>(ABSTRACTREPO, _adjustments);
        adjustment->setApplied(suggestedAdjustment->getApplied());
        adjustment->setEndTime(suggestedAdjustment->getEndTime());
//        adjustment->getRoute()->setRoute(suggestedAdjustment->getRoute()->getRoute());
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

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::forceRefresh()
{
    adaptDates(getAdjustment(MANUAL), getLastAdjustment(SUGGESTED));
    _adjustments->sort(compareAdjustment);
    masterController->adjustmentUpdated();
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::setChartData(Tucuxi::Gui::Core::ChartData *chartData)
{
    _chartData = chartData;
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::onDosageUpdated()
{
    // Check if we need to update the adjustment date
    QDateTime newAdjustmentDate;
    Tucuxi::Gui::Core::DosageHistory* dosageHistory = masterController->getInterpretation()->getDrugResponseAnalysis()->getTreatment()->getDosages();
    if (dosageHistory->getNextIntake(QDateTime::currentDateTime(), newAdjustmentDate)) {
        setAdjustmentDate(newAdjustmentDate);
    }
    reset();
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::reset()
{
    _adjustments->clear();
    setIsManualAdjustmentDefined(false);
}

bool Tucuxi::Gui::GuiUtils::AdjustmentTabController::isIndexValid(int index)
{
    return (index >= 0) && (index < _adjustments->size());
}

void Tucuxi::Gui::GuiUtils::AdjustmentTabController::removeFromTreatement(const QString &type)
{
    for (int i=0; i<_adjustments->size(); i++){
        if (static_cast<Tucuxi::Gui::Core::AdjustmentDosage*>(_adjustments->at(i))->getType() == type) {
            _adjustments->remove(i);
            if (i >= 0) {
                i --;
            }
        }
    }
}

bool Tucuxi::Gui::GuiUtils::AdjustmentTabController::compareAdjustment(const Tucuxi::Gui::Core::Dosage* a, const Tucuxi::Gui::Core::Dosage* b)
{
    return (a->getApplied() < b->getApplied());
}

Tucuxi::Gui::Core::AdjustmentDosage* Tucuxi::Gui::GuiUtils::AdjustmentTabController::getAdjustment(const QString &type)
{
    for (int i=0; i<_adjustments->size(); i++) {
        Tucuxi::Gui::Core::AdjustmentDosage* adj = static_cast<Tucuxi::Gui::Core::AdjustmentDosage*>(_adjustments->at(i));
        if (adj->getType() == type) {
            return adj;
        }
    }
    return nullptr;
}


Tucuxi::Gui::Core::AdjustmentDosage* Tucuxi::Gui::GuiUtils::AdjustmentTabController::getLastAdjustment(const QString &type)
{
    for (int i=_adjustments->size() - 1; i >= 0; i--) {
        Tucuxi::Gui::Core::AdjustmentDosage* adj = static_cast<Tucuxi::Gui::Core::AdjustmentDosage*>(_adjustments->at(i));
        if (adj->getType() == type) {
            return adj;
        }
    }
    return nullptr;
}


void Tucuxi::Gui::GuiUtils::AdjustmentTabController::adaptDates(const Tucuxi::Gui::Core::AdjustmentDosage *manual, Tucuxi::Gui::Core::AdjustmentDosage *suggested)
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
