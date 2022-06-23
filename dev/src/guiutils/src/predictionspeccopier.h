//@@license@@

#ifndef PREDICTIONSPECCOPIER_H
#define PREDICTIONSPECCOPIER_H


#include <QObject>

namespace Tucuxi {
namespace Gui {
namespace Core {

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
}

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class PredictionSpecCopier
{
public:
    PredictionSpecCopier();

    Tucuxi::Gui::Core::PredictionSpec* copy(Tucuxi::Gui::Core::PredictionSpec *spec, QObject *parent=0);
    Tucuxi::Gui::Core::DrugResponseAnalysis* copy(Tucuxi::Gui::Core::DrugResponseAnalysis *analysis, QObject *parent=0);
    Tucuxi::Gui::Core::DrugTreatment* copy(Tucuxi::Gui::Core::DrugTreatment *treatment, QObject *parent=0);
    Tucuxi::Gui::Core::PatientVariateList* copy(Tucuxi::Gui::Core::PatientVariateList* list, QObject *parent=0);
    Tucuxi::Gui::Core::DosageHistory* copy(Tucuxi::Gui::Core::DosageHistory* history, QObject *parent=0);
    Tucuxi::Gui::Core::Dosage* copy(Tucuxi::Gui::Core::Dosage* dosage, QObject *parent=0);
    Tucuxi::Gui::Core::PatientVariate* copy(Tucuxi::Gui::Core::PatientVariate* variate, QObject *parent=0);
    Tucuxi::Gui::Core::OperableAmount *copy(Tucuxi::Gui::Core::OperableAmount *amount, QObject *parent=0);
    Tucuxi::Gui::Core::IdentifiableAmount *copy(Tucuxi::Gui::Core::IdentifiableAmount *amount, QObject *parent=0);
    Tucuxi::Gui::Core::OperationList *copy(Tucuxi::Gui::Core::OperationList *source, QObject *parent=0);
    Tucuxi::Gui::Core::Operation *copy(Tucuxi::Gui::Core::Operation *source, QObject *parent=0);
    Tucuxi::Gui::Core::Admin* copy(Tucuxi::Gui::Core::Admin* admin, QObject *parent=0);
    Tucuxi::Gui::Core::DrugModel* copy(Tucuxi::Gui::Core::DrugModel *drug, QObject *parent=0);
    Tucuxi::Gui::Core::ADME* copy(Tucuxi::Gui::Core::ADME* adme, QObject *parent=0);
    Tucuxi::Gui::Core::AdminList* copy(Tucuxi::Gui::Core::AdminList* history, QObject *parent=0);
    Tucuxi::Gui::Core::DrugVariateList* copy(Tucuxi::Gui::Core::DrugVariateList* list, QObject *parent=0);
    Tucuxi::Gui::Core::DrugVariate* copy(Tucuxi::Gui::Core::DrugVariate* variate, QObject *parent=0);
    Tucuxi::Gui::Core::Concentrations* copy(Tucuxi::Gui::Core::Concentrations* concentrations, QObject *parent=0);
    Tucuxi::Gui::Core::StandardTreatment* copy(Tucuxi::Gui::Core::StandardTreatment* treatment, QObject *parent=0);
    Tucuxi::Gui::Core::ValidDoses* copy(Tucuxi::Gui::Core::ValidDoses* list, QObject *parent=0);
    Tucuxi::Gui::Core::ValidDose* copy(Tucuxi::Gui::Core::ValidDose* source, QObject *parent=0);
    Tucuxi::Gui::Core::ErrorModel* copy(Tucuxi::Gui::Core::ErrorModel *model, QObject *parent=0);
    Tucuxi::Gui::Core::Halflife* copy(Tucuxi::Gui::Core::Halflife *halflife, QObject *parent=0);
    Tucuxi::Gui::Core::ValidInfusion* copy(Tucuxi::Gui::Core::ValidInfusion *source, QObject *parent=0);
    Tucuxi::Gui::Core::ValidInfusions* copy(Tucuxi::Gui::Core::ValidInfusions *source, QObject *parent=0);
    Tucuxi::Gui::Core::ValidInterval* copy(Tucuxi::Gui::Core::ValidInterval *source, QObject *parent=0);
    Tucuxi::Gui::Core::ValidIntervals* copy(Tucuxi::Gui::Core::ValidIntervals *source, QObject *parent=0);
    Tucuxi::Gui::Core::TargetList* copy(Tucuxi::Gui::Core::TargetList* source, QObject *parent=0);
    Tucuxi::Gui::Core::Target* copy(Tucuxi::Gui::Core::Target* source, QObject *parent=0);
    Tucuxi::Gui::Core::CorePatient* copy(Tucuxi::Gui::Core::CorePatient* source, QObject *parent=0);
    Tucuxi::Gui::Core::CoreMeasureList* copy(Tucuxi::Gui::Core::CoreMeasureList* source, QObject *parent=0);
    Tucuxi::Gui::Core::CoreMeasure* copy(Tucuxi::Gui::Core::CoreMeasure* source, QObject *parent=0);
    Tucuxi::Gui::Core::TargetMethod* copy(Tucuxi::Gui::Core::TargetMethod* source, QObject *parent=0);
    Tucuxi::Gui::Core::ParameterSet* copy(Tucuxi::Gui::Core::ParameterSet* source, QObject *parent=0);
    Tucuxi::Gui::Core::Parameters* copy(Tucuxi::Gui::Core::Parameters* source, QObject *parent=0);
    Tucuxi::Gui::Core::Parameter* copy(Tucuxi::Gui::Core::Parameter* source, QObject *parent=0);
    Tucuxi::Gui::Core::Bsv* copy(Tucuxi::Gui::Core::Bsv* source, QObject *parent=0);
    Tucuxi::Gui::Core::ActiveSubstance* copy(Tucuxi::Gui::Core::ActiveSubstance* source, QObject *parent=0);




};

}
}
}

#endif // PREDICTIONSPECCOPIER_H
