//@@license@@

#ifndef PREDICTIONSPECCOPIER_H
#define PREDICTIONSPECCOPIER_H


#include <QObject>

namespace Tucuxi {
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

    Tucuxi::GuiCore::PredictionSpec* copy(Tucuxi::GuiCore::PredictionSpec *spec, QObject *parent=0);
    Tucuxi::GuiCore::DrugResponseAnalysis* copy(Tucuxi::GuiCore::DrugResponseAnalysis *analysis, QObject *parent=0);
    Tucuxi::GuiCore::DrugTreatment* copy(Tucuxi::GuiCore::DrugTreatment *treatment, QObject *parent=0);
    Tucuxi::GuiCore::PatientVariateList* copy(Tucuxi::GuiCore::PatientVariateList* list, QObject *parent=0);
    Tucuxi::GuiCore::DosageHistory* copy(Tucuxi::GuiCore::DosageHistory* history, QObject *parent=0);
    Tucuxi::GuiCore::Dosage* copy(Tucuxi::GuiCore::Dosage* dosage, QObject *parent=0);
    Tucuxi::GuiCore::PatientVariate* copy(Tucuxi::GuiCore::PatientVariate* variate, QObject *parent=0);
    Tucuxi::GuiCore::OperableAmount *copy(Tucuxi::GuiCore::OperableAmount *amount, QObject *parent=0);
    Tucuxi::GuiCore::IdentifiableAmount *copy(Tucuxi::GuiCore::IdentifiableAmount *amount, QObject *parent=0);
    Tucuxi::GuiCore::OperationList *copy(Tucuxi::GuiCore::OperationList *source, QObject *parent=0);
    Tucuxi::GuiCore::Operation *copy(Tucuxi::GuiCore::Operation *source, QObject *parent=0);
    Tucuxi::GuiCore::Admin* copy(Tucuxi::GuiCore::Admin* admin, QObject *parent=0);
    Tucuxi::GuiCore::DrugModel* copy(Tucuxi::GuiCore::DrugModel *drug, QObject *parent=0);
    Tucuxi::GuiCore::ADME* copy(Tucuxi::GuiCore::ADME* adme, QObject *parent=0);
    Tucuxi::GuiCore::AdminList* copy(Tucuxi::GuiCore::AdminList* history, QObject *parent=0);
    Tucuxi::GuiCore::DrugVariateList* copy(Tucuxi::GuiCore::DrugVariateList* list, QObject *parent=0);
    Tucuxi::GuiCore::DrugVariate* copy(Tucuxi::GuiCore::DrugVariate* variate, QObject *parent=0);
    Tucuxi::GuiCore::Concentrations* copy(Tucuxi::GuiCore::Concentrations* concentrations, QObject *parent=0);
    Tucuxi::GuiCore::StandardTreatment* copy(Tucuxi::GuiCore::StandardTreatment* treatment, QObject *parent=0);
    Tucuxi::GuiCore::ValidDoses* copy(Tucuxi::GuiCore::ValidDoses* list, QObject *parent=0);
    Tucuxi::GuiCore::ValidDose* copy(Tucuxi::GuiCore::ValidDose* source, QObject *parent=0);
    Tucuxi::GuiCore::ErrorModel* copy(Tucuxi::GuiCore::ErrorModel *model, QObject *parent=0);
    Tucuxi::GuiCore::Halflife* copy(Tucuxi::GuiCore::Halflife *halflife, QObject *parent=0);
    Tucuxi::GuiCore::ValidInfusion* copy(Tucuxi::GuiCore::ValidInfusion *source, QObject *parent=0);
    Tucuxi::GuiCore::ValidInfusions* copy(Tucuxi::GuiCore::ValidInfusions *source, QObject *parent=0);
    Tucuxi::GuiCore::ValidInterval* copy(Tucuxi::GuiCore::ValidInterval *source, QObject *parent=0);
    Tucuxi::GuiCore::ValidIntervals* copy(Tucuxi::GuiCore::ValidIntervals *source, QObject *parent=0);
    Tucuxi::GuiCore::TargetList* copy(Tucuxi::GuiCore::TargetList* source, QObject *parent=0);
    Tucuxi::GuiCore::Target* copy(Tucuxi::GuiCore::Target* source, QObject *parent=0);
    Tucuxi::GuiCore::CorePatient* copy(Tucuxi::GuiCore::CorePatient* source, QObject *parent=0);
    Tucuxi::GuiCore::CoreMeasureList* copy(Tucuxi::GuiCore::CoreMeasureList* source, QObject *parent=0);
    Tucuxi::GuiCore::CoreMeasure* copy(Tucuxi::GuiCore::CoreMeasure* source, QObject *parent=0);
    Tucuxi::GuiCore::TargetMethod* copy(Tucuxi::GuiCore::TargetMethod* source, QObject *parent=0);
    Tucuxi::GuiCore::ParameterSet* copy(Tucuxi::GuiCore::ParameterSet* source, QObject *parent=0);
    Tucuxi::GuiCore::Parameters* copy(Tucuxi::GuiCore::Parameters* source, QObject *parent=0);
    Tucuxi::GuiCore::Parameter* copy(Tucuxi::GuiCore::Parameter* source, QObject *parent=0);
    Tucuxi::GuiCore::Bsv* copy(Tucuxi::GuiCore::Bsv* source, QObject *parent=0);
    Tucuxi::GuiCore::ActiveSubstance* copy(Tucuxi::GuiCore::ActiveSubstance* source, QObject *parent=0);




};

#endif // PREDICTIONSPECCOPIER_H
