//@@license@@

#ifndef PROCESSINGTRAITS
#define PROCESSINGTRAITS


#include <QDateTime>
#include <QMap>
#include <QVariant>
#include "core/dal/drug/parameters.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class ParamTraits {
public:
    ParamTraits() {}
    ParamTraits(const QDateTime& adjDate) : _adjDate(adjDate) {}
    virtual ~ParamTraits() {}
    virtual ParameterType ptype() const = 0;
    virtual bool getAdjustmentDate(QDateTime &adjDate) { adjDate = _adjDate; return _adjDate.isValid(); }
    ParameterSet *pset;

private:
    QDateTime _adjDate;
};

class PopulationTraits : public ParamTraits
{
public:
    ParameterType ptype() const { return POPULATION;}
};

class AprioriTraits : public ParamTraits
{
public:
    AprioriTraits() {}
    AprioriTraits(const QDateTime& adjDate) : ParamTraits(adjDate) {}
    ParameterType ptype() const { return APRIORI;}
};

class AposterioriTraits : public ParamTraits
{
public:
    AposterioriTraits() {}
    AposterioriTraits(const QDateTime& adjDate) : ParamTraits(adjDate) {}
    ParameterType ptype() const { return APOSTERIORI;}
};

class CalcTraits {
public:
    CalcTraits() : traits(0) {}
    CalcTraits(ParamTraits* ptraits,
               const QDateTime& _start,
               const QDateTime& _end,
               const int _nbPoints) : traits(ptraits), nbPoints(_nbPoints), start(_start), end(_end){}

    virtual bool check() {
        if (traits == 0)
            return false;
        return true;
    }

    ParamTraits *traits;
    int nbPoints;
    QDateTime start;
    QDateTime end;
    QMap<QString,QVariant> options;
};

class PercentilesTraits : public CalcTraits
{
public:
    PercentilesTraits(){}

    PercentilesTraits(const int _nbPoints,
                      const std::vector<double> _percs,
                      const QDateTime _start,
                      const QDateTime _end,
                      ParamTraits* ptraits)
        : CalcTraits(ptraits, _start, _end, _nbPoints), percs(_percs)
    {}
    const std::vector<double> percs;
    bool clearCache{true};
    int cacheId{-1};
};

class PointsTraits : public CalcTraits
{
public:
    PointsTraits(){}

    PointsTraits(const QDateTime& _start,
                 const QDateTime& _end,
                 const int _nbPoints,
                 ParamTraits* ptraits,
                 const std::vector<double> _initialResiduals = std::vector<double>(0))
        : CalcTraits(ptraits, _start, _end, _nbPoints), initialResiduals(_initialResiduals)
    {}

    const std::vector<double> initialResiduals;
};

class PointTraits : public CalcTraits
{
public:
    PointTraits(){}

    PointTraits(const QDateTime& _start,
                const QDateTime& _end,
                const QDateTime& _time,
                ParamTraits* ptraits,
                 const std::vector<double> _initialResiduals = std::vector<double>(0))
        : CalcTraits(ptraits, _start, _end, 1), time(_time), initialResiduals(_initialResiduals)
    {}

    QDateTime time;
    const std::vector<double> initialResiduals;
};

class PeakTraits : public CalcTraits
{
public:
    PeakTraits(){}

    PeakTraits(const QDateTime& _start,
               const QDateTime& _end,
               const QDateTime& _time,
               ParamTraits* ptraits,
               const std::vector<double> _initialResiduals = std::vector<double>(0))
      : CalcTraits(ptraits, _start, _end, 1), time(_time), initialResiduals(_initialResiduals)
    {}
    QDateTime time;
    const std::vector<double> initialResiduals;
};

class ReverseTraits : public CalcTraits
{
public:
    ReverseTraits() : adjWithLoadingDose(false), adjWithRestPeriod(false){}

    ReverseTraits(
            bool _adjWithLoadingDose,
            bool _adjWithRestPeriod,
            const QDateTime& _start,
            const QDateTime& _end,
            const QDateTime& _time,
            const QDateTime& _endOfTreatment,
            const int _nbPoints,
            ParamTraits* ptraits,
            const std::vector<double> _initialResiduals = std::vector<double>(0))
        : CalcTraits(ptraits, _start, _end, _nbPoints), time(_time),
          adjWithLoadingDose(_adjWithLoadingDose), adjWithRestPeriod(_adjWithRestPeriod),
          initialResiduals(_initialResiduals), endOfTreatment(_endOfTreatment)
    {}

    const QDateTime time;
    const bool adjWithLoadingDose;
    const bool adjWithRestPeriod;
    const std::vector<double> initialResiduals;
    const QDateTime endOfTreatment;
};
}
}
}

#endif // PROCESSINGTRAITS

