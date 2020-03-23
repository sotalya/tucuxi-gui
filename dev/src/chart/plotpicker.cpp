#include "plotpicker.h"

#include "coremessagehandler.h"
#include "core.h"
#include <QDateTime>
#include "iostream"
#include "qwt_plot.h"
#include <QMouseEvent>

PlotPicker::PlotPicker(int xAxis, int yAxis, RubberBand rubberBand, DisplayMode trackerMode, QwtPlotCanvas * canvas)
    : QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, canvas)
{
}

QwtText PlotPicker::trackerTextF(const QPointF&) const
{
    return QwtText();
}

void PlotPicker::widgetMouseDoubleClickEvent(QMouseEvent* event)
{
    QwtPlotPicker::widgetMouseDoubleClickEvent(event);

    const QPointF point = QPointF(plot()->invTransform(QwtPlot::xBottom, event->x()), plot()->invTransform(QwtPlot::yLeft, event->y()));
    emit on_double_clicked(point);
}

void PlotPicker::widgetMouseMoveEvent(QMouseEvent* event)
{
    QwtPlotPicker::widgetMouseMoveEvent(event);

    const QPointF point = QPointF(plot()->invTransform(QwtPlot::xBottom, event->x()), plot()->invTransform(QwtPlot::yLeft, event->y()));
    emit on_mouse_moved(point);
}

void PlotPicker::widgetMousePressEvent(QMouseEvent* event)
{
    QwtPlotPicker::widgetMousePressEvent(event);

    position = event->pos();
}

void PlotPicker::widgetMouseReleaseEvent(QMouseEvent* event)
{
    QwtPlotPicker::widgetMouseReleaseEvent(event);

    // Only click if the mouse was released in the same position than the pressed event
    if (position.isNull() || (position == event->pos()))
    {
        const QPointF point = QPointF(plot()->invTransform(QwtPlot::xBottom, event->x()), plot()->invTransform(QwtPlot::yLeft, event->y()));
        emit on_clicked(point, event);
    }
}

void PlotPicker::widgetWheelEvent(QWheelEvent* event)
{
    emit on_mouse_wheeled(event->delta() > 0);
}

void PlotPicker::widgetLeaveEvent(QEvent* event)
{
    QwtPlotPicker::widgetLeaveEvent(event);

    emit on_mouse_leaved();
}

void PlotPicker::widgetEnterEvent(QEvent* event)
{
    QwtPlotPicker::widgetEnterEvent(event);

    emit on_mouse_entered();
}
