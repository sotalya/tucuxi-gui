#ifndef DOSAGETABCONTROLLER_H
#define DOSAGETABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"
#include "core/dal/dosage.h"

namespace ezechiel {
namespace core {

class DosageHistory;
class DrugModel;

}
}

class DosageTabController : public AbstractViewController
{
    Q_OBJECT

public:
    explicit DosageTabController(QObject *parent = nullptr);

    //! This property is used by DosageTab.qml to access the list of dosages
    STD_PROPERTY_DECL(ezechiel::core::DosageHistory*, dosages, Dosages)
    //STD_PROPERTY_DECL(ezechiel::core::DrugModel*, drugModel, DrugModel)
    Q_PROPERTY(bool canHaveMoreDosages READ getCanHaveMoreDosages NOTIFY canHaveMoreDosagesChanged);

public:
    /**
     * @brief addDosage Adds a new dosage to the dosage history
     * The new dosage is added at the end of the history, and lasts
     * for 1 day. This could be changed in the future, but for now
     * this 1-day choice is reasonable.
     */
    Q_INVOKABLE void addDosage();
    Q_INVOKABLE void removeDosage(int index);
    Q_INVOKABLE void setDbValue(int index, double value);
    Q_INVOKABLE void setDbInterval(int index, double interval);

    Q_INVOKABLE int getRelativeRouteValue(int index);


    /**
     * @brief setRouteValue Sets the route of administration
     * @param index index of the dosage in the dosage history
     * @param routeValue Index of the route relative to the drugmodel intakes
     * This function should be used with care, as routeValue is not the index
     * of the route in the entire routes supported by the software, but the
     * index of the route in the availables ones for the selected drug model.
     */
    Q_INVOKABLE void setRouteValue(int index, int routeValue);
    Q_INVOKABLE void setDbTinf(int index, double value);
    Q_INVOKABLE void setAppliedTime(int index, QDateTime time);
    Q_INVOKABLE void setEndTime(int index, QDateTime time);
    Q_INVOKABLE void setHasEndDate(int index, bool hasEndDate);
    Q_INVOKABLE void setAtSteadyState(int index, bool isAtSteadyState, QDateTime lastDoseTime);
    Q_INVOKABLE void forceRefresh();
    Q_INVOKABLE bool checkModifiedDates(int index, const QDateTime &appliedDate, const QDateTime &endDate);

    Q_INVOKABLE int getNbDosages();

    Q_INVOKABLE QDateTime getNewEndTime(QDateTime start);
    bool getCanHaveMoreDosages();

    void reset();


signals:
    void canHaveMoreDosagesChanged(bool);

protected:
    bool isIndexValid(int index);

public:
    DosageTabController();

private:
    static bool compareDosage(const ezechiel::core::Dosage* a, const ezechiel::core::Dosage* b);
};

#endif // DOSAGETABCONTROLLER_H
