#ifndef PERSON_H
#define PERSON_H

#include "core/dal/entity.h"
#include "location.h"
#include "phone.h"
#include "email.h"
#include <QDate>
#include <QDateTime>

namespace ezechiel {
namespace core {
class UncastedValueList;
}

}

class Person : public ezechiel::core::Entity
{
public:
    enum GenderType { Male = 1, Female = 0 };
    Q_ENUM(GenderType)

    Q_OBJECT

    ADMIN_ENTITY_UTILS(Person)

    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QDate, birthday)
    AUTO_PROPERTY(GenderType, gender)
    AUTO_PROPERTY(QString, firstname)
    AUTO_PROPERTY(Location*, location)
    AUTO_PROPERTY(int, location_id)
    AUTO_PROPERTY_DECL(ezechiel::core::UncastedValueList *, uncastedValues, UncastedValues)

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

public:
    Q_INVOKABLE Person(ezechiel::core::AbstractRepository *repository = 0, QObject *parent = nullptr);
    Q_INVOKABLE Person(ezechiel::core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    Q_INVOKABLE void setBirthdate(QDateTime date);

    QList<SharedPhone> phones() const {return _phones;}
    void setPhones(const QList<SharedPhone> &phones) {_phones = phones; emit phonesChanged(phones);}

    QList<Email*> emails() const {return _emails;}
    void setEmails(const QList<Email*> &emails) {_emails = emails; emit emailsChanged(emails);}

    int personId() const { return id();}

    void addEmail(Email* email) { _emails.append(email);}
    void addPhone(SharedPhone phone) { _phones.append(phone);}

    QVariant toQVariant(void) const;

signals:
    void phonesChanged(const QList<SharedPhone> &phones);
    void emailsChanged(const QList<Email*> &emails);

private:
    QList<Phone*> _phones;
    QList<Email*> _emails;
};

typedef Person* SharedPerson;
Q_DECLARE_METATYPE(Person*)
Q_DECLARE_METATYPE(QList<Person*>)

#endif // PERSON_H
