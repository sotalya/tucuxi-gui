
#include <QMessageBox>

#include "interpretationrequestbuilder.h"
#include "drugidtranslator.h"
#include "routetranslator.h"

#include "admin/src/adminfactory.h"

#include "apputils/src/apputilsfactory.h"
#include "apputils/src/apputilsrepository.h"


#include "core/core.h"
#include "core/corefactory.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/uncastedvalue.h"
#include "covariateidtranslator.h"
#include "core/dal/drug/target.h"


//InterpretationRequestBuilder::InterpretationRequestBuilder(const MessageContent &content) :
InterpretationRequestBuilder::InterpretationRequestBuilder(const QDomDocument &content) :
    content(content)
{
    //Get the control ID
//    content.setValue("controlId", doc.documentElement().attributeNode("controlId").value());
    datasetNode = content.documentElement().firstChildElement("dataset");

    //Get request data
//    content.setValue("request.id",    datasetNode.firstChildElement("requestId").firstChild().toText().data());
//    content.setValue("request.state", datasetNode.firstChildElement("requestState").firstChild().toText().data());

}


InterpretationRequestBuilder::~InterpretationRequestBuilder()
{

}

InterpretationRequest* InterpretationRequestBuilder::buildInterpretationRequest()
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: set requestId and state when added to DAL object
    //ToDo: set the practician when added to DAL object
    //ToDo: set the clinicals when added to DAL object

    InterpretationRequest* interpretationRequest = ezechiel::core::CoreFactory::createEntity<InterpretationRequest>(ABSTRACTREPO);
    ezechiel::core::DrugTreatment *treatment = interpretationRequest->getTreatment();
    treatment->setParent(interpretationRequest);
    //Prediction patient
    treatment->setPatient(buildPatient("patient"));
    treatment->getPatient()->setParent(treatment);
    //Prediction drug
    QString activeSubstanceId = buildDrug("drug");
    if (activeSubstanceId.isEmpty()) {
        EXLOG(QtWarningMsg, ezechiel::core::NOEZERROR, QObject::tr("Drug not found, cannot build interpretationRequest for analysis."));
        return nullptr;
    }

    treatment->setActiveSubstanceId(activeSubstanceId);

    //Prediction dosage
    treatment->setDosages(buildDosages("dosages"));
    treatment->getDosages()->setParent(treatment);

    //Prediction samples
    ezechiel::core::CoreMeasureList* measures = buildSamples("samples", treatment->getPatient(), activeSubstanceId);
    treatment->setMeasures(measures);
    treatment->getMeasures()->setParent(treatment);

    //Prediction covariates
    ezechiel::core::PatientVariateList * covariates = buildCovariates("covariates", treatment->getPatient());
    treatment->setCovariates(covariates);

    interpretationRequest->setClinicals(buildClinical("clinicals"));

    //targets


    return interpretationRequest;
}


QString InterpretationRequestBuilder::buildDrug(const QString &rootKey)
{

    // Currently we use the Chuv drug ID translator.
    // However in the future an XML file for translating it would be more flexible
    //DrugIdTranslator *translator = new ChuvDrugIdTranslator();
#ifdef CONFIG_DEMO
    DummyDrugIdTranslator *translator = new DummyDrugIdTranslator();
#else
    ExternalDrugIdTranslator *translator = new ExternalDrugIdTranslator();
    translator->setFileName(QCoreApplication::applicationDirPath() + "/drugidtranslations.ini");
#endif // CONFIG_DEMO
    QString restDrugId = datasetNode.firstChildElement("drug").firstChildElement("drugId").firstChild().toText().data();
    QString drugId = translator->restToInternalId(restDrugId);



    delete translator;
    if (drugId.isEmpty())
    {
        EXLOG(QtWarningMsg, ezechiel::core::NOEZERROR, QObject::tr("The drug corresponding to ID %1 is not available").arg(restDrugId));
        return "";
    }

    QList<ezechiel::core::DrugModel *> list;
    APPUTILSREPO->getDrugsList(list);
    bool valid = false;
    foreach(ezechiel::core::DrugModel *drugModel, list) {
        if (drugModel->getActiveSubstance()->getSubstanceId() == drugId)
            valid = true;
    }
    if (!valid) {
        EXLOG(QtWarningMsg, ezechiel::core::NOEZERROR, QObject::tr("Drug not found, cannot build drug for analysis."));
        return "";
    }

    return drugId;
}

ezechiel::core::DosageHistory* InterpretationRequestBuilder::buildDosages(const QString &rootKey)
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: check how to deal with dosage end date

    ezechiel::core::DosageHistory* dosages = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DosageHistory>(ABSTRACTREPO);

    //Get dosages data
    QDomElement dosageNode = datasetNode.firstChildElement("dosages").firstChildElement("dosage");
    while (!dosageNode.isNull()) {
        ezechiel::core::Dosage* dosage = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Dosage>(ABSTRACTREPO, dosages);

        //Dosage data
        ezechiel::core::Admin *admin = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Admin>(ABSTRACTREPO, dosage);


        RouteTranslator *translator = new ChuvRouteTranslator();
        QString restRoute = dosageNode.firstChildElement("intake").firstChild().toText().data();
        ezechiel::core::Admin::Route route = translator->restToInternalRoute(restRoute);
        delete translator;
        if (route == ezechiel::core::Admin::UNVALID)
        {
            EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The applied intake: %1 was not parsed into a valid intake").arg(restRoute));
            ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
            uncasted->setField("intake");
            uncasted->setText(restRoute);
            uncasted->setComment("The applied intake was not parsed into a valid intake.");
            dosage->getUncastedValues()->append(uncasted);
        }
        else if (route == ezechiel::core::Admin::DEFAULT)
        {
            EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The applied intake is an empty field. Assuming the default intake of the drug.").arg(restRoute));
            ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
            uncasted->setField("intake");
            uncasted->setText(restRoute);
            uncasted->setComment("The applied intake is an empty field. Assuming the default intake of the drug.");
            dosage->getUncastedValues()->append(uncasted);
        }
        admin->setRoute(route);
        dosage->setRoute(admin);

        {
            //Dosage date
            QString dateString = dosageNode.firstChildElement("startDate").firstChild().toText().data();
            QDateTime appl = QDateTime::fromString(dateString, Qt::ISODate);
            if (appl.isValid()) {
                dosage->setApplied(appl);
            } else {
                EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The applied date: %1 was not parsed into a valid QDateTime").arg(dateString));
                ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
                uncasted->setField("startDate");
                uncasted->setText(dateString);
                uncasted->setComment("The start time was not parsed into a valid date.");
                dosage->getUncastedValues()->append(uncasted);
            }
        }
        {
            QString dateString = dosageNode.firstChildElement("lastDate").firstChild().toText().data();
            QDateTime endt = QDateTime::fromString(dateString, Qt::ISODate);
            if (endt.isValid()) {
                dosage->setEndTime(endt);
            } else {
                EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The endtime date: %1 was not parsed into a valid QDateTime").arg(dateString));
                ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
                uncasted->setField("lastDate");
                uncasted->setText(dateString);
                uncasted->setComment("The end time was not parsed into a valid date.");
                dosage->getUncastedValues()->append(uncasted);
            }
        }

        if (dosage->getEndTime() < dosage->getApplied())
        {

            EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The endtime date: %1 is earlier than starttime %2").arg(dosage->getEndTime().toString()).arg(dosage->getApplied().toString()));
            ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
            uncasted->setField("End time");
            uncasted->setText(dosage->getEndTime().toString());
            uncasted->setComment(QString("Dosage end time prior to apply time. Using a fake end time 3 days after start"));
            dosage->setEndTime(dosage->getApplied().addDays(3));
            dosage->getUncastedValues()->append(uncasted);
        }

        //Dosage value
        {
            bool ok;
            QString valueString = dosageNode.firstChildElement("dose").firstChildElement("value").firstChild().toText().data();
            double value = valueString.toDouble(&ok);
            if (ok)
                dosage->getQuantity()->setValue(value);
            else {
                EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The value: %1 was not parsed into a valid double").arg(valueString));
                ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
                uncasted->setField("dose value");
                uncasted->setText(valueString);
                uncasted->setComment("Please fill yourself the dosage");
                dosage->getUncastedValues()->append(uncasted);
            }
        }

        {
            /**************************************************
             * TODO: Generated Uncasted values if required. Not yet ready to handle that correctly
             *************************************************/
            dosage->getQuantity()->setUnit(dosageNode.firstChildElement("dose").firstChildElement("unit").firstChild().toText().data());
        }

        //Dosage interval
        {
            bool ok;
            QString valueString = dosageNode.firstChildElement("interval").firstChildElement("value").firstChild().toText().data();
            QString unit = dosageNode.firstChildElement("interval").firstChildElement("unit").firstChild().toText().data();
            int value = valueString.toInt(&ok);
            if (ok) {
                if (unit == "h") {
                    dosage->setInterval(ezechiel::core::Duration(value));
                }
                else if (unit == "d") {
                    dosage->setInterval(ezechiel::core::Duration(24 * value));
                }
                else if ((unit == "m") || (unit == "min")) {
                    dosage->setInterval(ezechiel::core::Duration(((double) (value))/60));
                }
                else if ((value >= 1) && (value <= 4)) {
                    dosage->setInterval(ezechiel::core::Duration(24 / value));
                    ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
                    uncasted->setField("interval value");
                    uncasted->setText(valueString);
                    uncasted->setStatus(ezechiel::core::UncastedStatus::PartiallyCasted);
                    uncasted->setComment("Assuming the interval indicates the number of doses per day");
                    dosage->getUncastedValues()->append(uncasted);

                }
                else {
                    dosage->setInterval(ezechiel::core::Duration(value));
                    ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
                    uncasted->setField("interval value");
                    uncasted->setText(valueString);
                    uncasted->setStatus(ezechiel::core::UncastedStatus::PartiallyCasted);
                    uncasted->setComment("Assuming hours as unit");
                    dosage->getUncastedValues()->append(uncasted);
                }
            }
            else if (valueString.split(":").size() == 3) {
                QStringList list = valueString.split(":");
                bool ok1 = true;
                bool ok2 = true;
                bool ok3 = true;
                int h = list[0].toInt(&ok1);
                int m = list[1].toInt(&ok2);
                int s = list[2].toInt(&ok3);
                if (ok1 && ok2 && ok3) {
                    dosage->setInterval(ezechiel::core::Duration(h, m, s));
                }
                else {
                    EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The interval time: %1 was not parsed into a valid double").arg(valueString));
                    ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
                    uncasted->setField("interval value");
                    uncasted->setText(valueString);
                    uncasted->setComment("Please fill yourself the dosage interval");
                    dosage->getUncastedValues()->append(uncasted);
                }
            }
            else {
                EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The interval time: %1 was not parsed into a valid double").arg(valueString));
                ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
                uncasted->setField("interval value");
                uncasted->setText(valueString);
                uncasted->setComment("Please fill yourself the dosage interval");
                dosage->getUncastedValues()->append(uncasted);
            }
        }

        //Dosage infusion time, only do it in case of infusion
        if (dosage->getRoute()->getRoute() == ezechiel::core::Admin::INFUSION)
        {
            bool ok;
            QString valueString = dosageNode.firstChildElement("infusion").firstChildElement("value").firstChild().toText().data();
            QString unit = dosageNode.firstChildElement("infusion").firstChildElement("unit").firstChild().toText().data();
            int value = valueString.toInt(&ok);
            if (!ok) {
                double dValue = valueString.toDouble(&ok);
                if (ok) {
                    value = static_cast<int>(dValue);
                    if (static_cast<double>(value) != dValue) {
                        ok = false;
                    }
                }
            }
            if (ok) {

                if (unit == "h") {
                    dosage->setTinf(ezechiel::core::Duration(value));
                }
                else if (unit == "d") {
                    dosage->setTinf(ezechiel::core::Duration(24 * value));
                }
                else if ((unit == "m") || (unit == "min")) {
                    dosage->setTinf(ezechiel::core::Duration(0, value));
                }
                else {
                    dosage->setTinf(ezechiel::core::Duration(0,value));
                    ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
                    uncasted->setField("infusion time value");
                    uncasted->setText(valueString);
                    uncasted->setStatus(ezechiel::core::UncastedStatus::PartiallyCasted);
                    uncasted->setComment("Assuming minutes as unit");
                    dosage->getUncastedValues()->append(uncasted);
                }
            }
            else {
                // Set 60 minutes by default
                dosage->setTinf(ezechiel::core::Duration(0,60));
                EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The infusion time: %1 was not parsed into a valid double").arg(valueString));
                ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
                uncasted->setField("infusion value");
                uncasted->setText(valueString);
                uncasted->setComment("Assuming 1 hour, but please fill yourself the dosage infusion time");
                dosage->getUncastedValues()->append(uncasted);
            }
        }

        // TODO : To be checked
        dosage->setIsAtSteadyState(false);

        dosages->append(dosage);

        dosageNode = dosageNode.nextSiblingElement("dosage");
    }

    return dosages;
}

ezechiel::core::PatientVariateList * InterpretationRequestBuilder::buildCovariates(const QString &rootKey, const SharedPatient patient)
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: set the covariate type based on the drug data
    //ToDo: set the covariate nature when added to DAL object

    ChuvCovariateIdTranslator covariateIdTranslator;

    ezechiel::core::PatientVariateList * covariates = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PatientVariateList>(ABSTRACTREPO, patient);

    //Get covariates data
    QDomElement covariateNode = datasetNode.firstChildElement("covariates").firstChildElement("covariate");
    while (!covariateNode.isNull()) {
        ezechiel::core::PatientVariate* covariate = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PatientVariate>(ABSTRACTREPO);

        //Covariate data
        QString covariateId = covariateIdTranslator.restToInternalId(covariateNode.firstChildElement("name").firstChild().toText().data());
        covariate->setCovariateId(covariateId);


        //date
        QString dateString = covariateNode.firstChildElement("date").firstChild().toText().data();
        QDateTime date = QDateTime::fromString(dateString, Qt::ISODate);
        if (date.isValid()) {
            covariate->setDate(date);
        } else {
            EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The date: %1 was not parsed into a valid QDateTime").arg(dateString));
            ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, covariate->getUncastedValues());
            uncasted->setField("date");
            uncasted->setText(dateString);
            covariate->getUncastedValues()->append(uncasted);
        }


        //Covariate value
        {
            bool ok;
            QString valueString = covariateNode.firstChildElement("value").firstChildElement("value").firstChild().toText().data();

            /************************************************************
             * TODO: Check that. It uses int, but could be double I guess
             ************************************************************/
            double value = valueString.toDouble(&ok);
            if (ok)
                covariate->getQuantity()->setValue(value);
            else {
                EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The value: %1 was not parsed into a valid double").arg(valueString));
                ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, covariate->getUncastedValues());
                uncasted->setField("covariate value");
                uncasted->setText(valueString);
                covariate->getUncastedValues()->append(uncasted);
            }
        }

        /********************************************************************
         * TODO: The unit should be checked. Not yet ready to be implemented.
         ********************************************************************/
        covariate->getQuantity()->setUnit(covariateNode.firstChildElement("value").firstChildElement("unit").firstChild().toText().data());

        /************************************************************
         * TODO: Check that. It uses int, but could be double I guess.
         * This is related to the conversion also (See above).
         ************************************************************/
        covariate->setType(QMetaType::Int);

        covariates->append(covariate);
        covariateNode = covariateNode.nextSiblingElement("covariate");
    }

    return covariates;
}

/*
bool InterpretationRequestBuilder::buildClinicals(const QString &rootKey, InterpretationRequest *InterpretationRequest)
{

    QDomElement clinicalNode = datasetNode.firstChildElement(rootKey).firstChildElement("clinical");
        while (!clinicalNode.isNull()) {
            QString name = clinicalNode.firstChildElement("name").firstChild().toText().data();
            QString value = clinicalNode.firstChildElement("value").firstChild().toText().data();
            QString date = clinicalNode.firstChildElement("date").firstChild().toText().data();
            QString comment = clinicalNode.firstChildElement("comment").firstChild().toText().data();

            Clinical* clinical = AdminFactory::createEntity<Clinical>(ABSTRACTREPO);

            //Covariate data
            clinical->setName(clinicalNode.firstChildElement("name").firstChild().toText().data());
            clinical->setDate(QDateTime::fromString(clinicalNode.firstChildElement("date").firstChild().toText().data(), Qt::ISODate));
            clinical->setValue(clinicalNode.firstChildElement("value").firstChild().toText().data());
            clinical->setComment(clinicalNode.firstChildElement("comment").firstChild().toText().data());

    //        clinicals.append(clinical);
    //        clinicalNode = clinicalNode.nextSiblingElement("clinical");
        }
}
*/

ClinicalSet* InterpretationRequestBuilder::buildClinical(const QString &rootKey)
{

    ClinicalSet* clinicals = AdminFactory::createEntity<ClinicalSet>(ABSTRACTREPO);

    //Get clinicals data
    QDomElement clinicalNode = datasetNode.firstChildElement("clinicals").firstChildElement("clinical");
    while (!clinicalNode.isNull()) {
        Clinical* clinical = AdminFactory::createEntity<Clinical>(ABSTRACTREPO);

        //Covariate data
        clinical->setName(clinicalNode.firstChildElement("name").firstChild().toText().data());
        clinical->setDate(QDateTime::fromString(clinicalNode.firstChildElement("date").firstChild().toText().data(), Qt::ISODate));
        clinical->setValue(clinicalNode.firstChildElement("value").firstChild().toText().data());
        clinical->setComment(clinicalNode.firstChildElement("comment").firstChild().toText().data());

        clinicals->append(clinical);
        clinicalNode = clinicalNode.nextSiblingElement("clinical");
    }
    return clinicals;
}

SharedPatient InterpretationRequestBuilder::buildPatient(const QString &rootKey)
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: add the patient institute when added to DAL object

    SharedPatient shpatient = AdminFactory::createEntity<Patient>(ABSTRACTREPO);

    //Patient data
    Patient* patient = static_cast<Patient*>(shpatient);
    patient->externalId(datasetNode.firstChildElement("patient").firstChildElement("patientId").firstChild().toText().data());
    patient->stayNumber(datasetNode.firstChildElement("patient").firstChildElement("stayNumber").firstChild().toText().data());
    patient->person()->firstname(datasetNode.firstChildElement("patient").firstChildElement("name").firstChildElement("firstName").firstChild().toText().data());
    patient->person()->name(datasetNode.firstChildElement("patient").firstChildElement("name").firstChildElement("lastName").firstChild().toText().data());
    patient->person()->gender(datasetNode.firstChildElement("patient").firstChildElement("gender").firstChild().toText().data() == "male" ? Person::Male : Person::Female);

    //date
    QString dateString = datasetNode.firstChildElement("patient").firstChildElement("birthdate").firstChild().toText().data();
    QDate date = QDateTime::fromString(dateString, Qt::ISODate).date();
    if (date.isValid()) {
        patient->person()->birthday(date);
    } else {
        EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The date: %1 was not parsed into a valid QDateTime").arg(dateString));
        ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, patient->person()->getUncastedValues());
        uncasted->setField("birthdate");
        uncasted->setText(dateString);
        patient->person()->getUncastedValues()->append(uncasted);
    }


    //Patient contact
    patient->person()->location()->address(datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("address").firstChild().toText().data());
    patient->person()->location()->postcode(datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("postcode").firstChild().toText().data());
    patient->person()->location()->city(datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("city").firstChild().toText().data());
    patient->person()->location()->state(datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("state").firstChild().toText().data());
    patient->person()->location()->country(datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("country").firstChild().toText().data());

    //Patient phones
    patient->person()->setPhones(buildPhoneList(rootKey, ""));

    //Patient mails
    patient->person()->setEmails(buildEmails(rootKey, ""));

    //Get patient data
//    content.setValue("patient.name.middle", datasetNode.firstChildElement("patient").firstChildElement("name").firstChildElement("middleName").firstChild().toText().data());
    return shpatient;
}

SharedPractician InterpretationRequestBuilder::buildPractician(const QString &rootKey)
{
    //ToDo: check critical info and build, or store incomplete data

    SharedPractician practician = AdminFactory::createEntity<Practician>(ABSTRACTREPO);

    //Practician data
    practician->externalId(datasetNode.firstChildElement("mandator").firstChildElement("practicianId").firstChild().toText().data());
    practician->title(datasetNode.firstChildElement("mandator").firstChildElement("title").firstChild().toText().data());
    practician->person()->firstname(datasetNode.firstChildElement("mandator").firstChildElement("name").firstChildElement("firstName").firstChild().toText().data());
    practician->person()->name(datasetNode.firstChildElement("mandator").firstChildElement("name").firstChildElement("lastName").firstChild().toText().data());
    practician->person()->gender(datasetNode.firstChildElement("mandator").firstChildElement("gender").firstChild().toText().data() == "male" ? Person::Male : Person::Female);

    //date
    QString dateString = datasetNode.firstChildElement("mandator").firstChildElement("birthdate").firstChild().toText().data();
    QDate date = QDateTime::fromString(dateString, Qt::ISODate).date();
    if (date.isValid()) {
        practician->person()->birthday(date);
    } else {
        EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The date: %1 was not parsed into a valid QDate").arg(dateString));
        ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, practician->person()->getUncastedValues());
        uncasted->setField("birthdate");
        uncasted->setText(dateString);
        practician->person()->getUncastedValues()->append(uncasted);
    }


    //Practician contact
    practician->person()->location()->address(datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("address").firstChild().toText().data());
    practician->person()->location()->postcode(datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("postcode").firstChild().toText().data());
    practician->person()->location()->city(datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("city").firstChild().toText().data());
    practician->person()->location()->state(datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("state").firstChild().toText().data());
    practician->person()->location()->country(datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("country").firstChild().toText().data());

    //Practician phones
    practician->person()->setPhones(buildPhoneList(rootKey, ""));

    //Practician mails
    practician->person()->setEmails(buildEmails(rootKey, ""));

    //Practician institute
    practician->institute(buildInstitute(rootKey));

    //Get mandator data
//    content.setValue("mandator.name.middle", datasetNode.firstChildElement("mandator").firstChildElement("name").firstChildElement("middleName").firstChild().toText().data());
    return practician;
}

SharedInstitute InterpretationRequestBuilder::buildInstitute(const QString &rootKey)
{
    //ToDo: check critical info and build, or store incomplete data

    SharedInstitute institute = AdminFactory::createEntity<Institute>(ABSTRACTREPO);

    //Institute data
    institute->externalId(datasetNode.firstChildElement(rootKey).firstChildElement("institute").firstChildElement("instituteId").firstChild().toText().data());
    institute->name(datasetNode.firstChildElement(rootKey).firstChildElement("institute").firstChildElement("name").firstChild().toText().data());

    //Institute contact
    institute->location()->address(datasetNode.firstChildElement(rootKey).firstChildElement("institute").firstChildElement("contact").firstChildElement("address").firstChild().toText().data());
//    institute->location()->postcode(content.value(rootKey + ".contact.postcode"));
    institute->location()->city(datasetNode.firstChildElement(rootKey).firstChildElement("institute").firstChildElement("contact").firstChildElement("city").firstChild().toText().data());
    institute->location()->state(datasetNode.firstChildElement(rootKey).firstChildElement("institute").firstChildElement("contact").firstChildElement("state").firstChild().toText().data());
    institute->location()->country(datasetNode.firstChildElement(rootKey).firstChildElement("institute").firstChildElement("contact").firstChildElement("country").firstChild().toText().data());

    //Institute phones
    institute->setPhones(buildPhones(rootKey, "institute"));

    //Institute mails
    institute->setEmails(buildEmails(rootKey, "institute"));

    //Get patient institute contact
//    content.setValue("patient.institute.contact.postcode", datasetNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("contact").firstChildElement("postcode").firstChild().toText().data());
    return institute;
}

ezechiel::core::CoreMeasureList* InterpretationRequestBuilder::buildSamples(const QString &rootKey, const SharedPatient patient, const QString &activeSubstance)
{
    //ToDo: check critical info and build, or store incomplete data
    //ToDo: add multiple concentrations when DAL object supports it

    //Get samples data
    ezechiel::core::CoreMeasureList* measures = ezechiel::core::CoreFactory::createEntity<ezechiel::core::CoreMeasureList>(ABSTRACTREPO);
    QDomElement sampleNode = datasetNode.firstChildElement("samples").firstChildElement("sample");
    while (!sampleNode.isNull()) {
        Measure * measure = AdminFactory::createEntity<Measure>(ABSTRACTREPO, measures);

        //Measure data
        measure->sampleID(sampleNode.firstChildElement("id").firstChild().toText().data());

        /*******************************************************
         * TODO: When DAL is refactored, then the substance should
         * be checked to match form the XML to the substanceID or analyteID
         *******************************************************/
        measure->setSdrug(activeSubstance);

        //Measure dates

        {
            //date
            QString dateString = sampleNode.firstChildElement("sampleDate").firstChild().toText().data();
            QDateTime date = QDateTime::fromString(dateString, Qt::ISODate);
            if (date.isValid()) {
                measure->setMoment(date);
            } else {
                EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The date: %1 was not parsed into a valid QDateTime").arg(dateString));
                ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, measure->getUncastedValues());
                uncasted->setField("sampleDate");
                uncasted->setText(dateString);
                measure->getUncastedValues()->append(uncasted);
            }
        }

        {
            //date
            QString dateString = sampleNode.firstChildElement("arrivalDate").firstChild().toText().data();
            QDateTime date = QDateTime::fromString(dateString, Qt::ISODate);
            if (date.isValid()) {
                measure->arrivalDate(date);
            } else {
                EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The date: %1 was not parsed into a valid QDateTime").arg(dateString));
                ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, measure->getUncastedValues());
                uncasted->setField("arrivalDate");
                uncasted->setText(dateString);
                measure->getUncastedValues()->append(uncasted);
            }
        }

        QList<ezechiel::core::IdentifiableAmount*> amts;

        QDomElement concentrationNode = sampleNode.firstChildElement("concentrations").firstChildElement("concentration");
        while (!concentrationNode.isNull()) {
            ezechiel::core::IdentifiableAmount * amt = ezechiel::core::CoreFactory::createEntity<ezechiel::core::IdentifiableAmount>(ABSTRACTREPO, measure);

            //Dosage value
            {
                bool ok;
                QString valueString = concentrationNode.firstChildElement("value").firstChild().toText().data();
                double value = valueString.toDouble(&ok);
                if (ok)
                    amt->setValue(value);
                else {
                    EXLOG(QtWarningMsg, ezechiel::core::DATAERROR, QObject::tr("The value: %1 was not parsed into a valid double").arg(valueString));
                    ezechiel::core::UncastedValue *uncasted = ezechiel::core::CoreFactory::createEntity<ezechiel::core::UncastedValue>(ABSTRACTREPO, measure->getUncastedValues());
                    uncasted->setField("concentration value");
                    uncasted->setText(valueString);
                    measure->getUncastedValues()->append(uncasted);
                }
            }

            /*********************************************************************
             * TODO: If the unit is not recognized, should be handled correctly
             *********************************************************************/
            amt->setUnit(ezechiel::core::Unit(concentrationNode.firstChildElement("unit").firstChild().toText().data()));

            amts.append(amt);
            concentrationNode = concentrationNode.nextSiblingElement("concentration");
        }

        //Measure value
        measure->setConcentration(amts.at(0));
        measures->append(measure);
        sampleNode = sampleNode.nextSiblingElement("sample");
    }

    return measures;
}

QList<SharedPhone> InterpretationRequestBuilder::buildPhones(const QString &rootKey, const QString &rootKey2)
{
    QList<SharedPhone> phones;

    QDomElement patientPhoneNode;
    if (rootKey2 != "") {
        patientPhoneNode = datasetNode.firstChildElement(rootKey).firstChildElement(rootKey2).firstChildElement("contact").firstChildElement("phones").firstChildElement("email");
    } else {
        patientPhoneNode = datasetNode.firstChildElement(rootKey).firstChildElement("contact").firstChildElement("emails").firstChildElement("phones");
    }
    //Get patient phones
    while (!patientPhoneNode.isNull()) {
        SharedPhone phone = AdminFactory::createEntity<Phone>(ABSTRACTREPO);
        phone->setNumber(patientPhoneNode.firstChild().toText().data());
        phone->setType(toPhoneType(patientPhoneNode.attributeNode("type").value()));
        phones.append(phone);

        patientPhoneNode = patientPhoneNode.nextSiblingElement("phone");
    }

    return phones;
}


PhoneList *InterpretationRequestBuilder::buildPhoneList(const QString &rootKey, const QString &rootKey2)
{
    PhoneList *phones = AdminFactory::createEntity<PhoneList>(ABSTRACTREPO);

    QDomElement patientPhoneNode;
    if (rootKey2 != "") {
        patientPhoneNode = datasetNode.firstChildElement(rootKey).firstChildElement(rootKey2).firstChildElement("contact").firstChildElement("phones").firstChildElement("email");
    } else {
        patientPhoneNode = datasetNode.firstChildElement(rootKey).firstChildElement("contact").firstChildElement("emails").firstChildElement("phones");
    }
    //Get patient phones
    while (!patientPhoneNode.isNull()) {
        SharedPhone phone = AdminFactory::createEntity<Phone>(ABSTRACTREPO);
        phone->setNumber(patientPhoneNode.firstChild().toText().data());
        phone->setType(toPhoneType(patientPhoneNode.attributeNode("type").value()));
        phones->append(phone);

        patientPhoneNode = patientPhoneNode.nextSiblingElement("phone");
    }

    return phones;
}

QList<Email*> InterpretationRequestBuilder::buildEmails(const QString &rootKey, const QString &rootKey2)
{
    QList<Email*> emails;

    //Get patient emails
    QDomElement patientEmailNode;
    if (rootKey2 != "") {
        patientEmailNode = datasetNode.firstChildElement(rootKey).firstChildElement(rootKey2).firstChildElement("contact").firstChildElement("emails").firstChildElement("email");
    } else {
        patientEmailNode = datasetNode.firstChildElement(rootKey).firstChildElement("contact").firstChildElement("emails").firstChildElement("email");
    }
    while (!patientEmailNode.isNull()) {
        Email* email = AdminFactory::createEntity<Email>(ABSTRACTREPO);
        email->setEmail(patientEmailNode.firstChild().toText().data());
        email->setType(toEmailType(patientEmailNode.attributeNode("type").value()));
        emails.append(email);

        patientEmailNode = patientEmailNode.nextSiblingElement("email");
    }
    return emails;
}


PhoneType InterpretationRequestBuilder::toPhoneType(const QString &type)
{
    if (type == "work")
        return PhoneType::Professional;
    if (type == "home")
        return PhoneType::Private;
    if (type == "mobile")
        return PhoneType::Cell;

    return PhoneType::UnknownPhoneType;
}

Type InterpretationRequestBuilder::toEmailType(const QString &type)
{
    if (type == "work")
        return Type::Professional;
    if (type == "home")
        return Type::Private;

    Q_UNREACHABLE();
}
