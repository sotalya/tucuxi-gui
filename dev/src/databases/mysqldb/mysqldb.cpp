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


#include "mysqldb.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlResult>
#include <QtPlugin>
#include <QStringList>
#include <QPixmap>
#include <QDebug>
#include <QSqlDriver>

#include "core.h"
#include "securitymanager.h"
#include "coremessagehandler.h"
#include "units.h"
#include "QSqlQuery"

MySqlDB::MySqlDB() : _encryption(Unknown)
{
    _id = "ch.heig-vd.ezechiel.MySqlDB";
}

//If not already done, close the database
MySqlDB::~MySqlDB()
{
    if (_db.isOpen())
        _db.close();
}

Descriptor MySqlDB::getDescriptor()
{
    return Descriptor(_id, "Local DB", "Local SQLite database");
}

QString MySqlDB::databaseName() const
{
    //Check if the connexion is opened
    if (!_db.isOpen())
        return QString();

    return _db.databaseName();
}

bool MySqlDB::init(Core *core)
{
    Core::setInstance(core);
    return true;
}

QString MySqlDB::errorString()
{
    return _db.lastError().text();
}


// Connexion //////////////////////////////////////////////////////////////////////////////////////

//Open the database file
bool MySqlDB::connect(DBAccount account)
{
    //Check the database drivers
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        WARNING(tr("SQLite driver unavailable"));
        qDebug() << "List of available drivers : " + QSqlDatabase::drivers().join("\n");
        return false;
    }

    //Close previous database
    if (_db.isOpen())
        disconnect();

    //Set the database url
    QString url = account.url;
    if (url.isEmpty())
        url = QString("%1/%2/local.db").arg(CORE->path(Core::PluginsData)).arg(_id);
    qDebug() << "Opening database : " + url;

    //Set the database type
    _db = QSqlDatabase::addDatabase("QMYSQL");

    //Set the database url
    _db.setDatabaseName(url);

    _db.setHostName("localhost");
    _db.setDatabaseName("localdb");
    _db.setUserName("root");
    _db.setPassword("root");

    //Open the database
    if (!_db.open()) {
        WARNING(tr("Could not open the database '%1'").arg(url));
        return false;
    }

    //Check the transactions
    if (!_db.driver()->hasFeature(QSqlDriver::Transactions)) {
        WARNING(tr("SQLite drivers does not support transactions"));
        return false;
    }

    //Check the last inserted ID
    if (!_db.driver()->hasFeature(QSqlDriver::LastInsertId)) {
        WARNING(tr("SQLite drivers does not support the last inserted id feature"));
        return false;
    }

    //Enable FK support
    if (!enableFkSupport()) {
        WARNING(tr("Could not enable the FK support for the database '%1'").arg(url));
        return false;
    }

    return true;
}

//Close the database
bool MySqlDB::disconnect()
{
    //Close the database
    _db.close();

    //Reset the encryption
    _encryption = Unknown;

    return true;
}

//We might use the url, but we do not request it
void MySqlDB::needs(bool &url, bool &user, bool &password)
{
    url = false;
    user = false;
    password = false;
}

// Version ////////////////////////////////////////////////////////////////////////////////////////

Response MySqlDB::getVersion(QString &version)
{
    Response response(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare("SELECT MAX(id) FROM Version"))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        version.clear();
        response.message = tr("The current version of the database could not be found");
        response.error   = NotFound;
        return response;
    }

    //Set the version
    version = query.value(0).toString();

    return response;
}

Response MySqlDB::isCompatible(bool &compatible)
{
    Response response(_id);
    compatible = true;
    return response;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare("SELECT name FROM sqlite_master WHERE type = 'table' ORDER BY 'name'"))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    QStringList tables;
    while (query.next()) {

        //Get the table name
        QString table = query.value(0).toString();

        //Add it to the list
        if (!table.isEmpty())
            tables.append(table);
    }

    //Check some of the tables
    compatible = true;

    if (!tables.contains("Clinicals"))
        compatible = false;
    if (!tables.contains("Covariate"))
        compatible = false;
    if (!tables.contains("Curve"))
        compatible = false;
    if (!tables.contains("Dosage"))
        compatible = false;
    if (!tables.contains("Drug"))
        compatible = false;
    if (!tables.contains("Email"))
        compatible = false;
    if (!tables.contains("Encryption"))
        compatible = false;
    if (!tables.contains("ExO"))
        compatible = false;
    if (!tables.contains("Institute"))
        compatible = false;
    if (!tables.contains("Location"))
        compatible = false;
    if (!tables.contains("Measure"))
        compatible = false;
    if (!tables.contains("Model"))
        compatible = false;
    if (!tables.contains("MxO"))
        compatible = false;
    if (!tables.contains("Option"))
        compatible = false;
    if (!tables.contains("Patient"))
        compatible = false;
    if (!tables.contains("Person"))
        compatible = false;
    if (!tables.contains("Phone"))
        compatible = false;
    if (!tables.contains("Parameter"))
        compatible = false;
    if (!tables.contains("Percentile_Engine"))
        compatible = false;
    if (!tables.contains("Post_Engine"))
        compatible = false;
    if (!tables.contains("Practician"))
        compatible = false;
    if (!tables.contains("PxO"))
        compatible = false;
    if (!tables.contains("Reverse_Engine"))
        compatible = false;
    if (!tables.contains("RxO"))
        compatible = false;
    if (!tables.contains("User"))
        compatible = false;
    if (!tables.contains("Version"))
        compatible = false;

    return response;
}

// Users //////////////////////////////////////////////////////////////////////////////////////////

Response MySqlDB::getUser(const QString &username, User &user)
{
    Response response(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT authorization FROM UserTable WHERE username = '%1'").arg(username)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        response.message = tr("The user '%1' could not be found.").arg(username);
        response.error   = NotFound;
        user = User();
        return response;
    }

    //Set the user data
    user.setUsername(username);
    user.setAuthorization((User::Level) query.value(0).toInt());

    return response;
}

Response MySqlDB::setUser(const User &user)
{
    Response response(_id);

    //Prepare the query
    QSqlQuery query;

    QString content;
    if (exists("User", "username", user.username()))
        content = QString("UPDATE UserTable SET username = ?, authorization = ? WHERE username = '%1'").arg(user.username());
    else
        content = "INSERT INTO UserTable (username, authorization) VALUES (?, ?)";

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(user.username());
    query.addBindValue((int) user.authorization());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return response;
}

Response MySqlDB::getUserKey(const QString &username, QString &key)
{
    Response response(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT enckey FROM UserTable WHERE username = '%2'").arg(username)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        response.message = tr("The user '%1' could not be found.").arg(username);
        response.error   = NotFound;
        key.clear();
        return response;
    }

    //Set the user data
    key = query.value(0).toString();

    return response;
}

Response MySqlDB::setUserKey(const QString &username, const QString &key)
{
    Response response(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("UPDATE UserTable SET enckey = '%1' WHERE username = '%2'").arg(key, username)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return response;
}

Response MySqlDB::deleteUser(const QString &username)
{
    Response response(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM UserTable WHERE username = '%1'").arg(username)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return response;
}

Response MySqlDB::getUsersList(QList<User> &list)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT username, authorization FROM UserTable")))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    list.clear();
    while (query.next()) {

        //Get the current username and level
        QString username  = query.value(0).toString();
        User::Level level = (User::Level) query.value(1).toInt();

        //Add it only if it is valid
        User user(username, level);
        if (user.isValid())
            list.append(user);
    }

    return r;
}

// Practicians ////////////////////////////////////////////////////////////////////////////////////

Response MySqlDB::getPractician(const ident &id, DBPractician &practician)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Practician WHERE id = '%2'").arg("title, Person, Institute, external_id").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The practician ID '%1' could not be found.").arg(id);
        r.error   = NotFound;
        practician = DBPractician();
        return r;
    }

    practician.id         = id;
    practician.title      = query.value(0).toString();
    practician.person 	  = query.value(1).toInt();
    practician.institute  = query.value(2).isNull() ? -1 : query.value(2).toInt();
    practician.externalId = query.value(3).toString();

    return r;
}

Response MySqlDB::getPractician(const QString &externalId, DBPractician &practician)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Practician WHERE external_id = '%2'").arg("title, Person, Institute, id").arg(externalId)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The practician with the external ID '%1' could not be found.").arg(externalId);
        r.error   = NotFound;
        practician = DBPractician();
        return r;
    }

    practician.externalId = externalId;
    practician.title      = query.value(0).toString();
    practician.person     = query.value(1).toInt();
    practician.institute  = query.value(2).isNull() ? -1 : query.value(2).toInt();
    practician.id         = query.value(3).toInt();

    return r;
}

Response MySqlDB::setPractician(DBPractician &practician)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (practician.id == invalid_ident)
        content = "INSERT INTO Practician (title, Person, Institute, external_id) VALUES (?, ?, ?, ?)";
    else
        content = QString("UPDATE Practician SET title = ?, Person = ?, Institute = ?, external_id = ? WHERE id = %1").arg(practician.id);

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(practician.title);
    query.addBindValue(practician.person);
    query.addBindValue(practician.institute == -1 ? QVariant(QVariant::Int) : practician.institute);
    query.addBindValue(practician.externalId);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && practician.id == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted practician"));
        practician.id = id.toInt();
    }

    return r;
}

Response MySqlDB::deletePractician(const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM Practician WHERE id = %1").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return r;
}

Response MySqlDB::getPracticiansList(QList<DBPractician> &list)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Practician").arg("id, title, Person, Institute, external_id")))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    list.clear();
    while (query.next()) {
        DBPractician practician;
        practician.id         = query.value(0).toInt();;
        practician.title      = query.value(1).toString();
        practician.person     = query.value(2).toInt();
        practician.institute  = query.value(3).isNull() ? -1 : query.value(3).toInt();
        practician.externalId = query.value(4).toString();

        if (practician.id != invalid_ident)
            list.append(practician);
    }

    return r;
}


// Institutes /////////////////////////////////////////////////////////////////////////////////////

//Simple get
Response MySqlDB::getInstitute(const ident &id, DBInstitute &institute)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Institute WHERE id = '%2'").arg("name, logo, Location, external_id").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The institute ID '%1' could not be found.").arg(id);
        r.error   = NotFound;
        institute = DBInstitute();
        return r;
    }

    //Set up the institute
    institute.id         = id;
    institute.name       = encrypted ? CORE->securityManager()->decrypt(query.value(0).toString()).toString() : query.value(0).toString();
    institute.logo       = query.value(1);
    institute.location   = query.value(2).toInt();
    institute.externalId = query.value(3).toString();

    return r;
}

Response MySqlDB::getInstitute(const QString &externalId, DBInstitute &institute)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Institute WHERE external_id = '%2'").arg("name, logo, Location, id").arg(externalId)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The institute with the external ID '%1' could not be found.").arg(externalId);
        r.error   = NotFound;
        institute = DBInstitute();
        return r;
    }

    institute.externalId = externalId;
    institute.name       = encrypted ? CORE->securityManager()->decrypt(query.value(0).toString()).toString() : query.value(0).toString();
    institute.logo       = query.value(1);
    institute.location   = query.value(2).toInt();
    institute.id         = query.value(3).toInt();

    return r;
}

//Add or update it
Response MySqlDB::setInstitute(DBInstitute &institute)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (institute.id == invalid_ident)
        content = "INSERT INTO Institute (name, logo, Location, external_id) VALUES (?, ?, ?, ?)";
    else
        content = QString("UPDATE Institute SET name = ?, logo = ?, Location = ?, external_id = ? WHERE id = %1").arg(institute.id);

    if (!query.prepare(content))
        return queryError(query);

    //Bind the query values
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(institute.name).toString() : institute.name);
    query.addBindValue(institute.logo);
    query.addBindValue(institute.location);
    query.addBindValue(institute.externalId);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && institute.id == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted location"));
        institute.id = id.toInt();
    }

    return r;
}

//Simple remove
Response MySqlDB::deleteInstitute(const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Location FROM Institute WHERE id = %1").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The institute ID '%1' could not be found.").arg(id);
        r.error   = NotFound;
        return r;
    }

    int locationId = query.value(0).toInt();

    //Start a new transaction
    QString transactionId = "__deleteInstitute";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    if (!query.prepare(QString("DELETE FROM Institute WHERE id = %1").arg(id))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Delete the location
    if ((r = deleteLocation(locationId)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    return commitTransaction(transactionId);
}

Response MySqlDB::getInstitutesList(QList<DBInstitute> &list)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Institute").arg("id, name, logo, Location, external_id")))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    list.clear();
    while (query.next()) {
        DBInstitute institute;

        //Set up the institute
        institute.id         = query.value(0).toInt();;
        institute.name       = encrypted ? CORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString();
        institute.logo       = query.value(2);
        institute.location   = query.value(3).toInt();
        institute.externalId = query.value(4).toString();

        //Add it to the list
        if (institute.id != invalid_ident)
            list.append(institute);
    }

    return r;
}


// Persons ////////////////////////////////////////////////////////////////////////////////////////

//Fetch this person data
Response MySqlDB::getPerson(const ident &id, DBPerson &person)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Person p WHERE p.id = '%2'").arg("p.birthday,p.firstname,p.gender,p.Location,p.name").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The person ID '%1' could not be found.").arg(id);
        r.error   = NotFound;
        person = DBPerson();
        return r;
    }

    //Set up the person
    person.id        = id;
    person.firstname = encrypted ? CORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString();
    person.name      = encrypted ? CORE->securityManager()->decrypt(query.value(4).toString()).toString() : query.value(4).toString();
    person.birthday  = encrypted ? QDate::fromString(CORE->securityManager()->decrypt(query.value(0).toString()).toString(), Qt::ISODate) : query.value(0).toDate();
    person.gender    = query.value(2).toBool() ? MALE:FEMALE;
    person.location  = query.value(3).toInt();

    //Prepare the query
    if (!query.prepare(QString("SELECT address FROM Email WHERE Person = '%1'").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    while (query.next())
        person.emails << (encrypted ? CORE->securityManager()->decrypt(query.value(0).toString()).toString() : query.value(0).toString());

    //Prepare the query
    if (!query.prepare(QString("SELECT id,number,Phone_Type FROM Phone WHERE Person = '%1'").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    while (query.next()) {

        //Set up the phone
        DBPhone phone;
        phone.id     = query.value(0).toInt();
        phone.number = encrypted ? CORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString();
        phone.type   = encrypted ? CORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString();

        //Add it to the list
        person.phones << phone;
    }

    return r;
}

//Set the data
Response MySqlDB::setPerson(DBPerson &person)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Start a new transaction
    QString transactionId = "__setPerson";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (person.id == invalid_ident)
        content = "INSERT INTO Person (name,firstname,gender,birthday,Location) VALUES (?,?,?,?,?)";
    else
        content = QString("UPDATE Person SET name = ?, firstname = ?, gender = ?, birthday = ?, Location = ? WHERE id = %1").arg(person.id);

    if (!query.prepare(content)) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Bind the query values
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(person.name).toString()      : person.name);
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(person.firstname).toString() : person.firstname);
    query.addBindValue(person.gender == MALE ? "true":"false");
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(person.birthday.toString(Qt::ISODate)).toString() : person.birthday.toString(Qt::ISODate));
    query.addBindValue(person.location);

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //On insert, fetch the row ID
    if (r.error == NoError && person.id == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted person"));
        person.id = id.toInt();
    }

    //Prepare the query
    if (!query.prepare(QString("DELETE FROM Email WHERE Person = %1").arg(person.id))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Save the emails
    foreach (QString e, person.emails) {

        //Prepare the query
        if (!query.prepare("INSERT INTO Email (address,Person) VALUES (?,?)")) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Add the values
        query.addBindValue(encrypted ? CORE->securityManager()->encrypt(e).toString() : e);
        query.addBindValue(person.id);

        //Execute the query
        if (!query.exec()) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }
    }

    //Prepare the query
    if (!query.prepare(QString("DELETE FROM Phone WHERE Person = %1").arg(person.id))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Save the phones
    foreach (DBPhone phone, person.phones) {

        //Prepare the query
        if (!query.prepare("INSERT INTO Phone (number,Phone_Type,Person) VALUES (?,?,?)")) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Add the values
        query.addBindValue(encrypted ? CORE->securityManager()->encrypt(phone.number).toString() : phone.number);
        query.addBindValue(encrypted ? CORE->securityManager()->encrypt(phone.type).toString()   : phone.type);
        query.addBindValue(person.id);

        //Execute the query
        if (!query.exec()) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }
    }

    return commitTransaction(transactionId);
}

//Deletes a person and the related location
Response MySqlDB::deletePerson(const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Location FROM Person WHERE id = %1").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The person ID '%1' could not be found.").arg(id);
        r.error   = NotFound;
        return r;
    }

    int locationId = query.value(0).toInt();

    //Start a new transaction
    QString transactionId = "__deletePerson";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    if (!query.prepare(QString("DELETE FROM Person WHERE id = %1").arg(id))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Delete the location
    if ((r = deleteLocation(locationId)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    return commitTransaction(transactionId);
}

Response MySqlDB::getPersonsList(QList<DBPerson> &persons)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Person p").arg("p.id,p.birthday,p.firstname,p.gender,p.Location,p.name")))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    persons.clear();
    while (query.next()) {
        DBPerson person;

        //Set up the person
        person.id        = query.value(0).toInt();
        person.firstname = encrypted ? CORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString();
        person.name      = encrypted ? CORE->securityManager()->decrypt(query.value(5).toString()).toString() : query.value(5).toString();
        person.birthday  = encrypted ? QDate::fromString(CORE->securityManager()->decrypt(query.value(1).toString()).toString(), Qt::ISODate) : query.value(1).toDate();
        person.gender    = query.value(3).toBool() ? MALE:FEMALE;
        person.location  = query.value(4).toInt();

        //Prepare the query
        QSqlQuery emailQuery;
        if (!emailQuery.prepare(QString("SELECT address FROM Email WHERE Person = '%1'").arg(person.id)))
            return queryError(emailQuery);

        //Execute the query
        if (!emailQuery.exec())
            return queryError(emailQuery);

        //Fetch the results
        while (emailQuery.next())
            person.emails << (encrypted ? CORE->securityManager()->decrypt(emailQuery.value(0).toString()).toString() : emailQuery.value(0).toString());

        //Prepare the query
        QSqlQuery phoneQuery;
        if (!phoneQuery.prepare(QString("SELECT id,number,Phone_Type FROM Phone WHERE Person = '%1'").arg(person.id)))
            return queryError(phoneQuery);

        //Execute the query
        if (!phoneQuery.exec())
            return queryError(phoneQuery);

        //Fetch the results
        while (phoneQuery.next()) {

            //Set up the phone
            DBPhone phone;
            phone.id     = phoneQuery.value(0).toInt();
            phone.number = encrypted ? CORE->securityManager()->decrypt(phoneQuery.value(1).toString()).toString() : phoneQuery.value(1).toString();
            phone.type   = encrypted ? CORE->securityManager()->decrypt(phoneQuery.value(2).toString()).toString() : phoneQuery.value(2).toString();

            //Add it to the list
            person.phones << phone;
        }

        //Add the person to the list
        if (person.id != invalid_ident)
            persons.append(person);
    }

    return r;
}


// Locations //////////////////////////////////////////////////////////////////////////////////////

//Get this location
Response MySqlDB::getLocation(const ident &id, DBLocation &location)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Location WHERE id = '%2'").arg("country,state,address,city,postcode").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The institute ID '%1' could not be found.").arg(id);
        r.error   = NotFound;
        location = DBLocation();
        return r;
    }

    //Set up the location
    location.id       = id;
    location.country  = query.value(0).toString();
    location.state    = encrypted ? CORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString();
    location.address  = encrypted ? CORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString();
    location.city     = encrypted ? CORE->securityManager()->decrypt(query.value(3).toString()).toString() : query.value(3).toString();
    location.postcode = encrypted ? CORE->securityManager()->decrypt(query.value(4).toString()).toString() : query.value(4).toString();

    return r;
}

//Set or update the location
Response MySqlDB::setLocation(DBLocation &location)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (location.id == invalid_ident)
        content = "INSERT INTO Location (country,state,address,city,postcode) VALUES (?,?,?,?,?)";
    else
        content = QString("UPDATE Location SET country = ?, state = ?, address = ?, city = ?, postcode = ? WHERE id = %1").arg(location.id);

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(location.country);
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(location.state).toString()    : location.state);
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(location.address).toString()  : location.address);
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(location.city).toString()     : location.city);
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(location.postcode).toString() : location.postcode);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    if (!_db.driver()->hasFeature(QSqlDriver::LastInsertId)) {
        ERROR(MISSINGID, "no last insert id");
    }

    //On insert, fetch the row ID
    if (r.error == NoError && location.id == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted location"));
        location.id = id.toInt();
    }

    return r;
}

//Simpel remove
Response MySqlDB::deleteLocation(const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM Location WHERE id = %1").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return r;
}

//Get the list of locations
Response MySqlDB::getLocationsList(QList<DBLocation> &locations)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Location").arg("id,country,state,address,city,postcode")))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    locations.clear();
    while (query.next()) {
        DBLocation location;

        //Set up the location
        location.id       = query.value(0).toInt();
        location.country  = query.value(1).toString();
        location.state    = encrypted ? CORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString();
        location.address  = encrypted ? CORE->securityManager()->decrypt(query.value(3).toString()).toString() : query.value(3).toString();
        location.city     = encrypted ? CORE->securityManager()->decrypt(query.value(4).toString()).toString() : query.value(4).toString();
        location.postcode = encrypted ? CORE->securityManager()->decrypt(query.value(5).toString()).toString() : query.value(5).toString();

        //Add it to the list
        if (location.id != invalid_ident)
            locations.append(location);
    }

    return r;
}


// Patients ///////////////////////////////////////////////////////////////////////////////////////

//Return the patient data
Response MySqlDB::getPatient(const ident &pid, DBPatient *&patient)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Patient a WHERE a.id = '%2'").arg("a.id, a.stat_ok, a.Person, a.external_id, a.stay_number").arg(pid)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The patient ID '%1' could not be found.").arg(pid);
        r.error   = NotFound;
        patient = new DBPatient();
        return r;
    }

    patient = new DBPatient();
    patient->setId         ( query.value(0).toInt());
    patient->setStatOk     ( query.value(1).toBool());
    patient->setPerson     ( query.value(2).toInt());
    patient->setExternalId ( query.value(3).toString());
    patient->setStayNumber ( query.value(4).toString());

    return r;
}

Response MySqlDB::getPatient(const QString &externalId, DBPatient *&patient)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Patient a WHERE a.external_id = '%2'").arg("a.stat_ok, a.Person, a.stay_number, a.id").arg(externalId)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The patient with the external ID '%1' could not be found.").arg(externalId);
        r.error   = NotFound;
        patient = new DBPatient();
        return r;
    }
    patient = new DBPatient();

    patient->setExternalId ( externalId);
    patient->setStatOk     ( query.value(0).toBool());
    patient->setPerson     ( query.value(1).toInt());
    patient->setStayNumber ( query.value(2).toString());
    patient->setId         ( query.value(3).toInt());

    return r;
}

//Update or create this patient
Response MySqlDB::setPatient (DBPatient &patient)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (patient.id() == invalid_ident)
        content = "INSERT INTO Patient (stat_ok, Person, external_id, stay_number) VALUES (?, ?, ?, ?)";
    else
        content = QString("UPDATE Patient SET stat_ok = ?, Person = ?, external_id = ?, stay_number = ? WHERE id = %1").arg(patient.id());

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(patient.statOk() ? "true":"false");
    query.addBindValue(patient.person());
    query.addBindValue(patient.externalId());
    query.addBindValue(patient.stayNumber());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && patient.id() == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted patient"));
        patient.setId( id.toInt());
    }

    return r;
}

//Simple remove
Response MySqlDB::deletePatient (const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM Patient WHERE id = '%1';").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return r;
}

//Return the content of the Patient table
Response MySqlDB::getPatientsList(QList<DBPatient*> &list)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Patient a").arg("a.id, a.stat_ok, a.Person, a.external_id, a.stay_number")))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    list.clear();
    while (query.next()) {
        DBPatient *patient;
        patient = new DBPatient();

        //Set up the patient
        patient->setId         ( query.value(0).toInt());
        patient->setStatOk     ( query.value(1).toBool());
        patient->setPerson     ( query.value(2).toInt());
        patient->setExternalId ( query.value(3).toString());
        patient->setStayNumber ( query.value(4).toString());

        //Add it to the list
        if (patient->id() != invalid_ident)
            list.append(patient);
    }

    return r;
}


// Measures ///////////////////////////////////////////////////////////////////////////////////////

//Return the measures linked with this patient, sorted by date
Response MySqlDB::getMeasuresByDate(const ident &patient, QList<Measure> &list)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Measure m JOIN Drug d ON m.Drug = d.sql_id WHERE m.Patient = %2 ORDER BY m.date").arg("m.unit, m.value, m.date, m.id, d.id, m.sampleID, m.arrivalDate").arg(patient)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    list.clear();
    while (query.next()) {
        Measure measure;

        //Set up the measure
        measure.amount.unit = Unit(query.value(0).toString());
        measure.amount.value = query.value(1).toDouble();
        measure.moment = query.value(2).toDateTime();
        measure.dbid = query.value(3).toInt();
        measure.drug = query.value(4).toString();
        measure.sampleID = query.value(5).toString();
        measure.arrivalDate = query.value(6).toDateTime();

        //Add it to the list
        if (measure.dbid != invalid_ident)
            list.append(measure);
    }

    return r;
}

//Return the measures linked with this patient for the specidied drug, sorted by date
Response MySqlDB::getMeasuresByDrug(const ident &patient, const QString &drugId, QList<Measure> &list)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Measure m JOIN Drug d ON m.Drug = d.sql_id WHERE m.Patient = %2 AND d.id = \"%3\" ORDER BY m.date").arg("m.unit,m.value,m.date,m.id,d.id,m.sampleID,m.arrivalDate").arg(patient).arg(drugId)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    list.clear();
    while (query.next()) {
        Measure measure;

        //Set up the measure
        measure.amount.unit = Unit(query.value(0).toString());
        measure.amount.value = query.value(1).toDouble();
        measure.moment = query.value(2).toDateTime();
        measure.dbid = query.value(3).toInt();
        measure.drug = query.value(4).toString();
        measure.sampleID = query.value(5).toString();
        measure.arrivalDate = query.value(6).toDateTime();

        //Add it to the list
        if (measure.dbid != invalid_ident)
            list.append(measure);
    }

    return r;
}

Response MySqlDB::setMeasure (const ident &patient, Measure &measure)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (measure.dbid == invalid_ident)
        content = "INSERT INTO Measure (Patient,date,value,unit,drug,sampleID,arrivalDate) VALUES (?,?,?,?,?,?,?)";
    else
        content = QString("UPDATE Measure SET Patient = ?, date = ?, value = ?, unit = ?, drug = ?, sampleID = ?, arrivalDate = ? WHERE id = %1").arg(measure.dbid);

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(patient);
    query.addBindValue(measure.moment);
    query.addBindValue(measure.amount.value);
    query.addBindValue(measure.amount.unit.toString());
    query.addBindValue(findOrCreate(measure.drug, "Drug"));
    query.addBindValue(measure.sampleID);
    query.addBindValue(measure.arrivalDate);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && measure.dbid == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted measure"));
        measure.dbid = id.toInt();
    }

    return r;
}

Response MySqlDB::deleteMeasure (const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM Measure WHERE id = %1").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return r;
}

// Clinical ///////////////////////////////////////////////////////////////////////////////////////

//Return the clinicals linked with this patient, sorted by date
Response MySqlDB::getClinicalsByDate(const ident &patient, QList<Clinical> &list)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Clinicals c WHERE c.Patient = %2 ORDER BY c.date").arg("c.type, c.value, c.date, c.id").arg(patient)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    list.clear();
    while (query.next()) {
        Clinical clinical;

        //Set up the clinical
        clinical.setId(query.value(0).toString());
        clinical.setValue(encrypted ? CORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString());
        clinical.setDate(encrypted ? QDateTime::fromString(CORE->securityManager()->decrypt(query.value(2).toString()).toString(), Qt::ISODate) : query.value(2).toDateTime());
        clinical.setDbId(query.value(3).toInt());

        //Add it to the list
        if (clinical.dbid() != invalid_ident)
            list.append(clinical);
    }

    return r;
}

Response MySqlDB::setClinical(const ident &patient, Clinical &clinical)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (clinical.dbid() == invalid_ident)
        content = "INSERT INTO Clinicals (Patient,date,value,type) VALUES (?,?,?,?)";
    else
        content = QString("UPDATE Clinicals SET Patient = ?, date = ?, value = ?, type = ? WHERE id = %1").arg(clinical.dbid());

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(patient);
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(clinical.date().toString(Qt::ISODate)).toString() : clinical.date().toString(Qt::ISODate));
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(clinical.value()).toString() : clinical.value());
    query.addBindValue(clinical.id());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && clinical.dbid() == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted clinical"));
        clinical.setDbId(id.toInt());
    }

    return r;
}

Response MySqlDB::deleteClinical(const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM Clinicals WHERE id = %1").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return r;
}

// Covariates /////////////////////////////////////////////////////////////////////////////////////

//Return the covariates linked with this patient, sorted by date
Response MySqlDB::getCovariatesByDate(const ident &id, QList<Covariate> &list)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT c.mid,c.date,c.value,c.sql_id FROM Covariate c WHERE c.Patient = '%1' ORDER BY c.date").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    list.clear();
    while (query.next()) {
        Covariate covariate(query.value(0).toString(), query.value(1).toDateTime(), query.value(2).toDouble());
        covariate.setDbId(query.value(3).toInt());

        if (covariate.dbid() != invalid_ident)
            list.append(covariate);
    }

    return r;
}

Response MySqlDB::setCovariate (const ident &patient, Covariate &covariate)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (covariate.dbid() == invalid_ident)
        content = "INSERT INTO Covariate (Patient,date,value,mid) VALUES (?,?,?,?)";
    else
        content = QString("UPDATE Covariate SET Patient = ?, date = ?, value = ?, mid = ? WHERE sql_id = %1").arg(covariate.dbid());

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(patient);
    query.addBindValue(covariate.date());
    query.addBindValue(covariate.value());
    query.addBindValue(covariate.id());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && covariate.dbid() == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted covariate"));
        covariate.setDbId(id.toInt());
    }

    return r;
}

Response MySqlDB::deleteCovariate(const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM Covariate WHERE sql_id = %1").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return r;
}


// Curves /////////////////////////////////////////////////////////////////////////////////////////

Response MySqlDB::getCurve(const ident &id, DBCurve &curve)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT c.name, c.comment, c.first_take, c.ptype, c.ctype, c.Patient, d.id, m.id, pe.id, po.id, re.id FROM Curve c %1 %2 %3 %4 %5 WHERE c.id = '%6'")
                          .arg("JOIN Drug d ON c.Drug = d.sql_id")
                          .arg("JOIN Model m ON c.Model = m.sql_id")
                          .arg("JOIN Percentile_Engine pe ON c.Percentile_Engine = pe.sql_id")
                          .arg("JOIN Post_Engine po ON c.Post_Engine = po.sql_id")
                          .arg("JOIN Reverse_Engine re ON c.Reverse_Engine = re.sql_id")
                          .arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The curve ID '%1' could not be found.").arg(id);
        r.error   = NotFound;
        curve = DBCurve();
        return r;
    }

    //Set up the curve
    curve.name             = encrypted ? CORE->securityManager()->decrypt(query.value(0).toString()).toString() : query.value(0).toString();
    curve.comment          = encrypted ? CORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString();
    curve.firsttake        = query.value(2).toDateTime();
    curve.pType            = ParameterType(query.value(3).toInt());
    curve.cType            = CurveModel::CurveType(query.value(4).toInt());
    curve.patient          = query.value(5).toInt();
    curve.drug             = query.value(6).toString();
    curve.model            = query.value(7).toString();
    curve.percentileEngine = query.value(8).toString();
    curve.postEngine       = query.value(9).toString();
    curve.reverseEngine    = query.value(10).toString();

    return r;
}

Response MySqlDB::setCurve(DBCurve &curve)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (curve.id == invalid_ident)
        content = "INSERT INTO Curve (name, comment, first_take, ptype, ctype, Patient, Drug, Model, Percentile_Engine, Post_Engine, Reverse_Engine) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    else
        content = QString("UPDATE Curve SET name = ?, comment = ?, first_take = ?, ptype = ?, ctype = ?, Patient = ?, Drug = ?, Model = ?, Percentile_Engine = ?, Post_Engine = ?, Reverse_Engine = ? WHERE id = %1").arg(curve.id);

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(curve.name).toString()    : curve.name);
    query.addBindValue(encrypted ? CORE->securityManager()->encrypt(curve.comment).toString() : curve.comment);
    query.addBindValue(curve.firsttake);
    query.addBindValue((int)curve.pType);
    query.addBindValue((int)curve.cType);
    query.addBindValue(curve.patient);
    query.addBindValue(findOrCreate(curve.drug, "Drug"));
    query.addBindValue(findOrCreate(curve.model, "Model"));
    query.addBindValue(findOrCreate(curve.percentileEngine, "Percentile_Engine"));
    query.addBindValue(findOrCreate(curve.postEngine, "Post_Engine"));
    query.addBindValue(findOrCreate(curve.reverseEngine, "Reverse_Engine"));

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && curve.id == invalid_ident) {
        QVariant newId = query.lastInsertId();
        if (!newId.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted curve"));
        curve.id = newId.toInt();
    }

    return r;
}

Response MySqlDB::deleteCurve(ident id)
{
    Response r(_id);

    //Start a new transaction
    QString transactionId = "__deleteCurve";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Delete the model options
    if ((r = deleteModelsOptions(id)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    //Delete the percentiles options
    if ((r = deletePEsOptions(id)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    //Delete the post engine options
    if ((r = deletePOsOptions(id)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    //Delete the reverse engine options
    if ((r = deleteREsOptions(id)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM Curve WHERE id = %1").arg(id))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    return commitTransaction(transactionId);
}

Response MySqlDB::getCurvesList(QList<DBCurve> &curves)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT c.id, c.name, c.comment, c.first_take, c.ptype, c.ctype, c.Patient, d.id, m.id, pe.id, po.id, re.id FROM Curve c %1 %2 %3 %4 %5")
                          .arg("JOIN Drug d ON c.Drug = d.sql_id")
                          .arg("JOIN Model m ON c.Model = m.sql_id")
                          .arg("JOIN Percentile_Engine pe ON c.Percentile_Engine = pe.sql_id")
                          .arg("JOIN Post_Engine po ON c.Post_Engine = po.sql_id")
                          .arg("JOIN Reverse_Engine re ON c.Reverse_Engine = re.sql_id")))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    curves.clear();
    while (query.next()) {
        DBCurve curve;

        //Set up the curve
        curve.id               = query.value(0).toInt();
        curve.name             = encrypted ? CORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString();
        curve.comment          = encrypted ? CORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString();
        curve.firsttake        = query.value(3).toDateTime();
        curve.pType            = ParameterType(query.value(4).toInt());
        curve.cType            = CurveModel::CurveType(query.value(5).toInt());
        curve.patient          = query.value(6).toInt();
        curve.drug             = query.value(7).toString();
        curve.model            = query.value(8).toString();
        curve.percentileEngine = query.value(9).toString();
        curve.postEngine       = query.value(10).toString();
        curve.reverseEngine    = query.value(11).toString();

        //Add it to the list
        if (curve.id != invalid_ident)
            curves.append(curve);
    }

    return r;
}

Response MySqlDB::getCurvesList(QList<DBCurve> &curves, const ident patientId, const QString &drugId)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    QString content = QString("SELECT c.id, c.name, c.comment, c.first_take, c.ptype, c.ctype, c.Patient, d.id, m.id, pe.id, po.id, re.id FROM Curve c %1 %2 %3 %4 %5 WHERE c.Patient = '%6'")
                         .arg("JOIN Drug d ON c.Drug = d.sql_id")
                         .arg("JOIN Model m ON c.Model = m.sql_id")
                         .arg("JOIN Percentile_Engine pe ON c.Percentile_Engine = pe.sql_id")
                         .arg("JOIN Post_Engine po ON c.Post_Engine = po.sql_id")
                         .arg("JOIN Reverse_Engine re ON c.Reverse_Engine = re.sql_id")
                         .arg(patientId);

    if (!drugId.isEmpty())
        content.append(QString(" AND d.id = '%1'").arg(drugId));

    if (!query.prepare(content))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    curves.clear();
    while (query.next()) {
        DBCurve curve;

        //Set up the curve
        curve.id               = query.value(0).toInt();
        curve.name             = encrypted ? CORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString();
        curve.comment          = encrypted ? CORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString();
        curve.firsttake        = query.value(3).toDateTime();
        curve.pType            = ParameterType(query.value(4).toInt());
        curve.cType            = CurveModel::CurveType(query.value(5).toInt());
        curve.patient          = query.value(6).toInt();
        curve.drug             = query.value(7).toString();
        curve.model            = query.value(8).toString();
        curve.percentileEngine = query.value(9).toString();
        curve.postEngine       = query.value(10).toString();
        curve.reverseEngine    = query.value(11).toString();

        //Add it to the list
        if (curve.id != invalid_ident)
            curves.append(curve);
    }

    return r;
}

Response MySqlDB::getCurvesList(QList<ident> &ids, QStringList &names)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT id, name FROM Curve")))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    ids.clear();
    names.clear();
    while (query.next()) {
        ids.append(query.value(0).toInt());
        names.append(encrypted ? CORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString());
    }

    return r;
}


// Dosages ////////////////////////////////////////////////////////////////////////////////////////

Response MySqlDB::getDosagesByDate (const ident &curve, QList<Dosage> &list)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Dosage WHERE Curve = %2 ORDER BY date").arg("value,unit,dinterval,tinf,date,id").arg(curve)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    list.clear();
    while (query.next()) {
        Dosage dosage;

        //Set up the dosage
        dosage.dose.value = query.value(0).toDouble();
        dosage.dose.unit = Unit(query.value(1).toString());
        dosage.interval = Duration(0,0,query.value(2).toDouble());
        dosage.tinf = Duration(0,0,query.value(3).toDouble());
        dosage.applied = query.value(4).toDateTime();
        dosage.dbid = query.value(5).toInt();

        //Add it to the list
        if (dosage.dbid != invalid_ident)
            list.append(dosage);
    }

    return r;
}

Response MySqlDB::setDosage (const ident &curve, Dosage &dosage)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (dosage.dbid == invalid_ident)
        content = "INSERT INTO Dosage (Curve,value,unit,dinterval,tinf,date) VALUES ( ?, ?, ?, ?, ?, ?)";
    else
        content = QString("UPDATE Dosage SET Curve = ?, value = ?, unit = ?, dinterval = ?, tinf = ?, date = ? WHERE id = %1").arg(dosage.dbid);

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(curve);
    query.addBindValue(dosage.dose.value);
    query.addBindValue(dosage.dose.unit.toString());
    query.addBindValue(dosage.interval.toSeconds());
    query.addBindValue(dosage.tinf.toSeconds());
    query.addBindValue(dosage.applied);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && dosage.dbid == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted dosage"));
        dosage.dbid = id.toInt();
    }

    return r;
}

Response MySqlDB::deleteDosage(const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM Dosage WHERE id = %1").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return r;
}

// Models /////////////////////////////////////////////////////////////////////////////////////////

Response MySqlDB::getModelsOptions(const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option_id FROM MxO WHERE Curve = '%1'").arg(QString::number(curveId))))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    options.clear();
    while (query.next()) {
        Option option;

        //Get the option database ID
        ident optionId = (ident)query.value(0).toInt();

        //Get the option
        r = getOption(optionId, option);
        if (r.error != NoError)
            return r;

        //Store the option
        options.append(option);
    }

    return r;
}

Response MySqlDB::getModelOptions(const QString &modelId, const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option_id FROM MxO WHERE Curve = '%1' AND Model = '%2'").arg(QString::number(curveId), QString::number(findOrCreate(modelId, "Model")))))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    options.clear();
    while (query.next()) {
        Option option;

        //Get the option database ID
        ident optionId = (ident)query.value(0).toInt();

        //Get the option
        r = getOption(optionId, option);
        if (r.error != NoError)
            return r;

        //Store the option
        options.append(option);
    }

    return r;
}

Response MySqlDB::setModelOption(const QString &modelId, const ident &curveId, Option &option)
{
    Response r(_id);

    //Start a new transaction
    QString transactionId = "__setModelOption";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Set the option
    if ((r = setOption(option)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare("INSERT INTO MxO (Model, Option_id, Curve) VALUES (?, ?, ?)")) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Add the values
    query.addBindValue(findOrCreate(modelId, "Model"));
    query.addBindValue(option.sqlId);
    query.addBindValue(curveId);

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    return commitTransaction(transactionId);
}

Response MySqlDB::deleteModelsOptions(const ident &curveId)
{
    QList<Option> options;

    //Get the models options
    Response r = getModelsOptions(curveId, options);
    if (r.error != NoError)
        return r;

    //Start a new transaction
    QString transactionId = "__deleteModelsOptions";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM MxO WHERE Curve = '%1'").arg(QString::number(curveId)))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Delete the options
    foreach (Option option, options) {
        if ((r = deleteOption(option.sqlId)).error != NoError) {
            rollbackTransaction(transactionId);
            return r;
        }
    }

    return commitTransaction(transactionId);
}

Response MySqlDB::deleteModelOption(const QString &modelId, const ident &curveId, const ident &optionId)
{
    Response r(_id);

    //Start a new transaction
    QString transactionId = "__deleteModelOption";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM MxO WHERE Model = '%1' AND Curve = '%2' AND Option = '%3'").arg(modelId, QString::number(curveId), QString::number(optionId)))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Delete the option
    if ((r = deleteOption(optionId)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    return commitTransaction(transactionId);
}

// Percentiles Engines ////////////////////////////////////////////////////////////////////////////

Response MySqlDB::getPEsOptions(const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option_id FROM ExO WHERE Curve = '%1'").arg(QString::number(curveId))))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    options.clear();
    while (query.next()) {
        Option option;

        //Get the option database ID
        ident optionId = (ident)query.value(0).toInt();

        //Get the option
        r = getOption(optionId, option);
        if (r.error != NoError)
            return r;

        //Store the option
        options.append(option);
    }

    return r;
}

Response MySqlDB::getPEOptions(const QString &peId, const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option_id FROM ExO WHERE Curve = '%1' AND Prediction_Engine = '%2'").arg(QString::number(curveId), QString::number(findOrCreate(peId, "Percentile_Engine")))))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    options.clear();
    while (query.next()) {
        Option option;

        //Get the option database ID
        ident optionId = (ident)query.value(0).toInt();

        //Get the option
        r = getOption(optionId, option);
        if (r.error != NoError)
            return r;

        //Store the option
        options.append(option);
    }

    return r;
}

Response MySqlDB::setPEOption(const QString &peId, const ident &curveId, Option &option)
{
    Response r(_id);

    //Start a new transaction
    QString transactionId = "__setPEOption";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Set the option
    if ((r = setOption(option)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare("INSERT INTO ExO (Prediction_Engine, Option_id, Curve) VALUES (?, ?, ?)")) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Add the values
    query.addBindValue(findOrCreate(peId, "Percentile_Engine"));
    query.addBindValue(option.sqlId);
    query.addBindValue(curveId);

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    return commitTransaction(transactionId);
}

Response MySqlDB::deletePEsOptions(const ident &curveId)
{
    QList<Option> options;

    //Get the models options
    Response r = getPEsOptions(curveId, options);
    if (r.error != NoError)
        return r;

    //Start a new transaction
    QString transactionId = "__deletePEsOptions";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM ExO WHERE Curve = '%1'").arg(QString::number(curveId)))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Delete the options
    foreach (Option option, options) {
        if ((r = deleteOption(option.sqlId)).error != NoError) {
            rollbackTransaction(transactionId);
            return r;
        }
    }

    return commitTransaction(transactionId);
}

Response MySqlDB::deletePEOption(const QString &peId, const ident &curveId, const ident &optionId)
{
    Response r(_id);

    //Start a new transaction
    QString transactionId = "__deletePEOption";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM ExO WHERE Prediction_Engine = '%1' AND Curve = '%2' AND Option = '%3'").arg(peId, QString::number(curveId), QString::number(optionId)))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Delete the option
    if ((r = deleteOption(optionId)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    return commitTransaction(transactionId);
}

// Post Engines ///////////////////////////////////////////////////////////////////////////////////

Response MySqlDB::getPOsOptions(const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option_id FROM PxO WHERE Curve = '%1'").arg(QString::number(curveId))))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    options.clear();
    while (query.next()) {
        Option option;

        //Get the option database ID
        ident optionId = (ident)query.value(0).toInt();

        //Get the option
        r = getOption(optionId, option);
        if (r.error != NoError)
            return r;

        //Store the option
        options.append(option);
    }

    return r;
}

Response MySqlDB::getPOOptions(const QString &poId, const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option_id FROM PxO WHERE Curve = '%1' AND Post_Engine = '%2'").arg(QString::number(curveId), QString::number(findOrCreate(poId, "Post_Engine")))))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    options.clear();
    while (query.next()) {
        Option option;

        //Get the option database ID
        ident optionId = (ident)query.value(0).toInt();

        //Get the option
        r = getOption(optionId, option);
        if (r.error != NoError)
            return r;

        //Store the option
        options.append(option);
    }

    return r;
}

Response MySqlDB::setPOOption(const QString &poId, const ident &curveId, Option &option)
{
    Response r(_id);

    //Start a new transaction
    QString transactionId = "__setPOOption";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Set the option
    if ((r = setOption(option)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare("INSERT INTO PxO (Post_Engine, Option_id, Curve) VALUES (?, ?, ?)")) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Add the values
    query.addBindValue(findOrCreate(poId, "Post_Engine"));
    query.addBindValue(option.sqlId);
    query.addBindValue(curveId);

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    return commitTransaction(transactionId);
}

Response MySqlDB::deletePOsOptions(const ident &curveId)
{
    QList<Option> options;

    //Get the models options
    Response r = getPOsOptions(curveId, options);
    if (r.error != NoError)
        return r;

    //Start a new transaction
    QString transactionId = "__deletePOsOptions";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM PxO WHERE Curve = '%1'").arg(QString::number(curveId)))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Delete the options
    foreach (Option option, options) {
        if ((r = deleteOption(option.sqlId)).error != NoError) {
            rollbackTransaction(transactionId);
            return r;
        }
    }

    return commitTransaction(transactionId);
}

Response MySqlDB::deletePOOption(const QString &poId, const ident &curveId, const ident &optionId)
{
    Response r(_id);

    //Start a new transaction
    QString transactionId = "__deletePOOption";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM PxO WHERE Post_Engine = '%1' AND Curve = '%2' AND Option = '%3'").arg(poId, QString::number(curveId), QString::number(optionId)))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Delete the option
    if ((r = deleteOption(optionId)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    return commitTransaction(transactionId);
}

// Reverse Engines ////////////////////////////////////////////////////////////////////////////////

Response MySqlDB::getREsOptions(const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option_id FROM RxO WHERE Curve = '%1'").arg(QString::number(curveId))))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    options.clear();
    while (query.next()) {
        Option option;

        //Get the option database ID
        ident optionId = (ident)query.value(0).toInt();

        //Get the option
        r = getOption(optionId, option);
        if (r.error != NoError)
            return r;

        //Store the option
        options.append(option);
    }

    return r;
}

Response MySqlDB::getREOptions(const QString &reId, const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option_id FROM RxO WHERE Curve = '%1' AND Reverse_Engine = '%2'").arg(QString::number(curveId), QString::number(findOrCreate(reId, "Reverse_Engine")))))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    options.clear();
    while (query.next()) {
        Option option;

        //Get the option database ID
        ident optionId = (ident)query.value(0).toInt();

        //Get the option
        r = getOption(optionId, option);
        if (r.error != NoError)
            return r;

        //Store the option
        options.append(option);
    }

    return r;
}

Response MySqlDB::setREOption(const QString &reId, const ident &curveId, Option &option)
{
    Response r(_id);

    //Start a new transaction
    QString transactionId = "__setREOption";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Set the option
    if ((r = setOption(option)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare("INSERT INTO RxO (Reverse_Engine, Option_id, Curve) VALUES (?, ?, ?)")) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Add the values
    query.addBindValue(findOrCreate(reId, "Reverse_Engine"));
    query.addBindValue(option.sqlId);
    query.addBindValue(curveId);

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    return commitTransaction(transactionId);
}

Response MySqlDB::deleteREsOptions(const ident &curveId)
{
    QList<Option> options;

    //Get the models options
    Response r = getREsOptions(curveId, options);
    if (r.error != NoError)
        return r;

    //Start a new transaction
    QString transactionId = "__deleteREsOptions";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM RxO WHERE Curve = '%1'").arg(QString::number(curveId)))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Delete the options
    foreach (Option option, options) {
        if ((r = deleteOption(option.sqlId)).error != NoError) {
            rollbackTransaction(transactionId);
            return r;
        }
    }

    return commitTransaction(transactionId);
}

Response MySqlDB::deleteREOption(const QString &reId, const ident &curveId, const ident &optionId)
{
    Response r(_id);

    //Start a new transaction
    QString transactionId = "__deleteREOption";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM RxO WHERE Reverse_Engine = '%1' AND Curve = '%2' AND Option = '%3'").arg(reId, QString::number(curveId), QString::number(optionId)))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Delete the option
    if ((r = deleteOption(optionId)).error != NoError) {
        rollbackTransaction(transactionId);
        return r;
    }

    return commitTransaction(transactionId);
}

// Options ////////////////////////////////////////////////////////////////////////////////////////

Response MySqlDB::getOption(const ident &id, Option &option)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT id, value from OptionTable WHERE sql_id = '%1'").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The option ID '%1' could not be found.").arg(id);
        r.error   = NotFound;
        option = Option();
        return r;
    }

    option.oid   = query.value(0).toString();
    option.value = query.value(1);
    option.sqlId = id;

    return r;
}

Response MySqlDB::setOption(Option &option)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (option.sqlId == invalid_ident)
        content = "INSERT INTO OptionTable (id, value) VALUES (?, ?)";
    else
        content = QString("UPDATE OptionTable SET value = ? WHERE sql_id = '%1' AND id = '%2'").arg(QString::number(option.sqlId), option.oid);

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    if (option.sqlId == invalid_ident)
        query.addBindValue(option.oid);
    query.addBindValue(option.value);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && option.sqlId == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted option"));
        option.sqlId = id.toInt();
    }

    return r;
}

Response MySqlDB::deleteOption(const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM OptionTable WHERE sql_id = '%1'").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return r;
}

// Encryption /////////////////////////////////////////////////////////////////////////////////////

Response MySqlDB::isEncrypted(bool &encrypted)
{
    Response r(_id);

    //Check if available
    if (_encryption != Unknown) {
        encrypted = _encryption == Encrypted;
        return r;
    }

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare("SELECT value FROM Encryption WHERE enckey = 'encrypted'"))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The value 'encrypted' could not be found.");
        r.error   = NotFound;
        return r;
    }

    //Set the result
    encrypted = query.value(0).toBool();

    //Set the status
    _encryption = encrypted ? Encrypted : Clear;

    return r;
}

Response MySqlDB::setEncrypted(bool encrypted)
{
    Response r(_id);

    //Get the current status
    bool current;

    r = isEncrypted(current);

    //Check for error
    if (r.error != NoError)
        return r;

    //Check if unchanged
    if (current == encrypted)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare("UPDATE Encryption SET value = ? WHERE enckey = 'encrypted'"))
        return queryError(query);

    //Add the value
    query.addBindValue(encrypted);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Update the status
    _encryption = encrypted ? Encrypted : Clear;

    return r;
}

Response MySqlDB::encryptDatabase(bool encrypt)
{
    bool status;

    //Get the encryption status
    Response response = isEncrypted(status);
    if (response.error != NoError)
        return response;

    //Ignore if the status is equal
    if (encrypt == status) {
        response.message = tr("The database is already %1.").arg(encrypt ? tr("encrypted") : tr("decrypted"));
        response.error = QueryError;
        return response;
    }

    //Start a new transaction
    QString transactionId = "__encryptDatabase";
    if ((response = startTransaction(transactionId)).error != NoError)
        return response;

    //Encrypt/decrypt the persons
    if ((response = encryptPersons(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Encrypt/decrypt the patients
    if ((response = encryptPatients(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Encrypt/decrypt the practicians
    if ((response = encryptPracticians(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Encrypt/decrypt the locations
    if ((response = encryptLocations(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Encrypt/decrypt the institutes
    if ((response = encryptInstitutes(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Encrypt/decrypt the reports
    if ((response = encryptReports(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Encrypt/decrypt the covariates
    if ((response = encryptCovariates(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Encrypt/decrypt the clinicals
    if ((response = encryptClinicals(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Encrypt/decrypt the measures
    if ((response = encryptMeasures(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Encrypt/decrypt the curves
    if ((response = encryptCurves(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Encrypt/decrypt the dosages
    if ((response = encryptDosages(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Set the encryption status
    if ((response = setEncrypted(encrypt)).error != NoError) {
        rollbackTransaction(transactionId);
        return response;
    }

    //Commit the transaction
    return commitTransaction(transactionId);
}

// Utils //////////////////////////////////////////////////////////////////////////////////////////

//Enable the foreign key support for SQLite
bool MySqlDB::enableFkSupport()
{
    return true;
    QSqlQuery query("PRAGMA foreign_keys = ON");
    return query.isActive();
}

//Find the ID or create this element
ident MySqlDB::findOrCreate (const QString &id, const QString &table)
{
    //Try to find the element
    QSqlQuery query;
    if (!query.prepare(QString("SELECT sql_id FROM %1 WHERE id = '%2'").arg(table).arg(id)))
        ERROR(MISSINGID, tr("Could not find the requested ID '%1' in table '%2'").arg(id, table));

    if (!query.exec())
        ERROR(MISSINGID, tr("Could not find the requested ID '%1' in table '%2'").arg(id, table));

    //If not found, create it
    if (!query.next()) {

        //Prepare the query
        if (!query.prepare(QString("INSERT INTO %1 (id) VALUES (?)").arg(table)))
            ERROR(MISSINGID, tr("Could not find the requested ID '%1' in table '%2'").arg(id, table));

        query.addBindValue(id);

        //Excute the query
        if (!query.exec())
            ERROR(MISSINGID, tr("Could not find the requested ID '%1' in table '%2'").arg(id, table));

        return query.lastInsertId().toInt();

        //Prepare the query
        if (!query.prepare("SELECT last_insert_rowid()"))
            ERROR(MISSINGID, tr("Could not find the requested ID '%1' in table '%2'").arg(id, table));

        //Excute the query
        if (!query.exec())
            ERROR(MISSINGID, tr("Could not find the requested ID '%1' in table '%2'").arg(id, table));

        if (!query.next())
            ERROR(MISSINGID, tr("Could not find the requested ID '%1' in table '%2'").arg(id, table));
    }

    //Fetch and return the ID
    return query.value(0).toInt();
}

//Return an error message on a query failure
Response MySqlDB::queryError(const QSqlQuery &query) const
{
    Response response(_id);

    response.message = QString("[Code %1] %2;\n%3.").arg(QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
    response.error   = QueryError;

    return response;
}

//Return an error message on a database failure
Response MySqlDB::databaseError(const QSqlDatabase &database, const QString &message) const
{
    Response response(_id);

    response.message = QString("%1 The database returned the following error:\n\n[Code %2] %3;\n%4.").arg(message, QString::number(database.lastError().number()), database.lastError().databaseText(), database.lastError().driverText());
    response.error   = QueryError;

    return response;
}

//Starts a transaction
Response MySqlDB::startTransaction(const QString &id)
{
    Response response(_id);

    //Check if already active
    if (!_transactionId.isEmpty())
        return response;

    //Start a new transaction
    if (!_db.transaction())
        return databaseError(_db, tr("Failed to start a new transaction."));

    //Set the transactions ID
    _transactionId = id;

    return response;
}

//Rollback a transaction
Response MySqlDB::rollbackTransaction(const QString &id)
{
    Response response(_id);

    //Check if the ID matches
    if (_transactionId != id)
        return response;

    //Reset the transaction ID
    _transactionId.clear();

    //Rollback the transaction
    if (!_db.rollback())
        return databaseError(_db, tr("Failed to rollback the transaction"));

    return response;
}

//Commits a transaction
Response MySqlDB::commitTransaction(const QString &id)
{
    Response response(_id);

    //Check if the ID matches
    if (_transactionId != id)
        return response;

    //Reset the transaction ID
    _transactionId.clear();

    //Commit the transaction
    if (!_db.commit())
        return databaseError(_db, tr("Failed to commit the modification."));

    return response;
}

//Return true the given PK value already exists
bool MySqlDB::exists(const QString &table, const QString &pk, const QString &value)
{
    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg(pk, table, pk, value)))
        return false;

    //Execute the query
    if (!query.exec())
        return false;

    //Check the result
    if (!query.next())
        return false;

    return true;
}

//Return true the given PK value already exists
bool MySqlDB::exists(const QString &table, const QString &pk, ident value)
{
    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg(pk, table, pk, QString::number(value))))
        return false;

    //Execute the query
    if (!query.exec())
        return false;

    //Check the result
    if (!query.next())
        return false;

    return true;
}

Response MySqlDB::encryptPersons(bool encrypt)
{
    Response response(_id);

    //Get the list of persons
    QList<DBPerson> persons;

    if ((response = getPersonsList(persons)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each person
    for (int i = 0; i < persons.size(); i++)
        if ((response = setPerson(persons[i])).error != NoError)
            return response;

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response MySqlDB::encryptPatients(bool encrypt)
{
    Response response(_id);

    //Get the list of patients
    QList<DBPatient*> patients;

    if ((response = getPatientsList(patients)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each patient
    for (int i = 0; i < patients.size(); i++)
        if ((response = setPatient(*patients[i])).error != NoError)
            return response;

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response MySqlDB::encryptPracticians(bool encrypt)
{
    Response response(_id);

    //Get the list of practicians
    QList<DBPractician> practicians;

    if ((response = getPracticiansList(practicians)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each practician
    for (int i = 0; i < practicians.size(); i++)
        if ((response = setPractician(practicians[i])).error != NoError)
            return response;

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response MySqlDB::encryptLocations(bool encrypt)
{
    Response response(_id);

    //Get the list of locations
    QList<DBLocation> locations;

    if ((response = getLocationsList(locations)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each location
    for (int i = 0; i < locations.size(); i++)
        if ((response = setLocation(locations[i])).error != NoError)
            return response;

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response MySqlDB::encryptInstitutes(bool encrypt)
{
    Response response(_id);

    //Get the list of institutes
    QList<DBInstitute> institutes;

    if ((response = getInstitutesList(institutes)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each institute
    for (int i = 0; i < institutes.size(); i++)
        if ((response = setInstitute(institutes[i])).error != NoError)
            return response;

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response MySqlDB::encryptReports(bool encrypt)
{
    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // Implement me when the reports database function are implemented
    // ////////////////////////////////////////////////////////////////////////////////////////////

    Q_UNUSED(encrypt);
    return Response(_id);
}

Response MySqlDB::encryptCovariates(bool encrypt)
{
    Response response(_id);

    //Get the list of patients
    QList<DBPatient*> patients;
    if ((response = getPatientsList(patients)).error != NoError)
        return response;

    //Update each patient's covariates
    for (int i = 0; i < patients.size(); i++) {

        //Get the list of covariates
        QList<Covariate> covariates;

        if ((response = getCovariatesByDate(patients.at(i)->id(), covariates)).error != NoError)
            return response;

        //Set the encryption status
        _encryption = encrypt ? Encrypted : Clear;

        //Update each covariate
        for (int j = 0; j < covariates.size(); j++)
            if ((response = setCovariate(patients.at(i)->id(), covariates[j])).error != NoError)
                return response;

        //Reset the encryption status
        _encryption = encrypt ? Clear : Encrypted;
    }

    return response;
}

Response MySqlDB::encryptClinicals(bool encrypt)
{
    Response response(_id);

    //Get the list of patients
    QList<DBPatient*> patients;
    if ((response = getPatientsList(patients)).error != NoError)
        return response;

    //Update each patient's clinicals
    for (int i = 0; i < patients.size(); i++) {

        //Get the list of clinicals
        QList<Clinical> clinicals;

        if ((response = getClinicalsByDate(patients.at(i)->id(), clinicals)).error != NoError)
            return response;

        //Set the encryption status
        _encryption = encrypt ? Encrypted : Clear;

        //Update each clinical
        for (int j = 0; j < clinicals.size(); j++)
            if ((response = setClinical(patients.at(i)->id(), clinicals[j])).error != NoError)
                return response;

        //Reset the encryption status
        _encryption = encrypt ? Clear : Encrypted;
    }

    return response;
}

Response MySqlDB::encryptMeasures(bool encrypt)
{
    Response response(_id);

    //Get the list of patients
    QList<DBPatient*> patients;
    if ((response = getPatientsList(patients)).error != NoError)
        return response;

    //Update each patient's measures
    for (int i = 0; i < patients.size(); i++) {

        //Get the list of measures
        QList<Measure> measures;

        if ((response = getMeasuresByDate(patients.at(i)->id(), measures)).error != NoError)
            return response;

        //Set the encryption status
        _encryption = encrypt ? Encrypted : Clear;

        //Update each measure
        for (int j = 0; j < measures.size(); j++)
            if ((response = setMeasure(patients.at(i)->id(), measures[j])).error != NoError)
                return response;

        //Reset the encryption status
        _encryption = encrypt ? Clear : Encrypted;
    }

    return response;
}

Response MySqlDB::encryptCurves(bool encrypt)
{
    Response response(_id);

    //Get the list of curves
    QList<DBCurve> curves;

    if ((response = getCurvesList(curves)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each curve
    for (int i = 0; i < curves.size(); i++)
        if ((response = setCurve(curves[i])).error != NoError)
            return response;

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response MySqlDB::encryptDosages(bool encrypt)
{
    Response response(_id);

    //Get the list of curves
    QList<DBCurve> curves;

    if ((response = getCurvesList(curves)).error != NoError)
        return response;

    //Update each curve's dosages
    for (int i = 0; i < curves.size(); i++) {

        //Get the list of dosages
        QList<Dosage> dosages;

        if ((response = getDosagesByDate(curves.at(i).id, dosages)).error != NoError)
            return response;

        //Set the encryption status
        _encryption = encrypt ? Encrypted : Clear;

        //Update each dosages
        for (int j = 0; j < dosages.size(); j++)
            if ((response = setDosage(curves.at(i).id, dosages[j])).error != NoError)
                return response;

        //Reset the encryption status
        _encryption = encrypt ? Clear : Encrypted;
    }

    return response;
}
