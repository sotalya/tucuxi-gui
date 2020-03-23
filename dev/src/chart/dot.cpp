#include "dot.h"

Dot::Dot(QwtSymbol::Style style) :
    QwtPlotMarker()
{
    symbol = new QwtSymbol;
    symbol->setStyle(style);
    symbol->setBrush(QBrush(Qt::SolidPattern));
    symbol->setSize(8, 8);

    QPen pen;
    pen.setWidth(3);
    symbol->setPen(pen);
    setSymbol(symbol);

    setItemAttribute(QwtPlotItem::AutoScale);
}

void Dot::setPos(const QDateTime& time, double concentration)
{
    point.setRawData(concentration, time.toTime_t());
    setValue(point.data(xScaleType(), yScaleType()));
}

void Dot::setColor(QColor c)
{    
    symbol->setBrush(QBrush(c));
    setSymbol(symbol);
}

int Dot::rtti() const
{
    return Rtti_Dot;
}

QRectF Dot::boundingRect() const
{
    return QRectF(value(), value());
}

void Dot::yScaleChangedImpl()
{
    if (!point.isEmpty())
        setValue(point.data(xScaleType(), yScaleType()));
}
