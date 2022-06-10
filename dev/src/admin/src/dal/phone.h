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

class Phone : public Tucuxi::GuiCore::Entity
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(Phone)

    AUTO_PROPERTY_DECL(QString, number, Number)
    AUTO_PROPERTY_DECL(PhoneType, type, Type)

public:

    Q_INVOKABLE Phone(Tucuxi::GuiCore::AbstractRepository *repository, QObject *parent = nullptr);
    Q_INVOKABLE Phone(Tucuxi::GuiCore::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    void setTypeFromString(const QString &type);
    QString typeToString() const;

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

};

QML_POINTERLIST_CLASS_DECL(PhoneList, Phone)

typedef Phone* SharedPhone;
Q_DECLARE_METATYPE(PhoneType)
Q_DECLARE_METATYPE(Phone*)
Q_DECLARE_METATYPE(QList<Phone*>)
#endif // PHONE_H
