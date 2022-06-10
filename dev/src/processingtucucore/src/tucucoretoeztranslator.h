//@@license@@

#ifndef TUCUCORETOEZTRANSLATOR_H
#define TUCUCORETOEZTRANSLATOR_H

#include <QDateTime>
#include <QString>

#include "core/dal/drug/adme.h"

#include "tucucommon/datetime.h"
#include "tucucommon/translatablestring.h"

#include "tucucore/definitions.h"
#include "tucucore/drugmodel/targetdefinition.h"

namespace Tucuxi {
namespace GuiCore {

class TargetEvaluationResult;
class DosageHistory;
class DrugModel;
class OperableAmount;
class Target;
class TargetMethod;

} // namespace core
} // namespace Tucuxi

namespace Tucuxi {
namespace Core {

class TargetEvaluationResult;
class DosageHistory;
class FormulationAndRoute;
class SubTargetDefinition;
class TargetDefinition;

class DrugModel;

} // namespace Core
} // namespace Tucuxi

class TucucoreToEzTranslator
{
public:
    TucucoreToEzTranslator();


    QDateTime buildDateTime(const Tucuxi::Common::DateTime &date);

    bool buildTargetEvaluation(
            const Tucuxi::Core::TargetEvaluationResult *target,
            Tucuxi::GuiCore::TargetEvaluationResult *newTarget);

    bool buildDosageHistory(const Tucuxi::Core::DosageHistory & dosageHistory,
                                                      Tucuxi::GuiCore::DosageHistory *newHistory);

    Tucuxi::GuiCore::Admin::Route translateFormulationAndRoute(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute);
    Tucuxi::GuiCore::Admin::Route translateAbsorptionModel(Tucuxi::Core::AbsorptionModel _absorptionModel);

    QString description(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute);

    Tucuxi::GuiCore::DrugModel* buildLightDrugModel(const Tucuxi::Core::DrugModel *drugModel);

    Tucuxi::GuiCore::OperableAmount *translateMic(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Common::TucuUnit micUnit,
                                                 Tucuxi::Common::TucuUnit newUnit, Tucuxi::GuiCore::Target *newTarget);
    Tucuxi::GuiCore::OperableAmount *translate(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, Tucuxi::GuiCore::Target *newTarget);
    Tucuxi::GuiCore::OperableAmount *translateTime(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, Tucuxi::GuiCore::Target *newTarget);
    Tucuxi::GuiCore::TargetMethod *translate(Tucuxi::Core::TargetType targetType, Tucuxi::GuiCore::Target *newTarget);

    Tucuxi::GuiCore::TranslatableString *translate(const Tucuxi::Common::TranslatableString &str, Tucuxi::GuiCore::Entity *parent);
};

#endif // TUCUCORETOEZTRANSLATOR_H
