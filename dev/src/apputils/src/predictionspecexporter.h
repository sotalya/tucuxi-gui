#ifndef PREDICTIONSPECEXPORTER_H
#define PREDICTIONSPECEXPORTER_H

#include <QXmlStreamWriter>

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

}
}
}

class PredictionSpecExporter
{
public:
    PredictionSpecExporter();

    QString writeDate(QDateTime date);
    bool save(Tucuxi::Gui::Core::PredictionSpec *spec, QByteArray &data);

    void save(Tucuxi::Gui::Core::PredictionSpec *spec);
    void save(Tucuxi::Gui::Core::DrugResponseAnalysis *analysis);
    void save(Tucuxi::Gui::Core::DrugTreatment *treatment);
    void save(Tucuxi::Gui::Core::DrugModel *model);
    void save(Tucuxi::Gui::Core::DosageHistory *history);
    void save(Tucuxi::Gui::Core::PatientVariateList *list);
    void save(Tucuxi::Gui::Core::TargetList *list);
    void save(Tucuxi::Gui::Core::CoreMeasureList *list);
    void save(Tucuxi::Gui::Core::UncastedValueList *list);
    void save(Tucuxi::Gui::Core::ADME *adme);
    void save(Tucuxi::Gui::Core::ValidDoses *doses);
    void save(Tucuxi::Gui::Core::ValidInfusions *infusions);
    void save(Tucuxi::Gui::Core::ValidIntervals *intervals);
    void save(Tucuxi::Gui::Core::DrugVariateList *list);
    void save(Tucuxi::Gui::Core::ParameterSet *set);
    void save(Tucuxi::Gui::Core::OperationList *list);
    void saveIdentifiableAmount(const QString &tagName, Tucuxi::Gui::Core::IdentifiableAmount *amount);
    void saveOperableAmount(const QString &tagName, Tucuxi::Gui::Core::OperableAmount *amount);

protected:

    QXmlStreamWriter *writer;
};

#endif // PREDICTIONSPECEXPORTER_H
