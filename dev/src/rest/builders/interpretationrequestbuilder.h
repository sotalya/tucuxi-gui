//@@license@@

#ifndef INTERPRETATIONREQUESTBUILDER_H
#define INTERPRETATIONREQUESTBUILDER_H

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


class InterpretationRequestBuilder
{

public:
//    explicit InterpretationRequestBuilder(const MessageContent &content);
    explicit InterpretationRequestBuilder(const QDomDocument &content);
    ~InterpretationRequestBuilder();

    InterpretationRequest* buildInterpretationRequest();

private:
    QString buildDrug(const QString &rootKey);
    ezechiel::GuiCore::DosageHistory* buildDosages(const QString &rootKey);

    ezechiel::GuiCore::PatientVariateList * buildCovariates(const QString &rootKey, const SharedPatient patient);
    ClinicalSet* buildClinical(const QString &rootKey);
//    bool buildClinicals(const QString &rootKey, InterpretationRequest *InterpretationRequest);

    SharedPatient buildPatient(const QString &rootKey);
    SharedPractician buildPractician(const QString &rootKey);
    SharedInstitute buildInstitute(const QString &rootKey);

    ezechiel::GuiCore::CoreMeasureList* buildSamples(const QString &rootKey, const SharedPatient patient, const QString &activeSubstance);
    QList<SharedPhone> buildPhones(const QString &rootKey, const QString&);
    PhoneList* buildPhoneList(const QString &rootKey, const QString&);
    QList<Email*> buildEmails(const QString &rootKey, const QString&);

    PhoneType toPhoneType(const QString &type);
    Type toEmailType(const QString &type);

//    MessageContent content;
    QDomDocument content;
    QDomElement datasetNode;
};

#endif // INTERPRETATIONBUILDER_H
