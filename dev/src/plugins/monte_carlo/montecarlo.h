//@@license@@

#ifndef MONTECARLO_H
#define MONTECARLO_H

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
#include "prediction.h"

#include <QCoreApplication>
#include <QList>
#include <QMap>
#include <QString>

#include <boost/numeric/ublas/matrix.hpp>

/** \brief The MonteCarlo prediction engine factory
 * \ingroup factories
 * Simple factory.
 */
class MonteCarloFactory : public PercentileEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "monte_carlo")
    Q_INTERFACES(PercentileEngineFactory)
public:
    void init(Core *core);

    PercentileEngine* newPercentileEngine ();

    Descriptor descriptor ();

    int priority(const QString &id) const;

private:
    //Our ID is : ch.heig-vd.ezechiel.monte_carlo
    Descriptor _d;

    //Map of <Engine ID, Engine priority>
    QMap<QString, int> _priorities;
};

/** \brief The MonteCarlo prediction engine
 * \ingroup peengines
 * TODO Yann, please?
 */
class MonteCarlo : public PercentileEngine
{
//    Q_DECLARE_TR_FUNCTIONS(MonteCarlo)
//    Q_OBJECT

    typedef boost::numeric::ublas::row_major ORI;
    typedef double DBL;

public:

    MonteCarlo(const Descriptor &d);

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

    boost::numeric::ublas::matrix<DBL, ORI> createCorrMatrix(const QList<ParameterSet*> &parameters);

private:
    //The engine priority level
    static const int _priority;

    //Our ID is : ch.heig-vd.ezechiel.monte_carlo
    Descriptor _d;

    //Contains the errors, if any
    QString _errors;

    //Our curve model (gnierk)
    SharedDrugResponseAnalysis _model;

    //Ideal number of threads
    int idealThreadsNb;
};

#endif // MONTECARLO_H
