#include "targetinterval.h"
#include "core.h"
#include "coremessagehandler.h"
#include "core_errors.h"

TargetInterval::TargetInterval(Target::TargetType type) : QwtPlotMarker()
{   
    this->type = type;
    brush = QBrush(Qt::SolidPattern);

    setItemAttribute(QwtPlotItem::AutoScale);
}

void TargetInterval::setTarget(const Target* &target)
{
    this->target = target;

    points.setRawData(target->cmin().value(), target->cmax().value(), target->cbest().value());
    refreshTarget();
}

void TargetInterval::setDataSet(const DataSet &data)
{
    this->dataset = data;
}

QPen TargetInterval::createPen(int r, int g, int b, int alpha, int width) const
{
    QPen pen(QColor(r,g,b,alpha));
    pen.setWidth(width);
    return pen;
}

void TargetInterval::refreshTarget()
{
    target->cmin().setValue(points.minData(xScaleType(), yScaleType()));
    target->cmax().setValue(points.maxData(xScaleType(), yScaleType()));
    target->cbest().setValue(points.bestData(xScaleType(), yScaleType()));
}

void TargetInterval::drawPeak(QPainter* p, const QwtScaleMap& xMap, const QwtScaleMap& yMap, const QRectF& rect) const
{
    int yMax = yMap.transform(target->cmax().value());
    int yMin = yMap.transform(target->cmin().value());
    int yBest = yMap.transform(target->cbest().value());

    // Compute the pattern
    double start = dataset.allCycles().first().startTime().toTime_t();
    double stop = dataset.allCycles().last().endTime().toTime_t();
    QLinearGradient g(xMap.transform(start), yBest, xMap.transform(stop), yBest);

    if (start < stop) {
        foreach (CycleData cycle, dataset.allCycles())
        {
            if (cycle.type() == CycleData::START || cycle.type() == CycleData::COMPLETE)
            {
                // tMin
                double t = cycle.startTime().toTime_t() + target->tmin().toSeconds();
                g.setColorAt((t-start) / (stop-start), QColor(255,200,150,60));

                // tBest
                t = cycle.startTime().toTime_t() + target->tbest().toSeconds();
                g.setColorAt((t-start) / (stop-start), QColor(255,200,150,255));

                // tMax
                t = cycle.startTime().toTime_t() + target->tmax().toSeconds();
                g.setColorAt((t-start) / (stop-start), QColor(255,200,150,60));

                /*
                // Only show something that is visible :)
                double d = ((double) i) / data->size();
                if (d>0.03 && d<0.97)
                {
                    g.setColorAt(d-0.02, QColor(255,200,150,60));
                    g.setColorAt(d, QColor(255,200,150,255));
                    g.setColorAt(d+0.02, QColor(255,200,150,60));
                }
                */
            }
        }
    }

    // Draw the surface (the 45 comes from 15 (max steps of 0.06) * 3 stops)
    if ((g.stops().size() > 2) && (g.stops().size() < 45))
        p->fillRect(0, yMin, rect.width(), yMax-yMin, QBrush(g));
    else
        p->fillRect(0, yMin, rect.width(), yMax-yMin, QBrush(QColor(255,200,150,60)));

    // Set the main lines
    p->setPen(createPen(255,255,255,255,3));
    p->drawLine(0, yBest, rect.right(), yBest);

    // Add a legend
    p->setPen(QPen(QColor(Qt::gray)));
    QRectF r(rect);
    r.setBottom(yMin);
    p->drawText(r, Qt::AlignRight | Qt::AlignBottom, tr("Cpeak"));
}

void TargetInterval::drawRes(QPainter* p, const QwtScaleMap& xMap, const QwtScaleMap& yMap, const QRectF& rect) const
{
    int yMax = yMap.transform(target->cmax().value());
    int yMin = yMap.transform(target->cmin().value());
    int yBest = yMap.transform(target->cbest().value());

    // Compute the pattern
    double start = dataset.allCycles().first().startTime().toTime_t();
    double stop = dataset.allCycles().last().endTime().toTime_t();
    QLinearGradient g(xMap.transform(start), yBest, xMap.transform(stop), yBest);

    foreach (CycleData cycle, dataset.allCycles())
    {
        if (cycle.type() == CycleData::END || cycle.type() == CycleData::COMPLETE)
        {
            // Only show something that is visible :)
            double d = (cycle.endTime().toTime_t()-start) / (stop-start);

            if (d>0.03 && d<0.97)
            {
                g.setColorAt(d-0.02, QColor(175,100,30,40));
                g.setColorAt(d, QColor(175,100,30,150));
                g.setColorAt(d+0.02, QColor(175,100,30,40));
            }
        }
    }

    // Draw the surface (the 45 comes from 15 (max steps of 0.06) * 3 stops)
    if ((g.stops().size() > 2) && (g.stops().size() < 45))
        p->fillRect(0, yMin, rect.width(), yMax-yMin, QBrush(g));
    else
        p->fillRect(0, yMin, rect.width(), yMax-yMin, QBrush(QColor(175,100,30,40)));

    // Set the main lines
    p->setPen(createPen(255,255,255,255,3));
    p->drawLine(0, yBest, rect.right(), yBest);

    // Add a legend
    p->setPen(QPen(QColor(Qt::gray)));
    QRectF r(rect);
    r.setBottom(yMin);
    p->drawText(r, Qt::AlignRight | Qt::AlignBottom, tr("Cres"));
}

void TargetInterval::drawMean(QPainter* p, const QwtScaleMap&, const QwtScaleMap& yMap, const QRectF& rect) const
{
    int yMax = yMap.transform(target->cmax().value());
    int yMin = yMap.transform(target->cmin().value());
    int yBest = yMap.transform(target->cbest().value());

    // Set the main lines
    p->setPen(createPen(255,255,0,255,3));
    p->drawLine(0, yBest, rect.right(), yBest);

    p->setPen(createPen(255,255,0,70,3));
    p->drawLine(0, yMax, rect.right(), yMax);

    p->setPen(createPen(255,255,0,70,3));
    p->drawLine(0, yMin, rect.right(), yMin);

    // Draw the surface
    p->fillRect(0, yMin, rect.width(), yMax-yMin, QBrush(QColor(255,255,0,50)));

    // Add a legend
    p->setPen(QPen(QColor(Qt::gray)));
    QRectF r(rect);
    r.setBottom(yMin);
    p->drawText(r, Qt::AlignRight | Qt::AlignBottom, tr("Cmean"));
}

void TargetInterval::draw(QPainter* p, const QwtScaleMap& xMap, const QwtScaleMap& yMap, const QRectF& rect) const
{
    if (isVisible())
    {
        switch (type)
        {
        case Target::ResidualTarget:
            drawRes(p, xMap, yMap, rect);
            break;
        case Target::PeakTarget:
            drawPeak(p, xMap, yMap, rect);
            break;
        case Target::MeanTarget:
            drawMean(p, xMap, yMap, rect);
            break;
        default:
            ERROR(DATAERROR, tr("Unkown target type '%1'").arg(type));
        }
    }
}

void TargetInterval::yScaleChangedImpl()
{
    if (!points.isEmpty())
        refreshTarget();
}

/*
void TargetInterval::draw(QPainter* p, const QwtScaleMap&, const QwtScaleMap& yMap, const QRectF& rect) const
{
    int yMax = yMap.transform(max);
    int yMin = yMap.transform(min);
    int yBest = yMap.transform(best);
    int xPos = rect.right() - 5;

    // Antialiasing, on windows, 7 only, on the rest, it rocks!
#ifdef Q_OS_WIN32
    if (QSysInfo::WindowsVersion == QSysInfo::WV_WINDOWS7)
#endif

    // Set a brush to fill the triangles
    p->setRenderHint(QPainter::Antialiasing);
    p->setBrush(brush);

    // Set the pen (usefull for fading)
    QPen pen(Qt::SolidLine);
    QColor color(Qt::black);
    color.setAlpha(brush.color().alpha());
    pen.setColor(color);
    p->setPen(pen);

    // For residual targets, show a target
    if (type == ResidualTarget)
    {
        // Draw the vertical line
        p->drawLine(xPos, yMax, xPos, yMin);

        // Draw the top arrow
        QPolygon triangle;
        triangle.putPoints(0, 3, xPos,yMax, xPos-4,yMax+6, xPos+4,yMax+6);
        p->drawPolygon(triangle);
        p->drawLine(xPos-5, yMax, xPos+5, yMax);

        // Draw the diamond
        triangle.clear();
        triangle.putPoints(0, 4, xPos,yBest-6, xPos-4,yBest, xPos,yBest+6, xPos+4,yBest);
        p->drawPolygon(triangle);

        // Draw the bottom arrow
        triangle.clear();
        triangle.putPoints(0, 3, xPos,yMin, xPos-4,yMin-6, xPos+4,yMin-6);
        p->drawPolygon(triangle);
        p->drawLine(xPos-5, yMin, xPos+5, yMin);
    }

    // else show a line
    else
    {
        pen.setStyle(Qt::DashDotLine);
        p->setPen(pen);
        p->drawLine(10, yMax, rect.right()-10, yMax);
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(10, yBest, rect.right()-10, yBest);
        pen.setStyle(Qt::DashDotLine);
        p->setPen(pen);
        p->drawLine(10, yMin, rect.right()-10, yMin);
    }
}
*/
void TargetInterval::setColor(QColor c)
{
    this->brush = QBrush(c);
}

int TargetInterval::rtti() const
{
    return Rtti_Target;
}

QRectF TargetInterval::boundingRect() const
{
    return QRectF(0, target->cmax().value(), 0, target->cmax().value() - target->cmin().value());
}
