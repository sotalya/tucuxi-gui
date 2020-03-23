#ifndef EXCHANGEACTION_H
#define EXCHANGEACTION_H

#include <QDateTime>
#include <QCoreApplication>
#include <QDomDocument>
#include <QDomDocumentFragment>

#include "action.h"
#include "core/dal/amount.h"
#include "core/utils/duration.h"
#include "core/dal/prediction.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/predictionspec.h"
#include "core/utils/version.h"


namespace ezechiel {
namespace cliutils {
using namespace core;

class ExchangeAction : public Action
{
    Q_DECLARE_TR_FUNCTIONS(ExchangeAction)

public:

    //! Constructor.
    ExchangeAction();

    //! Virtual run function
    virtual bool run(const QStringList &args) = 0;

protected:
    //The interpretation class
    class InterpretationData
    {
    public:
        //The entity tags
        static const char *const _TAG_INTERPRETATION;
        static const char *const _TAG_ID;
        static const char *const _TAG_EXPECTEDNESS;
        static const char *const _TAG_SUITABILITY;
        static const char *const _TAG_PREDICTION;
        static const char *const _TAG_REMONITORING;
        static const char *const _TAG_WARNING;
        static const char *const _TAG_CURVE;
        static const char *const _TAG_DATE;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString id() const;
        void setId(const QString &id);

        QString expectedness() const;
        void setExpectedness(const QString &expectedness);

        QString suitability() const;
        void setSuitability(const QString &suitability);

        QString prediction() const;
        void setPrediction(const QString &prediction);

        QString remonitoring() const;
        void setRemonitoring(const QString &remonitoring);

        QString warning() const;
        void setWarning(const QString &warning);

        QByteArray curve() const;
        void setCurve(const QByteArray &curve);

        QDateTime date() const;
        void setDate(const QDateTime &date);

    private:
        //The entity data
        QString _id;
        QString _expectedness;
        QString _suitability;
        QString _prediction;
        QString _remonitoring;
        QString _warning;
        QByteArray _curve;
        QDateTime _date;
    };

    //The drug class
    class DrugData
    {
    public:
        //The entity tags
        static const char *const _TAG_DRUG;
        static const char *const _TAG_ID;
        static const char *const _TAG_ATC;
        static const char *const _TAG_BRAND;
        static const char *const _TAG_PRINCIPLE;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString id() const;
        void setId(const QString &id);

        QString atc() const;
        void setAtc(const QString &atc);

        QString brandName() const;
        void setBrandName(const QString &brandName);

        QString activePrinciple() const;
        void setActivePrinciple(const QString &activePrinciple);

    private:
        //The entity data
        QString _id;
        QString _atc;
        QString _brandName;
        QString _activePrinciple;
    };

    //The dosage class
    class DosageData
    {
    public:
        //The entity tags
        static const char *const _TAG_DOSAGES;
        static const char *const _TAG_DOSAGE;
        static const char *const _TAG_START_DATE;
        static const char *const _TAG_LAST_DATE;
        static const char *const _TAG_DOSE;
        static const char *const _TAG_INTERVAL;
        static const char *const _TAG_INFUSION;
        static const char *const _TAG_INTAKE;

        //Convertions
        static QDomDocumentFragment toXml(QDomDocument doc, const QList<DosageData> &dosages);
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QDateTime startDate() const;
        void setStartDate(const QDateTime &startDate);

        QDateTime lastDate() const;
        void setLastDate(const QDateTime &lastDate);

        Amount dose() const;
        void setDose(const Amount &dose);

        Duration interval() const;
        void setInterval(const Duration &interval);

        Duration infusion() const;
        void setInfusion(const Duration &infusion);

        QString intake() const;
        void setIntake(const QString &intake);

    private:
        //The entity data
        QDateTime _startDate;
        QDateTime _lastDate;
        Amount _dose;
        Duration _interval;
        Duration _infusion;
        QString _intake;
    };

    //The sample class
    class SampleData
    {
    public:
        //The entity tags
        static const char *const _TAG_SAMPLES;
        static const char *const _TAG_SAMPLE;
        static const char *const _TAG_ID;
        static const char *const _TAG_SAMPLE_DATE;
        static const char *const _TAG_ARRIVAL_DATE;
        static const char *const _TAG_ANALYTE;
        static const char *const _TAG_CONCENTRATION;

        //Convertions
        static QDomDocumentFragment toXml(QDomDocument doc, const QList<SampleData> &samples);
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString id() const;
        void setId(const QString &id);

        QString analyte() const;
        void setAnalyte(const QString &analyte);

        QDateTime sampleDate() const;
        void setSampleDate(const QDateTime &sampleDate);

        QDateTime arrivalDate() const;
        void setArrivalDate(const QDateTime &arrivalDate);

        Amount concentration() const;
        void setConcentration(const Amount &concentration);

    private:
        //The entity data
        QString _id;
        QString _analyte;
        QDateTime _sampleDate;
        QDateTime _arrivalDate;
        Amount _concentration;
    };

    //The covariate class
    class CovariateData
    {
    public:
        //The entity tags
        static const char *const _TAG_COVARIATES;
        static const char *const _TAG_COVARIATE;
        static const char *const _TAG_NAME;
        static const char *const _TAG_DATE;
        static const char *const _TAG_VALUE;
        static const char *const _TAG_NATURE;

        //Convertions
        static QDomDocumentFragment toXml(QDomDocument doc, const QList<CovariateData> &covariates);
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString name() const;
        void setName(const QString &name);

        QString nature() const;
        void setNature(const QString &nature);

        QDateTime date() const;
        void setDate(const QDateTime &date);

        double value() const;
        void setValue(double value);

        Unit unit() const;
        void setUnit(const Unit &unit);

    private:
        //The entity data
        QString _name;
        QString _nature;
        QDateTime _date;
        double _value;
        Unit _unit;
    };

    //The parameter classe
    class ParameterData
    {
    public:
        //The entity tags
        static const char *const _TAG_PARAMETERS;
        static const char *const _TAG_PARAMETER;
        static const char *const _TAG_NAME;
        static const char *const _TAG_DATE;
        static const char *const _TAG_TYPICAL;
        static const char *const _TAG_APRIORI;
        static const char *const _TAG_APOSTERIORI;

        //Convertions
        static QDomDocumentFragment toXml(QDomDocument doc, const QList<ParameterData> &parameters);
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString name() const;
        void setName(const QString &name);

        QDateTime date() const;
        void setDate(const QDateTime &date);

        double typicalValue() const;
        void setTypicalValue(double typicalValue);

        Unit typicalUnit() const;
        void setTypicalUnit(const Unit &typicalUnit);

        double aprioriValue() const;
        void setAprioriValue(double aprioriValue);

        Unit aprioriUnit() const;
        void setAprioriUnit(const Unit &aprioriUnit);

        double aposterioriValue() const;
        void setAposterioriValue(double aposterioriValue);

        Unit aposterioriUnit() const;
        void setAposterioriUnit(const Unit &aposterioriUnit);

    private:
        //The entity data
        QString _name;
        QDateTime _date;
        double _typicalValue;
        Unit _typicalUnit;
        double _aprioriValue;
        Unit _aprioriUnit;
        double _aposterioriValue;
        Unit _aposterioriUnit;
    };

    //The clinicals class
    class ClinicalData
    {
    public:
        //The entity tags
        static const char *const _TAG_CLINICALS;
        static const char *const _TAG_CLINICAL;
        static const char *const _TAG_NAME;
        static const char *const _TAG_DATE;
        static const char *const _TAG_VALUE;

        //Convertions
        static QDomDocumentFragment toXml(QDomDocument doc, const QList<ClinicalData> &clinicals);
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString name() const;
        void setName(const QString &name);

        QString value() const;
        void setValue(const QString &value);

        QDateTime date() const;
        void setDate(const QDateTime &date);

    private:
        //The entity data
        QString _name;
        QString _value;
        QDateTime _date;
    };

    //The emails class
    class EmailData
    {
    public:
        //The entity tags
        static const char *const _TAG_EMAILS;
        static const char *const _TAG_EMAIL;

        //The entity attributes
        static const char *const _ATTRIBUTE_TYPE;

        //Convertions
        static QDomDocumentFragment toXml(QDomDocument doc, const QList<EmailData> &emails);
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString email() const;
        void setEmail(const QString &email);

        QString type() const;
        void setType(const QString &type);

    private:
        //The entity data
        QString _email;
        QString _type;
    };

    //The phones class
    class PhoneData
    {
    public:
        //The entity tags
        static const char *const _TAG_PHONES;
        static const char *const _TAG_PHONE;

        //The entity attributes
        static const char *const _ATTRIBUTE_TYPE;

        //Convertions
        static QDomDocumentFragment toXml(QDomDocument doc, const QList<PhoneData> &phones);
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString number() const;
        void setNumber(const QString &number);

        QString type() const;
        void setType(const QString &type);

    private:
        //The entity data
        QString _number;
        QString _type;
    };

    //The name class
    class NameData
    {
    public:
        //The entity tags
        static const char *const _TAG_NAME;
        static const char *const _TAG_FIRST_NAME;
        static const char *const _TAG_MIDDLE_NAME;
        static const char *const _TAG_LAST_NAME;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString firstname() const;
        void setFirstname(const QString &firstname);

        QString middlename() const;
        void setMiddlename(const QString &middlename);

        QString lastname() const;
        void setLastname(const QString &lastname);

    private:
        //The entity data
        QString _firstname;
        QString _middlename;
        QString _lastname;
    };

    //The contact class
    class ContactData
    {
    public:
        //The entity tags
        static const char *const _TAG_CONTACT;
        static const char *const _TAG_ADDRESS;
        static const char *const _TAG_CITY;
        static const char *const _TAG_POSTCODE;
        static const char *const _TAG_STATE;
        static const char *const _TAG_COUNTRY;

        //Constructor
        ContactData();

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString address() const;
        void setAddress(const QString &address);

        QString city() const;
        void setCity(const QString &city);

        QString postcode() const;
        void setPostcode(const QString &postcode);

        QString state() const;
        void setState(const QString &state);

        QString country() const;
        void setCountry(const QString &country);

        QList<EmailData> emails() const;
        void setEmails(const QList<EmailData> &emails);

        QList<PhoneData> phones() const;
        void setPhones(const QList<PhoneData> &phones);

        bool ignored() const;
        void setIgnored(bool ignored);

    private:
        //The entity data
        QString _address;
        QString _city;
        QString _postcode;
        QString _state;
        QString _country;
        QList<EmailData> _emails;
        QList<PhoneData> _phones;

        //The ignored flag
        bool _ignored;
    };

    class PredictionSpecData {

    public:
        static const char *const _TAG_PREDICTIONSPEC;
        static const char *const _TAG_STARTDATE;
        static const char *const _TAG_ENDDATE;
        static const char *const _TAG_STEADYSTATE;
        static const char *const _TAG_NBPOINTS;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;
        QDateTime startDate() const {return _startDate;}
        void setStartDate(const QDateTime startDate) {_startDate = startDate;}

        QDateTime endDate() const {return _endDate;}
        void setEndDate(const QDateTime endDate) {_endDate = endDate;}

        bool steadyState() const {return _steadyState;}
        void setSteadyState(const bool steadyState) {_steadyState = steadyState;}

        int nbPoints() const {return _nbPoints;}
        void setNbPoints(const int nbPoints) {_nbPoints = nbPoints;}

    private:
        QDateTime _startDate;
        QDateTime _endDate;
        bool _steadyState;
        int _nbPoints;
    };

    class DrugModelData {
    public:
        static const char *const _TAG_NAME;
        static const char *const _TAG_DRUGMODEL;
        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;
        QString name() const {return _name;}
        void setName(const QString &name) {_name = name;}
    private:
        QString _name;
    };

    class PostEngineData {
    public:
        static const char *const _TAG_POSTENGINE;
        static const char *const _TAG_NAME;
        static const char *const _TAG_VERSION;
        static const char *const _TAG_DRUGRESPONSEANALYSIS;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;
        int id() const {return _id;}
        void setId(const int &id) {_id = id;}

        QString name() const {return _name;}
        void setName(const QString &name) {_name = name;}

        QString version() const {return _version.toString();}
        void setVersion(const QString &version) {_version.fromString(version);}

//        DrugResponseAnalysisData drugResponseAnalysisData() const {return _responsedata;}
//        void setDrugResponseAnalysisData(DrugResponseAnalysisData &_data) {_responsedata = _data;}

    private:
        Version _version;
        QString _name;
        int _id;
//        DrugResponseAnalysisData _responsedata;
    };

    class ReverseEngineData {
    public:
        static const char *const _TAG_REVERSEENGINE;
        static const char *const _TAG_NAME;
        static const char *const _TAG_VERSION;
        static const char *const _TAG_DRUGRESPONSEANALYSIS;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;
        int id() const {return _id;}
        void setId(const int &id) {_id = id;}

        QString name() const {return _name;}
        void setName(const QString &name) {_name = name;}

        QString version() {return _version.toString();}
        void setVersion(const QString &version) {_version.fromString(version);}

//        DrugResponseAnalysisData drugResponseAnalysisData() const {return _responsedata;}
//        void setDrugResponseAnalysisData(DrugResponseAnalysisData &_data) {_responsedata = _data;}

    private:
        Version _version;
        QString _name;
        int _id;
//        DrugResponseAnalysisData _drugResponseAnalysisData;
    };

    class PercentileEngineData {
    public:
        static const char *const _TAG_PERCENTILEENGINE;
        static const char *const _TAG_NAME;
        static const char *const _TAG_VERSION;
        static const char *const _TAG_DRUGRESPONSEANALYSIS;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;
        int id() const {return _id;}
        void setId(const int &id) {_id = id;}

        QString name() const {return _name;}
        void setName(const QString &name) {_name = name;}

        QString version() {return _version.toString();}
        void setVersion(const QString &version) {_version.fromString(version);}

//        DrugResponseAnalysisData drugResponseAnalysisData() const {return _responsedata;}
//        void setDrugResponseAnalysisData(DrugResponseAnalysisData &_data) {_responsedata = _data;}

    private:
        Version _version;
        QString _name;
        int _id;
//        DrugResponseAnalysisData _drugResponseAnalysisData;
    };



    //The institute class
    class InstituteData
    {
    public:
        //The entity tags
        static const char *const _TAG_INSTITUTE;
        static const char *const _TAG_ID;
        static const char *const _TAG_NAME;

        //Constructor
        InstituteData();

        //Conversions
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setter
        QString id() const;
        void setId(const QString &id);

        QString name() const;
        void setName(const QString &name);

        ContactData contact() const;
        void setContact(const ContactData &contact);

        bool ignored() const;
        void setIgnored(bool ignored);

    private:
        //The entity data
        QString _id;
        QString _name;
        ContactData _contact;

        //The ignored flag
        bool _ignored;
    };

    //The patient class
    class PatientData
    {
    public:
        //The entity tags
        static const char *const _TAG_PATIENT;
        static const char *const _TAG_ID;
        static const char *const _TAG_STAY_NUMBER;
        static const char *const _TAG_BIRTHDATE;
        static const char *const _TAG_GENDER;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString id() const;
        void setId(const QString &id);

        QString stayNumber() const;
        void setStayNumber(const QString &stayNumber);

        QString gender() const;
        void setGender(const QString &gender);

        NameData name() const;
        void setName(const NameData &name);

        QDateTime birthdate() const;
        void setBirthdate(const QDateTime &birthdate);

        ContactData contact() const;
        void setContact(const ContactData &contact);

        InstituteData institute() const;
        void setInstitute(const InstituteData &institute);

    private:
        //The entity data
        QString _id;
        QString _stayNumber;
        QString _gender;
        QDateTime _birthdate;
        NameData _name;
        ContactData _contact;
        InstituteData _institute;
    };

    class DrugTreatmentData {
    public:
        static const char *const _TAG_DRUGTREATMENT;
        static const char *const _TAG_NAME;
        static const char *const _TAG_PATIENT;
        static const char *const _TAG_DRUG;
        static const char *const _TAG_DRUGMODEL;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;
        QString name() const {return _name;}
        void setName(const QString &name) {_name = name;}

        PatientData patientData() const {return _patient;}
        void setPatientData(PatientData& _data) {_patient = _data;}

        DrugData drugData() const {return _drug;}
        void setDrugData(DrugData& _data) {_drug = _data;}

        DrugModelData drugModel() const {return _drugmodel;}
        void setDrugModel(DrugModelData _data) {_drugmodel = _data;}
    private:
        QString _name;
        PatientData _patient;
        DrugData _drug;
        DrugModelData _drugmodel;

    };

    class DrugResponseAnalysisData {

    public:
        static const char *const _TAG_POSTENGINENAME;
        static const char *const _TAG_POSTENGINEVERSION;
        static const char *const _TAG_PERCENTILEENGINENAME;
        static const char *const _TAG_PERCENTILEENGINEVERSION;
        static const char *const _TAG_REVERSEENGINENAME;
        static const char *const _TAG_REVERSEENGINEVERSION;
        static const char *const _TAG_DRUGTREATMENTNAME;
        static const char *const _TAG_DRUGRESPONSEANALYSIS;
        static const char *const _TAG_DRUGMODELNAME;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;

        QString postEngineName() const {return _postEngineName;}
        void setPostEngineName(QString _data) {_postEngineName = _data;}

        QString postEngineVersion() const {return _postEngineVersion;}
        void setPostEngineVersion(QString _data) {_postEngineVersion = _data;}

        QString percentileEngineName() const {return _percentileEngineName;}
        void setPercentileEngineName(QString _data) {_percentileEngineName = _data;}

        QString percentileEngineVersion() const {return _percentileEngineVersion;}
        void setPercentileEngineVersion(QString _data) {_percentileEngineVersion = _data;}

        QString reverseEngineName() const {return _reverseEngineName;}
        void setReverseEngineName(QString _data) {_reverseEngineName  =_data;}

        QString reverseEngineVersion() const {return _reverseEngineVersion;}
        void setReverseEngineVersion(QString _data) {_reverseEngineVersion =_data;}

        QString drugTreatmentName() const {return _drugTreatmentName;}
        void setDrugTreatmentName(QString _data) {_drugTreatmentName = _data;}

        QString drugTreatmentVersion() const {return _drugTreatmentVersion;}
        void setDrugTreatmentVersion(QString _data) {_drugTreatmentVersion = _data;}

        QString drugModelName() const {return _drugModelName;}
        void setDrugModelName(QString _data) {_drugModelName = _data;}

    private:
        QString _postEngineName;
        QString _postEngineVersion;
        QString _percentileEngineName;
        QString _percentileEngineVersion;
        QString _reverseEngineName;
        QString _reverseEngineVersion;
        QString _drugTreatmentName;
        QString _drugTreatmentVersion;
        QString _drugModelName;
    };


    class PredictionData
    {
    public:
        static const char *const _TAG_PREDICTION;
        static const char *const _TAG_NAME;
        static const char *const _TAG_COMMENT;
        static const char *const _TAG_FIRSTTAKE;
        static const char *const _TAG_PARAMSTYPE;
        static const char *const _TAG_CURVETYPE;
        static const char *const _TAG_DRUGRESPONSE;
        static const char *const _TAG_DRUGTREATMENT;
        static const char *const _TAG_STARTDATE;
        static const char *const _TAG_ENDDATE;
        static const char *const _TAG_NBPOINTS;
        static const char *const _TAG_STEADYSTATE;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;

        QString name() const {return _name;}
        void setName(const QString &name) {_name = name;}

        QString comment() const {return _comment;}
        void setComment(const QString &comment) {_comment = comment;}

        QString startdate() const {return _startdate;}
        void setStartdate(const QString &startdate) {_startdate = startdate;}

        QString enddate() const {return _enddate;}
        void setEnddate(const QString &enddate) {_enddate = enddate;}

        QString steadyState() const {return _steadystate;}
        void setSteadyState(const QString &steadyState) {_steadystate = steadyState;}

        QString nbPoints() const {return _nbpoints;}
        void setNbPoints(const QString &nbPoints) {_nbpoints = nbPoints;}

//        QDateTime firsttake() const {return treatment()->dosages()->firsttake();}
//        void setFirsttake(const QDateTime &firsttake) {treatment()->dosages()->setFirsttake(firsttake);}

        QString paramsType() const {return _paramstype;}
        void setParamsType(const QString &paramstype) {_paramstype = paramstype;}

        /** \brief Returns the translated curve's type.
         * @return The translated curve type.
         */
        static QString translatedCurveType(CurveType type);

        /** \brief Returns the curve's type.
         * @return The curve's type
         */
        QString curveType() const {return _curvetype;}
        void setCurveType(const QString curvetype) {_curvetype = curvetype;}

        QString drugResponseAnalysisData() const {return _responsedata;}
        void setDrugResponseAnalysisData(QString &_data) {_responsedata = _data;}

        QString drugTreatmentData() const {return _drugtreatdata;}
        void setDrugTreatmentData(QString &_data) {_drugtreatdata = _data;}

        QString predictionSpecData() const {return _predspecdata;}
        void setPredictionSpecData(QString _data) {_predspecdata = _data;}


    private:
        QString _curvetype;
        bool _modified;
        QString _paramstype;
        QString _name;
        QString _comment;
        QString _predspecdata;
        QString _drugtreatdata;
        QString _responsedata;
        QString _nbpoints;
        QString _steadystate;
        QString _startdate;
        QString _enddate;
    };

    //The practician class
    class PracticianData
    {
    public:
        //The entity tags
        static const char *const _TAG_MANDATOR;
        static const char *const _TAG_ANALYST;
        static const char *const _TAG_ID;
        static const char *const _TAG_TITLE;
        static const char *const _TAG_BIRTHDATE;
        static const char *const _TAG_GENDER;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc, const QString &type) const;

        //Constructor
        PracticianData();

        //Getters/setters
        QString id() const;
        void setId(const QString &id);

        QString title() const;
        void setTitle(const QString &title);

        QString gender() const;
        void setGender(const QString &gender);

        QDateTime birthdate() const;
        void setBirthdate(const QDateTime &birthdate);

        NameData name() const;
        void setName(const NameData &name);

        ContactData contact() const;
        void setContact(const ContactData &contact);

        InstituteData institute() const;
        void setInstitute(const InstituteData &institute);

        bool ignored() const;
        void setIgnored(bool ignored);

    private:
        //The entity data
        QString _id;
        QString _title;
        QString _gender;
        QDateTime _birthdate;
        NameData _name;
        ContactData _contact;
        InstituteData _institute;

        //The ignored flag
        bool _ignored;
    };

    //The error strings
    static const char *const _DATABASE_READ_FAILED;
    static const char *const _DATABASE_WRITE_FAILED;
    static const char *const _INVALID_XML_FILE;

    //The gender strings
    static const char *const _MALE;
    static const char *const _FEMALE;

    //The global/shared tags
    static const char *const _TAG_VALUE;
    static const char *const _TAG_UNIT;
    static const char *const _TAG_COMMENTS;

    //The global/shared attributes
    static const char *const _ATTRIBUTE_LANG;

    //The duration conversions
    static const int _FROM_SECONDS;
    static const int _FROM_MINUTES;
    static const int _FROM_HOURS;
    static const int _FROM_DAYS;
    static const int _FROM_WEEK;
};

} //namespace cliutils
} //namespace ezechiel
#endif // EXCHANGEACTION_H
