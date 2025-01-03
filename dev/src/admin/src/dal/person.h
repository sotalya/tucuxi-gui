/*
 * Tucuxi - Tucuxi-gui software.
 * This software is able to perform prediction of drug concentration in blood
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV.
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef PERSON_H
#define PERSON_H

#include "core/dal/entity.h"
#include "location.h"
#include "phone.h"
#include "email.h"
#include <QDate>
#include <QDateTime>

Q_MOC_INCLUDE("core/dal/uncastedvalue.h")

namespace Tucuxi {
namespace Gui {
namespace Core {
class UncastedValueList;
}
}
}

namespace Tucuxi {
namespace Gui {
namespace Admin {

class Person : public Tucuxi::Gui::Core::Entity
{
public:
    enum GenderType { Male = 1, Female = 0, Unknown = 2 };
    Q_ENUM(GenderType)

    Q_OBJECT

    ADMIN_ENTITY_UTILS(Person)

    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QDate, birthday)
    AUTO_PROPERTY(GenderType, gender)
    AUTO_PROPERTY(QString, firstname)
    AUTO_PROPERTY(Location*, location)
    AUTO_PROPERTY(int, location_id)
    AUTO_PROPERTY_DECL(Tucuxi::Gui::Core::UncastedValueList *, uncastedValues, UncastedValues)
    AUTO_PROPERTY_DECL(PhoneList*, phones, Phones)

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

public:
    Q_INVOKABLE Person(Tucuxi::Gui::Core::AbstractRepository *repository = nullptr, QObject *parent = nullptr);
    Q_INVOKABLE Person(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    Q_INVOKABLE void setBirthdate(QDateTime date);

    Q_INVOKABLE void setPrimaryPhone(QString phoneNumber);

//    Q_INVOKABLE QList<SharedPhone> phones() const {return _phones;}
//    Q_INVOKABLE void setPhones(const QList<SharedPhone> &phones) {_phones = phones; emit phonesChanged(phones);}

    QList<Email*> emails() const {return _emails;}
    void setEmails(const QList<Email*> &emails) {_emails = emails; emit emailsChanged(emails);}

    int personId() const { return id();}

    void addEmail(Email* email) { _emails.append(email);}
    void addPhone(SharedPhone phone) { _phones->append(phone);}

signals:
    //void phonesChanged(const QList<SharedPhone> &phones);
    void emailsChanged(const QList<Email*> &emails);

private:
//    QList<Phone*> _phones;
    QList<Email*> _emails;
};

}
}
}

typedef Tucuxi::Gui::Admin::Person* SharedPerson;
Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::Person*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Person*>)

#endif // PERSON_H
