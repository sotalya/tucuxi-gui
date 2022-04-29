//@@license@@

#ifndef TAYLOR_H
#define TAYLOR_H

#include "ezutils.h"
#include "core.h"
#include "dataset.h"
#include "percentileengine.h"
#include "percentileenginefactory.h"
#include "drugresponseanalysis.h"
#include "drugtreatment.h"
#include "duration.h"
#include "../dal/parameters.h"
#include "units.h"
#include "version.h"

#include <QCoreApplication>
#include <QList>
#include <QMap>
#include <QString>

//from boost
template<typename value_type, typename function_type>
value_type derivative(const value_type x, const value_type dx, function_type func)
{
   // Compute d/dx[func(*first)] using a three-point
   // central difference rule of O(dx^6).

   const value_type dx1 = dx;
   const value_type dx2 = dx1 * 2;
   const value_type dx3 = dx1 * 3;

   const value_type m1 = (func(x + dx1) - func(x - dx1)) / 2;
   const value_type m2 = (func(x + dx2) - func(x - dx2)) / 4;
   const value_type m3 = (func(x + dx3) - func(x - dx3)) / 6;

   const value_type fifteen_m1 = 15 * m1;
   const value_type six_m2     =  6 * m2;
   const value_type ten_dx1    = 10 * dx1;

   return ((fifteen_m1 - six_m2) + m3) / ten_dx1;
}

/** \brief The Taylor prediction engine factory
 * \ingroup factories
 * Simple factory.
 */
class TaylorFactory : public PercentileEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "taylor")
    Q_INTERFACES(PercentileEngineFactory)
public:
    void init(Core *core);

    PercentileEngine* newPercentileEngine ();

    Descriptor descriptor ();

    int priority(const QString &id) const;    

private:
    //Our ID is : ch.heig-vd.ezechiel.taylor
    Descriptor _d;

    //Map of <Engine ID, Engine priority>
    QMap<QString, int> _priorities;
};

//! \brief The Taylor percentile engine
/*! \ingroup peengines
   The Taylor approach is used to find the global variability and so provide the percentiles.
  */
class Taylor : public PercentileEngine
{
    Q_DECLARE_TR_FUNCTIONS(Taylor)
public:

    Taylor(const Descriptor &d);

    PercentileEngine* clone ();

    Descriptor descriptor ();

    int priority() const;

    Version version ();

    QString errorString ();

    void setAnalysis (SharedDrugResponseAnalysis model);

    SharedDrugResponseAnalysis analysis();

    virtual QMap<int,DataSet> percentiles (const QList<int> &values,
                                           const Duration &start,
                                           const Duration &stop,
                                           uint points,
                                           const QList<ParameterSet*> &parameters,
                                           bool uniform);
    DataSet taylorize(DataSet&, QMap<QString, double>&, const ParameterSet*);
    DataSet percentile(const int, DataSet&, DataSet&);
private:
    //The engine priority level
    static const int _priority;

    //Our ID is : ch.heig-vd.ezechiel.taylor
    Descriptor _d;

    //Contains the errors, if any
    QString _errors;

    //Our curve model (gnierk)
    Prediction* m_pred;
};

class MeanVarianceMarginalDistributionAtPoint
{
public:
    MeanVarianceMarginalDistributionAtPoint(Prediction*, const QList<ParameterSet*>&, const SharedMeasure, const Duration&);
    MeanVarianceMarginalDistributionAtPoint(Prediction*, ParameterSet*, const SharedMeasure);
    MeanVarianceMarginalDistributionAtPoint(Prediction*, ParameterSet*);
    void reinitialize();
    void computeMeanAndVariance(const QMap<QString, double>&);
    double getVariance() { return m_variance; }
    double getMean() { return m_mean; }
    void setMean(double _mean) { m_mean = _mean; }
    void setTimept(int _timept) { m_measuretime = Duration(0,0,0,_timept); }
    double calculateLogLikelihood(const QMap<QString, double>&);
    double getLogLikelihood();

private:
    Prediction* m_pred;
    double m_mean;
    double m_variance;
    double m_loglikelihood;
    SharedMeasure m_measure;
    Duration m_measuretime;
    ParameterSet* m_params_ind;
    ParameterSet* m_local_params_ind;
};

#endif // TAYLOR_H
