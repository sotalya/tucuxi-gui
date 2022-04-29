//@@license@@

#include "localdb.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlResult>
#include <QtPlugin>
#include <QStringList>
#include <QPixmap>
#include <QDebug>
#include <QDir>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QBuffer>

#include "appcore.h"
#include "securitymanager.h"
#include "coremessagehandler.h"
#include "units.h"
#include "reportdata.h"
#include "prediction.h"

LocalDB::LocalDB() : _encryption(Unknown)
{
    _id = "ch.heig-vd.ezechiel.localdb";
}

//If not already done, close the database
LocalDB::~LocalDB()
{
    if (_db.isOpen())
        _db.close();
}

Descriptor LocalDB::getDescriptor()
{
    return Descriptor(_id, "Local DB", "Local SQLite database");
}

QString LocalDB::databaseName() const
{
    //Check if the connexion is opened
    if (!_db.isOpen())
        return QString();

    return _db.databaseName();
}

bool LocalDB::init(Core *core)
{
    Core::setInstance(core);
    return true;
}

QString LocalDB::errorString()
{
    return _db.lastError().text();
}


// Connexion //////////////////////////////////////////////////////////////////////////////////////

//Open the database file
bool LocalDB::connect(DBAccount account)
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
        url = CORE->path(Core::PluginsData) + QString("/%1/local.db").arg(_id);
    qDebug() << "Opening database : " + url;

    //Set the database type
    _db = QSqlDatabase::addDatabase("QSQLITE");

    //Set the database url
    _db.setDatabaseName(url);

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
bool LocalDB::disconnect()
{
    //Close the database
    _db.close();

    //Reset the encryption
    _encryption = Unknown;

    return true;
}

//We might use the url, but we do not request it
void LocalDB::needs(bool &url, bool &user, bool &password)
{
    url = false;
    user = false;
    password = false;
}

// Version ////////////////////////////////////////////////////////////////////////////////////////

Response LocalDB::getVersion(QString &version)
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

Response LocalDB::isCompatible(bool &compatible)
{
    Response response(_id);

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

Response LocalDB::getUser(const QString &username, User &user)
{
    Response response(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT authorization FROM User WHERE username = '%1'").arg(username)))
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

Response LocalDB::setUser(const User &user)
{
    Response response(_id);

    //Prepare the query
    QSqlQuery query;

    QString content;
    if (exists("User", "username", user.username()))
        content = QString("UPDATE User SET username = ?, authorization = ? WHERE username = '%1'").arg(user.username());
    else
        content = "INSERT INTO User (username, authorization) VALUES (?, ?)";

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

Response LocalDB::getUserKey(const QString &username, QString &key)
{
    Response response(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT key FROM User WHERE username = '%2'").arg(username)))
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

Response LocalDB::setUserKey(const QString &username, const QString &key)
{
    Response response(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("UPDATE User SET key = '%1' WHERE username = '%2'").arg(key, username)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return response;
}

Response LocalDB::deleteUser(const QString &username)
{
    Response response(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM User WHERE username = '%1'").arg(username)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return response;
}

Response LocalDB::getUsersList(QList<User> &list)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT username, authorization FROM User")))
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

Response LocalDB::getPractician(const ident &id, SharedPractician &practician)
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
//        practician = DBPractician();
        return r;
    }

    Practician *p = new Practician();
    practician = SharedPractician(p);
    practician->setId     (id);
    practician->setTitle    (query.value(0).toString());
    //practician->setPersonId 	  ( query.value(1).toInt());
    Identifier personId = query.value(1).toInt();
    Identifier instituteId =  ( query.value(2).isNull() ? -1 : query.value(2).toInt());
    practician->setExternalId ( query.value(3).toString());

    SharedPerson person = practician->person();
    r = getPerson(personId,person);

    SharedInstitute institute(new Institute());
    r = getInstitute(instituteId, institute);
    practician->setInstitute(institute);

    return r;
}

Response LocalDB::getPractician(const QString &externalId, SharedPractician &practician)
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
    //    practician = DBPractician();
        return r;
    }

    Practician *p = new Practician();
    practician = SharedPractician(p);
    practician->setExternalId( externalId);
    practician->setTitle( query.value(0).toString());
    //practician->setPersonId( query.value(1).toInt());
    practician->setInstitute_id( query.value(2).isNull() ? -1 : query.value(2).toInt());
    practician->setId( query.value(3).toInt());

    SharedPerson person = practician->person();
    r = getPerson(practician->person()->id(),person);

    return r;
}

Response LocalDB::setPractician(SharedPractician practician)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (practician->id() == invalid_ident)
        content = "INSERT INTO Practician (title, Person, Institute, external_id) VALUES (?, ?, ?, ?)";
    else
        content = QString("UPDATE Practician SET title = ?, Person = ?, Institute = ?, external_id = ? WHERE id = %1").arg(practician->id());

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(practician->title());
    query.addBindValue(practician->person()->id());
    query.addBindValue(practician->institute_id() == -1 ? QVariant(QVariant::Int) : practician->institute_id());
    query.addBindValue(practician->externalId());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && practician->id() == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted practician"));
        practician->setId(id.toInt());
    }

    Response response;
    //Save the person's data to the database and update the patient's person ID
    SharedPerson person = practician->person();
    response = setPerson(person);
    if (response.error != NoError) {
        //ERROR(response, QString(tr("Impossible to save person [id%1] to the database")).arg(patient->person()->id()));
        return response;
    };

    return r;
}

Response LocalDB::deletePractician(const ident &id)
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

Response LocalDB::getPracticiansList(QList<SharedPractician> &list)
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
        SharedPractician practician;
        practician = SharedPractician(new Practician());
        practician->setId ( query.value(0).toInt());
        practician->setTitle( query.value(1).toString());
        practician->setExternalId( query.value(4).toString());

        Identifier personId = query.value(2).toInt();
        SharedPerson person = practician->person();
        getPerson(personId,person);
        Identifier instituteId = ( query.value(3).isNull() ? -1 : query.value(3).toInt());
        SharedInstitute institute;
        getInstitute(instituteId,institute);
        practician->setInstitute(institute);


        if (practician->id() != invalid_ident)
            list.append(practician);
    }

    return r;
}


// Institutes /////////////////////////////////////////////////////////////////////////////////////
#include <QImage>
//Simple get
Response LocalDB::getInstitute(const ident &id, SharedInstitute &institute)
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
        institute = SharedInstitute(new Institute());
        return r;
    }

    institute = SharedInstitute(new Institute());

    //Set up the institute
    institute->setId(id);
    institute->setName(encrypted ? APPCORE->securityManager()->decrypt(query.value(0).toString()).toString() : query.value(0).toString());
    QByteArray im = query.value(1).toByteArray();
    institute->setLogo(QImage(im));
    institute->setExternalId(query.value(3).toString());
    SharedLocation location;
    getLocation(query.value(2).toInt(),location);
    institute->setLocation(location);

    return r;
}

Response LocalDB::getInstitute(const QString &externalId, SharedInstitute &institute)
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
        institute = SharedInstitute(new Institute());
        return r;
    }

    institute = SharedInstitute(new Institute());
    institute->setExternalId(externalId);
    institute->setName(encrypted ? APPCORE->securityManager()->decrypt(query.value(0).toString()).toString() : query.value(0).toString());
    QByteArray im = query.value(1).toByteArray();
    institute->setLogo(QImage(im));
    institute->setId(query.value(3).toInt());
    SharedLocation location;
    getLocation(query.value(2).toInt(),location);
    institute->setLocation(location);

    return r;
}

//Add or update it
Response LocalDB::setInstitute(const SharedInstitute &institute)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (institute->id() == invalid_ident)
        content = "INSERT INTO Institute (name, logo, Location, external_id) VALUES (?, ?, ?, ?)";
    else
        content = QString("UPDATE Institute SET name = ?, logo = ?, Location = ?, external_id = ? WHERE id = %1").arg(institute->id());

    if (!query.prepare(content))
        return queryError(query);

    //Bind the query values
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(institute->name()).toString() : institute->name());
    query.addBindValue(institute->logo());
    query.addBindValue(institute->location()->id());
    query.addBindValue(institute->externalId());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && institute->id() == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted location"));
        institute->setId( id.toInt());
    }

    return r;
}

//Simple remove
Response LocalDB::deleteInstitute(const ident &id)
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

Response LocalDB::getInstitutesList(QList<SharedInstitute> &list)
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
        SharedInstitute institute(new Institute());

        //Set up the institute
        institute->setId(query.value(0).toInt());
        institute->setName(encrypted ? APPCORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString());
        institute->setLogo(query.value(2).value<QImage>());
        institute->setExternalId(query.value(4).toString());

        Identifier locationId = query.value(3).toInt();
        SharedLocation location;
        getLocation(locationId,location);
        institute->setLocation(location);

        //Add it to the list
        if (institute->id() != invalid_ident)
            list.append(institute);
    }

    return r;
}


// Persons ////////////////////////////////////////////////////////////////////////////////////////

//Fetch this person data
Response LocalDB::getPerson(const ident &id, SharedPerson &person)
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
        //person = DBPerson();
        return r;
    }

    //Set up the person
    person->setId(id);
    person->setFirstname(encrypted ? APPCORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString());
    person->setName(encrypted ? APPCORE->securityManager()->decrypt(query.value(4).toString()).toString() : query.value(4).toString());
    person->setBirthday(encrypted ? QDate::fromString(APPCORE->securityManager()->decrypt(query.value(0).toString()).toString(), Qt::ISODate) : query.value(0).toDate());
    person->setGender(query.value(2).toBool() ? Person::Male:Person::Female);

    Identifier locationId = query.value(3).toInt();
    SharedLocation location;
    r = getLocation(locationId,location);
    if (r.error != NoError)
        return r;
    person->setLocation(location);


    //Prepare the query
    if (!query.prepare(QString("SELECT address FROM Email WHERE Person = '%1'").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    while (query.next()) {
        SharedEmail email = SharedEmail(new Email);
        email->setEmail(encrypted ? APPCORE->securityManager()->decrypt(query.value(0).toString()).toString() : query.value(0).toString());
        person->addEmail(email);
    }

    //Prepare the query
    if (!query.prepare(QString("SELECT id,number,Phone_Type FROM Phone WHERE Person = '%1'").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    while (query.next()) {

        //Set up the phone
        SharedPhone phone = SharedPhone(new Phone());
        phone->setId(query.value(0).toInt());
        phone->setNumber(encrypted ? APPCORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString());
        phone->setTypeFromString(encrypted ? APPCORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString());

        //Add it to the list
        person->addPhone(phone);
    }

    return r;
}

//Set the data
Response LocalDB::setPerson(SharedPerson &person)
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
    if (person->id() == invalid_ident)
        content = "INSERT INTO Person (name,firstname,gender,birthday,Location) VALUES (?,?,?,?,?)";
    else
        content = QString("UPDATE Person SET name = ?, firstname = ?, gender = ?, birthday = ?, Location = ? WHERE id = %1").arg(person->id());

    if (!query.prepare(content)) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Bind the query values
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(person->name()).toString()      : person->name());
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(person->firstname()).toString() : person->firstname());
    query.addBindValue(person->gender() == Person::Male ? "true":"false");
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(person->birthday().toString(Qt::ISODate)).toString() : person->birthday().toString(Qt::ISODate));
    query.addBindValue(person->location()->id());

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //On insert, fetch the row ID
    if (r.error == NoError && person->id() == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted person"));
        person->setId(id.toInt());
    }

    //Prepare the query
    if (!query.prepare(QString("DELETE FROM Email WHERE Person == %1").arg(person->id()))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Save the emails
    foreach (SharedEmail email, person->emails()) {

        //Prepare the query
        if (!query.prepare("INSERT INTO Email (address,Person) VALUES (?,?)")) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Add the values
        query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(email->email()).toString() : email->email());
        query.addBindValue(person->id());

        //Execute the query
        if (!query.exec()) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }
    }

    //Prepare the query
    if (!query.prepare(QString("DELETE FROM Phone WHERE Person == %1").arg(person->id()))) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Save the phones
    foreach (SharedPhone phone, person->phones()) {

        //Prepare the query
        if (!query.prepare("INSERT INTO Phone (number,Phone_Type,Person) VALUES (?,?,?)")) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Add the values
        query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(phone->number()).toString() : phone->number());
        query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(phone->typeToString()).toString()   : phone->typeToString());
        query.addBindValue(person->id());

        //Execute the query
        if (!query.exec()) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }
    }

    setLocation(person->location());

    return commitTransaction(transactionId);
}

//Deletes a person and the related location
Response LocalDB::deletePerson(const ident &id)
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

Response LocalDB::getPersonsList(QList<SharedPerson> &persons)
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
        SharedPerson person = SharedPerson(new Person());

        //Set up the person
        person->setId(query.value(0).toInt());
        person->setFirstname(encrypted ? APPCORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString());
        person->setName(encrypted ? APPCORE->securityManager()->decrypt(query.value(5).toString()).toString() : query.value(5).toString());
        person->setBirthday(encrypted ? QDate::fromString(APPCORE->securityManager()->decrypt(query.value(1).toString()).toString(), Qt::ISODate) : query.value(1).toDate());
        person->setGender(query.value(3).toBool() ? Person::Male: Person::Female);

        Identifier locationId = query.value(4).toInt();
        SharedLocation location;
        getLocation(locationId,location);
        person->setLocation(location);

        //Prepare the query
        QSqlQuery emailQuery;
        if (!emailQuery.prepare(QString("SELECT address FROM Email WHERE Person = '%1'").arg(person->id())))
            return queryError(emailQuery);

        //Execute the query
        if (!emailQuery.exec())
            return queryError(emailQuery);

        //Fetch the results
        while (emailQuery.next()) {
            SharedEmail email(new Email());
            email->setEmail((encrypted ? APPCORE->securityManager()->decrypt(emailQuery.value(0).toString()).toString() : emailQuery.value(0).toString()));
            person->addEmail(email);
        }

        //Prepare the query
        QSqlQuery phoneQuery;
        if (!phoneQuery.prepare(QString("SELECT id,number,Phone_Type FROM Phone WHERE Person = '%1'").arg(person->id())))
            return queryError(phoneQuery);

        //Execute the query
        if (!phoneQuery.exec())
            return queryError(phoneQuery);

        //Fetch the results
        while (phoneQuery.next()) {

            //Set up the phone
            SharedPhone phone(new Phone());
            phone->setId(phoneQuery.value(0).toInt());
            phone->setNumber(encrypted ? APPCORE->securityManager()->decrypt(phoneQuery.value(1).toString()).toString() : phoneQuery.value(1).toString());
            phone->setTypeFromString(encrypted ? APPCORE->securityManager()->decrypt(phoneQuery.value(2).toString()).toString() : phoneQuery.value(2).toString());

            //Add it to the list
            person->addPhone(phone);
        }

        //Add the person to the list
        if (person->id() != invalid_ident)
            persons.append(person);
    }

    return r;
}


// Locations //////////////////////////////////////////////////////////////////////////////////////


Response LocalDB::getLocation(const ident &id, SharedLocation &location)
{
    Response r(_id);
    Location *l;
    r = getLocation(id,l);
    location = SharedLocation(l);
    return r;
}

//Get this location
Response LocalDB::getLocation(const ident &id, Location *&location)
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
//        location = Location();
        return r;
    }
    location = new Location();

    //Set up the location
    location->setId(id);
    location->setCountry(query.value(0).toString());
    location->setState(encrypted ? APPCORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString());
    location->setAddress(encrypted ? APPCORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString());
    location->setCity(encrypted ? APPCORE->securityManager()->decrypt(query.value(3).toString()).toString() : query.value(3).toString());
    location->setPostcode(encrypted ? APPCORE->securityManager()->decrypt(query.value(4).toString()).toString() : query.value(4).toString());

    return r;
}

Response LocalDB::setLocation(SharedLocation location)
{
    return setLocation(location.data());
}

//Set or update the location
Response LocalDB::setLocation(Location *location)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (location->id() == invalid_ident)
        content = "INSERT INTO Location (country,state,address,city,postcode) VALUES (?,?,?,?,?)";
    else
        content = QString("UPDATE Location SET country = ?, state = ?, address = ?, city = ?, postcode = ? WHERE id = %1").arg(location->id());

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(location->country());
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(location->state()).toString()    : location->state());
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(location->address()).toString()  : location->address());
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(location->city()).toString()     : location->city());
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(location->postcode()).toString() : location->postcode());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && location->id() == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted location"));
        location->setId(id.toInt());
    }

    return r;
}

//Simpel remove
Response LocalDB::deleteLocation(const ident &id)
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
Response LocalDB::getLocationsList(QList<Location*> &locations)
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
        Location *location;
        location = new Location();

        //Set up the location
        location->setId       ( query.value(0).toInt());
        location->setCountry  ( query.value(1).toString());
        location->setState    ( encrypted ? APPCORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString());
        location->setAddress  ( encrypted ? APPCORE->securityManager()->decrypt(query.value(3).toString()).toString() : query.value(3).toString());
        location->setCity     ( encrypted ? APPCORE->securityManager()->decrypt(query.value(4).toString()).toString() : query.value(4).toString());
        location->setPostcode ( encrypted ? APPCORE->securityManager()->decrypt(query.value(5).toString()).toString() : query.value(5).toString());

        //Add it to the list
        if (location->id() != invalid_ident)
            locations.append(location);
    }

    return r;
}


// Patients ///////////////////////////////////////////////////////////////////////////////////////

//Return the patient data
Response LocalDB::getPatient(const ident &pid, SharedPatient &patient)
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
        patient = SharedPatient(new Patient());
        return r;
    }

    patient = SharedPatient(new Patient());
    patient->setId         ( query.value(0).toInt());
    patient->setStatOk     ( query.value(1).toBool());
    Identifier personId = query.value(2).toInt();
    patient->setExternalId ( query.value(3).toString());
    patient->setStayNumber ( query.value(4).toString());

    if (r.error != NoError)
        return r;

    SharedPerson person = patient->person();
    r = getPerson(personId,person);

    return r;
}

Response LocalDB::getPatient(const QString &externalId, SharedPatient &patient)
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
        patient = SharedPatient(new Patient());
        return r;
    }
    patient = SharedPatient(new Patient());


    patient->setExternalId ( externalId);
    patient->setStatOk     ( query.value(0).toBool());
    Identifier personId = query.value(1).toInt();
    patient->setStayNumber ( query.value(2).toString());
    patient->setId         ( query.value(3).toInt());


    if (r.error != NoError)
        return r;

    SharedPerson person = patient->person();
    r = getPerson(personId,person);

    return r;
}

//Update or create this patient
Response LocalDB::setPatient (SharedPatient &patient)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (patient->id() == invalid_ident)
        content = "INSERT INTO Patient (stat_ok, Person, external_id, stay_number) VALUES (?, ?, ?, ?)";
    else
        content = QString("UPDATE Patient SET stat_ok = ?, Person = ?, external_id = ?, stay_number = ? WHERE id = %1").arg(patient->id());

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(patient->stat_ok() ? "true":"false");
    query.addBindValue(patient->person()->id());
    query.addBindValue(patient->externalId());
    query.addBindValue(patient->stayNumber());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && patient->id() == invalid_ident) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted patient"));
        patient->setId( id.toInt());
    }

    Response response;
    //Save the person's data to the database and update the patient's person ID
    SharedPerson person = patient->person();
    response = setPerson(person);
    if (response.error != NoError) {
        //ERROR(response, QString(tr("Impossible to save person [id%1] to the database")).arg(patient->person()->id()));
        return response;
    };

    return r;
}

//Simple remove
Response LocalDB::deletePatient (const ident &id)
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
Response LocalDB::getPatientsList(QList<SharedPatient> &list)
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
        SharedPatient patient = SharedPatient(new Patient());

        //Set up the patient
        patient->setId         ( query.value(0).toInt());
        patient->setStatOk     ( query.value(1).toBool());
        Identifier personId =  ( query.value(2).toInt());
        patient->setExternalId ( query.value(3).toString());
        patient->setStayNumber ( query.value(4).toString());

        SharedPerson person = patient->person();
        getPerson(personId,person);


        //Add it to the list
        if (patient->id() != invalid_ident)
            list.append(patient);
    }

    return r;
}


// Measures ///////////////////////////////////////////////////////////////////////////////////////

//Return the measures linked with this patient, sorted by date
Response LocalDB::getMeasuresByDate(const ident &patient, QList<SharedMeasure> &list)
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
        SharedMeasure measure = SharedMeasure(new Measure());

        //Set up the measure
        measure->amount().setUnit(Unit(query.value(0).toString()));
        measure->amount().setValue(query.value(1).toDouble());
        measure->setMoment(query.value(2).toDateTime());
        measure->setId(query.value(3).toInt());
        measure->setSdrug(query.value(4).toString());
        measure->setSampleID(query.value(5).toString());
        measure->setArrivalDate(query.value(6).toDateTime());

        //Add it to the list
        if (measure->id() != -1)
            list.append(measure);
    }

    return r;
}

//Return the measures linked with this patient for the specidied drug, sorted by date
Response LocalDB::getMeasuresByDrug(const ident &patient, const QString &drugId, QList<SharedMeasure> &list)
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
        SharedMeasure measure = SharedMeasure(new Measure());

        //Set up the measure
        measure->amount().setUnit(Unit(query.value(0).toString()));
        measure->amount().setValue(query.value(1).toDouble());
        measure->setMoment(query.value(2).toDateTime());
        measure->setId(query.value(3).toInt());
        measure->setSdrug(query.value(4).toString());
        measure->setSampleID(query.value(5).toString());
        measure->setArrivalDate(query.value(6).toDateTime());

        //Add it to the list
        if (measure->id() != -1)
            list.append(measure);
    }

    return r;
}

Response LocalDB::setMeasure (const ident &patient, SharedMeasure measure)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (measure->id() == -1)
        content = "INSERT INTO Measure (Patient,date,value,unit,drug,sampleID,arrivalDate) VALUES (?,?,?,?,?,?,?)";
    else
        content = QString("UPDATE Measure SET Patient = ?, date = ?, value = ?, unit = ?, drug = ?, sampleID = ?, arrivalDate = ? WHERE id = %1").arg(measure->id());

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(patient);
    query.addBindValue(measure->moment());
    query.addBindValue(measure->amount().value());
    query.addBindValue(measure->amount().unit().toString());
    query.addBindValue(findOrCreate(measure->sdrug(), "Drug"));
    query.addBindValue(measure->sampleID());
    query.addBindValue(measure->arrivalDate());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && measure->id() == -1) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted measure"));
        measure->setId(id.toInt());
    }

    return r;
}

Response LocalDB::deleteMeasure (const ident &id)
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
Response LocalDB::getClinicalsByDate(const ident &patient, QList<SharedClinical> &list)
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
        SharedClinical clinical = SharedClinical(new Clinical());

        //Set up the clinical
        clinical->setName(query.value(0).toString());
        clinical->setValue(encrypted ? APPCORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString());
        clinical->setDate(encrypted ? QDateTime::fromString(APPCORE->securityManager()->decrypt(query.value(2).toString()).toString(), Qt::ISODate) : query.value(2).toDateTime());
        clinical->setId(query.value(3).toInt());

        //Add it to the list
        if (clinical->id() != -1)
            list.append(clinical);
    }

    return r;
}

Response LocalDB::setClinical(const ident &patient, SharedClinical &clinical)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (clinical->id() == -1)
        content = "INSERT INTO Clinicals (Patient,date,value,type) VALUES (?,?,?,?)";
    else
        content = QString("UPDATE Clinicals SET Patient = ?, date = ?, value = ?, type = ? WHERE id = %1").arg(clinical->id());

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(patient);
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(clinical->date().toString(Qt::ISODate)).toString() : clinical->date().toString(Qt::ISODate));
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(clinical->value()).toString() : clinical->value());
    query.addBindValue(clinical->id());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && clinical->id() == -1) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted clinical"));
        clinical->setId(id.toInt());
    }

    return r;
}

Response LocalDB::deleteClinical(const ident &id)
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
Response LocalDB::getCovariatesByDate(const ident &id, QList<SharedPatientVariate> &list)
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
        SharedPatientVariate covariate = SharedPatientVariate(new PatientVariate(query.value(0).toString(), query.value(1).toDateTime(), query.value(2).toDouble()));
        covariate->setId(query.value(3).toInt());

        if (covariate->id() != -1)
            list.append(covariate);
    }

    return r;
}

Response LocalDB::setCovariate (const ident &patient, SharedPatientVariate &covariate)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (covariate->id() == -1)
        content = "INSERT INTO Covariate (Patient,date,value,mid) VALUES (?,?,?,?)";
    else
        content = QString("UPDATE Covariate SET Patient = ?, date = ?, value = ?, mid = ? WHERE sql_id = %1").arg(covariate->id());

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(patient);
    query.addBindValue(covariate->date());
    query.addBindValue(covariate->value());
    query.addBindValue(covariate->id());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && covariate->id() == -1) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted covariate"));
        covariate->setId(id.toInt());
    }

    return r;
}

Response LocalDB::deleteCovariate(const ident &id)
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

// Reports ////////////////////////////////////////////////////////////////////////////////////////

Response LocalDB::getReport(const ident &id, ReportData &data)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Report WHERE id = '%2'").arg("template").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The report report ID '%1' could not be found.").arg(id);
        r.error   = NotFound;
        return r;
    }

    //Set up the report
    data.setReportId(id);
    data.setTemplateName(query.value(0).toString());

    //Prepare the query
    if (!query.prepare(QString("SELECT %1 FROM ReportValue WHERE reportId = '%2'").arg("id, value").arg(data.reportId())))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    while (query.next())
        data.setValue(query.value(0).toString(), (encrypted ? APPCORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString()));

    //Prepare the query
    if (!query.prepare(QString("SELECT %1 FROM ReportImage WHERE reportId = '%2'").arg("id, image").arg(data.reportId())))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    while (query.next()) {
        if (encrypted) {

            //Get the encrypted value as byte array
            QByteArray encryptedArray = query.value(1).toByteArray();

            //Convert it to std::string and decrypt
            std::string encryptedStr(encryptedArray.constData(), encryptedArray.length());
            std::string plainStr(APPCORE->securityManager()->decrypt(QVariant::fromValue<std::string>(encryptedStr)).value<std::string>());

            //Convert it to QImage and store it
            data.setImage(query.value(0).toString(), QImage::fromData(QByteArray(plainStr.c_str(), plainStr.length())));
        } else {
            data.setImage(query.value(0).toString(), QImage::fromData(query.value(1).toByteArray()));
        }
    }

    //Prepare the query
    if (!query.prepare(QString("SELECT %1 FROM ReportHiddenSection WHERE reportId = '%2'").arg("section").arg(data.reportId())))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    while (query.next())
        data.hideSection(query.value(0).toString());

    return r;
}

Response LocalDB::getReport(const QString &field, const QVariant &value, ReportData &data)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT id FROM Report WHERE %1 = :value").arg(field)))
        return queryError(query);

    //Bind the value
    query.bindValue(":value", value);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    if (!query.next()) {
        r.message = tr("The report with the field '%1' set to '%2' could not be found.").arg(field, value.toString());
        r.error   = NotFound;
        return r;
    }

    //Cache the result
    int id = query.value(0).toInt();

    //Check if unique
    if (query.next()) {
        r.message = tr("The field '%1' set to '%2' matches more than one reports.").arg(field, value.toString());
        r.error   = NotFound;
        return r;
    }

    return getReport(id, data);
}

Response LocalDB::setReport(ReportData &data)
{
    Response r(_id);

    //Get the encryption status
    bool encrypted;
    if ((r = isEncrypted(encrypted)).error != NoError)
        return r;

    //Start a new transaction
    QString transactionId = "__setReport";
    if ((r = startTransaction(transactionId)).error != NoError)
        return r;

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (data.reportId() == invalid_ident)
        content = "INSERT INTO Report (template, title, reference, creation, publication, patientId, analystId, recipientId, instituteId, treatmentId) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    else
        content = QString("UPDATE Report SET template = ?, title = ?, reference = ?, creation = ?, publication = ?, patientId = ?, analystId = ?, recipientId = ?, instituteId = ?, treatmentId = ? WHERE id = %1").arg(data.reportId());

    if (!query.prepare(content)) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    //Bind the query values
    query.addBindValue(data.templateName());
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(data.title()).toString()     : data.title());
    query.addBindValue(data.reference());
    query.addBindValue(data.creationDate());
    query.addBindValue(data.publicationDate());
    query.addBindValue(data.patientId() == -1   ? QVariant(QVariant::Int) : data.patientId());
    query.addBindValue(data.analystId() == -1   ? QVariant(QVariant::Int) : data.analystId());
    query.addBindValue(data.recipientId() == -1 ? QVariant(QVariant::Int) : data.recipientId());
    query.addBindValue(data.instituteId() == -1 ? QVariant(QVariant::Int) : data.instituteId());
    query.addBindValue(data.treatmentId() == -1 ? QVariant(QVariant::Int) : data.treatmentId());

    //Execute the query
    if (!query.exec()) {
        rollbackTransaction(transactionId);
        return queryError(query);
    }

    int id;

    //Delete previous data
    if (data.reportId() != invalid_ident) {

        //Prepare the query
        if (!query.prepare(QString("DELETE FROM ReportValue WHERE reportId == %1").arg(data.reportId()))) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Execute the query
        if (!query.exec()) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Prepare the query
        if (!query.prepare(QString("DELETE FROM ReportImage WHERE reportId == %1").arg(data.reportId()))) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Execute the query
        if (!query.exec()) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Prepare the query
        if (!query.prepare(QString("DELETE FROM ReportHiddenSection WHERE reportId == %1").arg(data.reportId()))) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Execute the query
        if (!query.exec()) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        id = data.reportId();
    } else {
        QVariant rawId = query.lastInsertId();
        if (!rawId.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted report"));
        id = rawId.toInt();
    }

    //Save the values
    const QHash<QString, QString> values = data.values();
    for (QHash<QString, QString>::const_iterator i = values.constBegin(); i != values.end(); ++i) {

        //Prepare the query
        if (!query.prepare("INSERT INTO ReportValue (reportId, id, value) VALUES (?, ?, ?)")) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Add the values
        query.addBindValue(id);
        query.addBindValue(i.key());
        query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(i.value()).toString() : i.value());

        //Execute the query
        if (!query.exec()) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }
    }

    //Save the images
    const QHash<QString, QImage> images = data.images();
    for (QHash<QString, QImage>::const_iterator i = images.constBegin(); i != images.end(); ++i) {

        //Prepare the query
        if (!query.prepare("INSERT INTO ReportImage (reportId, id, image) VALUES (?, ?, ?)")) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Serialize the image
        QByteArray plainArray;
        QBuffer buffer(&plainArray);
        buffer.open(QIODevice::WriteOnly);
        i.value().save(&buffer, "PNG");

        //Add the ID and key
        query.addBindValue(id);
        query.addBindValue(i.key());

        //Add the image
        if (encrypted) {

            //Convert the byte array to std::string and encrypt
            std::string plainStr(plainArray.constData(), plainArray.length());
            std::string encryptedStr(APPCORE->securityManager()->encrypt(QVariant::fromValue<std::string>(plainStr)).value<std::string>());

            //Bind the crypted byte array in the query
            query.addBindValue(QByteArray(encryptedStr.c_str(), encryptedStr.length()));
        } else {
            query.addBindValue(plainArray);
        }

        //Execute the query
        if (!query.exec()) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }
    }

    //Save the sections
    const QStringList sections = data.hiddenSections();
    for (QStringList::const_iterator i = sections.constBegin(); i != sections.end(); ++i) {

        //Prepare the query
        if (!query.prepare("INSERT INTO ReportHiddenSection (reportId, section) VALUES (?, ?)")) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }

        //Add the section
        query.addBindValue(id);
        query.addBindValue(*i);

        //Execute the query
        if (!query.exec()) {
            rollbackTransaction(transactionId);
            return queryError(query);
        }
    }

    //Set the report ID
    if (data.reportId() == invalid_ident)
        data.setReportId(id);

    return commitTransaction(transactionId);
}

Response LocalDB::deleteReport(ReportData &data)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM Report WHERE id = %1").arg(data.reportId())))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Reset the DB ID
    data.setReportId(invalid_ident);

    return r;
}

Response LocalDB::getReportsList(QList<ReportData *> &dataList, const QString &where)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM Report%2").arg("id").arg(where)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the result
    qDeleteAll(dataList);
    dataList.clear();

    while (query.next()) {
        QScopedPointer<ReportData> data(new ReportData());

        //Get the current report
        if ((r = getReport(query.value(0).toInt(), *data.data())).error != NoError) {
            qDeleteAll(dataList);
            dataList.clear();
            return r;
        }

        //Add it to the list
        dataList.append(data.take());
    }

    return r;
}

// Curves /////////////////////////////////////////////////////////////////////////////////////////

Response LocalDB::getCurve(const ident &id, DBCurve &curve)
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
    curve.name             = encrypted ? APPCORE->securityManager()->decrypt(query.value(0).toString()).toString() : query.value(0).toString();
    curve.comment          = encrypted ? APPCORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString();
    curve.firsttake        = query.value(2).toDateTime();
    curve.pType            = ParameterType(query.value(3).toInt());
    curve.cType            = Prediction::CurveType(query.value(4).toInt());
    curve.patient          = query.value(5).toInt();
    curve.drug             = query.value(6).toString();
    curve.model            = query.value(7).toString();
    curve.percentileEngine = query.value(8).toString();
    curve.postEngine       = query.value(9).toString();
    curve.reverseEngine    = query.value(10).toString();

    return r;
}

Response LocalDB::setCurve(DBCurve &curve)
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
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(curve.name).toString()    : curve.name);
    query.addBindValue(encrypted ? APPCORE->securityManager()->encrypt(curve.comment).toString() : curve.comment);
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

Response LocalDB::deleteCurve(ident id)
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

Response LocalDB::getCurvesList(QList<DBCurve> &curves)
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
        curve.name             = encrypted ? APPCORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString();
        curve.comment          = encrypted ? APPCORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString();
        curve.firsttake        = query.value(3).toDateTime();
        curve.pType            = ParameterType(query.value(4).toInt());
        curve.cType            = Prediction::CurveType(query.value(5).toInt());
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

Response LocalDB::getCurvesList(QList<DBCurve> &curves, const ident patientId, const QString &drugId)
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
        curve.name             = encrypted ? APPCORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString();
        curve.comment          = encrypted ? APPCORE->securityManager()->decrypt(query.value(2).toString()).toString() : query.value(2).toString();
        curve.firsttake        = query.value(3).toDateTime();
        curve.pType            = ParameterType(query.value(4).toInt());
        curve.cType            = Prediction::CurveType(query.value(5).toInt());
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

Response LocalDB::getCurvesList(QList<ident> &ids, QStringList &names)
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
        names.append(encrypted ? APPCORE->securityManager()->decrypt(query.value(1).toString()).toString() : query.value(1).toString());
    }

    return r;
}


// Dosages ////////////////////////////////////////////////////////////////////////////////////////

Response LocalDB::getDosagesByDate (const ident &curve, QList<SharedDosage> &list)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT %1 FROM SharedDosage WHERE Curve = %2 ORDER BY date").arg("value,unit,interval,tinf,date,id").arg(curve)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //Fetch the results
    list.clear();
    while (query.next()) {
        SharedDosage dosage;

        //Set up the dosage
        dosage->dose().setValue(query.value(0).toDouble());
        dosage->dose().setUnit(Unit(query.value(1).toString()));
        dosage->setInterval(Duration(0,0,query.value(2).toDouble()));
        dosage->setTinf(Duration(0,0,query.value(3).toDouble()));
        dosage->setApplied(query.value(4).toDateTime());
        dosage->setId(query.value(5).toInt());

        //Add it to the list
        if (dosage->id() != -1)
            list.append(dosage);
    }

    return r;
}

Response LocalDB::setDosage (const ident &curve, SharedDosage &dosage)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (dosage->id() == -1)
        content = "INSERT INTO SharedDosage (Curve,value,unit,interval,tinf,date) VALUES (?, ?, ?, ?, ?, ?)";
    else
        content = QString("UPDATE SharedDosage SET Curve = ?, value = ?, unit = ?, interval = ?, tinf = ?, date = ? WHERE id = %1").arg(dosage->id());

    if (!query.prepare(content))
        return queryError(query);

    //Add the values
    query.addBindValue(curve);
    query.addBindValue(dosage->dose().value());
    query.addBindValue(dosage->dose().unit().toString());
    query.addBindValue(dosage->interval().toSeconds());
    query.addBindValue(dosage->tinf().toSeconds());
    query.addBindValue(dosage->applied());

    //Execute the query
    if (!query.exec())
        return queryError(query);

    //On insert, fetch the row ID
    if (r.error == NoError && dosage->id() == -1) {
        QVariant id = query.lastInsertId();
        if (!id.isValid())
            ERROR(MISSINGID, tr("Could not retrieve the database ID of the newly inserted dosage"));
        dosage->setId(id.toInt());
    }

    return r;
}

Response LocalDB::deleteDosage(const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM SharedDosage WHERE id = %1").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return r;
}

// Models /////////////////////////////////////////////////////////////////////////////////////////

Response LocalDB::getModelsOptions(const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option FROM MxO WHERE Curve = '%1'").arg(QString::number(curveId))))
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

Response LocalDB::getModelOptions(const QString &modelId, const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option FROM MxO WHERE Curve = '%1' AND Model = '%2'").arg(QString::number(curveId), QString::number(findOrCreate(modelId, "Model")))))
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

Response LocalDB::setModelOption(const QString &modelId, const ident &curveId, Option &option)
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
    if (!query.prepare("INSERT INTO MxO (Model, Option, Curve) VALUES (?, ?, ?)")) {
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

Response LocalDB::deleteModelsOptions(const ident &curveId)
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

Response LocalDB::deleteModelOption(const QString &modelId, const ident &curveId, const ident &optionId)
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

Response LocalDB::getPEsOptions(const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option FROM ExO WHERE Curve = '%1'").arg(QString::number(curveId))))
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

Response LocalDB::getPEOptions(const QString &peId, const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option FROM ExO WHERE Curve = '%1' AND Prediction_Engine = '%2'").arg(QString::number(curveId), QString::number(findOrCreate(peId, "Percentile_Engine")))))
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

Response LocalDB::setPEOption(const QString &peId, const ident &curveId, Option &option)
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
    if (!query.prepare("INSERT INTO ExO (Prediction_Engine, Option, Curve) VALUES (?, ?, ?)")) {
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

Response LocalDB::deletePEsOptions(const ident &curveId)
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

Response LocalDB::deletePEOption(const QString &peId, const ident &curveId, const ident &optionId)
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

Response LocalDB::getPOsOptions(const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option FROM PxO WHERE Curve = '%1'").arg(QString::number(curveId))))
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

Response LocalDB::getPOOptions(const QString &poId, const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option FROM PxO WHERE Curve = '%1' AND Post_Engine = '%2'").arg(QString::number(curveId), QString::number(findOrCreate(poId, "Post_Engine")))))
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

Response LocalDB::setPOOption(const QString &poId, const ident &curveId, Option &option)
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
    if (!query.prepare("INSERT INTO PxO (Post_Engine, Option, Curve) VALUES (?, ?, ?)")) {
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

Response LocalDB::deletePOsOptions(const ident &curveId)
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

Response LocalDB::deletePOOption(const QString &poId, const ident &curveId, const ident &optionId)
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

Response LocalDB::getREsOptions(const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option FROM RxO WHERE Curve = '%1'").arg(QString::number(curveId))))
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

Response LocalDB::getREOptions(const QString &reId, const ident &curveId, QList<Option> &options)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT Option FROM RxO WHERE Curve = '%1' AND Reverse_Engine = '%2'").arg(QString::number(curveId), QString::number(findOrCreate(reId, "Reverse_Engine")))))
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

Response LocalDB::setREOption(const QString &reId, const ident &curveId, Option &option)
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
    if (!query.prepare("INSERT INTO RxO (Reverse_Engine, Option, Curve) VALUES (?, ?, ?)")) {
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

Response LocalDB::deleteREsOptions(const ident &curveId)
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

Response LocalDB::deleteREOption(const QString &reId, const ident &curveId, const ident &optionId)
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

Response LocalDB::getOption(const ident &id, Option &option)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("SELECT id, value FROM Option WHERE sql_id = '%1'").arg(id)))
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

Response LocalDB::setOption(Option &option)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    QString content;
    if (option.sqlId == invalid_ident)
        content = "INSERT INTO Option (id, value) VALUES (?, ?)";
    else
        content = QString("UPDATE Option SET value = ? WHERE sql_id = '%1' AND id = '%2'").arg(QString::number(option.sqlId), option.oid);

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

Response LocalDB::deleteOption(const ident &id)
{
    Response r(_id);

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare(QString("DELETE FROM Option WHERE sql_id = '%1'").arg(id)))
        return queryError(query);

    //Execute the query
    if (!query.exec())
        return queryError(query);

    return r;
}

// Encryption /////////////////////////////////////////////////////////////////////////////////////

Response LocalDB::isEncrypted(bool &encrypted)
{
    Response r(_id);

    //Check if available
    if (_encryption != Unknown) {
        encrypted = _encryption == Encrypted;
        return r;
    }

    //Prepare the query
    QSqlQuery query;
    if (!query.prepare("SELECT value FROM Encryption WHERE key = 'encrypted'"))
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

Response LocalDB::setEncrypted(bool encrypted)
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
    if (!query.prepare("UPDATE Encryption SET value = ? WHERE key = 'encrypted'"))
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

Response LocalDB::encryptDatabase(bool encrypt)
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
bool LocalDB::enableFkSupport()
{
    QSqlQuery query("PRAGMA foreign_keys = ON");
    return query.isActive();
}

//Find the ID or create this element
ident LocalDB::findOrCreate (const QString &id, const QString &table)
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
Response LocalDB::queryError(const QSqlQuery &query) const
{
    Response response(_id);

    response.message = QString("[Code %1] %2;\n%3.").arg(QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
    response.error   = QueryError;

    return response;
}

//Return an error message on a database failure
Response LocalDB::databaseError(const QSqlDatabase &database, const QString &message) const
{
    Response response(_id);

    response.message = QString("%1 The database returned the following error:\n\n[Code %2] %3;\n%4.").arg(message, QString::number(database.lastError().number()), database.lastError().databaseText(), database.lastError().driverText());
    response.error   = QueryError;

    return response;
}

//Starts a transaction
Response LocalDB::startTransaction(const QString &id)
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
Response LocalDB::rollbackTransaction(const QString &id)
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
Response LocalDB::commitTransaction(const QString &id)
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
bool LocalDB::exists(const QString &table, const QString &pk, const QString &value)
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
bool LocalDB::exists(const QString &table, const QString &pk, ident value)
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

Response LocalDB::encryptPersons(bool encrypt)
{
    Response response(_id);

    //Get the list of persons
    QList<SharedPerson> persons;

    if ((response = getPersonsList(persons)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each person
    for (int i = 0; i < persons.size(); i++) {
        if ((response = setPerson(persons[i])).error != NoError) {
            _encryption = encrypt ? Clear : Encrypted;
            return response;
        }
    }

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response LocalDB::encryptPatients(bool encrypt)
{
    Response response(_id);

    //Get the list of patients
    QList<SharedPatient> patients;

    if ((response = getPatientsList(patients)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each patient
    for (int i = 0; i < patients.size(); i++) {
        if ((response = setPatient(patients[i])).error != NoError) {
            _encryption = encrypt ? Clear : Encrypted;
            return response;
        }
    }

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response LocalDB::encryptPracticians(bool encrypt)
{
    Response response(_id);

    //Get the list of practicians
    QList<SharedPractician> practicians;

    if ((response = getPracticiansList(practicians)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each practician
    for (int i = 0; i < practicians.size(); i++) {
        if ((response = setPractician(practicians[i])).error != NoError) {
            _encryption = encrypt ? Clear : Encrypted;
            return response;
        }
    }

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response LocalDB::encryptLocations(bool encrypt)
{
    Response response(_id);

    //Get the list of locations
    QList<Location*> locations;

    if ((response = getLocationsList(locations)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each location
    for (int i = 0; i < locations.size(); i++) {
        if ((response = setLocation(locations[i])).error != NoError) {
            _encryption = encrypt ? Clear : Encrypted;
            return response;
        }
    }

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response LocalDB::encryptInstitutes(bool encrypt)
{
    Response response(_id);

    //Get the list of institutes
    QList<SharedInstitute> institutes;

    if ((response = getInstitutesList(institutes)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each institute
    for (int i = 0; i < institutes.size(); i++) {
        if ((response = setInstitute(institutes[i])).error != NoError) {
            _encryption = encrypt ? Clear : Encrypted;
            return response;
        }
    }

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response LocalDB::encryptReports(bool encrypt)
{
    Response response(_id);

    //Get the list of reports
    QList<ReportData *> dataList;

    if ((response = getReportsList(dataList)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each report
    for (int i = 0; i < dataList.size(); i++) {
        if ((response = setReport(*(dataList[i]))).error != NoError) {
            _encryption = encrypt ? Clear : Encrypted;
            qDeleteAll(dataList);
            return response;
        }
    }

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    //Delete the list of reports
    qDeleteAll(dataList);

    return response;
}

Response LocalDB::encryptCovariates(bool encrypt)
{
    Response response(_id);

    //Get the list of patients
    QList<SharedPatient> patients;
    if ((response = getPatientsList(patients)).error != NoError)
        return response;

    //Update each patient's covariates
    for (int i = 0; i < patients.size(); i++) {

        //Get the list of covariates
        QList<SharedPatientVariate> covariates;

        if ((response = getCovariatesByDate(patients.at(i)->id(), covariates)).error != NoError)
            return response;

        //Set the encryption status
        _encryption = encrypt ? Encrypted : Clear;

        //Update each covariate
        for (int j = 0; j < covariates.size(); j++) {
            if ((response = setCovariate(patients.at(i)->id(), covariates[j])).error != NoError) {
                _encryption = encrypt ? Clear : Encrypted;
                return response;
            }
        }

        //Reset the encryption status
        _encryption = encrypt ? Clear : Encrypted;
    }

    return response;
}

Response LocalDB::encryptClinicals(bool encrypt)
{
    Response response(_id);

    //Get the list of patients
    QList<SharedPatient> patients;
    if ((response = getPatientsList(patients)).error != NoError)
        return response;

    //Update each patient's clinicals
    for (int i = 0; i < patients.size(); i++) {

        //Get the list of clinicals
        QList<SharedClinical> clinicals;

        if ((response = getClinicalsByDate(patients.at(i)->id(), clinicals)).error != NoError)
            return response;

        //Set the encryption status
        _encryption = encrypt ? Encrypted : Clear;

        //Update each clinical
        for (int j = 0; j < clinicals.size(); j++) {
            if ((response = setClinical(patients.at(i)->id(), clinicals[j])).error != NoError) {
                _encryption = encrypt ? Clear : Encrypted;
                return response;
            }
        }

        //Reset the encryption status
        _encryption = encrypt ? Clear : Encrypted;
    }

    return response;
}

Response LocalDB::encryptMeasures(bool encrypt)
{
    Response response(_id);

    //Get the list of patients
    QList<SharedPatient> patients;
    if ((response = getPatientsList(patients)).error != NoError)
        return response;

    //Update each patient's measures
    for (int i = 0; i < patients.size(); i++) {

        //Get the list of measures
        QList<SharedMeasure> measures;

        if ((response = getMeasuresByDate(patients.at(i)->id(), measures)).error != NoError)
            return response;

        //Set the encryption status
        _encryption = encrypt ? Encrypted : Clear;

        //Update each measure
        for (int j = 0; j < measures.size(); j++) {
            if ((response = setMeasure(patients.at(i)->id(), measures[j])).error != NoError) {
                _encryption = encrypt ? Clear : Encrypted;
                return response;
            }
        }

        //Reset the encryption status
        _encryption = encrypt ? Clear : Encrypted;
    }

    return response;
}

Response LocalDB::encryptCurves(bool encrypt)
{
    Response response(_id);

    //Get the list of curves
    QList<DBCurve> curves;

    if ((response = getCurvesList(curves)).error != NoError)
        return response;

    //Set the encryption status
    _encryption = encrypt ? Encrypted : Clear;

    //Update each curve
    for (int i = 0; i < curves.size(); i++) {
        if ((response = setCurve(curves[i])).error != NoError) {
            _encryption = encrypt ? Clear : Encrypted;
            return response;
        }
    }

    //Reset the encryption status
    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Response LocalDB::encryptDosages(bool encrypt)
{
    Response response(_id);

    //Get the list of curves
    QList<DBCurve> curves;

    if ((response = getCurvesList(curves)).error != NoError)
        return response;

    //Update each curve's dosages
    for (int i = 0; i < curves.size(); i++) {

        //Get the list of dosages
        QList<SharedDosage> dosages;

        if ((response = getDosagesByDate(curves.at(i).id, dosages)).error != NoError)
            return response;

        //Set the encryption status
        _encryption = encrypt ? Encrypted : Clear;

        //Update each dosages
        for (int j = 0; j < dosages.size(); j++) {
            if ((response = setDosage(curves.at(i).id, dosages[j])).error != NoError) {
                _encryption = encrypt ? Clear : Encrypted;
                return response;
            }
        }

        //Reset the encryption status
        _encryption = encrypt ? Clear : Encrypted;
    }

    return response;
}
