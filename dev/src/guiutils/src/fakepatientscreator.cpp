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


#include "fakepatientscreator.h"

#include "core/dal/corepatient.h"
#include "admin/src/dal/patient.h"
#include "admin/src/adminfactory.h"

using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::GuiUtils;

FakePatientsCreator::FakePatientsCreator()
{

}


void FakePatientsCreator::createFakePatients(Tucuxi::Gui::Core::CorePatientList* patientsList)
{
    patientsList->clear();
    // Constants //////////////////////////////////////////////////////////////
    const int nbFakePatients = 1;
    QList<QString> names;
    names.append("Demo Patient John");
    names.append("Demo Patient Jane");

    // Patients ///////////////////////////////////////////////////////////////
    SharedPatient patient;
    for (int i = 0; i < nbFakePatients; ++i) {
        patient = AdminFactory::createEntity<Patient>(ABSTRACTREPO, patientsList);
        static_cast<Patient*>(patient)->externalId(QString("P%1").arg(i + 1, 3, 10, QChar('0')));
        static_cast<Patient*>(patient)->stayNumber(QString("S%1").arg(i + 1, 3, 10, QChar('0')));
        static_cast<Patient*>(patient)->statOk(i % 2);
        static_cast<Patient*>(patient)->person()->firstname(names[i]);
        static_cast<Patient*>(patient)->person()->name(QString("Doe"));
        static_cast<Patient*>(patient)->person()->gender(Person::GenderType((i + 1) % 2));
        static_cast<Patient*>(patient)->person()->birthday(QDate(1960, 1, i + 1));
        static_cast<Patient*>(patient)->person()->location()->address(QString("Avenue de Lausanne 1"));
        static_cast<Patient*>(patient)->person()->location()->postcode(QString("1000"));
        static_cast<Patient*>(patient)->person()->location()->city("Genève");
        static_cast<Patient*>(patient)->person()->location()->state("GE");
        static_cast<Patient*>(patient)->person()->location()->country("Switzerland");



        patientsList->append(patient);
    }
    // ////////////////////////////////////////////////////////////////////////


}
