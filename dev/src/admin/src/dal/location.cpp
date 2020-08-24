#include "location.h"

Location::Location(ezechiel::core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _address(),
    _postcode(),
    _city(),
    _state(),
    _country()
{
}

Location::Location(ezechiel::core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository, id, parent),
    _address(),
    _postcode(),
    _city(),
    _state(),
    _country()
{
}

QVariant Location::toQVariant() const{
    QMap<QString, QVariant> locationMap;

    locationMap["city"] = _city;
    locationMap["postcode"] = _postcode;
    locationMap["state"] = _state;
    locationMap["country"] = _country;

    return locationMap;
}

//QString Location::address() const
//{
//    return _address;
//}

//void Location::setAddress(const QString &address)
//{
//    _address = address;
//    emit addressChanged(address);
//}

//QString Location::postcode() const
//{
//    return _postcode;
//}

//void Location::setPostcode(const QString &postcode)
//{
//    _postcode = postcode;
//    emit postcodeChanged(postcode);
//}

//QString Location::city() const
//{
//    return _city;
//}

//void Location::setCity(const QString &city)
//{
//    _city = city;
//    emit cityChanged(city);
//}

//QString Location::state() const
//{
//    return _state;
//}

//void Location::setState(const QString &state)
//{
//    _state = state;
//    emit stateChanged(state);
//}

//QString Location::country() const
//{
//    return _country;
//}

//void Location::setCountry(const QString &country)
//{
//    _country = country;
//    emit countryChanged(country);
//}
