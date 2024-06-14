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

namespace Tucuxi {
namespace Gui {
namespace Admin {

class StdAdminDbInterface : public Tucuxi::Gui::Core::AbstractRepository
{
public:

    virtual bool tryRequest(const Tucuxi::Gui::Core::Response&, const QString&, QString&) = 0;

    virtual Tucuxi::Gui::Core::Response getPatientFromId(const int &id, SharedPatient &patient) = 0;
//    virtual Tucuxi::Gui::Core::Response getPatient (const QString &externalId, SharedPatient &patient) = 0;
    virtual Tucuxi::Gui::Core::Response getCorePatient (const int &id, Tucuxi::Gui::Core::CorePatient* corepatient) = 0;
    virtual Tucuxi::Gui::Core::Response setPatient (SharedPatient &patient) = 0;
    virtual Tucuxi::Gui::Core::Response deletePatient (SharedPatient &id) = 0;
    virtual Tucuxi::Gui::Core::Response getPatientsList (QList<SharedPatient> &list) = 0;

    virtual Tucuxi::Gui::Core::Response getLocationFromId(const int &id, Location* location) = 0;
    virtual Tucuxi::Gui::Core::Response setLocation (Location* location) = 0;
    virtual Tucuxi::Gui::Core::Response deleteLocation (const int &id) = 0;
    virtual Tucuxi::Gui::Core::Response getLocationsList(QList<Location*> &locations) = 0;

    virtual Tucuxi::Gui::Core::Response getPracticianFromId(const int &id, SharedPractician &practician) = 0;
//    virtual Tucuxi::Gui::Core::Response getPractician (const QString &externalId, SharedPractician &practician) = 0;
    virtual Tucuxi::Gui::Core::Response setPractician (SharedPractician &practician) = 0;
    virtual Tucuxi::Gui::Core::Response deletePractician (const int &id) = 0;
    virtual Tucuxi::Gui::Core::Response getPracticiansList (QList<SharedPractician> &list) = 0;

    virtual Tucuxi::Gui::Core::Response getInstituteFromId(const int &id, SharedInstitute &institute) = 0;
//    virtual Tucuxi::Gui::Core::Response getInstitute (const QString &externalId, SharedInstitute &institute) = 0;
    virtual Tucuxi::Gui::Core::Response setInstitute (SharedInstitute &institute) = 0;
    virtual Tucuxi::Gui::Core::Response deleteInstitute (const int &id) = 0;
    virtual Tucuxi::Gui::Core::Response getInstitutesList (QList<SharedInstitute> &list) = 0;

//    virtual Tucuxi::Gui::Core::Response getPerson (const int &id, SharedPerson &person) = 0;
//    virtual Tucuxi::Gui::Core::Response setPerson (SharedPerson &person) = 0;
//    virtual Tucuxi::Gui::Core::Response deletePerson (const int &id) = 0;
//    virtual Tucuxi::Gui::Core::Response getPersonsList(QList<SharedPerson> &persons) = 0;

//    virtual Tucuxi::Gui::Core::Response getPredictionFromId(int, SharedPrediction &) = 0;
//    virtual Tucuxi::Gui::Core::Response setPrediction(SharedPrediction &) = 0;
//    virtual Tucuxi::Gui::Core::Response getPredictionsList(QList<SharedPrediction>&) = 0;
//    virtual Tucuxi::Gui::Core::Response getPredictionsList(QList<int>&, QStringList&) = 0;
//    virtual Tucuxi::Gui::Core::Response getPredictionsList(QList<int>&, const ident&, const QString&) = 0;
//    virtual Tucuxi::Gui::Core::Response getPredictionsList(QList<SharedPrediction>&, const ident&, const QString&) = 0;
//    virtual Tucuxi::Gui::Core::Response deletePrediction(SharedPrediction&) = 0;
//    virtual Tucuxi::Gui::Core::Response deletePrediction(int) = 0;

    virtual Tucuxi::Gui::Core::Response getMeasureFromId(const int&, Measure * ) = 0;
    virtual Tucuxi::Gui::Core::Response setMeasure(Measure * ) = 0;
    virtual Tucuxi::Gui::Core::Response setMeasure(const int&, Measure * ) = 0;
    virtual Tucuxi::Gui::Core::Response getMeasuresList(QList<Measure *>&) = 0;
    virtual Tucuxi::Gui::Core::Response deleteMeasure(Measure *) = 0;
    virtual Tucuxi::Gui::Core::Response deleteMeasure(const int&) = 0;
    virtual Tucuxi::Gui::Core::Response getMeasuresFromPatientAndDrug(const int&, const QString&, QList<Measure *>&) = 0;
    virtual Tucuxi::Gui::Core::Response getMeasuresFromPatientAndDrug(const int&, const int&, QList<Measure *>&) = 0;
    virtual Tucuxi::Gui::Core::Response getMeasuresByDate(const int&, QList<Measure *>&) = 0;

    virtual Tucuxi::Gui::Core::Response getPatientVariateFromId(const int&, Tucuxi::Gui::Core::PatientVariate*) = 0;
    virtual Tucuxi::Gui::Core::Response setPatientVariate(Tucuxi::Gui::Core::PatientVariate*) = 0;
    virtual Tucuxi::Gui::Core::Response setPatientVariate(const int&, Tucuxi::Gui::Core::PatientVariate*) = 0;
    virtual Tucuxi::Gui::Core::Response getPatientVariatesofPatient(const int&, QList<Tucuxi::Gui::Core::PatientVariate*>&) = 0;
    virtual Tucuxi::Gui::Core::Response getPatientVariatesList(QList<Tucuxi::Gui::Core::PatientVariate*>&) = 0;
    virtual Tucuxi::Gui::Core::Response getPatientVariatesByDate(const int&, QList<Tucuxi::Gui::Core::PatientVariate*>&) = 0;
    virtual Tucuxi::Gui::Core::Response getPatientVariatesOfNames(const int&, QStringList&, QList<Tucuxi::Gui::Core::PatientVariate*>&) = 0;
    virtual Tucuxi::Gui::Core::Response deletePatientVariate(Tucuxi::Gui::Core::PatientVariate*) = 0;
    virtual Tucuxi::Gui::Core::Response deletePatientVariate(const int&) = 0;

    virtual Tucuxi::Gui::Core::Response getClinicalFromId(const int&, Clinical*) = 0;
    virtual Tucuxi::Gui::Core::Response setClinical(Clinical*) = 0;
    virtual Tucuxi::Gui::Core::Response getClinicalsList(QList<Clinical*>&) = 0;
    virtual Tucuxi::Gui::Core::Response getClinicalsByDate(const int&, QList<Clinical*>&) = 0;
    virtual Tucuxi::Gui::Core::Response deleteClinical(Clinical*) = 0;
    virtual Tucuxi::Gui::Core::Response deleteClinical(const int&) = 0;

    virtual Tucuxi::Gui::Core::Response getPartialRequestFromId(const int&, PartialRequest*) = 0;
    virtual Tucuxi::Gui::Core::Response getPartialRequestFromRequestId(QString&, PartialRequest*&) = 0;
    virtual Tucuxi::Gui::Core::Response setPartialRequest(PartialRequest*) = 0;
    virtual Tucuxi::Gui::Core::Response getPartialRequestsList(QList<PartialRequest*>&) = 0;
    virtual Tucuxi::Gui::Core::Response getPartialRequestsByDate(const int&, QList<PartialRequest*>&) = 0;
    virtual Tucuxi::Gui::Core::Response deletePartialRequest(PartialRequest*) = 0;
    virtual Tucuxi::Gui::Core::Response deletePartialRequest(const int&) = 0;
};

}
}
}

Q_DECLARE_INTERFACE(Tucuxi::Gui::Admin::StdAdminDbInterface, "ch.heig-vd.ezechiel.StdAdminDbInterface/0.1")

#endif // STDADMINREPOSITORY_H
