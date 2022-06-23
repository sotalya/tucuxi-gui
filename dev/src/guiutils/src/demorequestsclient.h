//@@license@@

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
