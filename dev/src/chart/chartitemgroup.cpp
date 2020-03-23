#include "chartitemgroup.h"
#include "chartitem.h"
#include "percentilecurve.h"
#include "dot.h"
#include "line.h"
#include "curve.h"
#include "targetinterval.h"

#include <QTimeLine>

ChartItemGroup::ChartItemGroup() :
    QObject(NULL), chart(NULL), fadeLvl(FADE_MIN_LVL)
{
}

ChartItemGroup::~ChartItemGroup()
{
    if (chart)
        detach();

    // The group is responsible for items that are attached to it
    foreach (QwtPlotItem* item, items)
        delete item;
}

void ChartItemGroup::setFading(int fadeLvl)
{
    // Update the current colors
    this->fadeLvl = fadeLvl;
    refColor.setAlpha(fadeLvl);

    // Hide the targets
    if (fadeLvl == FADE_MAX_LVL)
    {
        setVisible(false);
    }
    else
    {
        // Change the pens of all items
        foreach (QwtPlotItem *item, items)
        {
            setColor(item);

            // Set the z axis depending on the fading, but the targets on the top
            item->setZ(fadeLvl + (item->rtti() == Rtti_Target ? -1 : 0));
        }
    }

    if (chart)
        chart->replot();
}

void ChartItemGroup::setVisible(bool b)
{
    // Show/hide selected targets and dots
    foreach (QwtPlotItem *item, items)
        if (item->rtti() == Rtti_Dot || item->rtti() == Rtti_Line || item->rtti() == Rtti_Target)
            item->setVisible(b);
}

void ChartItemGroup::fade(bool b)
{
    int from = fadeLvl;
    int to = (b ? FADE_MAX_LVL : FADE_MIN_LVL);

    // If we are attached to a chart, we have to change all items
    if (chart)
    {
        // We want to fade in 300ms
        QTimeLine *time = new QTimeLine(FADE_DURATION, this);
        time->setFrameRange(from, to);
        connect(time, SIGNAL(frameChanged(int)), this, SLOT(setFading(int)));
        time->start();
    }
    else
        setFading(to);

    // Show the targets if needed
    if (!b && (from == FADE_MAX_LVL))
        setVisible(true);
}

void ChartItemGroup::refresh()
{
    if (chart) {

        // Remove the old color
        chart->freeColor(refColor);

        // Get a new one
        // Find the main curve
        foreach (QwtPlotItem *item, items)
            if (item->rtti() == Rtti_Curve)
                refColor = chart->getFreeColor(((Curve*) item)->getType());

        setFading(fadeLvl);
    }
}

void ChartItemGroup::setColor(QwtPlotItem* item)
{    
    switch (item->rtti())
    {
    case Rtti_Curve :
        //Only update if not a proposition
        if (((Curve*) item)->getType() != TMP)
            ((Curve*) item)->setColor(refColor);
        break;

    case Rtti_Percentile :
    {
        QColor c(chart->getDefaultColor(((PercentileCurve*) item)->getType()));
        c.setAlpha(refColor.alpha());
        ((PercentileCurve*) item)->setColor(c);
        break;
    }

    case Rtti_Dot :
        ((Dot*) item)->setColor(refColor);
        break;

    case Rtti_Line :
        ((Line*) item)->setColor(refColor);
        break;

    case Rtti_Target :
        ((TargetInterval*) item)->setColor(refColor);
        break;
    }
}

void ChartItemGroup::attachToChart(QwtPlotItem* item)
{
    // Check if it is a curve
    if (item->rtti() == Rtti_Curve)
    {
        // Set the group color depending of the curve type and only if
        if (refColor == QColor())
            refColor = chart->getFreeColor(((Curve*) item)->getType());
    }

    setFading(fadeLvl);
    chart->attachItem(item);
}

void ChartItemGroup::attach(Chart* chart)
{
    this->chart = chart;

    foreach (QwtPlotItem* item, items)
        attachToChart(item);
}

void ChartItemGroup::detach()
{
    foreach (QwtPlotItem* item, items)
        if (chart)
            chart->detachItem(item);

    // Free the used color
    chart->freeColor(refColor);
    refColor = QColor();

    chart = NULL;
}

void ChartItemGroup::attachItem(QwtPlotItem* item)
{
    if (!items.contains(item))
    {                
        items.append(item);
        if (chart)
            attachToChart(item);
    }
}

void ChartItemGroup::detachItem(QwtPlotItem* item)
{
    items.removeAll(item);
    if (chart)
        chart->detachItem(item);
}
