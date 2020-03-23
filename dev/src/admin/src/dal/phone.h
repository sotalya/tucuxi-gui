#ifndef PHONE_H
#define PHONE_H

#include "core/dal/entity.h"
#include "core/utils/autoproperty.h"

enum class PhoneType {
    Private,
    Professional,
    Cell,
    UnknownPhoneType
};

class Phone : public ezechiel::core::Entity
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(QString, number, Number)
    AUTO_PROPERTY_DECL(PhoneType, type, Type)

public:

    Q_INVOKABLE Phone(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr);
    Q_INVOKABLE Phone(ezechiel::core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    void setTypeFromString(const QString &type);
    QString typeToString() const;

};

typedef Phone* SharedPhone;
Q_DECLARE_METATYPE(Phone*)
Q_DECLARE_METATYPE(QList<Phone*>)
#endif // PHONE_H
