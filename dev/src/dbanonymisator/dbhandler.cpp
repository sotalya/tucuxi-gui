//@@license@@

#include "dbhandler.h"

#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>

DBHandler *DBHandler::handler = 0;

DBHandler::DBHandler()
{
}

DBHandler* DBHandler::instance()
{
    if(!handler)
        handler = new DBHandler();
    return handler;
}

bool DBHandler::connect(QString dbName)
{

    if(dbName == "")
    {
        dbName = path;
    }
    //If the driver is not available, print a list of available ones
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qDebug() << tr("SQLite driver unavailable");
        qDebug() << ("List of available drivers : " + QSqlDatabase::drivers().join("\n"));
        return false;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    return db.open() && enableFkSupport();
}

bool DBHandler::disconnect()
{
    db.close();
    return true;
}

bool DBHandler::hasBasicFingerPrint()
{
    bool measure = false;
    bool curve = false;
    bool dosage = false;
    bool patient = false;
    bool person = false;
    bool practician = false;
    bool report = false;
    bool institute = false;
    bool covariate = false;
    bool post_engine = false;
    bool percentile_engine = false;

    QSqlQuery query(QString("SELECT name FROM sqlite_master WHERE type='table';"));

    while(query.next())
    {
        measure = measure || query.value(0).toString().toLower() == "measure";
        curve = curve || query.value(0).toString().toLower() == "curve";
        dosage = dosage || query.value(0).toString().toLower() == "dosage";
        patient = patient || query.value(0).toString().toLower() == "patient";
        person = person || query.value(0).toString().toLower() == "person";
        practician = practician || query.value(0).toString().toLower() == "practician";
        report = report || query.value(0).toString().toLower() == "report";
        institute = institute || query.value(0).toString().toLower() == "institute";
        covariate = covariate || query.value(0).toString().toLower() == "covariate";
        post_engine = post_engine || query.value(0).toString().toLower() == "post_engine";
        percentile_engine = percentile_engine || query.value(0).toString().toLower() == "percentile_engine";
    }

    return measure && curve && dosage && patient && person && practician && report && institute && covariate && post_engine && percentile_engine;
}

bool DBHandler::enableFkSupport()
{
    QSqlQuery query("PRAGMA foreign_keys = ON");

    return query.isActive();
}

void DBHandler::setDBPath(QString path)
{
    this->path = path;
}

void DBHandler::setAnonDBName(QString name)
{
    this->name = name;
}

void DBHandler::evaluate()
{
    bool success = true;
    //Connexion to the database
    if(connect())
    {
        success = hasBasicFingerPrint();
    }
    else
    {
        qDebug() << "UNABLE TO CONNECT TO THE DATABASE. ";
        success = false;
    }

    emit publishResult(success);
    return;
}

void DBHandler::anonymizeDB()
{
    QFile originalDB(path);
    originalDB.copy(name);

    //Disconnect from the database
    if(!disconnect())
    {
        errMsg = "Could not disconnect from the database";
        emit publishAnonymizeStatus(false);
        return;
    }

    //Reconnect to the database
    if (!connect(name)) {
        errMsg = "Could not reconnect to the database";
        emit publishAnonymizeStatus(false);
        return;
    }

    //Update the men
    QSqlQuery men;
    if (!men.prepare(QString("UPDATE Person SET name = ('Doe_' || id), firstname = ('John_' || id) WHERE gender = 'true';"))) {
        errMsg = QString("Invalid query for men (code %1);\n%2;\n%3.").arg(QString::number(men.lastError().number()), men.lastError().databaseText(), men.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    if (!men.exec()) {
        errMsg = QString("Could not update the men (code %1);\n%2;\n%3.").arg(QString::number(men.lastError().number()), men.lastError().databaseText(), men.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    //Update the women
    QSqlQuery women;
    if (!women.prepare(QString("UPDATE Person SET name = ('Doe_' || id), firstname = ('Jane_' || id) WHERE gender = 'false';"))) {
        errMsg = QString("Invalid query for women (code %1);\n%2;\n%3.").arg(QString::number(women.lastError().number()), women.lastError().databaseText(), women.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    if (!women.exec()) {
        errMsg = QString("Could not update the women (code %1);\n%2;\n%3.").arg(QString::number(women.lastError().number()), women.lastError().databaseText(), women.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    //Update the locations
    QSqlQuery locations;
    if (!locations.prepare(QString("UPDATE Location SET country = ('Country_' || id), state = ('State_' || id), address = ('Address_' || id), city = ('City_' || id), postcode = ('Postcode_' || id);"))) {
        errMsg = QString("Invalid query for locations (code %1);\n%2;\n%3.").arg(QString::number(locations.lastError().number()), locations.lastError().databaseText(), locations.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    if (!locations.exec()) {
        errMsg = QString("Could not update the locations (code %1);\n%2;\n%3.").arg(QString::number(locations.lastError().number()), locations.lastError().databaseText(), locations.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    //Update the emails
    QSqlQuery emails;
    if (!emails.prepare(QString("UPDATE Email SET address = ('someone_' || id || '@somewhere.com');"))) {
        errMsg = QString("Invalid query for emails (code %1);\n%2;\n%3.").arg(QString::number(emails.lastError().number()), emails.lastError().databaseText(), emails.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    if (!emails.exec()) {
        errMsg = QString("Could not update the emails (code %1);\n%2;\n%3.").arg(QString::number(emails.lastError().number()), emails.lastError().databaseText(), emails.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    //Update the phone numbers
    QSqlQuery phones;
    if (!phones.prepare(QString("UPDATE Phone SET number = NULL;"))) {
        errMsg = QString("Invalid query for phones (code %1);\n%2;\n%3.").arg(QString::number(phones.lastError().number()), phones.lastError().databaseText(), phones.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    if (!phones.exec()) {
        errMsg = QString("Could not clear the phone numbers (code %1);\n%2;\n%3.").arg(QString::number(phones.lastError().number()), phones.lastError().databaseText(), phones.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    //Update the curves names
    QSqlQuery curves;
    if (!curves.prepare(QString("UPDATE Curve SET name = ('Curve_' || id);"))) {
        errMsg = QString("Invalid query for curves (code %1);\n%2;\n%3.").arg(QString::number(curves.lastError().number()), curves.lastError().databaseText(), curves.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    if (!curves.exec()) {
        errMsg = QString("Could not clear the curves names (code %1);\n%2;\n%3.").arg(QString::number(curves.lastError().number()), curves.lastError().databaseText(), curves.lastError().driverText());
        emit publishAnonymizeStatus(false);
        return;
    }

    emit publishAnonymizeStatus(true);
}

QString DBHandler::getErrMsg()
{
    return errMsg;
}
