//@@license@@

#include "genericrepository.h"

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
#include "securitymanager.h"
#include "coremessagehandler.h"
#include "dal/genericdbobjects.h"
#include "genericdatabase.h"
#include "QSqlQuery"
#include <QUrl>

namespace ezechiel {

void GenericRepositoryFactory::init (Core* core) {

}

PluginBase* GenericRepositoryFactory::newPlugin (QString &id) {
    return 0;//new GenericRepository();
}

DescriptorBase* GenericRepositoryFactory::descriptor() {
    desc.id = "ch.heig-vd.ezechiel.GenericLocalDB";
    desc.name = "GenericLocalDB";
    return &desc;
}
//int GenericLocalDBFactory::priority(const QString &id) const {
//    return -1;
//}

//int GenericLocalDB::priority(const QString &id) const {
//    return -1;
//}







Response GenericRepository::save(Patient *patient)
{
    database->setDBObject(*(dynamic_cast<QObject*>(patient)),*currentUser);
}

Response GenericRepository::save(Practician *practician)
{

}

Response GenericRepository::save(Institute *institute)
{

}

Response GenericRepository::save(Drug *drug)
{

}

Response GenericRepository::save(MedicalTreatment *treatment)
{

}

Response GenericRepository::save(TreatmentAnalysis *analysis)
{

}

Response GenericRepository::save(ReportData *report)
{

}


Response GenericRepository::deleteObject(Aggregate *aggregate)
{

}


// We can us pointers to pointers so as to let the repository allocate the object
Response GenericRepository::getPatientById(int id, Patient **patient)
{
    ezechiel::Response r;
    WhereClauseMap dvwhere;
    dvwhere.insert(Patient::staticMetaObject.indexOfProperty("patient_id"), Comparison(AND_Equals, id));
    r = database->getDBObjects(Patient::staticMetaObject, *currentUser, dvwhere);
    QList<Patient*> list = r.castresults<Patient*>();
    if (list.size() != 1) {
        // Error
        return r;
    }
    *patient = list.first();
    return r;


}

Response GenericRepository::getPracticianById(int id, Practician **practician)
{

}

Response GenericRepository::getInstituteById(int id, Institute **institute)
{

}

Response GenericRepository::getDrugById(int id, Drug **drug)
{

}

Response GenericRepository::getMedicalTreatmentById(int id, MedicalTreatment **treatment)
{

}

Response GenericRepository::getTreatmentAnalysisById(int id, TreatmentAnalysis **analysis)
{

}

Response GenericRepository::getReportById(int id, ReportData **report)
{

}

// returns a list of patients (not all their information, but only first name and last name
Response GenericRepository::getPatientList(QString from,QString to, QList<ShortPatient> *list)
{

}






}
