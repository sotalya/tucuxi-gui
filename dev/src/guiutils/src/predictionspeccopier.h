//@@license@@

#ifndef PREDICTIONSPECCOPIER_H
#define PREDICTIONSPECCOPIER_H


#include <QObject>

namespace ezechiel {
namespace GuiCore {
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

    ezechiel::GuiCore::PredictionSpec* copy(ezechiel::GuiCore::PredictionSpec *spec, QObject *parent=0);
    ezechiel::GuiCore::DrugResponseAnalysis* copy(ezechiel::GuiCore::DrugResponseAnalysis *analysis, QObject *parent=0);
    ezechiel::GuiCore::DrugTreatment* copy(ezechiel::GuiCore::DrugTreatment *treatment, QObject *parent=0);
    ezechiel::GuiCore::PatientVariateList* copy(ezechiel::GuiCore::PatientVariateList* list, QObject *parent=0);
    ezechiel::GuiCore::DosageHistory* copy(ezechiel::GuiCore::DosageHistory* history, QObject *parent=0);
    ezechiel::GuiCore::Dosage* copy(ezechiel::GuiCore::Dosage* dosage, QObject *parent=0);
    ezechiel::GuiCore::PatientVariate* copy(ezechiel::GuiCore::PatientVariate* variate, QObject *parent=0);
    ezechiel::GuiCore::OperableAmount *copy(ezechiel::GuiCore::OperableAmount *amount, QObject *parent=0);
    ezechiel::GuiCore::IdentifiableAmount *copy(ezechiel::GuiCore::IdentifiableAmount *amount, QObject *parent=0);
    ezechiel::GuiCore::OperationList *copy(ezechiel::GuiCore::OperationList *source, QObject *parent=0);
    ezechiel::GuiCore::Operation *copy(ezechiel::GuiCore::Operation *source, QObject *parent=0);
    ezechiel::GuiCore::Admin* copy(ezechiel::GuiCore::Admin* admin, QObject *parent=0);
    ezechiel::GuiCore::DrugModel* copy(ezechiel::GuiCore::DrugModel *drug, QObject *parent=0);
    ezechiel::GuiCore::ADME* copy(ezechiel::GuiCore::ADME* adme, QObject *parent=0);
    ezechiel::GuiCore::AdminList* copy(ezechiel::GuiCore::AdminList* history, QObject *parent=0);
    ezechiel::GuiCore::DrugVariateList* copy(ezechiel::GuiCore::DrugVariateList* list, QObject *parent=0);
    ezechiel::GuiCore::DrugVariate* copy(ezechiel::GuiCore::DrugVariate* variate, QObject *parent=0);
    ezechiel::GuiCore::Concentrations* copy(ezechiel::GuiCore::Concentrations* concentrations, QObject *parent=0);
    ezechiel::GuiCore::StandardTreatment* copy(ezechiel::GuiCore::StandardTreatment* treatment, QObject *parent=0);
    ezechiel::GuiCore::ValidDoses* copy(ezechiel::GuiCore::ValidDoses* list, QObject *parent=0);
    ezechiel::GuiCore::ValidDose* copy(ezechiel::GuiCore::ValidDose* source, QObject *parent=0);
    ezechiel::GuiCore::ErrorModel* copy(ezechiel::GuiCore::ErrorModel *model, QObject *parent=0);
    ezechiel::GuiCore::Halflife* copy(ezechiel::GuiCore::Halflife *halflife, QObject *parent=0);
    ezechiel::GuiCore::ValidInfusion* copy(ezechiel::GuiCore::ValidInfusion *source, QObject *parent=0);
    ezechiel::GuiCore::ValidInfusions* copy(ezechiel::GuiCore::ValidInfusions *source, QObject *parent=0);
    ezechiel::GuiCore::ValidInterval* copy(ezechiel::GuiCore::ValidInterval *source, QObject *parent=0);
    ezechiel::GuiCore::ValidIntervals* copy(ezechiel::GuiCore::ValidIntervals *source, QObject *parent=0);
    ezechiel::GuiCore::TargetList* copy(ezechiel::GuiCore::TargetList* source, QObject *parent=0);
    ezechiel::GuiCore::Target* copy(ezechiel::GuiCore::Target* source, QObject *parent=0);
    ezechiel::GuiCore::CorePatient* copy(ezechiel::GuiCore::CorePatient* source, QObject *parent=0);
    ezechiel::GuiCore::CoreMeasureList* copy(ezechiel::GuiCore::CoreMeasureList* source, QObject *parent=0);
    ezechiel::GuiCore::CoreMeasure* copy(ezechiel::GuiCore::CoreMeasure* source, QObject *parent=0);
    ezechiel::GuiCore::TargetMethod* copy(ezechiel::GuiCore::TargetMethod* source, QObject *parent=0);
    ezechiel::GuiCore::ParameterSet* copy(ezechiel::GuiCore::ParameterSet* source, QObject *parent=0);
    ezechiel::GuiCore::Parameters* copy(ezechiel::GuiCore::Parameters* source, QObject *parent=0);
    ezechiel::GuiCore::Parameter* copy(ezechiel::GuiCore::Parameter* source, QObject *parent=0);
    ezechiel::GuiCore::Bsv* copy(ezechiel::GuiCore::Bsv* source, QObject *parent=0);
    ezechiel::GuiCore::ActiveSubstance* copy(ezechiel::GuiCore::ActiveSubstance* source, QObject *parent=0);




};

#endif // PREDICTIONSPECCOPIER_H
