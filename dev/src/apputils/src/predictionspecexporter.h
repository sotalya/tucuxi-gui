#ifndef PREDICTIONSPECEXPORTER_H
#define PREDICTIONSPECEXPORTER_H

#include <QXmlStreamWriter>

namespace Tucuxi {
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

}
}

class PredictionSpecExporter
{
public:
    PredictionSpecExporter();

    QString writeDate(QDateTime date);
    bool save(Tucuxi::GuiCore::PredictionSpec *spec, QByteArray &data);

    void save(Tucuxi::GuiCore::PredictionSpec *spec);
    void save(Tucuxi::GuiCore::DrugResponseAnalysis *analysis);
    void save(Tucuxi::GuiCore::DrugTreatment *treatment);
    void save(Tucuxi::GuiCore::DrugModel *model);
    void save(Tucuxi::GuiCore::DosageHistory *history);
    void save(Tucuxi::GuiCore::PatientVariateList *list);
    void save(Tucuxi::GuiCore::TargetList *list);
    void save(Tucuxi::GuiCore::CoreMeasureList *list);
    void save(Tucuxi::GuiCore::UncastedValueList *list);
    void save(Tucuxi::GuiCore::ADME *adme);
    void save(Tucuxi::GuiCore::ValidDoses *doses);
    void save(Tucuxi::GuiCore::ValidInfusions *infusions);
    void save(Tucuxi::GuiCore::ValidIntervals *intervals);
    void save(Tucuxi::GuiCore::DrugVariateList *list);
    void save(Tucuxi::GuiCore::ParameterSet *set);
    void save(Tucuxi::GuiCore::OperationList *list);
    void saveIdentifiableAmount(const QString &tagName, Tucuxi::GuiCore::IdentifiableAmount *amount);
    void saveOperableAmount(const QString &tagName, Tucuxi::GuiCore::OperableAmount *amount);

protected:

    QXmlStreamWriter *writer;
};

#endif // PREDICTIONSPECEXPORTER_H
