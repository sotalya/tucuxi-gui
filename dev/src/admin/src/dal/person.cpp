#include "person.h"
#include "../adminfactory.h"
#include "core/dal/uncastedvalue.h"

using namespace Tucuxi::Gui::Admin;

AUTO_PROPERTY_IMPL(Person, Tucuxi::Gui::Core::UncastedValueList *, uncastedValues, UncastedValues)
AUTO_PROPERTY_IMPL(Person, PhoneList*, phones, Phones)

Person::Person(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _location_id(-1),
    _name(),
    _firstname(),
    _birthday(),
    _location(),
    _emails(),
    _phones(AdminFactory::createEntity<PhoneList>(repository, this)),
    _gender(GenderType::Male),
    _uncastedValues(AdminFactory::createEntity<Tucuxi::Gui::Core::UncastedValueList>(repository, this))
{    
//    SharedLocation _loc = AdminFactory::createEntity<Location>(repository);
//    setLocation(_loc.data());
//    setLocation(0);
    _location = AdminFactory::createEntity<Location>(repository);
    // setPhones(QList<SharedPhone>());
    setEmails(QList<Email*>());
}

Person::Person(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository,id, parent),
    _location_id(-1),
    _name(),
    _firstname(),
    _location(),
    _birthday(),
    _emails(),
    _phones(AdminFactory::createEntity<PhoneList>(repository, this)),
    _gender(GenderType::Male),
    _uncastedValues(NULL)
{
    Location* _loc = AdminFactory::createEntity<Location>(repository);
    location(_loc);
    // setPhones(QList<SharedPhone>());
    setEmails(QList<Email*>());
}

void Person::setBirthdate(QDateTime date)
{
    _birthday = date.date();
}


void Person::setPrimaryPhone(QString phoneNumber)
{
    if (_phones->size() > 0) {
        _phones->at(0)->setNumber(phoneNumber);
    }
    else {
        auto phone = Tucuxi::Gui::Core::CoreFactory::createEntity<Phone>(_repository, _phones);
        phone->setType(PhoneType::Professional);
        phone->setNumber(phoneNumber);
        _phones->append(phone);
    }
}
//void Person::setGender(GenderType gender) {
//    _gender = gender;
//    emit genderChanged(gender);
//}

//Person::GenderType Person::gender() const {
//    return _gender;
//}

//void Person::setBirthday(QDate birthday) {
//    _birthday = birthday;
//    emit birthdayChanged(birthday);
//}

//QDate Person::birthday() const {
//    return _birthday;
//}

//void Person::setName(const QString& name) {
//    _name = name;
//    emit nameChanged(name);
//}

//QString Person::name() const {
//    return _name;
//}

//void Person::setFirstname(const QString& firstname) {
//    _firstname = firstname;
//    emit firstnameChanged(firstname);
//}

//QString Person::firstname() const {
//    return _firstname;
//}

//SharedLocation Person::location() const
//{
//    return _location;
//}

//void Person::setLocation(const SharedLocation &location)
//{
//    _location = location;
//    emit locationChanged(location);
//}

//QList<SharedPhone> Person::phones() const
//{
//    return _phones;
//}

//void Person::setPhones(const QList<SharedPhone> &phones)
//{
//    _phones = phones;
//    emit phonesChanged(phones);
//}

//QList<SharedEmail> Person::emails() const
//{
//    return _emails;
//}

//void Person::setEmails(const QList<SharedEmail> &emails)
//{
//    _emails = emails;
//    emit emailsChanged(emails);
//}

//Phone* Person::phone(const Phone::Type &type) const
//{
//    for (int i = 0; i < _phones.count(); ++i)
//        if (_phones.at(i)->type() == type)
//            return _phones.at(i);

//    return new Phone();
//}

//Email* Person::email(const Email::Type &type) const
//{
//    for (int i = 0; i < _emails.count(); ++i)
//        if (_emails.at(i)->type() == type)
//            return _emails.at(i);

//    return new Email(ABSTRACTREPO);
//}
