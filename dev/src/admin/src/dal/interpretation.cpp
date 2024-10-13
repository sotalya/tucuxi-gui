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


#include "admin/src/dal/interpretation.h"

#include "interpretationrequest.h"
#include "interpretationanalysis.h"
#include "core/corefactory.h"
#include "core/dal/dosage.h"
#include "core/dal/drugresponseanalysis.h"
#include "admin/src/dal/validationstatus.h"
#include "admin/src/dal/practician.h"

using namespace Tucuxi::Gui::Admin;

Interpretation::Interpretation(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent), _intepretationType(InterpretationType::Undefined)
{
    setId(-1);
    _request = Tucuxi::Gui::Core::CoreFactory::createEntity<InterpretationRequest>(repository, this);
    _analysis = Tucuxi::Gui::Core::CoreFactory::createEntity<InterpretationAnalysis>(repository, this);
    _drugResponseAnalysis = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugResponseAnalysis>(repository, this);
    _analyst = Tucuxi::Gui::Core::CoreFactory::createEntity<Practician>(repository, this);
    _validationStatus = Tucuxi::Gui::Core::CoreFactory::createEntity<ValidationStatus>(repository, this);
    _selectedAdjustment = nullptr;
    _adjustmentDateIsSet = false;
    _adjustmentWithRestPeriod = false;
    _adjustmentWithLoadingDose = false;
}

AUTO_PROPERTY_IMPL(Interpretation, Interpretation::InterpretationType, intepretationType, InterpretationType)
AUTO_PROPERTY_IMPL(Interpretation, Tucuxi::Gui::Admin::InterpretationAnalysis*, analysis, Analysis)
AUTO_PROPERTY_IMPL(Interpretation, Tucuxi::Gui::Admin::InterpretationRequest*, request, Request)
AUTO_PROPERTY_IMPL(Interpretation, Tucuxi::Gui::Core::DrugResponseAnalysis*, drugResponseAnalysis, DrugResponseAnalysis)

AUTO_PROPERTY_IMPL(Interpretation, Tucuxi::Gui::Admin::Practician*, analyst, Analyst)
AUTO_PROPERTY_IMPL(Interpretation, Tucuxi::Gui::Admin::ValidationStatus*, validationStatus, ValidationStatus)


AUTO_PROPERTY_IMPL(Interpretation, bool, adjustmentDateIsSet, AdjustmentDateIsSet)
AUTO_PROPERTY_IMPL(Interpretation, bool, adjustmentWithLoadingDose, AdjustmentWithLoadingDose)
AUTO_PROPERTY_IMPL(Interpretation, bool, adjustmentWithRestPeriod, AdjustmentWithRestPeriod)
AUTO_PROPERTY_IMPL(Interpretation, QDateTime, adjustmentDate, AdjustmentDate)
AUTO_PROPERTY_IMPL(Interpretation, Tucuxi::Gui::Core::AdjustmentDosage*, selectedAdjustment, SelectedAdjustment)
AUTO_PROPERTY_IMPL(Interpretation, QDateTime, startInterpretationTime, StartInterpretationTime)
AUTO_PROPERTY_IMPL(Interpretation, QDateTime, validateInterpretationTime, ValidateInterpretationTime)

bool Interpretation::justForDebuggingPurpose()
{
    return true;
}
