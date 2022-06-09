//@@license@@

#ifndef STDADMINREPOSITORY_H
#define STDADMINREPOSITORY_H

#include "admin/src/dal/location.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/institute.h"
#include "admin/src/dal/phone.h"
#include "admin/src/dal/email.h"
#include "admin/src/dal/measure.h"
#include "admin/src/dal/clinical.h"
#include "admin/src/dal/partialrequest.h"
#include "core/utils/errorenums.h"
#include "core/core.h"
#include "core/dal/prediction.h"
#include "core/dal/covariate.h"
#include "core/interfaces/abstractrepository.h"


#define ADMINREPO (dynamic_cast<StdAdminDbInterface*>(ABSTRACTREPO))


class StdAdminDbInterface : public ezechiel::GuiCore::AbstractRepository
{
public:

    virtual bool tryRequest(const ezechiel::GuiCore::Response&, const QString&, QString&) = 0;

    virtual ezechiel::GuiCore::Response getPatientFromId(const int &id, SharedPatient &patient) = 0;
//    virtual ezechiel::GuiCore::Response getPatient (const QString &externalId, SharedPatient &patient) = 0;
    virtual ezechiel::GuiCore::Response getCorePatient (const int &id, ezechiel::GuiCore::CorePatient* corepatient) = 0;
    virtual ezechiel::GuiCore::Response setPatient (SharedPatient &patient) = 0;
    virtual ezechiel::GuiCore::Response deletePatient (SharedPatient &id) = 0;
    virtual ezechiel::GuiCore::Response getPatientsList (QList<SharedPatient> &list) = 0;

    virtual ezechiel::GuiCore::Response getLocationFromId(const int &id, Location* location) = 0;
    virtual ezechiel::GuiCore::Response setLocation (Location* location) = 0;
    virtual ezechiel::GuiCore::Response deleteLocation (const int &id) = 0;
    virtual ezechiel::GuiCore::Response getLocationsList(QList<Location*> &locations) = 0;

    virtual ezechiel::GuiCore::Response getPracticianFromId(const int &id, SharedPractician &practician) = 0;
//    virtual ezechiel::GuiCore::Response getPractician (const QString &externalId, SharedPractician &practician) = 0;
    virtual ezechiel::GuiCore::Response setPractician (SharedPractician &practician) = 0;
    virtual ezechiel::GuiCore::Response deletePractician (const int &id) = 0;
    virtual ezechiel::GuiCore::Response getPracticiansList (QList<SharedPractician> &list) = 0;

    virtual ezechiel::GuiCore::Response getInstituteFromId(const int &id, SharedInstitute &institute) = 0;
//    virtual ezechiel::GuiCore::Response getInstitute (const QString &externalId, SharedInstitute &institute) = 0;
    virtual ezechiel::GuiCore::Response setInstitute (SharedInstitute &institute) = 0;
    virtual ezechiel::GuiCore::Response deleteInstitute (const int &id) = 0;
    virtual ezechiel::GuiCore::Response getInstitutesList (QList<SharedInstitute> &list) = 0;

//    virtual ezechiel::GuiCore::Response getPerson (const int &id, SharedPerson &person) = 0;
//    virtual ezechiel::GuiCore::Response setPerson (SharedPerson &person) = 0;
//    virtual ezechiel::GuiCore::Response deletePerson (const int &id) = 0;
//    virtual ezechiel::GuiCore::Response getPersonsList(QList<SharedPerson> &persons) = 0;

//    virtual ezechiel::GuiCore::Response getPredictionFromId(int, SharedPrediction &) = 0;
//    virtual ezechiel::GuiCore::Response setPrediction(SharedPrediction &) = 0;
//    virtual ezechiel::GuiCore::Response getPredictionsList(QList<SharedPrediction>&) = 0;
//    virtual ezechiel::GuiCore::Response getPredictionsList(QList<int>&, QStringList&) = 0;
//    virtual ezechiel::GuiCore::Response getPredictionsList(QList<int>&, const ident&, const QString&) = 0;
//    virtual ezechiel::GuiCore::Response getPredictionsList(QList<SharedPrediction>&, const ident&, const QString&) = 0;
//    virtual ezechiel::GuiCore::Response deletePrediction(SharedPrediction&) = 0;
//    virtual ezechiel::GuiCore::Response deletePrediction(int) = 0;

    virtual ezechiel::GuiCore::Response getMeasureFromId(const int&, Measure * ) = 0;
    virtual ezechiel::GuiCore::Response setMeasure(Measure * ) = 0;
    virtual ezechiel::GuiCore::Response setMeasure(const int&, Measure * ) = 0;
    virtual ezechiel::GuiCore::Response getMeasuresList(QList<Measure *>&) = 0;
    virtual ezechiel::GuiCore::Response deleteMeasure(Measure *) = 0;
    virtual ezechiel::GuiCore::Response deleteMeasure(const int&) = 0;
    virtual ezechiel::GuiCore::Response getMeasuresFromPatientAndDrug(const int&, const QString&, QList<Measure *>&) = 0;
    virtual ezechiel::GuiCore::Response getMeasuresFromPatientAndDrug(const int&, const int&, QList<Measure *>&) = 0;
    virtual ezechiel::GuiCore::Response getMeasuresByDate(const int&, QList<Measure *>&) = 0;

    virtual ezechiel::GuiCore::Response getPatientVariateFromId(const int&, ezechiel::GuiCore::PatientVariate*) = 0;
    virtual ezechiel::GuiCore::Response setPatientVariate(ezechiel::GuiCore::PatientVariate*) = 0;
    virtual ezechiel::GuiCore::Response setPatientVariate(const int&, ezechiel::GuiCore::PatientVariate*) = 0;
    virtual ezechiel::GuiCore::Response getPatientVariatesofPatient(const int&, QList<ezechiel::GuiCore::PatientVariate*>&) = 0;
    virtual ezechiel::GuiCore::Response getPatientVariatesList(QList<ezechiel::GuiCore::PatientVariate*>&) = 0;
    virtual ezechiel::GuiCore::Response getPatientVariatesByDate(const int&, QList<ezechiel::GuiCore::PatientVariate*>&) = 0;
    virtual ezechiel::GuiCore::Response getPatientVariatesOfNames(const int&, QStringList&, QList<ezechiel::GuiCore::PatientVariate*>&) = 0;
    virtual ezechiel::GuiCore::Response deletePatientVariate(ezechiel::GuiCore::PatientVariate*) = 0;
    virtual ezechiel::GuiCore::Response deletePatientVariate(const int&) = 0;

    virtual ezechiel::GuiCore::Response getClinicalFromId(const int&, Clinical*) = 0;
    virtual ezechiel::GuiCore::Response setClinical(Clinical*) = 0;
    virtual ezechiel::GuiCore::Response getClinicalsList(QList<Clinical*>&) = 0;
    virtual ezechiel::GuiCore::Response getClinicalsByDate(const int&, QList<Clinical*>&) = 0;
    virtual ezechiel::GuiCore::Response deleteClinical(Clinical*) = 0;
    virtual ezechiel::GuiCore::Response deleteClinical(const int&) = 0;

    virtual ezechiel::GuiCore::Response getPartialRequestFromId(const int&, PartialRequest*) = 0;
    virtual ezechiel::GuiCore::Response getPartialRequestFromRequestId(QString&, PartialRequest*&) = 0;
    virtual ezechiel::GuiCore::Response setPartialRequest(PartialRequest*) = 0;
    virtual ezechiel::GuiCore::Response getPartialRequestsList(QList<PartialRequest*>&) = 0;
    virtual ezechiel::GuiCore::Response getPartialRequestsByDate(const int&, QList<PartialRequest*>&) = 0;
    virtual ezechiel::GuiCore::Response deletePartialRequest(PartialRequest*) = 0;
    virtual ezechiel::GuiCore::Response deletePartialRequest(const int&) = 0;
};

Q_DECLARE_INTERFACE(StdAdminDbInterface, "ch.heig-vd.ezechiel.StdAdminDbInterface/0.1")

#endif // STDADMINREPOSITORY_H
