//@@license@@

#ifndef TARGETTABCONTROLLER_H
#define TARGETTABCONTROLLER_H


#include <QObject>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"
#include "core/dal/drug/target.h"

namespace ezechiel {
namespace core {

class TargetList;

}
}

class TargetTabController : public AbstractViewController
{
    Q_OBJECT

public:
    explicit TargetTabController(QObject *parent = nullptr);

    //! This property is used by TargetTab.qml to access the list of targets
    STD_PROPERTY_DECL(ezechiel::core::TargetList*, targets, Targets)

public:
    Q_INVOKABLE void addTarget();
    Q_INVOKABLE void removeTarget(int index);
    Q_INVOKABLE void setTargetType(int index, int value);
    Q_INVOKABLE void setCminDbValue(int index, double value);
    Q_INVOKABLE void setCbestDbValue(int index, double value);
    Q_INVOKABLE void setCmaxDbValue(int index, double value);
    Q_INVOKABLE void setTminDbValue(int index, double value);
    Q_INVOKABLE void setTbestDbValue(int index, double value);
    Q_INVOKABLE void setTmaxDbValue(int index, double value);
    Q_INVOKABLE void setMicDbValue(int index, double value);
    Q_INVOKABLE void setUnit(int index, QString unitString);

    Q_INVOKABLE void forceRefresh();

    Q_INVOKABLE QStringList getTargetTypes();

    Q_INVOKABLE QString calculateUnit(int type, QString baseUnit);

private:
    bool isIndexValid(int index);

    void adaptMin(double &min, double &best, double &max, const double dMinBest, const double dMaxBest);
    void adaptBest(double &min, double &best, double &max, const double dMinBest, const double dMaxBest);
    void adaptMax(double &min, double &best, double &max, const double dMinBest, const double dMaxBest);

    void prepareCupdate(int index, double &min, double &best, double &max, double &dMinBest, double& dMaxBest);
    void updateCvalues(int index, double &min, double &best, double &max);

    void prepareTupdate(int index, double &min, double &best, double &max, double &dMinBest, double& dMaxBest);
    void updateTvalues(int index, double &min, double &best, double &max);
};


#endif // TARGETTABCONTROLLER_H
