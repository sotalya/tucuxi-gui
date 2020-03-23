#ifndef CHART_H
#define CHART_H

class Curve;

#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <QDateTime>
#include <QPixmap>
#include <QTimeLine>
#include <QMouseEvent>
#include <QPrinter>

#include "timescaledraw.h"
#include "chartcolormanager.h"

#define FLASH_DURATION 1000

class ChartItemGroup;

class Chart : public QwtPlot, public ChartColorManager
{
    Q_OBJECT

public:
    enum ScaleType {
        Linear,
        Log10
    };

    explicit Chart(QWidget* parent = 0);
    virtual ~Chart();

    void attachItem(QwtPlotItem* item);
    void attachGroup(ChartItemGroup* group);

    void detachItem(QwtPlotItem* item);
    void detachGroup(ChartItemGroup* group);

    //In [px] (Use the display's DPI)
    bool save(QString filename, int width = 800, int height = 600);
    //In [mm]
    bool save(QString filename, int width, int height, int resolution);
    void renderTo(QPaintDevice &device);
    void setAxesNames(QString x, QString y);
    void print(QPrinter& printer);
    void showGrid(bool show);
    void setRange(QDateTime start, QDateTime stop);

    void setXScaleType(ScaleType type);
    ScaleType getXScaleType() const;

    void setYScaleType(ScaleType type);
    ScaleType getYScaleType() const;

signals:
    
    // Clicked on nothing
    void mouseClicked(QDateTime time, double amout);
    void mouseDoubleClicked(QDateTime time, double amout);
    void pointOnCurveMoved(QDateTime time, double amout);

    void plotRangePanned(QDateTime start, QDateTime stop);
    void mouseWheeled(bool up);

    void xScaleTypeChanged(Chart::ScaleType type);
    void yScaleTypeChanged(Chart::ScaleType type);

public slots:

    void flashPointAt(QDateTime time, double amount);

private slots:

    void onMouseClicked(QPointF point, QMouseEvent *event);
    void onMouseDoubleClicked(QPointF point);
    void onMouseMoved(QPointF point);
    void onMouseLeaved();

    void onPlotPanned(int dx, int dy);

    void onLegendClicked(QVariant info, int id);

    void onFlashTimer(int frame);
    void onFlashFinished();

public:
    void drawItems(QPainter *painter, const QRectF &rect, const QwtScaleMap maps[axisCnt]) const;

private:
    void manageMouseClick(QPointF point, bool doubleClicked = false);
    QwtScaleDiv dateScaleDiv(QDateTime from, QDateTime to);

    // Return the closest curve, if there is one very near
    Curve* closestCurve(QPointF point, bool ignoreFadedCurves = false);

    // Our time (X axis) scale
    TimeScaleDraw *timeScale;

    // We have to manager attached groups
    QList<ChartItemGroup *> groups;

    QwtPlotMarker* pointOnCurve;

    // Flasing point utils
    QTimeLine *timeline;
    QwtPlotMarker* flashPoint;

    // Display settings
    void showContextMenu(const QPoint &pos);
    ScaleType xScaleType;
    ScaleType yScaleType;
    bool showMarkers;
    bool showTargets;
};

#endif // CHART_H

