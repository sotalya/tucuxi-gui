//@@license@@

#include "importaction.h"
#include "core/utils/xmlvalidator.h"
#include "core/definitionmanager.h"
#include "core/core.h"
//#include "core/interfaces/database.h"
#include "core/definitionmanager.h"
#include "apputils/src/drugmanager.h"
#include "core/pluginmanager.h"
#include "core/dal/drug/drug.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/practician.h"
#include "apputils/src/appcore.h"
#include "admin/src/dal/measure.h"
#include "core/corefactory.h"
#include "admin/src/stdadminrepository.h"
#include "admin/src/adminfactory.h"
#include "core/corerepository.h"
#include "admin/src/adminfactory.h"
#include "climanager.h"
#include "core/utils/logging.h"
#include "errors_cliutils.h"

#include "core/dal/drug/infusions.h"

#include <QDomDocument>

namespace ezechiel {
namespace cliutils {
//The error messages
const char *const ImportAction::_NODE_LOCATION          = QT_TR_NOOP_UTF8("(line '%1', column '%2')");
const char *const ImportAction::_INVALID_DATASET_STRUCT = QT_TR_NOOP_UTF8("The dataset no. %1 (line '%2', column '%3') is invalid, ignoring...");
const char *const ImportAction::_INVALID_DATASET_DATA   = QT_TR_NOOP_UTF8("Failed to process the dataset no. %1 %2 data, ignoring...");
const char *const ImportAction::_INVALID_ENGINE         = QT_TR_NOOP_UTF8("Could not find any %1");
const char *const ImportAction::_INVALID_CURVE          = QT_TR_NOOP_UTF8("An error occured while setting the curve %1 for patient with external ID '%2'");
const char *const ImportAction::_MISSING_TAG            = QT_TR_NOOP_UTF8("Could not find the tag '%1'");
const char *const ImportAction::_MISSING_DATA           = QT_TR_NOOP_UTF8("The '%1' could not be found (line '%2', column '%3')");

//The action commands
const char *const ImportAction::_ACTION_COMMAND = "import";

//The curve name template
const char *const ImportAction::_CURVE_NAME  = "%1 %2 [%3/%4]";
const char *const ImportAction::_CURVE_REGEX = "^.*\\[%1/%2\\]$";

QString ImportAction::name()
{
    return _ACTION_COMMAND;
}

QString ImportAction::help()
{
    QString str;
    str = tr("   NAME") + "\n" +
          tr("       EzeCHieL Import Command") + "\n" + "\n" +
          tr("   SYNOPSIS") + "\n" +
             "       " + QString("ezechiel-cli [OPTIONS] %1 %2USERNAME %3PASSWORD PATH").arg(_ACTION_COMMAND, _CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD) + "\n" + "\n" +
          tr("   DESCRIPTION") + "\n" +
          tr("       The EzeCHieL Import Command can be used to import data from a valid import XML file. The PATH of the file can be either absolute, or relative to the application folder.") + "\n" + "\n" +
          "\n";

    return str;
}

ImportAction::ImportAction()
{

}

bool ImportAction::run(const QStringList &args)
{
    QStringList _args = args;
    CliCommand _cmd(_args);
    QDomElement root;

    //switch if want remote (mirth) or local import


    //Initialize the action
    if (!initializeAction(args))
        return false;

    //Initialize the root
    if (!initializeRoot(args, root))
        return false;

    //Extract the data sets
    if (!extractDatasets(root))
        return false;

    //Process the data sets
    if (!processDatasets())
        return false;


    if (args.size() > 1) {
        //we have a possible command after the import command
        //remove the first argument which has the filename
        _cmd.actionName = _cmd.actionArgs.first();
        _cmd.actionArgs.pop_front();
        //we want to call the cli again with this new argument list
        bool _actionIsSuccess = CliManager::selectAction(&_cmd, true);
    }

    return true;
}

bool ImportAction::initializeAction(const QStringList &args)
{
    //Clear the previous errors
    _errors.clear();

    //Check if the path is present
    if (args.isEmpty()) {
        _errors = tr("Missing path to the import XML file\n");
        return false;
    }

    //Check if too many arguments
    //commenting this out in order to add import + execute
//    if (args.size() > 1) {
//        _errors = tr("Incorrect number of parameters given\n");
//        return false;
//    }

    return true;
}

bool ImportAction::initializeRoot(const QStringList &args, QDomElement &root)
{
    QString fileName = args[0];

    //Create the file
    QFile file(fileName);

    //Open the file
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        _errors = tr("Could not open the exchange file '%1'\n").arg(fileName);
        return false;
    }

    //Validate the file
    if (!XMLVALIDATOR->validate(fileName, XmlValidator::Import)) {
        _errors = tr(_INVALID_XML_FILE).arg(fileName, QString::number(XMLVALIDATOR->errorLine()), XMLVALIDATOR->errorMessage());
        return false;
    }

    //Set the document
    QDomDocument document;
    if (!document.setContent(&file, &_errors))
        return false;

    //Get the root element
    if ((root = document.firstChildElement(Document::_TAG_ROOT)).isNull()) {
        _errors = tr(_MISSING_TAG).arg(Document::_TAG_ROOT);
        return false;
    }

    return true;
}

bool ImportAction::extractDatasets(const QDomElement &root)
{
    int datasetNbr = 0;

    //Get the first element
    QDomElement datasetNode = root.firstChildElement(Dataset::_TAG_DATASET);

    //Add each element
    while (!datasetNode.isNull()) {
        Dataset dataset;

        //Display current set
//        MESSAGE(tr("Parsing data set no. %1...").arg(QString::number(++datasetNbr)));

        //Extract the language
        dataset.setLang(datasetNode.attribute(_ATTRIBUTE_LANG));
        if (dataset.lang().isEmpty()) {
            LOG(QtWarningMsg, NOEZERROR, tr("The language of the dataset no. %1 (line '%2', column '%3') is invalid or unknown, ignoring...")
                .arg(QString::number(datasetNbr), QString::number(datasetNode.lineNumber()), QString::number(datasetNode.columnNumber())));
            datasetNode = datasetNode.nextSiblingElement(Dataset::_TAG_DATASET);
            continue;
        }

        //Extract the drug data
        if (!extractDrug(datasetNode, dataset)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_STRUCT).arg(QString::number(datasetNbr), QString::number(datasetNode.lineNumber()), QString::number(datasetNode.columnNumber())));
            datasetNode = datasetNode.nextSiblingElement(Dataset::_TAG_DATASET);
            continue;
        }

        //Extract the dosages data
        if (!extractDosages(datasetNode, dataset)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_STRUCT).arg(QString::number(datasetNbr), QString::number(datasetNode.lineNumber()), QString::number(datasetNode.columnNumber())));
            datasetNode = datasetNode.nextSiblingElement(Dataset::_TAG_DATASET);
            continue;
        }

        //Extract the samples data
        if (!extractSamples(datasetNode, dataset)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_STRUCT).arg(QString::number(datasetNbr), QString::number(datasetNode.lineNumber()), QString::number(datasetNode.columnNumber())));
            datasetNode = datasetNode.nextSiblingElement(Dataset::_TAG_DATASET);
            continue;
        }

        //Extract the covariates data
        if (!extractCovariates(datasetNode, dataset)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_STRUCT).arg(QString::number(datasetNbr), QString::number(datasetNode.lineNumber()), QString::number(datasetNode.columnNumber())));
            datasetNode = datasetNode.nextSiblingElement(Dataset::_TAG_DATASET);
            continue;
        }

        //Extract the clinicals data
        if (!extractClinicals(datasetNode, dataset)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_STRUCT).arg(QString::number(datasetNbr), QString::number(datasetNode.lineNumber()), QString::number(datasetNode.columnNumber())));
            datasetNode = datasetNode.nextSiblingElement(Dataset::_TAG_DATASET);
            continue;
        }

        //Extract the patient
        if (!extractPatient(datasetNode, dataset)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_STRUCT).arg(QString::number(datasetNbr), QString::number(datasetNode.lineNumber()), QString::number(datasetNode.columnNumber())));
            datasetNode = datasetNode.nextSiblingElement(Dataset::_TAG_DATASET);
            continue;
        }

        //Extract the mandator
        if (!extractMandator(datasetNode, dataset)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_STRUCT).arg(QString::number(datasetNbr), QString::number(datasetNode.lineNumber()), QString::number(datasetNode.columnNumber())));
            datasetNode = datasetNode.nextSiblingElement(Dataset::_TAG_DATASET);
            continue;
        }

        //Extract the predictions
//        if (!extractPredictions(datasetNode, dataset)) {
//            WARNING(tr(_INVALID_DATASET_STRUCT).arg(QString::number(datasetNbr), QString::number(datasetNode.lineNumber()), QString::number(datasetNode.columnNumber())));
//            datasetNode = datasetNode.nextSiblingElement(Dataset::_TAG_DATASET);
//            continue;
//        }

        //Extract the drugresponses
        if (!extractDrugResponses(datasetNode, dataset)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_STRUCT).arg(QString::number(datasetNbr), QString::number(datasetNode.lineNumber()), QString::number(datasetNode.columnNumber())));
            datasetNode = datasetNode.nextSiblingElement(Dataset::_TAG_DATASET);
            continue;
        }


        //Add the data set
        _datasets.append(dataset);

        //Get the next element
        datasetNode = datasetNode.nextSiblingElement(Dataset::_TAG_DATASET);
    }

    //Check the data sets
    if (_datasets.isEmpty()) {
        _errors = tr("Could not find any valid data set in the import file");
        return false;
    }

    return true;
}


bool ImportAction::extractPredictions(const QDomElement &root, Dataset &dataset)
{
    PredictionData prediction;
    QDomElement predictionNode = root.firstChildElement(PredictionData::_TAG_PREDICTION);

    while (!predictionNode.isNull())
    {
        prediction.setName(predictionNode.firstChildElement(PredictionData::_TAG_NAME).firstChild().toText().data());
        prediction.setComment(predictionNode.firstChildElement(PredictionData::_TAG_COMMENT).firstChild().toText().data());
        prediction.setCurveType(predictionNode.firstChildElement(PredictionData::_TAG_CURVETYPE).firstChild().toText().data());
        prediction.setParamsType(predictionNode.firstChildElement(PredictionData::_TAG_PARAMSTYPE).firstChild().toText().data());
        prediction.setEnddate(predictionNode.firstChildElement(PredictionData::_TAG_ENDDATE).firstChild().toText().data());
        prediction.setStartdate(predictionNode.firstChildElement(PredictionData::_TAG_STARTDATE).firstChild().toText().data());
        prediction.setNbPoints(predictionNode.firstChildElement(PredictionData::_TAG_NBPOINTS).firstChild().toText().data());
        prediction.setSteadyState(predictionNode.firstChildElement(PredictionData::_TAG_STEADYSTATE).firstChild().toText().data());
        dataset.addPrediction(prediction);
        predictionNode = root.nextSiblingElement(PredictionData::_TAG_PREDICTION);
    }
    return true;
}

bool ImportAction::extractDrugResponses(const QDomElement &root, Dataset &dataset)
{
    QList<DrugResponseAnalysisData> _resps;
    DrugResponseAnalysisData response;
    QDomElement responseNode = root.firstChildElement(DrugResponseAnalysisData::_TAG_DRUGRESPONSEANALYSIS);

    //Check if ignored
    while (!responseNode.isNull()) {
        response.setDrugTreatmentName(responseNode.firstChildElement(DrugResponseAnalysisData::_TAG_DRUGTREATMENTNAME).firstChild().toText().data());
        response.setDrugModelName(responseNode.firstChildElement(DrugResponseAnalysisData::_TAG_DRUGMODELNAME).firstChild().toText().data());
        response.setPercentileEngineName(responseNode.firstChildElement(DrugResponseAnalysisData::_TAG_PERCENTILEENGINENAME).firstChild().toText().data());
        response.setPercentileEngineVersion(responseNode.firstChildElement(DrugResponseAnalysisData::_TAG_PERCENTILEENGINEVERSION).firstChild().toText().data());
        response.setPostEngineName(responseNode.firstChildElement(DrugResponseAnalysisData::_TAG_POSTENGINENAME).firstChild().toText().data());
        response.setPostEngineVersion(responseNode.firstChildElement(DrugResponseAnalysisData::_TAG_POSTENGINEVERSION).firstChild().toText().data());
        response.setReverseEngineName(responseNode.firstChildElement(DrugResponseAnalysisData::_TAG_REVERSEENGINENAME).firstChild().toText().data());
        response.setReverseEngineVersion(responseNode.firstChildElement(DrugResponseAnalysisData::_TAG_REVERSEENGINEVERSION).firstChild().toText().data());
        if (!extractPredictions(responseNode, dataset)) {
//            WARNING(tr(_INVALID_DATASET_STRUCT).arg(QString::number(datasetNbr), QString::number(datasetNode.lineNumber()), QString::number(datasetNode.columnNumber())));
            responseNode = responseNode.nextSiblingElement(Dataset::_TAG_DATASET);
            continue;
        }
        _resps.append(response);
        responseNode = root.nextSiblingElement(DrugResponseAnalysisData::_TAG_DRUGRESPONSEANALYSIS);
    }
    dataset.setResponses(_resps);
    return true;
}

bool ImportAction::extractDrug(const QDomElement &root, Dataset &dataset)
{
    DrugData drug;

    //Get the drug element
    QDomElement drugNode = root.firstChildElement(DrugData::_TAG_DRUG);

    //Check if ignored
    if (drugNode.isNull())
        return true;

    //Extract the data
    drug.setId(drugNode.firstChildElement(DrugData::_TAG_ID).firstChild().toText().data());
    drug.setAtc(drugNode.firstChildElement(DrugData::_TAG_ATC).firstChild().toText().data());
    drug.setBrandName(drugNode.firstChildElement(DrugData::_TAG_BRAND).firstChild().toText().data());
    drug.setActivePrinciple(drugNode.firstChildElement(DrugData::_TAG_PRINCIPLE).firstChild().toText().data());

    //Check the data
    if (drug.id().isEmpty() && drug.atc().isEmpty()) {
        LOG(QtWarningMsg, NOEZERROR, tr(_MISSING_DATA).arg(tr("drug ID or ATC"), QString::number(drugNode.lineNumber()), QString::number(drugNode.columnNumber())));
        return false;
    }

    //Set the drug
    dataset.setDrug(drug);

    return true;
}

bool ImportAction::extractDosages(const QDomElement &root, Dataset &dataset)
{
    bool intervalOk, infusionOk, valueOk;

    //Get the first element
    QDomElement dosageNode = root.firstChildElement(DosageData::_TAG_DOSAGES).firstChildElement(DosageData::_TAG_DOSAGE);

    //Add each element
    while (!dosageNode.isNull()) {
        DosageData dosage;

        //Extract the data
        dosage.setStartDate(QDateTime::fromString(dosageNode.firstChildElement(DosageData::_TAG_START_DATE).firstChild().toText().data(), Action::xmlDateTimeFormat()));
        dosage.setLastDate(QDateTime::fromString(dosageNode.firstChildElement(DosageData::_TAG_LAST_DATE).firstChild().toText().data(), Action::xmlDateTimeFormat()));
        dosage.setInterval(Duration(0, 0, 0, dosageNode.firstChildElement(DosageData::_TAG_INTERVAL).firstChildElement(_TAG_VALUE).firstChild().toText().data().toInt(&intervalOk) * _FROM_HOURS));
        dosage.setInfusion(Duration(0, 0, 0, dosageNode.firstChildElement(DosageData::_TAG_INFUSION).firstChildElement(_TAG_VALUE).firstChild().toText().data().toInt(&infusionOk) * _FROM_MINUTES));
        dosage.setIntake(dosageNode.firstChildElement(DosageData::_TAG_INTAKE).firstChild().toText().data());

        double value = dosageNode.firstChildElement(DosageData::_TAG_DOSE).firstChildElement(_TAG_VALUE).firstChild().toText().data().toDouble(&valueOk);
        QString unit = dosageNode.firstChildElement(DosageData::_TAG_DOSE).firstChildElement(_TAG_UNIT).firstChild().toText().data();
        dosage.setDose(Amount(value, Unit(unit)));

        //Check the data
        if (!dosage.startDate().isValid() || !dosage.lastDate().isValid() || dosage.interval().isEmpty() || !dosage.dose().isValid() || !intervalOk || !infusionOk || !valueOk)
        {
            LOG(QtDebugMsg, NOEZERROR, tr("\tIgnoring invalid dosage") + " " + tr(_NODE_LOCATION).arg(QString::number(dosageNode.lineNumber()), QString::number(dosageNode.columnNumber())) + "...");
        }
        else
            dataset.addDosage(dosage);

        //Get the next element
        dosageNode = dosageNode.nextSiblingElement(DosageData::_TAG_DOSAGE);
    }

    return true;
}

bool ImportAction::extractSamples(const QDomElement &root, Dataset &dataset)
{
    bool valueOk;

    //Get the first element
    QDomElement sampleNode = root.firstChildElement(SampleData::_TAG_SAMPLES).firstChildElement(SampleData::_TAG_SAMPLE);

    //Add each element
    while (!sampleNode.isNull()) {
        SampleData sample;

        //Extract the data
        sample.setId(sampleNode.firstChildElement(SampleData::_TAG_ID).firstChild().toText().data());
        sample.setAnalyte(sampleNode.firstChildElement(SampleData::_TAG_ANALYTE).firstChild().toText().data());
        sample.setSampleDate(QDateTime::fromString(sampleNode.firstChildElement(SampleData::_TAG_SAMPLE_DATE).firstChild().toText().data(), Action::xmlDateTimeFormat()));
        sample.setArrivalDate(QDateTime::fromString(sampleNode.firstChildElement(SampleData::_TAG_ARRIVAL_DATE).firstChild().toText().data(), Action::xmlDateTimeFormat()));

        double value = sampleNode.firstChildElement(SampleData::_TAG_CONCENTRATION).firstChildElement(_TAG_VALUE).firstChild().toText().data().toDouble(&valueOk);
        QString unit = sampleNode.firstChildElement(SampleData::_TAG_CONCENTRATION).firstChildElement(_TAG_UNIT).firstChild().toText().data();
        sample.setConcentration(Amount(value, Unit(unit)));

        //Check the data
        if (!sample.sampleDate().isValid() || !sample.concentration().isValid() || !valueOk)
        {
            LOG(QtDebugMsg, NOEZERROR, tr("\tIgnoring invalid sample") + " " + tr(_NODE_LOCATION).arg(QString::number(sampleNode.lineNumber()), QString::number(sampleNode.columnNumber())) + "...");
        } else
            dataset.addSample(sample);

        //Get the next element
        sampleNode = sampleNode.nextSiblingElement(SampleData::_TAG_SAMPLE);
    }

    return true;
}

bool ImportAction::extractCovariates(const QDomElement &root, Dataset &dataset)
{
    bool valueOk;

    //Get the first element
    QDomElement covariateNode = root.firstChildElement(CovariateData::_TAG_COVARIATES).firstChildElement(CovariateData::_TAG_COVARIATE);

    //Add each element
    while (!covariateNode.isNull()) {
        CovariateData covariate;

        //Extract the data
        covariate.setName(covariateNode.firstChildElement(CovariateData::_TAG_NAME).firstChild().toText().data());
        covariate.setNature(covariateNode.firstChildElement(CovariateData::_TAG_NATURE).firstChild().toText().data());
        covariate.setDate(QDateTime::fromString(covariateNode.firstChildElement(CovariateData::_TAG_DATE).firstChild().toText().data(), Action::xmlDateTimeFormat()));
        covariate.setValue(covariateNode.firstChildElement(CovariateData::_TAG_VALUE).firstChildElement(_TAG_VALUE).firstChild().toText().data().toDouble(&valueOk));
        covariate.setUnit(Unit(covariateNode.firstChildElement(CovariateData::_TAG_VALUE).firstChildElement(_TAG_UNIT).firstChild().toText().data()));

        //Check the data
        if (covariate.name().isEmpty() || !covariate.date().isValid() || !covariate.unit().isValid() || !valueOk)
        {
            LOG(QtDebugMsg, NOEZERROR, tr("\tIgnoring invalid covariate") + " " + tr(_NODE_LOCATION).arg(QString::number(covariateNode.lineNumber()), QString::number(covariateNode.columnNumber())) + "...");
        } else
            dataset.addCovariate(covariate);

        //Get the next element
        covariateNode = covariateNode.nextSiblingElement(CovariateData::_TAG_COVARIATE);
    }

    return true;
}

bool ImportAction::extractClinicals(const QDomElement &root, Dataset &dataset)
{
    //Get the first element
    QDomElement clinicalNode = root.firstChildElement(ClinicalData::_TAG_CLINICALS).firstChildElement(ClinicalData::_TAG_CLINICAL);

    //Add each element
    while (!clinicalNode.isNull()) {
        ClinicalData clinical;

        //Extract the data
        clinical.setName(clinicalNode.firstChildElement(ClinicalData::_TAG_NAME).firstChild().toText().data());
        clinical.setValue(clinicalNode.firstChildElement(ClinicalData::_TAG_VALUE).firstChild().toText().data());
        clinical.setDate(QDateTime::fromString(clinicalNode.firstChildElement(ClinicalData::_TAG_DATE).firstChild().toText().data(), Action::xmlDateTimeFormat()));

        //Check the data
        if (clinical.name().isEmpty() || !clinical.date().isValid())
        {
            LOG(QtDebugMsg, NOEZERROR, tr("\tIgnoring invalid clinical") + " " + tr(_NODE_LOCATION).arg(QString::number(clinicalNode.lineNumber()), QString::number(clinicalNode.columnNumber())) + "...");
        } else
            dataset.addClinical(clinical);

        //Get the next element
        clinicalNode = clinicalNode.nextSiblingElement(ClinicalData::_TAG_CLINICAL);
    }

    return true;
}

bool ImportAction::extractPatient(const QDomElement &root, Dataset &dataset)
{
    PatientData patient;

    //Get the patient element
    QDomElement patientNode = root.firstChildElement(PatientData::_TAG_PATIENT);

    //Extract the data
    patient.setId(patientNode.firstChildElement(PatientData::_TAG_ID).firstChild().toText().data());
    patient.setStayNumber(patientNode.firstChildElement(PatientData::_TAG_STAY_NUMBER).firstChild().toText().data());
    patient.setBirthdate(QDateTime::fromString(patientNode.firstChildElement(PatientData::_TAG_BIRTHDATE).firstChild().toText().data(), Action::xmlDateTimeFormat()));
    patient.setGender(patientNode.firstChildElement(PatientData::_TAG_GENDER).firstChild().toText().data());

    //Check the data
    if (patient.id().isEmpty() || patient.gender().isEmpty() || !patient.birthdate().isValid()) {
        LOG(QtWarningMsg, NOEZERROR, tr(_MISSING_DATA).arg(tr("patient data"), QString::number(patientNode.lineNumber()), QString::number(patientNode.columnNumber())));
        return false;
    }

    //Extract the name
    NameData name;
    if (!extractName(patientNode, name))
        return false;
    patient.setName(name);

    //Extract the contact
    ContactData contact;
    if (!extractContact(patientNode, contact))
        return false;
    patient.setContact(contact);

    //Extract the institute
    InstituteData institute;
    if (!extractInstitute(patientNode, institute))
        return false;
    patient.setInstitute(institute);

    //Set the patient
    dataset.setPatient(patient);

    return true;
}

bool ImportAction::extractMandator(const QDomElement &root, Dataset &dataset)
{
    PracticianData mandator;

    //Get the mandator element
    QDomElement mandatorNode = root.firstChildElement(PracticianData::_TAG_MANDATOR);

    //Check if ignored
    if (mandatorNode.isNull())
        return true;

    //Extract the data
    mandator.setId(mandatorNode.firstChildElement(PracticianData::_TAG_ID).firstChild().toText().data());
    mandator.setTitle(mandatorNode.firstChildElement(PracticianData::_TAG_TITLE).firstChild().toText().data());
    mandator.setBirthdate(QDateTime::fromString(mandatorNode.firstChildElement(PracticianData::_TAG_BIRTHDATE).firstChild().toText().data(), Action::xmlDateTimeFormat()));
    mandator.setGender(mandatorNode.firstChildElement(PracticianData::_TAG_GENDER).firstChild().toText().data());

    //Check the data
    if (mandator.id().isEmpty()) {
        LOG(QtWarningMsg, NOEZERROR, tr(_MISSING_DATA).arg(tr("mandator ID"), QString::number(mandatorNode.lineNumber()), QString::number(mandatorNode.columnNumber())));
        return false;
    }

    //Extract the name
    NameData name;
    if (!extractName(mandatorNode, name))
        return false;
    mandator.setName(name);

    //Extract the contact
    ContactData contact;
    if (!extractContact(mandatorNode, contact))
        return false;
    mandator.setContact(contact);

    //Extract the institute
    InstituteData institute;
    if (!extractInstitute(mandatorNode, institute))
        return false;
    mandator.setInstitute(institute);

    //Set the mandator
    mandator.setIgnored(false);
    dataset.setMandator(mandator);

    return true;
}

bool ImportAction::extractName(const QDomElement &root, ImportAction::NameData &name)
{
    QDomElement nameNode = root.firstChildElement(NameData::_TAG_NAME);

    //Extract the data
    name.setFirstname(nameNode.firstChildElement(NameData::_TAG_FIRST_NAME).firstChild().toText().data());
    name.setMiddlename(nameNode.firstChildElement(NameData::_TAG_MIDDLE_NAME).firstChild().toText().data());
    name.setLastname(nameNode.firstChildElement(NameData::_TAG_LAST_NAME).firstChild().toText().data());

    //Check the data
    if (name.firstname().isEmpty() || name.lastname().isEmpty()) {
        LOG(QtWarningMsg, NOEZERROR, tr(_MISSING_DATA).arg(tr("name"), QString::number(nameNode.lineNumber()), QString::number(nameNode.columnNumber())));
        return false;
    }

    return true;
}

bool ImportAction::extractInstitute(const QDomElement &root, ImportAction::InstituteData &institute)
{
    QDomElement instituteNode = root.firstChildElement(InstituteData::_TAG_INSTITUTE);

    //Check if ignored
    if (instituteNode.isNull())
        return true;

    //Extract the data
    institute.setId(instituteNode.firstChildElement(InstituteData::_TAG_ID).firstChild().toText().data());
    institute.setName(instituteNode.firstChildElement(InstituteData::_TAG_NAME).firstChild().toText().data());

    //Check the data
    if (institute.id().isNull() || institute.name().isNull()) {
        LOG(QtWarningMsg, NOEZERROR, tr(_MISSING_DATA).arg(tr("institute ID or name"), QString::number(instituteNode.lineNumber()), QString::number(instituteNode.columnNumber())));
        return false;
    }

    //Extract the contact
    ContactData contact;
    if (!extractContact(instituteNode, contact))
        return false;
    institute.setContact(contact);

    //Enable the institute
    institute.setIgnored(false);

    return true;
}

bool ImportAction::extractContact(const QDomElement &root, ImportAction::ContactData &contact)
{
    QDomElement contactNode = root.firstChildElement(ContactData::_TAG_CONTACT);

    //Check if ignored
    if (contactNode.isNull())
        return true;

    //Extract the data
    contact.setAddress(contactNode.firstChildElement(ContactData::_TAG_ADDRESS).firstChild().toText().data());
    contact.setCity(contactNode.firstChildElement(ContactData::_TAG_CITY).firstChild().toText().data());
    contact.setPostcode(contactNode.firstChildElement(ContactData::_TAG_POSTCODE).firstChild().toText().data());
    contact.setState(contactNode.firstChildElement(ContactData::_TAG_STATE).firstChild().toText().data());
    contact.setCountry(contactNode.firstChildElement(ContactData::_TAG_COUNTRY).firstChild().toText().data());

    //Extract the emails
    QList<EmailData> emails;
    if (!extractEmails(contactNode, emails))
        return false;
    contact.setEmails(emails);

    //Extract the phones
    QList<PhoneData> phones;
    if (!extractPhones(contactNode, phones))
        return false;
    contact.setPhones(phones);

    //Enable the contact
    contact.setIgnored(false);

    return true;
}

bool ImportAction::extractEmails(const QDomElement &root, QList<ImportAction::EmailData> &emails)
{
    QDomElement emailNode = root.firstChildElement(EmailData::_TAG_EMAILS).firstChildElement(EmailData::_TAG_EMAIL);

    //Add each element
    while (!emailNode.isNull()) {
        EmailData email;

        //Extract the data
        email.setEmail(emailNode.firstChild().toText().data());
        email.setType(emailNode.attribute(EmailData::_ATTRIBUTE_TYPE));

        //Check the data
        if (email.email().isEmpty() || email.type().isEmpty())
        {
            LOG(QtDebugMsg, NOEZERROR, tr("\tIgnoring invalid email address") + " " + tr(_NODE_LOCATION).arg(QString::number(emailNode.lineNumber()), QString::number(emailNode.columnNumber())) + "...");
        } else
            emails.append(email);

        //Get the next element
        emailNode = emailNode.nextSiblingElement(EmailData::_TAG_EMAIL);
    }

    return true;
}

bool ImportAction::extractPhones(const QDomElement &root, QList<ImportAction::PhoneData> &phones)
{
    QDomElement phoneNode = root.firstChildElement(PhoneData::_TAG_PHONES).firstChildElement(PhoneData::_TAG_PHONE);

    //Add each element
    while (!phoneNode.isNull()) {
        PhoneData phone;

        //Extract the data
        phone.setNumber(phoneNode.firstChild().toText().data());
        phone.setType(phoneNode.attribute(PhoneData::_ATTRIBUTE_TYPE));

        //Check the data
        if (phone.number().isEmpty() || phone.type().isEmpty())
        {
            LOG(QtDebugMsg, NOEZERROR, tr("\tIgnoring invalid phone number") + " " + tr(_NODE_LOCATION).arg(QString::number(phoneNode.lineNumber()), QString::number(phoneNode.columnNumber())) + "...");
        } else
            phones.append(phone);

        //Get the next element
        phoneNode = phoneNode.nextSiblingElement(PhoneData::_TAG_PHONE);
    }

    return true;
}

bool ImportAction::processDatasets()
{
    QList<Dataset>::const_iterator i;
    int datasetNbr = 0;

    //Process each datasets
    for (i = _datasets.constBegin(); i != _datasets.constEnd(); i++) {

        //Display current set
//        MESSAGE(tr("Processing data set no. %1...").arg(QString::number(++datasetNbr)));

        //Process the mandator
        if (!processMandator(*i)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_DATA).arg(QString::number(datasetNbr), tr("mandator")));
            continue;
        }

        //Process the patient
        SharedPatient patient = processPatient(*i);
        if (!patient) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_DATA).arg(QString::number(datasetNbr), tr("patient")));
            continue;
        }

        //Process the drug
        DrugModel* drug = processDrug(*i);
        if (!drug) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_DATA).arg(QString::number(datasetNbr), tr("drug")));
            continue;
        }

        //Process the covariates
        if (!processCovariates(*i, patient)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_DATA).arg(QString::number(datasetNbr), tr("covariates")));
            continue;
        }

        //Process the clinicals
        if (!processClinicals(*i, patient)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_DATA).arg(QString::number(datasetNbr), tr("clinicals")));
            continue;
        }

        DrugResponseAnalysis* resp = processResponses(*i, patient, drug);

        if (!resp) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_DATA).arg(QString::number(datasetNbr), tr("drugresponseanalysis")));
            continue;
        }

        //Process the predictions
        if (!processPredictions(*i, resp)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_DATA).arg(QString::number(datasetNbr), tr("predictions")));
            continue;
        }

        //Process the dosages
        if (!processDosages(*i, resp)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_DATA).arg(QString::number(datasetNbr), tr("dosages")));
            continue;
        }

        //Process the samples
        if (!processSamples(*i, resp)) {
            LOG(QtWarningMsg, NOEZERROR, tr(_INVALID_DATASET_DATA).arg(QString::number(datasetNbr), tr("measures")));
            continue;
        }
    }

    return true;
}


DrugResponseAnalysis* ImportAction::processResponses(const Dataset &dataset, SharedPatient patient, DrugModel* drug) {

    if (dataset.responses().empty()) {return 0;}

    foreach (DrugResponseAnalysisData _data, dataset.responses()) {
        DrugResponseAnalysis* resp = CoreFactory::createEntity<DrugResponseAnalysis>(REPO);
        DrugTreatment* _treat = CoreFactory::createEntity<DrugTreatment>(REPO);
        _treat->setPatient(patient);
        resp->setDrugModel(drug);
        _treat->setActiveSubstanceId(drug->getActiveSubstance()->getSubstanceId());
        resp->setTreatment(_treat);
        _treat->setName(_data.drugTreatmentName());
//        SharedDrugModel _model = CoreFactory::createEntity<DrugModel>(REPO);
//        _model->setName(_data.drugModelName());
//        resp->setDrugModel(_model);

        //I guess we leave out the validation for now, as everything will be from the math module.
//        if (CORE->pluginManager()->postEngineExists(_data.postEngineName())) {
//          resp->setPostEngine(SharedPostEngine(CORE->pluginManager()->newPostEngine(_data.postEngineName())));
//        }
//        if (CORE->pluginManager()->reverseEngineExists(_data.reverseEngineName())) {
//          resp->setReverseEngine(SharedReverseEngine(CORE->pluginManager()->newReverseEngine(_data.reverseEngineName())));
//        }
//        if (CORE->pluginManager()->percentileEngineExists(_data.percentileEngineName())) {
//          resp->setPercentileEngine(SharedPercentileEngine(CORE->pluginManager()->newPercentileEngine(_data.percentileEngineName())));
//        }

        Response response = REPO->setDrugResponseAnalysis(resp);
        if (response.error != NoError) {
            LOG(QtWarningMsg, NOEZERROR, tr(_DATABASE_WRITE_FAILED).arg(tr("drugresponseanalysis"), response.message));
            return 0;
        }
        return resp;
    }
}

bool ImportAction::processMandator(const ImportAction::Dataset &dataset)
{
    ezechiel::GuiCore::Response response;

    //The database objects
    SharedPractician practician = AdminFactory::createEntity<Practician>(ADMINREPO);
    SharedInstitute institute = AdminFactory::createEntity<Institute>(ADMINREPO);

    //Check if ignored
    if (dataset.mandator().ignored())
        return true;

    //Search for an existing mandator
//    response = DB->getPractician(dataset.mandator().id(), practician);
    response = ADMINREPO->getPracticianFromId(dataset.mandator().id().toInt(), practician);

    //Retrieve associated data
    if (response.error == NoError) {

        //Get the institute
//        response = DB->getInstitute(practician->institute_id(), institute);
        response = ADMINREPO->getInstituteFromId(practician->institute_id(), institute);
        if (response.error != NoError) {
            LOG(QtWarningMsg, NOEZERROR, tr(_DATABASE_READ_FAILED).arg(tr("institute"), tr("ID"), QString::number(practician->institute_id()), response.message));
            return false;
        }

    } else if (response.error != NotFound) {
        LOG(QtWarningMsg, NOEZERROR, tr(_DATABASE_READ_FAILED).arg(tr("practician"), tr("with the external ID"), dataset.mandator().id(), response.message));
        return false;
    }

    //Set or update the practician data
    if (practician->externalId().isEmpty() || practician->externalId() != dataset.mandator().id())
        practician->externalId(dataset.mandator().id());
    if (practician->title().isEmpty() || practician->title() != dataset.mandator().title())
        practician->title(dataset.mandator().title());

    //Set or update the person data
    if (practician->person()->firstname().isEmpty() || practician->person()->firstname() != dataset.mandator().name().firstname())
        practician->person()->firstname(dataset.mandator().name().firstname());
    if (practician->person()->name().isEmpty() || practician->person()->name() != dataset.mandator().name().lastname())
        practician->person()->name(dataset.mandator().name().lastname());
    if (dataset.mandator().birthdate().isValid())
        practician->person()->birthday(dataset.mandator().birthdate().date());
    if (!dataset.mandator().gender().isEmpty())
        practician->person()->gender(dataset.mandator().gender() == _FEMALE ? Person::Female : Person::Male);

    //Set or update the contact data
    SharedPerson person = practician->person();
    if (!dataset.mandator().contact().ignored() && !processContact(dataset.mandator().contact(), person))
        return false;

    // YTA : TODO: refactor this part
    //Set or update the institute data
//    if (!dataset.mandator().institute().ignored() && !processInstitute(dataset.mandator().institute(), institute, instituteLocation))
//        return false;

//    institute.location = instituteLocation.id;

    //Save the institute
//    response = DB->setInstitute(institute);
    response = ADMINREPO->setInstitute(institute);
    if (response.error != NoError) {
        LOG(QtWarningMsg, NOEZERROR, tr(_DATABASE_WRITE_FAILED).arg(tr("institute"), tr("for the practician with the external ID"), dataset.mandator().id(), response.message));
        return false;
    }
    practician->institute(institute);

    //Save the practician
//    response = DB->setPractician(practician);
    response = ADMINREPO->setPractician(practician);
    if (response.error != NoError) {
        LOG(QtWarningMsg, NOEZERROR, tr(_DATABASE_WRITE_FAILED).arg(tr("practician"), tr("with the external ID"), dataset.mandator().id(), response.message));
        return false;
    }

    return true;
}

SharedPatient ImportAction::processPatient(const ImportAction::Dataset &dataset)
{
    ezechiel::GuiCore::Response response;

    //The database objects
    SharedPatient patient = AdminFactory::createEntity<Patient>(ADMINREPO);
//    SharedLocation location;

    //Search for an existing patient
//    response = DB->getPatient(dataset.patient().id(), patient);
//    response = ADMINREPO->getPatient(dataset.patient().id().toInt(), patient);

    //Retrieve associated data
//    if (response.error == NoError) {

//    } else if (response.error != NotFound) {
//        WARNING(tr(_DATABASE_READ_FAILED).arg(tr("patient"), tr("with the external ID"), dataset.patient().id(), response.message));
//        return false;
//    }

    //Set or update the patient data
    if (static_cast<Patient*>(patient)->externalId().isEmpty() || static_cast<Patient*>(patient)->externalId() != dataset.patient().id())
        static_cast<Patient*>(patient)->externalId(dataset.patient().id());
    if (static_cast<Patient*>(patient)->stayNumber().isEmpty() || static_cast<Patient*>(patient)->stayNumber() != dataset.patient().stayNumber())
        static_cast<Patient*>(patient)->stayNumber(dataset.patient().stayNumber());

    //Set or update the person data
    if (static_cast<Patient*>(patient)->person()->firstname().isEmpty() || static_cast<Patient*>(patient)->person()->firstname() != dataset.patient().name().firstname())
        static_cast<Patient*>(patient)->person()->firstname(dataset.patient().name().firstname());
    if (static_cast<Patient*>(patient)->person()->name().isEmpty() || static_cast<Patient*>(patient)->person()->name() != dataset.patient().name().lastname())
        static_cast<Patient*>(patient)->person()->name(dataset.patient().name().lastname());
    if (!static_cast<Patient*>(patient)->person()->birthday().isValid() || static_cast<Patient*>(patient)->person()->birthday() != dataset.patient().birthdate().date())
        static_cast<Patient*>(patient)->person()->birthday(dataset.patient().birthdate().date());
    static_cast<Patient*>(patient)->person()->gender(dataset.patient().gender() == _FEMALE ? Person::Female : Person::Male);

    //Set or update the contact data
    SharedPerson person = static_cast<Patient*>(patient)->person();
    if (!dataset.patient().contact().ignored() && !processContact(dataset.patient().contact(), person))
        return SharedPatient(0);

    //Save the patient
//    response = DB->setPatient(patient);
    response = ADMINREPO->setPatient(patient);
    if (response.error != NoError) {
        LOG(QtWarningMsg, NOEZERROR, tr(_DATABASE_WRITE_FAILED).arg(tr("patient"), tr("with the external ID"), dataset.patient().id(), response.message));
        return SharedPatient(0);
    }

    return patient;
}

DrugModel* ImportAction::processDrug(const ImportAction::Dataset &dataset)
{
    APPCORE->pluginManager();
    ezechiel::GuiCore::Response response;

    //The database objects
//    SharedPatient patient;
    QList<ident> _preds;
    QStringList names;

    //The curve object
    QString drugId;
//    SharedDrugResponseAnalysis _resp = ezechiel::GuiCore::CoreFactory::createEntity<DrugResponseAnalysis>(REPO);
//    SharedDrugTreatment _treat = ezechiel::GuiCore::CoreFactory::createEntity<DrugTreatment>(REPO);
//    _resp->setTreatment(_treat);

    //Get the drug ID
    drugId = getDrugId(dataset);
    if (drugId.isEmpty()) {
        LOG(QtDebugMsg, NOEZERROR, tr("\tIgnoring unknown drug with ATC '%1' and ID '%2', prescribed to patient with external ID '%3', as well as related sample(s) and dosage(s)").arg(dataset.drug().atc(), dataset.drug().id(), dataset.patient().id()));
        return 0;
    }

    //Pre-load the drug
    DrugModel* _drg;
    if (!(_drg = APPCORE->drugManager()->drug(drugId))) {
        LOG(QtWarningMsg, NOEZERROR, tr("An error occured while loading the drug ID '%1', prescribed to patient with external ID '%2'").arg(drugId, dataset.patient().id()));
        return 0;
    }
    return _drg;
//    _treat->setDrug(_drg);

    //Get the patient
//    response = DB->getPatient(dataset.patient().id(), patient);
//    response = ADMINREPO->getPatient(dataset.patient().id().toInt(), patient);
//    if (response.error != NoError) {
//        WARNING(tr(_DATABASE_READ_FAILED).arg(tr("patient"), tr("with the external ID"), dataset.patient().id(), response.message));
//        return false;
//    }
//    _treat->setPatient(patient);

    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // Remove this version of getCurvesList() (and this version only) when we have found a better
    // way to find a related curve (most certainly through human decision).
    // ////////////////////////////////////////////////////////////////////////////////////////////

    //Get the curves
//    response = DB->getCurvesList(curves, names);
//    response = REPO->getPredictionsList(_preds, names);
//    if (response.error != NoError) {
//        WARNING(tr(_DATABASE_READ_FAILED).arg(tr("curves"), tr("for patient with the external ID"), dataset.patient().id(), response.message));
//        return false;
//    }
//    SharedPredictionSet _predset = ezechiel::GuiCore::CoreFactory::createEntity<PredictionSet>(REPO);
//    foreach (SharedPrediction _pr, *_predset) {
//        _predset->append(_pr);
//    }
//    _resp->setPredictionSet(_predset);

//    //Get the curve
//    int index = names.indexOf(QRegExp(QString(_CURVE_REGEX).arg(dataset.patient().id(), drugId)));
//    if (index != -1) {

//        //Load the curve from the database
//        if (!_resp->load(_preds.at(index))) {
//            WARNING(tr(_DATABASE_READ_FAILED).arg(tr("curve"), tr("ID"), QString::number(_preds.at(index)), _resp->errorString()));
//            return false;
//        }

//    } else {

//        //Get the model ID
//        QString modelId = getModelId(drugId);
//        if (modelId.isEmpty()) {
//            MESSAGE(tr("\tIgnoring drug ID '%1', prescribed to patient with external ID '%2', for a compatible model could not be found").arg(drugId, dataset.patient().id()));
//            return true;
//        }

//        //Get the percentiles engine ID
//        QString peId = getPeId();
//        if (peId.isEmpty()) {
//            WARNING(tr(_INVALID_ENGINE).arg(tr("percentiles engine")));
//            return false;
//        }

//        //Get the post engine ID
//        QString poId = getPoId();
//        if (poId.isEmpty()) {
//            WARNING(tr(_INVALID_ENGINE).arg(tr("post engine")));
//            return false;
//        }

//        //Get the percentiles engine ID
//        QString reId = getReId();
//        if (reId.isEmpty()) {
//            WARNING(tr(_INVALID_ENGINE).arg(tr("reverse engine")));
//            return false;
//        }

//        //Set the model
//        SharedDrugModel _mdl;
//        if (!(_mdl = SharedDrugModel(CORE->pluginManager()->newDrugModelEngine(modelId)))) {
//            WARNING(tr(_INVALID_CURVE).arg(tr("model"), dataset.patient().id()));
//            return false;
//        }
//        _treat->setDrugmodel(_mdl);

//        //Set the drug
//        SharedDrug _drg;
//        if (!(_drg = APPCORE->drugManager()->drug(drugId))) {
//            WARNING(tr(_INVALID_CURVE).arg(tr("drug"), dataset.patient().id()));
//            return false;
//        }
//        _treat->setDrug(_drg);

//        //Set the engines
//        SharedPercentileEngine _prc;
//        if (!(_prc = SharedPercentileEngine(CORE->pluginManager()->newPercentileEngine(peId)))) {
//            WARNING(tr(_INVALID_CURVE).arg(tr("percentiles engine"), dataset.patient().id()));
//            return false;
//        }
//        _resp->setPercentileEngine(_prc);

//        SharedPostEngine _post;
//        if (!(_post = SharedPostEngine(CORE->pluginManager()->newPostEngine(poId)))) {
//            WARNING(tr(_INVALID_CURVE).arg(tr("post engine"), dataset.patient().id()));
//            return false;
//        }
//        _resp->setPostEngine(_post);

//        SharedReverseEngine _re;
//        if (!(_re = SharedReverseEngine(CORE->pluginManager()->newReverseEngine(reId)))) {
//            WARNING(tr(_INVALID_CURVE).arg(tr("reverse engine"), dataset.patient().id()));
//            return false;
//        }
//        _resp->setReverseEngine(_re);

//        //Set the patient
//        _resp->getTreatment()->setPatient(patient);
////        if (!curve->setPatient(patient)) {
////            WARNING(tr(_INVALID_CURVE).arg(tr("patient"), dataset.patient().id()));
////            return false;
////        }

//        //Finalize the curve
////        curve->setCurveType(CurveModel::Partial);
////        curve->setParameterType(APOSTERIORI);
//        _resp->getTreatment()->setName(QString(_CURVE_NAME).arg(dataset.patient().name().firstname(), dataset.patient().name().lastname(), dataset.patient().id(), drugId));
//    }

//    //Set or update the dosages
//    if (!processDosages(dataset, _resp))
//        return false;

//    //Set or update the samples
//    if (!processSamples(dataset, _resp))
//        return false;

//    //Set or update the curve
//    if (!_resp->save()) {
//        WARNING(tr(_DATABASE_WRITE_FAILED).arg(tr("curve"), tr("for patient with external ID"), dataset.patient().id(), _resp->errorString()));
//        return false;
//    }

//    return true;
}

bool ImportAction::processCovariates(const ImportAction::Dataset &dataset, SharedPatient patient)
{
    ezechiel::GuiCore::Response response;

    //The database objects
    QList<PatientVariate*> covariates;
    QMultiMap<QDateTime, PatientVariate*> sorted;
//    SharedPatient patient;

    //Get the patient
//    response = DB->getPatient(dataset.patient().id(), patient);
//    response = ADMINREPO->getPatient(dataset.patient().id().toInt(), patient);
//    if (response.error != NoError) {
//        WARNING(tr(_DATABASE_READ_FAILED).arg(tr("patient"), tr("with the external ID"), dataset.patient().id(), response.message));
//        return false;
//    }

    //Get the covariates
//    response = DB->getCovariatesByDate(patient->id(), covariates);
    response = ADMINREPO->getPatientVariatesByDate(patient->id(), covariates);
    if (response.error != NoError) {
        LOG(QtWarningMsg, NOEZERROR, tr(_DATABASE_READ_FAILED).arg(tr("covariates"), tr("for patient ID"), QString::number(patient->id()), response.message));
        return false;
    }

    //Sort the covariates
    for (int i = 0; i < covariates.size(); i++)
        sorted.insert(covariates.at(i)->getDate(), covariates.at(i));

    //Process covariates
    for (int i = 0; i < dataset.covariates().size(); i++) {
        CovariateData data = dataset.covariates().at(i);

        //Build the covariate
        PatientVariate* covariate = ezechiel::GuiCore::CoreFactory::createEntity<PatientVariate>(ADMINREPO);
        covariate->setCovariateId(data.name());
        covariate->setDate(data.date());
        covariate->getQuantity()->setValue(data.value());
//        covariate->patient(patient);
//        if (!covariate->isValid()) {
//            MESSAGE(tr("\tIgnoring invalid covariate '%1' of patient with external ID '%2'").arg(covariate->name(), dataset.patient().id()));
//            continue;
//        }

        //Set the database ID
        if (sorted.contains(covariate->getDate())) {
            QList<PatientVariate*> sameDate = sorted.values(covariate->getDate());

            //Search for an identical covariate
            for (int j = 0; j < sameDate.size(); j++) {
                if (sameDate.at(j)->getCovariateId() == covariate->getCovariateId()) {
                    covariate->setId(sameDate.at(j)->id());
                    break;
                }
            }
        }

        //Set or update the covariate
//        response = DB->setCovariate(patient->id(), covariate);
        response = ADMINREPO->setPatientVariate(covariate);
        if (response.error != NoError) {
            LOG(QtDebugMsg, NOEZERROR, tr("\tFailed to set or update covariate '%1' of patient with external ID '%2' (%3)").arg(covariate->getCovariateId(), dataset.patient().id(), response.message));
            continue;
        }
    }

    return true;
}

bool ImportAction::processClinicals(const ImportAction::Dataset &dataset, SharedPatient patient)
{
    ezechiel::GuiCore::Response response;

    //The database objects
//    SharedPatient patient;
    QList<Clinical*> clinicals;
    QMultiMap<QDateTime, Clinical*> sorted;

    //Get the patient
//    response = DB->getPatient(dataset.patient().id(), patient);
//    response = ADMINREPO->getPatient(dataset.patient().id().toInt(), patient);
//    if (response.error != NoError) {
//        WARNING(tr(_DATABASE_READ_FAILED).arg(tr("patient"), tr("with the external ID"), dataset.patient().id(), response.message));
//        return false;
//    }

    //Get the clinicals
//    response = DB->getClinicalsByDate(patient->id(), clinicals);
    response = ADMINREPO->getClinicalsByDate(patient->id(), clinicals);
    if (response.error != NoError && response.error != NotFound) {
        LOG(QtWarningMsg, NOEZERROR, tr(_DATABASE_READ_FAILED).arg(tr("clinicals"), tr("for patient ID"), QString::number(patient->id()), response.message));
        return false;
    }

    //Sort the clinicals
    for (int i = 0; i < clinicals.size(); i++)
        sorted.insert(clinicals.at(i)->getDate(), clinicals.at(i));

    //Process the clinicals
    for (int i = 0; i < dataset.clinicals().size(); i++) {
        ClinicalData data = dataset.clinicals().at(i);

        //Build the clinical
        Clinical* clinical = AdminFactory::createEntity<Clinical>(ADMINREPO);
        clinical->setName(data.name());
        clinical->setDate(data.date());
        clinical->setValue(data.value());

//        if (!clinical->isValid()) {
//            MESSAGE(tr("\tIgnoring invalid clinical '%1' of patient with external ID '%2'").arg(clinical->name(), dataset.patient().id()));
//            continue;
//        }

        //Set the database ID
        if (sorted.contains(clinical->getDate())) {
            QList<Clinical*> sameDate = sorted.values(clinical->getDate());

            //Search for an identical clinical
            for (int j = 0; j < sameDate.size(); j++) {
                if (sameDate.at(j)->id() == clinical->id()) {
                    clinical->setId(sameDate.at(j)->id());
                    break;
                }
            }
        }

        //Set or update the clinical
//        response = DB->setClinical(patient->id(), clinical);
        response = ADMINREPO->setClinical(clinical);
        if (response.error != NoError) {
            LOG(QtDebugMsg, NOEZERROR, tr("\tFailed to set or update clinical '%1' of patient with external ID '%2' (%3)").arg(clinical->getName(), dataset.patient().id(), response.message));
            continue;
        }
    }

    return true;
}

bool ImportAction::processPredictions(const ImportAction::Dataset &dataset, DrugResponseAnalysis* dresponse)
{
    ezechiel::GuiCore::Response response;

    //The database objects
//    SharedPatient patient;
    QList<Prediction*> predictions;
    QMultiMap<QDateTime, Prediction*> sorted;

    //Get the patient
//    response = ADMINREPO->getPatient(dataset.patient().id().toInt(), patient);
//    if (response.error != NoError) {
//        WARNING(tr(_DATABASE_READ_FAILED).arg(tr("patient"), tr("with the external ID"), dataset.patient().id(), response.message));
//        return false;
//    }

    //Get the predictions
    response = REPO->getPredictionsList({dresponse->getTreatment()->getPatient()->id()}, predictions);
    if (response.error != NoError && response.error != NotFound) {
        LOG(QtWarningMsg, NOEZERROR, tr(_DATABASE_READ_FAILED).arg(tr("predictions"), tr("for patient ID"), QString::number(dresponse->getTreatment()->getPatient()->id()), response.message));
        return false;
    }

    //Sort the predictions
    for (int i = 0; i < predictions.size(); i++)
        sorted.insert(predictions.at(i)->getSpec()->getStartDate(), predictions.at(i));

    //Process the predictions
    for (int i = 0; i < dataset.predictions().size(); i++) {
        PredictionData data = dataset.predictions().at(i);

        //Build the Prediction
        Prediction* prediction = ezechiel::GuiCore::CoreFactory::createEntity<Prediction>(REPO);
        prediction->setName(data.name());
        ezechiel::GuiCore::ParameterType _ptype;
        if (data.paramsType() == "apriori") {
            _ptype = ezechiel::GuiCore::APRIORI;
        } else if (data.paramsType() == "aposteriori") {
            _ptype = ezechiel::GuiCore::APOSTERIORI;
        } else if (data.paramsType() == "population") {
            _ptype = ezechiel::GuiCore::POPULATION;
        } else if (data.paramsType() == "manual") {
            _ptype = ezechiel::GuiCore::MANUAL;
        } else if (data.paramsType() == "last") {
            _ptype = ezechiel::GuiCore::LAST_PT;
        }
        prediction->setParamsType(_ptype);
        prediction->setCurveType(data.curveType() == "continuous" ? Continous : Partial);
        PredictionSpec* _spec = ezechiel::GuiCore::CoreFactory::createEntity<PredictionSpec>(REPO);
        _spec->setNbPoints(data.nbPoints().toInt());
        _spec->setStartDate(QDateTime::fromString(data.startdate(), _xmlDateTimeFormat));
        _spec->setEndDate(QDateTime::fromString(data.enddate(), _xmlDateTimeFormat));
        _spec->setNbPoints(data.nbPoints().toInt());
        _spec->setSteadyState(data.steadyState() == "true"? 1 : 0);
        prediction->setSpec(_spec);
        prediction->setAnalysis(dresponse);

//        if (!prediction->isValid()) {
//            MESSAGE(tr("\tIgnoring invalid clinical '%1' of patient with external ID '%2'").arg(prediction->name(), dataset.patient().id()));
//            continue;
//        }

        //Set the database ID
        if (sorted.contains(prediction->getSpec()->getStartDate())) {
            QList<Prediction*> sameDate = sorted.values(prediction->getSpec()->getStartDate());

            //Search for an identical clinical
            for (int j = 0; j < sameDate.size(); j++) {
                if (sameDate.at(j)->id() == prediction->id()) {
                    prediction->setId(sameDate.at(j)->id());
                    break;
                }
            }
        }

        response = REPO->setPrediction(prediction);
        if (response.error != NoError) {
            LOG(QtDebugMsg, NOEZERROR, tr("\tFailed to set or update clinical '%1' of patient with external ID '%2' (%3)").arg(prediction->getName(), dataset.patient().id(), response.message));
            continue;
        }
    }

    return true;
}

bool ImportAction::processInstitute(const InstituteData &instituteData, SharedInstitute &institute)
{
//    DBPerson *ignored = 0;

    //Set or update the institute data
    if (institute->externalId().isEmpty() || institute->externalId() != instituteData.id())
        institute->externalId(instituteData.id());
    if (institute->name().isEmpty() || institute->name() != instituteData.name())
        institute->name(instituteData.name());

    //Set or update the contact
    // YTA : TODO : refactor this
//    if (!instituteData.contact().ignored() && !processContact(instituteData.contact(), ignored, location))
  //      return false;

    return true;
}

bool ImportAction::processContact(const ImportAction::ContactData &contactData, SharedPerson &person)
{
    Location* location;
    location = person->location();
    //Set or update the location data
    if (location->address().isEmpty() || location->address() != contactData.address())
        location->address(contactData.address());
    if (location->city().isEmpty() || location->city() != contactData.city())
        location->city(contactData.city());
    if (location->country().isEmpty() || location->country() != contactData.country())
        location->country(contactData.country());
    if (location->postcode().isEmpty() || location->postcode() != contactData.postcode())
        location->postcode(contactData.postcode());
    if (location->state().isEmpty() || location->state() != contactData.state())
        location->state(contactData.state());

    //Set or update the phones data
    for (int i = 0; i < contactData.phones().size(); i++) {

        //Build the phone
        SharedPhone phone = AdminFactory::createEntity<Phone>(ADMINREPO);
        phone->setNumber(contactData.phones().at(i).number());
        phone->setTypeFromString(contactData.phones().at(i).type());

        //Add the phone
//        if (!person->getPhones()->contains(phone))
//            person->addPhone(phone);
    }

    //Set or update the email
    for (int i = 0; i < contactData.emails().size(); i++) {
        // YTA : TODO : Refactor this
//        if (!person->emails().contains(contactData.emails().at(i).email())) {
  //          SharedEmail email = SharedEmail(new Email());
    //        email->setEmail(contactData.emails().at(i).email());
      //      person->addEmail();
       // }
    }
    return true;
}

bool ImportAction::processDosages(const ImportAction::Dataset &dataset, DrugResponseAnalysis* resp)
{
    QMap<QDateTime, Dosage*> dosages;

    //Create the dosages
    for (int i = 0; i < dataset.dosages().size(); i++) {
        DosageData data = dataset.dosages().at(i);

        //Set the dosage data
        Dosage* dosage = ezechiel::GuiCore::CoreFactory::createEntity<Dosage>(REPO);
        dosage->getQuantity()->setValue(data.dose().value());
        dosage->getQuantity()->setUnit(data.dose().unit());
        dosage->setInterval(data.interval());

        //Set the infusion time
        if (CORE->definitionManager()->isInfusion(data.intake()))
            dosage->setTinf(data.infusion().isEmpty() ? Duration(0, 0, resp->getDrugModel()->getInfusions()->getQuantity()->getDbvalue() * 60) : data.infusion());
//        dosage->setTinf(data.infusion().isEmpty() ? Duration(0, 0, APPCORE->drugManager()->drug(resp->getTreatment()->getDrug()->descriptor().id)->getInfusions()->getQuantity()->getDbvalue() * 60) : data.infusion());

        //Set the start date
        if (data.startDate().isValid()) {
            dosage->setApplied(data.startDate());
            //if (dosage->isValid())
            //    dosages.insert(dosage->getApplied(), dosage);
        }

        //Set the last date
        if (data.lastDate().isValid()) {
            dosage->setEndTime(data.lastDate());
            //if (dosage->isValid())
            //    dosages.insert(dosage->getApplied(), dosage);
        }
        // YTA: Added this in order to allow the tests to run
        dosages.insert(dosage->getApplied(), dosage);

        //Check if inserted
        //if (!dosage->isValid()) {
        //    LOG(QtDebugMsg, NOEZERROR, tr("\tIgnoring invalid dosage of drug ID '%1' for patient with external ID '%2'").arg(resp->getDrugModel()->descriptor().id, dataset.patient().id()));
        //    continue;
        //}
    }

    //Set the first take
    if (!resp->getTreatment()->getDosages()->firsttake().isValid()) {

        //Ignore if no dosages
        if (dosages.isEmpty()) {
            LOG(QtWarningMsg, NOEZERROR, tr("Could not find any dosage for drug ID '%1', prescribed to patient with external ID '%2'").arg(resp->getDrugModel()->descriptor().id, dataset.patient().id()));
            return false;
        }

        //Set the first take
        resp->getTreatment()->getDosages()->setFirsttake(dosages.firstKey());
    }

    //Set the dosages
    QMap<QDateTime, Dosage*>::iterator crt;

    for (crt = dosages.begin(); crt != dosages.end(); crt++) {
        Response r = REPO->setDosage(crt.value());
        if (r.error != NoError && r.error != NotFound) {
            LOG(QtWarningMsg, NOEZERROR, tr("An error occured while setting the curve dosages for drug ID '%1', prescribed to patient with external ID '%2'").arg(resp->getDrugModel()->descriptor().id, dataset.patient().id()));
            return false;
        }
        resp->getTreatment()->getDosages()->append(crt.value());
    }

    return true;
}

bool ImportAction::processSamples(const ImportAction::Dataset &dataset, DrugResponseAnalysis* resp)
{
    ezechiel::GuiCore::Response response;

    // YTA: I modified the DAL object, so the measure does not know the drug

    //The database objects
    QList<Measure *> samples;
    QMap<QDateTime, Measure *> sorted;

    //Get the samples
//    response = DB->getMeasuresByDate(curve->patient()->id(), samples);
//    response = ADMINREPO->getMeasuresByDate(resp->getTreatment()->getPatient()->id(), samples);
//    if (response.error != NoError && response.error != NotFound) {
//        LOG(QtWarningMsg, NOEZERROR, tr(_DATABASE_READ_FAILED).arg(tr("measures"), tr("for patient ID"), QString::number(resp->getTreatment()->getPatient()->id()), response.message));
//        return false;
//    }

    // it belongs to. So here we should get the measures directly from the medicaltreatment,
    // not by asking measures by patient ID and then checking with the drug id
/*
    //Sort the samples
    for (int i = 0; i < samples.size(); i++)
        if (samples.at(i)->sdrug() == resp->getTreatment()->drug()->descriptor().id)
            sorted.insert(samples.at(i)->getMoment(), samples.at(i));
*/
    QList<CoreMeasure*> measures = resp->getTreatment()->getMeasures()->getList();
    //Process the samples
    for (int i = 0; i < dataset.samples().size(); i++) {
        SampleData data = dataset.samples().at(i);

        //Build the sample
        Measure * sample = AdminFactory::createEntity<Measure>(ADMINREPO);
        sample->setMoment(data.sampleDate());
        //sample->patient(resp->getTreatment()->patient());
        sample->setConcentration(CoreFactory::createEntity<IdentifiableAmount>(ADMINREPO, sample));
        sample->getConcentration()->setValue(data.concentration().value());
        sample->getConcentration()->setUnit(Unit("ug/L"));
//        sample->setValue(data.concentration().value());
//        sample->setUnit(data.concentration().unit().toString());
        //sample->setDrug(resp->getTreatment()->drug());
        sample->setSdrug(resp->getTreatment()->getActiveSubstanceId());
        static_cast<Measure*>(sample)->sampleID(data.id());
        static_cast<Measure*>(sample)->arrivalDate(data.arrivalDate());

//        if (!sample->isValid()) {
//            MESSAGE(tr("\tIgnoring invalid sample ID '%1' of patient with external ID '%2'").arg(data.id(), dataset.patient().id()));
//            continue;
//        }

        //Set the database ID
        if (sorted.contains(sample->getMoment()) && static_cast<Measure*>(sorted.value(sample->getMoment()))->sampleID() == static_cast<Measure*>(sample)->sampleID())
            sample->setId(sorted.value(sample->getMoment())->id());

        //Set or update the sample
//        response = DB->setMeasure(curve->patient()->id(), sample);
        response = ADMINREPO->setMeasure(sample);
        if (response.error != NoError) {
            LOG(QtDebugMsg, NOEZERROR, tr("\tFailed to set or update sample ID '%1' of patient with external ID '%2' (%3)").arg(data.id(), dataset.patient().id(), response.message));
            continue;
        }
        //shouldnt have both SharedCoreMeasure and SharedMeasure here
        resp->getTreatment()->getMeasures()->append(sample);
    }
    return true;
}

QString ImportAction::getDrugId(const ImportAction::Dataset &dataset) const
{
    //Get using the drug ID
    if (!dataset.drug().id().isEmpty() && APPCORE->drugManager()->exists(dataset.drug().id()))
        return dataset.drug().id();

    //Get using the drug ATC
    if (!dataset.drug().atc().isEmpty() && APPCORE->drugManager()->descriptionsForAtc(dataset.drug().atc()).size() == 1)
        return APPCORE->drugManager()->descriptionsForAtc(dataset.drug().atc()).first().id;

    //Failed to find a unique ID
    return QString();
}

QString ImportAction::getModelId(const QString &drugId) const
{
    //Get the compatible models
    QStringList rawModels = APPCORE->drugManager()->models(drugId);
    QStringList available = CORE->pluginManager()->filter(rawModels);

    if (available.isEmpty())
        return QString();

    //Sort the models by priority
    QMultiMap<int, QString> sorted;

    //Return the highest priority
    return QString(""); // sorted.first();
}


// ////////////////////////////////////////////////////////////////////////////////////////////////
// Data set
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ImportAction::Dataset::_TAG_DATASET = "dataset";

QString ImportAction::Dataset::lang() const
{
    return _lang;
}

void ImportAction::Dataset::setLang(const QString &lang)
{
    _lang = lang;
}

ImportAction::DrugData ImportAction::Dataset::drug() const
{
    return _drug;
}

void ImportAction::Dataset::setDrug(const DrugData &drug)
{
    _drug = drug;
}

ImportAction::PatientData ImportAction::Dataset::patient() const
{
    return _patient;
}

void ImportAction::Dataset::setPatient(const PatientData &patient)
{
    _patient = patient;
}

ImportAction::PracticianData ImportAction::Dataset::mandator() const
{
    return _mandator;
}

void ImportAction::Dataset::setMandator(const PracticianData &mandator)
{
    _mandator = mandator;
}

QList<ImportAction::DosageData> ImportAction::Dataset::dosages() const
{
    return _dosages;
}

void ImportAction::Dataset::setDosages(const QList<DosageData> &dosages)
{
    _dosages = dosages;
}

void ImportAction::Dataset::addDosage(const ImportAction::DosageData &dosage)
{
    _dosages.append(dosage);
}

QList<ImportAction::SampleData> ImportAction::Dataset::samples() const
{
    return _samples;
}

void ImportAction::Dataset::setSamples(const QList<SampleData> &samples)
{
    _samples = samples;
}

void ImportAction::Dataset::addSample(const ImportAction::SampleData &sample)
{
    _samples.append(sample);
}

QList<ImportAction::CovariateData> ImportAction::Dataset::covariates() const
{
    return _covariates;
}

void ImportAction::Dataset::setCovariates(const QList<CovariateData> &covariates)
{
    _covariates = covariates;
}

void ImportAction::Dataset::addCovariate(const ImportAction::CovariateData &covariate)
{
    _covariates.append(covariate);
}

QList<ImportAction::ClinicalData> ImportAction::Dataset::clinicals() const
{
    return _clinicals;
}

void ImportAction::Dataset::setClinicals(const QList<ClinicalData> &clinicals)
{
    _clinicals = clinicals;
}

void ImportAction::Dataset::addClinical(const ImportAction::ClinicalData &clinical)
{
    _clinicals.append(clinical);
}

QList<ImportAction::PredictionData> ImportAction::Dataset::predictions() const
{
    return _predictions;
}

void ImportAction::Dataset::setPredictions(const QList<PredictionData> &predictions)
{
    _predictions = predictions;
}

void ImportAction::Dataset::addPrediction(const ImportAction::PredictionData &prediction)
{
    _predictions.append(prediction);
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Document data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ImportAction::Document::_TAG_ROOT = "data";
const char *const ImportAction::Document::_ATTRIBUTE_VERSION = "version";
const char *const ImportAction::Document::_ATTRIBUTE_XSI = "xmlns:xsi";
const char *const ImportAction::Document::_ATTRIBUTE_SCHEMA_LOCATION = "xsi:noNamespaceSchemaLocation";

ImportAction::Document::Document()
{

}

} //namespace cliutils
} //namespace ezechiel
