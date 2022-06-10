#ifndef PREDICTIONSPECIMPORTER_H
#define PREDICTIONSPECIMPORTER_H


#include <QXmlStreamReader>

#include <QTextStream>
#include <QFile>


namespace Tucuxi {
namespace Gui {
namespace Core {

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
class DosageHistory;
class Bsv;
class StandardTreatment;
}
}
}

class PredictionSpecImporter
{
public:
    PredictionSpecImporter();
    // The only method to be public
    Tucuxi::Gui::Core::PredictionSpec *load(const QByteArray &data);

protected:
    QXmlStreamReader reader;

    bool isOk;

    QString _errorString;

    void checkValidity(bool condition);

    void checkReaderError();

    Tucuxi::Gui::Core::PredictionSpec *loadPredictionSpec(const QString &tagName, QObject *parent = nullptr);
    std::vector<double> loadPercentiles(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::DrugResponseAnalysis *loadDrugResponseAnalysis(const QString &tagName, QObject *parent);

    Tucuxi::Gui::Core::DrugModel *loadDrugModel(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::DrugTreatment *loadDrugTreatment(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::OperationList *loadOperations(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::IdentifiableAmount *loadIdentifiableAmount(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::OperableAmount *loadOperableAmount(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::StandardTreatment *loadStandardTreatment(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::ValidDoses *loadValidDoses(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::ValidIntervals *loadValidIntervals(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::ValidInfusions *loadValidInfusions(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::PatientVariateList *loadPatientVariates(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::PatientVariate *loadPatientVariate(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::DosageHistory *loadDosageHistory(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::Dosage *loadDosage(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::ADME *loadAdme(const QString &tagName, QObject *parent);
    Tucuxi::Gui::Core::TargetList *loadTargets(const QString &tagName, QObject *parent);

    Tucuxi::Gui::Core::Target *loadTarget(const QString &tagName, QObject *parent);

    Tucuxi::Gui::Core::DrugVariateList *loadCovariates(const QString &tagName, QObject *parent);

    Tucuxi::Gui::Core::DrugVariate *loadCovariate(const QString &tagName, QObject *parent);

    Tucuxi::Gui::Core::ErrorModel *loadErrorModel(const QString &tagName, QObject *parent);

    Tucuxi::Gui::Core::ParameterSet *loadParameters(const QString &tagName, QObject *parent);

    Tucuxi::Gui::Core::Parameter *loadParameter(const QString &tagName, QObject *parent);

    Tucuxi::Gui::Core::Bsv *loadBsv(const QString &tagName, QObject *parent);

    QString extractor(QXmlStreamAttributes *attributes = nullptr,bool readText = true);

    QMetaType::Type extractType();

    QDateTime extractDate(bool essential = true);

    bool extractBool();

    double extractDouble();

    QMetaType::Type qStringToType(QString stringType,bool &isConvertible);

    QString errorString() const;

    void raiseConversionError();
};


#endif // PREDICTIONSPECIMPORTER_H
