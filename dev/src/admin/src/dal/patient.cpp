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


#include "admin/src/dal/patient.h"
#include "../adminfactory.h"

using namespace Tucuxi::Gui::Admin;

Patient::Patient(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    CorePatient(repository,parent),
    _person_id(-1),
    _externalId(),
    _stayNumber(),
    _statOk()
{
    _person = AdminFactory::createEntity<Person>(repository);
//    SharedPerson _pers = AdminFactory::createEntity<Person>(repository);
//    _pers->setName("testyjo");
//    setPerson(_pers.data());
}

Patient::Patient(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent) :
    CorePatient(repository,parent),
    _person_id(-1),
    _externalId(),
    _stayNumber(),
    _statOk()
{
    SharedPerson _pers = AdminFactory::createEntity<Person>(repository);
    person(_pers);
}

//QML_POINTERLIST_CLASS_IMPL(PatientList, Patient)
