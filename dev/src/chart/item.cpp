//@@license@@

#include "item.h"

#include <algorithm>

// ////////////////////////////////////////////////////////////////////////////////////////////////
// AbstractItem
// ////////////////////////////////////////////////////////////////////////////////////////////////

AbstractItem::AbstractItem(QObject *parent) : QObject(parent), _xScaleType(Chart::Linear), _yScaleType(Chart::Linear)
{

}

AbstractItem::~AbstractItem()
{

}

void AbstractItem::setXScaleType(Chart::ScaleType type)
{
    if (type == xScaleType())
        return;

    _xScaleType = type;
    xScaleChangedImpl();
}

Chart::ScaleType AbstractItem::xScaleType() const
{
    return _xScaleType;
}

void AbstractItem::setYScaleType(Chart::ScaleType type)
{
    if (type == yScaleType())
        return;

    _yScaleType = type;
    yScaleChangedImpl();
}

Chart::ScaleType AbstractItem::yScaleType() const
{
    return _yScaleType;
}

void AbstractItem::xScaleChanged(Chart::ScaleType type)
{
    setXScaleType(type);
}

void AbstractItem::yScaleChanged(Chart::ScaleType type)
{
    setYScaleType(type);
}

void AbstractItem::xScaleChangedImpl()
{

}

void AbstractItem::yScaleChangedImpl()
{

}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// AbstractItemData
// ////////////////////////////////////////////////////////////////////////////////////////////////

const int AbstractItemData::LOG10_FILTER = 100;

AbstractItemData::~AbstractItemData()
{

}

bool AbstractItemData::isEmpty() const
{
    return isEmptyImpl();
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// AbstractItemPointData
// ////////////////////////////////////////////////////////////////////////////////////////////////

AbstractItemPointData::~AbstractItemPointData()
{

}

double AbstractItemPointData::convertDataToXScale(double data, Chart::ScaleType type)
{
    Q_UNUSED(type);
    return data;
}

double AbstractItemPointData::convertDataToYScale(double data, Chart::ScaleType type)
{
    double result = data;

    //Convert to base 10 log scale
    if (type == Chart::Log10)
        if (result <= 0.0)
            result = 1.0 / LOG10_FILTER;

    return result;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// PointData
// ////////////////////////////////////////////////////////////////////////////////////////////////

void PointData::setRawData(double data, double time)
{
    _convertedData.clear();
    _convertedTime.clear();
    _convertedData.insert(Chart::Linear, data);
    _convertedTime.insert(Chart::Linear, time);
}

QPointF PointData::data(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType)
{
    if (!_convertedTime.contains(xScaleType))
        _convertedTime.insert(xScaleType, convertDataToXScale(_convertedTime.value(Chart::Linear), xScaleType));
    if (!_convertedData.contains(yScaleType))
        _convertedData.insert(yScaleType, convertDataToYScale(_convertedData.value(Chart::Linear), yScaleType));

    return QPointF(_convertedTime.value(xScaleType), _convertedData.value(yScaleType));
}

bool PointData::isEmptyImpl() const
{
    return _convertedData.isEmpty() || _convertedTime.isEmpty();
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// TargetData
// ////////////////////////////////////////////////////////////////////////////////////////////////

void TargetData::setRawData(double min, double max, double best)
{
    _convertedMin.clear();
    _convertedMin.insert(Chart::Linear, min);

    _convertedMax.clear();
    _convertedMax.insert(Chart::Linear, max);

    _convertedBest.clear();
    _convertedBest.insert(Chart::Linear, best);
}

double TargetData::minData(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType)
{
    Q_UNUSED(xScaleType);

    if (!_convertedMin.contains(yScaleType))
        _convertedMin.insert(yScaleType, convertDataToYScale(_convertedMin.value(Chart::Linear), yScaleType));

    return _convertedMin.value(yScaleType);
}

double TargetData::maxData(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType)
{
    Q_UNUSED(xScaleType);

    if (!_convertedMax.contains(yScaleType))
        _convertedMax.insert(yScaleType, convertDataToYScale(_convertedMax.value(Chart::Linear), yScaleType));

    return _convertedMax.value(yScaleType);
}

double TargetData::bestData(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType)
{
    Q_UNUSED(xScaleType);

    if (!_convertedBest.contains(yScaleType))
        _convertedBest.insert(yScaleType, convertDataToYScale(_convertedBest.value(Chart::Linear), yScaleType));

    return _convertedBest.value(yScaleType);
}

bool TargetData::isEmptyImpl() const
{
    return _convertedMin.isEmpty() || _convertedMax.isEmpty() || _convertedBest.isEmpty();
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// AbstractItemArrayData
// ////////////////////////////////////////////////////////////////////////////////////////////////

AbstractItemArrayData::~AbstractItemArrayData()
{

}

QVector<double> AbstractItemArrayData::convertDataToXScale(const QVector<double> &data, Chart::ScaleType type)
{
    Q_UNUSED(type);
    return data;
}

QVector<double> AbstractItemArrayData::convertDataToYScale(const QVector<double> &data, Chart::ScaleType type)
{
    Q_UNUSED(type);
    return data;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// CurvePointArrayData
// ////////////////////////////////////////////////////////////////////////////////////////////////

void CurvePointArrayData::setRawData(const QVector<double> &data, const QVector<double> &time)
{
    _convertedData.clear();
    _convertedTime.clear();
    _convertedData.insert(Chart::Linear, data);
    _convertedTime.insert(Chart::Linear, time);
}

QwtPointArrayData *CurvePointArrayData::data(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType)
{
    if (!_convertedTime.contains(xScaleType))
        _convertedTime.insert(xScaleType, convertDataToXScale(_convertedTime.value(Chart::Linear), xScaleType));
    if (!_convertedData.contains(yScaleType))
        _convertedData.insert(yScaleType, convertDataToYScale(_convertedData.value(Chart::Linear), yScaleType));

    return new QwtPointArrayData(_convertedTime.value(xScaleType), _convertedData.value(yScaleType));
}

QVector<double> CurvePointArrayData::convertDataToYScale(const QVector<double> &data, Chart::ScaleType type)
{
    QVector<double> result = data;

    //Convert to base 10 log scale
    if (type == Chart::Log10) {
        QVector<double> sorted = data;

        //Sort the current data
        std::sort(sorted.begin(), sorted.end());

        //Search the lower bound
        if (sorted.first() <= 0.0) {

            //Get the index of the first positive value
            int index = sorted.lastIndexOf(0.0);

            if (index == -1 || ++index == sorted.size())
                return result;

            //Determine the lower bound
            double lowerBound = sorted.at(index) / LOG10_FILTER;

            //Trunkate the values
            for (int i = 0; i < result.size(); ++i)
                if (result[i] < lowerBound)
                    result[i] = lowerBound;
        }
    }

    return result;
}

bool CurvePointArrayData::isEmptyImpl() const
{
    return _convertedData.isEmpty() || _convertedTime.isEmpty();
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// PercentilesPointArrayData
// ////////////////////////////////////////////////////////////////////////////////////////////////

void PercentilesPointArrayData::setRawData(const QVector<double> &top, const QVector<double> &bottom, const QVector<double> &time)
{
    _convertedTop.clear();
    _convertedTop.insert(Chart::Linear, top);

    _convertedBottom.clear();
    _convertedBottom.insert(Chart::Linear, bottom);

    _convertedTime.clear();
    _convertedTime.insert(Chart::Linear, time);
}

QVector<QwtIntervalSample> PercentilesPointArrayData::data(Chart::ScaleType xScaleType, Chart::ScaleType yScaleType)
{
    //Convert the time
    if (!_convertedTime.contains(xScaleType))
        _convertedTime.insert(xScaleType, convertDataToXScale(_convertedTime.value(Chart::Linear), xScaleType));

    //Convert the data
    if (!_convertedTop.contains(yScaleType) || !_convertedBottom.contains(yScaleType)) {
        _convertedTop.insert(yScaleType, _convertedTop.value(Chart::Linear));
        _convertedBottom.insert(yScaleType, _convertedBottom.value(Chart::Linear));
        convertDataToYScale(_convertedTop[yScaleType], _convertedBottom[yScaleType], yScaleType);
    }

    return toIntervalSample(_convertedTime.value(xScaleType), _convertedTop.value(yScaleType), _convertedBottom.value(yScaleType));
}

bool PercentilesPointArrayData::isEmptyImpl() const
{
    return _convertedTop.isEmpty() || _convertedBottom.isEmpty() || _convertedTime.isEmpty();
}

void PercentilesPointArrayData::convertDataToYScale(QVector<double> &top, QVector<double> &bottom, Chart::ScaleType type)
{
    Q_ASSERT(top.size() == bottom.size());

    //Convert to base 10 log scale
    if (type == Chart::Log10) {
        QVector<double> sorted = bottom;

        //Determine the lower bound
        std::sort(sorted.begin(), sorted.end());

        //Search the lower bound
        if (sorted.first() <= 0.0) {

            //Get the index of the first positive value
            int index = sorted.lastIndexOf(0.0);

            if (index == -1 || ++index == sorted.size())
                return;

            //Determine the lower bound
            double lowerBound = sorted.at(index) / LOG10_FILTER;

            //Trunkate the values
            for (int i = 0; i < top.size(); ++i) {
                if (top[i] < lowerBound)
                    top[i] = lowerBound;
                if (bottom[i] < lowerBound)
                    bottom[i] = lowerBound;
            }
        }
    }
}

QVector<QwtIntervalSample> PercentilesPointArrayData::toIntervalSample(const QVector<double> &time, const QVector<double> &top, const QVector<double> &bottom) const
{
    Q_ASSERT(top.size() == bottom.size());
    Q_ASSERT(bottom.size() == time.size());

    //Convert the vectors
    QVector<QwtIntervalSample> result;

    for (int i = 0; i < top.size(); ++i)
        result.append(QwtIntervalSample(time.at(i), bottom.at(i), top.at(i)));

    //Return the intervals
    return result;
}
