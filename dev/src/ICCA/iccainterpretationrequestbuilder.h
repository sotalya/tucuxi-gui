#ifndef ICCAINTERPRETATIONREQUESTBUILDER_H
#define ICCAINTERPRETATIONREQUESTBUILDER_H


#include <QDomDocument>

#include "core/dal/drug/drug.h"
#include "core/dal/covariate.h"
#include "core/dal/dosage.h"
#include "admin/src/dal/institute.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/clinical.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/measure.h"
#include "admin/src/dal/interpretationrequest.h"


namespace Tucuxi {
namespace Gui {
namespace ICCA {

class ICCAInterpretationRequestBuilder
{

public:
//    explicit InterpretationRequestBuilder(const MessageContent &content);
    explicit ICCAInterpretationRequestBuilder(const QDomDocument &content);
    ~ICCAInterpretationRequestBuilder();

    Tucuxi::Gui::Admin::InterpretationRequest* buildInterpretationRequest();

private:
    QString buildDrug(const QString &rootKey);
    Tucuxi::Gui::Core::DosageHistory* buildDosages(const QString &rootKey);

    Tucuxi::Gui::Core::PatientVariateList * buildCovariates(const QString &rootKey, const SharedPatient patient);
    Tucuxi::Gui::Admin::ClinicalSet* buildClinical(const QString &rootKey);
//    bool buildClinicals(const QString &rootKey, InterpretationRequest *InterpretationRequest);

    SharedPatient buildPatient(const QString &rootKey);
    SharedPractician buildPractician(const QString &rootKey);
    SharedInstitute buildInstitute(const QString &rootKey);

    Tucuxi::Gui::Core::CoreMeasureList* buildSamples(const QString &rootKey, const SharedPatient patient, const QString &activeSubstance);
    QList<SharedPhone> buildPhones(const QString &rootKey, const QString&);
    Tucuxi::Gui::Admin::PhoneList* buildPhoneList(const QString &rootKey, const QString&);
    QList<Tucuxi::Gui::Admin::Email*> buildEmails(const QString &rootKey, const QString&);

    Tucuxi::Gui::Admin::PhoneType toPhoneType(const QString &type);
    Tucuxi::Gui::Admin::Type toEmailType(const QString &type);

//    MessageContent content;
    QDomDocument content;
    QDomElement datasetNode;
};

}
}
}


#endif // ICCAINTERPRETATIONREQUESTBUILDER_H
