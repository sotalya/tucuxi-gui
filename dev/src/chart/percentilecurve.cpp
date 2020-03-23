#include "percentilecurve.h"

#include <QPen>

PercentileCurve::PercentileCurve(ParameterType type) :
    QwtPlotIntervalCurve(), type(type)
{
    // Antialiasing, on windows, 7 only, on the rest, it rocks!
#ifdef Q_OS_WIN32
    if (QSysInfo::WindowsVersion == QSysInfo::WV_WINDOWS7)
#endif
    setRenderHint(QwtPlotItem::RenderAntialiased);
    setItemAttribute(QwtPlotIntervalCurve::Legend, false);    
}

void PercentileCurve::setDataset(const QDateTime& start, const QDateTime& stop, const DataSet& min, const DataSet& max)
{
    Q_UNUSED(start);
    Q_UNUSED(stop);

    QVector<double> top, bottom, time;
    max.getDataTimes(top, time);
    min.getDataTimes(bottom, time);

    intervals.setRawData(top, bottom, time);
    setSamples(intervals.data(xScaleType(), yScaleType()));
}

void PercentileCurve::setPoints(QDateTime start, Duration duration, double* min, double* max, int nbPoints)
{
    QDateTime stop(start);
    stop = stop.addSecs(duration.seconds());

    QVector<QwtIntervalSample> samples;
    double delta = (double) duration.seconds() / (double) (nbPoints-1);

    for (int i=0; i<nbPoints; i++)
        samples.append(QwtIntervalSample((double) start.toTime_t() + (delta*i), min[i], max[i]));

    setSamples(samples);
}

int PercentileCurve::rtti() const
{
    return Rtti_Percentile;
}

ParameterType PercentileCurve::getType() const
{
    return type;
}

void PercentileCurve::setType(ParameterType type)
{
    this->type = type;
}

void PercentileCurve::yScaleChangedImpl()
{
    if (!intervals.isEmpty())
        setSamples(intervals.data(xScaleType(), yScaleType()));
}

void PercentileCurve::setColor(QColor c)
{
    // Transparency for surface
    c.setAlpha(qSqrt(c.alpha()) *3);
    QwtPlotIntervalCurve::setBrush(QBrush(c));

    // Invisible lines
    c.setAlpha(0);
    setPen(QPen(c));
}
