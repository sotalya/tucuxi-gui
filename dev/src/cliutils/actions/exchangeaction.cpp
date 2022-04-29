//@@license@@

#include "exchangeaction.h"

namespace ezechiel {
namespace cliutils {
//The error strings
const char *const ExchangeAction::_DATABASE_READ_FAILED   = QT_TR_NOOP_UTF8("Failed to retrieve the %1 %2 '%3' from the database (%4)");
const char *const ExchangeAction::_DATABASE_WRITE_FAILED  = QT_TR_NOOP_UTF8("Failed to save the %1 %2 '%3' to the database (%4)");
const char *const ExchangeAction::_INVALID_XML_FILE       = QT_TR_NOOP_UTF8("The exchange file '%1' is invalid. At line %2: %3.");

//The gender strings
const char *const ExchangeAction::_MALE   = "male";
const char *const ExchangeAction::_FEMALE = "female";

//The global and shared tags
const char *const ExchangeAction::_TAG_VALUE    = "value";
const char *const ExchangeAction::_TAG_UNIT     = "unit";
const char *const ExchangeAction::_TAG_COMMENTS = "comments";

//The global and shared attributes
const char *const ExchangeAction::_ATTRIBUTE_LANG = "lang";

//The duration conversions
const int ExchangeAction::_FROM_SECONDS = 1000;
const int ExchangeAction::_FROM_MINUTES = ExchangeAction::_FROM_SECONDS * 60;
const int ExchangeAction::_FROM_HOURS   = ExchangeAction::_FROM_MINUTES * 60;
const int ExchangeAction::_FROM_DAYS    = ExchangeAction::_FROM_HOURS   * 24;
const int ExchangeAction::_FROM_WEEK    = ExchangeAction::_FROM_DAYS    * 7;

ExchangeAction::ExchangeAction()
{

}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Interpretation data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::InterpretationData::_TAG_INTERPRETATION = "interpretation";
const char *const ExchangeAction::InterpretationData::_TAG_ID = "id";
const char *const ExchangeAction::InterpretationData::_TAG_EXPECTEDNESS = "expectedness";
const char *const ExchangeAction::InterpretationData::_TAG_SUITABILITY = "suitability";
const char *const ExchangeAction::InterpretationData::_TAG_PREDICTION = "prediction";
const char *const ExchangeAction::InterpretationData::_TAG_REMONITORING = "remonitoring";
const char *const ExchangeAction::InterpretationData::_TAG_WARNING = "warning";
const char *const ExchangeAction::InterpretationData::_TAG_CURVE = "curve";
const char *const ExchangeAction::InterpretationData::_TAG_DATE = "date";

QDomDocumentFragment ExchangeAction::InterpretationData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the interpretation
    QDomElement interpretationNode = doc.createElement(_TAG_INTERPRETATION);
    fragment.appendChild(interpretationNode);

    //Insert the ID
    QDomElement idNode = doc.createElement(_TAG_ID);
    QDomText idContent = doc.createTextNode(id());

    idNode.appendChild(idContent);
    interpretationNode.appendChild(idNode);

    //Insert the expectedness
    QDomElement expectednessNode = doc.createElement(_TAG_EXPECTEDNESS);
    QDomText expectednessContent = doc.createTextNode(expectedness());

    expectednessNode.appendChild(expectednessContent);
    interpretationNode.appendChild(expectednessNode);

    //Insert the suitability
    QDomElement suitabilityNode = doc.createElement(_TAG_SUITABILITY);
    QDomText suitabilityContent = doc.createTextNode(suitability());

    suitabilityNode.appendChild(suitabilityContent);
    interpretationNode.appendChild(suitabilityNode);

    //Insert the prediction
    QDomElement predictionNode = doc.createElement(_TAG_PREDICTION);
    QDomText predictionContent = doc.createTextNode(prediction());

    predictionNode.appendChild(predictionContent);
    interpretationNode.appendChild(predictionNode);

    //Insert the remonitoring
    QDomElement remonitoringNode = doc.createElement(_TAG_REMONITORING);
    QDomText remonitoringContent = doc.createTextNode(remonitoring());

    remonitoringNode.appendChild(remonitoringContent);
    interpretationNode.appendChild(remonitoringNode);

    //Insert the warnings
    QDomElement warningsNode = doc.createElement(_TAG_WARNING);
    QDomText warningsContent = doc.createTextNode(warning());

    warningsNode.appendChild(warningsContent);
    interpretationNode.appendChild(warningsNode);

    //Insert the curve
    QDomElement curveNode = doc.createElement(_TAG_CURVE);
    QDomText curveContent = doc.createTextNode(curve().toBase64());

    curveNode.appendChild(curveContent);
    interpretationNode.appendChild(curveNode);

    //Insert the date
    QDomElement dateNode = doc.createElement(_TAG_DATE);
    QDomText dateContent = doc.createTextNode(date().toString(Action::xmlDateTimeFormat()));

    dateNode.appendChild(dateContent);
    interpretationNode.appendChild(dateNode);

    //Insert the comments
    QDomElement commentsNode = doc.createElement(_TAG_COMMENTS);
    interpretationNode.appendChild(commentsNode);

    return fragment;
}

QString ExchangeAction::InterpretationData::id() const
{
    return _id;
}

void ExchangeAction::InterpretationData::setId(const QString &id)
{
    _id = id;
}

QString ExchangeAction::InterpretationData::expectedness() const
{
    return _expectedness;
}

void ExchangeAction::InterpretationData::setExpectedness(const QString &expectedness)
{
    _expectedness = expectedness;
}

QString ExchangeAction::InterpretationData::suitability() const
{
    return _suitability;
}

void ExchangeAction::InterpretationData::setSuitability(const QString &suitability)
{
    _suitability = suitability;
}

QString ExchangeAction::InterpretationData::prediction() const
{
    return _prediction;
}

void ExchangeAction::InterpretationData::setPrediction(const QString &prediction)
{
    _prediction = prediction;
}

QString ExchangeAction::InterpretationData::remonitoring() const
{
    return _remonitoring;
}

void ExchangeAction::InterpretationData::setRemonitoring(const QString &remonitoring)
{
    _remonitoring = remonitoring;
}

QString ExchangeAction::InterpretationData::warning() const
{
    return _warning;
}

void ExchangeAction::InterpretationData::setWarning(const QString &warning)
{
    _warning = warning;
}

QByteArray ExchangeAction::InterpretationData::curve() const
{
    return _curve;
}

void ExchangeAction::InterpretationData::setCurve(const QByteArray &curve)
{
    _curve = curve;
}

QDateTime ExchangeAction::InterpretationData::date() const
{
    return _date;
}

void ExchangeAction::InterpretationData::setDate(const QDateTime &date)
{
    _date = date;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Drug data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::DrugData::_TAG_DRUG = "drug";
const char *const ExchangeAction::DrugData::_TAG_ID = "id";
const char *const ExchangeAction::DrugData::_TAG_ATC = "atc";
const char *const ExchangeAction::DrugData::_TAG_BRAND = "brandName";
const char *const ExchangeAction::DrugData::_TAG_PRINCIPLE = "activePrinciple";

QDomDocumentFragment ExchangeAction::DrugData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the drug
    QDomElement drugNode = doc.createElement(_TAG_DRUG);
    fragment.appendChild(drugNode);

    //Insert the ID
    QDomElement idNode = doc.createElement(_TAG_ID);
    QDomText idContent = doc.createTextNode(id());

    idNode.appendChild(idContent);
    drugNode.appendChild(idNode);

    //Insert the ATC
    QDomElement atcNode = doc.createElement(_TAG_ATC);
    QDomText atcContent = doc.createTextNode(atc());

    atcNode.appendChild(atcContent);
    drugNode.appendChild(atcNode);

    //Insert the brand name
    QDomElement brandNode = doc.createElement(_TAG_BRAND);
    QDomText brandContent = doc.createTextNode(brandName());

    brandNode.appendChild(brandContent);
    drugNode.appendChild(brandNode);

    //Insert the active principle
    QDomElement apNode = doc.createElement(_TAG_PRINCIPLE);
    QDomText apContent = doc.createTextNode(activePrinciple());

    apNode.appendChild(apContent);
    drugNode.appendChild(apNode);

    //Insert the comments
    QDomElement commentsNode = doc.createElement(_TAG_COMMENTS);
    drugNode.appendChild(commentsNode);

    return fragment;
}

QString ExchangeAction::DrugData::id() const
{
    return _id;
}

void ExchangeAction::DrugData::setId(const QString &id)
{
    _id = id;
}

QString ExchangeAction::DrugData::atc() const
{
    return _atc;
}

void ExchangeAction::DrugData::setAtc(const QString &atc)
{
    _atc = atc;
}

QString ExchangeAction::DrugData::activePrinciple() const
{
    return _activePrinciple;
}

void ExchangeAction::DrugData::setActivePrinciple(const QString &activePrinciple)
{
    _activePrinciple = activePrinciple;
}

QString ExchangeAction::DrugData::brandName() const
{
    return _brandName;
}

void ExchangeAction::DrugData::setBrandName(const QString &brandName)
{
    _brandName = brandName;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Dosage* data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::DosageData::_TAG_DOSAGES = "dosages";
const char *const ExchangeAction::DosageData::_TAG_DOSAGE = "dosage";
const char *const ExchangeAction::DosageData::_TAG_START_DATE = "startDate";
const char *const ExchangeAction::DosageData::_TAG_LAST_DATE = "lastDate";
const char *const ExchangeAction::DosageData::_TAG_DOSE = "dose";
const char *const ExchangeAction::DosageData::_TAG_INTERVAL = "interval";
const char *const ExchangeAction::DosageData::_TAG_INFUSION = "infusion";
const char *const ExchangeAction::DosageData::_TAG_INTAKE = "intake";

QDomDocumentFragment ExchangeAction::DosageData::toXml(QDomDocument doc, const QList<DosageData> &dosages)
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the dosages
    QDomElement dosagesNode = doc.createElement(_TAG_DOSAGES);
    fragment.appendChild(dosagesNode);

    //Insert each dosage
    for (int i = 0; i < dosages.size(); ++i)
        dosagesNode.appendChild(dosages.at(i).toXml(doc));

    return fragment;
}

QDomDocumentFragment ExchangeAction::DosageData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the dosage
    QDomElement dosageNode = doc.createElement(_TAG_DOSAGE);
    fragment.appendChild(dosageNode);

    //Insert the start date
    QDomElement startDateNode = doc.createElement(_TAG_START_DATE);
    QDomText startDateContent = doc.createTextNode(startDate().toString(Action::xmlDateTimeFormat()));

    startDateNode.appendChild(startDateContent);
    dosageNode.appendChild(startDateNode);

    //Insert the last date
    QDomElement lastDateNode = doc.createElement(_TAG_LAST_DATE);
    QDomText lastDateContent = doc.createTextNode(lastDate().toString(Action::xmlDateTimeFormat()));

    lastDateNode.appendChild(lastDateContent);
    dosageNode.appendChild(lastDateNode);

    //Insert the dose
    QDomElement doseNode = doc.createElement(_TAG_DOSE);
    dosageNode.appendChild(doseNode);

    QDomElement doseValueNode = doc.createElement(_TAG_VALUE);
    QDomText doseValueContent = doc.createTextNode(QString::number(dose().value()));

    doseValueNode.appendChild(doseValueContent);
    doseNode.appendChild(doseValueNode);

    QDomElement doseUnitNode = doc.createElement(_TAG_UNIT);
    QDomText doseUnitContent = doc.createTextNode(dose().unit().name());

    doseUnitNode.appendChild(doseUnitContent);
    doseNode.appendChild(doseUnitNode);

    //Insert the interval
    QDomElement intervalNode = doc.createElement(_TAG_INTERVAL);
    dosageNode.appendChild(intervalNode);

    QDomElement intervalValueNode = doc.createElement(_TAG_VALUE);
    QDomText intervalValueContent = doc.createTextNode(QString::number(interval().toHours()));

    intervalValueNode.appendChild(intervalValueContent);
    intervalNode.appendChild(intervalValueNode);

    QDomElement intervalUnitNode = doc.createElement(_TAG_UNIT);
    QDomText intervalUnitContent = doc.createTextNode("h");

    intervalUnitNode.appendChild(intervalUnitContent);
    intervalNode.appendChild(intervalUnitNode);

    //Insert the infusion
    QDomElement infusionNode = doc.createElement(_TAG_INFUSION);
    dosageNode.appendChild(infusionNode);

    QDomElement infusionValueNode = doc.createElement(_TAG_VALUE);
    QDomText infusionValueContent = doc.createTextNode(QString::number(infusion().toMinutes()));

    infusionValueNode.appendChild(infusionValueContent);
    infusionNode.appendChild(infusionValueNode);

    QDomElement infusionUnitNode = doc.createElement(_TAG_UNIT);
    QDomText infusionUnitContent = doc.createTextNode("min");

    infusionUnitNode.appendChild(infusionUnitContent);
    infusionNode.appendChild(infusionUnitNode);

    //Insert the intake
    QDomElement intakeNode = doc.createElement(_TAG_INTAKE);
    QDomText intakeContent = doc.createTextNode(intake());

    intakeNode.appendChild(intakeContent);
    dosageNode.appendChild(intakeNode);

    //Insert the comments
    QDomElement commentsNode = doc.createElement(_TAG_COMMENTS);
    dosageNode.appendChild(commentsNode);

    return fragment;
}

QDateTime ExchangeAction::DosageData::startDate() const
{
    return _startDate;
}

void ExchangeAction::DosageData::setStartDate(const QDateTime &startDate)
{
    _startDate = startDate;
}

QDateTime ExchangeAction::DosageData::lastDate() const
{
    return _lastDate;
}

void ExchangeAction::DosageData::setLastDate(const QDateTime &lastDate)
{
    _lastDate = lastDate;
}

Amount ExchangeAction::DosageData::dose() const
{
    return _dose;
}

void ExchangeAction::DosageData::setDose(const Amount &dose)
{
    _dose = dose;
}

Duration ExchangeAction::DosageData::interval() const
{
    return _interval;
}

void ExchangeAction::DosageData::setInterval(const Duration &interval)
{
    _interval = interval;
}

Duration ExchangeAction::DosageData::infusion() const
{
    return _infusion;
}

void ExchangeAction::DosageData::setInfusion(const Duration &infusion)
{
    _infusion = infusion;
}

QString ExchangeAction::DosageData::intake() const
{
    return _intake;
}

void ExchangeAction::DosageData::setIntake(const QString &intake)
{
    _intake = intake;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Sample data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::SampleData::_TAG_SAMPLES = "samples";
const char *const ExchangeAction::SampleData::_TAG_SAMPLE = "sample";
const char *const ExchangeAction::SampleData::_TAG_ID = "id";
const char *const ExchangeAction::SampleData::_TAG_SAMPLE_DATE = "sampleDate";
const char *const ExchangeAction::SampleData::_TAG_ARRIVAL_DATE = "arrivalDate";
const char *const ExchangeAction::SampleData::_TAG_ANALYTE = "analyte";
const char *const ExchangeAction::SampleData::_TAG_CONCENTRATION = "concentration";

QDomDocumentFragment ExchangeAction::SampleData::toXml(QDomDocument doc, const QList<SampleData> &samples)
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the samples
    QDomElement samplesNode = doc.createElement(_TAG_SAMPLES);
    fragment.appendChild(samplesNode);

    //Insert each sample
    for (int i = 0; i < samples.size(); ++i)
        samplesNode.appendChild(samples.at(i).toXml(doc));

    return fragment;
}

QDomDocumentFragment ExchangeAction::SampleData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the sample
    QDomElement sampleNode = doc.createElement(_TAG_SAMPLE);
    fragment.appendChild(sampleNode);

    //Insert the ID
    QDomElement idNode = doc.createElement(_TAG_ID);
    QDomText idContent = doc.createTextNode(id());

    idNode.appendChild(idContent);
    sampleNode.appendChild(idNode);

    //Insert the sample date
    QDomElement sampleDateNode = doc.createElement(_TAG_SAMPLE_DATE);
    QDomText sampleDateContent = doc.createTextNode(sampleDate().toString(Action::xmlDateTimeFormat()));

    sampleDateNode.appendChild(sampleDateContent);
    sampleNode.appendChild(sampleDateNode);

    //Insert the arrival date
    QDomElement arrivalDateNode = doc.createElement(_TAG_ARRIVAL_DATE);
    QDomText arrivalDateContent = doc.createTextNode(arrivalDate().toString(Action::xmlDateTimeFormat()));

    arrivalDateNode.appendChild(arrivalDateContent);
    sampleNode.appendChild(arrivalDateNode);

    //Insert the analyte
    QDomElement analyteNode = doc.createElement(_TAG_ANALYTE);
    QDomText analyteContent = doc.createTextNode(analyte());

    analyteNode.appendChild(analyteContent);
    sampleNode.appendChild(analyteNode);

    //Insert the concentration
    QDomElement concNode = doc.createElement(_TAG_CONCENTRATION);
    sampleNode.appendChild(concNode);

    QDomElement concValueNode = doc.createElement(_TAG_VALUE);
    QDomText concValueContent = doc.createTextNode(QString::number(concentration().value()));

    concValueNode.appendChild(concValueContent);
    concNode.appendChild(concValueNode);

    QDomElement concUnitNode = doc.createElement(_TAG_UNIT);
    QDomText concUnitContent = doc.createTextNode(concentration().unit().name());

    concUnitNode.appendChild(concUnitContent);
    concNode.appendChild(concUnitNode);

    //Insert the comments
    QDomElement commentsNode = doc.createElement(_TAG_COMMENTS);
    sampleNode.appendChild(commentsNode);

    return fragment;
}

QString ExchangeAction::SampleData::id() const
{
    return _id;
}

void ExchangeAction::SampleData::setId(const QString &id)
{
    _id = id;
}

QString ExchangeAction::SampleData::analyte() const
{
    return _analyte;
}

void ExchangeAction::SampleData::setAnalyte(const QString &analyte)
{
    _analyte = analyte;
}

QDateTime ExchangeAction::SampleData::sampleDate() const
{
    return _sampleDate;
}

void ExchangeAction::SampleData::setSampleDate(const QDateTime &sampleDate)
{
    _sampleDate = sampleDate;
}

QDateTime ExchangeAction::SampleData::arrivalDate() const
{
    return _arrivalDate;
}

void ExchangeAction::SampleData::setArrivalDate(const QDateTime &arrivalDate)
{
    _arrivalDate = arrivalDate;
}

Amount ExchangeAction::SampleData::concentration() const
{
    return _concentration;
}

void ExchangeAction::SampleData::setConcentration(const Amount &concentration)
{
    _concentration = concentration;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Covariate data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::CovariateData::_TAG_COVARIATES = "covariates";
const char *const ExchangeAction::CovariateData::_TAG_COVARIATE = "covariate";
const char *const ExchangeAction::CovariateData::_TAG_NAME = "name";
const char *const ExchangeAction::CovariateData::_TAG_DATE = "date";
const char *const ExchangeAction::CovariateData::_TAG_VALUE = "value";
const char *const ExchangeAction::CovariateData::_TAG_NATURE = "nature";

QDomDocumentFragment ExchangeAction::CovariateData::toXml(QDomDocument doc, const QList<ExchangeAction::CovariateData> &covariates)
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the covariates
    QDomElement covariatesNode = doc.createElement(_TAG_COVARIATES);
    fragment.appendChild(covariatesNode);

    //Insert each covariate
    for (int i = 0; i < covariates.size(); ++i)
        covariatesNode.appendChild(covariates.at(i).toXml(doc));

    return fragment;
}

QDomDocumentFragment ExchangeAction::CovariateData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the covariate
    QDomElement covariateNode = doc.createElement(_TAG_COVARIATE);
    fragment.appendChild(covariateNode);

    //Insert the name
    QDomElement nameNode = doc.createElement(_TAG_NAME);
    QDomText nameContent = doc.createTextNode(name());

    nameNode.appendChild(nameContent);
    covariateNode.appendChild(nameNode);

    //Inserte the date
    QDomElement dateNode = doc.createElement(_TAG_DATE);
    QDomText dateContent = doc.createTextNode(date().toString(Action::xmlDateTimeFormat()));

    dateNode.appendChild(dateContent);
    covariateNode.appendChild(dateNode);

    //Insert the value
    QDomElement valueNode = doc.createElement(_TAG_VALUE);
    covariateNode.appendChild(valueNode);

    QDomElement valueValueNode = doc.createElement(_TAG_VALUE);
    QDomText valueValueContent = doc.createTextNode(QString::number(value()));

    valueValueNode.appendChild(valueValueContent);
    valueNode.appendChild(valueValueNode);

    QDomElement valueUnitNode = doc.createElement(_TAG_UNIT);
    QDomText valueUnitContent = doc.createTextNode(unit().name());

    valueUnitNode.appendChild(valueUnitContent);
    valueNode.appendChild(valueUnitNode);

    //Insert the nature
    QDomElement natureNode = doc.createElement(_TAG_NATURE);
    QDomText natureContent = doc.createTextNode(nature());

    natureNode.appendChild(natureContent);
    covariateNode.appendChild(natureNode);

    //Insert the comments
    QDomElement commentsNode = doc.createElement(_TAG_COMMENTS);
    covariateNode.appendChild(commentsNode);

    return fragment;
}

QString ExchangeAction::CovariateData::name() const
{
    return _name;
}

void ExchangeAction::CovariateData::setName(const QString &name)
{
    _name = name;
}

QString ExchangeAction::CovariateData::nature() const
{
    return _nature;
}

void ExchangeAction::CovariateData::setNature(const QString &nature)
{
    _nature = nature;
}

QDateTime ExchangeAction::CovariateData::date() const
{
    return _date;
}

void ExchangeAction::CovariateData::setDate(const QDateTime &date)
{
    _date = date;
}

double ExchangeAction::CovariateData::value() const
{
    return _value;
}

void ExchangeAction::CovariateData::setValue(double value)
{
    _value = value;
}

Unit ExchangeAction::CovariateData::unit() const
{
    return _unit;
}

void ExchangeAction::CovariateData::setUnit(const Unit &unit)
{
    _unit = unit;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Parameter data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::ParameterData::_TAG_PARAMETERS = "parameters";
const char *const ExchangeAction::ParameterData::_TAG_PARAMETER = "parameter";
const char *const ExchangeAction::ParameterData::_TAG_NAME = "name";
const char *const ExchangeAction::ParameterData::_TAG_DATE = "date";
const char *const ExchangeAction::ParameterData::_TAG_TYPICAL = "typical";
const char *const ExchangeAction::ParameterData::_TAG_APRIORI = "apriori";
const char *const ExchangeAction::ParameterData::_TAG_APOSTERIORI = "aposteriori";

QDomDocumentFragment ExchangeAction::ParameterData::toXml(QDomDocument doc, const QList<ExchangeAction::ParameterData> &parameters)
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the parameters
    QDomElement parametersNode = doc.createElement(_TAG_PARAMETERS);
    fragment.appendChild(parametersNode);

    //Insert each paramter
    for (int i = 0; i < parameters.size(); ++i)
        parametersNode.appendChild(parameters.at(i).toXml(doc));

    return fragment;
}

QDomDocumentFragment ExchangeAction::ParameterData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the parameter
    QDomElement parameterNode = doc.createElement(_TAG_PARAMETER);
    fragment.appendChild(parameterNode);

    //Insert the name
    QDomElement nameNode = doc.createElement(_TAG_NAME);
    QDomText nameContent = doc.createTextNode(name());

    nameNode.appendChild(nameContent);
    parameterNode.appendChild(nameNode);

    //Insert the date
    QDomElement dateNode = doc.createElement(_TAG_DATE);
    QDomText dateContent = doc.createTextNode(date().toString(Action::xmlDateTimeFormat()));

    dateNode.appendChild(dateContent);
    parameterNode.appendChild(dateNode);

    //Insert the typical value
    QDomElement typicalNode = doc.createElement(_TAG_TYPICAL);
    parameterNode.appendChild(typicalNode);

    QDomElement typicalValueNode = doc.createElement(_TAG_VALUE);
    QDomText typicalValueContent = doc.createTextNode(QString::number(typicalValue()));

    typicalValueNode.appendChild(typicalValueContent);
    typicalNode.appendChild(typicalValueNode);

    QDomElement typicalUnitNode = doc.createElement(_TAG_UNIT);
    QDomText typicalUnitContent = doc.createTextNode(typicalUnit().name());

    typicalUnitNode.appendChild(typicalUnitContent);
    typicalNode.appendChild(typicalUnitNode);

    //Insert the a priori value
    QDomElement aprioriNode = doc.createElement(_TAG_APRIORI);
    parameterNode.appendChild(aprioriNode);

    QDomElement aprioriValueNode = doc.createElement(_TAG_VALUE);
    QDomText aprioriValueContent = doc.createTextNode(QString::number(aprioriValue()));

    aprioriValueNode.appendChild(aprioriValueContent);
    aprioriNode.appendChild(aprioriValueNode);

    QDomElement aprioriUnitNode = doc.createElement(_TAG_UNIT);
    QDomText aprioriUnitContent = doc.createTextNode(aprioriUnit().name());

    aprioriUnitNode.appendChild(aprioriUnitContent);
    aprioriNode.appendChild(aprioriUnitNode);

    //Insert the a posteriori value
    QDomElement aposterioriNode = doc.createElement(_TAG_APOSTERIORI);
    parameterNode.appendChild(aposterioriNode);

    QDomElement aposterioriValueNode = doc.createElement(_TAG_VALUE);
    QDomText aposterioriValueContent = doc.createTextNode(QString::number(aposterioriValue()));

    aposterioriValueNode.appendChild(aposterioriValueContent);
    aposterioriNode.appendChild(aposterioriValueNode);

    QDomElement aposterioriUnitNode = doc.createElement(_TAG_UNIT);
    QDomText aposterioriUnitContent = doc.createTextNode(aposterioriUnit().name());

    aposterioriUnitNode.appendChild(aposterioriUnitContent);
    aposterioriNode.appendChild(aposterioriUnitNode);

    //Insert the comments
    QDomElement commentsNode = doc.createElement(_TAG_COMMENTS);
    parameterNode.appendChild(commentsNode);

    return fragment;
}

QString ExchangeAction::ParameterData::name() const
{
    return _name;
}

void ExchangeAction::ParameterData::setName(const QString &name)
{
    _name = name;
}

QDateTime ExchangeAction::ParameterData::date() const
{
    return _date;
}

void ExchangeAction::ParameterData::setDate(const QDateTime &date)
{
    _date = date;
}

double ExchangeAction::ParameterData::typicalValue() const
{
    return _typicalValue;
}

void ExchangeAction::ParameterData::setTypicalValue(double typicalValue)
{
    _typicalValue = typicalValue;
}

Unit ExchangeAction::ParameterData::typicalUnit() const
{
    return _typicalUnit;
}

void ExchangeAction::ParameterData::setTypicalUnit(const Unit &typicalUnit)
{
    _typicalUnit = typicalUnit;
}

double ExchangeAction::ParameterData::aprioriValue() const
{
    return _aprioriValue;
}

void ExchangeAction::ParameterData::setAprioriValue(double aprioriValue)
{
    _aprioriValue = aprioriValue;
}

Unit ExchangeAction::ParameterData::aprioriUnit() const
{
    return _aprioriUnit;
}

void ExchangeAction::ParameterData::setAprioriUnit(const Unit &aprioriUnit)
{
    _aprioriUnit = aprioriUnit;
}

double ExchangeAction::ParameterData::aposterioriValue() const
{
    return _aposterioriValue;
}

void ExchangeAction::ParameterData::setAposterioriValue(double aposterioriValue)
{
    _aposterioriValue = aposterioriValue;
}

Unit ExchangeAction::ParameterData::aposterioriUnit() const
{
    return _aposterioriUnit;
}

void ExchangeAction::ParameterData::setAposterioriUnit(const Unit &aposterioriUnit)
{
    _aposterioriUnit = aposterioriUnit;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Clinical data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::ClinicalData::_TAG_CLINICALS = "clinicals";
const char *const ExchangeAction::ClinicalData::_TAG_CLINICAL = "clinical";
const char *const ExchangeAction::ClinicalData::_TAG_NAME = "name";
const char *const ExchangeAction::ClinicalData::_TAG_DATE = "date";
const char *const ExchangeAction::ClinicalData::_TAG_VALUE = "value";

QDomDocumentFragment ExchangeAction::ClinicalData::toXml(QDomDocument doc, const QList<ExchangeAction::ClinicalData> &clinicals)
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the clinicals
    QDomElement clinicalsNode = doc.createElement(_TAG_CLINICALS);
    fragment.appendChild(clinicalsNode);

    //Insert each clinical
    for (int i = 0; i < clinicals.size(); ++i)
        clinicalsNode.appendChild(clinicals.at(i).toXml(doc));

    return fragment;
}

QDomDocumentFragment ExchangeAction::ClinicalData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the clinical
    QDomElement clinicalNode = doc.createElement(_TAG_CLINICAL);
    fragment.appendChild(clinicalNode);

    //Insert the name
    QDomElement nameNode = doc.createElement(_TAG_NAME);
    QDomText nameContent = doc.createTextNode(name());

    nameNode.appendChild(nameContent);
    clinicalNode.appendChild(nameNode);

    //Insert the date
    QDomElement dateNode = doc.createElement(_TAG_DATE);
    QDomText dateContent = doc.createTextNode(date().toString(Action::xmlDateTimeFormat()));

    dateNode.appendChild(dateContent);
    clinicalNode.appendChild(dateNode);

    //Insert the value
    QDomElement valueNode = doc.createElement(_TAG_VALUE);
    QDomText valueContent = doc.createTextNode(value());

    valueNode.appendChild(valueContent);
    clinicalNode.appendChild(valueNode);

    //Insert the comments
    QDomElement commentsNode = doc.createElement(_TAG_COMMENTS);
    clinicalNode.appendChild(commentsNode);

    return fragment;
}

QString ExchangeAction::ClinicalData::name() const
{
    return _name;
}

void ExchangeAction::ClinicalData::setName(const QString &name)
{
    _name = name;
}

QString ExchangeAction::ClinicalData::value() const
{
    return _value;
}

void ExchangeAction::ClinicalData::setValue(const QString &value)
{
    _value = value;
}

QDateTime ExchangeAction::ClinicalData::date() const
{
    return _date;
}

void ExchangeAction::ClinicalData::setDate(const QDateTime &date)
{
    _date = date;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Email data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::EmailData::_TAG_EMAILS = "emails";
const char *const ExchangeAction::EmailData::_TAG_EMAIL = "email";
const char *const ExchangeAction::EmailData::_ATTRIBUTE_TYPE = "type";

QDomDocumentFragment ExchangeAction::EmailData::toXml(QDomDocument doc, const QList<ExchangeAction::EmailData> &emails)
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the emails
    QDomElement emailsNode = doc.createElement(_TAG_EMAILS);
    fragment.appendChild(emailsNode);

    //Insert each email
    for (int i = 0; i < emails.size(); ++i)
        emailsNode.appendChild(emails.at(i).toXml(doc));

    return fragment;
}

QDomDocumentFragment ExchangeAction::EmailData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the email
    QDomElement emailNode = doc.createElement(_TAG_EMAIL);
    QDomText emailContent = doc.createTextNode(email());

    emailNode.appendChild(emailContent);
    fragment.appendChild(emailNode);

    //Insert the type
    QDomAttr typeAttribute = doc.createAttribute(_ATTRIBUTE_TYPE);
    typeAttribute.setValue(type());

    emailNode.setAttributeNode(typeAttribute);

    return fragment;
}

QString ExchangeAction::EmailData::email() const
{
    return _email;
}

void ExchangeAction::EmailData::setEmail(const QString &email)
{
    _email = email;
}

QString ExchangeAction::EmailData::type() const
{
    return _type;
}

void ExchangeAction::EmailData::setType(const QString &type)
{
    _type = type;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Phone data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::PhoneData::_TAG_PHONES = "phones";
const char *const ExchangeAction::PhoneData::_TAG_PHONE = "phone";
const char *const ExchangeAction::PhoneData::_ATTRIBUTE_TYPE = "type";

QDomDocumentFragment ExchangeAction::PhoneData::toXml(QDomDocument doc, const QList<PhoneData> &phones)
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the phones
    QDomElement phonesNode = doc.createElement(_TAG_PHONES);
    fragment.appendChild(phonesNode);

    //Insert each phone
    for (int i = 0; i < phones.size(); ++i)
        phonesNode.appendChild(phones.at(i).toXml(doc));

    return fragment;
}

QDomDocumentFragment ExchangeAction::PhoneData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the phone
    QDomElement phoneNode = doc.createElement(_TAG_PHONE);
    QDomText phoneContent = doc.createTextNode(number());

    phoneNode.appendChild(phoneContent);
    fragment.appendChild(phoneNode);

    //Insert the type
    QDomAttr typeAttribute = doc.createAttribute(_ATTRIBUTE_TYPE);
    typeAttribute.setValue(type());

    phoneNode.setAttributeNode(typeAttribute);

    return fragment;
}

QString ExchangeAction::PhoneData::number() const
{
    return _number;
}

void ExchangeAction::PhoneData::setNumber(const QString &number)
{
    _number = number;
}

QString ExchangeAction::PhoneData::type() const
{
    return _type;
}

void ExchangeAction::PhoneData::setType(const QString &type)
{
    _type = type;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Name data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::NameData::_TAG_NAME = "name";
const char *const ExchangeAction::NameData::_TAG_FIRST_NAME = "firstName";
const char *const ExchangeAction::NameData::_TAG_MIDDLE_NAME = "middleName";
const char *const ExchangeAction::NameData::_TAG_LAST_NAME = "lastName";

QDomDocumentFragment ExchangeAction::NameData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the name
    QDomElement nameNode = doc.createElement(_TAG_NAME);
    fragment.appendChild(nameNode);

    //Insert the firstname
    QDomElement firstnameNode = doc.createElement(_TAG_FIRST_NAME);
    QDomText firstnameContent = doc.createTextNode(firstname());

    firstnameNode.appendChild(firstnameContent);
    nameNode.appendChild(firstnameNode);

    //Insert the middle name
    QDomElement middlenameNode = doc.createElement(_TAG_MIDDLE_NAME);
    QDomText middlenameContent = doc.createTextNode(middlename());

    middlenameNode.appendChild(middlenameContent);
    nameNode.appendChild(middlenameNode);

    //Insert the last name
    QDomElement lastnameNode = doc.createElement(_TAG_LAST_NAME);
    QDomText lastnameContent = doc.createTextNode(lastname());

    lastnameNode.appendChild(lastnameContent);
    nameNode.appendChild(lastnameNode);

    return fragment;
}

QString ExchangeAction::NameData::firstname() const
{
    return _firstname;
}

void ExchangeAction::NameData::setFirstname(const QString &firstname)
{
    _firstname = firstname;
}

QString ExchangeAction::NameData::middlename() const
{
    return _middlename;
}

void ExchangeAction::NameData::setMiddlename(const QString &middlename)
{
    _middlename = middlename;
}

QString ExchangeAction::NameData::lastname() const
{
    return _lastname;
}

void ExchangeAction::NameData::setLastname(const QString &lastname)
{
    _lastname = lastname;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Contact data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::ContactData::_TAG_CONTACT = "contact";
const char *const ExchangeAction::ContactData::_TAG_ADDRESS = "address";
const char *const ExchangeAction::ContactData::_TAG_CITY = "city";
const char *const ExchangeAction::ContactData::_TAG_POSTCODE = "postcode";
const char *const ExchangeAction::ContactData::_TAG_STATE = "state";
const char *const ExchangeAction::ContactData::_TAG_COUNTRY = "country";

ExchangeAction::ContactData::ContactData() : _ignored(true)
{

}

QDomDocumentFragment ExchangeAction::ContactData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Check if valid
    if (ignored())
        return fragment;

    //Insert the contact
    QDomElement contactNode = doc.createElement(_TAG_CONTACT);
    fragment.appendChild(contactNode);

    //Insert the address
    QDomElement addressNode = doc.createElement(_TAG_ADDRESS);
    QDomText addressContent = doc.createTextNode(address());

    addressNode.appendChild(addressContent);
    contactNode.appendChild(addressNode);

    //Insert the city
    QDomElement cityNode = doc.createElement(_TAG_CITY);
    QDomText cityContent = doc.createTextNode(city());

    cityNode.appendChild(cityContent);
    contactNode.appendChild(cityNode);

    //Insert the postcode
    QDomElement postcodeNode = doc.createElement(_TAG_POSTCODE);
    QDomText postcodeContent = doc.createTextNode(postcode());

    postcodeNode.appendChild(postcodeContent);
    contactNode.appendChild(postcodeNode);

    //Insert the state
    QDomElement stateNode = doc.createElement(_TAG_STATE);
    QDomText stateContent = doc.createTextNode(state());

    stateNode.appendChild(stateContent);
    contactNode.appendChild(stateNode);

    //Insert the country
    QDomElement countryNode = doc.createElement(_TAG_COUNTRY);
    QDomText countryContent = doc.createTextNode(country());

    countryNode.appendChild(countryContent);
    contactNode.appendChild(countryNode);

    //Insert the emails and phones
    contactNode.appendChild(EmailData::toXml(doc, emails()));
    contactNode.appendChild(PhoneData::toXml(doc, phones()));

    return fragment;
}

QString ExchangeAction::ContactData::address() const
{
    return _address;
}

void ExchangeAction::ContactData::setAddress(const QString &address)
{
    _address = address;
}

QString ExchangeAction::ContactData::city() const
{
    return _city;
}

void ExchangeAction::ContactData::setCity(const QString &city)
{
    _city = city;
}

QString ExchangeAction::ContactData::postcode() const
{
    return _postcode;
}

void ExchangeAction::ContactData::setPostcode(const QString &postcode)
{
    _postcode = postcode;
}

QString ExchangeAction::ContactData::state() const
{
    return _state;
}

void ExchangeAction::ContactData::setState(const QString &state)
{
    _state = state;
}

QString ExchangeAction::ContactData::country() const
{
    return _country;
}

void ExchangeAction::ContactData::setCountry(const QString &country)
{
    _country = country;
}

QList<ExchangeAction::EmailData> ExchangeAction::ContactData::emails() const
{
    return _emails;
}

void ExchangeAction::ContactData::setEmails(const QList<EmailData> &emails)
{
    _emails = emails;
}

QList<ExchangeAction::PhoneData> ExchangeAction::ContactData::phones() const
{
    return _phones;
}

void ExchangeAction::ContactData::setPhones(const QList<PhoneData> &phones)
{
    _phones = phones;
}

bool ExchangeAction::ContactData::ignored() const
{
    return _ignored;
}

void ExchangeAction::ContactData::setIgnored(bool ignored)
{
    _ignored = ignored;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// PostEngine data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::PostEngineData::_TAG_POSTENGINE = "postengine";
const char *const ExchangeAction::PostEngineData::_TAG_NAME = "name";
const char *const ExchangeAction::PostEngineData::_TAG_VERSION = "version";
const char *const ExchangeAction::PostEngineData::_TAG_DRUGRESPONSEANALYSIS = "drugresponseanalysis";

QDomDocumentFragment ExchangeAction::PostEngineData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the patient
    QDomElement postengineNode = doc.createElement(_TAG_POSTENGINE);
    fragment.appendChild(postengineNode);

    QDomElement nameNode = doc.createElement(_TAG_NAME);
    QDomText nameContent = doc.createTextNode(name());

    nameNode.appendChild(nameContent);
    postengineNode.appendChild(nameNode);

    QDomElement versionNode = doc.createElement(_TAG_VERSION);
    QDomText versionContent = doc.createTextNode(version());

    versionNode.appendChild(versionContent);
    postengineNode.appendChild(versionNode);

    return fragment;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// DrugResponseAnalysis data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::DrugResponseAnalysisData::_TAG_DRUGRESPONSEANALYSIS = "drugresponseanalysis";
const char *const ExchangeAction::DrugResponseAnalysisData::_TAG_POSTENGINENAME = "postenginename";
const char *const ExchangeAction::DrugResponseAnalysisData::_TAG_POSTENGINEVERSION = "postengineversion";
const char *const ExchangeAction::DrugResponseAnalysisData::_TAG_PERCENTILEENGINENAME = "percentileenginename";
const char *const ExchangeAction::DrugResponseAnalysisData::_TAG_PERCENTILEENGINEVERSION = "percentileengineversion";
const char *const ExchangeAction::DrugResponseAnalysisData::_TAG_REVERSEENGINENAME = "reverseenginename";
const char *const ExchangeAction::DrugResponseAnalysisData::_TAG_REVERSEENGINEVERSION = "reverseengineversion";
const char *const ExchangeAction::DrugResponseAnalysisData::_TAG_DRUGTREATMENTNAME = "drugtreatmentname";
const char *const ExchangeAction::DrugResponseAnalysisData::_TAG_DRUGMODELNAME = "drugmodelname";

QDomDocumentFragment ExchangeAction::DrugResponseAnalysisData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the patient
    QDomElement drugresponseanalysisNode = doc.createElement(_TAG_DRUGRESPONSEANALYSIS);
    fragment.appendChild(drugresponseanalysisNode);

    QDomElement postenginenameNode = doc.createElement(_TAG_POSTENGINENAME);
    QDomText postenginenameContent = doc.createTextNode(postEngineName());

    postenginenameNode.appendChild(postenginenameContent);
    drugresponseanalysisNode.appendChild(postenginenameNode);

    QDomElement postengineversionNode = doc.createElement(_TAG_POSTENGINEVERSION);
    QDomText postengineversionContent = doc.createTextNode(postEngineVersion());

    postengineversionNode.appendChild(postengineversionContent);
    drugresponseanalysisNode.appendChild(postengineversionNode);

    QDomElement percentileenginenameNode = doc.createElement(_TAG_PERCENTILEENGINENAME);
    QDomText percentileenginenameContent = doc.createTextNode(percentileEngineName());

    percentileenginenameNode.appendChild(percentileenginenameContent);
    drugresponseanalysisNode.appendChild(percentileenginenameNode);

    QDomElement percentileengineversionNode = doc.createElement(_TAG_PERCENTILEENGINEVERSION);
    QDomText percentileengineversionContent = doc.createTextNode(percentileEngineVersion());

    percentileengineversionNode.appendChild(percentileengineversionContent);
    drugresponseanalysisNode.appendChild(percentileengineversionNode);

    QDomElement reverseenginenameNode = doc.createElement(_TAG_REVERSEENGINENAME);
    QDomText reverseenginenameContent = doc.createTextNode(reverseEngineName());

    reverseenginenameNode.appendChild(reverseenginenameContent);
    drugresponseanalysisNode.appendChild(reverseenginenameNode);

    QDomElement reverseengineversionNode = doc.createElement(_TAG_REVERSEENGINEVERSION);
    QDomText reverseengineversionContent = doc.createTextNode(reverseEngineVersion());

    reverseengineversionNode.appendChild(reverseengineversionContent);
    drugresponseanalysisNode.appendChild(reverseengineversionNode);

    QDomElement drugtreatmentnameNode = doc.createElement(_TAG_DRUGTREATMENTNAME);
    QDomText drugtreatmentnameContent = doc.createTextNode(drugTreatmentName());

    drugtreatmentnameNode.appendChild(drugtreatmentnameContent);
    drugresponseanalysisNode.appendChild(drugtreatmentnameNode);

    return fragment;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Intitute data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::InstituteData::_TAG_INSTITUTE = "institute";
const char *const ExchangeAction::InstituteData::_TAG_ID = "id";
const char *const ExchangeAction::InstituteData::_TAG_NAME = "name";

ExchangeAction::InstituteData::InstituteData() : _ignored(true)
{

}

QDomDocumentFragment ExchangeAction::InstituteData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Check if valid
    if (ignored())
        return fragment;

    //Insert the institute
    QDomElement instituteNode = doc.createElement(_TAG_INSTITUTE);
    fragment.appendChild(instituteNode);

    //Insert the ID
    QDomElement idNode = doc.createElement(_TAG_ID);
    QDomText idContent = doc.createTextNode(id());

    idNode.appendChild(idContent);
    instituteNode.appendChild(idNode);

    //Insert the name
    QDomElement nameNode = doc.createElement(_TAG_NAME);
    QDomText nameContent = doc.createTextNode(name());

    nameNode.appendChild(nameContent);
    instituteNode.appendChild(nameNode);

    //Insert the contact
    instituteNode.appendChild(contact().toXml(doc));

    return fragment;
}

QString ExchangeAction::InstituteData::id() const
{
    return _id;
}

void ExchangeAction::InstituteData::setId(const QString &id)
{
    _id = id;
}

QString ExchangeAction::InstituteData::name() const
{
    return _name;
}

void ExchangeAction::InstituteData::setName(const QString &name)
{
    _name = name;
}

ExchangeAction::ContactData ExchangeAction::InstituteData::contact() const
{
    return _contact;
}

void ExchangeAction::InstituteData::setContact(const ContactData &contact)
{
    _contact = contact;
}

bool ExchangeAction::InstituteData::ignored() const
{
    return _ignored;
}

void ExchangeAction::InstituteData::setIgnored(bool ignored)
{
    _ignored = ignored;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Prediction data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::PredictionData::_TAG_PREDICTION = "prediction";
const char *const ExchangeAction::PredictionData::_TAG_NAME = "name";
const char *const ExchangeAction::PredictionData::_TAG_COMMENT = "comment";
const char *const ExchangeAction::PredictionData::_TAG_FIRSTTAKE = "firsttake";
const char *const ExchangeAction::PredictionData::_TAG_PARAMSTYPE = "paramstype";
const char *const ExchangeAction::PredictionData::_TAG_CURVETYPE = "curvetype";
const char *const ExchangeAction::PredictionData::_TAG_DRUGRESPONSE = "drugresponse";
const char *const ExchangeAction::PredictionData::_TAG_DRUGTREATMENT = "drugtreatment";
const char *const ExchangeAction::PredictionData::_TAG_ENDDATE = "enddate";
const char *const ExchangeAction::PredictionData::_TAG_STARTDATE = "startdate";
const char *const ExchangeAction::PredictionData::_TAG_NBPOINTS = "nbpoints";
const char *const ExchangeAction::PredictionData::_TAG_STEADYSTATE = "steadystate";

QDomDocumentFragment ExchangeAction::PredictionData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the patient
    QDomElement predictionNode = doc.createElement(_TAG_PREDICTION);
    fragment.appendChild(predictionNode);

    QDomElement nameNode = doc.createElement(_TAG_NAME);
    QDomText nameContent = doc.createTextNode(name());

    nameNode.appendChild(nameContent);
    predictionNode.appendChild(nameNode);

    QDomElement commentNode = doc.createElement(_TAG_COMMENT);
    QDomText commentContent = doc.createTextNode(comment());

    commentNode.appendChild(commentContent);
    predictionNode.appendChild(commentNode);

    QDomElement curvetypeNode = doc.createElement(_TAG_CURVETYPE);
    QDomText curvetypeContent = doc.createTextNode(curveType());

    curvetypeNode.appendChild(curvetypeContent);
    predictionNode.appendChild(curvetypeNode);

    QDomElement paramstypeNode = doc.createElement(_TAG_PARAMSTYPE);
    QDomText paramstypeContent = doc.createTextNode(paramsType());

    paramstypeNode.appendChild(paramstypeContent);
    predictionNode.appendChild(paramstypeNode);

    return fragment;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Patient data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::PatientData::_TAG_PATIENT = "patient";
const char *const ExchangeAction::PatientData::_TAG_ID = "id";
const char *const ExchangeAction::PatientData::_TAG_STAY_NUMBER = "stayNumber";
const char *const ExchangeAction::PatientData::_TAG_BIRTHDATE = "birthdate";
const char *const ExchangeAction::PatientData::_TAG_GENDER = "gender";

QDomDocumentFragment ExchangeAction::PatientData::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the patient
    QDomElement patientNode = doc.createElement(_TAG_PATIENT);
    fragment.appendChild(patientNode);

    //Insert name and contact
    patientNode.appendChild(name().toXml(doc));
    patientNode.appendChild(contact().toXml(doc));
    patientNode.appendChild(institute().toXml(doc));

    //Insert the ID
    QDomElement idNode = doc.createElement(_TAG_ID);
    QDomText idContent = doc.createTextNode(id());

    idNode.appendChild(idContent);
    patientNode.appendChild(idNode);

    //Insert the stay number
    QDomElement stayNumberNode = doc.createElement(_TAG_STAY_NUMBER);
    QDomText stayNumberContent = doc.createTextNode(stayNumber());

    stayNumberNode.appendChild(stayNumberContent);
    patientNode.appendChild(stayNumberNode);

    //Insert the birthdate
    QDomElement birthdateNode = doc.createElement(_TAG_BIRTHDATE);
    QDomText birthdateContent = doc.createTextNode(birthdate().toString(Action::xmlDateTimeFormat()));

    birthdateNode.appendChild(birthdateContent);
    patientNode.appendChild(birthdateNode);

    //Insert the gender
    QDomElement genderNode = doc.createElement(_TAG_GENDER);
    QDomText genderContent = doc.createTextNode(gender());

    genderNode.appendChild(genderContent);
    patientNode.appendChild(genderNode);

    //Insert the comments
    QDomElement commentsNode = doc.createElement(_TAG_COMMENTS);
    patientNode.appendChild(commentsNode);

    return fragment;
}

QString ExchangeAction::PatientData::id() const
{
    return _id;
}

void ExchangeAction::PatientData::setId(const QString &id)
{
    _id = id;
}

QString ExchangeAction::PatientData::stayNumber() const
{
    return _stayNumber;
}

void ExchangeAction::PatientData::setStayNumber(const QString &stayNumber)
{
    _stayNumber = stayNumber;
}

QDateTime ExchangeAction::PatientData::birthdate() const
{
    return _birthdate;
}

void ExchangeAction::PatientData::setBirthdate(const QDateTime &birthdate)
{
    _birthdate = birthdate;
}

QString ExchangeAction::PatientData::gender() const
{
    return _gender;
}

void ExchangeAction::PatientData::setGender(const QString &gender)
{
    _gender = gender;
}

ExchangeAction::NameData ExchangeAction::PatientData::name() const
{
    return _name;
}

void ExchangeAction::PatientData::setName(const NameData &name)
{
    _name = name;
}

ExchangeAction::ContactData ExchangeAction::PatientData::contact() const
{
    return _contact;
}

void ExchangeAction::PatientData::setContact(const ContactData &contact)
{
    _contact = contact;
}

ExchangeAction::InstituteData ExchangeAction::PatientData::institute() const
{
    return _institute;
}

void ExchangeAction::PatientData::setInstitute(const InstituteData &institute)
{
    _institute = institute;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Practician data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExchangeAction::PracticianData::_TAG_MANDATOR = "mandator";
const char *const ExchangeAction::PracticianData::_TAG_ANALYST = "analyst";
const char *const ExchangeAction::PracticianData::_TAG_ID = "id";
const char *const ExchangeAction::PracticianData::_TAG_TITLE = "title";
const char *const ExchangeAction::PracticianData::_TAG_BIRTHDATE = "birthdate";
const char *const ExchangeAction::PracticianData::_TAG_GENDER = "gender";

QDomDocumentFragment ExchangeAction::PracticianData::toXml(QDomDocument doc, const QString &type) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Check if valid
    if (ignored())
        return fragment;

    //Insert the patient
    QDomElement practicianNode = doc.createElement(type);
    fragment.appendChild(practicianNode);

    //Insert name and contact
    practicianNode.appendChild(name().toXml(doc));
    practicianNode.appendChild(contact().toXml(doc));
    practicianNode.appendChild(institute().toXml(doc));

    //Insert the ID
    QDomElement idNode = doc.createElement(_TAG_ID);
    QDomText idContent = doc.createTextNode(id());

    idNode.appendChild(idContent);
    practicianNode.appendChild(idNode);

    //Insert the title
    QDomElement titleNode = doc.createElement(_TAG_TITLE);
    QDomText titleContent = doc.createTextNode(title());

    titleNode.appendChild(titleContent);
    practicianNode.appendChild(titleNode);

    //Insert the birthdate
    QDomElement birthdateNode = doc.createElement(_TAG_BIRTHDATE);
    QDomText birthdateContent = doc.createTextNode(birthdate().toString(Action::xmlDateTimeFormat()));

    birthdateNode.appendChild(birthdateContent);
    practicianNode.appendChild(birthdateNode);

    //Insert the gender
    QDomElement genderNode = doc.createElement(_TAG_GENDER);
    QDomText genderContent = doc.createTextNode(gender());

    genderNode.appendChild(genderContent);
    practicianNode.appendChild(genderNode);

    //Insert the comments
    QDomElement commentsNode = doc.createElement(_TAG_COMMENTS);
    practicianNode.appendChild(commentsNode);

    return fragment;
}

ExchangeAction::PracticianData::PracticianData() : _ignored(true)
{

}

QString ExchangeAction::PracticianData::id() const
{
    return _id;
}

void ExchangeAction::PracticianData::setId(const QString &id)
{
    _id = id;
}

QString ExchangeAction::PracticianData::title() const
{
    return _title;
}

void ExchangeAction::PracticianData::setTitle(const QString &title)
{
    _title = title;
}

QDateTime ExchangeAction::PracticianData::birthdate() const
{
    return _birthdate;
}

void ExchangeAction::PracticianData::setBirthdate(const QDateTime &birthdate)
{
    _birthdate = birthdate;
}

QString ExchangeAction::PracticianData::gender() const
{
    return _gender;
}

void ExchangeAction::PracticianData::setGender(const QString &gender)
{
    _gender = gender;
}

ExchangeAction::NameData ExchangeAction::PracticianData::name() const
{
    return _name;
}

void ExchangeAction::PracticianData::setName(const NameData &name)
{
    _name = name;
}

ExchangeAction::ContactData ExchangeAction::PracticianData::contact() const
{
    return _contact;
}

void ExchangeAction::PracticianData::setContact(const ContactData &contact)
{
    _contact = contact;
}

ExchangeAction::InstituteData ExchangeAction::PracticianData::institute() const
{
    return _institute;
}

void ExchangeAction::PracticianData::setInstitute(const InstituteData &institute)
{
    _institute = institute;
}

bool ExchangeAction::PracticianData::ignored() const
{
    return _ignored;
}

void ExchangeAction::PracticianData::setIgnored(bool ignored)
{
    _ignored = ignored;
}

} //namespace cliutils
} //namespace ezechiel
