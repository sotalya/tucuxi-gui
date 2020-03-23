#include "institute.h"
#include "../adminfactory.h"

Institute::Institute(ezechiel::core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _location_id(-1),
    _name(),
    _externalId(),
    _logo(),
    _location(),
    _phones(),
    _emails()
{
    _location = AdminFactory::createEntity<Location>(repository);
}

Institute::Institute(ezechiel::core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository,id, parent),
    _location_id(-1),
    _name(),
    _externalId(),
    _logo(),
    _location(),
    _phones(),
    _emails()
{
    Location* _loc = AdminFactory::createEntity<Location>(repository);
    location(_loc);
}

//QString Institute::name() const
//{
//    return _name;
//}

//void Institute::setName(const QString &name)
//{
//    _name = name;
//    emit nameChanged(name);
//}

//QString Institute::externalId() const
//{
//    return _externalId;
//}

//void Institute::setExternalId(const QString &id)
//{
//    _externalId = id;
//    emit externalIdChanged(id);
//}

//QImage Institute::logo() const
//{
//    return _logo;
//}

//void Institute::setLogo(const QImage &logo)
//{
//    _logo = logo;
//    emit logoChanged(logo);
//}

//SharedLocation Institute::location() const
//{
//    return _location;
//}

//void Institute::setLocation(const SharedLocation &location)
//{
//    _location = location;
//    emit locationChanged(location);
//}

QList<SharedPhone> Institute::phones() const
{
    return _phones;
}

void Institute::setPhones(const QList<SharedPhone> &phones)
{
    _phones = phones;
//    emit phonesChanged(phones);
}

QList<Email*> Institute::emails() const
{
    return _emails;
}

void Institute::setEmails(const QList<Email*> &emails)
{
    _emails = emails;
//    emit emailsChanged(emails);
}

SharedPhone Institute::phone(const PhoneType &type) const
{
    for (int i = 0; i < _phones.count(); ++i)
        if (_phones.at(i)->getType() == type)
            return _phones.at(i);

    return 0;
}

Email* Institute::email(const Type &type) const
{
    for (int i = 0; i < _emails.count(); ++i)
        if (_emails.at(i)->getType() == type)
            return _emails.at(i);

    return 0;
}

InstituteSet::InstituteSet(ezechiel::core::AbstractRepository *repository, QObject *parent, const InstituteSet* &other)
{
    foreach (SharedInstitute _i, *other) {
        SharedInstitute _ni = SharedInstitute(new Institute(repository, parent));
        _ni->setId(_i->id());
        _ni->location_id(_i->location_id());
        _ni->name(_i->name());
        _ni->externalId(_i->externalId());
        _ni->logo(_i->logo());
        this->append(_ni);
    }
}
