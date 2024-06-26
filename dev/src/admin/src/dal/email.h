#ifndef EMAIL_H
#define EMAIL_H

#include "core/dal/entity.h"
#include "core/utils/autoproperty.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

enum class Type {
    Private,
    Professional
};

class Email : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(QString, email, Email)
    AUTO_PROPERTY_DECL(Type, type, Type)

public:


    Q_INVOKABLE Email(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);
    Q_INVOKABLE Email(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);
};

}
}
}

Q_DECLARE_OPAQUE_POINTER(Tucuxi::Gui::Admin::Email*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Email*>)
#endif // EMAIL_H
