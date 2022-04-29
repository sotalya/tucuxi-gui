//@@license@@

#ifndef CURVE_H
#define CURVE_H

#include "duration.h"
#include "dataset.h"
#include "chartitem.h"
#include "../dal/parameters.h"
#include "item.h"

#include <QObject>
#include <qwt_plot_curve.h>
#include <QDateTime>

class Curve : public AbstractItem, public QwtPlotCurve
{
    Q_OBJECT

public:
    explicit Curve(ParameterType type = APOSTERIORI);

    void setDataset(const QDateTime& start, const QDateTime& stop, const DataSet& dataset);
    void setPoints(const QDateTime& start, const Duration& duration, double *points, int nbPoints);

    int rtti() const;
    ParameterType getType() const;
    void setType(ParameterType type);

    bool isNear(const QPoint& p);
    QPointF getXProjPoint(const QPointF& point);  

    QColor getColor() const;
    void setColor(QColor c);

public slots:
    void highlight(bool b);

signals:
    void clicked();
    void doubleClicked();

private slots:
    void onClick();
    void onDoubleClick();

private:
    virtual void yScaleChangedImpl();

    // Protect the Qwt standart attach/detach mechanism
    void attach(QwtPlot *plot);
    void detach();

    CurvePointArrayData points;
    ParameterType type;
    bool highlighted;

    friend class Chart;
};

#endif // CURVE_H
