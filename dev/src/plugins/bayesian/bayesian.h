#ifndef BAYESIAN_H
#define BAYESIAN_H

#include <QCoreApplication>
#include <QList>
#include <QMap>
#include <QString>

#include "ezutils.h"
#include "core.h"
#include "dataset.h"
#include "postengine.h"
#include "postenginefactory.h"
#include "drugresponseanalysis.h"
#include "drugtreatment.h"
#include "duration.h"
#include "../dal/parameters.h"
#include "units.h"
#include "version.h"

#include <QTextStream>

// REMEMBER to update "lu.hpp" header includes from boost-CVS
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

// Options used by this module
#define EPSILON            "1epsilon"
#define DELTA_EPSILON      "1epsilon_d"
#define PRECISION          "1precision"
#define IGNOREPARAMETERS   "2ignorepar"
#define IGNORERESIDUAL     "3ignoreres"
#define CALCULIMP          "4calcimp"
#define IGNORECORRELATIONS "5ignorecorr"
#define EXPORTSTATS        "6exportstats"
#define STATSFILE_OPTION   "7exportdir"
#define RESIDERRORIMP      "8residerrorimp"

//fix this!
//#define private public

typedef boost::numeric::ublas::row_major ORI;
typedef double DBL;

/** \brief The Bayesian post engine factory
 * \ingroup factories
 * Simple factory.
 */
class BayesianFactory : public PostEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "bayesian")
    Q_INTERFACES(PostEngineFactory)
public:
    void init(Core *core);

    PostEngine* newPostEngine ();

    Descriptor descriptor ();

    int priority(const QString &id) const;

private:
    //Our ID is : ch.heig-vd.ezechiel.bayesian
    Descriptor _d;

    //Map of <Engine ID, Engine priority>
    QMap<QString, int> _priorities;
};

/** \brief The Bayesian post engine
 * \ingroup poengines
 * Approximates prediction parameters for a specific patient.
 */
class Bayesian : public PostEngine
{
    Q_DECLARE_TR_FUNCTIONS(Bayesian)
public:

    Bayesian(const Descriptor &d);

    PostEngine* clone ();

    Descriptor descriptor ();

    int priority() const;

    Version version ();

    QString errorString ();

    void setPrediction (Prediction* model);

    ParameterSet* aposteriori (ParameterSet* const &input,
                              QList<SharedMeasure> const &measures,
                              Duration const &intervalStart,
                              Duration const &intervalStop);




    virtual bool calculateEta (QList<ParameterSet*> const &apriori_params,
                               SharedMeasureSet const &measures);

    virtual ParameterSet* aPosterioriParameters (ParameterSet* const &aprioriParams);

    virtual void aPosterioriParameters (QList<ParameterSet*> const &aprioriParams,
                                        QList<ParameterSet*> &aposterioriParams);

private:
    //The engine priority level
    static const int _priority;

    ParameterSet* aposteriori_yann (ParameterSet* const &input,
                              QList<SharedMeasure> const &measures,
                              Duration const &intervalStart,
                              Duration const &intervalStop);

    ParameterSet* aposteriori_lionel (ParameterSet* const &input,
                              QList<SharedMeasure> const &measures,
                              Duration const &intervalStart,
                              Duration const &intervalStop);

    /** Do some stuff, done by Lionel.
     * TODO This one deserve a little bit of documentation
     */
    double phi(const QList<SharedMeasure> &measures, Duration *measureTimes, ParameterSet* params_ind, ParameterSet* params_pre);



    double phiWithCorrelation(const SharedMeasureSet measures,
                                     Duration *measureTimes,
                                     const QList<ParameterSet*> &params_ind);

    void calculateGradient(const SharedMeasureSet measures,
                                     Duration *measureTimes,
                                     const QList<ParameterSet*> &params_pre,
                                     double phi0,
                                     double eps,
                                     double *deltas);

    void updateEta(double deltas[], bool positive);

    void moveVariableParameters(ParameterSet* &parameters,double deltas[],bool positive);

    void generatePhiExplorer(const QString& fileName);

    void exportPhiSpace(const SharedMeasureSet measures,
                        Duration *measureTimes,
                        const QList<ParameterSet*> &params_pre,
                        const double pmin[], const double pmax[]);

    void printInfos();

    bool logPhiCalls;

    // Just not to instantiate and calculate multiple times per iteration
    Duration duration;
    //Our ID is : ch.heig-vd.ezechiel.bayesian
    Descriptor _d;
    //Contains the errors, if any
    QString _errors;
    //Our curve model (gnierk)
    Prediction* m_pred;
    // Shall it ignore the parameters variability during the bayesian process
    bool ignoreParameters;
    // Shall it ignore the residual error during the bayesian process
    bool ignoreResidual;
    // Shall it ignore the correlations between the parameters
    bool ignoreCorrelations;
    // Shall it export statistics about the computation
    bool exportStats;
    // Name of the directory into which exported files should be put
    DirName exportStatsDir;
    // Type of calculus for the residual error
    int residualErrorType;

    QTextStream outPhiParamTravel;
    QTextStream outPhiObsTravel;

    boost::numeric::ublas::matrix<DBL, ORI> InvCovMatrix;

    boost::numeric::ublas::matrix<DBL, ORI> cov_matrix;

    boost::numeric::ublas::vector<DBL> mu;

    int covMatrixSize;

    double *eta;
    int nbVariableParameters;

    QStringList keysOfVariableParameters;
};

#endif // BAYESIAN_H
