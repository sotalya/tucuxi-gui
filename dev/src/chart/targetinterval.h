//@@license@@

#ifndef TARGETINTERVAL_H
#define TARGETINTERVAL_H

#include "chartitem.h"
#include "../dal/target.h"
#include "curve.h"
#include "item.h"

#include <QCoreApplication>
#include <qwt_plot_marker.h>
#include <QDateTime>
#include <QPainter>
#include <qwt_scale_map.h>

class TargetInterval : public AbstractItem, public QwtPlotMarker
{
    Q_DECLARE_TR_FUNCTIONS(TargetInterval)

public:
    explicit TargetInterval(Target::TargetType type = Target::ResidualTarget);

    void setTarget(const Target* &target);
    void setDataSet(const DataSet &data);

    void setColor(QColor c);

    int rtti() const;
    QRectF boundingRect() const;

protected:
    void draw(QPainter* p, const QwtScaleMap& xMap, const QwtScaleMap& yMap, const QRectF& rect) const;

private:
    virtual void yScaleChangedImpl();

    void drawMean(QPainter *p,  const QwtScaleMap& xMap, const QwtScaleMap& yMap, const QRectF& rect) const;
    void drawPeak(QPainter *p, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &rect) const;
    void drawRes(QPainter *p, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &rect) const;

    QPen createPen(int r, int g, int b, int alpha, int width) const;
    void refreshTarget();

    Target* target;
    Target::TargetType type;
    TargetData points;
    QBrush brush;
    DataSet dataset;

    // Protect the Qwt standart attach/detach mechanism
    void attach(QwtPlot *plot);
    void detach();

    friend class Chart;
};

#endif
