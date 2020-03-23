#include "chart.h"

#include "coremessagehandler.h"
#include "core.h"
#include "core_errors.h"
#include "plotpicker.h"
#include "chartitemgroup.h"
#include "item.h"
#include "curve.h"

#include <QFile>
#include <QDesktopWidget>
#include <QApplication>
#include <QMenu>
#include <QPainter>

#include <limits>

#include <qwt_plot_renderer.h>
#include <qwt_plot_grid.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_item.h>
#include <qwt_plot_panner.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_scale_engine.h>

Chart::Chart(QWidget *parent) : QwtPlot(parent), xScaleType(Linear), yScaleType(Linear)
{
    //Background color
    this->setCanvasBackground(QBrush(QColor(Qt::white)));

    //Scale Draw
    timeScale = new TimeScaleDraw();    
    setAxisScaleDraw(QwtPlot::xBottom, timeScale);   
    setAxisAutoScale(QwtPlot::xBottom, false);

    // Point projection of the cursor on curve
    pointOnCurve = new QwtPlotMarker();
    pointOnCurve->setZ(1000); // To be sure that it is always on top
    pointOnCurve->setSymbol(new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::SolidPattern), QPen(), QSize(6,6)));
    pointOnCurve->attach(this);

    // Add a legend for the curves : Needs to be improved
    QwtLegend *legend = new QwtLegend();
    legend->setDefaultItemMode(QwtLegendData::Clickable);
    insertLegend(legend, QwtPlot::TopLegend);
    connect(legend, SIGNAL(clicked(QVariant,int)), this, SLOT(onLegendClicked(QVariant,int)));

    // Create the flashpoint
    flashPoint = new QwtPlotMarker();
    flashPoint->attach(this);
    timeline = new QTimeLine(FLASH_DURATION);
    timeline->setFrameRange(0,255);
    connect(timeline, SIGNAL(frameChanged(int)), this, SLOT(onFlashTimer(int)));
    connect(timeline, SIGNAL(finished()), this, SLOT(onFlashFinished()));

    // Add a picker to get the clicks on the plot
    PlotPicker* picker = new PlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, (QwtPlotCanvas*) this->canvas());
    picker->setStateMachine(new QwtPickerClickPointMachine());
    connect(picker, SIGNAL(on_clicked(QPointF,QMouseEvent*)), this, SLOT(onMouseClicked(QPointF,QMouseEvent*)));
    connect(picker, SIGNAL(on_double_clicked(QPointF)), this, SLOT(onMouseDoubleClicked(QPointF)));
    connect(picker, SIGNAL(on_mouse_moved(QPointF)), this, SLOT(onMouseMoved(QPointF)));
    connect(picker, SIGNAL(on_mouse_leaved()), this, SLOT(onMouseLeaved()));
    connect(picker, SIGNAL(on_mouse_wheeled(bool)), this, SIGNAL(mouseWheeled(bool)));

    // For panning
    QwtPlotPanner* panner = new QwtPlotPanner(canvas());
    ((QwtPlotCanvas*) canvas())->setFrameStyle(QFrame::NoFrame);
    panner->setMouseButton(Qt::LeftButton);
    panner->setOrientations(Qt::Horizontal);
    connect(panner, SIGNAL(panned(int,int)), this, SLOT(onPlotPanned(int,int)));

    // Display preferencies
    showTargets = true;
    showMarkers = true;
}

Chart::~Chart()
{
    // At first, we need to remove the groups
    foreach (ChartItemGroup* group, groups)
        delete group;
}

void Chart::attachItem(QwtPlotItem *item)
{
    //Attach the item
    item->attach(this);

    //Setup the item
    AbstractItem *absItem = dynamic_cast<AbstractItem *>(item);

    if (absItem) {

        //Connect the item
        connect(this, SIGNAL(xScaleTypeChanged(Chart::ScaleType)), absItem, SLOT(xScaleChanged(Chart::ScaleType)));
        connect(this, SIGNAL(yScaleTypeChanged(Chart::ScaleType)), absItem, SLOT(yScaleChanged(Chart::ScaleType)));

        //Set the properties
        absItem->setXScaleType(getXScaleType());
        absItem->setYScaleType(getYScaleType());
    }

    //Replot the chart
    replot();
}

void Chart::attachGroup(ChartItemGroup* group)
{
    group->attach(this);
    groups.append(group);
}

void Chart::detachItem(QwtPlotItem *item)
{
    //Detach the item
    item->detach();

    //Unset the item
    AbstractItem *absItem = dynamic_cast<AbstractItem *>(item);

    if (absItem) {

        //Disconnect the item
        connect(this, SIGNAL(xScaleTypeChanged(Chart::ScaleType)), absItem, SLOT(xScaleChanged(Chart::ScaleType)));
        connect(this, SIGNAL(yScaleTypeChanged(Chart::ScaleType)), absItem, SLOT(yScaleChanged(Chart::ScaleType)));
    }

    //Replot the chart
    replot();
}

void Chart::detachGroup(ChartItemGroup* group)
{
    groups.removeAll(group);
    group->detach();
}

void Chart::setRange(QDateTime start, QDateTime stop)
{
    qDebug() << "Set Chart range to : " + start.toString() + " - " + stop.toString();

    // Set the interval
    setAxisScaleDiv(QwtPlot::xBottom, dateScaleDiv(start, stop));

    // We have to reset the autoscale everytime after having panned
    setAxisAutoScale(QwtPlot::yLeft);
    updateAxes();
    replot();
}

void Chart::setXScaleType(Chart::ScaleType type)
{
    if (type == getXScaleType())
        return;

    emit xScaleTypeChanged(type);

    xScaleType = type;
}

Chart::ScaleType Chart::getXScaleType() const
{
    return xScaleType;
}

void Chart::setYScaleType(ScaleType type)
{
    //Ignore if unmodified
    if (type == getYScaleType())
        return;

    //Set the new engine
    switch (type) {
    case Linear:
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());
        break;
    case Log10:
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine());
        break;
    default:
        ERROR(DATAERROR, tr("Unknown scale type '%1'").arg((int)type));
    }

    //Signal the change
    emit yScaleTypeChanged(type);

    //Set the new value
    yScaleType = type;
}

Chart::ScaleType Chart::getYScaleType() const
{
    return yScaleType;
}

void Chart::setAxesNames(QString x, QString y)
{
    setAxisTitle(QwtPlot::xBottom, x);
    setAxisTitle(QwtPlot::yLeft, y);

    replot();
}

//Use [px]
bool Chart::save(QString filename, int width, int height)
{
    QwtPlotRenderer renderer;
    QSizeF size;
    const int dpi = QApplication::desktop()->physicalDpiX();
    size.setHeight((25.4*height) / dpi);
    size.setWidth((25.4*width) / dpi);

    //replot(); -- done in onSelectCurve()
    renderer.renderDocument(this, filename, size, dpi);
    qDebug() << QString("Save chart under '%1' in %2x%3 [px] (%4 dpi)").arg(filename).arg(size.width()).arg(size.height()).arg(dpi);

    return QFile::exists(filename);
}

//Use [mm]
bool Chart::save(QString filename, int width, int height, int dpi)
{
    QwtPlotRenderer renderer;
    QSizeF size;
    size.setHeight(height);
    size.setWidth(width);

    //If no DPI is given, use the screen one
    if (dpi == -1)
        dpi = QApplication::desktop()->physicalDpiX();

    //replot(); -- done in onSelectCurve()
    renderer.renderDocument(this, filename, size, dpi);
    qDebug() << QString("Save chart under '%1' in %2x%3 [mm] (%4 dpi)").arg(filename).arg(size.width()).arg(size.height()).arg(dpi);

    return QFile::exists(filename);
}

void Chart::renderTo(QPaintDevice &device)
{
    QwtPlotRenderer renderer;
    renderer.renderTo(this,device);
}

void Chart::showGrid(bool show)
{
    // Remote the old grids
    detachItems(QwtPlotItem::Rtti_PlotGrid);

    if (show)
    {
        QwtPlotGrid* grid = new QwtPlotGrid();
        grid->attach(this);

        // Enable or disable the axes
        grid->enableX(show);
        grid->enableY(show);

        grid->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));
        grid->setMinorPen(QPen(Qt::black, 0, Qt::DotLine));
    }

    replot();
}

void Chart::print(QPrinter & printer)
{
    QwtPlotRenderer renderer;
    renderer.renderTo(this, printer);
}

void Chart::flashPointAt(QDateTime time, double amount)
{
    if (timeline->state() != QTimeLine::NotRunning)
        timeline->stop();

    // Create the marker
    QwtSymbol* symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::red), QPen(Qt::red), QSize(0,0));
    flashPoint->setSymbol(symbol);
    flashPoint->attach(this);
    flashPoint->setValue(time.toTime_t(), amount);
    flashPoint->setVisible(true);
    flashPoint->setZ(1000);

    timeline->start();
}

void Chart::onFlashTimer(int frame)
{
    QColor color(Qt::red);
    color.setAlpha(frame < 30 ? 255/(30-frame) : 255 - frame);
    QwtSymbol* symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(color), QPen(color), QSize(frame/4,frame/4));
    flashPoint->setSymbol(symbol);
}

void Chart::onFlashFinished()
{
    flashPoint->setVisible(false);
    repaint();
}

void Chart::drawItems(QPainter *painter, const QRectF &rect, const QwtScaleMap maps[]) const
{
    // Sets the painter as antialiased. This is needed because we are overriding the default painting process
    painter->setRenderHint(QPainter::Antialiasing);

    foreach (QwtPlotItem *item, itemList())

        // Displays targets or markers depending on settings
        if (item->isVisible() && (showTargets || (item->rtti() != Rtti_Target)) && (showMarkers || (item->rtti() != Rtti_Line)))
            item->draw(painter, maps[QwtPlot::xBottom], maps[QwtPlot::yLeft], rect);
}

void Chart::manageMouseClick(QPointF point, bool doubleClicked)
{
    // The idea is to find the curve with the biggest z that is near the mouse. Unselect every other curve
    Curve* selection = closestCurve(point);

    // Set the winner as selected
    if (selection)
    {
        pointOnCurve->setValue(selection->getXProjPoint(point));
        replot();

        if (doubleClicked)
            selection->onDoubleClick();
        else
            selection->onClick();
    }

    // Else we clicked on nothing
    else
    {
        pointOnCurve->hide();
        replot();

        if (doubleClicked)
            emit mouseDoubleClicked(QDateTime::fromTime_t(point.x()), point.y());
        else
            emit mouseClicked(QDateTime::fromTime_t(point.x()), point.y());
    }
}

QwtScaleDiv Chart::dateScaleDiv(QDateTime from, QDateTime to)
{
    QList<double> ticks[QwtScaleDiv::NTickTypes];
    QDateTime cursor = from;
    int step = 3600;

    // Ignore seconds and minutes
    if (cursor.time().second() || cursor.time().minute())
    {
        cursor = cursor.addSecs(60-cursor.time().second());
        cursor = cursor.addSecs(60 * (60-cursor.time().minute()));
    }

    // We are in a day range
    if (from.secsTo(to) > (8*3600))
    {
        if (cursor.time().hour()%6)
            cursor = cursor.addSecs(3600 * (6-(cursor.time().hour()%6)));

        step = 3600 * 6;
    }

    // We are in a bigger range, now only show some days
    if (from.daysTo(to) >= 2)
    {
        if (cursor.time().hour())
            cursor = cursor.addSecs(3600 * (24-(cursor.time().hour())));

        step = 3600 * 24 * (1+(from.daysTo(to) / 6));
    }

    // Count the ticks
    while (cursor < to)
    {
        ticks[QwtScaleDiv::MajorTick].append(cursor.toTime_t());

        // For the days steps, add a medium at midday, a minor at 6
        if (step == (3600*24))
        {
            ticks[QwtScaleDiv::MediumTick].append(cursor.toTime_t() + (step/2));
            ticks[QwtScaleDiv::MinorTick].append(cursor.toTime_t() + (step/4));
            ticks[QwtScaleDiv::MinorTick].append(cursor.toTime_t() + (3*step/4));
        }

        cursor = cursor.addSecs(step);
    }

    // Set the interval
    QwtScaleDiv div((double) from.toTime_t(), (double) to.toTime_t(), ticks);
    return div;
}

Curve *Chart::closestCurve(QPointF point, bool ignoreFadedCurves)
{
    Curve* curve = NULL;
    QPointF nearest;
    double nearestDist = std::numeric_limits<double>::max();

    // Get the neareste projection
    foreach (QwtPlotItem* item, itemList(Rtti_Curve))
    {
        // If we want to ignore faded curves
        if (ignoreFadedCurves && (((Curve*) item)->pen().color().alpha() < FADE_MIN_LVL))
            continue;

        QPointF pF = ((Curve*) item)->getXProjPoint(point);
        double d = qAbs(pF.y() - point.y());

        // If it is the first curve, or this one is closest than the last, keep it
        if (!pF.isNull() && (nearest.isNull() || d < nearestDist))
        {
            // This is the new point
            nearest = pF;
            nearestDist = qAbs(nearest.y() - point.y());
            curve = (Curve*) item;
        }
    }

    return curve;
}

void Chart::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu;

    // Show or hide the targets
    QAction actShowTargets(tr("Show Targets"), this);
    actShowTargets.setCheckable(true);
    actShowTargets.setChecked(showTargets);
    contextMenu.addAction(&actShowTargets);

    // Show or hide the markers
    QAction actShowMarkers(tr("Show Markers"), this);
    actShowMarkers.setCheckable(true);
    actShowMarkers.setChecked(showMarkers);
    contextMenu.addAction(&actShowMarkers);

    // Set or unset the log scale
    QAction actShowLogScale(tr("Show log scale"), this);
    actShowLogScale.setCheckable(true);
    actShowLogScale.setChecked(getYScaleType() == Log10);
    contextMenu.addAction(&actShowLogScale);

    // Show the menu
    QAction* act = contextMenu.exec(pos);

    // Manager the action
    if (act == &actShowMarkers)
        showMarkers = act->isChecked();
    else if (act == &actShowTargets)
        showTargets = act->isChecked();
    else if (act == &actShowLogScale)
        act->isChecked() ? setYScaleType(Log10) : setYScaleType(Linear);

    replot();
}

void Chart::onMouseLeaved()
{
    pointOnCurve->hide();
    replot();
}

void Chart::onMouseClicked(QPointF point, QMouseEvent *event)
{
    qDebug() << "Clicked on plot. Time : " + QDateTime::fromTime_t(point.x()).toString() + ", val : " + QString::number((double) point.y());

    // If right button, show the contextual menu
    if (event->button() == Qt::RightButton)
        showContextMenu(event->globalPos());

    // Else process the click as a selection
    else
        manageMouseClick(point);
}

void Chart::onMouseDoubleClicked(QPointF point)
{
    qDebug() << "Double Clicked on plot. Time : " + QDateTime::fromTime_t(point.x()).toString() + ", val : " + QString::number((double) point.y());

    manageMouseClick(point, true);
}

void Chart::onLegendClicked(QVariant info, int id)
{
    Q_UNUSED(id);

    QwtPlotItem *item = infoToItem(info);

    if (item->rtti() == Rtti_Curve)
        ((Curve*) item)->onClick();
}

void Chart::onMouseMoved(QPointF point)
{
    Curve* curve = closestCurve(point, true);

    if (curve)
    {
        QPointF pF = curve->getXProjPoint(point);

        if (pF.isNull())
            pointOnCurve->hide();
        else
            pointOnCurve->show();

        pointOnCurve->setValue(pF);
        emit pointOnCurveMoved(QDateTime::fromTime_t((uint) pF.x()), pF.y());
        replot();
    }
}

void Chart::onPlotPanned(int, int)
{
    emit plotRangePanned(QDateTime::fromTime_t(axisScaleDiv(QwtPlot::xBottom).lowerBound()), QDateTime::fromTime_t(axisScaleDiv(QwtPlot::xBottom).upperBound()));
}

