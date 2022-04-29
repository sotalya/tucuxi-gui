//@@license@@

#include "curve.h"

#include <qwt_plot.h>

Curve::Curve(ParameterType type) : QwtPlotCurve(), type(type), highlighted(false)
{
    /* No more needed. To be confirmed before deletion
    // Antialiasing, on windows, 7 only, on the rest, it rocks!
#ifdef Q_OS_WIN32
    if (QSysInfo::WindowsVersion == QSysInfo::WV_WINDOWS7)
#endif
    setRenderHint(QwtPlotItem::RenderAntialiased);
    */

    // Set a width of 2 for all interresting curves
    QPen p = pen();
    if (type != TMP)
        p.setWidth(2);
    else
        p.setWidth(1);
    setPen(p);
}

// TODO : The usage of setRawSamples may accelerate the execution

void Curve::setDataset(const QDateTime& start, const QDateTime& stop, const DataSet& dataset)
{
    Q_UNUSED(start);
    Q_UNUSED(stop);

    QVector<double> data;
    QVector<double> time;
    dataset.getDataTimes(data, time);

    points.setRawData(data, time);
    setSamples(points.data(xScaleType(), yScaleType()));
}

void Curve::setPoints(const QDateTime& start, const Duration& duration, double *points, int nbPoints)
{
    QDateTime stop(start);
    stop = stop.addSecs(duration.seconds());

    // Create an array of times
    double times[nbPoints];
    double delta = (double) duration.seconds() / (double) (nbPoints-1);
    for (int i=0; i<nbPoints; i++)
        times[i] = (double) start.toTime_t() + (delta * i);

    setSamples(times, points, nbPoints);    
}

int Curve::rtti() const
{
    return Rtti_Curve;
}

ParameterType Curve::getType() const
{
    return type;
}

void Curve::setType(ParameterType type)
{
    this->type = type;
}

bool Curve::isNear(const QPoint& p)
{
    // At first, check if we are near a point
    double dist;

    closestPoint(p, &dist);
    if (dist < 10)
        return true;

    // If it is not the case, check if we are on a segment between 2 points
    for (unsigned int i=0; i<dataSize()-1; i++)
    {

        int x1 = plot()->transform(QwtPlot::xBottom, sample(i).x());
        int x2 = plot()->transform(QwtPlot::xBottom, sample(i+1).x());

        if ((p.x() >= x1) && (p.x() < x2))
        {
            int y1 = plot()->transform(QwtPlot::yLeft, sample(i).y());
            int y2 = plot()->transform(QwtPlot::yLeft, sample(i+1).y());

            // Check if we are on the segment (length)
            if (pow(x2-x1, 2) + pow(y2-y1, 2) > pow(p.x()-x1, 2) + pow(p.y()-y1, 2))
            {
                // Check if the angle is near
                double alpha = atan(double(y2-y1) / double(x2-x1));
                if (fabs(alpha - atan(double(p.y()-y1) / double(p.x()-x1))) < 0.1)
                    return true;
            }

            return false;
        }
    }

    return false;
}

QPointF Curve::getXProjPoint(const QPointF& point)
{
    // Check if on the curve
    if (point.x() < sample(0).x() || point.x() > sample(dataSize()-1).x())
        return QPointF();

    // For each point on the curve
    for (unsigned int i=1; i<dataSize(); i++)        
    {
        // Find the nearest
        if (sample(i).x() >= point.x())
        {
            // We need to check if there are more than one point at the same x position
            unsigned int bestId = i;
            double bestDist = qAbs(sample(i).y() - point.y());

            for (unsigned int j=i+1; j<dataSize() && sample(j).x() == sample(i).x(); j++)
            {
                // Calculate the distance
                double d = qAbs(sample(j).y() - point.y());

                if (d < bestDist)
                {
                    bestId = j;
                    bestDist = d;
                }
            }

            return sample(bestId);
        }
    }

    return QPointF();
}

QColor Curve::getColor() const
{
    return pen().color();
}

void Curve::setColor(QColor c)
{
    QPen p = pen();

    QColor current = c;

    //Set the alpha if proposition
    if (type == TMP)
        current.setAlpha(c.alpha());

    //Set the new color
    p.setColor(current);

    //Set the updated pen
    setPen(p);
}

void Curve::onDoubleClick()
{
    emit doubleClicked();
}

void Curve::yScaleChangedImpl()
{
    if (!points.isEmpty())
        setSamples(points.data(xScaleType(), yScaleType()));
}

void Curve::onClick()
{
    emit clicked();
}

void Curve::highlight(bool b)
{
    // If we are already in the correct state, do nothing
    if (highlighted == b)
        return;
    else
        highlighted = b;

    QPen p = pen();
    p.setWidth(b ? p.width()*2 : p.width()/2);
    setPen(p);
    plot()->replot();
}
