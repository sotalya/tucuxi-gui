#ifndef ADJUSTMENT_H
#define ADJUSTMENT_H

#include <QObject>
#include <QString>
#include "core/utils/autoproperty.h"
#include "predictive.h"
#include "core/dal/dosage.h"
#include "core/dal/entity.h"
#include "core/dal/drug/target.h"

namespace ezechiel {
namespace core {

/**
 * @brief The TargetEvaluationResult class
 * This class is meant to embed information about the evaluation of a specific target.
 * It is composed of the type of target, the calculated value, and the score
 */
class TargetEvaluationResult : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(TargetEvaluationResult)

    //! Type of target, as there should be only a single one of each type it is sufficient to discriminate
    AUTO_PROPERTY_DECL(TargetMethod*, targetType, TargetType)

    //! Target value calculated by the TargetEvaluator
    AUTO_PROPERTY_DECL(double, value, Value)

    //! Target value calculated by the TargetEvaluator
    AUTO_PROPERTY_DECL(QString, unit, Unit)

    //! Score of the target, calculated by the TargetEvaluator
    AUTO_PROPERTY_DECL(double, score, Score)

public:

    Q_INVOKABLE QString toHtml() const;

public:
    explicit TargetEvaluationResult(AbstractRepository *repository, QObject *parent = nullptr);
};

QML_POINTERLIST_CLASS_DECL(TargetEvaluationResults, TargetEvaluationResult)

class Adjustment : public Predictive
{
    Q_OBJECT
    ENTITY_UTILS(Adjustment)
    AUTO_PROPERTY_DECL(DosageHistory*, dosageHistory, DosageHistory)

    AUTO_PROPERTY_DECL(double, score, Score)

    AUTO_PROPERTY_DECL(TargetEvaluationResults*, targetEvaluationResults, TargetEvaluationResults)

public:
    explicit Adjustment(AbstractRepository *repository, QObject *parent = nullptr);

    bool isValid() {
        if (getDosageHistory()->isValid() && getPredictionData()->isValid()) {
            return true;
        } else {
            return false;
        }
    }
};

QML_POINTERLIST_CLASS_DECL(Adjustments, Adjustment)
}
}

Q_DECLARE_METATYPE(ezechiel::core::TargetEvaluationResult*)
Q_DECLARE_METATYPE(ezechiel::core::TargetEvaluationResults*)

Q_DECLARE_METATYPE(ezechiel::core::Adjustment*)
Q_DECLARE_METATYPE(QList<ezechiel::core::Adjustment*>)
Q_DECLARE_METATYPE(ezechiel::core::Adjustments*)
#endif // ADJUSTMENT_H
