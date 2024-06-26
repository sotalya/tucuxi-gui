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


#include "dosagetabcontroller.h"
#include "interpretationcontroller.h"
#include "core/dal/dosage.h"
#include "core/utils/duration.h"
#include "core/corefactory.h"
#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drug/drug.h"
#include "core/dal/drug/doses.h"
#include "core/utils/units.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/standardtreatment.h"

using namespace Tucuxi::Gui::GuiUtils;

STD_PROPERTY_IMPL(Tucuxi::Gui::GuiUtils::DosageTabController, Tucuxi::Gui::Core::DosageHistory*, dosages, Dosages)

//STD_PROPERTY_IMPL(DosageTabController, Tucuxi::Gui::Core::DrugModel*, drugModel, DrugModel)

Tucuxi::Gui::GuiUtils::DosageTabController::DosageTabController(QObject *parent) : AbstractViewController(parent)
{
//    _drugModel = nullptr;
    _dosages = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DosageHistory>(ABSTRACTREPO, this);
}

bool Tucuxi::Gui::GuiUtils::DosageTabController::isIndexValid(int index)
{
    return (index >= 0) && (index < _dosages->size());
}

void Tucuxi::Gui::GuiUtils::DosageTabController::setAppliedTime(int index, QDateTime time)
{
    if (!isIndexValid(index)) return;

    Tucuxi::Gui::Core::Dosage *dosage = _dosages->at(index);
    if (dosage->getApplied() == time) return;

    if (time > dosage->getEndTime())
    {
        qint64 dt = dosage->getApplied().secsTo(time);
        dosage->setEndTime(dosage->getEndTime().addSecs(dt));
    }

    _dosages->at(index)->setApplied(time);

    // If this is the second dosage and the first one is a steady state,
    // then update the steady state end time
    if (index == 1) {
        if (_dosages->at(0)->getIsAtSteadyState()) {
            _dosages->at(0)->setEndTime(time);
        }
    }
}

void Tucuxi::Gui::GuiUtils::DosageTabController::setEndTime(int index, QDateTime time)
{
    if (!isIndexValid(index)) return;

    Tucuxi::Gui::Core::Dosage *dosage = _dosages->at(index);
    if (dosage->getEndTime() == time) return;

    if (time < dosage->getApplied())
    {
        qint64 dt = dosage->getEndTime().secsTo(time);
        dosage->setApplied(dosage->getApplied().addSecs(dt));
    }

    _dosages->at(index)->setEndTime(time);
}

void Tucuxi::Gui::GuiUtils::DosageTabController::setDbValue(int index, double value)
{
    if (!isIndexValid(index)) return;
    if (_dosages->at(index)->getQuantity()->getDbvalue() == value) return;

    _dosages->at(index)->getQuantity()->setDbvalue(value);
}


void Tucuxi::Gui::GuiUtils::DosageTabController::setDbTinf(int index, double value)
{
    if (!isIndexValid(index)) return;

    // We set the infusion time in minutes
    if ( _dosages->at(index)->getDbtinf() == value) return;

    _dosages->at(index)->setDbtinf(value);
}

void Tucuxi::Gui::GuiUtils::DosageTabController::setDbInterval(int index, double interval)
{
    if (!isIndexValid(index)) return;

    // We set the interval in hours
    if ( _dosages->at(index)->getInterval() == Tucuxi::Gui::Core::Duration(interval)) return;

    _dosages->at(index)->setDbinterval(interval);
}

int Tucuxi::Gui::GuiUtils::DosageTabController::getRelativeRouteValue(int index)
{
    if (!isIndexValid(index)) return 0;

    Tucuxi::Gui::Core::AdminList* adminList = masterController->getInterpretation()->getDrugResponseAnalysis()->getDrugModel()->getAdme()->getIntakes();
    Tucuxi::Gui::Core::Admin::Route route = _dosages->at(index)->getRoute()->getRoute();

    for (int i=0; i<adminList->size(); i++)
    {
        if (adminList->at(i)->getRoute() == route) return i;
    }

    return 0;
}

void Tucuxi::Gui::GuiUtils::DosageTabController::setRouteValue(int index, int routeValue)
{
    if (!isIndexValid(index)) return;

    Tucuxi::Gui::Core::Admin::Route route;
    Tucuxi::Gui::Core::AdminList* adminList = masterController->getInterpretation()->getDrugResponseAnalysis()->getDrugModel()->getAdme()->getIntakes();
    if (routeValue > adminList->size()) return;

    route = adminList->at(routeValue)->getRoute();
    if ( _dosages->at(index)->getRoute()->getRoute() == route) return;

    _dosages->at(index)->getRoute()->setRoute(route);
    _dosages->at(index)->getRoute()->setFormulationAndRoute(adminList->at(routeValue)->getFormulationAndRoute());
}

void Tucuxi::Gui::GuiUtils::DosageTabController::setHasEndDate(int index, bool hasEndDate)
{
    if (!isIndexValid(index)) return;
    _dosages->at(index)->setHasEndDate(hasEndDate);
}

void Tucuxi::Gui::GuiUtils::DosageTabController::setAtSteadyState(int index, bool isAtSteadyState, QDateTime lastDoseTime)
{
    if (!isIndexValid(index)) return;
    _dosages->at(index)->setIsAtSteadyState(isAtSteadyState);
    if (isAtSteadyState) {
        _dosages->at(index)->setApplied(lastDoseTime.addMonths(-2));
        _dosages->at(index)->setEndTime(lastDoseTime.addMonths(12));
        // Because of changing clock, the two next lines are better than the two previous
        // Well, that was true previously, but now it is not
//        _dosages->at(index)->setApplied(lastDoseTime.addSecs(-2*30*24*3600));
//        _dosages->at(index)->setEndTime(lastDoseTime.addSecs(12*30*24*3600));
        _dosages->at(index)->setSteadyStateLastDoseDate(lastDoseTime);
    }
}

void Tucuxi::Gui::GuiUtils::DosageTabController::forceRefresh()
{
    _dosages->sort(compareDosage);
    masterController->dosageUpdated();
    emit canHaveMoreDosagesChanged(getCanHaveMoreDosages());
}

void Tucuxi::Gui::GuiUtils::DosageTabController::addDosage()
{ 
    Tucuxi::Gui::Core::DrugModel *drugModel;
    drugModel = masterController->getInterpretation()->getDrugResponseAnalysis()->getDrugModel();

    Tucuxi::Gui::Core::Dosage* dosage = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Dosage>(ABSTRACTREPO, _dosages);
    dosage->getRoute()->setRoute(drugModel->getAdme()->getDefaultIntake()->getRoute());
    dosage->getRoute()->setFormulationAndRoute(drugModel->getAdme()->getDefaultIntake()->getFormulationAndRoute());
    dosage->getRoute()->setDescription(drugModel->getAdme()->getDefaultIntake()->getDescription());
    dosage->getQuantity()->setDbvalue(drugModel->getDoses()->getQuantity()->value());
    dosage->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit("mg"));
    dosage->setDbinterval(drugModel->getIntervals()->getQuantity()->value());


    if (drugModel->getInfusions()->getQuantity()->value() > 0.0)
    {
        // The Dbtinf expects minutes, so let's do a conversion here
        if (drugModel->getInfusions()->getQuantity()->getUnitstring() == "h") {
            dosage->setDbtinf(drugModel->getInfusions()->getQuantity()->value() * 60.0);
        }
        else {
            dosage->setDbtinf(drugModel->getInfusions()->getQuantity()->value());
        }
    }

    if (_dosages->size() > 0)
    {
        dosage->setApplied(_dosages->endTime());
        dosage->setEndTime(_dosages->endTime().addDays(1));
    }
    else
    {
        dosage->setApplied(QDateTime(QDate::currentDate(), QTime(8, 0)));
        if (drugModel->getStandardTreatment()->getIsFixedDuration()) {
            QDateTime date = QDateTime(QDate::currentDate(), QTime(8,0));
            date = date.addSecs(drugModel->getStandardTreatment()->getDuration().toSeconds());
            dosage->setEndTime(date);
            dosage->setIsAtSteadyState(false);
        }
        else {
            dosage->setEndTime(QDateTime(QDate::currentDate(), QTime(8, 0)).addDays(2));
        }
    }

    _dosages->append(dosage);
}


QDateTime Tucuxi::Gui::GuiUtils::DosageTabController::getNewEndTime(QDateTime start)
{
    Tucuxi::Gui::Core::DrugModel *drugModel;
    drugModel = masterController->getInterpretation()->getDrugResponseAnalysis()->getDrugModel();

    if (drugModel->getStandardTreatment()->getIsFixedDuration()) {
        QDateTime date = start.addSecs(drugModel->getStandardTreatment()->getDuration().toSeconds());
        return date;
    }
    return start;
}

void Tucuxi::Gui::GuiUtils::DosageTabController::removeDosage(int index)
{
    if (index >= _dosages->size()) return;
    _dosages->remove(index);
    forceRefresh();
}

bool Tucuxi::Gui::GuiUtils::DosageTabController::compareDosage(const Tucuxi::Gui::Core::Dosage* a, const Tucuxi::Gui::Core::Dosage* b)
{
    return (a->getApplied() < b->getApplied());
}

int Tucuxi::Gui::GuiUtils::DosageTabController::getNbDosages()
{
    return _dosages->getList().size();
}

bool Tucuxi::Gui::GuiUtils::DosageTabController::getCanHaveMoreDosages()
{
    return true;
    QList<Tucuxi::Gui::Core::Dosage*>::iterator it = _dosages->getList().begin();
    while (it != _dosages->getList().end()) {
        Tucuxi::Gui::Core::Dosage *dosage = *it;
        if (dosage->getIsAtSteadyState()) {
            return false;
        }
        it++;
    }
    return true;
}

void Tucuxi::Gui::GuiUtils::DosageTabController::reset()
{
    emit canHaveMoreDosagesChanged(getCanHaveMoreDosages());
}

bool Tucuxi::Gui::GuiUtils::DosageTabController::checkModifiedDates(int index, const QDateTime &appliedDate, const QDateTime &endDate)
{
    if (!isIndexValid(index)) return true;

    // We accept to modify the steady state first dosage
    if ((index == 1) && (_dosages->at(0)->getIsAtSteadyState())) {
        return true;
    }

    // Get the modified dosage
    Tucuxi::Gui::Core::Dosage *dosage = _dosages->at(index);

    // Check if dates overlap exising dosages
    QList<Tucuxi::Gui::Core::Dosage*>::iterator it = _dosages->getList().begin();
    while (it != _dosages->getList().end()) {
        Tucuxi::Gui::Core::Dosage *d = *it;
        if (d != dosage &&
            ((appliedDate > d->getApplied() && appliedDate < d->getEndTime()) ||  // new applied date is within the current dosage period
             (endDate > d->getApplied() && endDate < d->getEndTime()) ||          // new end date is within the current dosage period
             (appliedDate < d->getApplied() && endDate > d->getEndTime()))) {     // new dates totally overlap the current dosage period
            return false;
        }
        it++;
    }
    return true;
}
