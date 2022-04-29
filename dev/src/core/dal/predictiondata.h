//@@license@@

#ifndef PREDICTIONDATA_H
#define PREDICTIONDATA_H

#include <cmath>

#include <QObject>
#include "core/utils/autoproperty.h"
#include "core/dal/entity.h"
#include "core/dal/drug/parameters.h"

namespace ezechiel {
namespace core {

class Point {
public:
    Point(double _t, double _v) {time = _t; value = _v;}
    Point() {}
    bool operator==(const Point& pt) {
       if (time == pt.time && value == pt.value) {
           return true;
       }
       return false;
    }

    inline void setTime(double _t) {time = _t;}
    inline void setValue(double _v) {value = _v;}

    inline double getTime() {return time;}
    inline double getValue() {return value;}
private:
    double time;
    double value;
} ;

VALUELIST_CLASS_DECL(Points, Point)

class FancyPoint : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(FancyPoint)
    AUTO_PROPERTY_DECL(double, time, Time)
    AUTO_PROPERTY_DECL(double, value, Value)
    AUTO_PROPERTY_DECL(ParameterSet*, pset, Pset)
public:
    explicit FancyPoint(AbstractRepository *repository,QObject *parent = nullptr);

//    bool operator==(const FancyPoint& pt) {
//       if (_time == pt.time && _value == pt.value) {
//           return true;
//       }
//       return false;
//    }
};

QML_POINTERLIST_CLASS_DECL(FancyPoints, FancyPoint)

}
}
Q_DECLARE_METATYPE(ezechiel::core::Point)
Q_DECLARE_METATYPE(ezechiel::core::Points*)

Q_DECLARE_METATYPE(ezechiel::core::FancyPoint*)
Q_DECLARE_METATYPE(ezechiel::core::FancyPoints*)

namespace ezechiel {
namespace core {

struct CycleStatistics
{
    CycleStatistics(double _cycleStart, double _duration, double _mean, double _auc, double _auc24, double _cumulatedAuc, double _trough, double _peak) :
        cycleStart(_cycleStart), duration(_duration), mean(_mean), auc(_auc), auc24(_auc24), cumulatedAuc(_cumulatedAuc), trough(_trough), peak(_peak) {}
    double cycleStart;
    double duration;
    double mean;
    double auc;
    double auc24;
    double cumulatedAuc;
    double trough;
    double peak;
};

///
/// \brief The MeasureValues struct
/// This struct embeds information about a measure and its corresponding predicted concentration
///
struct MeasureValues
{
    /// Time the measure was taken
    QDateTime m_time;

    /// Value of the measure
    double m_measureValue;

    /// Value of the predicted concentration
    double m_predictedValue;
};


typedef QList<int> IntList;

class PredictionData : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(PredictionData)
    AUTO_PROPERTY_DECL(FancyPoints*, points, Points)
    Q_PROPERTY(IntList peaks READ getPeaks NOTIFY peaksChanged)
    Q_PROPERTY(IntList troughs READ getTroughs NOTIFY troughsChanged)
public:
    explicit PredictionData(AbstractRepository *repository,QObject *parent = nullptr);

    void addStats(double _cycleStart, double _duration, double _mean, double _auc, double _cumulatedAuc, double _trough, double _peak)
    {
        m_stats.push_back(CycleStatistics(_cycleStart, _duration, _mean, _auc, _auc / _duration * 24.0, _cumulatedAuc, _trough, _peak));
    }

    Q_INVOKABLE double meanAt(const int index) {
        const CycleStatistics *pStats = getStats(index);
        if (nullptr != pStats) {
            return pStats->mean;
        }
        return 0.0;
    }

    Q_INVOKABLE double aucAt(const int index) {
        const CycleStatistics *pStats = getStats(index);
        if (nullptr != pStats) {
            return pStats->auc;
        }
        return 0.0;
    }

    Q_INVOKABLE double auc24At(const int index) {
        const CycleStatistics *pStats = getStats(index);
        if (nullptr != pStats) {
            return pStats->auc / pStats->duration * 24.0;;
        }
        return 0.0;
    }

    Q_INVOKABLE double cumulatedAucAt(const int index) {
        const CycleStatistics *pStats = getStats(index);
        if (nullptr != pStats) {
            return pStats->cumulatedAuc;
        }
        return 0.0;
    }

    Q_INVOKABLE double troughAt(const int index) {
        const CycleStatistics *pStats = getStats(index);
        if (nullptr != pStats) {
            return pStats->trough;
        }
        return 0.0;
    }

    Q_INVOKABLE double peakAt(const int index) {
        const CycleStatistics *pStats = getStats(index);
        if (nullptr != pStats) {
            return pStats->peak;
        }
        return 0.0;
    }

    Q_INVOKABLE double timeAfterDose(const int index) {
        const CycleStatistics *pStats = getStats(index);
        if (nullptr != pStats) {
            double hours = (timeAt(index) - pStats->cycleStart) / 3600.0;
            hours = (hours*10)/10;
            return floor(hours*2+0.5)/2;
        }
        return 0.0;
    }

    Q_INVOKABLE double cycleDuration(const int index) {
        const CycleStatistics *pStats = getStats(index);
        if (nullptr != pStats) {
            return floor(pStats->duration*2+0.5)/2;
        }
        return 0.0;
    }

    Q_INVOKABLE IntList getTroughs() { return _troughs; }
    Q_INVOKABLE IntList getPeaks() { return _peaks; }

    Q_INVOKABLE double timeAt(const int index) {
        if (index > 0 && index < _points->size()) {
            return _points->at(index)->getTime();
        }
        return 0.0;
    }

    Q_INVOKABLE double valueAt(const int index) {
        return _points->at(index)->getValue();
    }

    Q_INVOKABLE int nbPoints() const {
        return _points->size();
    }

    Q_INVOKABLE QList<double> times () {
        QList<double> _t;
        for (int i = 0; i < _points->size(); ++i) {
            _t.append(_points->at(i)->getTime());
        }
        return _t;
    }

    Q_INVOKABLE QList<double> values () {
        QList<double> _v;
        for (int i = 0; i < _points->size(); ++i) {
            _v.append(_points->at(i)->getValue());
        }
        return _v;
    }

    bool isValid() {
        return !_points->isEmpty();
    }

    void addMeasureValue(QDateTime _time, double _measureValue, double _predictedValue)
    {
        m_measureValues.push_back({_time, _measureValue, _predictedValue});
    }

    const std::vector<MeasureValues> getMeasureValues() const { return m_measureValues;}

    int getNbCycles() const { return m_stats.size();}

    const CycleStatistics& getCycleStatistics(int index) const { return m_stats[index];}

private:
    const CycleStatistics* getStats(const int index) {
        std::vector<CycleStatistics>::iterator it = m_stats.begin();
        std::vector<CycleStatistics>::iterator prev = m_stats.end();
        double time = timeAt(index);
        if (time > 0) {
            while (it != m_stats.end()) {
                if (it->cycleStart > time && prev != m_stats.end()) {
                    return &(*prev);
                }
                prev = it;
                it++;
            }
        }
        // Check if we ask for the last cycle
        if (m_stats.size() > 0) {
            if (m_stats.at(m_stats.size() - 1).cycleStart < time) {
                return &m_stats.at(m_stats.size() - 1);
            }
        }
        return nullptr;
    }

signals:
    void peaksChanged(QList<int>);
    void troughsChanged(QList<int>);

public slots:
    void findLoci(FancyPoints*);
    void findLociAtPoint(FancyPoint&);

private:
    IntList _troughs;
    IntList _peaks;
    std::vector<CycleStatistics> m_stats;

    std::vector<MeasureValues> m_measureValues;
};

}
}
Q_DECLARE_METATYPE(ezechiel::core::PredictionData*)
Q_DECLARE_METATYPE(QList<ezechiel::core::PredictionData*>)
#endif // PREDICTIONDATA_H
