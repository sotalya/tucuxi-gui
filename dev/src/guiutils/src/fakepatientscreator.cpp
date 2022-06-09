//@@license@@

#include "fakepatientscreator.h"

#include "core/dal/corepatient.h"
#include "admin/src/dal/patient.h"
#include "admin/src/adminfactory.h"

FakePatientsCreator::FakePatientsCreator()
{

}


void FakePatientsCreator::createFakePatients(ezechiel::GuiCore::CorePatientList* patientsList)
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
