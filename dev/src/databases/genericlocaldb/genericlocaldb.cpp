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


#include "genericlocaldb.h"

#include <QPointer>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlIndex>
#include <QSqlError>
#include <QSqlResult>
#include <QtPlugin>
#include <QStringList>
#include <QPixmap>
#include <QDebug>
#include <QSqlDriver>
#include <QMetaProperty>

#include "core.h"
//#include "securitymanager.h"
#include "coremessagehandler.h"
#include "dal/genericdbobjects.h"
#include "genericdatabase.h"
#include "QSqlQuery"
#include <QUrl>

#include "clinical.h"
#include "drug.h"
#include "curve.h"
#include "adme.h"
#include "covariate.h"
#include "dosage.h"
#include "doses.h"
#include "email.h"
#include "halflife.h"
#include "infusions.h"
#include "institute.h"
#include "intervals.h"
#include "location.h"
#include "measure.h"
#include "operations.h"
#include "parameters.h"
#include "patient.h"
#include "person.h"
#include "percentile.h"
#include "phone.h"
#include "posology.h"
#include "practician.h"
#include "residual.h"
#include "target.h"
#include "drugmanager.h"
//#include "drugmodel.h"
//#include "drugtreatment.h"

namespace Tucuxi {

//void GenericLocalDBFactory::init (Core* core) {

//}

//PluginBase* GenericLocalDBFactory::newPlugin (QString &id) {
//    return new GenericLocalDB();
//}

//Descriptor GenericLocalDBFactory::descriptor() {
//    desc.id = "ch.heig-vd.ezechiel.GenericLocalDB";
//    desc.name = "GenericLocalDB";
//    return desc;
//}
//int GenericLocalDBFactory::priority(const QString &id) const {
//    return -1;
//}

//int GenericLocalDB::priority(const QString &id) const {
//    return -1;
//}

GenericLocalDB::GenericLocalDB() : _encryption(Unknown)
{
    _id = "ch.heig-vd.ezechiel.GenericLocalDB";
    init(CORE->instance());
}

//If not already done, close the database
GenericLocalDB::~GenericLocalDB()
{
    if (_db.isOpen())
        _db.close();
}

void GenericLocalDB::visit(CurveManager &cm) {}

Descriptor GenericLocalDB::descriptor()
{
    return Descriptor(_id, "Local DB", "Local SQLite database");
}

QString GenericLocalDB::databaseName() const
{
    //Check if the connexion is opened
    if (!_db.isOpen())
        return QString();

    return _db.databaseName();
}

bool GenericLocalDB::init(Core *core)
{
    Core::setInstance(core);

    qRegisterMetaType<Drug*>("drug");
    qRegisterMetaType<DrugModel*>("drugModel");
    qRegisterMetaType<DrugTreatment*>("drugTreatment");
    qRegisterMetaType<DrugResponseAnalysis*>("drugResponseAnalysis");
    qRegisterMetaType<Prediction*>("prediction");
    qRegisterMetaType<PredictionSpec*>("predictionSpec");
    qRegisterMetaType<DrugModelEngine*>("modelEngine");
    qRegisterMetaType<PercentileEngine*>("percentileEngine");
    qRegisterMetaType<PostEngine*>("postEngine");
    qRegisterMetaType<ReverseEngine*>("reverseEngine");
    qRegisterMetaType<Clinical*>("clinical");
    qRegisterMetaType<PatientVariate*>("patientVariate");
    qRegisterMetaType<Dosage*>("dosage");
    qRegisterMetaType<Institute*>("institute");
    qRegisterMetaType<Location*>("location");
    qRegisterMetaType<Measure*>("measure");
    qRegisterMetaType<Operation*>("operation");
    qRegisterMetaType<Parameter*>("parameter");
    qRegisterMetaType<Patient*>("patient");
    qRegisterMetaType<Person*>("person");
    qRegisterMetaType<Practician*>("practician");
    qRegisterMetaType<Target*>("target");
    qRegisterMetaType<ValidDoses*>("validDoses");
    qRegisterMetaType<ValidInfusions*>("validInfusions");
    qRegisterMetaType<ValidIntervals*>("validIntervals");
    qRegisterMetaType<Phone*>("phone");
    qRegisterMetaType<Email*>("email");
    qRegisterMetaType<ADME*>("adme");

    user_def_types.append(qRegisterMetaType<SharedDrugResponseAnalysis>("SharedDrugResponseAnalysis"));
    user_def_types.append(qRegisterMetaType<QPointer<DrugResponseAnalysis>>("QPDrugResponseAnalysis"));
    user_def_types.append(qRegisterMetaType<SharedDrugTreatment>("SharedDrugTreatment"));
    user_def_types.append(qRegisterMetaType<SharedDrugModel>("SharedDrugModel"));
    user_def_types.append(qRegisterMetaType<SharedPredictionSpec>("SharedPredictionSpec"));
    user_def_types.append(qRegisterMetaType<SharedPrediction>("SharedPrediction"));
    user_def_types.append(qRegisterMetaType<SharedModel>("SharedModel"));
    user_def_types.append(qRegisterMetaType<SharedPercentileEngine>("SharedPercentileEngine"));
    user_def_types.append(qRegisterMetaType<SharedPostEngine>("SharedPostEngine"));
    user_def_types.append(qRegisterMetaType<SharedReverseEngine>("SharedReverse"));
    user_def_types.append(qRegisterMetaType<SharedDrug>("SharedDrug"));
    user_def_types.append(qRegisterMetaType<QPointer<Drug>>("QPDrug"));
    user_def_types.append(qRegisterMetaType<SharedADME>("SharedADME"));
    user_def_types.append(qRegisterMetaType<SharedClinical>("SharedClinical"));
    user_def_types.append(qRegisterMetaType<SharedPatientVariate>("SharedPatientVariate"));
    user_def_types.append(qRegisterMetaType<SharedCurve>("SharedCurve"));
    user_def_types.append(qRegisterMetaType<SharedDosage>("SharedDosage"));
    user_def_types.append(qRegisterMetaType<SharedInstitute>("SharedInstitute"));
    user_def_types.append(qRegisterMetaType<SharedLocation>("SharedLocation"));
    user_def_types.append(qRegisterMetaType<SharedMeasure>("SharedMeasure"));
    user_def_types.append(qRegisterMetaType<SharedOperation>("SharedOperation"));
//    user_def_types.append(qRegisterMetaType<SharedOption>("option"));
    user_def_types.append(qRegisterMetaType<SharedParameter>("SharedParameter"));
    user_def_types.append(qRegisterMetaType<SharedPatient>("SharedPatient"));
    user_def_types.append(qRegisterMetaType<SharedPerson>("SharedPerson"));
    user_def_types.append(qRegisterMetaType<SharedPractician>("SharedPractician"));
    user_def_types.append(qRegisterMetaType<SharedTarget>("SharedTarget"));
    user_def_types.append(qRegisterMetaType<SharedValidDoses>("SharedDoses"));
    user_def_types.append(qRegisterMetaType<SharedValidInfusions>("SharedInfusions"));
    user_def_types.append(qRegisterMetaType<SharedValidIntervals>("SharedIntervals"));

    user_def_types.append(qRegisterMetaType<QList<SharedPhone>>("phones"));
    user_def_types.append(qRegisterMetaType<QList<SharedEmail>>("emails"));

    //    user_def_types.append(qRegisterMetaType<DrugParameter*>("drugparameter"));
    //    user_def_types.append(qRegisterMetaType<DrugVariate*>("drugvariate"));
    //    user_def_types.append(qRegisterMetaType<ComputeEngine*>("engine"));
    //    user_def_types.append(qRegisterMetaType<ComputeEngine*>("model"));
    //    user_def_types.append(qRegisterMetaType<ComputeEngine*>("percentileengine"));
    //    user_def_types.append(qRegisterMetaType<ComputeEngine*>("postengine"));
    //    user_def_types.append(qRegisterMetaType<ComputeEngine*>("reverseengine"));
    //    user_def_types.append(qRegisterMetaType<ErrorModel*>("errormodel"));
    //    user_def_types.append(qRegisterMetaType<Report*>("report"));
    //    user_def_types.append(qRegisterMetaType<PatientVariate*>("patientvariate"));
    //    user_def_types.append(qRegisterMetaType<CurveOption*>("curve_option"));
    //    user_def_types.append(qRegisterMetaType<Correlation*>("correlation"));

    return true;
}

//QString GenericLocalDB::errorString()
//{
//    return _db.lastError().text();
//}


// Connexion //////////////////////////////////////////////////////////////////////////////////////

//Open the database file
bool GenericLocalDB::connect(DBAccount &account)
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
bool GenericLocalDB::disconnect()
{
    //Close the database
    _db.close();

    //Reset the encryption
    _encryption = Unknown;

    return true;
}

//We might use the url, but we do not request it
void GenericLocalDB::needs(bool &url, bool &parent, bool &password)
{
    url = false;
    parent = false;
    password = false;
}

Tucuxi::Response GenericLocalDB::insertDBObject(QObject& newobj, QObject* parent, QString& trans_id, bool is_new = true, int modif_id = 0) {

    QSqlQuery query;
    const QMetaObject* metaobj = newobj.metaObject();
    Tucuxi::Response r(metaobj->className());
    QString qry = "";
    QString into = "";
    QString values = "";
    QSqlIndex p_key = _db.primaryIndex(metaobj->className());

    int count = metaobj->propertyCount();
    bool first = true;
    for (int i=1; i<count; ++i) {
        QMetaProperty metaprop = metaobj->property(i);
        QString name = metaprop.name();
        if (!user_def_types.contains(metaprop.userType()) && !p_key.contains(name)) {
            if (first) {
                into = name;
                values = "'" + metaprop.read(&newobj).toString() + "'";
                first = false;
            }
            else {
                into += "," + name;
                QVariant val = metaprop.read(&newobj);
                values += ",'" + val.toString() + "'";
            }
        }
    }
    QString cname = metaobj->className();
    if (cname == "Tucuxi::parent") {
        if (is_new) {
            qry = QString("INSERT INTO %1 (%2) VALUES (%3)")
                    .arg(metaobj->className()).arg(into).arg(values);
        }
        else {
            qry = QString("INSERT INTO %1 (%2,postmod_id,modification_id,is_modified) VALUES (%3,%4,%5,%6)")
                    .arg(metaobj->className()).arg(into).arg(values).arg(newobj.property("id").toString()).arg(modif_id).arg(0/*SECURITY->user()->id()*/);
        }
    }
    else {
        if (is_new) {
            qry = QString("INSERT INTO %1 (%2,created_by) VALUES (%3,%4)")
                    .arg(metaobj->className()).arg(into).arg(values).arg(0/*SECURITY->user()->id()*/);
        }
        else {
            qry = QString("INSERT INTO %1 (%2,postmod_id,modification_id,is_modified,created_by) VALUES (%3,%4,%5,1,%6)")
                    .arg(metaobj->className()).arg(into).arg(values).arg(newobj.property("id").toString()).arg(modif_id).arg(0/*SECURITY->user()->id()*/);
        }

    }

    if (!query.prepare(qry)) {
        qDebug() << "Query failure: " << qry;
        rollbackTransaction(trans_id);
        return queryError(query);
    }


    if (!query.exec()) {
        qDebug() << "Query failure: " << qry;
        rollbackTransaction(trans_id);
        return queryError(query);
    }

    if (modif_id == 0) {
        QVariant inserted_id = query.lastInsertId();
        newobj.setProperty("id", inserted_id);
    }

    return r;
}

Tucuxi::Response GenericLocalDB::updateDBObject(QObject& dbobj, QObject* parent, QString& trans_id, bool is_deleted = false) {

    const QMetaObject* metaobj = dbobj.metaObject();
    Tucuxi::Response r(metaobj->className());
    QSqlQuery query;

    QString set = "";
    int count = metaobj->propertyCount();
    bool first = true;
    for (int i=1; i<count; ++i) {
        QMetaProperty metaprop = metaobj->property(i);
        QString name = metaprop.name();
        if (name != "id" && !user_def_types.contains(metaprop.userType())) {
            if (first) {
                set = name + "='" + metaprop.read(&dbobj).toString() + "'";
                first = false;
            }
            else {
                set += "," + name + "='" + metaprop.read(&dbobj).toString() + "'";
            }
        }
    }   

    QString qry = "";
    if (is_deleted) {
        qry = QString("UPDATE %1 SET %2,is_deleted = %3 where id = %4")
                .arg(metaobj->className()).arg(set).arg(is_deleted).arg(dbobj.property("id").toString());
    }
    else {
        qry = QString("UPDATE %1 SET %2 where id = %3")
                .arg(metaobj->className()).arg(set).arg(dbobj.property("id").toString());
    }

    if (!query.prepare(qry))
        return queryError(query);

    if (!query.exec()) {
        rollbackTransaction(trans_id);
        return queryError(query);
    }

    return r;
}

Tucuxi::Response GenericLocalDB::setDBObjects (QList<QObject*>& dbobjs, QObject* parent) {

    Tucuxi::Response r;
    QSqlQuery query;
    QString trans_id = "__setDBObjects";
    if ((r = startTransaction(trans_id)).error != NoError)
        return r;

    for (QList<QObject*>::iterator it = dbobjs.begin(); it != dbobjs.end(); ++it) {
        r = setDBObject(**it, parent, trans_id);
    }

    if (r.error == NoError) {
        r = commitTransaction(trans_id);
    }
    else {
        rollbackTransaction(trans_id);
    }
    return r;
}

Tucuxi::Response GenericLocalDB::setDBObject (QObject& dbobj, QObject* parent) {

    Tucuxi::Response r;
    QSqlQuery query;
    QString trans_id = "__setDBObject";
    if ((r = startTransaction(trans_id)).error != NoError)
        return r;
    r = setDBObject(dbobj, parent, trans_id);

    if (r.error == NoError) {
        r = commitTransaction(trans_id);
    }
    else {
        rollbackTransaction(trans_id);
    }
    return r;
}

Tucuxi::Response GenericLocalDB::setDBObject (QObject& dbobj, QObject* parent, QString& trans_id) {

    Tucuxi::Response r;
    const QMetaObject* metaobj = dbobj.metaObject();
    r.responder = metaobj->className();
    WhereClauseMap where;
    where.insert(metaobj->indexOfProperty("id"), Comparison(AND_Equals, dbobj.property("id")));

    for (int i=1; i<metaobj->propertyCount(); ++i) {
        QMetaProperty metaprop = metaobj->property(i);
        QString name = metaprop.name();
        if (user_def_types.contains(metaprop.userType())) {
            QObject* rdbobj = metaprop.read(&dbobj).value<QObject*>();
            if (rdbobj == 0) {
                qDebug() << "QObject* prop " << name << " not set in object of type " << metaobj->className();
                rollbackTransaction(trans_id);
                r.message = tr("The property %1 of the given object with type %2, must be set.")
                        .arg(metaprop.name())
                        .arg(metaobj->className());
                return r;
            }
            else {
                r = setDBObject(*rdbobj, parent, trans_id);
                QString cname = rdbobj->metaObject()->className();
                cname = cname.toLower();
                QVariant idval = rdbobj->property("id");
                dbobj.setProperty((name + "_id").toStdString().c_str(), idval);
            }
        }
    }

    if ((r = getDBObjects(*metaobj, parent, where)).error != NoError) {        

        if ((r = insertDBObject(dbobj, parent, trans_id, true, 0)).error != NoError) {
            rollbackTransaction(trans_id);
            return r;
        }
        if (dbobj.parent() != parent) {
            dbobj.setParent(parent);
        }
        return r;
    }
    else {
        QObject *olddbobj = r.results.last();

        //check if object changed
        bool changed = false;
        for (int i=1; i<metaobj->propertyCount(); ++i) {
            QMetaProperty metaprop = metaobj->property(i);
            QString name = metaprop.name();            
            if (olddbobj->property(name.toStdString().c_str()) != dbobj.property(name.toStdString().c_str())) {
                changed = true;
            }
        }
        if (changed == true) {
            QString qry = QString("INSERT INTO Modification (created_by) values (\"%1\")")
                    .arg(0/*SECURITY->user()->id()*/);

            QSqlQuery query;
            if (!query.prepare(qry)) {
                qDebug() << "Bad Modification insert query for object of type " << metaobj->className();
                return queryError(query);
            }

            if (!query.exec()) {
                qDebug() << "Bad Modification insert query for object of type " << metaobj->className();
                rollbackTransaction(trans_id);
                return queryError(query);
            }

            QVariant modif_id = query.lastInsertId();

            if ((r = insertDBObject(*olddbobj, parent, trans_id, false, modif_id.toInt())).error != NoError) {
                qDebug() << "Failure to insert object of type " << metaobj->className();
                rollbackTransaction(trans_id);
                return r;
            }

            if ((r = updateDBObject(dbobj, parent, trans_id, false)).error != NoError) {
                qDebug() << "Failure to update object of type " << metaobj->className();
                rollbackTransaction(trans_id);
                return r;
            }
        }

        if (dbobj.parent() != &parent) {
            dbobj.setParent(parent);
        }
        return r;
    }
}

Tucuxi::Response GenericLocalDB::deleteDBObject (QObject& dbobj, QObject* parent) {
    Tucuxi::Response r;
    QSqlQuery query;
    QString trans_id = "__deleteDBObjects";
    if ((r = startTransaction(trans_id)).error != NoError)
        return r;

    WhereClauseMap where;
    where.insert(dbobj.metaObject()->indexOfProperty("id"), Comparison(AND_Equals, dbobj.property("id")));
    if ((r = getDBObjects(*dbobj.metaObject(), parent, where)).error == NoError) {
        QString qry = QString("INSERT INTO Modification (created_by) values (\"%1\")")
                .arg(0/*SECURITY->user()->id()*/);

        if (!query.prepare(qry))
            return queryError(query);

        if (!query.exec()) {
            rollbackTransaction(trans_id);
            return queryError(query);
        }

        if ((r = updateDBObject(dbobj, parent, trans_id, true)).error != NoError) {
            rollbackTransaction(trans_id);
            return r;
        }
    }
    else {
        rollbackTransaction(trans_id);
        return queryError(query);
    }

    r = commitTransaction(trans_id);
    dbobj.deleteLater();
    return r;
}

Tucuxi::Response GenericLocalDB::deleteDBObjects (QList<QObject*>& dbobjs, QObject* parent) {
    Tucuxi::Response r;
    QSqlQuery query;
    QString trans_id = "__deleteDBObjects";
    if ((r = startTransaction(trans_id)).error != NoError)
        return r;

    for (QList<QObject*>::iterator it = dbobjs.begin(); it != dbobjs.end(); ++it) {
        WhereClauseMap where;
        where.insert((*it)->metaObject()->indexOfProperty("id"), Comparison(AND_Equals, (*it)->property("id")));
        if ((r = getDBObjects(*(*it)->metaObject(), parent, where)).error == NoError) {
            QString qry = QString("INSERT INTO Modification (created_by) values (\"%1\")")
                    .arg(0/*SECURITY->user()->id()*/);

            if (!query.prepare(qry))
                return queryError(query);

            if (!query.exec()) {
                rollbackTransaction(trans_id);
                return queryError(query);
            }

            if ((r = updateDBObject(**it, parent, trans_id, true)).error != NoError) {
                rollbackTransaction(trans_id);
                return r;
            }
        }
        else {
            rollbackTransaction(trans_id);
            return queryError(query);
        }

    }
    r = commitTransaction(trans_id);
    for (QList<QObject*>::iterator it = dbobjs.begin(); it != dbobjs.end(); ++it) {
        (*it)->deleteLater();
    }
    return r;
}

Tucuxi::Response GenericLocalDB::getDBObjects(const QMetaObject& dbobj, QObject* parent)  {
    return getDBObjects(dbobj, parent, SelectClauseMap(), WhereClauseMap(), OrderByClauseMap());
}

Tucuxi::Response GenericLocalDB::getDBObjects(const QMetaObject& dbobj, QObject* parent, const SelectClauseMap& slctconds) {
    return getDBObjects(dbobj, parent, slctconds, WhereClauseMap(), OrderByClauseMap());
}

Tucuxi::Response GenericLocalDB::getDBObjects(const QMetaObject& dbobj, QObject* parent, const WhereClauseMap& whereconds) {
    return getDBObjects(dbobj, parent, SelectClauseMap(), whereconds, OrderByClauseMap());
}

Tucuxi::Response GenericLocalDB::getDBObjects(const QMetaObject& dbobj, QObject* parent, const WhereClauseMap& whereconds, const OrderByClauseMap& orderconds) {
    return getDBObjects(dbobj, parent, SelectClauseMap(), whereconds, orderconds);
}

Tucuxi::Response GenericLocalDB::getDBObjects(const QMetaObject& dbobj, QObject* parent, const SelectClauseMap& slctconds, const WhereClauseMap& whereconds) {
    return getDBObjects(dbobj, parent, slctconds, whereconds, OrderByClauseMap());
}

QList<QString> GenericLocalDB::getOperatorString(const ComparisonOperator &comp) {
    QList<QString> ret;
    ret.append({"",""});
    switch (comp) {
    case AND_Equals:
    default:
        ret[0] = " AND ";
        ret[1] = " = ";
        break;
    case AND_GreaterThanOrEquals:
        ret[0] = " AND ";
        ret[1] = " >= ";
        break;
    case AND_LessThanOrEquals:
        ret[0] = " AND ";
        ret[1] = " <= ";
        break;
    case AND_GreaterThan:
        ret[0] = " AND ";
        ret[1] = " > ";
        break;
    case AND_LessThan:
        ret[0] = " AND ";
        ret[1] = " < ";
        break;
    case AND_NotEqual:
        ret[0] = " AND ";
        ret[1] = " != ";
        break;
    case OR_Equals:
        ret[0] = " OR ";
        ret[1] = " = ";
        break;
    case OR_GreaterThanOrEquals:
        ret[0] = " OR ";
        ret[1] = " >= ";
        break;
    case OR_LessThanOrEquals:
        ret[0] = " OR ";
        ret[1] = " <= ";
        break;
    case OR_GreaterThan:
        ret[0] = " OR ";
        ret[1] = " > ";
        break;
    case OR_LessThan:
        ret[0] = " OR ";
        ret[1] = " < ";
        break;
    case OR_NotEqual:
        ret[0] = " OR ";
        ret[1] = " != ";
        break;
    }
    return ret;
}

QString GenericLocalDB::whereBuilder(const QMetaObject& metaobj, const WhereClauseMap& whereconds) {
    QString baby = "";
    QString ret = "";
    bool isfirst = true;
    QString babyop = "";
    for (WhereClauseMap::const_iterator cond = whereconds.constBegin(); cond != whereconds.constEnd(); ++cond) {
        if (cond.key() == -1) {
            //recurse
            WhereClauseMap wcm = QVariant::fromValue(cond.value().second).value<WhereClauseMap>();
            baby = " ( " + whereBuilder(metaobj, wcm) + " ) ";
            babyop = getOperatorString(cond.value().first).first();
        } else {
            QList<QString> ops = getOperatorString(cond.value().first);
            if (isfirst) {
                ret += QString("%1.%2 %3 \"%4\"").arg(metaobj.className()).arg(metaobj.property(cond.key()).name()).arg(ops[1]).arg(cond.value().second.toString());
                isfirst = false;
            } else {
                ret += QString("%1 %2.%3 %4 \"%5\"").arg(ops[0]).arg(metaobj.className()).arg(metaobj.property(cond.key()).name()).arg(ops[1]).arg(cond.value().second.toString());
            }
        }
    }
    ret += babyop + baby;
    return ret;
}

Tucuxi::Response GenericLocalDB::getDBObjects(const QMetaObject& metaobj, QObject* parent, const SelectClauseMap& slctconds, const WhereClauseMap& whereconds, const OrderByClauseMap& orderconds) {

    Tucuxi::Response r(metaobj.className());
    QList<QObject*> results;
    QSqlQuery query;

    QString slct = "*";    
    QString where = QString("WHERE %1.is_modified = 0 AND %1.is_deleted = 0").arg(metaobj.className());
    if (!whereconds.empty() && !(whereconds.size() == 1 && whereconds.keys().first() == -1)) { where += " AND "; }
    QString orderby = "";
    int count = metaobj.propertyCount();
    bool firstslct = true;    
    for (int i=1; i<count; ++i) {
        QMetaProperty metaprop = metaobj.property(i);
        QString name = metaprop.name();

        if (!user_def_types.contains(metaprop.userType()) && (slctconds.size() == 0 || slctconds.contains(i))) {
            if (firstslct) {
                slct = name;
                firstslct = false;
            }
            else {
                slct += "," + name;
            }
        }        
    }

    where += whereBuilder(metaobj, whereconds);

    bool firstorder = true;
    for (OrderByClauseMap::const_iterator ord = orderconds.constBegin(); ord != orderconds.constEnd(); ++ord) {
        if (firstorder) {
            orderby += "ORDER BY " + QString(metaobj.property(ord.key()).name());
            firstorder = false;
        } else {
            orderby += ", " + QString(metaobj.property(ord.key()).name());
        }
        switch (*ord) {
        case Ascending:
        default:
            orderby += " ASC ";
            break;
        case Descending:
            orderby += " DESC ";
            break;
        }
    }

    QString qry = QString("SELECT %1 FROM %2 %3 %4").arg(slct).arg(metaobj.className()).arg(where).arg(orderby);    
    if (!query.prepare(qry))
        return queryError(query);

    if (!query.exec())
        return queryError(query);

    if (!query.next()) {
        r.message = tr("No records found.");
        r.error   = NotFound;
        return r;
    }
    query.previous();
    while (query.next()) {
        bool existingmainobj = false;
        QObject* newdbobj;
        Q_ASSERT(query.record().count() <= metaobj.propertyCount());
//        foreach (QObject * parentobj, parent->findChildren<QObject*>()) {
//            if (parentobj->metaObject()->className() == metaobj.className()) {
//                if (parentobj->property("id") == query.record().value("id")) {
//                    bool allpropsequal = true;
//                    for (int j = 1; j < parentobj->metaObject()->propertyCount(); j++) {
//                        QMetaProperty metaprop = parentobj->metaObject()->property(j);
//                        QString name = metaprop.name();
//                        QVariant prop1 = parentobj->property(name.toStdString().c_str());
//                        QVariant prop2 = query.record().value(name);
//                        if (!user_def_types.contains(metaprop.userType()) && !(prop1 == prop2)) {
//                            allpropsequal = false;
//                        }
//                    }
//                    if (allpropsequal == true) {
//                        newdbobj = parentobj;
//                        existingmainobj = true;
//                    }
//                }
//            }
//        }
        if (existingmainobj == false) {
            newdbobj = metaobj.newInstance();        
            for (int i = 1; i < metaobj.propertyCount(); ++i) {
                if (user_def_types.contains(metaobj.property(i).userType())) {
                    QString propname = metaobj.property(i).name();
                    QVariant propval = query.record().value((QString(propname) + "_id").toStdString().c_str());
                    bool existingobj = false;
                    foreach (QObject * parentobj, parent->findChildren<QObject*>()) {
                        QString classname = parentobj->metaObject()->className();
                        int refobjid = propval.toInt();
                        int childid = parentobj->property("id").toInt();
                        if (childid == refobjid && classname.toLower() == propname) {
                            newdbobj->setProperty(propname.toStdString().c_str(), QVariant::fromValue(QObject*(parentobj)));
                            newdbobj->setParent(parent);
                            existingobj = true;
                            break;
                        }
                    }
                    if (existingobj == false) {
                        const QMetaObject *refmetaobj = QMetaType::metaObjectForType(QMetaType::type(metaobj.property(i).name()));
                        WhereClauseMap where;
                        where.insert(refmetaobj->indexOfProperty("id"), Comparison(AND_Equals, propval));
                        Tucuxi::Response resp = getDBObjects(*refmetaobj, parent, SelectClauseMap(), where, OrderByClauseMap());
                        QObject *newrefobj = resp.results.first();
                        newdbobj->setProperty(propname.toStdString().c_str(), QVariant::fromValue(QObject*(newrefobj)));
                    }
                }
                else {
                    for (int j = 0; j < query.record().count(); ++j) {

                        if (query.record().fieldName(j) == metaobj.property(i).name()) {
                            QVariant var = query.value(j);
                            newdbobj->setProperty(query.record().fieldName(j).toStdString().c_str(), var);
                            break;
                        }
                    }
                }
            }
            newdbobj->setParent(parent);
        }
        results.append(newdbobj);
    }
    r.results = results;
    return r;
}

Tucuxi::Response GenericLocalDB::getDBObjectsOfDBObject(const QMetaObject &primarymetaobj, const QMetaObject &secondarymetaobj, QObject* parent, const WhereClauseMap &primarywhere) {
    return getDBObjectsOfDBObject(primarymetaobj, secondarymetaobj, parent, primarywhere, WhereClauseMap(), OrderByClauseMap());
}

Tucuxi::Response GenericLocalDB::getDBObjectsOfDBObject(const QMetaObject &primarymetaobj, const QMetaObject &secondarymetaobj, QObject* parent, const WhereClauseMap &primarywhere, const OrderByClauseMap& orderconds) {
    return getDBObjectsOfDBObject(primarymetaobj, secondarymetaobj, parent, primarywhere, WhereClauseMap(), orderconds);
}

Tucuxi::Response GenericLocalDB::getDBObjectsOfDBObject(const QMetaObject &primarymetaobj, const QMetaObject &secondarymetaobj, QObject* parent, const WhereClauseMap &primarywhere, const WhereClauseMap &secondarywhere) {
    return getDBObjectsOfDBObject(primarymetaobj, secondarymetaobj, parent, primarywhere, secondarywhere, OrderByClauseMap());
}

Tucuxi::Response GenericLocalDB::getDBObjectsOfDBObject(const QMetaObject& primarymetaobj, const QMetaObject& secondarymetaobj, QObject* parent, const WhereClauseMap& primarywhere, const WhereClauseMap& secondarywhere, const OrderByClauseMap& secordconds) {
    return getDBObjectsOfDBObject(primarymetaobj, secondarymetaobj, parent, primarywhere, secondarywhere, OrderByClauseMap(), secordconds);
}

Tucuxi::Response GenericLocalDB::getDBObjectsOfDBObject(const QMetaObject& primarymetaobj, const QMetaObject& secondarymetaobj, QObject* parent, const WhereClauseMap& primarywhere, const WhereClauseMap& secondarywhere, const OrderByClauseMap& primordconds, const OrderByClauseMap& secordconds) {
    Tucuxi::Response r(QString(primarymetaobj.className()) + ".join." + QString(secondarymetaobj.className()));
    QList<QObject*> ret;
    QSqlQuery query;

    QString slct = "*";
    QString where = QString("WHERE %1.is_modified = 0 AND %1.is_deleted = 0").arg(secondarymetaobj.className());
    if (!secondarywhere.empty()
            || !primarywhere.empty()
            && !(secondarywhere.size() + primarywhere.size() == 1
            && (secondarywhere.contains(-1) || primarywhere.contains(-1)))) { where += " AND "; }

    QString orderby = "";
    int count = secondarymetaobj.propertyCount();
    bool firstslct = true;
    bool firstorder = true;
    for (int i=1; i<count; ++i) {
        QMetaProperty metaprop = secondarymetaobj.property(i);
        QString name = metaprop.name();

        if (!user_def_types.contains(metaprop.userType())) {
            if (firstslct) {
                slct = QString("%1.%2").arg(secondarymetaobj.className()).arg(name);
                firstslct = false;
            }
            else {
                slct += QString(", %1.%2").arg(secondarymetaobj.className()).arg(name);
            }
        }       
    }
    where += whereBuilder(secondarymetaobj, secondarywhere);
    where += whereBuilder(primarymetaobj, primarywhere);

    for (OrderByClauseMap::const_iterator ord = secordconds.constBegin(); ord != secordconds.constEnd(); ++ord) {
        if (firstorder) {
            orderby += QString("ORDER BY %1.%2").arg(secondarymetaobj.className())
                    .arg(secondarymetaobj.property(ord.key()).name());
            firstorder = false;
        } else {
            orderby += ", " + QString(secondarymetaobj.property(ord.key()).name());
        }
        switch (*ord) {
        case Ascending:
        default:
            orderby += " ASC ";
            break;
        case Descending:
            orderby += " DESC ";
            break;
        }
    }

    for (OrderByClauseMap::const_iterator ord = primordconds.constBegin(); ord != primordconds.constEnd(); ++ord) {
        if (firstorder) {
            orderby += QString("ORDER BY %1.%2").arg(primarymetaobj.className())
                    .arg(primarymetaobj.property(ord.key()).name());
            firstorder = false;
        } else {
            orderby += ", " + QString(primarymetaobj.property(ord.key()).name());
        }
        switch (*ord) {
        case Ascending:
        default:
            orderby += " ASC ";
            break;
        case Descending:
            orderby += " DESC ";
            break;
        }
    }


    QString qry = QString("SELECT %1 FROM %2 INNER JOIN %3 ON %2.%3_id = %3.id %4 %5").arg(slct).arg(secondarymetaobj.className()).arg(primarymetaobj.className()).arg(where).arg(orderby);
    if (!query.prepare(qry)){
        r = queryError(query);
        r.results = ret;
        return r;
    }

    if (!query.exec()){
        r = queryError(query);
        r.results = ret;
        return r;
    }

    if (!query.next()) {
        r.message = tr("The ID");
        r.error   = NotFound;
        r.results = ret;
        return r;
    }
    query.previous();
    while (query.next()) {
        bool existingmainobj = false;
        QObject* newdbobj;
        Q_ASSERT(query.record().count() <= secondarymetaobj.propertyCount());
        foreach (QObject * parentobj, parent->findChildren<QObject*>()) {
            if (parentobj->metaObject()->className() == secondarymetaobj.className()) {
                if (parentobj->property("id") == query.record().value(QString("id")).toInt()) {
                    bool allpropsequal = true;
                    for (int j = 1; j < parentobj->metaObject()->propertyCount(); j++) {
                        QMetaProperty metaprop = parentobj->metaObject()->property(j);
                        QString name = metaprop.name();
                        QVariant prop1 = parentobj->property(name.toStdString().c_str());
                        QVariant prop2 = query.record().value(name);
                        if (!user_def_types.contains(metaprop.userType()) && !(prop1 == prop2)) {
                            allpropsequal = false;
                        }
                    }
                    if (allpropsequal == true) {
                        newdbobj = parentobj;
                        existingmainobj = true;
                    }
                }
            }
        }
        if (existingmainobj == false) {
            newdbobj = secondarymetaobj.newInstance();
            for (int i = 1; i < secondarymetaobj.propertyCount(); ++i) {
                if (user_def_types.contains(secondarymetaobj.property(i).userType())) {
                    QString propname = secondarymetaobj.property(i).name();
                    QVariant propval = query.record().value((QString(propname) + "_id").toStdString().c_str());
                    bool existingobj = false;
                    foreach (QObject * parentobj, parent->findChildren<QObject*>()) {
                        QString classname = parentobj->metaObject()->className();
                        int refobjid = propval.toInt();
                        int childid = parentobj->property("id").toInt();
                        if (childid == refobjid && classname.toLower() == propname) {
                            newdbobj->setProperty(propname.toStdString().c_str(), QVariant::fromValue(QObject*(parentobj)));
                            newdbobj->setParent(parent);
                            existingobj = true;
                            break;
                        }
                    }
                    if (existingobj == false) {
                        QString propname = secondarymetaobj.property(i).name();
                        QVariant propval = query.record().value((QString(propname) + "_id").toStdString().c_str());
                        int t = QMetaType::type(propname.toStdString().c_str());
                        const QMetaObject *refmetaobj = QMetaType::metaObjectForType(t);
                        WhereClauseMap where;
                        where.insert(refmetaobj->indexOfProperty("id"), Comparison(AND_Equals, propval));
                        getDBObjects(*refmetaobj, parent, SelectClauseMap(), where, OrderByClauseMap());
                        QObject *newrefobj = parent->children().last();
                        newdbobj->setProperty(propname.toStdString().c_str(), QVariant::fromValue(QObject*(newrefobj)));
                    }
                }
                else {
                    for (int j = 0; j < query.record().count(); ++j) {

                        if (query.record().fieldName(j) == secondarymetaobj.property(i).name()) {
                            QVariant var = query.value(j);
                            QString fname = query.record().fieldName(j);
                            newdbobj->setProperty(fname.toStdString().c_str(), var);
                            break;
                        }
                    }
                }
            }
            newdbobj->setParent(parent);
        }
        ret.append(newdbobj);
    }
    r.results = ret;
    return r;
}

QList<QVariant> GenericLocalDB::getDBData(const QMetaObject& metaobj, QObject* parent, const SelectClauseMap& slctconds) {
    return getDBData(metaobj, parent, slctconds, WhereClauseMap(), OrderByClauseMap());
}

QList<QVariant> GenericLocalDB::getDBData(const QMetaObject& metaobj, QObject* parent, const SelectClauseMap& slctconds, const WhereClauseMap& whereconds) {
    return getDBData(metaobj, parent, slctconds, whereconds, OrderByClauseMap());
}

QList<QVariant> GenericLocalDB::getDBData(const QMetaObject& metaobj, QObject* parent, const SelectClauseMap& slctconds, const WhereClauseMap& whereconds, const OrderByClauseMap& orderconds) {

    QList<QVariant> ret;
    Tucuxi::Response r(metaobj.className());
    QSqlQuery query;

    QString slct = "*";
    QString where = "WHERE is_modified = 0 AND is_deleted = 0";
    QString orderby = "";
    int count = metaobj.propertyCount();
    bool firstslct = true;
    bool firstorder = true;
    for (int i=1; i<count; ++i) {
        QMetaProperty metaprop = metaobj.property(i);
        QString name = metaprop.name();

        if (!user_def_types.contains(metaprop.userType()) && (slctconds.size() == 0 || slctconds.contains(i))) {
            if (firstslct) {
                slct = name;
                firstslct = false;
            }
            else {
                slct += "," + name;
            }
        }
    }

    for (OrderByClauseMap::const_iterator ord = orderconds.constBegin(); ord != orderconds.constEnd(); ++ord) {
        if (firstorder) {
            orderby += "ORDER BY " + QString(metaobj.property(ord.key()).name());
            firstorder = false;
        } else {
            orderby += ", " + QString(metaobj.property(ord.key()).name());
        }
        switch (*ord) {
        case Ascending:
        default:
            orderby += " ASC ";
            break;
        case Descending:
            orderby += " DESC ";
            break;
        }
    }

    QString qry = QString("SELECT %1 FROM %2 %3 %4").arg(slct).arg(metaobj.className()).arg(where).arg(orderby);
    if (!query.prepare(qry)){
        r = queryError(query);
        return ret;
    }

    if (!query.exec()) {
        r = queryError(query);
        return ret;
    }

    if (!query.next()) {
        r.message = tr("The ID");
        r.error   = NotFound;
        return ret;
    }
    query.previous();
    while (query.next()) {
        for (int j = 0; j < query.record().count(); ++j) {
            ret.append(query.record().value(j));
        }
    }
    return ret;
}

// Concenience ////////////////////////////////////////////////////////////////////////////////////

Response GenericLocalDB::getPatientFromId(int _id, SharedPatient& _p) {
    WhereClauseMap _where;
    _where.insert(Patient::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Patient::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedPatient _sp = SharedPatient(r.castresults<Patient*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setPatient(SharedPatient& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getPatientsList(QList<SharedPatient>& _in) {
    Response r = getDBObjects(Patient::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedPatient(qobj));
    }
    return r;
}

Response GenericLocalDB::deletePatient(SharedPatient & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getPracticianFromId(int _id, SharedPractician& _p) {
    WhereClauseMap _where;
    _where.insert(Practician::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Practician::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedPractician _sp = SharedPractician(r.castresults<Practician*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setPractician(SharedPractician& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getPracticiansList(QList<SharedPractician>& _in) {
    Response r = getDBObjects(Practician::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedPractician(qobj));
    }
    return r;
}

Response GenericLocalDB::deletePractician(SharedPractician & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::deletePractician(int _id) {
    WhereClauseMap _where;
    _where.insert(Practician::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Practician::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    return deleteDBObjects(r.results, 0);
}

Response GenericLocalDB::getInstituteFromId(int _id, SharedInstitute& _p) {
    WhereClauseMap _where;
    _where.insert(Institute::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Institute::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedInstitute _sp = SharedInstitute(r.castresults<Institute*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setInstitute(SharedInstitute& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getInstitutesList(QList<SharedInstitute>& _in) {
    Response r = getDBObjects(Institute::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedInstitute(qobj));
    }
    return r;
}

Response GenericLocalDB::deleteInstitute(SharedInstitute & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::deleteInstitute(int _id) {
    WhereClauseMap _where;
    _where.insert(Institute::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Institute::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    return deleteDBObjects(r.results, 0);
}

Response GenericLocalDB::getClinicalFromId(int _id, SharedClinical& _p) {
    WhereClauseMap _where;
    _where.insert(Clinical::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Clinical::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedClinical _sp = SharedClinical(r.castresults<Clinical*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setClinical(SharedClinical& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getClinicalsList(QList<SharedClinical>& _in) {
    Response r = getDBObjects(Clinical::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedClinical(qobj));
    }
    return r;
}

Response GenericLocalDB::getClinicalsByDate(int _id, QList<SharedClinical>& _in) {
    WhereClauseMap _where;
    _where.insert(Clinical::staticMetaObject.indexOfProperty("patient_id"), Comparison(AND_Equals, _id));
    OrderByClauseMap _order;
    _order.insert(Clinical::staticMetaObject.indexOfProperty("date"), Ascending);
    Response r = getDBObjects(Clinical::staticMetaObject,  0, _where, _order);
    if (r.error != NoError) {
        return r;
    }
    foreach (QObject* qobj, r.results) {
        _in.append(SharedClinical(qobj));
    }
    return r;
}

Response GenericLocalDB::deleteClinical(SharedClinical & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::deleteClinical(int _id) {
    WhereClauseMap _where;
    _where.insert(Clinical::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Clinical::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    return deleteDBObjects(r.results, 0);
}

Response GenericLocalDB::getPatientVariateFromId(int _id, SharedPatientVariate& _p) {
    WhereClauseMap _where;
    _where.insert(PatientVariate::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(PatientVariate::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedPatientVariate _sp = SharedPatientVariate(r.castresults<PatientVariate*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setPatientVariate(SharedPatientVariate& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::setPatientVariate(int _pid, SharedPatientVariate& _p) {
    WhereClauseMap _where;
    _where.insert(Patient::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _pid));
    Response r = getDBObjects(Patient::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    _p->setPatient(SharedPatient(r.castresults<Patient*>().last()));
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getPatientVariatesList(QList<SharedPatientVariate>& _in) {
    Response r = getDBObjects(PatientVariate::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedPatientVariate(qobj));
    }
    return r;
}

Response GenericLocalDB::getPatientVariatesByDate(int _id, QList<SharedPatientVariate>& _in) {
    WhereClauseMap _where;
    _where.insert(PatientVariate::staticMetaObject.indexOfProperty("patient_id"), Comparison(AND_Equals, _id));
    OrderByClauseMap _order;
    _order.insert(PatientVariate::staticMetaObject.indexOfProperty("date"), Ascending);
    Response r = getDBObjects(PatientVariate::staticMetaObject,  0, _where, _order);
    if (r.error != NoError) {
        return r;
    }
    foreach (QObject* qobj, r.results) {
        _in.append(SharedPatientVariate(qobj));
    }
    return r;
}

Response GenericLocalDB::deletePatientVariate(SharedPatientVariate & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::deletePatientVariate(int _id) {
    WhereClauseMap _where;
    _where.insert(PatientVariate::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(PatientVariate::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    return deleteDBObjects(r.results, 0);
}

Response GenericLocalDB::getDrugVariateFromId(int _id, SharedDrugVariate& _p) {
    WhereClauseMap _where;
    _where.insert(DrugVariate::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(DrugVariate::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedDrugVariate _sp = SharedDrugVariate(r.castresults<DrugVariate*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setDrugVariate(int _id, SharedDrugVariate & _in) {
    WhereClauseMap _where;
    _where.insert(Drug::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Drug::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    _in->setDrug(SharedDrug(r.castresults<Drug*>().last()));
    return setDBObject(*_in, _in->parent());
}

Response GenericLocalDB::getDrugVariatesList(QList<SharedDrugVariate>& _in) {
    Response r = getDBObjects(DrugVariate::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedDrugVariate(qobj));
    }
    return r;
}

Response GenericLocalDB::getDrugVariatesByDate(int _id, QList<SharedDrugVariate>& _in) {
    WhereClauseMap _where;
    _where.insert(DrugVariate::staticMetaObject.indexOfProperty("drug_id"), Comparison(AND_Equals, _id));
    OrderByClauseMap _order;
    _order.insert(DrugVariate::staticMetaObject.indexOfProperty("date"), Ascending);
    Response r = getDBObjects(DrugVariate::staticMetaObject,  0, _where, _order);
    if (r.error != NoError) {
        return r;
    }
    foreach (QObject* qobj, r.results) {
        _in.append(SharedDrugVariate(qobj));
    }
    return r;
}

Response GenericLocalDB::deleteDrugVariate(SharedDrugVariate & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::deleteDrugVariate(int _id) {
    WhereClauseMap _where;
    _where.insert(DrugVariate::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(DrugVariate::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    return deleteDBObjects(r.results, 0);
}

Response GenericLocalDB::getCurveFromId(int _id, SharedCurve& _p) {
    WhereClauseMap _where;
    _where.insert(NewCurve::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(NewCurve::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedCurve _sp = SharedCurve(r.castresults<NewCurve*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setCurve(SharedCurve& _p) {
    WhereClauseMap _where;
//    _where.insert(Drug::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _p->drugname()));
//    Response r = getDBObjects(Drug::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }
//    _p->setDrug(SharedDrug(r.castresults<Drug*>().first()));

//    _where.insert(Model::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _p->modelname()));
//    Response r = getDBObjects(Model::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }

//    _where.insert(PercentileEngine::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _p->percentileEnginename()));
//    r = getDBObjects(PercentileEngine::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }

//    _where.insert(PostEngine::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _p->postEnginename()));
//    r = getDBObjects(PostEngine::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }

//    _where.insert(ReverseEngine::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _p->reverseEnginename()));
//    r = getDBObjects(ReverseEngine::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }

    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getCurvesList(QList<SharedCurve>& _in) {
    Response r = getDBObjects(NewCurve::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedCurve(qobj));
    }
    return r;
}

Response GenericLocalDB::getCurvesList(QList<int>& _in, QStringList& _slist) {
    SelectClauseMap _select;
    _select.append(NewCurve::staticMetaObject.indexOfProperty("id"));
    _select.append(NewCurve::staticMetaObject.indexOfProperty("name"));
    QList<QVariant> r = getDBData(NewCurve::staticMetaObject, 0, _select);
    for(QList<QVariant>::iterator it = r.begin(); it != r.end(); ++it) {
        _in.append(it->toInt());
        it++;
        _slist.append(it->toString());
    }
    return Response();
}

Response GenericLocalDB::getCurvesList(QList<int>& _in, const ident &_id, const QString &_drugid) {
    WhereClauseMap _where;
    _where.insert(NewCurve::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    _where.insert(NewCurve::staticMetaObject.indexOfProperty("drug"), Comparison(AND_Equals, _drugid));
    SelectClauseMap _select;
    _select.append(NewCurve::staticMetaObject.indexOfProperty("id"));
    QList<QVariant> r = getDBData(NewCurve::staticMetaObject, 0, _select, _where);
    for(QList<QVariant>::iterator it = r.begin(); it != r.end(); ++it) {
        _in.append(it->toInt());
    }
    return Response();
}

Response GenericLocalDB::getCurvesList(QList<SharedCurve>& _in, const ident &_id, const QString &_drugid) {
    WhereClauseMap _where;
    _where.insert(NewCurve::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    _where.insert(NewCurve::staticMetaObject.indexOfProperty("drug"), Comparison(AND_Equals, _drugid));
    Response r = getDBObjects(NewCurve::staticMetaObject, 0, _where);
    QList<NewCurve*> _tmp = r.castresults<NewCurve*>();
    foreach (NewCurve* _nc, _tmp) {
        _in.append(SharedCurve(_nc));
    }

    return r;
}

Response GenericLocalDB::deleteCurve(SharedCurve & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::deleteCurve(int _id) {
    WhereClauseMap _where;
    _where.insert(NewCurve::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(NewCurve::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    return deleteDBObjects(r.results, 0);
}

Response GenericLocalDB::getPredictionFromId(int _id, SharedPrediction& _p) {
    WhereClauseMap _where;
    _where.insert(Prediction::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Prediction::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedPrediction _sp = SharedPrediction(r.castresults<Prediction*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setPrediction(SharedPrediction& _p) {
    WhereClauseMap _where;
//    _where.insert(Drug::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _p->drugname()));
//    Response r = getDBObjects(Drug::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }
//    _p->setDrug(SharedDrug(r.castresults<Drug*>().first()));

//    _where.insert(Model::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _p->modelname()));
//    Response r = getDBObjects(Model::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }

//    _where.insert(PercentileEngine::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _p->percentileEnginename()));
//    r = getDBObjects(PercentileEngine::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }

//    _where.insert(PostEngine::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _p->postEnginename()));
//    r = getDBObjects(PostEngine::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }

//    _where.insert(ReverseEngine::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _p->reverseEnginename()));
//    r = getDBObjects(ReverseEngine::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }

    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getPredictionsList(QList<SharedPrediction>& _in) {
    Response r = getDBObjects(Prediction::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedPrediction(qobj));
    }
    return r;
}

Response GenericLocalDB::getPredictionsList(QList<int>& _in, QStringList& _slist) {
    SelectClauseMap _select;
    _select.append(Prediction::staticMetaObject.indexOfProperty("id"));
    _select.append(Prediction::staticMetaObject.indexOfProperty("name"));
    QList<QVariant> r = getDBData(Prediction::staticMetaObject, 0, _select);
    for(QList<QVariant>::iterator it = r.begin(); it != r.end(); ++it) {
        _in.append(it->toInt());
        it++;
        _slist.append(it->toString());
    }
    return Response();
}

Response GenericLocalDB::getPredictionsList(QList<int>& _in, const ident &_id, const QString &_drugid) {
    WhereClauseMap _where;
    _where.insert(Prediction::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    _where.insert(Prediction::staticMetaObject.indexOfProperty("drug_id"), Comparison(AND_Equals, _drugid));
    SelectClauseMap _select;
    _select.append(Prediction::staticMetaObject.indexOfProperty("id"));
    QList<QVariant> r = getDBData(Prediction::staticMetaObject, 0, _select, _where);
    for(QList<QVariant>::iterator it = r.begin(); it != r.end(); ++it) {
        _in.append(it->toInt());
    }
    return Response();
}

Response GenericLocalDB::getPredictionsList(QList<SharedPrediction>& _in, const ident &_id, const QString &_drugid) {
    WhereClauseMap _where;
    _where.insert(Prediction::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    _where.insert(Prediction::staticMetaObject.indexOfProperty("drug_id"), Comparison(AND_Equals, _drugid));
    Response r = getDBObjects(Prediction::staticMetaObject, 0, _where);
    QList<Prediction*> _tmp = r.castresults<Prediction*>();
    foreach (Prediction* _nc, _tmp) {
        _in.append(SharedPrediction(_nc));
    }

    return r;
}

Response GenericLocalDB::deletePrediction(SharedPrediction & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::deletePrediction(int _id) {
    WhereClauseMap _where;
    _where.insert(Prediction::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Prediction::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    return deleteDBObjects(r.results, 0);
}

Response GenericLocalDB::getDosageFromId(int _id, SharedDosage& _p) {
    WhereClauseMap _where;
    _where.insert(Dosage::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Dosage::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedDosage _sp = SharedDosage(r.castresults<Dosage*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setDosage(SharedDosage& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getDosagesList(SharedDosageHistory& _in) {
    Response r = getDBObjects(Dosage::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in->append(SharedDosage(qobj));
    }
    return r;
}

Response GenericLocalDB::getDosagesByDate(int _id, SharedDosageHistory& _in) {
    WhereClauseMap _where;
    _where.insert(Dosage::staticMetaObject.indexOfProperty("prediction_id"), Comparison(AND_Equals, _id));
    OrderByClauseMap _order;
    _order.insert(Dosage::staticMetaObject.indexOfProperty("applied"), Ascending);
    Response r = getDBObjects(Dosage::staticMetaObject,  0, _where, _order);
    if (r.error != NoError) {
        return r;
    }
    foreach (QObject* qobj, r.results) {
        _in->append(SharedDosage(qobj));
    }
    return r;
}

Response GenericLocalDB::deleteDosage(SharedDosage & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::deleteDosage(int _id) {
    WhereClauseMap _where;
    _where.insert(Dosage::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Dosage::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    return deleteDBObjects(r.results, 0);
}

Response GenericLocalDB::getDrugFromId(int _id, SharedDrug& _p) {
    WhereClauseMap _where;
    _where.insert(Drug::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Drug::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedDrug _sp = SharedDrug(r.castresults<Drug*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setDrug(SharedDrug& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getDrugsList(QList<SharedDrug>& _in) {
    Response r = getDBObjects(Drug::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedDrug(qobj));
    }
    return r;
}

Response GenericLocalDB::deleteDrug(SharedDrug & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::setValidDoses(int _id, SharedValidDoses &_in) {
    WhereClauseMap _where;
    _where.insert(Drug::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Drug::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    QList<QObject*> _objs;
    for(int i = 0; i < _in->size(); i++) {
        _in->at(i)->setDrug(QPointer<Drug>(r.castresults<Drug*>().first()));
        _objs.append((*_in)[i].data());
    }
    return setDBObjects(_objs, 0);
}

Response GenericLocalDB::setValidIntervals(int _id, SharedValidIntervals &_in) {
    WhereClauseMap _where;
    _where.insert(Drug::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Drug::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    QList<QObject*> _objs;
    for(int i = 0; i < _in->size(); i++) {
        _in->at(i)->setDrug(QPointer<Drug>(r.castresults<Drug*>().first()));
        _objs.append((*_in)[i].data());
    }
    return setDBObjects(_objs, 0);
}

Response GenericLocalDB::setValidInfusions(int _id, SharedValidInfusions &_in) {
    WhereClauseMap _where;
    _where.insert(Drug::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Drug::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    QList<QObject*> _objs;
    for(int i = 0; i < _in->size(); i++) {
        _in->at(i)->setDrug(QPointer<Drug>(r.castresults<Drug*>().first()));
        _objs.append((*_in)[i].data());
    }
    return setDBObjects(_objs, 0);
}

Response GenericLocalDB::getEmailFromId(int _id, SharedEmail& _p) {
    WhereClauseMap _where;
    _where.insert(Email::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Email::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedEmail _sp = SharedEmail(r.castresults<Email*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setEmail(SharedEmail& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getEmailsList(QList<SharedEmail>& _in) {
    Response r = getDBObjects(Email::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedEmail(qobj));
    }
    return r;
}

Response GenericLocalDB::deleteEmail(SharedEmail & _p) {
    return deleteDBObject(*_p, _p->parent());
}

//Response GenericLocalDB::getInfusionsFromId(int _id, SharedValidInfusions& _p) {
//    WhereClauseMap _where;
//    _where.insert(ValidInfusions::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
//    Response r = getDBObjects(ValidInfusions::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }
//    SharedValidInfusions _sp = SharedValidInfusions(r.castresults<ValidInfusions*>().first());
//    _p.swap(_sp);
//    return r;
//}

//Response GenericLocalDB::setInfusions(SharedValidInfusions& _p) {
//    return setDBObject(*_p, _p->parent());
//}

//Response GenericLocalDB::getInfusionsList(QList<SharedValidInfusions>& _in) {
//    Response r = getDBObjects(ValidInfusions::staticMetaObject, 0);
//    foreach (QObject* qobj, r.results) {
//        _in.append(SharedValidInfusions(qobj));
//    }
//    return r;
//}

//Response GenericLocalDB::deleteInfusions(SharedValidInfusions & _p) {
//    return deleteDBObject(*_p, _p->parent());
//}

//Response GenericLocalDB::getIntervalsFromId(int _id, SharedValidIntervals& _p) {
//    WhereClauseMap _where;
//    _where.insert(ValidIntervals::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
//    Response r = getDBObjects(ValidIntervals::staticMetaObject, 0, _where);
//    if (r.error != NoError) {
//        return r;
//    }
//    SharedValidIntervals _sp = SharedValidIntervals(r.castresults<ValidIntervals*>().first());
//    _p.swap(_sp);
//    return r;
//}

//Response GenericLocalDB::setIntervals(SharedValidIntervals& _p) {
//    return setDBObject(*_p, _p->parent());
//}

//Response GenericLocalDB::getIntervalssList(QList<SharedValidIntervals>& _in) {
//    Response r = getDBObjects(ValidIntervals::staticMetaObject, 0);
//    foreach (QObject* qobj, r.results) {
//        _in.append(SharedValidIntervals(qobj));
//    }
//    return r;
//}

//Response GenericLocalDB::deleteIntervals(SharedValidIntervals & _p) {
//    return deleteDBObject(*_p, _p->parent());
//}

Response GenericLocalDB::getMeasureFromId(int _id, SharedMeasure& _p) {
    WhereClauseMap _where;
    _where.insert(Measure::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Measure::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedMeasure _sp = SharedMeasure(r.castresults<Measure*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setMeasure(SharedMeasure& _p) {
    _p->setValue(_p->amount().value());
    _p->setUnit(_p->amount().unit().toString());
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::setMeasure(int _pid, SharedMeasure& _p) {
    WhereClauseMap _where;
    _where.insert(Patient::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _pid));
    Response r = getDBObjects(Patient::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    _p->setPatient(SharedPatient(r.castresults<Patient*>().last()));

    _where.clear();
    QString _nm = _p->sdrug().split(".").last();
    _nm[0] = _nm[0].toUpper();
    _where.insert(Drug::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _nm));
    r = getDBObjects(Drug::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    _p->setDrug(SharedDrug(r.castresults<Drug*>().last()));

    _p->setValue(_p->amount().value());
    _p->setUnit(_p->amount().unit().toString());
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getMeasuresList(QList<SharedMeasure>& _in) {
    Response r = getDBObjects(Measure::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedMeasure(qobj));
    }
    return r;
}

Response GenericLocalDB::deleteMeasure(SharedMeasure & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::deleteMeasure(int _id) {
    WhereClauseMap _where;
    _where.insert(Measure::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Measure::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    return deleteDBObjects(r.results, 0);
}

Response GenericLocalDB::getMeasuresByDate(int _id, QList<SharedMeasure>& _in) {
    WhereClauseMap _where;
    _where.insert(Measure::staticMetaObject.indexOfProperty("patient_id"), Comparison(AND_Equals, _id));
    OrderByClauseMap _order;
    _order.insert(Measure::staticMetaObject.indexOfProperty("arrivalDate"), Ascending);
    Response r = getDBObjects(Measure::staticMetaObject,  0, _where, _order);
    if (r.error != NoError) {
        return r;
    }
    foreach (QObject* qobj, r.results) {
        _in.append(SharedMeasure(qobj));
    }
    return r;
}

Response GenericLocalDB::getMeasuresFromPatientAndDrug(const int& _id, const QString& _drugid, QList<SharedMeasure>& _in) {
    WhereClauseMap _dwhere;
    _dwhere.insert(Drug::staticMetaObject.indexOfProperty("name"), Comparison(AND_Equals, _drugid));
    SelectClauseMap _dselect;
    _dselect.append(Drug::staticMetaObject.indexOfProperty("id"));
    QList<QVariant> r = getDBData(Drug::staticMetaObject, 0, _dselect, _dwhere);
    if (r.empty()) {
        return Response("Drug", NotFound);
    }
    WhereClauseMap _mwhere;
    _mwhere.insert(Measure::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, r.first()));
    _mwhere.insert(Measure::staticMetaObject.indexOfProperty("patient_id"), Comparison(AND_Equals, _id));
    return getDBObjects(Measure::staticMetaObject, 0, _mwhere);

}


Response GenericLocalDB::getOperationFromId(int _id, SharedOperation& _p) {
    WhereClauseMap _where;
    _where.insert(Operation::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Operation::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedOperation _sp = SharedOperation(r.castresults<Operation*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setOperation(SharedOperation& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getOperationsList(QList<SharedOperation>& _in) {
    Response r = getDBObjects(Operation::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedOperation(qobj));
    }
    return r;
}

Response GenericLocalDB::deleteOperation(SharedOperation & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getParameterFromId(int _id, SharedParameter& _p) {
    WhereClauseMap _where;
    _where.insert(Parameter::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Parameter::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedParameter _sp = SharedParameter(r.castresults<Parameter*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setParameter(SharedParameter& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getParametersList(QList<SharedParameter>& _in) {
    Response r = getDBObjects(Parameter::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedParameter(qobj));
    }
    return r;
}

Response GenericLocalDB::deleteParameter(SharedParameter & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getPhoneFromId(int _id, SharedPhone& _p) {
    WhereClauseMap _where;
    _where.insert(Phone::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Phone::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedPhone _sp = SharedPhone(r.castresults<Phone*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setPhone(SharedPhone& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getPhonesList(QList<SharedPhone>& _in) {
    Response r = getDBObjects(Phone::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedPhone(qobj));
    }
    return r;
}

Response GenericLocalDB::deletePhone(SharedPhone & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getPosologyFromId(int _id, SharedPosology& _p) {
    WhereClauseMap _where;
    _where.insert(Posology::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Posology::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedPosology _sp = SharedPosology(r.castresults<Posology*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setPosology(SharedPosology& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getPosologysList(QList<SharedPosology>& _in) {
    Response r = getDBObjects(Posology::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedPosology(qobj));
    }
    return r;
}

Response GenericLocalDB::deletePosology(SharedPosology & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getTargetFromId(int _id, SharedTarget& _p) {
    WhereClauseMap _where;
    _where.insert(Target::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Target::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedTarget _sp = SharedTarget(r.castresults<Target*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setTarget(SharedTarget& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getTargetsList(QList<SharedTarget>& _in) {
    Response r = getDBObjects(Target::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedTarget(qobj));
    }
    return r;
}

Response GenericLocalDB::deleteTarget(SharedTarget & _p) {
    return deleteDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getLocationFromId(int _id, SharedLocation& _p) {
    WhereClauseMap _where;
    _where.insert(Location::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Location::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    SharedLocation _sp = SharedLocation(r.castresults<Location*>().first());
    _p.swap(_sp);
    return r;
}

Response GenericLocalDB::setLocation(SharedLocation& _p) {
    return setDBObject(*_p, _p->parent());
}

Response GenericLocalDB::getLocationsList(QList<SharedLocation>& _in) {
    Response r = getDBObjects(Location::staticMetaObject, 0);
    foreach (QObject* qobj, r.results) {
        _in.append(SharedLocation(qobj));
    }
    return r;
}

Response GenericLocalDB::deleteLocation(SharedLocation & _p) {
    return deleteDBObject(*_p, _p->parent());
}
// Encryption /////////////////////////////////////////////////////////////////////////////////////

Tucuxi::Response GenericLocalDB::isEncrypted(bool &encrypted)
{
    Tucuxi::Response r(_id);

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

Tucuxi::Response GenericLocalDB::setEncrypted(bool encrypted)
{
    Tucuxi::Response r(_id);

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

Tucuxi::Response GenericLocalDB::encryptDatabase(bool encrypt)
{
    bool status;

    //Get the encryption status
    Tucuxi::Response response = isEncrypted(status);
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
bool GenericLocalDB::enableFkSupport()
{
    QSqlQuery query("PRAGMA foreign_keys = ON");
    return query.isActive();
}

//Find the ID or create this element
int GenericLocalDB::findOrCreate (const QString &id, const QString &table)
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
Tucuxi::Response GenericLocalDB::queryError(const QSqlQuery &query) const
{
    Tucuxi::Response response(_id);
    response.message = QString("[Code %1] %2;\n%3.").arg(QString::number(query.lastError().number()), query.lastError().databaseText(), query.lastError().driverText());
    response.error   = QueryError;

    return response;
}

//Return an error message on a database failure
Tucuxi::Response GenericLocalDB::databaseError(const QSqlDatabase &database, const QString &message) const
{
    Tucuxi::Response response(_id);

    response.message = QString("%1 The database returned the following error:\n\n[Code %2] %3;\n%4.").arg(message, QString::number(database.lastError().number()), database.lastError().databaseText(), database.lastError().driverText());
    response.error   = QueryError;

    return response;
}

//Starts a transaction
Tucuxi::Response GenericLocalDB::startTransaction(const QString &id)
{
    Tucuxi::Response response(_id);

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
Tucuxi::Response GenericLocalDB::rollbackTransaction(const QString &id)
{
    Tucuxi::Response response(_id);

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
Tucuxi::Response GenericLocalDB::commitTransaction(const QString &id)
{
    Tucuxi::Response response(_id);

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
bool GenericLocalDB::exists(const QString &table, const QString &pk, const QString &value)
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
bool GenericLocalDB::exists(const QString &table, const QString &pk, int value)
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

Tucuxi::Response GenericLocalDB::encryptPersons(bool encrypt)
{
    Tucuxi::Response response(_id);

    //Get the list of persons
//    QList<Person> persons;

//    if ((response = getPersonsList(persons)).error != NoError)
//        return response;

//    //Set the encryption status
//    _encryption = encrypt ? Encrypted : Clear;

//    //Update each person
//    for (int i = 0; i < persons.size(); i++)
//        if ((response = setPerson(persons[i])).error != NoError)
//            return response;

//    //Reset the encryption status
//    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Tucuxi::Response GenericLocalDB::encryptPatients(bool encrypt)
{
    Tucuxi::Response response(_id);

    //Get the list of patients
//    QList<Patient> patients;

//    if ((response = getPatientsList(patients)).error != NoError)
//        return response;

//    //Set the encryption status
//    _encryption = encrypt ? Encrypted : Clear;

//    //Update each patient
//    for (int i = 0; i < patients.size(); i++)
//        if ((response = setPatient(patients[i])).error != NoError)
//            return response;

//    //Reset the encryption status
//    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Tucuxi::Response GenericLocalDB::encryptPracticians(bool encrypt)
{
    Tucuxi::Response response(_id);

    //Get the list of practicians
//    QList<Practician> practicians;

//    if ((response = getPracticiansList(practicians)).error != NoError)
//        return response;

//    //Set the encryption status
//    _encryption = encrypt ? Encrypted : Clear;

//    //Update each practician
//    for (int i = 0; i < practicians.size(); i++)
//        if ((response = setPractician(practicians[i])).error != NoError)
//            return response;

//    //Reset the encryption status
//    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Tucuxi::Response GenericLocalDB::encryptLocations(bool encrypt)
{
    Tucuxi::Response response(_id);

//    //Get the list of locations
//    QList<Location> locations;

//    if ((response = getLocationsList(locations)).error != NoError)
//        return response;

//    //Set the encryption status
//    _encryption = encrypt ? Encrypted : Clear;

//    //Update each location
//    for (int i = 0; i < locations.size(); i++)
//        if ((response = setLocation(locations[i])).error != NoError)
//            return response;

//    //Reset the encryption status
//    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Tucuxi::Response GenericLocalDB::encryptInstitutes(bool encrypt)
{
    Tucuxi::Response response(_id);

    //Get the list of institutes
//    QList<Institute> institutes;

//    if ((response = getInstitutesList(institutes)).error != NoError)
//        return response;

//    //Set the encryption status
//    _encryption = encrypt ? Encrypted : Clear;

//    //Update each institute
//    for (int i = 0; i < institutes.size(); i++)
//        if ((response = setInstitute(institutes[i])).error != NoError)
//            return response;

//    //Reset the encryption status
//    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Tucuxi::Response GenericLocalDB::encryptReports(bool encrypt)
{
    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // Implement me when the reports database function are implemented
    // ////////////////////////////////////////////////////////////////////////////////////////////

    Q_UNUSED(encrypt);
    return Tucuxi::Response(_id);
}

Tucuxi::Response GenericLocalDB::encryptCovariates(bool encrypt)
{
    Tucuxi::Response response(_id);

    //Get the list of patients
//    QList<Patient> patients;
//    if ((response = getPatientsList(patients)).error != NoError)
//        return response;

//    //Update each patient's covariates
//    for (int i = 0; i < patients.size(); i++) {

//        //Get the list of covariates
//        QList<Covariate> covariates;

//        if ((response = getCovariatesByDate(patients.at(i).id, covariates)).error != NoError)
//            return response;

//        //Set the encryption status
//        _encryption = encrypt ? Encrypted : Clear;

//        //Update each covariate
//        for (int j = 0; j < covariates.size(); j++)
//            if ((response = setCovariate(patients.at(i).id, covariates[j])).error != NoError)
//                return response;

//        //Reset the encryption status
//        _encryption = encrypt ? Clear : Encrypted;
//    }

    return response;
}

Tucuxi::Response GenericLocalDB::encryptClinicals(bool encrypt)
{
    Tucuxi::Response response(_id);

    //Get the list of patients
//    QList<Patient> patients;
//    if ((response = getPatientsList(patients)).error != NoError)
//        return response;

//    //Update each patient's clinicals
//    for (int i = 0; i < patients.size(); i++) {

//        //Get the list of clinicals
//        QList<Clinical> clinicals;

//        if ((response = getClinicalsByDate(patients.at(i).id, clinicals)).error != NoError)
//            return response;

//        //Set the encryption status
//        _encryption = encrypt ? Encrypted : Clear;

//        //Update each clinical
//        for (int j = 0; j < clinicals.size(); j++)
//            if ((response = setClinical(patients.at(i).id, clinicals[j])).error != NoError)
//                return response;

//        //Reset the encryption status
//        _encryption = encrypt ? Clear : Encrypted;
//    }

    return response;
}

Tucuxi::Response GenericLocalDB::encryptMeasures(bool encrypt)
{
    Tucuxi::Response response(_id);

    //Get the list of patients
//    QList<Patient> patients;
//    if ((response = getPatientsList(patients)).error != NoError)
//        return response;

//    //Update each patient's measures
//    for (int i = 0; i < patients.size(); i++) {

//        //Get the list of measures
//        QList<Measure> measures;

//        if ((response = getMeasuresByDate(patients.at(i).id, measures)).error != NoError)
//            return response;

//        //Set the encryption status
//        _encryption = encrypt ? Encrypted : Clear;

//        //Update each measure
//        for (int j = 0; j < measures.size(); j++)
//            if ((response = setMeasure(patients.at(i).id, measures[j])).error != NoError)
//                return response;

//        //Reset the encryption status
//        _encryption = encrypt ? Clear : Encrypted;
//    }

    return response;
}

Tucuxi::Response GenericLocalDB::encryptCurves(bool encrypt)
{
    Tucuxi::Response response(_id);

    //Get the list of curves
//    QList<Curve> curves;

//    if ((response = getCurvesList(curves)).error != NoError)
//        return response;

//    //Set the encryption status
//    _encryption = encrypt ? Encrypted : Clear;

//    //Update each curve
//    for (int i = 0; i < curves.size(); i++)
//        if ((response = setCurve(curves[i])).error != NoError)
//            return response;

//    //Reset the encryption status
//    _encryption = encrypt ? Clear : Encrypted;

    return response;
}

Tucuxi::Response GenericLocalDB::encryptDosages(bool encrypt)
{
    Tucuxi::Response response(_id);

    //Get the list of curves
//    QList<Curve> curves;

//    if ((response = getCurvesList(curves)).error != NoError)
//        return response;

//    //Update each curve's dosages
//    for (int i = 0; i < curves.size(); i++) {

//        //Get the list of dosages
//        QList<Dosage> dosages;

//        if ((response = getDosagesByDate(curves.at(i).id, dosages)).error != NoError)
//            return response;

//        //Set the encryption status
//        _encryption = encrypt ? Encrypted : Clear;

//        //Update each dosages
//        for (int j = 0; j < dosages.size(); j++)
//            if ((response = setDosage(curves.at(i).id, dosages[j])).error != NoError)
//                return response;

//        //Reset the encryption status
//        _encryption = encrypt ? Clear : Encrypted;
//    }

    return response;
}

}
