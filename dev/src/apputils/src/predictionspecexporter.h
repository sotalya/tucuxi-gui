#ifndef PREDICTIONSPECEXPORTER_H
#define PREDICTIONSPECEXPORTER_H

#include <QXmlStreamWriter>

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

}
}

class PredictionSpecExporter
{
public:
    PredictionSpecExporter();

    QString writeDate(QDateTime date);
    bool save(ezechiel::GuiCore::PredictionSpec *spec, QByteArray &data);

    void save(ezechiel::GuiCore::PredictionSpec *spec);
    void save(ezechiel::GuiCore::DrugResponseAnalysis *analysis);
    void save(ezechiel::GuiCore::DrugTreatment *treatment);
    void save(ezechiel::GuiCore::DrugModel *model);
    void save(ezechiel::GuiCore::DosageHistory *history);
    void save(ezechiel::GuiCore::PatientVariateList *list);
    void save(ezechiel::GuiCore::TargetList *list);
    void save(ezechiel::GuiCore::CoreMeasureList *list);
    void save(ezechiel::GuiCore::UncastedValueList *list);
    void save(ezechiel::GuiCore::ADME *adme);
    void save(ezechiel::GuiCore::ValidDoses *doses);
    void save(ezechiel::GuiCore::ValidInfusions *infusions);
    void save(ezechiel::GuiCore::ValidIntervals *intervals);
    void save(ezechiel::GuiCore::DrugVariateList *list);
    void save(ezechiel::GuiCore::ParameterSet *set);
    void save(ezechiel::GuiCore::OperationList *list);
    void saveIdentifiableAmount(const QString &tagName, ezechiel::GuiCore::IdentifiableAmount *amount);
    void saveOperableAmount(const QString &tagName, ezechiel::GuiCore::OperableAmount *amount);

protected:

    QXmlStreamWriter *writer;
};

#endif // PREDICTIONSPECEXPORTER_H
