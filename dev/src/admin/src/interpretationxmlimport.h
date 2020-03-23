#ifndef INTERPRETATIONXMLIMPORT_H
#define INTERPRETATIONXMLIMPORT_H


#include <QXmlStreamReader>

#include <QTextStream>
#include <QFile>


class Interpretation;
class InterpretationAnalysis;
class InterpretationRequest;
class Practician;
class ValidationStatus;
class Patient;
class Person;
class ClinicalSet;
class Clinical;

namespace ezechiel {
namespace core {
class PredictionSpec;
class DrugResponseAnalysis;
class DrugTreatment;
class DrugModel;
class DosageHistory;
class PatientVariateList;
class TargetList;
class CoreMeasureList;
class IdentifiableAmount;
class OperableAmount;
class UncastedValueList;
class ADME;
class ValidDoses;
class ValidInfusions;
class ValidIntervals;
class DrugVariateList;
class ParameterSet;
class OperationList;
class Parameter;
class TranslatableString;
class ErrorModel;
class Target;
class DrugVariate;
class PatientVariate;
class Dosage;
class AdjustmentDosage;
class DosageHistory;
class Bsv;
}

}

class InterpretationXmlImport
{
public:
    InterpretationXmlImport();
    // The only method to be public
    Interpretation *load(const QByteArray &data);

protected:
    QXmlStreamReader reader;

    bool isOk;

    QString _errorString;

    void checkValidity(bool condition);

    void checkReaderError();

    Interpretation *loadInterpretation(const QString &tagName, QObject *parent = nullptr);
    std::vector<double> loadPercentiles(const QString &tagName, QObject *parent);
    ezechiel::core::DrugResponseAnalysis *loadDrugResponseAnalysis(const QString &tagName, QObject *parent);

    void loadAdjustment(const QString &tagName, Interpretation *interpretation);
    ezechiel::core::DrugModel *loadDrugModel(const QString &tagName, QObject *parent);
    ezechiel::core::DrugTreatment *loadDrugTreatment(const QString &tagName, QObject *parent);
    ezechiel::core::OperationList *loadOperations(const QString &tagName, QObject *parent);
    ezechiel::core::IdentifiableAmount *loadIdentifiableAmount(const QString &tagName, QObject *parent);
    ezechiel::core::OperableAmount *loadOperableAmount(const QString &tagName, QObject *parent);
    ezechiel::core::ValidDoses *loadValidDoses(const QString &tagName, QObject *parent);
    ezechiel::core::ValidIntervals *loadValidIntervals(const QString &tagName, QObject *parent);
    ezechiel::core::ValidInfusions *loadValidInfusions(const QString &tagName, QObject *parent);
    ezechiel::core::PatientVariateList *loadPatientVariates(const QString &tagName, QObject *parent);
    ezechiel::core::PatientVariate *loadPatientVariate(const QString &tagName, QObject *parent);
    ezechiel::core::DosageHistory *loadDosageHistory(const QString &tagName, QObject *parent, bool isAdjustment = false);
    ezechiel::core::AdjustmentDosage *loadAdjustmentDosage(const QString &tagName, QObject *parent);
    ezechiel::core::Dosage *loadDosage(const QString &tagName, QObject *parent);
    ezechiel::core::ADME *loadAdme(const QString &tagName, QObject *parent);
    ezechiel::core::TargetList *loadTargets(const QString &tagName, QObject *parent);

    ezechiel::core::Target *loadTarget(const QString &tagName, QObject *parent);

    ezechiel::core::DrugVariateList *loadCovariates(const QString &tagName, QObject *parent);

    ezechiel::core::DrugVariate *loadCovariate(const QString &tagName, QObject *parent);

    ezechiel::core::ErrorModel *loadErrorModel(const QString &tagName, QObject *parent);

    ezechiel::core::ParameterSet *loadParameters(const QString &tagName, QObject *parent);

    ezechiel::core::Parameter *loadParameter(const QString &tagName, QObject *parent);

    ezechiel::core::Bsv *loadBsv(const QString &tagName, QObject *parent);

    InterpretationAnalysis *loadAnalysis(const QString &tagName, QObject *parent);
    InterpretationRequest *loadRequest(const QString &tagName, QObject *parent);
    Practician *loadPractician(const QString &tagName, QObject *parent);
    Patient *loadPatient(const QString &tagName, QObject *parent);
    Person *loadPerson(const QString &tagName, QObject *parent);
    ValidationStatus *loadValidationStatus(const QString &tagName, QObject *parent);
    ClinicalSet *loadClinicals(const QString &tagName, QObject *parent);
    Clinical *loadClinical(const QString &tagName, QObject *parent);




    QString extractor(QXmlStreamAttributes *attributes = nullptr,bool readText = true);

    QMetaType::Type extractType();

    QDateTime extractDate(bool essential = true);

    bool extractBool();

    int extractInt();

    double extractDouble();

    QMetaType::Type qStringToType(QString stringType,bool &isConvertible);

    QString errorString() const;

    void raiseConversionError();
};

#endif // INTERPRETATIONXMLIMPORT_H
