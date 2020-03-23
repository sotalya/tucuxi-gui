#ifndef CURVEGENERATORTHREAD_H
#define CURVEGENERATORTHREAD_H

#include <QThread>
#include <QSemaphore>

#include "drugresponseanalysis.h"
#include "drugtreatment.h"
#include "dataset.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/random/mersenne_twister.hpp>

typedef boost::numeric::ublas::row_major ORI;
typedef double DBL;

class CurveGeneratorThread : public QThread
{
public:    
    explicit CurveGeneratorThread(QObject *parent = 0);
    ~CurveGeneratorThread();

    //! Calculate the percentile for these peoples
    void generateCurves(QList<ParameterSet*>, DataSet**, SharedDrugResponseAnalysis, int , uint , int, int, const Duration&, const Duration&, boost::numeric::ublas::matrix<DBL, ORI>&);
    boost::numeric::ublas::vector<DBL> generateCorrelatedRandomValues();
    void applyResidualVariance(DataSet&);
    void modifyParameters(Prediction*, boost::numeric::ublas::vector<DBL>&);
    //! Wait for the thread to finish its work
    void waitForEnd();

    //! Terminates the thread if the thread is waiting for a new processing
    void askForTermination();

protected:

    virtual void run();

    QSemaphore semStart;
    QSemaphore semEnd;

    DataSet **m_people;
    int nbPeople;
    int m_plb;
    int m_pub;
    double m_additive;
    double m_proportional;
    Duration m_start;
    Duration m_stop;
    uint m_points;
    DataSet *result;
    bool shouldTerminate;
    SharedDrugResponseAnalysis m_lcmod;
    QList<ParameterSet*> m_local_params;
    boost::numeric::ublas::matrix<DBL, ORI> m_lower_cholesky_matrix;
    boost::mt19937* m_randgen;
};

#endif // CURVEGENERATORTHREAD_H
