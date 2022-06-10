#include "admin/src/dal/practician.h"
#include "core/interfaces/abstractrepository.h"
#include "../adminfactory.h"
#include "core/settings.h"

Practician::Practician(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _externalId(),
    _title(),
    _role(),
    _person_id(-1),
    _institute_id(-1)
{
    _institute = AdminFactory::createEntity<Institute>(repository);
    _person = AdminFactory::createEntity<Person>(repository);
}

Practician::Practician(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository,parent),
    _externalId(),
    _title(),
    _role(),
    _person_id(-1),
    _institute_id(-1)
{
    SharedInstitute _ins = AdminFactory::createEntity<Institute>(repository);
    institute(_ins);
    SharedPerson _pers = AdminFactory::createEntity<Person>(repository);
    person(_pers);
}

/*
void Practician::copyTo(Practician *other)
{
    other->externalId(_externalId);
    other->title(_title);
    other->role(_role);
    other->person_id(_person_id);
    other->institute_id(_institute_id);
    _person->copyTo(other->person());
}
*/

PracticianSet::PracticianSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent, const PracticianSet* &other)
{
    foreach (SharedPractician _p, *other) {
        SharedPractician _np = SharedPractician(new Practician(repository, parent));
        _np->setId(_p->id());
        _np->role(_p->role());
        _np->person_id(_p->person_id());
        _np->institute_id(_p->institute_id());
        _np->externalId(_p->externalId());
        _np->title(_p->title());
        this->append(_np);
    }
}
