#include "phone.h"

Phone::Phone(ezechiel::core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _number(),
    _type(PhoneType::Professional)
{
    setId(-1);
}

Phone::Phone(ezechiel::core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository,id, parent),
    _number(),
    _type(PhoneType::Professional)
{
    setId(id);
}

AUTO_PROPERTY_IMPL(Phone, QString, number, Number)
AUTO_PROPERTY_IMPL(Phone, PhoneType, type, Type)

QML_POINTERLIST_CLASS_IMPL(PhoneList, Phone)

//QString Phone::number() const
//{
//    return _number;
//}

//void Phone::setNumber(const QString &number)
//{
//    _number = number;
//    emit numberChanged(number);
//}

//Phone::Type Phone::type() const
//{
//    return _type;
//}

//void Phone::setType(const Phone::Type &type)
//{
//    _type = type;
//    emit typeChanged(type);
//}


void Phone::setTypeFromString(const QString &stype)
{
    if (stype.compare("private") == 0)
        setType(PhoneType::Private);
    else if (stype.compare("prof") == 0)
        setType(PhoneType::Professional);
    else if (stype.compare("cell") == 0)
        setType(PhoneType::Cell);
    else setType(PhoneType::UnknownPhoneType);
}

QString Phone::typeToString() const
{
    switch (_type) {
    case PhoneType::Private          : return "private";
    case PhoneType::Professional     : return "prof";
    case PhoneType::Cell             : return "cell";
    case PhoneType::UnknownPhoneType : return "unknown";
    }
    return "unknown";
}
