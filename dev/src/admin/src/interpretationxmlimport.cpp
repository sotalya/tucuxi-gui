//@@license@@

#include "interpretationxmlimport.h"

#include "dal/interpretation.h"
#include "dal/interpretationanalysis.h"
#include "dal/interpretationrequest.h"
#include "dal/measure.h"
#include "dal/clinical.h"
#include "dal/patient.h"
#include "dal/validationstatus.h"
#include "dal/steptypes.h"

#include "adminfactory.h"
#include "stdadminrepository.h"

#include "core/corefactory.h"
#include "core/core.h"
#include "core/dal/drug/drug.h"
#include "core/dal/drug/adme.h"
#include "core/dal/drug/halflife.h"
#include "core/dal/drug/concentrations.h"
#include "core/dal/drug/results.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/target.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/drug/errormodel.h"
#include "core/dal/drug/bsv.h"
#include "core/dal/drug/operation.h"
#include "core/dal/drug/metadata.h"
#include "core/dal/drug/editors.h"
#include "core/dal/drug/references.h"

#include "core/dal/drug/parameters.h"
#include "core/dal/predictionspec.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/coremeasure.h"
#include "core/dal/covariate.h"
#include "core/dal/dosage.h"

#include "admin/src/dal/practician.h"

#include "core/utils/duration.h"

#include <iostream>

#define WHILE_NOT_END_ELEM(elem)     while(isOk && !reader.hasError() && (!reader.isEndElement() || reader.name() != elem))



InterpretationXmlImport::InterpretationXmlImport()
{

}


Interpretation *InterpretationXmlImport::load(const QByteArray &data)
{
    isOk = true;

    Interpretation *interpretation = nullptr;

    reader.addData(data);
    reader.readNext();
    while(!reader.atEnd() && isOk){
        QString name = reader.name().toString();

        switch(reader.tokenType()){
        case QXmlStreamReader::StartElement :
            if (name == "interpretation") {
                interpretation = loadInterpretation("interpretation");
            }
            break;
        default:
            break;
        }
        reader.readNext();
    }

    if(reader.hasError() || !isOk){
        std::cout <<"reader error " << qPrintable(errorString()) << std::endl;
        std::cout <<"reader error " << qPrintable(_errorString) << std::endl;
        delete interpretation;
        interpretation = nullptr;
    }

    return interpretation;
}

void InterpretationXmlImport::checkValidity(bool condition)
{
    if (!condition) {
        /*****************
         * TODO: store information about the error
         *****************/
        isOk = false;
        _errorString = errorString();
        std::cout <<"reader error " << qPrintable(errorString()) << std::endl;
    }
}

void InterpretationXmlImport::checkReaderError()
{
    if (reader.hasError()) {
        /*****************
         * TODO: store information about the error
         *****************/
        isOk = false;
        _errorString = errorString();
        std::cout <<"reader error " << qPrintable(errorString()) << std::endl;
    }

}

QDateTime InterpretationXmlImport::extractDate(bool essential)
{
    QDateTime date = QDateTime::fromString(extractor(),Qt::ISODate);
    if (essential)
        checkValidity(date.isValid());
    return date;
}

int InterpretationXmlImport::extractInt()
{
    QString value = extractor();
    bool ok;
    int result;
    result = value.toInt(&ok);
    checkValidity(ok);
    return result;
}

double InterpretationXmlImport::extractDouble()
{
    QString value = extractor();
    bool ok;
    double result;
    result = value.toDouble(&ok);
    checkValidity(ok);
    return result;
}

bool InterpretationXmlImport::extractBool()
{
    QString value = extractor();
    if (value.compare("true") == 0)
        return true;
    if (value.compare("false") == 0)
        return false;
    // There is an error
    checkValidity(false);

    return false;
}

Interpretation::InterpretationType stringToInterpretationType(const QString &s)
{
    if (s == "fromRequest")
        return Interpretation::InterpretationType::FromRequest;
    if (s == "noRequest")
        return Interpretation::InterpretationType::NoRequest;
    if (s == "undefinedRequest")
        return Interpretation::InterpretationType::Undefined;
    // TODO Handle error
    return Interpretation::InterpretationType::Undefined;
}

Interpretation *InterpretationXmlImport::loadInterpretation(const QString &tagName, QObject *parent)
{
    Interpretation *interpretation = ezechiel::GuiCore::CoreFactory::createEntity<Interpretation>(ADMINREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "interpretationType") {
                QString t = extractor();
                Interpretation::InterpretationType interpretationType;
                interpretationType = stringToInterpretationType(t);
                interpretation->setInterpretationType(interpretationType);

            } else if (name == "analysis") {
                interpretation->setAnalysis(loadAnalysis("analysis", interpretation));
            }
            else if (name == "drugResponseAnalysis") {
                ezechiel::GuiCore::DrugResponseAnalysis *analysis = loadDrugResponseAnalysis("drugResponseAnalysis", interpretation);
                if (analysis == nullptr) {
                    // Something went wrong
                    return nullptr;
                }
                interpretation->setDrugResponseAnalysis(analysis);
            }
            else if (name == "interpretationRequest") {
                interpretation->setRequest(loadRequest("interpretationRequest", interpretation));
            }
            else if (name == "analyst") {
                interpretation->setAnalyst(loadPractician("analyst", interpretation));
            }
            else if (name == "validationStatus") {
                interpretation->setValidationStatus(loadValidationStatus("validationStatus", interpretation));
            }
            else if (name == "adjustment") {
                loadAdjustment("adjustment", interpretation);
            }
            else if (name == "interpretationStartDate") {
                interpretation->setStartInterpretationTime(extractDate());
            }
            else if (name == "interpretationValidationDate") {
                interpretation->setValidateInterpretationTime(extractDate());
            }
        }
    }
    return interpretation;
}

void InterpretationXmlImport::loadAdjustment(const QString &tagName, Interpretation *interpretation)
{
    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "adjustmentDateIsSet") {
                interpretation->setAdjustmentDateIsSet(extractBool());
            }
            else if (name == "adjustmentWithLoadingDose") {
                interpretation->setAdjustmentWithLoadingDose(extractBool());
            }
            else if (name == "adjustmentWithRestPeriod") {
                interpretation->setAdjustmentWithRestPeriod(extractBool());
            }
            else if (name == "adjustmentDate") {
                interpretation->setAdjustmentDate(extractDate());
            }
        }
    }
}

InterpretationAnalysis *InterpretationXmlImport::loadAnalysis(const QString &tagName, QObject *parent)
{
    InterpretationAnalysis *analysis = ezechiel::GuiCore::CoreFactory::createEntity<InterpretationAnalysis>(ADMINREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();
            if (name == "nextControl") {
                analysis->setNextControl(extractDate());
            }
            else if (name == "dosageDescription") {
                analysis->setDosageDescription(extractor());
            }
            else if (name == "field") {
                QString fieldName;
                QString fieldValue;
                WHILE_NOT_END_ELEM("field"){
                    if(isOk && reader.readNextStartElement()){
                        QString insideName = reader.name().toString();
                        if (insideName == "name")
                            fieldName = extractor();
                        else if (insideName == "value")
                            fieldValue = extractor();
                    }
                }
                if (fieldName == "expectedness")
                    analysis->setExpectedness(fieldValue);
                else if (fieldName == "suitability")
                    analysis->setSuitability(fieldValue);
                else if (fieldName == "prediction")
                    analysis->setPrediction(fieldValue);
                else if (fieldName == "remonitoring")
                    analysis->setRemonitoring(fieldValue);
                else if (fieldName == "warning")
                    analysis->setWarning(fieldValue);
                else {
                    // TODO We are facing an error
                }
            }
        }
    }
    return analysis;
}

InterpretationRequest *InterpretationXmlImport::loadRequest(const QString &tagName, QObject *parent)
{

    InterpretationRequest *request = ezechiel::GuiCore::CoreFactory::createEntity<InterpretationRequest>(ADMINREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();
            if (name == "clinicals") {
                request->setClinicals(loadClinicals("clinicals", request));
            }
            if (name == "physician"){
                request->setPractician(loadPractician("physician", request));
            }
        }
    }
    return request;
}

ClinicalSet *InterpretationXmlImport::loadClinicals(const QString &tagName, QObject *parent)
{

    ClinicalSet *clinicals = new ClinicalSet(ADMINREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "clinical") {
                clinicals->append(loadClinical("clinical", nullptr));
            }
        }
    }
    return clinicals;
}

Clinical *InterpretationXmlImport::loadClinical(const QString &tagName, QObject *parent)
{
    Clinical *clinical = ezechiel::GuiCore::CoreFactory::createEntity<Clinical>(ADMINREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();
            if (name == "name")
                clinical->setName(extractor());
            else if (name == "date")
                clinical->setDate(extractDate());
            else if (name == "value")
                clinical->setName(extractor());
            else if (name == "comment")
                clinical->setName(extractor());
        }
    }
    return clinical;
}


Practician *InterpretationXmlImport::loadPractician(const QString &tagName, QObject *parent)
{
    Practician *practician = ezechiel::GuiCore::CoreFactory::createEntity<Practician>(ADMINREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();
            if (name == "personId")
                practician->person_id(extractInt());
            else if (name == "externalId")
                practician->externalId(extractor());
            else if (name == "title")
                practician->title(extractor());
            else if (name == "role")
                practician->role(extractor());
            else if (name == "address")
                practician->institute()->location()->address(extractor());
            else if (name == "affilition")
                practician->institute()->name(extractor());
            else if (name == "person")
                practician->person(loadPerson("person", practician));
        }
    }
    return practician;
}

ValidationStatus::ValidationStatusType statusFromString(const QString &status)
{
    if (status == "validated")
        return ValidationStatus::Validated;
    if (status == "unvalidated")
        return ValidationStatus::UnValidated;
    // TODO Handle error

    return ValidationStatus::UnValidated;
}

StepType::Enum validationStepFromString(const QString &step)
{
    if (step == "patient")
        return  StepType::Patient;
    if (step == "drug")
        return StepType::Drug;
    if (step == "dosage")
        return StepType::Dosage;
    if (step == "covariate")
        return StepType::Covariate;
    if (step == "measure")
        return StepType::Measure;
    if (step == "target")
        return StepType::Target;
    if (step == "adjustment")
        return StepType::Adjustment;
    if (step == "validation")
        return StepType::Validation;
    if (step == "report")
        return StepType::Report;

    // TODO Handle error
    return StepType::Patient;
}

ValidationStatus *InterpretationXmlImport::loadValidationStatus(const QString &tagName, QObject *parent)
{
    ValidationStatus *validationStatus = ezechiel::GuiCore::CoreFactory::createEntity<ValidationStatus>(ADMINREPO,parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();
            if (name == "stepStatus") {
                QString step;
                QString status;
                WHILE_NOT_END_ELEM("stepStatus"){
                    if(isOk && reader.readNextStartElement()){
                        QString insideName = reader.name().toString();
                        if (insideName == "step")
                            step = extractor();
                        else if (insideName == "status")
                            status = extractor();
                    }
                }
                validationStatus->setValidationStatus(validationStepFromString(step), statusFromString(status));
            }
        }
    }
    return validationStatus;
}


std::vector<double> InterpretationXmlImport::loadPercentiles(const QString &tagName, QObject *parent)
{
    Q_UNUSED(parent);
    std::vector<double> percentiles;

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "p") {
                percentiles.push_back(extractDouble());
            }
        }
    }

    return percentiles;
}
#include "apputils/src/appcore.h"
#include "apputils/src/drugmanager.h"
#include "apputils/src/apputilsrepository.h"
#include "core/utils/errorenums.h"

ezechiel::GuiCore::DrugModel *findDrugModelFromId(const QString &drugModelId)
{
    return APPCORE->drugManager()->drug(drugModelId);
}

ezechiel::GuiCore::DrugResponseAnalysis * InterpretationXmlImport::loadDrugResponseAnalysis(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::DrugResponseAnalysis *analysis = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugResponseAnalysis>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "treatment") {
                analysis->setTreatment(loadDrugTreatment("treatment", analysis));
            }
            else if (name == "drugModelId") {
                QString drugModelId = extractor();
                ezechiel::GuiCore::DrugModel *drugModel;
                drugModel = findDrugModelFromId(drugModelId);
                if (drugModel == nullptr) {
                    // TODO : We are facing an issue here
                    // Log error
                    return nullptr;
                }
                else
                    analysis->setDrugModel(drugModel);
            }
        }
    }
    return analysis;
}


ezechiel::GuiCore::DrugTreatment * InterpretationXmlImport::loadDrugTreatment(const QString &tagName, QObject *parent)
{

    ezechiel::GuiCore::DrugTreatment *treatment = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugTreatment>(ABSTRACTREPO, parent);


    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "activeSubstanceId") {
                treatment->setActiveSubstanceId(extractor());
            }
            else if (name == "patient") {
                treatment->setPatient(loadPatient("patient", treatment));
            } else if (name == "measures") {
                ezechiel::GuiCore::CoreMeasureList *list =  ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::CoreMeasureList>(ABSTRACTREPO, treatment);

                WHILE_NOT_END_ELEM("measures"){

                    if(isOk && reader.readNextStartElement()){
                        QString name = reader.name().toString();

                        if (name == "measure") {
                            Measure *measure =  ezechiel::GuiCore::CoreFactory::createEntity<Measure>(ADMINREPO, list);

                            WHILE_NOT_END_ELEM("measure"){

                                if(isOk && reader.readNextStartElement()){
                                    QString name = reader.name().toString();

                                    if (name == "sampleId") {
                                        measure->sampleID(extractor());
                                    }
                                    else if (name == "arrivalDate") {
                                        measure->arrivalDate(extractDate());
                                    }
                                    else if (name == "concentration") {
                                        measure->setConcentration(loadIdentifiableAmount("concentration", measure));
                                    }
                                    else if (name == "moment") {
                                        measure->setMoment(extractDate());
                                    }
                                    else if (name == "uncastedValues") {

                                    }
                                }
                            }

                            list->append(measure);
                        }
                    }
                }
                treatment->setMeasures(list);
            }
            else if (name == "patientVariates") {
                treatment->setCovariates(loadPatientVariates("patientVariates", treatment));
            }
            else if (name == "dosageHistory") {
                treatment->setDosages(loadDosageHistory("dosageHistory", treatment));
            }
            else if (name == "targets") {
                treatment->setTargets(loadTargets("targets", treatment));
            }
            else if (name == "adjustments") {
                // The third argument allows to load AdjustmentDosage objects instead of Dosage ones
                treatment->setAdjustments(loadDosageHistory("adjustments", treatment, true));
            }
        }
    }

    return treatment;
}



Patient *InterpretationXmlImport::loadPatient(const QString &tagName, QObject *parent)
{

    Patient *patient = ezechiel::GuiCore::CoreFactory::createEntity<Patient>(ADMINREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "personId") {
                patient->person_id(extractInt());
            }
            else if (name == "externalId") {
                patient->externalId(extractor());
            }
            else if (name == "stayNumber") {
                patient->stayNumber(extractor());
            }
            else if (name == "person") {
                patient->person(loadPerson("person", patient));
            }
        }
    }
    return patient;
}

Person *InterpretationXmlImport::loadPerson(const QString &tagName, QObject *parent)
{
    Person *person = ezechiel::GuiCore::CoreFactory::createEntity<Person>(ADMINREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "name") {
                person->name(extractor());
            }
            else if (name == "firstname") {
                person->firstname(extractor());
            }
            else if (name == "birthday") {
                person->birthday(extractDate().date());
            }
            else if (name == "gender") {
                QString value = extractor();
                Person::GenderType gender;
                if (value == "male")
                    gender = Person::Male;
                else if (value == "female")
                    gender = Person::Female;
                else {
                    // TODO Handle error
                }
                person->gender(gender);
            }
            else if (name == "address")
                person->location()->address(extractor());
            else if (name == "city")
                person->location()->city(extractor());
            else if (name == "postcode")
                person->location()->postcode(extractor());
            else if (name == "state")
                person->location()->state(extractor());
            else if (name == "country")
                person->location()->country(extractor());
            else if (name == "phoneNumber")
                person->setPhones(loadPhoneNumber("phoneNumber", person));
        }
    }
    return person;
}

PhoneList *InterpretationXmlImport::loadPhoneNumber(const QString &tagName, QObject *parent)
{
    PhoneList *phoneListSet = ezechiel::GuiCore::CoreFactory::createEntity<PhoneList>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() && isOk){
            Phone *phone = ezechiel::GuiCore::CoreFactory::createEntity<Phone>(ABSTRACTREPO, phoneListSet);
            phone->setNumber(extractor());
            phoneListSet->append(phone);
        }
    }

    checkReaderError();
    return phoneListSet;
}

ezechiel::GuiCore::PatientVariateList *InterpretationXmlImport::loadPatientVariates(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::PatientVariateList *patientVariateSet = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PatientVariateList>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){
        /******************************************
         * TODO : Check the tag name : covariate
         *****************************************/
        if(reader.readNextStartElement() && isOk){
            patientVariateSet->append(loadPatientVariate("patientVariate", patientVariateSet));
        }
    }
    checkReaderError();

    return patientVariateSet;
}

ezechiel::GuiCore::PatientVariate *InterpretationXmlImport::loadPatientVariate(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::PatientVariate *patientVariate = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PatientVariate>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "name") {
                patientVariate->setCovariateId(extractor());
            }
            else if (name == "date") {
                patientVariate->setDate(extractDate());
            }
            else if (name == "valueAsString") {
                patientVariate->setValueAsString(extractor());
            }
            else if (name == "quantity") {
                patientVariate->setQuantity(loadOperableAmount("quantity", patientVariate));
            }
            else if (name == "type") {
                patientVariate->setType(qStringToType(extractor(), isOk));
            }
            else if (name == "forced") {
                patientVariate->setForced(extractBool());
            }
        }
    }

    checkReaderError();

    return patientVariate;
}

ezechiel::GuiCore::DosageHistory *InterpretationXmlImport::loadDosageHistory(const QString &tagName, QObject *parent, bool isAdjustment)
{
    ezechiel::GuiCore::DosageHistory *history = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DosageHistory>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){
        /******************************************
         * TODO : Check the tag name : dosage
         *****************************************/
        if(reader.readNextStartElement() && isOk){
            if (isAdjustment)
                history->append(loadAdjustmentDosage("dosage", history));
            else
                history->append(loadDosage("dosage", history));
        }
    }

    checkReaderError();

    return history;
}



Tucuxi::Core::Formulation extractFormulation(QString s)
{

    static std::map<std::string, Tucuxi::Core::Formulation> m =
    {
        {"undefined", Tucuxi::Core::Formulation::Undefined},
        {"parenteral solution", Tucuxi::Core::Formulation::ParenteralSolution},
        {"oral solution", Tucuxi::Core::Formulation::OralSolution},
        {"test", Tucuxi::Core::Formulation::Test}
    };

    auto it = m.find(s.toStdString());
    if (it != m.end()) {
        return it->second;
    }
    return Tucuxi::Core::Formulation::Undefined;
}


Tucuxi::Core::AdministrationRoute extractAdministrationRoute(QString s)
{

    static std::map<std::string, Tucuxi::Core::AdministrationRoute> m =
    {
        {"undefined", Tucuxi::Core::AdministrationRoute::Undefined},
        {"intramuscular", Tucuxi::Core::AdministrationRoute::Intramuscular},
        {"intravenousBolus", Tucuxi::Core::AdministrationRoute::IntravenousBolus},
        {"intravenousDrip", Tucuxi::Core::AdministrationRoute::IntravenousDrip},
        {"nasal", Tucuxi::Core::AdministrationRoute::Nasal},
        {"oral", Tucuxi::Core::AdministrationRoute::Oral},
        {"rectal", Tucuxi::Core::AdministrationRoute::Rectal},
        {"subcutaneous", Tucuxi::Core::AdministrationRoute::Subcutaneous},
        {"sublingual", Tucuxi::Core::AdministrationRoute::Sublingual},
        {"transdermal", Tucuxi::Core::AdministrationRoute::Transdermal},
        {"vaginal", Tucuxi::Core::AdministrationRoute::Vaginal}
    };

    auto it = m.find(s.toStdString());
    if (it != m.end()) {
        return it->second;
    }
    return Tucuxi::Core::AdministrationRoute::Undefined;
}


Tucuxi::Core::AbsorptionModel extractAbsorptionModel(QString s)
{

    static std::map<std::string, Tucuxi::Core::AbsorptionModel> m =
    {
        {"undefined", Tucuxi::Core::AbsorptionModel::Undefined},
        {"bolus", Tucuxi::Core::AbsorptionModel::Intravascular},
        {"extra", Tucuxi::Core::AbsorptionModel::Extravascular},
        {"extra.lag", Tucuxi::Core::AbsorptionModel::ExtravascularLag},
        {"infusion", Tucuxi::Core::AbsorptionModel::Infusion}
    };

    auto it = m.find(s.toStdString());
    if (it != m.end()) {
        return it->second;
    }

    return Tucuxi::Core::AbsorptionModel::Undefined;

}


ezechiel::GuiCore::Admin *InterpretationXmlImport::loadAdmin(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::Admin *admin = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO, parent);

    // By default, not at steady state

    Tucuxi::Core::Formulation formulation;
    Tucuxi::Core::AbsorptionModel absorptionModel;
    Tucuxi::Core::AdministrationRoute administrationRoute;
    std::string administrationName;

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "formulation")
                formulation = extractFormulation(extractor());
            else if (name == "administrationName")
                administrationName = extractor().toStdString();
            else if (name == "administrationRoute")
                administrationRoute = extractAdministrationRoute(extractor());
            else if (name == "absorptionModel") {
                absorptionModel = extractAbsorptionModel(extractor());
            }
        }
    }
    checkReaderError();

    admin->setFormulationAndRoute(Tucuxi::Core::FormulationAndRoute(formulation, administrationRoute, absorptionModel, administrationName));

    return admin;
}

ezechiel::GuiCore::AdjustmentDosage *InterpretationXmlImport::loadAdjustmentDosage(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::AdjustmentDosage *dosage = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::AdjustmentDosage>(ABSTRACTREPO, parent);

    // By default, not at steady state

    dosage->setIsAtSteadyState(false);
    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "type")
                dosage->setType(extractor());
            else if (name == "dbinterval")
                dosage->setDbinterval(extractDouble());
            else if (name == "dbtinf")
                dosage->setDbtinf(extractDouble());
            else if (name == "route") {
                ezechiel::GuiCore::Admin *admin = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO, dosage);
                admin->setLabel(extractor());
                dosage->setRoute(admin);
            }
            else if (name == "formulationAndRoute") {
                auto admin = loadAdmin("formulationAndRoute", dosage);
                dosage->setRoute(admin);
            }
            else if (name == "quantity") {
                dosage->setQuantity(loadIdentifiableAmount("quantity", dosage));
            }
            else if (name == "applied")
                dosage->setApplied(extractDate());
            else if (name == "endTime")
                dosage->setEndTime(extractDate());
            else if (name == "isAtSteadyState")
                dosage->setIsAtSteadyState(extractBool());
            else if (name == "tinf") {
                dosage->setTinf(ezechiel::GuiCore::Duration(0, extractDouble()));
            }
            else if (name == "interval") {
                dosage->setInterval(ezechiel::GuiCore::Duration(0, extractDouble()));
            }
            else if (name == "uncastedValues") {
                // Handle uncasted values
            }
        }
    }
    checkReaderError();

    return dosage;
}



ezechiel::GuiCore::Dosage *InterpretationXmlImport::loadDosage(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::Dosage *dosage = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Dosage>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "dbinterval")
                dosage->setDbinterval(extractDouble());
            else if (name == "dbtinf")
                dosage->setDbtinf(extractDouble());
            else if (name == "route") {
                ezechiel::GuiCore::Admin *admin = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO, dosage);
                admin->setLabel(extractor());
                dosage->setRoute(admin);
            }
            else if (name == "formulationAndRoute") {
                auto admin = loadAdmin("formulationAndRoute", dosage);
                dosage->setRoute(admin);
            }
            else if (name == "quantity") {
                dosage->setQuantity(loadIdentifiableAmount("quantity", dosage));
            }
            else if (name == "applied")
                dosage->setApplied(extractDate());
            else if (name == "endTime")
                dosage->setEndTime(extractDate());
            else if (name == "isAtSteadyState")
                dosage->setIsAtSteadyState(extractBool());
            else if (name == "steadyStateLastDoseTime")
                dosage->setSteadyStateLastDoseDate(extractDate());
            else if (name == "tinf") {
                dosage->setTinf(ezechiel::GuiCore::Duration(0, extractDouble()));
            }
            else if (name == "interval") {
                dosage->setInterval(ezechiel::GuiCore::Duration(0, extractDouble()));
            }
            else if (name == "uncastedValues") {
                // Handle uncasted values
            }
        }
    }
    checkReaderError();

    return dosage;
}



ezechiel::GuiCore::DrugModel *InterpretationXmlImport::loadDrugModel(const QString &tagName, QObject *parent)
{

    ezechiel::GuiCore::DrugModel *drugModel = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugModel>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "drugModelId")
                drugModel->setDrugModelId(extractor());
            if (name == "pkModelId")
                drugModel->setPkModelId(extractor());
            else if(name == "adme"){
                drugModel->setAdme(loadAdme("adme", drugModel));
                //            }else if(name == "halflife"){
                //                isOk = loadHalflife(drugModel);
                //            }else if(name == "conversions"){
                //                isOk = loadConversions(drugModel);
            }else if(name == "dosages"){

                WHILE_NOT_END_ELEM("dosages") {
                    if(isOk && reader.readNextStartElement()) {
                        QString name = reader.name().toString();
                        if(name == "validDoses") {
                            drugModel->setDoses(loadValidDoses("validDoses", drugModel));
                        } else if(name == "validIntervals"){
                            drugModel->setIntervals(loadValidIntervals("validIntervals", drugModel));
                        } else if(name == "validInfusions"){
                            drugModel->setInfusions(loadValidInfusions("validInfusions", drugModel));
                        }
                    }
                }

            }else if(name == "targets"){
                drugModel->setTargets(loadTargets("targets", drugModel));
            }else if(name == "drugVariates"){
                drugModel->setCovariates(loadCovariates("drugVariates", drugModel));
            }else if(name == "errorModel"){
                drugModel->setErrorModel(loadErrorModel("errorModel", drugModel));
            }else if(name == "parameterSet"){
                drugModel->setParameters(loadParameters("parameterSet", drugModel));
                // }else if(name == "operations"){
                //     drugModel->setOperations(loadOperations("operations", drugModel));
            }
        }
    }

    return drugModel;
}


ezechiel::GuiCore::ADME *InterpretationXmlImport::loadAdme(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::ADME* adme = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ADME>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if (name == "defaultIntake") {
                ezechiel::GuiCore::Admin* def = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO, adme);
                def->setLabel(extractor());
                adme->setDefaultIntake(def);
            }
            else if (name == "intakes") {
                ezechiel::GuiCore::AdminList* intakes = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::AdminList>(ABSTRACTREPO, adme);


                WHILE_NOT_END_ELEM("intakes"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "intake"){
                            ezechiel::GuiCore::Admin* def = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO, adme);
                            def->setLabel(extractor());
                            intakes->append(def);
                        }
                    }
                }

                adme->setIntakes(intakes);
            } else if(name == "distribution"){
                adme->setDistribution(extractor());
            } else if(name == "elimination"){
                adme->setElimination(extractor());
            }
        }
    }

    checkReaderError();

    return adme;
}



ezechiel::GuiCore::ValidDoses *InterpretationXmlImport::loadValidDoses(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::ValidDoses* doses = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidDoses>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if (name == "anyDose")
                doses->setAnyDose(extractBool());
            else if (name == "defaultDose")
                doses->setQuantity(loadIdentifiableAmount("defaultDose", doses));
            else if (name == "doses") {

                WHILE_NOT_END_ELEM("doses"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "dose"){
                            ezechiel::GuiCore::ValidDose* dose = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidDose>(ABSTRACTREPO,doses);
                            WHILE_NOT_END_ELEM("dose") {
                                if(reader.readNextStartElement() ){
                                    name = reader.name().toString();
                                    if (name == "route") {
                                        ezechiel::GuiCore::Admin* admin = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO,dose);
                                        admin->setLabel(extractor());
                                        dose->setRoute(admin);
                                    }
                                    else if (name == "quantity")
                                        dose->setQuantity(loadIdentifiableAmount("quantity", dose));
                                }
                            }
                            doses->append(dose);
                        }
                    }
                }
            } else {
                break;
            }
        }
    }

    checkReaderError();

    return doses;

}

ezechiel::GuiCore::ValidIntervals *InterpretationXmlImport::loadValidIntervals(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::ValidIntervals* intervals = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidIntervals>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if (name == "any")
                intervals->setAny(extractBool());
            else if (name == "defaultInterval")
                intervals->setQuantity(loadIdentifiableAmount("defaultInterval", intervals));
            else if (name == "intervals") {

                WHILE_NOT_END_ELEM("intervals"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "interval"){
                            ezechiel::GuiCore::ValidInterval* interval = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidInterval>(ABSTRACTREPO,intervals);
                            WHILE_NOT_END_ELEM("interval") {
                                if(reader.readNextStartElement() ){
                                    name = reader.name().toString();
                                    if (name == "quantity")
                                        interval->setQuantity(loadIdentifiableAmount("quantity", interval));
                                }
                            }
                            intervals->append(interval);
                        }
                    }
                }
            } else {
                break;
            }
        }
    }

    checkReaderError();

    return intervals;
}

ezechiel::GuiCore::ValidInfusions *InterpretationXmlImport::loadValidInfusions(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::ValidInfusions* infusions = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidInfusions>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if (name == "any")
                infusions->setAny(extractBool());
            else if (name == "defaultInfusion")
                infusions->setQuantity(loadIdentifiableAmount("defaultInfusion", infusions));
            else if (name == "infusions") {

                WHILE_NOT_END_ELEM("infusions"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "infusion"){
                            ezechiel::GuiCore::ValidInfusion* infusion = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidInfusion>(ABSTRACTREPO,infusions);
                            WHILE_NOT_END_ELEM("infusion") {
                                if(reader.readNextStartElement() ){
                                    name = reader.name().toString();
                                    if (name == "quantity")
                                        infusion->setQuantity(loadIdentifiableAmount("quantity", infusion));
                                }
                            }
                            infusions->append(infusion);
                        }
                    }
                }
            } else {
                break;
            }
        }
    }

    checkReaderError();

    return infusions;
}

ezechiel::GuiCore::TargetList *InterpretationXmlImport::loadTargets(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::TargetList* targetSet = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TargetList>(ABSTRACTREPO, parent);
    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() && isOk){
            QString name = reader.name().toString();
            if(name == "target"){
                targetSet->append(loadTarget("target", targetSet));
            }
        }
    }

    checkReaderError();

    return targetSet;
}

ezechiel::GuiCore::Target *InterpretationXmlImport::loadTarget(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::Target* target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Target>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "method"){
                target->getType()->setLabel(extractor());
            } else if(name == "concentrations"){
                QXmlStreamAttributes attributes ;
                extractor(&attributes,false);
                ezechiel::GuiCore::Unit unit;
                WHILE_NOT_END_ELEM("concentrations"){
                    if(reader.readNextStartElement() ){
                        name = reader.name().toString();
                        if(name == "cmin"){
                            target->setCmin(loadOperableAmount("cmin", target));
                        } else if(name == "cmax"){
                            target->setCmax(loadOperableAmount("cmax", target));
                        } else if(name == "cbest"){
                            target->setCbest(loadOperableAmount("cbest", target));
                        }
                    }
                }
            } else if(name == "times"){
                QXmlStreamAttributes attributes ;
                extractor(&attributes,false);
                ezechiel::GuiCore::Unit unit;
                WHILE_NOT_END_ELEM("times"){
                    if(reader.readNextStartElement() ){
                        name = reader.name().toString();
                        if(name == "tmin"){
                            target->setTmin(loadOperableAmount("tmin", target));
                        } else if(name == "tmax"){
                            target->setTmax(loadOperableAmount("tmax", target));
                        } else if(name == "tbest"){
                            target->setTbest(loadOperableAmount("tbest", target));
                        }
                    }
                }
            } else if (name == "mic") {
                target->setMic(loadOperableAmount("mic", target));
            }
        }
    }

    checkReaderError();

    return target;
}

ezechiel::GuiCore::DrugVariateList *InterpretationXmlImport::loadCovariates(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::DrugVariateList *drugVariateSet = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugVariateList>(ABSTRACTREPO, parent);

    /*******************************
     * TODO: Totally wrong down there
     *******************************/
    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement()){
            QString name = reader.name().toString();
            if (name == "drugVariate") {
                drugVariateSet->append(loadCovariate("drugVariate", drugVariateSet));
            }
        }
    }
    checkReaderError();

    return drugVariateSet;
}

ezechiel::GuiCore::DrugVariate *InterpretationXmlImport::loadCovariate(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::DrugVariate* drugVariate = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugVariate>(ABSTRACTREPO, parent);

    ezechiel::GuiCore::TranslatableString *nameTranslation, *descriptionTranslation;

    nameTranslation = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(ABSTRACTREPO,drugVariate);
    descriptionTranslation = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(ABSTRACTREPO,drugVariate);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "name"){
                drugVariate->setCovariateId(extractor());
            } else if (name == "quantity"){
                drugVariate->setQuantity(loadOperableAmount("quantity", drugVariate));
            } else if (name == "type"){
                drugVariate->setType(extractType());
            }
        }
    }

    checkReaderError();

    return drugVariate;
}

ezechiel::GuiCore::ErrorModel *InterpretationXmlImport::loadErrorModel(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::ErrorModel* errorModel = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ErrorModel>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "additive"){
                errorModel->setAdditive(extractDouble());
            } else if (name == "proportional"){
                errorModel->setProportional(extractDouble());
            }
        }
    }

    checkReaderError();

    return errorModel;
}

ezechiel::GuiCore::ParameterSet *InterpretationXmlImport::loadParameters(const QString &tagName, QObject *parent)
{

    ezechiel::GuiCore::ParameterSet* parameterSet = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ParameterSet>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "parameters"){

                WHILE_NOT_END_ELEM("parameters"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "parameter"){
                            parameterSet->set(loadParameter("parameter", parameterSet));
                        }
                    }
                }

            }
            else if (name == "correlations") {

                QString param1, param2;
                double value;
                WHILE_NOT_END_ELEM("correlations"){
                    if(isOk && reader.readNextStartElement()  && reader.name() == "correlation"){
                        WHILE_NOT_END_ELEM("correlation"){
                            if(reader.readNextStartElement() ){
                                QString name = reader.name().toString();
                                if(name == "param1"){
                                    param1 = extractor();
                                } else if (name == "param2"){
                                    param2 = extractor();
                                } else if(name == "value"){
                                    value = extractDouble();
                                }
                            }
                        }
                        //check and load
                        if(isOk){
                            parameterSet->setCorrelation(param1,param2,value);
                        }

                    }
                }

            }
            else if (name =="time") {
                /*****************************************
                 * TODO: Check this. It seems to be empty
                 ****************************************/
                // parameterSet->setTime(extractDate());
            }
        }
    }

    checkReaderError();

    return parameterSet;
}

ezechiel::GuiCore::Parameter *InterpretationXmlImport::loadParameter(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::Parameter* parameter = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Parameter>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "name"){
                parameter->setName(extractor());
            } else if (name == "unit") {//FIXME unkown unit in fileout
                QString extract = extractor();
                ezechiel::GuiCore::Unit unit(extract);
                bool isValid = unit.isValid();
                parameter->getQuantity()->setUnit(extract);
            } else if (name == "value") {
                parameter->getQuantity()->setValue(extractDouble());
            } else if (name == "bsv") {
                parameter->setBsv(loadBsv("bsv", parameter));
            } else if (name == "min") {
                parameter->setMin(extractDouble());
            } else if (name == "max") {
                parameter->setMax(extractDouble());
            } else if (name == "step") {//FIXME crazy steps in fileout
                parameter->setStep(extractDouble());
            }
            else if (name == "operations") {
                parameter->setOperations(loadOperations("operations", parameter));
            }
        }
    }

    checkReaderError();

    return parameter;
}

ezechiel::GuiCore::OperationList *InterpretationXmlImport::loadOperations(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::OperationList *operations = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::OperationList>(ABSTRACTREPO,parent);

    QMap<QString,ezechiel::GuiCore::OperationList*> mapOperationLists;

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "operation"){
                ezechiel::GuiCore::Operation* operation = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Operation>(ABSTRACTREPO);
                WHILE_NOT_END_ELEM("operation"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "type"){
                            QString t = extractor();
                            if (t == "Hardcoded")
                                operation->setType(ezechiel::GuiCore::OperationType::HARDCODED);
                            else if (t == "Imported")
                                operation->setType(ezechiel::GuiCore::OperationType::IMPORTED);
                            else
                                // TODO: There is an error. It should be reported
                                operation->setType(ezechiel::GuiCore::OperationType::NOOP);
                        }
                        else if(name == "formula"){
                            QString formula = extractor();
                            operation->setFormula(formula);
                        }
                    }
                }
                if (isOk)
                    operations->append(operation);
            }
        }
    }

    checkReaderError();

    return operations;
}


ezechiel::GuiCore::Bsv *InterpretationXmlImport::loadBsv(const QString &tagName, QObject *parent)
{
    ezechiel::GuiCore::Bsv* bsv = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Bsv>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(isOk && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "additive"){
                bsv->setStandard(extractDouble());
            } else if ( name == "proportional"){
                bsv->setProportional(extractDouble());
            }
        }
    }


    checkReaderError();

    return bsv;
}


QString InterpretationXmlImport::extractor(QXmlStreamAttributes *attributes,bool readText)
{
    QString data;
    if(attributes != nullptr){
        *attributes = reader.attributes();
    }
    if(readText){
        data = reader.readElementText();
    }
    return data;
}

QMetaType::Type InterpretationXmlImport::extractType()
{
    QString s = extractor();
    bool isConvertible = true;
    QMetaType::Type t = qStringToType(s, isConvertible);
    checkValidity(isConvertible);
    return t;
}

QMetaType::Type InterpretationXmlImport::qStringToType(QString stringType,bool &isConvertible)
{//FIXME how many different types are there
    QMetaType::Type type;
    if(stringType == "double"){
        type = QMetaType::Type::Double;
    } else if(stringType == "int"){
        type = QMetaType::Type::Int;
    } else if (stringType == "bool"){
        type = QMetaType::Type::Bool;
    }else{
        isConvertible = false;
    }

    return type;
}


QString InterpretationXmlImport::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(reader.errorString())
            .arg(reader.lineNumber())
            .arg(reader.columnNumber());
}

void InterpretationXmlImport::raiseConversionError()
{
    reader.raiseError("syntaxe error, probably text instead of double");
}


ezechiel::GuiCore::IdentifiableAmount * InterpretationXmlImport::loadIdentifiableAmount(const QString &tagName, QObject *parent)
{

    ezechiel::GuiCore::IdentifiableAmount* amount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::IdentifiableAmount>(ABSTRACTREPO, parent);
    QString parameter;
    WHILE_NOT_END_ELEM(tagName){
        if(isOk && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "amountId"){
                amount->setAmountId(extractor());
            }else if(name == "value"){
                amount->setDbvalue(extractDouble());
            } else if (name == "unit"){
                ezechiel::GuiCore::Unit unit(extractor());
                checkValidity(unit.isValid());
                amount->setUnit(unit);
            }
        }
    }

    checkReaderError();

    return amount;
}

ezechiel::GuiCore::OperableAmount *InterpretationXmlImport::loadOperableAmount(const QString &tagName, QObject *parent)
{

    ezechiel::GuiCore::OperableAmount* amount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::OperableAmount>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){
        if(isOk && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "amountId"){
                amount->setAmountId(extractor());
            }else if(name == "value"){
                amount->setDbvalue(extractDouble());
            } else if (name == "unit"){
                ezechiel::GuiCore::Unit unit(extractor());
                checkValidity(unit.isValid());
                amount->setUnit(unit);
            }
            else if (name == "operations") {
                amount->setOperations(loadOperations("operations", amount));
            }
        }
    }

    checkReaderError();

    return amount;
}
