//@@license@@

#ifndef ADJUSTMENTTABCONTROLLER_H
#define ADJUSTMENTTABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"

namespace Tucuxi {
namespace GuiCore {

class DosageHistory;
class Dosage;
class ChartData;
class AdjustmentDosage;
}
}

class AdjustmentTabController : public AbstractViewController
{
    Q_OBJECT

public:
    explicit AdjustmentTabController(QObject *parent = nullptr);

    //! This property is used by AdjustmentTab.qml to access the list of dosages
    STD_PROPERTY_DECL(Tucuxi::GuiCore::DosageHistory*, adjustments, Adjustments)

    //! This property is not implemented using the standard macro, as it
    //! Shall refer to the PredictionSpec object
    STD_PROPERTY_DECL(QDateTime, adjustmentDate, AdjustmentDate)

    //! This property is not implemented using the standard macro, as it
    //! Shall refer to the PredictionSpec object
    STD_PROPERTY_DECL(bool, adjustmentWithLoadingDose, AdjustmentWithLoadingDose)

    //! This property is not implemented using the standard macro, as it
    //! Shall refer to the PredictionSpec object
    STD_PROPERTY_DECL(bool, adjustmentWithRestPeriod, AdjustmentWithRestPeriod)

    STD_PROPERTY_DECL(bool, isManualAdjustmentDefined, IsManualAdjustmentDefined)

public:
    Q_INVOKABLE void addAdjustment();
    Q_INVOKABLE void removeAdjustment(int index);
    Q_INVOKABLE void setDbValue(int index, double value);
    Q_INVOKABLE void setDbInterval(int index, double interval);
    Q_INVOKABLE void selectAdjustment(int index);

    void setAdjustmentDateFromMaster(QDateTime date);
    void setInitialAdjustmentDateFromMaster(QDateTime date);

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
    Q_INVOKABLE void forceRefresh();

    void setChartData(Tucuxi::GuiCore::ChartData *chartData);
    void onDosageUpdated();
    void reset();

private:
    bool isIndexValid(int index);
    void removeFromTreatement(const QString &type);
    static bool compareAdjustment(const Tucuxi::GuiCore::Dosage* a, const Tucuxi::GuiCore::Dosage* b);

    Tucuxi::GuiCore::AdjustmentDosage* getAdjustment(const QString &type);
    Tucuxi::GuiCore::AdjustmentDosage* getLastAdjustment(const QString &type);
    void adaptDates(const Tucuxi::GuiCore::AdjustmentDosage *manual, Tucuxi::GuiCore::AdjustmentDosage *suggested);

private:
    Tucuxi::GuiCore::ChartData *_chartData;
};


#endif // ADJUSTMENTTABCONTROLLER_H
