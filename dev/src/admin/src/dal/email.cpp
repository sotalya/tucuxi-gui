#include "email.h"

AUTO_PROPERTY_IMPL(Email, QString, email, Email)
AUTO_PROPERTY_IMPL(Email, Type, type, Type)

Email::Email(Tucuxi::GuiCore::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _email(),
    _type(Type::Professional)
{
    setId(-1);
}

Email::Email(Tucuxi::GuiCore::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository,id, parent),
    _email(),
    _type(Type::Professional)
{
    setId(-1);
}

//QString Email::email() const
//{
//    return _email;
//}

//void Email::setEmail(const QString &email)
//{
//    _email = email;
//    emit emailChanged(email);
//}

//Email::Type Email::type() const
//{
//    return _type;
//}

//void Email::setType(const Email::Type &type)
//{
//    _type = type;
//    emit typeChanged(type);
//}
