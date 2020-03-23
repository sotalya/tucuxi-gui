#include "admin/src/dal/practician.h"
#include "core/interfaces/abstractrepository.h"
#include "../adminfactory.h"

Practician::Practician(ezechiel::core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _externalId(),
    _person_id(-1),
    _institute_id(-1),
    _title()
{
    _institute = AdminFactory::createEntity<Institute>(repository);
    _person = AdminFactory::createEntity<Person>(repository);
}

Practician::Practician(ezechiel::core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository,parent),
    _externalId(),
    _person_id(-1),
    _institute_id(-1),
    _title()
{
    SharedInstitute _ins = AdminFactory::createEntity<Institute>(repository);
    institute(_ins);
    SharedPerson _pers = AdminFactory::createEntity<Person>(repository);
    person(_pers);
}

PracticianSet::PracticianSet(ezechiel::core::AbstractRepository *repository, QObject *parent, const PracticianSet* &other)
{
    foreach (SharedPractician _p, *other) {
        SharedPractician _np = SharedPractician(new Practician(repository, parent));
        _np->setId(_p->id());
        _np->person_id(_p->person_id());
        _np->institute_id(_p->institute_id());
        _np->externalId(_p->externalId());
        _np->title(_p->title());
        this->append(_np);
    }
}
