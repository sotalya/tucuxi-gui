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


#ifndef DemoRequestsClient_H
#define DemoRequestsClient_H

#include "admin/src/requestsclient.h"



#include "core/dal/drug/drug.h"
#include "core/dal/dosage.h"
#include "core/dal/covariate.h"
#include "core/dal/prediction.h"

#include "admin/src/dal/patient.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/institute.h"
#include "admin/src/dal/measure.h"
#include "admin/src/dal/clinical.h"
#include "admin/src/dal/phone.h"
#include "admin/src/dal/email.h"
#include "admin/src/dal/interpretationrequest.h"

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class DemoInterpretationRequestBuilder
{

public:
    explicit DemoInterpretationRequestBuilder(const QString &requestId);
    ~DemoInterpretationRequestBuilder();

    Tucuxi::Gui::Admin::InterpretationRequest* buildInterpretationRequest(const QString &patientId, const QString &drugId);

private:
    QString buildDrug(const QString &rootKey);
    Tucuxi::Gui::Core::DosageHistory* buildDosages(const QString &rootKey);

    Tucuxi::Gui::Core::PatientVariateList * buildCovariates(const QString &rootKey, const SharedPatient patient);
    QList<Tucuxi::Gui::Admin::Clinical*> buildClinical(const QString &rootKey, const SharedPatient patient);

    SharedPatient buildPatient(const QString &rootKey);
    SharedPractician buildPractician(const QString &rootKey);
    SharedInstitute buildInstitute(const QString &rootKey);

    Tucuxi::Gui::Core::CoreMeasureList * buildSamples(const QString &rootKey, const SharedPatient patient, const QString &drug);
    QList<SharedPhone> buildPhones(const QString &rootKey);
    Tucuxi::Gui::Admin::PhoneList* buildPhoneList(const QString &rootKey);
    QList<Tucuxi::Gui::Admin::Email*> buildEmails(const QString &rootKey);

    Tucuxi::Gui::Core::Admin::Route toDosageRoute(const QString &route);

    Tucuxi::Gui::Admin::PhoneType toPhoneType(const QString &type);
    Tucuxi::Gui::Admin::Type toEmailType(const QString &type);

    QString requestId;
};


class DemoRequestsClient : public Tucuxi::Gui::Admin::RequestsClient
{
    Q_OBJECT

    public:
    explicit DemoRequestsClient(QObject *parent = nullptr);
    virtual ~DemoRequestsClient() Q_DECL_OVERRIDE;

    public slots:
    virtual void queryList(QDateTime from = QDateTime::currentDateTime().addYears(-10),
                           QDateTime to = QDateTime::currentDateTime().addYears(10),
                           bool state = true) Q_DECL_OVERRIDE;
    virtual void queryRequest(const QString &requestId, const QString &patientId, const QString &drugId) Q_DECL_OVERRIDE;


private:

};

}
}
}

#endif // DemoRequestsClient_H
