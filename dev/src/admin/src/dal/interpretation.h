#ifndef INTERPRETATION_H
#define INTERPRETATION_H

#include <QDateTime>

#include "core/dal/entity.h"


class InterpretationAnalysis;
class InterpretationRequest;
class Practician;
class ValidationStatus;

namespace Tucuxi {
namespace Gui {
namespace Core {
    class DrugResponseAnalysis;
    class AdjustmentDosage;
}
}
}

class Interpretation : public Tucuxi::Gui::Core::Entity
{
public:

    enum class InterpretationType
    {
        Undefined,
        FromRequest,
        NoRequest
    };

    Q_OBJECT

    ADMIN_ENTITY_UTILS(Interpretation)

    AUTO_PROPERTY_DECL(InterpretationType, intepretationType, InterpretationType)
    AUTO_PROPERTY_DECL(InterpretationAnalysis*, analysis, Analysis)
    AUTO_PROPERTY_DECL(InterpretationRequest*, request, Request)
    AUTO_PROPERTY_DECL(Tucuxi::Gui::Core::DrugResponseAnalysis*, drugResponseAnalysis, DrugResponseAnalysis)

    AUTO_PROPERTY_DECL(Practician*, analyst, Analyst)
    AUTO_PROPERTY_DECL(ValidationStatus*, validationStatus, ValidationStatus)

    AUTO_PROPERTY_DECL(bool, adjustmentDateIsSet, AdjustmentDateIsSet)
    AUTO_PROPERTY_DECL(bool, adjustmentWithLoadingDose, AdjustmentWithLoadingDose)
    AUTO_PROPERTY_DECL(bool, adjustmentWithRestPeriod, AdjustmentWithRestPeriod)
    AUTO_PROPERTY_DECL(QDateTime, adjustmentDate, AdjustmentDate)
    AUTO_PROPERTY_DECL(Tucuxi::Gui::Core::AdjustmentDosage*, selectedAdjustment, SelectedAdjustment)

    /// This property stores the exact time the user started the interpretation
    AUTO_PROPERTY_DECL(QDateTime, startInterpretationTime, StartInterpretationTime)

    /// This property stores the exact time the user validated the interpretation.
    /// This time is used in the report to display its time of generation.
    /// Together with startInterpretationTime it allows to evaluate the time required to deal
    /// with the interpretation.
    AUTO_PROPERTY_DECL(QDateTime, validateInterpretationTime, ValidateInterpretationTime)

protected:

    Q_INVOKABLE Interpretation(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);

public:

    Q_INVOKABLE bool justForDebuggingPurpose();

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;

    }

};

typedef Interpretation* SharedInterpretation;
Q_DECLARE_METATYPE(Interpretation*)
Q_DECLARE_METATYPE(QList<Interpretation*>)

#endif // INTERPRETATION_H
