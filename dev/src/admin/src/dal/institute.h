#ifndef INSTITUTE_H
#define INSTITUTE_H

#include "core/dal/entity.h"
#include "location.h"
#include "phone.h"
#include "email.h"

#include <QImage>

namespace Tucuxi {
namespace Gui {
namespace Admin {

class Institute : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, externalId)
    AUTO_PROPERTY(QImage, logo)
    AUTO_PROPERTY(Location*, location)
    AUTO_PROPERTY(int, location_id)

public:
    Q_INVOKABLE Institute(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);
    Q_INVOKABLE Institute(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    QList<SharedPhone> phones() const;
    void setPhones(const QList<SharedPhone> &phones);

    QList<Email*> emails() const;
    void setEmails(const QList<Email*> &emails);

    SharedPhone phone(const PhoneType &type) const;
    Email* email(const Type &type) const;

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

private:
    QList<SharedPhone> _phones;
    QList<Email*> _emails;
};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::Institute*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Institute*>)

namespace Tucuxi {
namespace Gui {
namespace Admin {

class InstituteSet : public QList<Tucuxi::Gui::Admin::Institute*>
{
public :
    InstituteSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent = 0) {}
    InstituteSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent, const InstituteSet* &other);
};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::InstituteSet*)

typedef Tucuxi::Gui::Admin::Institute* SharedInstitute;
typedef Tucuxi::Gui::Admin::InstituteSet* SharedInstituteSet;

#endif // INSTITUTE_H
