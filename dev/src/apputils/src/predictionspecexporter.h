#ifndef PREDICTIONSPECEXPORTER_H
#define PREDICTIONSPECEXPORTER_H

#include <QXmlStreamWriter>

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

}
}

class PredictionSpecExporter
{
public:
    PredictionSpecExporter();

    QString writeDate(QDateTime date);
    bool save(ezechiel::core::PredictionSpec *spec, QByteArray &data);

    void save(ezechiel::core::PredictionSpec *spec);
    void save(ezechiel::core::DrugResponseAnalysis *analysis);
    void save(ezechiel::core::DrugTreatment *treatment);
    void save(ezechiel::core::DrugModel *model);
    void save(ezechiel::core::DosageHistory *history);
    void save(ezechiel::core::PatientVariateList *list);
    void save(ezechiel::core::TargetList *list);
    void save(ezechiel::core::CoreMeasureList *list);
    void save(ezechiel::core::UncastedValueList *list);
    void save(ezechiel::core::ADME *adme);
    void save(ezechiel::core::ValidDoses *doses);
    void save(ezechiel::core::ValidInfusions *infusions);
    void save(ezechiel::core::ValidIntervals *intervals);
    void save(ezechiel::core::DrugVariateList *list);
    void save(ezechiel::core::ParameterSet *set);
    void save(ezechiel::core::OperationList *list);
    void saveIdentifiableAmount(const QString &tagName, ezechiel::core::IdentifiableAmount *amount);
    void saveOperableAmount(const QString &tagName, ezechiel::core::OperableAmount *amount);

protected:

    QXmlStreamWriter *writer;
};

#endif // PREDICTIONSPECEXPORTER_H
