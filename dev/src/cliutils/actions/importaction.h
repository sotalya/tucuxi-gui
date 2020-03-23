#ifndef IMPORTACTION_H
#define IMPORTACTION_H

//#include "core/interfaces/genericdatabase.h"
#include "core/utils/errorenums.h"

#include "exchangeaction.h"
#include "core/dal/drugresponseanalysis.h"
#include "admin/src/dal/patient.h"

#include <QCoreApplication>

#include "admin/src/dal/institute.h"
#include "admin/src/dal/person.h"

class QDomElement;
class CurveModel;

namespace ezechiel {
namespace cliutils {
class ImportAction : public ExchangeAction
{
    Q_DECLARE_TR_FUNCTIONS(ImportAction)

public:

    //! The action static getters.
    static QString name();
    QString help();

    //! Constructor.
    ImportAction();

    //! The action run function.
    virtual bool run(const QStringList &args);

private:
    //The data set class
    class Dataset
    {
    public:
        //The entity tags
        static const char *const _TAG_DATASET;

        //Getters/setters
        QString lang() const;
        void setLang(const QString &lang);

        DrugData drug() const;
        void setDrug(const DrugData &drug);

        PatientData patient() const;
        void setPatient(const PatientData &patient);

        PracticianData mandator() const;
        void setMandator(const PracticianData &mandator);

        QList<DosageData> dosages() const;
        void setDosages(const QList<DosageData> &dosages);
        void addDosage(const DosageData &dosage);

        QList<SampleData> samples() const;
        void setSamples(const QList<SampleData> &samples);
        void addSample(const SampleData &sample);

        QList<CovariateData> covariates() const;
        void setCovariates(const QList<CovariateData> &covariates);
        void addCovariate(const CovariateData &covariate);

        QList<ClinicalData> clinicals() const;
        void setClinicals(const QList<ClinicalData> &clinicals);
        void addClinical(const ClinicalData &clinical);

        QList<PredictionData> predictions() const;
        void setPredictions(const QList<PredictionData> &predictions);
        void addPrediction(const PredictionData &prediction);

        QList<DrugResponseAnalysisData> responses() const {return _responses;}
        void setResponses(const QList<DrugResponseAnalysisData> &responses) {_responses = responses;}
        void addResponse(const DrugResponseAnalysisData &response) {_responses.append(response);}

    private:
        //The entity data
        QString _lang;
        DrugData _drug;
        PatientData _patient;
        PracticianData _mandator;
        QList<DosageData> _dosages;
        QList<SampleData> _samples;
        QList<CovariateData> _covariates;
        QList<ClinicalData> _clinicals;
        QList<PredictionData> _predictions;
        QList<DrugResponseAnalysisData> _responses;
    };

    //The document class
    class Document
    {
    public:
        //The entity tags
        static const char *const _TAG_ROOT;

        //The entity attributes
        static const char *const _ATTRIBUTE_VERSION;
        static const char *const _ATTRIBUTE_XSI;
        static const char *const _ATTRIBUTE_SCHEMA_LOCATION;

        //Constructor
        Document();
    };

    //The error messages
    static const char *const _NODE_LOCATION;
    static const char *const _INVALID_DATASET_STRUCT;
    static const char *const _INVALID_DATASET_DATA;
    static const char *const _INVALID_ENGINE;
    static const char *const _INVALID_CURVE;
    static const char *const _MISSING_TAG;
    static const char *const _MISSING_DATA;

    //The action commands
    static const char *const _ACTION_COMMAND;

    //The curve name template
    static const char *const _CURVE_NAME;
    static const char *const _CURVE_REGEX;

    //Initializes the data
    bool initializeAction(const QStringList &args);
    bool initializeRoot(const QStringList &args, QDomElement &root);

    //Extracts the datasets
    bool extractDatasets(const QDomElement &root);

    //Extracts the global data
    bool extractDrug(const QDomElement &root, Dataset &dataset);
    bool extractDosages(const QDomElement &root, Dataset &dataset);
    bool extractSamples(const QDomElement &root, Dataset &dataset);
    bool extractCovariates(const QDomElement &root, Dataset &dataset);
    bool extractClinicals(const QDomElement &root, Dataset &dataset);
    bool extractPatient(const QDomElement &root, Dataset &dataset);
    bool extractMandator(const QDomElement &root, Dataset &dataset);
    bool extractPredictions(const QDomElement &root, Dataset &dataset);
    bool extractDrugResponses(const QDomElement &root, Dataset &dataset);

    //Extracts the specific data
    bool extractName(const QDomElement &root, NameData &name);
    bool extractInstitute(const QDomElement &root, InstituteData &institute);
    bool extractContact(const QDomElement &root, ContactData &contact);
    bool extractEmails(const QDomElement &root, QList<EmailData> &emails);
    bool extractPhones(const QDomElement &root, QList<PhoneData> &phones);

    //Processes the datasets
    bool processDatasets();

    //Processes the global data
    bool processMandator(const Dataset &dataset);
    SharedPatient processPatient(const Dataset &dataset);
    DrugModel* processDrug(const Dataset &dataset);
    DrugResponseAnalysis* processResponses(const Dataset &dataset, SharedPatient patient, DrugModel* drug);
    bool processCovariates(const Dataset &dataset, SharedPatient patient);
    bool processClinicals(const Dataset &dataset, SharedPatient patient);
    bool processPredictions(const Dataset &dataset, DrugResponseAnalysis* response);

    //Processes the specific data
    bool processInstitute(const InstituteData &instituteData, SharedInstitute &institute);
    bool processContact(const ContactData &contactData, SharedPerson &person);
    bool processDosages(const Dataset &dataset, DrugResponseAnalysis* resp);
    bool processSamples(const Dataset &dataset, DrugResponseAnalysis* curve);

    //Helper functions
    QString getDrugId(const Dataset &dataset) const;
    QString getModelId(const QString &drugId) const;

    //The data sets
    QList<Dataset> _datasets;

    //The shared data
    QString _peId;
    QString _poId;
    QString _reId;
};

} //namespace cliutils
} //namespace ezechiel
#endif // IMPORTACTION_H
