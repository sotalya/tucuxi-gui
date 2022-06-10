//@@license@@

#include "demorequestsclient.h"



#include "admin/src/dal/partialrequest.h"
#include "admin/src/adminfactory.h"
#include "admin/src/stdadminrepository.h"
#include "admin/src/dal/person.h"
#include "admin/src/dal/partialrequest.h"

#include "apputils/src/appcore.h"
#include "apputils/src/drugmanager.h"
#include "apputils/src/appcore.h"
#include "apputils/src/drugmanager.h"
#include "apputils/src/apputilsfactory.h"
#include "apputils/src/apputilsrepository.h"

#include "core/core.h"
#include "core/corefactory.h"
#include "core/utils/units.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/uncastedvalue.h"

#include "models/partialrequestlistmodel.h"
#include "models/sortfilterentitylistproxymodel.h"
#include "models/partialrequestlistmodel.h"
#include "models/sortfilterentitylistproxymodel.h"





using namespace Tucuxi::Gui::Core;

DemoRequestsClient::DemoRequestsClient(QObject *parent) : RequestsClient(parent)
{
}

DemoRequestsClient::~DemoRequestsClient()
{

}

void DemoRequestsClient::queryList(QDateTime from,
                                      QDateTime to,
                                      bool state)
{
    Q_UNUSED(from)
    Q_UNUSED(to)
    Q_UNUSED(state)

    QList<SharedPartialRequest> requests;

    //ToDo: use interpretation request builder instead


    for (int i = 0; i < 10; ++i) {

        SharedPartialRequest request = AdminFactory::createEntity<PartialRequest>(ABSTRACTREPO, this);

        //ToDo: complete the share request data
        //ToDo: uncomment the drug value below and remove imatinib

        //The request data
        request->requestId(QString("id %1").arg(i));
        request->requestCpcl("open");

        //The patient data
        Patient* patient = static_cast<Patient*>(request->patient());
        patient->externalId(QString("pid %1").arg(i));
        patient->stayNumber(QString("stay %1").arg(i));
        patient->person()->firstname(QString("firstname %1").arg(i));
        patient->person()->name(QString("last name %1").arg(i));
        QString string = "Tuesday, 23 April 12 22:51:41";
        QString format = "dddd, d MMMM yy hh:mm:ss";

        patient->person()->birthday(QDateTime::fromString(string, format).date());
        patient->person()->gender("male" == "male" ? Person::Male : Person::Female);

        //The patient institute
        request->institute()->externalId("patient.institute.id");
        request->institute()->name("patient.institute.name");

        //The sample data
        Measure* measure = static_cast<Measure*>(request->sample());
        measure->sampleID("sample.id");
        measure->setMoment(QDateTime::currentDateTime());
        measure->arrivalDate(QDateTime::currentDateTime());
        measure->setConcentration(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO, measure));

        if ((i % 2) == 1) {
            UncastedValue *uncasted = CoreFactory::createEntity<UncastedValue>(ABSTRACTREPO, measure->getUncastedValues());
            uncasted->setField("value");
            uncasted->setText(QString("100-0-200"));
            measure->getUncastedValues()->append(uncasted);
        }
        else
            measure->getConcentration()->setValue(100*i);

        if ((i % 4) == 1) {
            UncastedValue *uncasted = CoreFactory::createEntity<UncastedValue>(ABSTRACTREPO, measure->getUncastedValues());
            uncasted->setField("unit");
            uncasted->setText(QString("uugml"));
            measure->getUncastedValues()->append(uncasted);

        }
        else
            measure->getConcentration()->setUnit(Tucuxi::Gui::Core::Unit("mg/l"));

        //The drug data

        Tucuxi::Gui::Core::ActiveSubstance *substance;
        APPUTILSREPO->getActiveSubstanceOfId("imatinib", substance);
        request->drug(substance);
//        request->drug(APPCORE->drugManager()->drug(content.value("drug.id")));

        requests.append(request);
    }

    emit requestListReady(requests);
}

void DemoRequestsClient::queryRequest(const QString &requestId, const QString &patientId, const QString &drugId)
{

    DemoInterpretationRequestBuilder irBuilder(requestId);

    InterpretationRequest *interpretationRequest = irBuilder.buildInterpretationRequest(patientId, drugId);

    emit requestReady(interpretationRequest);

}

DemoInterpretationRequestBuilder::DemoInterpretationRequestBuilder(const QString &requestId) :
    requestId(requestId)
{

}

DemoInterpretationRequestBuilder::~DemoInterpretationRequestBuilder()
{

}

InterpretationRequest* DemoInterpretationRequestBuilder::buildInterpretationRequest(const QString &patientId, const QString &drugId)
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: set requestId and state when added to DAL object
    //ToDo: set the practician when added to DAL object
    //ToDo: set the clinicals when added to DAL object

    InterpretationRequest* interpretationRequest = CoreFactory::createEntity<InterpretationRequest>(ABSTRACTREPO);

    Tucuxi::Gui::Core::DrugTreatment *treatment;

    treatment = interpretationRequest->getTreatment();
    treatment->setParent(interpretationRequest);

    //Prediction patient
    treatment->setPatient(buildPatient("patient"));
    treatment->getPatient()->setParent(interpretationRequest);

    //Prediction drug
    treatment->setActiveSubstanceId(buildDrug("drug"));

    //Prediction dosage
    treatment->setDosages(buildDosages("dosages"));
    treatment->getDosages()->setParent(treatment);

    //Prediction samples
    CoreMeasureList * measures = buildSamples("samples", treatment->getPatient(), treatment->getActiveSubstanceId());
    treatment->setMeasures(measures);
    measures->setParent(treatment);

    //Prediction covariates
//    Tucuxi::Gui::Core::PatientVariateList * covariates = buildCovariates("covariates", treatment->getPatient());
//    treatment->setCovariates(covariates);

    return interpretationRequest;
}


QString DemoInterpretationRequestBuilder::buildDrug(const QString &rootKey)
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: not sure how apputils work, but shouldn't use the drug name to retrieve it
    //ToDo: uncomment the drug value below and remove imatinib

    //Tucuxi::Gui::Core::ActiveSubstance* drug;
//    APPUTILSREPO->getDrugOfName(content.value(rootKey + ".id").split('.').last(), drug);
    //APPUTILSREPO->getDrugOfName("Imatinib", drug);
    //APPUTILSREPO->getActiveSubstanceOfId("daptomycin" /*"Imatinib"*/, drug);

    return "imatinib";
}

Tucuxi::Gui::Core::DosageHistory* DemoInterpretationRequestBuilder::buildDosages(const QString &rootKey)
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: check how to deal with dosage end date

    Tucuxi::Gui::Core::DosageHistory* dosages = CoreFactory::createEntity<Tucuxi::Gui::Core::DosageHistory>(ABSTRACTREPO);

    for (int i = 0; i < 1; ++i) {
        Tucuxi::Gui::Core::Dosage* dosage = CoreFactory::createEntity<Tucuxi::Gui::Core::Dosage>(ABSTRACTREPO, dosages);

        //Dosage data
        dosage->getRoute()->setRoute(toDosageRoute("extra"));
//        dosage->setTaken(true);

        //Dosage date
        dosage->setApplied(QDateTime::currentDateTime());
        dosage->setEndTime(QDateTime::currentDateTime().addDays(5));

        //Dosage value
        dosage->getQuantity()->setValue(400);
        dosage->getQuantity()->setUnit(Unit("mg/l"));

        //Dosage interval
        dosage->setInterval(Duration(24));

        //Dosage infusion
        dosage->setTinf(Duration(1));

        dosages->append(dosage);
    }

    return dosages;
}

Tucuxi::Gui::Core::PatientVariateList * DemoInterpretationRequestBuilder::buildCovariates(const QString &rootKey, const SharedPatient patient)
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: set the covariate type based on the drug data
    //ToDo: set the covariate nature when added to DAL object

    Tucuxi::Gui::Core::PatientVariateList * covariates = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariateList>(ABSTRACTREPO, patient);

    for (int i = 0; i < 2; ++i) {
        Tucuxi::Gui::Core::PatientVariate* covariate = CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariate>(ABSTRACTREPO, patient);

        //Covariate data
        if (i == 0) {
            covariate->setCovariateId("weight");
            covariate->setDate(QDateTime::currentDateTime());
            covariate->getQuantity()->setValue(73);
            covariate->getQuantity()->setUnit(Unit("Kg"));
            covariate->setType(QMetaType::Int);
        }
        else {
            covariate->setCovariateId("height");
            covariate->setDate(QDateTime::currentDateTime());
            covariate->getQuantity()->setValue(175);
            covariate->getQuantity()->setUnit(Unit("cm"));
            covariate->setType(QMetaType::Int);
        }
        covariates->append(covariate);
    }

    return covariates;
}

QList<Clinical*> DemoInterpretationRequestBuilder::buildClinical(const QString &rootKey, const SharedPatient patient)
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: set the covariate type based on the drug data
    //ToDo: set the covariate nature when added to DAL object

    QList<Clinical*> clinicals;

    for (int i = 0; i < 1; ++i) {
        Clinical* clinical = AdminFactory::createEntity<Clinical>(ABSTRACTREPO, patient);

        //Covariate data
        clinical->setPatient_id(patient->id());
        clinical->setPatient(static_cast<Patient*>(patient));
        clinical->setName("clinicalname");
        clinical->setDate(QDateTime::currentDateTime());
        clinical->setValue("coucou les amis");

        clinicals.append(clinical);
    }

    return clinicals;
}

SharedPatient DemoInterpretationRequestBuilder::buildPatient(const QString &rootKey)
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: add the patient institute when added to DAL object

    SharedPatient shpatient = AdminFactory::createEntity<Patient>(ABSTRACTREPO);

    int i = 0;

    //Patient data
    Patient* patient = static_cast<Patient*>(shpatient);
    patient->externalId(QString("id %1").arg(i));
    patient->stayNumber(QString("id %1").arg(i));
    patient->person()->firstname(QString("id %1").arg(i));
    patient->person()->name(QString("id %1").arg(i));
    patient->person()->gender("male" == "male" ? Person::Male : Person::Female);
//    QString string = "Tuesday, 23 April 12 22:51:41";
//    QString format = "dddd, d MMMM yy hh:mm:ss";

    patient->person()->birthday(QDate::currentDate().addYears(-25));

    //Patient contact
    patient->person()->location()->address(QString("id %1").arg(i));
    patient->person()->location()->postcode(QString("id %1").arg(i));
    patient->person()->location()->city(QString("id %1").arg(i));
    patient->person()->location()->state(QString("id %1").arg(i));
    patient->person()->location()->country(QString("id %1").arg(i));

    //Patient phones
    patient->person()->setPhones(buildPhoneList(rootKey + ".contact.phones"));

    //Patient mails
    patient->person()->setEmails(buildEmails(rootKey + ".contact.emails"));

    return shpatient;
}

SharedPractician DemoInterpretationRequestBuilder::buildPractician(const QString &rootKey)
{
    //ToDo: check critical info and build, or store incomplete data

    SharedPractician practician = AdminFactory::createEntity<Practician>(ABSTRACTREPO);

    int i = 3;

    //Practician data
    practician->externalId(QString("id %1").arg(i));
    practician->title(QString("id %1").arg(i));
    practician->person()->firstname(QString("id %1").arg(i));
    practician->person()->name(QString("id %1").arg(i));
    practician->person()->gender("male" == "male" ? Person::Male : Person::Female);
    QString string = "Tuesday, 23 April 12 22:51:41";
    QString format = "dddd, d MMMM yy hh:mm:ss";

    practician->person()->birthday(QDateTime::fromString(string, format).date());

    //Practician contact
    practician->person()->location()->address(QString("id %1").arg(i));
    practician->person()->location()->postcode(QString("id %1").arg(i));
    practician->person()->location()->city(QString("id %1").arg(i));
    practician->person()->location()->state(QString("id %1").arg(i));
    practician->person()->location()->country(QString("id %1").arg(i));

    //Practician phones
    practician->person()->setPhones(buildPhoneList(rootKey + ".contact.phones"));

    //Practician mails
    practician->person()->setEmails(buildEmails(rootKey + ".contact.emails"));

    //Practician institute
    practician->institute(buildInstitute(rootKey + ".institute"));

    return practician;
}

SharedInstitute DemoInterpretationRequestBuilder::buildInstitute(const QString &rootKey)
{
    //ToDo: check critical info and build, or store incomplete data

    SharedInstitute institute = AdminFactory::createEntity<Institute>(ABSTRACTREPO);

    //Institute data
    institute->externalId("instituteId");
    institute->name("institute name");

    //Institute contact
    institute->location()->address("asdfasdf");
    institute->location()->postcode("asdfasdf");
    institute->location()->city("asdfasdf");
    institute->location()->state("asdfasdf");
    institute->location()->country("asdfasdf");

    //Institute phones
    institute->setPhones(buildPhones(rootKey + ".contact.phones"));

    //Institute mails
    institute->setEmails(buildEmails(rootKey + ".contact.emails"));

    return institute;
}

CoreMeasureList * DemoInterpretationRequestBuilder::buildSamples(const QString &rootKey, const SharedPatient patient, const QString & drug)
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: add multiple concentrations when DAL object supports it

    CoreMeasureList * samples = CoreFactory::createEntity<CoreMeasureList>(ABSTRACTREPO, patient);

    for (int i = 0; i < 1; ++i) {
        Measure * sample = AdminFactory::createEntity<Measure>(ABSTRACTREPO, samples);

        //Measure data
        sample->sampleID(QString("sample id %1").arg(i));
        //sample->patient_id(patient->id());
        //sample->patient(patient);
        sample->setSdrug(drug);
        //sample->setDrug(drug);

        //Measure dates
        sample->setMoment(QDateTime::currentDateTime().addDays(1).addSecs(8*3600));
        sample->arrivalDate(QDateTime::currentDateTime().addDays(1).addSecs(8*3600));

        //Measure value
        double value = 3;
        sample->getConcentration()->setValue(value);

        if (true) {
            UncastedValue *uncasted = CoreFactory::createEntity<UncastedValue>(ABSTRACTREPO, sample->getUncastedValues());
            uncasted->setField("value");
            uncasted->setText(QString("100-0-200"));
            sample->getUncastedValues()->append(uncasted);

            uncasted = CoreFactory::createEntity<UncastedValue>(ABSTRACTREPO, sample->getUncastedValues());
            uncasted->setField("unit");
            uncasted->setText(QString("uugml"));
            sample->getUncastedValues()->append(uncasted);
        }
        else {
            sample->getConcentration()->setValue(100*i);
            sample->getConcentration()->setUnit(Tucuxi::Gui::Core::Unit("mg/l"));
        }
        samples->append(sample);
    }

    return samples;
}

QList<SharedPhone> DemoInterpretationRequestBuilder::buildPhones(const QString &rootKey)
{
    QList<SharedPhone> phones;

    for (int i = 0; i < 2; ++i) {
        SharedPhone phone = AdminFactory::createEntity<Phone>(ABSTRACTREPO);
        phone->setNumber("021 210 10 10");
        phone->setType(PhoneType::Professional);
        phones.append(phone);
    }

    return phones;
}

PhoneList* DemoInterpretationRequestBuilder::buildPhoneList(const QString &rootKey)
{
    PhoneList *phones = AdminFactory::createEntity<PhoneList>(ABSTRACTREPO);

    for (int i = 0; i < 2; ++i) {
        SharedPhone phone = AdminFactory::createEntity<Phone>(ABSTRACTREPO);
        phone->setNumber("021 210 10 10");
        phone->setType(PhoneType::Professional);
        phones->append(phone);
    }

    return phones;
}

QList<Email*> DemoInterpretationRequestBuilder::buildEmails(const QString &rootKey)
{
    QList<Email*> emails;

    for (int i = 0; i < 2; ++i) {
        Email* email = AdminFactory::createEntity<Email>(ABSTRACTREPO);
        email->setEmail("email@mail.com");
        email->setType(Type::Professional);
        emails.append(email);
    }

    return emails;
}

Tucuxi::Gui::Core::Admin::Route DemoInterpretationRequestBuilder::toDosageRoute(const QString &route)
{
    if (route == "bolus")
        return Tucuxi::Gui::Core::Admin::BOLUS;
    if (route == "infu")
        return Tucuxi::Gui::Core::Admin::INFUSION;
    if (route == "extra")
        return Tucuxi::Gui::Core::Admin::EXTRA;

    Q_UNREACHABLE();
}

PhoneType DemoInterpretationRequestBuilder::toPhoneType(const QString &type)
{
    if (type == "work")
        return PhoneType::Professional;
    if (type == "home")
        return PhoneType::Private;
    if (type == "mobile")
        return PhoneType::Cell;

    return PhoneType::UnknownPhoneType;
}

Type DemoInterpretationRequestBuilder::toEmailType(const QString &type)
{
    if (type == "work")
        return Type::Professional;
    if (type == "home")
        return Type::Private;

    Q_UNREACHABLE();
}
