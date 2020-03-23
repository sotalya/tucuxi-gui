#include "partialrequest.h"
#include "../adminfactory.h"
#include "core/corefactory.h"
#include "core/dal/drug/drug.h"

PartialRequest::PartialRequest(ezechiel::core::AbstractRepository *repository, QObject *parent) :
    Entity(repository, parent)
{
    setId(-1);
    _patient = AdminFactory::createEntity<Patient>(repository);
    _practician = AdminFactory::createEntity<Practician>(repository);
    _institute = AdminFactory::createEntity<Institute>(repository);
    _sample = AdminFactory::createEntity<Measure>(repository);
}

PartialRequest::PartialRequest(ezechiel::core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository, id, parent)
{
    setId(id);
}

PartialRequestSet::PartialRequestSet(ezechiel::core::AbstractRepository *repository, QObject *parent, const PartialRequestSet* &other)
{
    foreach (SharedPartialRequest _pr, *other) {
        SharedPartialRequest _npr = SharedPartialRequest(new PartialRequest(repository, parent));
        _npr->requestId(_pr->requestId());
        _npr->requestCpcl(_pr->requestCpcl());
        _npr->patient(_pr->patient());
        _npr->practician(_pr->practician());
        _npr->institute(_pr->institute());
        _npr->drug(_pr->drug());
        _npr->sample(_pr->sample());
        this->append(_npr);
    }
}
