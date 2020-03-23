#include "dbhandler.h"

#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>

DBHandler *DBHandler::handler = 0;

DBHandler::DBHandler()
{
    versions << "0.2.1";
    versions << "0.2.2";
    versions << "0.2.3";
    versions << "0.2.4";
    versions << "0.2.5";
    versions << "0.2.6";
    versions << "0.2.7";
}

DBHandler* DBHandler::instance()
{
    if(!handler)
        handler = new DBHandler();
    return handler;
}

bool DBHandler::connect()
{
    //If the driver is not available, print a list of available ones
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qDebug() << tr("SQLite driver unavailable");
        qDebug() << ("List of available drivers : " + QSqlDatabase::drivers().join("\n"));
        return false;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

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

void DBHandler::evaluate()
{
    bool success = true;

    //Connexion to the database
    if(connect())
    {
        qDebug() << "CONNECTED.";

        //Check for the presence of the Version table
        QSqlQuery query(QString("SELECT name FROM sqlite_master WHERE type='table' AND name='Version';"));

        if (query.next() && hasBasicFingerPrint()) {
            QSqlQuery versionsQuery(QString("SELECT id FROM Version"));
            while(versionsQuery.next())
                versions.removeAll(versionsQuery.value(0).toString());
            success = true;

        } else {
            qDebug() << "UNABLE TO FIND VERSION TABLE. ";
            success = hasBasicFingerPrint();
        }
    }
    else
    {
        qDebug() << "UNABLE TO CONNECT TO THE DATABASE. ";
        success = false;
    }

    emit publishResult(success, versions);
    return;
}

void DBHandler::updateDB()
{
    //Update from [0.2.0] to [0.2.1]
    if (versions.contains("0.2.1"))
        if (!update_0_2_1("0.2.1"))
            return;

    //Update from [0.2.1] to [0.2.2]
    if (versions.contains("0.2.2"))
        if (!update_0_2_2("0.2.2"))
            return;

    //Update from [0.2.2] to [0.2.3]
    if (versions.contains("0.2.3"))
        if (!update_0_2_3("0.2.3"))
            return;

    //Update from [0.2.3] to [0.2.4]
    if (versions.contains("0.2.4"))
        if (!update_0_2_4("0.2.4"))
            return;

    //Update from [0.2.4] to [0.2.5]
    if (versions.contains("0.2.5"))
        if (!update_0_2_5("0.2.5"))
            return;

    //Update from [0.2.5] to [0.2.6]
    if (versions.contains("0.2.6"))
        if (!update_0_2_6("0.2.6"))
            return;

    //Update from [0.2.6] to [0.2.7]
    if (versions.contains("0.2.7"))
        if (!update_0_2_7("0.2.7"))
            return;

    emit publishUpdateStatus(true);
}

bool DBHandler::update_0_2_1(const QString &version)
{
    QSqlQuery query;

    //Create the 'Version' table
    if (!query.prepare(QString("CREATE TABLE 'Version' ('id' TEXT PRIMARY KEY NOT NULL);"))) {
        errMsg = QString("Invalid query for 'Version' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create the 'Version' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Add the 'arrivalDate' column to 'Measure' table
    if (!query.prepare(QString("ALTER TABLE 'Measure' ADD 'arrivalDate' DATETIME;"))) {
        errMsg = QString("Query error on 'Measure' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not alter table 'Measure' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    return updateVersion(version);
}

bool DBHandler::update_0_2_2(const QString &version)
{
    QSqlQuery query;

    //Create the 'User' table
    if (!query.prepare(QString("CREATE TABLE 'User' ('username' TEXT PRIMARY KEY NOT NULL, 'key' TEXT, 'authorization' INTEGER NOT NULL DEFAULT 0);"))) {
        errMsg = QString("Invalid query for user 'User' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create the 'User' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    return updateVersion(version);
}

bool DBHandler::update_0_2_3(const QString &version)
{
    QSqlQuery query;

    //Add the 'external_id' column to 'Practician' table
    if (!query.prepare(QString("ALTER TABLE 'Practician' ADD COLUMN 'external_id' TEXT;"))) {
        errMsg = QString("Query error on 'Practician' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not add column 'extenal_id' to table 'Practician' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Add the 'external_id' column to 'Institute' table
    if (!query.prepare(QString("ALTER TABLE 'Institute' ADD COLUMN 'external_id' TEXT;"))) {
        errMsg = QString("Query error on 'Institute' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not add column 'extenal_id' to table 'Institute' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Add the 'external_id' column to 'Patient' table
    if (!query.prepare(QString("ALTER TABLE 'Patient' ADD COLUMN 'external_id' TEXT;"))) {
        errMsg = QString("Query error on 'Patient' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not add column 'extenal_id' to table 'Patient' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Add the 'stay_number' column to 'Patient' table
    if (!query.prepare(QString("ALTER TABLE 'Patient' ADD COLUMN 'stay_number' TEXT;"))) {
        errMsg = QString("Query error on 'Patient' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not add column 'stay_number' to table 'Patient' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    return updateVersion(version);
}

bool DBHandler::update_0_2_4(const QString &version)
{
    QSqlQuery query;

    //Add the 'ctype' column to 'Curve' table
    if (!query.prepare(QString("ALTER TABLE 'Curve' ADD COLUMN 'ctype' INT;"))) {
        errMsg = QString("Query error on ALTER TABLE 'Curve' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not add column 'ctype' to table 'Curve' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Set the default curves type
    if (!query.prepare(QString("UPDATE Curve SET ctype = '0';"))) {
        errMsg = QString("Query error on UPDATE 'Curve' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not set a default type for the existing curves (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    return updateVersion(version);
}

bool DBHandler::update_0_2_5(const QString &version)
{
    QSqlQuery query;

    //Add the 'value' column to 'Encryption' table
    if (!query.prepare(QString("ALTER TABLE 'Encryption' ADD COLUMN 'value' BOOL NOT NULL DEFAULT '0';"))) {
        errMsg = QString("Query error on ALTER TABLE 'Encryption' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not add column 'value' to table 'Encryption' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Insert the 'encryption' key to the 'Encryption' table
    if (!query.prepare(QString("INSERT INTO Encryption (key) VALUES ('encrypted');"))) {
        errMsg = QString("Query error on INSERT 'Encryption' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not insert the 'encryption' key into the 'Encryption' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Update the models
    if (!query.prepare("SELECT id FROM Model;")) {
        errMsg = QString("Query error on SELECT id of 'Model' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not retrieve the models IDs (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    while (query.next()) {

        //Get the current model ID
        QString id = query.value(0).toString();
        if (id.endsWith(".clv") || id.endsWith(".k"))
            continue;

        //Update the current model
        QSqlQuery modelQuery;
        if (!modelQuery.prepare(QString("UPDATE Model SET id = '%1' WHERE id = '%2'").arg(QString("%1.clv").arg(id), id))) {
            errMsg = QString("Query error on UPDATE 'Model' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(modelQuery.lastError().number()), modelQuery.lastError().databaseText(), modelQuery.lastError().driverText());
            emit publishUpdateStatus(false);
            return false;
        }

        if (!modelQuery.exec()) {
            errMsg = QString("Could not update model ID '%1' (version %2, code %3);\n%4;\n%5.").arg(id, version, QString::number(modelQuery.lastError().number()), modelQuery.lastError().databaseText(), modelQuery.lastError().driverText());
            emit publishUpdateStatus(false);
            return false;
        }
    }

    return updateVersion(version);
}

bool DBHandler::update_0_2_6(const QString &version)
{
    QSqlQuery query;

    //Drop the 'Report' table
    if (!query.prepare(QString("DROP TABLE 'Report';"))) {
        errMsg = QString("Query error on DROP TABLE 'Report' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not drop table 'Report' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Drop the 'View' table
    if (!query.prepare(QString("DROP TABLE 'View';"))) {
        errMsg = QString("Query error on DROP TABLE 'View' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not drop table 'View' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Drop the 'FxP' table
    if (!query.prepare(QString("DROP TABLE 'FxP';"))) {
        errMsg = QString("Query error on DROP TABLE 'FxP' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not drop table 'FxP' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Drop the 'Fields' table
    if (!query.prepare(QString("DROP TABLE 'Fields';"))) {
        errMsg = QString("Query error on DROP TABLE 'Fields' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not drop table 'Fields' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Drop the 'Copy_at' table
    if (!query.prepare(QString("DROP TABLE 'Copy_at';"))) {
        errMsg = QString("Query error on DROP TABLE 'Copy_at' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not drop table 'Copy_at' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the 'Report' table
    if (!query.prepare(QString("CREATE TABLE 'Report'("
                               "'id' INTEGER PRIMARY KEY NOT NULL, "
                               "'template' TEXT NOT NULL, "
                               "'title' TEXT, "
                               "'reference' TEXT, "
                               "'creation' DATETIME, "
                               "'publication' DATETIME, "
                               "'patientId' INTEGER, "
                               "'analystId' INTEGER, "
                               "'recipientId' INTEGER, "
                               "'instituteId' INTEGER, "
                               "'treatmentId' INTEGER, "
                               "CONSTRAINT 'fk_Report_Patient' "
                                 "FOREIGN KEY('patientId') "
                                 "REFERENCES 'Patient'('id') "
                                 "ON DELETE SET NULL "
                                 "ON UPDATE CASCADE, "
                               "CONSTRAINT 'fk_Report_Analyst' "
                                 "FOREIGN KEY('analystId') "
                                 "REFERENCES 'Practician'('id') "
                                 "ON DELETE SET NULL "
                                 "ON UPDATE CASCADE, "
                               "CONSTRAINT 'fk_Report_Recipient' "
                                 "FOREIGN KEY('recipientId') "
                                 "REFERENCES 'Practician'('id') "
                                 "ON DELETE SET NULL "
                                 "ON UPDATE CASCADE, "
                               "CONSTRAINT 'fk_Report_Institute' "
                                 "FOREIGN KEY('instituteId') "
                                 "REFERENCES 'Institute'('id') "
                                 "ON DELETE SET NULL "
                                 "ON UPDATE CASCADE, "
                               "CONSTRAINT 'fk_Report_Treatment' "
                                 "FOREIGN KEY('treatmentId') "
                                 "REFERENCES 'Curve'('id') "
                                 "ON DELETE SET NULL "
                                 "ON UPDATE CASCADE);"))) {
        errMsg = QString("Query error on CREATE TABLE 'Report' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create table 'Report' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the Report's patient index
    if (!query.prepare(QString("CREATE INDEX 'Report.fk_Report_Patient_idx' ON 'Report'('patientId');"))) {
        errMsg = QString("Query error on CREATE INDEX for report's patient (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create index for report's patient (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the Report's analyst index
    if (!query.prepare(QString("CREATE INDEX 'Report.fk_Report_Analyst_idx' ON 'Report'('analystId');"))) {
        errMsg = QString("Query error on CREATE INDEX for report's analyst (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create index for report's analyst (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the Report's recipient index
    if (!query.prepare(QString("CREATE INDEX 'Report.fk_Report_Recipient_idx' ON 'Report'('recipientId');"))) {
        errMsg = QString("Query error on CREATE INDEX for report's recipient (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create index for report's recipient (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the Report's institute index
    if (!query.prepare(QString("CREATE INDEX 'Report.fk_Report_Institute_idx' ON 'Report'('instituteId');"))) {
        errMsg = QString("Query error on CREATE INDEX for report's institute (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create index for report's institute (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the Report's treatment index
    if (!query.prepare(QString("CREATE INDEX 'Report.fk_Report_Treatment_idx' ON 'Report'('treatmentId');"))) {
        errMsg = QString("Query error on CREATE INDEX for report's treatment (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create index for report's treatment (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the 'ReportValue' table
    if (!query.prepare(QString("CREATE TABLE 'ReportValue'("
                               "'reportId' INTEGER NOT NULL, "
                               "'id' TEXT NOT NULL, "
                               "'value' TEXT, "
                               "PRIMARY KEY('reportId','id'), "
                               "CONSTRAINT 'fk_ReportValue_Report' "
                                 "FOREIGN KEY('reportId') "
                                 "REFERENCES 'Report'('id') "
                                 "ON DELETE CASCADE "
                                 "ON UPDATE CASCADE);"))) {
        errMsg = QString("Query error on CREATE TABLE 'ReportValue' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create table 'ReportValue' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the 'ReportImage' table
    if (!query.prepare(QString("CREATE TABLE 'ReportImage'("
                               "'reportId' INTEGER NOT NULL, "
                               "'id' TEXT NOT NULL, "
                               "'image' BLOB, "
                               "PRIMARY KEY('reportId','id'), "
                               "CONSTRAINT 'fk_ReportImage_Report' "
                                 "FOREIGN KEY('reportId') "
                                 "REFERENCES 'Report'('id') "
                                 "ON DELETE CASCADE "
                                 "ON UPDATE CASCADE);"))) {
        errMsg = QString("Query error on CREATE TABLE 'ReportImage' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create table 'ReportImage' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the 'ReportHiddenSection' table
    if (!query.prepare(QString("CREATE TABLE 'ReportHiddenSection'("
                               "'reportId' INTEGER NOT NULL, "
                               "'section' TEXT NOT NULL, "
                               "PRIMARY KEY('reportId','section'), "
                               "CONSTRAINT 'fk_ReportHiddenSection_Report' "
                                 "FOREIGN KEY('reportId') "
                                 "REFERENCES 'Report'('id') "
                                 "ON DELETE CASCADE "
                                 "ON UPDATE CASCADE);"))) {
        errMsg = QString("Query error on CREATE TABLE 'ReportHiddenSection' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create table 'ReportHiddenSection' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Fix for the practician creation issues on old databases
    if (!query.prepare("SELECT location FROM 'Institute' WHERE id = '0'")) {
        errMsg = QString("Query error on SELECT 'Institute' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not select 'Institute' location (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (query.next()) {
        int locationId;

        //Get the location ID
        if ((locationId = query.value(0).toInt()) < 1) {
            errMsg = QString("Could not select 'Institute' location (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
            emit publishUpdateStatus(false);
            return false;
        }

        //Update the location
        if (!query.prepare(QString("UPDATE Location SET country = ?, state = ?, address = ?, city = ?, postcode = ? WHERE id = '%1'").arg(locationId))) {
            errMsg = QString("Query error on UPDATE 'Location' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
            emit publishUpdateStatus(false);
            return false;
        }

        query.addBindValue("Suisse");
        query.addBindValue("VD");
        query.addBindValue("Division de pharmacologie clinique\nRue du Bugnon 17");
        query.addBindValue("Lausanne");
        query.addBindValue("1005");

        if (!query.exec()) {
            errMsg = QString("Could not update 'Institute' location (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
            emit publishUpdateStatus(false);
            return false;
        }

        //Update the institute
        if (!query.prepare(QString("UPDATE Institute SET id = ?, name = ? WHERE id = '0'").arg(locationId))) {
            errMsg = QString("Query error on UPDATE 'Institute' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
            emit publishUpdateStatus(false);
            return false;
        }

        query.addBindValue(1);
        query.addBindValue("Centre hospitalier universitaire vaudois");

        if (!query.exec()) {
            errMsg = QString("Could not update 'Institute' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
            emit publishUpdateStatus(false);
            return false;
        }

        //Update the practicians
        if (!query.prepare(QString("UPDATE Practician SET Institute = '1'").arg(locationId))) {
            errMsg = QString("Query error on UPDATE 'Practician' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
            emit publishUpdateStatus(false);
            return false;
        }

        if (!query.exec()) {
            errMsg = QString("Could not update 'Practician' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
            emit publishUpdateStatus(false);
            return false;
        }
    }

    return updateVersion(version);
}

bool DBHandler::update_0_2_7(const QString &version)
{
    QSqlQuery query;

    //Disable key constraints
    if (!query.prepare("PRAGMA foreign_keys = OFF;")) {
        errMsg = QString("Query error on PRAGMA foreign keys off (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not disable key constraints (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the 'new_Practician' table
    if (!query.prepare(QString("CREATE TABLE 'new_Practician'("
                               "'id' INTEGER PRIMARY KEY NOT NULL, "
                               "'title' TEXT, "
                               "'Person' INTEGER NOT NULL, "
                               "'Institute' INTEGER, "
                               "'external_id' TEXT, "
                               "CONSTRAINT 'external_id_UNIQUE' "
                                 "UNIQUE('external_id'), "
                               "CONSTRAINT 'fk_Doctor_Person1' "
                                 "FOREIGN KEY('Person') "
                                 "REFERENCES 'Person'('id') "
                                 "ON DELETE CASCADE "
                                 "ON UPDATE CASCADE, "
                               "CONSTRAINT 'fk_Doctor_Institute1' "
                                 "FOREIGN KEY('Institute') "
                                 "REFERENCES 'Institute'('id') "
                                 "ON DELETE SET NULL "
                                 "ON UPDATE CASCADE);"))) {
        errMsg = QString("Query error on CREATE TABLE 'new_Practician' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create table 'new_Practician' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Copy the data from the table to the new_table
    if (!query.prepare(QString("INSERT INTO 'new_Practician' (id, title, Person, Institute, external_id) "
                       "SELECT id, title, Person, Institute, external_id "
                       "FROM 'Practician';"))) {
        errMsg = QString("Query error on INSERT INTO new_Practician (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not copy data from old 'Practician' talbe to new 'new_Practician' table (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Drop the 'Practician' table
    if (!query.prepare(QString("DROP TABLE 'Practician';"))) {
        errMsg = QString("Query error on DROP TABLE 'Practician' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not drop table 'Practician' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Rename the 'new_Practician' table
    if (!query.prepare("ALTER TABLE 'new_Practician' RENAME TO 'Practician';")) {
        errMsg = QString("Query error on ALTER TABLE 'new_Practician' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not alter table 'new_Practician' (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the Practician's person index
    if (!query.prepare(QString("CREATE INDEX 'Practician.fk_Doctor_Person_idx' ON 'Practician'('Person');"))) {
        errMsg = QString("Query error on CREATE INDEX for practician's person (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create index for practician's person (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Create the Practician's institute index
    if (!query.prepare(QString("CREATE INDEX 'Practician.fk_Doctor_Institute_idx' ON 'Practician'('Institute');"))) {
        errMsg = QString("Query error on CREATE INDEX for practician's institute (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not create index for practician's institute (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Check the foreign keys constraints
    if (!query.prepare(QString("PRAGMA foreign_key_check;"))) {
        errMsg = QString("Query error on PRAGMA foreign keys check (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not check the foreign keys constraints (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (query.next()) {
        errMsg = QString("Foreign keys constraints violation found (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    //Enable key constraints
    if (!query.prepare("PRAGMA foreign_keys = ON;")) {
        errMsg = QString("Query error on PRAGMA foreign keys on (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not enable key constraints (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    return updateVersion(version);
}

bool DBHandler::updateVersion(const QString &version)
{
    QSqlQuery query;

    //Update the database version
    if (!query.prepare(QString("INSERT INTO 'Version' VALUES ('%1');").arg(version))) {
        errMsg = QString("Query error on version update (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    if (!query.exec()) {
        errMsg = QString("Could not update the version (version %1, code %2);\n%3;\n%4.").arg(version, QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
        emit publishUpdateStatus(false);
        return false;
    }

    return true;
}

QString DBHandler::getErrMsg()
{
    return errMsg;
}
