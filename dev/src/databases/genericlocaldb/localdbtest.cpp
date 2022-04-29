//@@license@@

#include <QString>
#include <QApplication>
#include <QCoreApplication>

#include "testmessagehandler.h"

#include "core.h"
#include "definitionmanager.h"
#include "pluginmanager.h"
#include "dbinterface.h"
#include "database.h"
#include "security.h"
#include "dbobjects.h"
#include "distributor.h"
#include "version.h"
#include "config.h"
#include "localdb.h"

int main (int argc, char *argv[])
{
    QApplication app(argc,argv);
    CORE->setPath(Core::ApplicationData, CORE->path(Core::Executable));

    TESTING(app.arguments());
    
    TEST("Create");
    LocalDB *db = new LocalDB();
    db->init(CORE);
    PASS;
    
    TEST("Connect to the test DB");
    DBAccount account;
    account.url = "local.db";
    ASSERT(db->connect(account));
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();


    // Location tests
    MESSAGE("--> Locations:");

    TEST("Get a test location");
    DBLocation loc;
    db->getLocation(0,loc);
    MULTI();
    VASSERT(loc.id, 0);
    VASSERT(loc.country, "CH");
    VASSERT(loc.state, "VD");
    VASSERT(loc.address, "Rue de Lausanne 1");
    VASSERT(loc.city, "Lausanne");
    VASSERT(loc.postcode, "1000");
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Add a new location");
    loc.state = "A";
    loc.country = "B";
    loc.address = "C";
    loc.city = "D";
    loc.postcode = "E";
    loc.id = invalid_ident;
    ASSERT(db->setLocation(loc).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Modify the new location");
    loc.state = "AA";
    loc.country = "BB";
    loc.address = "CC";
    loc.city = "DD";
    loc.postcode = "EE";
    ASSERT(db->setLocation(loc).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Get the new location");
    DBLocation loc2;
    MULTI();
    ASSERT(db->getLocation(loc.id,loc2).error == NoError);
    VASSERT(loc2.id, loc.id);
    VASSERT(loc2.address, loc.address);
    VASSERT(loc2.state, loc.state);
    VASSERT(loc2.country, loc.country);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Delete the new location");
    MULTI();
    ASSERT(db->deleteLocation(loc2.id).error == NoError);
    ASSERT(db->getLocation(loc2.id, loc).error == NotFound);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();


    // Institute tests
    MESSAGE("--> Institutes:");

    TEST("Get a test institute");
    DBInstitute inst;
    db->getInstitute(0, inst);
    MULTI();
    VASSERT(inst.id, 0);
    VASSERT(inst.name, "CHUV");
    VASSERT(inst.location, 5);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Institute insertion constraint check");
    inst.location = 100; // Constraint check failure
    inst.id = invalid_ident;
    ASSERT(db->setInstitute(inst).error == NotFound);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Add a new institute (No logo)");
    MULTI();
    loc2.state = "A";
    loc2.country = "B";
    loc2.address = "C";
    loc2.id = invalid_ident;
    ASSERT(db->setLocation(loc2).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    inst.name = "A";
    inst.location = loc2.id;
    inst.id = invalid_ident;
    ASSERT(db->setInstitute(inst).error == NoError);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Modify the new institute (No logo)");
    inst.name = "B";
    ASSERT(db->setInstitute(inst).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Get the new institute (No logo)");
    DBInstitute inst2;
    MULTI();
    ASSERT(db->getInstitute(inst.id, inst2).error == NoError);
    VASSERT(inst2.id, inst.id);
    VASSERT(inst2.name, inst.name);
    VASSERT(inst2.location, inst.location);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Delete the new institute");
    MULTI();
    ASSERT(db->deleteInstitute(inst2.id).error == NoError);
    ASSERT(db->getInstitute(inst2.id, inst).error == NotFound);
    ASSERT(db->getLocation(loc2.id, loc).error == NotFound);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();


    // Person tests
    MESSAGE("--> Persons:");

    TEST("Get a test person");
    DBPerson per;
    QDate birth(1901, 01, 01);
    db->getPerson(0, per);
    MULTI();
    VASSERT(per.id, 0);
    VASSERT(per.name, "Test1");
    VASSERT(per.firstname, "Patient");
    ASSERT(per.gender == MALE);
    VASSERT(per.birthday.toString(), birth.toString());
    VASSERT(per.location, 0);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Person insertion constraint check");
    per.location = 100;
    per.id = invalid_ident;
    ASSERT(db->setPerson(per).error == NotFound);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Add a new person");
    MULTI();
    loc2.state = "A";
    loc2.country = "B";
    loc2.address = "C";
    loc2.id = invalid_ident;
    ASSERT(db->setLocation(loc2).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    per.name = "A";
    per.firstname = "B";
    per.birthday = birth;
    per.location = loc2.id;
    per.emails << "a@b.com";
    per.phones << DBPhone("fax","000");
    per.id = invalid_ident;
    ASSERT(db->setPerson(per).error == NoError);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Modify the new person");
    per.name = "C";
    ASSERT(db->setPerson(per).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Get the new person");
    DBPerson per2;
    MULTI();
    ASSERT(db->getPerson(per.id, per2).error == NoError);
    VASSERT(per2.id, per.id);
    VASSERT(per2.name, per.name);
    VASSERT(per2.firstname, per.firstname);
    VASSERT(per2.birthday.toString(), per.birthday.toString());
    VASSERT(per2.location, per.location);
    VASSERT(per2.emails.at(0), per.emails.at(0));
    VASSERT(per2.phones.at(0).number, per.phones.at(0).number);
    VASSERT(per2.phones.at(0).type, per.phones.at(0).type);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Delete the new person");
    MULTI();

    DBPatient pat;
    pat.id = invalid_ident;
    pat.person = per2.id;
    ASSERT(db->setPatient(pat).error == NoError);
    ASSERT(db->deletePerson(per2.id).error == NoError);
    ASSERT(db->getPerson(per2.id, per).error == NotFound);
    ASSERT(db->getLocation(loc2.id, loc).error == NotFound);
    ASSERT(db->getPatient(pat.id, pat).error == NotFound);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();


    // Patient tests
    MESSAGE("--> Patients:");

    TEST("Get a test patient");
    db->getPatient(0, pat);
    MULTI();
    VASSERT(pat.id, 0);
    ASSERT(!pat.stat_ok);
    VASSERT(pat.person, 0);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Patient insertion constraint check");
    pat.person = 100;
    pat.id = invalid_ident;
    ASSERT(db->setPatient(pat).error == NotFound);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Add a new patient");
    pat.stat_ok = true;
    pat.person = 0;
    pat.id = invalid_ident;
    ASSERT(db->setPatient(pat).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Modify the new patient");
    pat.stat_ok = false;
    ASSERT(db->setPatient(pat).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Get the new patient");
    DBPatient pat2;
    MULTI();
    ASSERT(db->getPatient(pat.id, pat2).error == NoError);
    VASSERT(pat2.id, pat.id);
    ASSERT(!pat2.stat_ok);
    VASSERT(pat2.person, 0);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Delete the new patient");
    MULTI();
    ASSERT(db->deletePatient(pat2.id).error == NoError);
    ASSERT(db->getPatient(pat2.id, pat).error == NotFound);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Get the patient list");
    QList<DBPatient> list;
    MULTI();
    ASSERT(db->getPatientsList(list).error == NoError);
    VASSERT(list.size(), 2);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();


    // Practician tests
    MESSAGE("--> Practicians:");

    TEST("Get a test practician");
    DBPractician pract;
    db->getPractician(0, pract);
    MULTI();
    VASSERT(pract.id, 0);
    VASSERT(pract.title, "Dr");
    VASSERT(pract.person, 2);
    VASSERT(pract.institute, 0);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Add a new practician");
    pract.title = "A";
    pract.person = 0;
    pract.institute = 0;
    pract.id = invalid_ident;
    ASSERT(db->setPractician(pract).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Modify the new practician");
    pract.title = "B";
    ASSERT(db->setPractician(pract).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Get the new practician");
    DBPractician pract2;
    MULTI();
    ASSERT(db->getPractician(pract.id, pract2).error == NoError);
    VASSERT(pract2.id, pract.id);
    VASSERT(pract2.title, pract.title);
    VASSERT(pract2.institute, pract.institute);
    VASSERT(pract2.person, pract.person);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Delete the new practician");
    MULTI();
    ASSERT(db->deletePractician(pract2.id).error == NoError);
    ASSERT(db->getPractician(pract2.id, pract).error == NotFound);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Get the practician list");
    QList<DBPractician> plist;
    MULTI();
    ASSERT(db->getPracticiansList(plist).error == NoError);
    VASSERT(plist.size(), 2);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();


    // Measures and drugs tests
    MESSAGE("--> Measures & drugs:");
    
    TEST("Add a measure");
    QDateTime moment(QDateTime(QDate(2012,1,1),QTime(8,0)));
    Measure m(moment, Amount("1000 [mg/l]"), "tato");
    MULTI();
    ASSERT(db->setMeasure(0, m).error == NoError);
    ASSERT(m.dbid != invalid_ident);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("List measures by date");
    QList<Measure> mlist;
    MULTI();
    ASSERT(db->getMeasuresByDate(0, mlist).error == NoError);
    VASSERT(mlist.size(), 2);
    VASSERT(mlist.at(1).amount.value, 1000.0);
    VASSERT(mlist.at(1).amount.unit.name(), "mg/l");
    VASSERT(mlist.at(1).dbid, m.dbid);
    VASSERT(mlist.at(1).drug, "tato");
    VASSERT(mlist.at(1).moment.toString(), moment.toString());
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("List measures by drug");
    mlist.clear();
    MULTI();
    ASSERT(db->getMeasuresByDrug(0, "tato", mlist).error == NoError);
    VASSERT(mlist.size(), 1);
    VASSERT(mlist.at(0).amount.value, 1000.0);
    VASSERT(mlist.at(0).amount.unit.name(), "mg/l");
    VASSERT(mlist.at(0).dbid, m.dbid);
    VASSERT(mlist.at(0).drug, "tato");
    VASSERT(mlist.at(0).moment.toString(), moment.toString());
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Update a measure");
    mlist.clear();
    m.amount.value = 200.0;
    m.drug = "dalafgan";
    MULTI();
    ASSERT(db->setMeasure(0, m).error == NoError);
    db->getMeasuresByDate(0, mlist);
    VASSERT(mlist.size(),2);
    VASSERT(mlist.at(1).amount.value, 200.0);
    VASSERT(mlist.at(1).amount.unit.name(), "mg/l");
    VASSERT(mlist.at(1).dbid, m.dbid);
    VASSERT(mlist.at(1).drug, "dalafgan");
    VASSERT(mlist.at(1).moment.toString(),moment.toString());
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Delete a measure");
    mlist.clear();
    MULTI();
    ASSERT(db->deleteMeasure(m.dbid).error == NoError);
    ASSERT(db->getMeasuresByDate(0, mlist).error == NoError);
    VASSERT(mlist.size(), 1);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    

    // Covariates tests
    MESSAGE("--> Covariates:");

    TEST("Add a covariate");
    Covariate c("c1", moment, 100.0);
    MULTI();
    ASSERT(db->setCovariate(-1,c).error == NoError);
    ASSERT(c.dbid() != invalid_ident);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("List covariates");
    QList<Covariate> clist;
    MULTI();
    ASSERT(db->getCovariatesByDate(-1,clist).error == NoError);
    VASSERT(clist.size(),1);
    VASSERT(clist.at(0).value(),100.0);
    VASSERT(clist.at(0).dbid(),c.dbid());
    VASSERT(clist.at(0).id(),"c1");
    VASSERT(clist.at(0).date().toString(),moment.toString());
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Update a covariate");
    clist.clear();
    c.setValue(200.0);
    c.setId("c2");
    MULTI();
    ASSERT(db->setCovariate(-1,c).error == NoError);
    db->getCovariatesByDate(-1,clist);
    VASSERT(clist.size(),1);
    VASSERT(clist.at(0).value(),200.0);
    VASSERT(clist.at(0).dbid(),c.dbid());
    VASSERT(clist.at(0).id(),"c2");
    VASSERT(clist.at(0).date().toString(),moment.toString());
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();

    TEST("Delete a covariate");
    MULTI();
    ASSERT(db->deleteCovariate(c.dbid()).error == NoError);
    ASSERT(db->getCovariatesByDate(-1,clist).error == NoError);
    VASSERT(clist.size(),0);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    

    // Curves tests
    MESSAGE("--> Curves:");
    
    TEST("Add a curve");
    ident cid;
    ASSERT(db->addCurve("test", "tato", "pe1", "po1", "re1", "m1", "comment1", moment, MANUAL, -1, cid).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Get the curve");
    QString name,drug,model,pe,po,re,comment;
    ParameterType ptype;
    QDateTime date;
    ident pid;
    MULTI();
    ASSERT(db->getCurve(cid, name, drug, pe, po, re, model, comment, date, ptype, pid).error == NoError);
    VASSERT(name, "test");
    VASSERT(drug, "tato");
    VASSERT(pe, "pe1");
    VASSERT(po, "po1");
    VASSERT(re, "re1");
    VASSERT(model, "m1");
    VASSERT(comment, "comment1");
    VASSERT(date.toString(), moment.toString());
    VASSERT(ptype,MANUAL);
    VASSERT(pid,-1);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Set the curve engines");
    ASSERT(db->setCurveEngines(cid, "pe2", "po2", "re2").error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Set the curve comment");
    ASSERT(db->setCurveComment(cid, "comment2").error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Set the curve name");
    ASSERT(db->setCurveName(cid, "test2").error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Set the curve parameter type");
    ASSERT(db->setCurvePType(cid,APOSTERIORI).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Set the curve first take");
    ASSERT(db->setCurveFirstTake(cid,date.addDays(1)).error == NoError);
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Get the curve's new values");
    MULTI();
    ASSERT(db->getCurve(cid,name,drug,pe,po,re,model,comment,date,ptype,pid).error == NoError);
    VASSERT(name, "test2");
    VASSERT(drug, "tato");
    VASSERT(pe, "pe2");
    VASSERT(po, "po2");
    VASSERT(re, "re2");
    VASSERT(model, "m1");
    VASSERT(comment, "comment2");
    VASSERT(date.toString(), moment.addDays(1).toString());
    VASSERT(ptype,APOSTERIORI);
    VASSERT(pid,-1);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Get the curve list");
    QList<ident> ids;
    QStringList names;
    MULTI();
    ASSERT(db->getCurves(ids, names).error == NoError);
    VASSERT(ids.size(), 1);
    VASSERT(names.size(), 1);
    VASSERT(ids.at(0),cid);
    VASSERT(names.at(0),"test2");
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    

    //Curve dosages tests
    MESSAGE("--> Curve dosages:");
    
    TEST("Add a curve dosage");
    Dosage d;
    d.applied = moment;
    d.dose = Amount("20 [mg]");
    d.interval = Duration(1);
    d.tinf = Duration(2);
    MULTI();
    ASSERT(db->setDosage(cid, d).error == NoError);
    ASSERT(d.dbid != invalid_ident);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("List dosages");
    QList<Dosage> dlist;
    MULTI();
    ASSERT(db->getDosagesByDate(cid, dlist).error == NoError);
    VASSERT(dlist.size(), 1);
    VASSERT(dlist.at(0).dose.value, 20.0);
    VASSERT(dlist.at(0).dose.unit.name(), "mg");
    VASSERT(dlist.at(0).dbid, d.dbid);
    VASSERT(dlist.at(0).interval.toString(), Duration(1).toString());
    VASSERT(dlist.at(0).tinf.toString(), Duration(2).toString());
    VASSERT(dlist.at(0).applied.toString(), moment.toString());
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Delete a curve dosage");
    MULTI();
    ASSERT(db->deleteDosage(d.dbid).error == NoError);
    ASSERT(db->getDosagesByDate(cid, dlist).error == NoError);
    VASSERT(dlist.size(),0);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    TEST("Delete the curve");
    MULTI();
    d.dbid = invalid_ident;
    ASSERT(db->setDosage(cid, d).error == NoError);
    ASSERT(d.dbid != invalid_ident);
    dlist.clear();
    ASSERT(db->getDosagesByDate(cid, dlist).error == NoError);
    VASSERT(dlist.size(), 1);
    ASSERT(db->deleteCurve(cid).error == NoError);
    ASSERT(db->getCurves(ids,names).error == NoError);
    VASSERT(ids.size(),0);
    VASSERT(names.size(),0);
    dlist.clear();
    ASSERT(db->getDosagesByDate(cid, dlist).error == NoError);
    VASSERT(dlist.size(), 0);
    ENDMULTI();
    if (!LAST_ASSERT)
        qDebug() << "Error : " + db->errorString();
    
    //Speed part
    SPEEDTEST
    
    ident first;

    TEST("100 patient creation");
    for (int i=0; i<100; i++) {
        pat.id = invalid_ident;
        db->setPatient(pat);
        if (i==0)
            first = pat.id;
    }
    PASS;
    
    TEST("100K patient reading");
    for (int i=0; i<100000; i++)
        db->getPatient((i%100)+first,pat);
    PASS;
    
    TEST("100 patient deleting");
    for (int i=0; i<100; i++)
        db->deletePatient(i+first);
    PASS;
    
    TEST("10 curve creation");
    for (int i=0; i<10; i++) {
        db->addCurve("test","tato","pe1","po1","re1","m1","comment1",moment,MANUAL,-1,cid);
        if (i==0)
            first = cid;
    }
    PASS;
    
    TEST("10K curve reading");
    for (int i=0; i<100000; i++)
        db->getCurve((i%10)+first,name,drug,pe,po,re,model,comment,date,ptype,pid);
    PASS;
    
    TEST("10 curve deleting");
    for (int i=0; i<10; i++)
        db->deleteCurve(i+first);
    PASS;

    ENDSPEEDTEST
    
    return TESTING_OK;
}
