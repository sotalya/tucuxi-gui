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


class StdAdminDbInterface : public ezechiel::core::AbstractRepository
{
public:

    virtual bool tryRequest(const ezechiel::core::Response&, const QString&, QString&) = 0;

    virtual ezechiel::core::Response getPatientFromId(const int &id, SharedPatient &patient) = 0;
//    virtual ezechiel::core::Response getPatient (const QString &externalId, SharedPatient &patient) = 0;
    virtual ezechiel::core::Response getCorePatient (const int &id, ezechiel::core::CorePatient* corepatient) = 0;
    virtual ezechiel::core::Response setPatient (SharedPatient &patient) = 0;
    virtual ezechiel::core::Response deletePatient (SharedPatient &id) = 0;
    virtual ezechiel::core::Response getPatientsList (QList<SharedPatient> &list) = 0;

    virtual ezechiel::core::Response getLocationFromId(const int &id, Location* location) = 0;
    virtual ezechiel::core::Response setLocation (Location* location) = 0;
    virtual ezechiel::core::Response deleteLocation (const int &id) = 0;
    virtual ezechiel::core::Response getLocationsList(QList<Location*> &locations) = 0;

    virtual ezechiel::core::Response getPracticianFromId(const int &id, SharedPractician &practician) = 0;
//    virtual ezechiel::core::Response getPractician (const QString &externalId, SharedPractician &practician) = 0;
    virtual ezechiel::core::Response setPractician (SharedPractician &practician) = 0;
    virtual ezechiel::core::Response deletePractician (const int &id) = 0;
    virtual ezechiel::core::Response getPracticiansList (QList<SharedPractician> &list) = 0;

    virtual ezechiel::core::Response getInstituteFromId(const int &id, SharedInstitute &institute) = 0;
//    virtual ezechiel::core::Response getInstitute (const QString &externalId, SharedInstitute &institute) = 0;
    virtual ezechiel::core::Response setInstitute (SharedInstitute &institute) = 0;
    virtual ezechiel::core::Response deleteInstitute (const int &id) = 0;
    virtual ezechiel::core::Response getInstitutesList (QList<SharedInstitute> &list) = 0;

//    virtual ezechiel::core::Response getPerson (const int &id, SharedPerson &person) = 0;
//    virtual ezechiel::core::Response setPerson (SharedPerson &person) = 0;
//    virtual ezechiel::core::Response deletePerson (const int &id) = 0;
//    virtual ezechiel::core::Response getPersonsList(QList<SharedPerson> &persons) = 0;

//    virtual ezechiel::core::Response getPredictionFromId(int, SharedPrediction &) = 0;
//    virtual ezechiel::core::Response setPrediction(SharedPrediction &) = 0;
//    virtual ezechiel::core::Response getPredictionsList(QList<SharedPrediction>&) = 0;
//    virtual ezechiel::core::Response getPredictionsList(QList<int>&, QStringList&) = 0;
//    virtual ezechiel::core::Response getPredictionsList(QList<int>&, const ident&, const QString&) = 0;
//    virtual ezechiel::core::Response getPredictionsList(QList<SharedPrediction>&, const ident&, const QString&) = 0;
//    virtual ezechiel::core::Response deletePrediction(SharedPrediction&) = 0;
//    virtual ezechiel::core::Response deletePrediction(int) = 0;

    virtual ezechiel::core::Response getMeasureFromId(const int&, Measure * ) = 0;
    virtual ezechiel::core::Response setMeasure(Measure * ) = 0;
    virtual ezechiel::core::Response setMeasure(const int&, Measure * ) = 0;
    virtual ezechiel::core::Response getMeasuresList(QList<Measure *>&) = 0;
    virtual ezechiel::core::Response deleteMeasure(Measure *) = 0;
    virtual ezechiel::core::Response deleteMeasure(const int&) = 0;
    virtual ezechiel::core::Response getMeasuresFromPatientAndDrug(const int&, const QString&, QList<Measure *>&) = 0;
    virtual ezechiel::core::Response getMeasuresFromPatientAndDrug(const int&, const int&, QList<Measure *>&) = 0;
    virtual ezechiel::core::Response getMeasuresByDate(const int&, QList<Measure *>&) = 0;

    virtual ezechiel::core::Response getPatientVariateFromId(const int&, ezechiel::core::PatientVariate*) = 0;
    virtual ezechiel::core::Response setPatientVariate(ezechiel::core::PatientVariate*) = 0;
    virtual ezechiel::core::Response setPatientVariate(const int&, ezechiel::core::PatientVariate*) = 0;
    virtual ezechiel::core::Response getPatientVariatesofPatient(const int&, QList<ezechiel::core::PatientVariate*>&) = 0;
    virtual ezechiel::core::Response getPatientVariatesList(QList<ezechiel::core::PatientVariate*>&) = 0;
    virtual ezechiel::core::Response getPatientVariatesByDate(const int&, QList<ezechiel::core::PatientVariate*>&) = 0;
    virtual ezechiel::core::Response getPatientVariatesOfNames(const int&, QStringList&, QList<ezechiel::core::PatientVariate*>&) = 0;
    virtual ezechiel::core::Response deletePatientVariate(ezechiel::core::PatientVariate*) = 0;
    virtual ezechiel::core::Response deletePatientVariate(const int&) = 0;

    virtual ezechiel::core::Response getClinicalFromId(const int&, Clinical*) = 0;
    virtual ezechiel::core::Response setClinical(Clinical*) = 0;
    virtual ezechiel::core::Response getClinicalsList(QList<Clinical*>&) = 0;
    virtual ezechiel::core::Response getClinicalsByDate(const int&, QList<Clinical*>&) = 0;
    virtual ezechiel::core::Response deleteClinical(Clinical*) = 0;
    virtual ezechiel::core::Response deleteClinical(const int&) = 0;

    virtual ezechiel::core::Response getPartialRequestFromId(const int&, PartialRequest*) = 0;
    virtual ezechiel::core::Response getPartialRequestFromRequestId(QString&, PartialRequest*&) = 0;
    virtual ezechiel::core::Response setPartialRequest(PartialRequest*) = 0;
    virtual ezechiel::core::Response getPartialRequestsList(QList<PartialRequest*>&) = 0;
    virtual ezechiel::core::Response getPartialRequestsByDate(const int&, QList<PartialRequest*>&) = 0;
    virtual ezechiel::core::Response deletePartialRequest(PartialRequest*) = 0;
    virtual ezechiel::core::Response deletePartialRequest(const int&) = 0;
};

Q_DECLARE_INTERFACE(StdAdminDbInterface, "ch.heig-vd.ezechiel.StdAdminDbInterface/0.1")

#endif // STDADMINREPOSITORY_H
