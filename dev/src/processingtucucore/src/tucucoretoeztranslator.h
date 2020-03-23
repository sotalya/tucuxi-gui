#ifndef TUCUCORETOEZTRANSLATOR_H
#define TUCUCORETOEZTRANSLATOR_H

#include <QDateTime>
#include <QString>

#include "core/dal/drug/adme.h"

#include "tucucommon/datetime.h"
#include "tucucommon/translatablestring.h"

#include "tucucore/definitions.h"
#include "tucucore/drugmodel/targetdefinition.h"

namespace ezechiel {
namespace core {

class TargetEvaluationResult;
class DosageHistory;
class DrugModel;
class OperableAmount;
class Target;
class TargetMethod;

} // namespace core
} // namespace ezechiel

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
            ezechiel::core::TargetEvaluationResult *newTarget);

    bool buildDosageHistory(const Tucuxi::Core::DosageHistory & dosageHistory,
                                                      ezechiel::core::DosageHistory *newHistory);

    ezechiel::core::Admin::Route translateFormulationAndRoute(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute);
    ezechiel::core::Admin::Route translateAbsorptionModel(Tucuxi::Core::AbsorptionModel _absorptionModel);

    QString description(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute);

    ezechiel::core::DrugModel* buildLightDrugModel(const Tucuxi::Core::DrugModel *drugModel);

    ezechiel::core::OperableAmount *translate(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, ezechiel::core::Target *newTarget);
    ezechiel::core::OperableAmount *translateTime(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, ezechiel::core::Target *newTarget);
    ezechiel::core::TargetMethod *translate(Tucuxi::Core::TargetType targetType, ezechiel::core::Target *newTarget);

    ezechiel::core::TranslatableString *translate(const Tucuxi::Common::TranslatableString &str, ezechiel::core::Entity *parent);
};

#endif // TUCUCORETOEZTRANSLATOR_H
