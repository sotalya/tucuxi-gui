//@@license@@

#include "line.h"

#include <QPainter>
#include "qwt_scale_map.h"
#include "qwt_plot.h"

Line::Line(QwtPlotMarker::LineStyle style) : QwtPlotMarker()
{
    setLineStyle(style);
}

void Line::setVPos(double concentration)
{
    setYValue(concentration);
}

void Line::setHPos(const QDateTime& time)
{
    setXValue(time.toTime_t());
}

void Line::setColor(QColor c)
{
    setLinePen(QPen(c));
}

void Line::setPrevText(const QString& text)
{
    prevText = text;
}

void Line::setNextText(const QString& text)
{
    nextText = text;
}

int Line::rtti() const
{
    return Rtti_Line;
}

void Line::draw(QPainter* p, const QwtScaleMap& xMap, const QwtScaleMap& yMap, const QRectF& rect) const
{
    // Draw the original vertical line
    QwtPlotMarker::draw(p, xMap, yMap, rect);

    // Only for vertical lines at the moment
    if (lineStyle() == VLine)
    {
        // Antialiasing, on windows, 7 only, on the rest, it rocks!
    #ifdef Q_OS_WIN32
        if (QSysInfo::WindowsVersion == QSysInfo::WV_WINDOWS7)
    #endif

        // Set a brush to fill the triangles
        p->setRenderHint(QPainter::Antialiasing);

            double xPos = xMap.transform(xValue());
            double minDistLeft = xPos;
            double minDistRight = rect.right()-xPos;

            // Draw as second vertical line
            p->setPen(QColor(Qt::black));
            p->drawLine(xMap.transform(xValue())-1, 0, xPos-1, rect.bottom());

            // In order to not overlap texts, we have to find the nearest line position
            foreach (QwtPlotItem* item, plot()->itemList(Rtti_Line))
            {
                Line* line = (Line*) item;

                // If it is a vertical line, it will have an interest for us
                if (line != this && line->lineStyle() == VLine)
                {
                    // Get the other line position
                    double pos = xMap.transform(line->xValue());

                    // And the minimum left position
                    if (pos < xPos && ((xPos-pos) / 2) < minDistLeft)
                        minDistLeft = ((xPos-pos) / 2);

                    // And the right one
                    if (pos > xPos && ((pos-xPos) / 2) < minDistRight)
                        minDistRight = ((pos-xPos) / 2);
                }
            }

            // Draws the labels
            if (prevText.size())
            {
                QString text = p->fontMetrics().elidedText(prevText, Qt::ElideRight, minDistLeft-20);
                p->drawText(xPos - p->fontMetrics().width(text) - 10, 20, text);
            }

            if (nextText.size())
            {
                QString text = p->fontMetrics().elidedText(nextText, Qt::ElideLeft, minDistRight-20);
                p->drawText(xPos + 10, 20, text);
        }
    }
}
