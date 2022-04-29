//@@license@@

#ifndef LINE_H
#define LINE_H

#include "chartitem.h"

#include <qwt_plot_marker.h>
#include <QDateTime>
#include <qwt_symbol.h>

class Line : public QwtPlotMarker
{
public:
    explicit Line(QwtPlotMarker::LineStyle style = HLine);

    void setVPos(double concentration);
    void setHPos(const QDateTime& time);

    void setColor(QColor c);

    void setPrevText(const QString& text);
    void setNextText(const QString& text);

    int rtti() const;

    void draw(QPainter* p, const QwtScaleMap& xMap, const QwtScaleMap& yMap, const QRectF& rect) const;

private:
    // Protect the Qwt standart attach/detach mechanism
    void attach(QwtPlot *plot);
    void detach();

    QString prevText;
    QString nextText;

    friend class Chart;
};

#endif // LINE_H
