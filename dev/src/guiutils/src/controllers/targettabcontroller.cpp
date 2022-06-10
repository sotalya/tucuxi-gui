//@@license@@

#include "targettabcontroller.h"
#include "interpretationcontroller.h"
#include "core/dal/drug/target.h"
#include "core/utils/duration.h"
#include "core/corefactory.h"
#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/drug.h"
#include "core/dal/drug/doses.h"
#include "core/utils/units.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/target.h"

STD_PROPERTY_IMPL(TargetTabController, Tucuxi::Gui::Core::TargetList*, targets, Targets)

TargetTabController::TargetTabController(QObject *parent) : AbstractViewController(parent)
{
    _targets = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::TargetList>(ABSTRACTREPO, this);
}

void TargetTabController::setTargetType(int index, int value)
{
    if (!isIndexValid(index)) return;
    if (_targets->at(index)->getType()->getValue() == value) return;

    _targets->at(index)->getType()->setValue(value);
}

void TargetTabController::setUnit(int index, QString unitString)
{
    if (!isIndexValid(index)) {
        return;
    }
    if (_targets->at(index)->getCbest()->getUnitstring() == unitString) {
        return;
    }
    _targets->at(index)->getCmin()->setUnitstring(unitString);
    _targets->at(index)->getCbest()->setUnitstring(unitString);
    _targets->at(index)->getCmax()->setUnitstring(unitString);
}

void TargetTabController::setCminDbValue(int index, double value)
{
    if (!isIndexValid(index)) return;
    if (_targets->at(index)->getCmin()->getDbvalue() == value) return;

    double min, best, max, dMinBest, dMaxBest;
    prepareCupdate(index, min, best, max, dMinBest, dMaxBest);
    min = value;
    adaptMin(min, best, max, dMinBest, dMaxBest);
    updateCvalues(index, min, best, max);
}

void TargetTabController::setCmaxDbValue(int index, double value)
{
    if (!isIndexValid(index)) return;
    if ( _targets->at(index)->getCmax()->getDbvalue() == value) return;

    double min, best, max, dMinBest, dMaxBest;
    prepareCupdate(index, min, best, max, dMinBest, dMaxBest);
    max = value;
    adaptMax(min, best, max, dMinBest, dMaxBest);
    updateCvalues(index, min, best, max);
}

void TargetTabController::setCbestDbValue(int index, double value)
{
    if (!isIndexValid(index)) return;
    if ( _targets->at(index)->getCbest()->getDbvalue() == value) return;

    double min, best, max, dMinBest, dMaxBest;
    prepareCupdate(index, min, best, max, dMinBest, dMaxBest);
    best = value;
    adaptBest(min, best, max, dMinBest, dMaxBest);
    updateCvalues(index, min, best, max);
}

void TargetTabController::setTminDbValue(int index, double value)
{
    if (!isIndexValid(index)) return;
    if ( _targets->at(index)->getTmin()->getDbvalue() == value) return;

    double min, best, max, dMinBest, dMaxBest;
    prepareTupdate(index, min, best, max, dMinBest, dMaxBest);
    min = value;
    adaptMin(min, best, max, dMinBest, dMaxBest);
    updateTvalues(index, min, best, max);
}

void TargetTabController::setTmaxDbValue(int index, double value)
{
    if (!isIndexValid(index)) return;
    if ( _targets->at(index)->getTmax()->getDbvalue() == value) return;

    double min, best, max, dMinBest, dMaxBest;
    prepareTupdate(index, min, best, max, dMinBest, dMaxBest);
    max = value;
    adaptMax(min, best, max, dMinBest, dMaxBest);
    updateTvalues(index, min, best, max);
}

void TargetTabController::setTbestDbValue(int index, double value)
{
    if (!isIndexValid(index)) return;
    if (_targets->at(index)->getTbest()->getDbvalue() == value) return;

    double min, best, max, dMinBest, dMaxBest;
    prepareTupdate(index, min, best, max, dMinBest, dMaxBest);
    best = value;
    adaptBest(min, best, max, dMinBest, dMaxBest);
    updateTvalues(index, min, best, max);
}

void TargetTabController::setMicDbValue(int index, double value)
{
    if (!isIndexValid(index)) return;
    if ( _targets->at(index)->getMic()->getDbvalue() == value) return;

    _targets->at(index)->getMic()->setDbvalue(value);
}

void TargetTabController::addTarget()
{
    Tucuxi::Gui::Core::TargetList* tlist = masterController->getInterpretation()->getDrugResponseAnalysis()->getTreatment()->getTargets();
    Tucuxi::Gui::Core::Target* target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Target>(ABSTRACTREPO, tlist);
    Tucuxi::Gui::Core::Unit defaultUnit = Tucuxi::Gui::Core::Unit(masterController->getDefaultUnit());
    target->getCbest()->setUnit(defaultUnit);
    target->getCmax()->setUnit(defaultUnit);
    target->getCmin()->setUnit(defaultUnit);
    target->getMic()->setUnit(defaultUnit);
    tlist->append(target);
}

void TargetTabController::removeTarget(int index)
{
    if (index >= _targets->size()) return;
    _targets->remove(index);
    masterController->targetUpdated();
}

void TargetTabController::forceRefresh()
{
    masterController->targetUpdated();
}

bool TargetTabController::isIndexValid(int index)
{
    return (index >= 0) && (index < _targets->size());
}

void TargetTabController::adaptMin(double &min, double &best, double &max, const double dMinBest, const double dMaxBest)
{
    if (min > best) best = min + dMinBest;
    if (best > max) max = best + dMaxBest;
}

void TargetTabController::adaptBest(double &min, double &best, double &max, const double dMinBest, const double dMaxBest)
{
    if (best > max) max = best + dMaxBest;
    if (best < min) min = best - dMinBest;
}

void TargetTabController::adaptMax(double &min, double &best, double &max, const double dMinBest, const double dMaxBest)
{
    if (max < best) best = max - dMaxBest;
    if (best < min) min = best - dMinBest;
}

void TargetTabController::prepareCupdate(int index, double &min, double &best, double &max, double &dMinBest, double& dMaxBest)
{
    min = _targets->at(index)->getCmin()->getDbvalue();
    best = _targets->at(index)->getCbest()->getDbvalue();
    max = _targets->at(index)->getCmax()->getDbvalue();
    dMinBest = best-min;
    dMaxBest = max-best;
}

void TargetTabController::updateCvalues(int index, double &min, double &best, double &max)
{
    _targets->at(index)->getCmin()->setDbvalue(min);
    _targets->at(index)->getCbest()->setDbvalue(best);
    _targets->at(index)->getCmax()->setDbvalue(max);
}

void TargetTabController::prepareTupdate(int index, double &min, double &best, double &max, double &dMinBest, double& dMaxBest)
{
    min = _targets->at(index)->getTmin()->getDbvalue();
    best = _targets->at(index)->getTbest()->getDbvalue();
    max = _targets->at(index)->getTmax()->getDbvalue();
    dMinBest = best-min;
    dMaxBest = max-best;
}

void TargetTabController::updateTvalues(int index, double &min, double &best, double &max)
{
    _targets->at(index)->getTmin()->setDbvalue(min);
    _targets->at(index)->getTbest()->setDbvalue(best);
    _targets->at(index)->getTmax()->setDbvalue(max);
}

QStringList TargetTabController::getTargetTypes()
{
    QStringList list = {"Residual",                   //  0
                        "Peak",                       //  1
                        "Mean",                       //  2
                        "AUC",                        //  3
                        "AUC 24h",                    //  4
                        "Cumulative AUC",             //  5
                        "AUC divided by MIC",         //  6
                        "AUC 24h divided by MIC",     //  7
                        "AUC over MIC",               //  8
                        "AUC 24h over MIC",           //  9
                        "Peak divided by MIC",        // 10
                        "Time over Mic",              // 11
                        "Residual divided by MIC",    // 12
                        "Fraction of time over MIC"}; // 13
    return list;
}

QString TargetTabController::calculateUnit(int type, QString baseUnit)
{
    QString result;
    switch (type) {
    case 0:
    case 1:
    case 2: {
        result = "ug/l";
    }
        break;
    case 3:
    case 4:
    case 5:
    case 8:
    case 9:
    {
        result = "ug*h/l";
    }    break;
    case 10:
    case 12:
    case 13: {
        result = "";
    }    break;
    case 6:
    case 7:
    case 11: {
        result = "h";
    }
        break;
    }

    return result;

}
