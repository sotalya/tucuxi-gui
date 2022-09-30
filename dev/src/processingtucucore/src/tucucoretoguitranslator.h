//@@license@@

#ifndef TUCUCORETOGUITRANSLATOR_H
#define TUCUCORETOGUITRANSLATOR_H

#include <QDateTime>
#include <QString>

#include "core/dal/drug/adme.h"

#include "tucucommon/datetime.h"
#include "tucucommon/translatablestring.h"

#include "tucucore/definitions.h"
#include "tucucore/drugmodel/targetdefinition.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class TargetEvaluationResult;
class DosageHistory;
class DrugModel;
class OperableAmount;
class Target;
class TargetMethod;

} // namespace Core
} // namespace Gui
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

namespace Tucuxi {
namespace Gui {
namespace Processing {

class TucucoreToGuiTranslator
{
public:
    TucucoreToGuiTranslator();


    QDateTime buildDateTime(const Tucuxi::Common::DateTime &date);

    bool buildTargetEvaluation(
            const Tucuxi::Core::TargetEvaluationResult *target,
            Tucuxi::Gui::Core::TargetEvaluationResult *newTarget);

    bool buildDosageHistory(const Tucuxi::Core::DosageHistory & dosageHistory,
                                                      Tucuxi::Gui::Core::DosageHistory *newHistory);

    Tucuxi::Gui::Core::Admin::Route translateFormulationAndRoute(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute);
    Tucuxi::Gui::Core::Admin::Route translateAbsorptionModel(Tucuxi::Core::AbsorptionModel _absorptionModel);

    QString description(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute);

    Tucuxi::Gui::Core::DrugModel* buildLightDrugModel(const Tucuxi::Core::DrugModel *drugModel, const QString filePath);

    Tucuxi::Gui::Core::OperableAmount *translateMic(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Common::TucuUnit micUnit,
                                                 Tucuxi::Common::TucuUnit newUnit, Tucuxi::Gui::Core::Target *newTarget);
    Tucuxi::Gui::Core::OperableAmount *translate(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, Tucuxi::Gui::Core::Target *newTarget);
    Tucuxi::Gui::Core::OperableAmount *translateTime(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, Tucuxi::Gui::Core::Target *newTarget);
    Tucuxi::Gui::Core::TargetMethod *translate(Tucuxi::Core::TargetType targetType, Tucuxi::Gui::Core::Target *newTarget);

    Tucuxi::Gui::Core::TranslatableString *translate(const Tucuxi::Common::TranslatableString &str, Tucuxi::Gui::Core::Entity *parent);
};

}
}
}

#endif // TUCUCORETOGUITRANSLATOR_H
