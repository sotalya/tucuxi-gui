#ifndef PREDICTIONSPECIMPORTER_H
#define PREDICTIONSPECIMPORTER_H


#include <QXmlStreamReader>

#include <QTextStream>
#include <QFile>


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
class DosageHistory;
class Bsv;
class StandardTreatment;
}

}

class PredictionSpecImporter
{
public:
    PredictionSpecImporter();
    // The only method to be public
    ezechiel::GuiCore::PredictionSpec *load(const QByteArray &data);

protected:
    QXmlStreamReader reader;

    bool isOk;

    QString _errorString;

    void checkValidity(bool condition);

    void checkReaderError();

    ezechiel::GuiCore::PredictionSpec *loadPredictionSpec(const QString &tagName, QObject *parent = nullptr);
    std::vector<double> loadPercentiles(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::DrugResponseAnalysis *loadDrugResponseAnalysis(const QString &tagName, QObject *parent);

    ezechiel::GuiCore::DrugModel *loadDrugModel(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::DrugTreatment *loadDrugTreatment(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::OperationList *loadOperations(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::IdentifiableAmount *loadIdentifiableAmount(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::OperableAmount *loadOperableAmount(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::StandardTreatment *loadStandardTreatment(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::ValidDoses *loadValidDoses(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::ValidIntervals *loadValidIntervals(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::ValidInfusions *loadValidInfusions(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::PatientVariateList *loadPatientVariates(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::PatientVariate *loadPatientVariate(const QString &tagName, QObject *parent);
    ezechiel::GuiCore::DosageHistory *loadDosageHistory(const QString &tagName, QObject *parent);
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
