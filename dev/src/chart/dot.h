#ifndef DOT_H
#define DOT_H

#include "chartitem.h"
#include "item.h"

#include <qwt_plot_marker.h>
#include <QDateTime>
#include <qwt_symbol.h>

class Dot : public AbstractItem, public QwtPlotMarker
{
public:
    explicit Dot(QwtSymbol::Style style = QwtSymbol::Ellipse);
    
    void setPos(const QDateTime& time, double concentration);
    void setColor(QColor c);

    int rtti() const;    
    QRectF boundingRect() const;

private:
    virtual void yScaleChangedImpl();

    // Protect the Qwt standart attach/detach mechanism
    void attach(QwtPlot *plot);
    void detach();

    PointData point;
    QwtSymbol *symbol;

    friend class Chart;
};

#endif // DOT_H
