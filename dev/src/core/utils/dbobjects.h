/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef DBCONTAINER_H
#define DBCONTAINER_H

#include <QString>
#include <QDate>
#include <QList>
#include <QVariant>
#include <QMetaType>

namespace Tucuxi {
namespace Gui {
namespace Core {



//! Define a ID as in a database
typedef int ident;

//! Invalid identifier, can be used as default value
static const ident invalid_ident = -1;

//! A database account
/** \ingroup database
  An account is used by a database to connect to its storing method.
  A database backend (plugin) is able to give which of these data it needs through the DB::needs() function.
  */
class DBAccount {
public:
   //! The database URL, may contain the protocol (http://,ssh://,sql://) and the port used (:80,:1234)
   QString url;
   //! The user name used for the connection (no link with the Tucuxi user)
   QString username;
   //! The password used for the connection (no link with an Tucuxi password), in plain text
   QString password;
   //! The backend to use for this account
   QString backend;
   //! Used to know is the account is currently valid or not
   bool valid;
};

//! \ingroup database
enum Gender {
   MALE,
   FEMALE
};

/** A physical location.
 * \ingroup database
 * Corresponds to an address, splitted in multiple parts
 */
/*class Location {
public:
    Location();

    void setId(ident id) { _id = id;}
    ident id() const { return _id;}

    void setCountry(const QString &country) { _country = country;}
    QString country() const { return _country;}

    void setState(const QString &state) { _state = state;}
    QString state() const { return _state;}

    void setAddress(const QString &address) { _address = address;}
    QString address() const { return _address;}

    void setCity(const QString &city) { _city = city;}
    QString city() const { return _city;}

    void setPostcode(const QString &postcode) { _postcode = postcode;}
    QString postcode() const { return _postcode;}


protected:
    ident _id;
    QString _country;
    QString _state;
    QString _address;
    QString _city;
    QString _postcode;
};
*/

//#include "location.h"


/** A phone number.
 * \ingroup database
 * As a single person/institute can have multiple phone numbers, a phone type is used.
 * The type might be Fax, Personnal or Work, but is not restricted.
 */
/*struct DBPhone {
    DBPhone();
    DBPhone(QString type, QString number);

    ident id;
    QString type;
    QString number;

    bool operator== (const DBPhone &rhv);
};
*/
/** Institute.
 * \ingroup database
 * An institute is where a practician work.
 * It's basically a name with a location and a logo
 */
/*
 * struct DBInstitute {
    DBInstitute();

    ident id;
    ident location;
    QString externalId;
    QString name;
    QVariant logo;
};
*/
/** A person's data.
 * \ingroup database
 */
/*
 * class DBPerson : public QObject {
    Q_OBJECT

public:
    DBPerson();


    ident id() const { return _id;}
    void setId(ident id) { _id = id;}

    QString name() const {return _name;}
    void setName(const QString &name) {_name = name;}

    QString firstname() const {return _firstname;}
    void setFirstname(const QString &firstname) {_firstname = firstname;}

    void setGender(Gender gender) {_gender = gender;}
    Gender gender() const {return _gender;}

    void setBirthday(QDate birthday) {_birthday = birthday;}
    QDate birthday() const {return _birthday;}

    ident locationId() const {return _locationId;}
    void setLocationId(ident location) {_locationId = location;}

    QList<DBPhone> phones() const {return _phones;}
    void setPhones(QList<DBPhone> &phones) {_phones = phones;}
    void addPhone(const DBPhone &phone) {_phones.append(phone);}

    QList<QString> emails() const {return _emails;}
    void setEmails(QList<QString> &emails) {_emails = emails;}
    void addEmail(const QString &email) {_emails.append(email);}


    SharedLocation location() const {return _location;}
    void setLocation(SharedLocation location) {_location = location;setLocationId(location->id());}

private:


    ident _id;
    ident _locationId;
    Gender _gender;
    QDate _birthday;
    QString _name;
    QString _firstname;
    QList<QString> _emails;
    QList<DBPhone> _phones;

    SharedLocation _location;
};
*/

//#include "person.h"
//#include "admin/src/dal/patient.h"

/** A patient.
 * \ingroup database
 */
/*class DBPatient : public QObject {
    Q_OBJECT
public:
    DBPatient();

    ident id() const { return _id;}
    ident personId() const { return _personId;}
    QString externalId() const { return _externalId;}
    QString stayNumber() const { return _stayNumber;}
    bool stat_ok() const { return _stat_ok;}
    void setId(ident id) { _id = id;}
    void setPersonId(ident person) { _personId = person;}
    void setExternalId(const QString &externalId) { _externalId = externalId;}
    void setStayNumber(const QString &stayNumber) { _stayNumber = stayNumber;}
    void setStatOk(bool stat_ok) { _stat_ok = stat_ok;}



    QString name() const {return _person->name();}
    void setName(const QString &name) {_person->setName(name);}

    QString firstname() const {return _person->firstname();}
    void setFirstname(const QString &firstname) {_person->setFirstname(firstname);}

    void setGender(Gender gender) {_person->setGender(gender);}
    Gender gender() const {return _person->gender();}

    void setBirthday(QDate birthday) {_person->setBirthday(birthday);}
    QDate birthday() const {return _person->birthday();}

    SharedLocation location() const {return _person->location();}
    void setLocation(SharedLocation location) {_person->setLocation(location);}

    QList<DBPhone> phones() const {return _person->phones();}
    void setPhones(QList<DBPhone> &phones) {_person->setPhones(phones);}
    void addPhone(const DBPhone &phone) {_person->addPhone(phone);}

    QList<QString> emails() const {return _person->emails();}
    void setEmails(QList<QString> &emails) {_person->setEmails(emails);}
    void addEmail(const QString &email) {_person->addEmail(email);}

    void setPerson(DBPerson *person) { _person = person; _personId = person->id();}
    DBPerson *person() { return _person;}
private:
    ident _id;
    ident _personId;
    QString _externalId;
    QString _stayNumber;
    bool _stat_ok;

    DBPerson *_person;
};
*/
//#include <iostream>


//#include "admin/src/dal/practician.h"


/** A practician (physician/pharmacologist).
 * \ingroup database
 */
/*
struct DBPractician : public QObject {
    Q_OBJECT
public:
    DBPractician();
    virtual ~DBPractician() {std::cout << "Deleting a practician" << std::endl;}

    ident id() const { return _id;}
    ident personId() const { return _personId;}
    ident institute() const { return _institute;}
    QString externalId() const { return _externalId;}
    QString title() const { return _title;}
    void setId(ident id) { _id = id;}
    void setPersonId(ident person) { _personId = person;}
    void setInstitute(ident institute) { _institute = institute;}
    void setExternalId(const QString &externalId) { _externalId = externalId;}
    void setTitle(const QString &title) { _title = title;}



    QString name() const {return _person->name();}
    void setName(const QString &name) {_person->setName(name);}

    QString firstname() const {return _person->firstname();}
    void setFirstname(const QString &firstname) {_person->setFirstname(firstname);}

    void setGender(Gender gender) {_person->setGender(gender);}
    Gender gender() const {return _person->gender();}

    void setBirthday(QDate birthday) {_person->setBirthday(birthday);}
    QDate birthday() const {return _person->birthday();}

    QList<DBPhone> phones() const {return _person->phones();}
    void setPhones(QList<DBPhone> &phones) {_person->setPhones(phones);}
    void addPhone(const DBPhone &phone) {_person->addPhone(phone);}

    QList<QString> emails() const {return _person->emails();}
    void setEmails(QList<QString> &emails) {_person->setEmails(emails);}
    void addEmail(const QString &email) {_person->addEmail(email);}

    void setPerson(DBPerson *person) { _person = person;}
    DBPerson *person() { return _person;}

    SharedLocation location() const {return _person->location();}
    void setLocation(SharedLocation location) {_person->setLocation(location);}

private:
    ident _id;
    ident _personId;
    ident _institute;
    QString _externalId;
    QString _title;
    DBPerson *_person;
};
*/

/** A report.
 * \ingroup database
 * The report contains the sections enumeration, which corresponds to the optional sections of a report.
 * It also contains the database IDs of needed object, as well as sections specific data and meta data, like the creation date and the reference date.
 * The creation date is the date at which the report was created, and the refence date is the date that will be used to fetch information like, for exemple, the last measure used in the report.
 */
struct DBReport {

    //! The report's sections enumeration
    enum sections {
        INTERPRETATION,
        ANALYST,
        APPLICANT,
        CHART,
        MONITORING,
        LOGO,
        MEASURES,
        PARAMETERS,
        COVARIATES,
        PATIENT,
        RECIPIENT,
        REFERENCES,
        GENERAL,
        LAST_SECTION   //Only used to get the total number of values in this enum
    };

    //Constructors
    DBReport();

    QString templateName;
    QString templateLang;

    //Database IDs
    ident id;
    ident analyst;
    ident applicant;
    ident recipient;
    ident curve;

    //General section
    QString title;
    QString reference;

    //Patient section
    QString comedic;
    QString adverse;
    QString answer;
    QString comments;

    //Interpretation section
    QString interpretation;
    QString posology;

    //Monitoring section
    QString monitoring;

    //Chart section
    QVariant image;

    //References section
    QString references;

    //Meta data
    bool distributed;
    QDateTime crtDate;
    QDateTime refDate;
    QList<sections> layout;
    QList<ident> copyAt;
};

/** \brief Curve database object.
 *  \ingroup database
 *
 * This object contains all the data required to save and load a curve to and from the database.
 */
struct DBCurve {
    //Constructor
    DBCurve();

    //Database IDs
    ident id;
    ident patient;

    //Other IDs
    QString drug;
    QString model;
    QString percentileEngine;
    QString postEngine;
    QString reverseEngine;

    //Enums values
    int pType;
    int cType;

    //Other values
    QString name;
    QString comment;
    QDateTime firsttake;
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi


#endif // DBCONTAINER_H
