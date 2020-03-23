#include "curvegeneratorthread.h"
#include "prediction.h"

#include <math.h>

#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/lagged_fibonacci.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/math/distributions/normal.hpp>

#include "../dal/parameters.h"

#include "cholesky.hpp"

typedef boost::numeric::ublas::row_major ORI;
typedef double DBL;

CurveGeneratorThread::CurveGeneratorThread(QObject *parent): QThread(parent)
    ,m_people(0),
    nbPeople(0),
    m_lcmod(0)
{
}

CurveGeneratorThread::~CurveGeneratorThread()
{
    if (0 != this->m_lcmod) {
        this->m_lcmod->deleteLater();
    }
    if (0 != this->m_randgen) {
        delete(this->m_randgen);
    }
}

void CurveGeneratorThread::generateCurves(QList<ParameterSet*> _local_params, DataSet **people, SharedDrugResponseAnalysis _cmod, int nbPeople, uint points, int people_lbound, int people_ubound, const Duration& start, const Duration& stop, boost::numeric::ublas::matrix<DBL, ORI>& lower_cholesky_matrix)
{
    m_lcmod = _cmod;
    this->m_people = &*people;
    this->nbPeople = nbPeople;
    this->m_plb = people_lbound;
    this->m_pub = people_ubound;
    this->m_start = Duration(start);
    this->m_stop = Duration(stop);
    this->shouldTerminate = false;
    for (QList<ParameterSet*>::const_iterator it = _local_params.constBegin(); it != _local_params.constEnd(); ++it) {
        this->m_local_params.append(ParameterSet*(new ParameterSet(*it)));
    }
    this->m_points = points;
    this->m_lower_cholesky_matrix = lower_cholesky_matrix;
    this->m_additive = 0;
    this->m_proportional = 0;
    this->m_randgen = new boost::mt19937(static_cast<unsigned> (std::time(0)));

    semStart.release();
}

void CurveGeneratorThread::waitForEnd() {
    semEnd.acquire();
}

void CurveGeneratorThread::askForTermination() {
    shouldTerminate=true;
    semStart.release();
}

ublas::vector<DBL> CurveGeneratorThread::generateCorrelatedRandomValues() {


    // static random number generator
    static boost::mt19937 rng(static_cast<unsigned> (std::time(0)));
    boost::normal_distribution<> norm(0,1.0);
    boost::variate_generator<boost::mt19937&,boost::normal_distribution<> > var_nor(rng, norm);

    double v = var_nor();
    m_additive = m_local_params.first()->globalAdditive();
//    m_proportional = exp(v * m_local_params.first().globalProportional());
    m_proportional = m_local_params.first()->globalProportional();

    // Vector that will contain the random values for the variable parameters
    ublas::vector<DBL> rnd_values (m_lower_cholesky_matrix.size1());

    //Get the first parameters set (the one before or at the initial start duration)
    ParameterSet* currentSet = m_local_params.first();

    int param_index=0;
    foreach(QString pName, currentSet->pids()) {
        SharedParameter param = currentSet->get(pName);
        if ((param->proportional()!=0.0) || (param->standard()!=0.0)) {
            if (param->proportional()!=0.0) {
                rnd_values(param_index) = var_nor() * param->proportional();
            }
            else if (param->standard()!=0.0) {
                rnd_values(param_index) = var_nor() * param->standard();
            }
            param_index++;
        }
    }
    rnd_values = prod(rnd_values,m_lower_cholesky_matrix);

    return rnd_values;
}

void CurveGeneratorThread::modifyParameters(SharedPrediction _pred, ublas::vector<DBL>& rnd_values) {

    //Randomly modifie each parameter for this person

    QList<ParameterSet*> varParameters;

    //Get the first parameters set (the one before or at the initial start duration)
    QList<ParameterSet*>::iterator i = m_local_params.begin();//upperBound(start);

    while (i != m_local_params.end() && (*i)->duration() < m_pub) {

        //Get the current parameters set
        ParameterSet* currentSet = SharedParameterSet(new ParameterSet(*i));

        int param_index=0;
        foreach(QString pName, currentSet->pids()) {
            SharedParameter param = currentSet->get(pName);
            if ((param->proportional()!=0.0) || (param->standard()!=0.0)) {
                if (param->proportional()!=0.0) {
                    // The parameter value is modified, using the LogNormal distribution
                    param->setValue(param->value() * exp(rnd_values(param_index)));
                }
                if (param->standard()!=0.0) {
                    // The parameter value is modified, using the Normal distribution
                    param->setValue(param->value() + rnd_values(param_index));
                //    param.value = param.value+rnd_values(param_index)*sqrt(sqrt(param.standard));
                }
                if (param->value() <= 0.0)
                    param->setValue(0.00001);
                param_index++;
            }
            currentSet->set(param);
        }

        *i = currentSet;

        i++;
    }

    //Set the parameters in the model and predict for the wanted range
    m_lcmod->setParameters(_pred, varParameters,TMP);
}

void CurveGeneratorThread::applyResidualVariance(DataSet& p) {

    boost::normal_distribution<> norm(0,1.0);
    boost::variate_generator<boost::mt19937&,boost::normal_distribution<> > var_nor(*this->m_randgen, norm);

    double v = var_nor();
    for(uint i = 0; i < p.size(); i++) {                        
        //        double v = 0;
        double val = p.at(i);
        if (val <= 0) {
            val = 0.001;
        }
        double ret;
        if (m_additive > 0) {
//            ret = p.at(i) + sqrt(pow(p.at(i)*m_proportional,2) + pow(m_additive,2)) * v;
//            if (ret < 0) {
                ret = sqrt(pow(m_proportional,2) + pow(m_additive,2) / pow(val,2)) * v;
                ret = exp(log(val) + ret);
//            }
        } else if (m_proportional > 0) {
            ret = val * exp(v * m_proportional);
        }
        p.set(i, ret >= 0.0 ? ret : 0.001);
    }
}

void CurveGeneratorThread::run()
{
    semStart.acquire();
    if (shouldTerminate)
    {

        return;
    }    

    for(int p = m_plb; p < m_pub; p++)
    {
        ublas::vector<DBL> rnd_values = generateCorrelatedRandomValues();
        SharedPrediction _pred = SharedPrediction(new Prediction());
        modifyParameters(_pred, rnd_values);
        m_people[p] = new DataSet(m_lcmod->points(_pred, m_start, m_stop, m_points, TMP, SharedDosage(new Dosage()), false));
        applyResidualVariance(*m_people[p]);
    }   


    semEnd.release();
}
