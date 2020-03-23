#ifndef PERCENTILECURVE_H
#define PERCENTILECURVE_H

#include "duration.h"
#include "dataset.h"
#include "chartitem.h"
#include "../dal/parameters.h"
#include "item.h"

#include <QDateTime>
#include <qwt_plot_intervalcurve.h>

class PercentileCurve : public AbstractItem, public QwtPlotIntervalCurve
{
public:
    explicit PercentileCurve(ParameterType type = POPULATION);
    
    void setDataset(const QDateTime& start, const QDateTime& stop, const DataSet& min, const DataSet& max);
    void setPoints(QDateTime start, Duration duration, double* min, double* max, int nbPoints);
    void setColor(QColor c);

    int rtti() const;
    ParameterType getType() const;
    void setType(ParameterType type);

private:
    virtual void yScaleChangedImpl();

    // Protect the Qwt standart attach/detach mechanism
    void attach(QwtPlot *plot);
    void detach();

    PercentilesPointArrayData intervals;
    ParameterType type;

    friend class Chart;
};

#endif // PERCENTILECURVE_H
