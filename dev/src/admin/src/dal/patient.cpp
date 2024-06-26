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
