#ifndef PREDICTIONSPECCOPIER_H
#define PREDICTIONSPECCOPIER_H


#include <QObject>

namespace ezechiel {
namespace core {
class PredictionSpec;
class DrugResponseAnalysis;
class DrugTreatment;
class PatientVariateList;
class DosageHistory;
class PatientVariate;
class OperableAmount;
class IdentifiableAmount;
class OperationList;
class Operation;
class Dosage;
class Admin;
class DrugModel;
class ADME;
class AdminList;
class DrugVariateList;
class DrugVariate;
class Concentrations;
class ValidDoses;
class ValidDose;
class ErrorModel;
class Halflife;
class ValidInfusions;
class ValidInfusion;
class ValidIntervals;
class ValidInterval;
class TargetList;
class Target;
class CorePatient;
class CoreMeasureList;
class CoreMeasure;
class TargetMethod;
class ParameterSet;
class Parameters;
class Parameter;
class Bsv;
class ActiveSubstance;
class StandardTreatment;
}
}

class PredictionSpecCopier
{
public:
    PredictionSpecCopier();

    ezechiel::core::PredictionSpec* copy(ezechiel::core::PredictionSpec *spec, QObject *parent=0);
    ezechiel::core::DrugResponseAnalysis* copy(ezechiel::core::DrugResponseAnalysis *analysis, QObject *parent=0);
    ezechiel::core::DrugTreatment* copy(ezechiel::core::DrugTreatment *treatment, QObject *parent=0);
    ezechiel::core::PatientVariateList* copy(ezechiel::core::PatientVariateList* list, QObject *parent=0);
    ezechiel::core::DosageHistory* copy(ezechiel::core::DosageHistory* history, QObject *parent=0);
    ezechiel::core::Dosage* copy(ezechiel::core::Dosage* dosage, QObject *parent=0);
    ezechiel::core::PatientVariate* copy(ezechiel::core::PatientVariate* variate, QObject *parent=0);
    ezechiel::core::OperableAmount *copy(ezechiel::core::OperableAmount *amount, QObject *parent=0);
    ezechiel::core::IdentifiableAmount *copy(ezechiel::core::IdentifiableAmount *amount, QObject *parent=0);
    ezechiel::core::OperationList *copy(ezechiel::core::OperationList *source, QObject *parent=0);
    ezechiel::core::Operation *copy(ezechiel::core::Operation *source, QObject *parent=0);
    ezechiel::core::Admin* copy(ezechiel::core::Admin* admin, QObject *parent=0);
    ezechiel::core::DrugModel* copy(ezechiel::core::DrugModel *drug, QObject *parent=0);
    ezechiel::core::ADME* copy(ezechiel::core::ADME* adme, QObject *parent=0);
    ezechiel::core::AdminList* copy(ezechiel::core::AdminList* history, QObject *parent=0);
    ezechiel::core::DrugVariateList* copy(ezechiel::core::DrugVariateList* list, QObject *parent=0);
    ezechiel::core::DrugVariate* copy(ezechiel::core::DrugVariate* variate, QObject *parent=0);
    ezechiel::core::Concentrations* copy(ezechiel::core::Concentrations* concentrations, QObject *parent=0);
    ezechiel::core::StandardTreatment* copy(ezechiel::core::StandardTreatment* treatment, QObject *parent=0);
    ezechiel::core::ValidDoses* copy(ezechiel::core::ValidDoses* list, QObject *parent=0);
    ezechiel::core::ValidDose* copy(ezechiel::core::ValidDose* source, QObject *parent=0);
    ezechiel::core::ErrorModel* copy(ezechiel::core::ErrorModel *model, QObject *parent=0);
    ezechiel::core::Halflife* copy(ezechiel::core::Halflife *halflife, QObject *parent=0);
    ezechiel::core::ValidInfusion* copy(ezechiel::core::ValidInfusion *source, QObject *parent=0);
    ezechiel::core::ValidInfusions* copy(ezechiel::core::ValidInfusions *source, QObject *parent=0);
    ezechiel::core::ValidInterval* copy(ezechiel::core::ValidInterval *source, QObject *parent=0);
    ezechiel::core::ValidIntervals* copy(ezechiel::core::ValidIntervals *source, QObject *parent=0);
    ezechiel::core::TargetList* copy(ezechiel::core::TargetList* source, QObject *parent=0);
    ezechiel::core::Target* copy(ezechiel::core::Target* source, QObject *parent=0);
    ezechiel::core::CorePatient* copy(ezechiel::core::CorePatient* source, QObject *parent=0);
    ezechiel::core::CoreMeasureList* copy(ezechiel::core::CoreMeasureList* source, QObject *parent=0);
    ezechiel::core::CoreMeasure* copy(ezechiel::core::CoreMeasure* source, QObject *parent=0);
    ezechiel::core::TargetMethod* copy(ezechiel::core::TargetMethod* source, QObject *parent=0);
    ezechiel::core::ParameterSet* copy(ezechiel::core::ParameterSet* source, QObject *parent=0);
    ezechiel::core::Parameters* copy(ezechiel::core::Parameters* source, QObject *parent=0);
    ezechiel::core::Parameter* copy(ezechiel::core::Parameter* source, QObject *parent=0);
    ezechiel::core::Bsv* copy(ezechiel::core::Bsv* source, QObject *parent=0);
    ezechiel::core::ActiveSubstance* copy(ezechiel::core::ActiveSubstance* source, QObject *parent=0);




};

#endif // PREDICTIONSPECCOPIER_H
