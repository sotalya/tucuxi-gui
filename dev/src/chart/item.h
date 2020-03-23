#ifndef ITEM_H
#define ITEM_H

#include "chart.h"
#include "dataset.h"

#include <qwt_series_data.h>
#include <qwt_point_data.h>

class AbstractItem : public QObject
{
    Q_OBJECT

public:
    AbstractItem(QObject *parent = 0);
    virtual ~AbstractItem() = 0;

    void setXScaleType(Chart::ScaleType type);
    Chart::ScaleType xScaleType() const;

    void setYScaleType(Chart::ScaleType type);
    Chart::ScaleType yScaleType() const;

public slots:
    void xScaleChanged(Chart::ScaleType type);
    void yScaleChanged(Chart::ScaleType type);

private:
    virtual void xScaleChangedImpl();
    virtual void yScaleChangedImpl();

    Chart::ScaleType _xScaleType;
    Chart::ScaleType _yScaleType;
};

class AbstractItemData
{
public:
    virtual ~AbstractItemData() = 0;
    bool isEmpty() const;

protected:
    static const int LOG10_FILTER;

private:
    virtual bool isEmptyImpl() const = 0;
};

class AbstractItemPointData: public AbstractItemData
{
public:
    virtual ~AbstractItemPointData() = 0;

protected:
    virtual double convertDataToXScale(double data, Chart::ScaleType type);
    virtual double convertDataToYScale(double data, Chart::ScaleType type);
};

class PointData: public AbstractItemPointData
{
public:
    void setRawData(double data, double time);
    QPointF data(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType);

private:
    virtual bool isEmptyImpl() const;

    QMap<Chart::ScaleType, double> _convertedData;
    QMap<Chart::ScaleType, double> _convertedTime;
};

class TargetData: public AbstractItemPointData
{
public:
    void setRawData(double min, double max, double best);
    double minData(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType);
    double maxData(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType);
    double bestData(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType);

private:
    virtual bool isEmptyImpl() const;

    QMap<Chart::ScaleType, double> _convertedMin;
    QMap<Chart::ScaleType, double> _convertedMax;
    QMap<Chart::ScaleType, double> _convertedBest;
};

class AbstractItemArrayData: public AbstractItemData
{
public:
    virtual ~AbstractItemArrayData() = 0;

protected:
    virtual QVector<double> convertDataToXScale(const QVector<double> &data, Chart::ScaleType type);
    virtual QVector<double> convertDataToYScale(const QVector<double> &data, Chart::ScaleType type);
};

class CurvePointArrayData : public AbstractItemArrayData
{
public:
    void setRawData(const QVector<double> &data, const QVector<double> &time);
    QwtPointArrayData *data(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType);

protected:
    virtual QVector<double> convertDataToYScale(const QVector<double> &data, Chart::ScaleType type);

private:
    virtual bool isEmptyImpl() const;

    QMap<Chart::ScaleType, QVector<double> > _convertedData;
    QMap<Chart::ScaleType, QVector<double> > _convertedTime;
};

class PercentilesPointArrayData: public AbstractItemArrayData
{
public:
    void setRawData(const QVector<double> &top, const QVector<double> &bottom, const QVector<double> &time);
    QVector<QwtIntervalSample> data(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType);

private:
    virtual bool isEmptyImpl() const;
    void convertDataToYScale(QVector<double> &top, QVector<double> &bottom, Chart::ScaleType type);
    QVector<QwtIntervalSample> toIntervalSample(const QVector<double> &time, const QVector<double> &top, const QVector<double> &bottom) const;

    QMap<Chart::ScaleType, QVector<double> > _convertedTop;
    QMap<Chart::ScaleType, QVector<double> > _convertedBottom;
    QMap<Chart::ScaleType, QVector<double> > _convertedTime;
};

#endif // ITEM_H
