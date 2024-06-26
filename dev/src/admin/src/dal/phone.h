#ifndef PHONE_H
#define PHONE_H

#include "core/dal/entity.h"
#include "core/utils/autoproperty.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

enum class PhoneType {
    Private,
    Professional,
    Cell,
    UnknownPhoneType
};

class Phone : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(Phone)

    AUTO_PROPERTY_DECL(QString, number, Number)
    AUTO_PROPERTY_DECL(PhoneType, type, Type)

public:

    Q_INVOKABLE Phone(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);
    Q_INVOKABLE Phone(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    void setTypeFromString(const QString &type);
    QString typeToString() const;

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

};

QML_POINTERLIST_CLASS_DECL(PhoneList, Phone)

}
}
}

typedef Tucuxi::Gui::Admin::Phone* SharedPhone;
Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::PhoneType)
Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::Phone*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Phone*>)
#endif // PHONE_H
