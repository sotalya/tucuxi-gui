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


#include "interpretationrequest.h"
#include "admin/src/adminfactory.h"
#include "core/corefactory.h"

#include "core/dal/drugtreatment.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/clinical.h"

using namespace Tucuxi::Gui::Admin;

AUTO_PROPERTY_IMPL(InterpretationRequest, Tucuxi::Gui::Core::DrugTreatment*, treatment, Treatment)
AUTO_PROPERTY_IMPL(InterpretationRequest, Tucuxi::Gui::Admin::Patient*, patient, Patient)
AUTO_PROPERTY_IMPL(InterpretationRequest, Practician*, practician, Practician)

AUTO_PROPERTY_IMPL(InterpretationRequest, QString, comedication, Comedication)
AUTO_PROPERTY_IMPL(InterpretationRequest, QString, diagnosis, Diagnosis)
AUTO_PROPERTY_IMPL(InterpretationRequest, QString, indication, Indication)
AUTO_PROPERTY_IMPL(InterpretationRequest, QString, response, Response)
AUTO_PROPERTY_IMPL(InterpretationRequest, QString, toxicity, Toxicity)
AUTO_PROPERTY_IMPL(InterpretationRequest, QString, source, Source)
AUTO_PROPERTY_IMPL(InterpretationRequest, ClinicalSet*, clinicals, Clinicals)


InterpretationRequest::InterpretationRequest(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent)
{
    setId(-1);
    _patient = AdminFactory::createEntity<Patient>(repository, this);
    _practician = AdminFactory::createEntity<Practician>(repository, this);
    _treatment = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugTreatment>(repository, this);
    _clinicals = AdminFactory::createEntity<ClinicalSet>(repository, this);
}

InterpretationRequest::InterpretationRequest(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository,id, parent)
{
    setId(id);
}
