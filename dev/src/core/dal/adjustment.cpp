//@@lisence@@ 

#include "adjustment.h"
#include "dal/drug/target.h"

namespace ezechiel {
namespace core {

AUTO_PROPERTY_IMPL(TargetEvaluationResult, TargetMethod*, targetType, TargetType)
AUTO_PROPERTY_IMPL(TargetEvaluationResult, double, value, Value)
AUTO_PROPERTY_IMPL(TargetEvaluationResult, QString, unit, Unit)
AUTO_PROPERTY_IMPL(TargetEvaluationResult, double, score, Score)

TargetEvaluationResult::TargetEvaluationResult(AbstractRepository *repository, QObject *parent) :
    Entity(repository, parent),
    _targetType(nullptr),
    _value(0.0),
    _unit("-"),
    _score(-1.0)
{

}

QString TargetEvaluationResult::toHtml() const
{
    QString res;
    res += "Target type : " + _targetType->getLabel() + "\n";
    QString val;
    val.setNum(_value, 'f', 2);
    res += "Value : " + val + " " + _unit + "\n";
    val.setNum(_score, 'f', 2);
    res += "Score : " +val + "\n";
    return res;

}


QML_POINTERLIST_CLASS_IMPL(TargetEvaluationResults, TargetEvaluationResult)

QML_POINTERLIST_CLASS_IMPL(Adjustments, Adjustment)

AUTO_PROPERTY_IMPL(Adjustment, DosageHistory*, dosageHistory, DosageHistory)
AUTO_PROPERTY_IMPL(Adjustment, double, score, Score)
AUTO_PROPERTY_IMPL(Adjustment, TargetEvaluationResults*, targetEvaluationResults, TargetEvaluationResults)

Adjustment::Adjustment(AbstractRepository *repository, QObject *parent)
    : Predictive(repository, parent),
      _dosageHistory(CoreFactory::createEntity<DosageHistory>(repository, this)),
      _score(-1.0),
      _targetEvaluationResults(CoreFactory::createEntity<TargetEvaluationResults>(repository, this))
{

}

}
}

