//@@license@@

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
class PhoneList;

namespace ezechiel {
namespace GuiCore {
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
class Admin;
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
    ezechiel::GuiCore::DrugResponseAnalysis *loadDrugResponseAnalysis(const QString &tagName, QObject *parent);

    void loadAdjustment(const QString &tagName, Interpretation *interpretation);
    ezechiel::GuiCore::DrugModel *loadDrugModel(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::DrugTreatment *loadDrugTreatment(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::OperationList *loadOperations(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::IdentifiableAmount *loadIdentifiableAmount(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::OperableAmount *loadOperableAmount(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::ValidDoses *loadValidDoses(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::ValidIntervals *loadValidIntervals(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::ValidInfusions *loadValidInfusions(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::PatientVariateList *loadPatientVariates(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::PatientVariate *loadPatientVariate(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::DosageHistory *loadDosageHistory(const QString &tagName, QObject *parent, bool isAdjustment = false);
    ezechiel::GuiCore::AdjustmentDosage *loadAdjustmentDosage(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::Dosage *loadDosage(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::ADME *loadAdme(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::TargetList *loadTargets(const QString &tagName, QObject *parent);

    ezechiel::GuiCore::Target *loadTarget(const QString &tagName, QObject *parent);

    ezechiel::GuiCore::DrugVariateList *loadCovariates(const QString &tagName, QObject *parent);

    ezechiel::GuiCore::DrugVariate *loadCovariate(const QString &tagName, QObject *parent);

    ezechiel::GuiCore::ErrorModel *loadErrorModel(const QString &tagName, QObject *parent);

    ezechiel::GuiCore::ParameterSet *loadParameters(const QString &tagName, QObject *parent);

    ezechiel::GuiCore::Parameter *loadParameter(const QString &tagName, QObject *parent);

    ezechiel::GuiCore::Bsv *loadBsv(const QString &tagName, QObject *parent);

    InterpretationAnalysis *loadAnalysis(const QString &tagName, QObject *parent);
    InterpretationRequest *loadRequest(const QString &tagName, QObject *parent);
    Practician *loadPractician(const QString &tagName, QObject *parent);
    Patient *loadPatient(const QString &tagName, QObject *parent);
    Person *loadPerson(const QString &tagName, QObject *parent);
    ValidationStatus *loadValidationStatus(const QString &tagName, QObject *parent);
    ClinicalSet *loadClinicals(const QString &tagName, QObject *parent);
    Clinical *loadClinical(const QString &tagName, QObject *parent);
    PhoneList *loadPhoneNumber(const QString &tagName, QObject *parent);

    ezechiel::GuiCore::Admin *loadAdmin(const QString &tagName, QObject *parent);




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
