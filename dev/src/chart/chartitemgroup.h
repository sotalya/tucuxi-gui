//@@license@@

#ifndef CHARTITEMGROUP_H
#define CHARTITEMGROUP_H

#include "chart.h"

#include <QObject>
#include <qwt_plot_item.h>

#define FADE_MAX_LVL    30
#define FADE_MIN_LVL    255
#define FADE_DURATION   300

class ChartItemGroup : public QObject
{
    Q_OBJECT

public:
    explicit ChartItemGroup();
    virtual ~ChartItemGroup();

    void attachItem(QwtPlotItem* item);
    void detachItem(QwtPlotItem* item);

public slots:
    void fade(bool b = true);
    void refresh();

private slots:
    void setFading(int fadeLvl);
    void setVisible(bool b = true);

private:
    void attachToChart(QwtPlotItem* item);
    void setColor(QwtPlotItem* item);

    Chart* chart;
    QColor refColor;
    QList<QwtPlotItem*> items;

    int fadeLvl;

    // Protect the Qwt standart attach/detach mechanism
    void attach(Chart* chart);
    void detach();

    friend class Chart;
};

#endif // CHARTITEMGROUP_H
