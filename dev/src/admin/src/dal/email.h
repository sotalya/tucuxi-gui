#ifndef EMAIL_H
#define EMAIL_H

#include "core/dal/entity.h"
#include "core/utils/autoproperty.h"

enum class Type {
    Private,
    Professional
};

class Email : public Tucuxi::GuiCore::Entity
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(QString, email, Email)
    AUTO_PROPERTY_DECL(Type, type, Type)

public:


    Q_INVOKABLE Email(Tucuxi::GuiCore::AbstractRepository *repository, QObject *parent = nullptr);
    Q_INVOKABLE Email(Tucuxi::GuiCore::AbstractRepository *repository, const int &id, QObject *parent = nullptr);
};

Q_DECLARE_OPAQUE_POINTER(Email*)
Q_DECLARE_METATYPE(QList<Email*>)
#endif // EMAIL_H
