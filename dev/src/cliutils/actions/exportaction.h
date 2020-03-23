#ifndef EXPORTACTION_H
#define EXPORTACTION_H


#include <QCoreApplication>
#include <QDomDocumentFragment>

#include "admin/src/dal/person.h"
#include "core/utils/errorenums.h"
#include "core/utils/ezutils.h"
#include "core/dal/percentile.h"
#include "exchangeaction.h"

namespace ezechiel {
namespace cliutils {
class ExportAction : public ExchangeAction
{
    Q_DECLARE_TR_FUNCTIONS(ExportAction)

public:

    //! The action static getters.
    static QString name();
    QString help();

    //! Constructor.
    ExportAction();

    //! The action run function.
    virtual bool run(const QStringList &args);

private:
    //The data set class
    class Dataset
    {
    public:
        //The entity tags
        static const char *const _TAG_DATASET;

        //Convertions
        QDomDocumentFragment toXml(QDomDocument doc) const;

        //Getters/setters
        QString lang() const;
        void setLang(const QString &lang);

        InterpretationData interpretation() const;
        void setInterpretation(const InterpretationData &interpretation);

        DrugData drug() const;
        void setDrug(const DrugData &drug);

        PatientData patient() const;
        void setPatient(const PatientData &patient);

        PracticianData mandator() const;
        void setMandator(const PracticianData &mandator);

        PracticianData analyst() const;
        void setAnalyst(const PracticianData &analyst);

        QList<DosageData> dosages() const;
        void setDosages(const QList<DosageData> &dosages);
        void addDosage(const DosageData &dosage);

        QList<SampleData> samples() const;
        void setSamples(const QList<SampleData> &samples);
        void addSample(const SampleData &sample);

        QList<CovariateData> covariates() const;
        void setCovariates(const QList<CovariateData> &covariates);
        void addCovariate(const CovariateData &covariate);

        QList<ParameterData> parameters() const;
        void setParameters(const QList<ParameterData> &parameters);
        void addParameter(const ParameterData &parameter);

        QList<ClinicalData> clinicals() const;
        void setClinicals(const QList<ClinicalData> &clinicals);
        void addClinical(const ClinicalData &clinical);

    private:
        //The entity data
        QString _lang;
        InterpretationData _interpretation;
        DrugData _drug;
        PatientData _patient;
        PracticianData _mandator;
        PracticianData _analyst;
        QList<DosageData> _dosages;
        QList<SampleData> _samples;
        QList<CovariateData> _covariates;
        QList<ParameterData> _parameters;
        QList<ClinicalData> _clinicals;
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

        //Convertions
        QDomDocument toXml() const;
        QString toString(int indent = 2) const;

        //Getters/setters
        QList<Dataset> datasets() const;
        void setDatasets(const QList<Dataset> &datasets);
        void addDataset(const Dataset &dataset);

    private:
        //The entity data
        QList<Dataset> _datasets;
    };

    //The error messages
    static const char *const _INVALID_SUBCOMMAND;
    static const char *const _INVALID_ARGUMENTS;
    static const char *const _INVALID_ARGUMENT;
    static const char *const _INVALID_DATE;
    static const char *const _TOO_FEW_ARGUMENTS;
    static const char *const _TOO_MANY_ARGUMENTS;
    static const char *const _FILE_OPEN_ERROR;

    //The shared messages
    static const char *const _OVERRIDE_FILE;

    //The action commands
    static const char *const _ACTION_COMMAND;
    static const char *const _CMD_MODELS;
    static const char *const _CMD_INTERPRETATION;
    static const char *const _CMD_PARAM_LATEX;
    static const char *const _CMD_PARAM_HTML;
    static const char *const _CMD_PARAM_SPHINX;
    static const char *const _CMD_PARAM_CURVEID;
    static const char *const _CMD_PARAM_PERCENTILES;
    static const char *const _CMD_PARAM_MANDATORID;
    static const char *const _CMD_PARAM_ANALYSTID;

    //The subcommands functions
    bool exportModels(QStringList &args);
    bool exportInterpretation(QStringList &args);

    //The export model functions
    bool modelsToHtml(QTextStream &out);
    bool modelsToLatex(QTextStream &out);
    bool modelsToSphinx(QTextStream &out);

    //The export interpretation functions
    bool initializeInterpretation(QStringList &args, QDateTime &date, QString &filename, ident &patientId, ident &curveId, PercentileList &percentiles, ident &mandatorId, ident &analystId);

    //The datasets construction functions
    bool buildDataset(const QDateTime &date, const ident &patientId, const ident &curveId, const PercentileList &percentiles, const ident &mandatorId, const ident &analystId);
    bool buildInterpretation(Dataset &dataset, const QDateTime &date, const ident &patientId);
//    bool buildCurve(Dataset &dataset, const QDateTime &date, const ident &curveId, const PercentileList &percentiles, const uint nbPoints = 200);
    bool buildPatient(PatientData &patientData, const ident &patientId);
    bool buildPractician(PracticianData &practicianData, const ident &practicianId);
    bool buildInstitute(InstituteData &instituteData, const ident &instituteId);
    bool buildContact(ContactData &contactData, const SharedPerson &person);

    //The datasets exportation functions
    bool exportDatasets(const QString &filename);
    bool exportXmlDeclaration();

    //The utilities functions
    bool overrideFile(const QFile &file);

    //The current document
    Document _document;
};

} //namespace cliutils
} //namespace ezechiel
#endif // EXPORTACTION_H
