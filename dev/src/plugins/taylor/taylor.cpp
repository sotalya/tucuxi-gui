//@@license@@

#include "taylor.h"

#include <math.h>

#include <boost/math/distributions.hpp>
#include <boost/math/special_functions/erf.hpp>
#include "prediction.h"
//#include <boost/random/normal_distribution.hpp>
//#include <boost/random/variate_generator.hpp>
//#include <boost/math/distributions/normal.hpp>
//#include <boost/random/uniform_real.hpp>
//#include <boost/random/mersenne_twister.hpp>

#include <QtPlugin>
#include <QFile>
#include <QTextStream>

#include "coremessagehandler.h"
#include "core_errors.h"
#include "units.h"

//------------------------------------ FACTORY -------------------------------------

//Save it, and create our descriptor
void TaylorFactory::init(Core *core)
{
    //Set the core
    Core::setInstance(core);

    //Oh yes we are
    _d.id = TAYLOR_ID;
    _d.name = "Taylor";
    _d.description = tr("Prediction engine using taylor based probability and other math stuff.");

    //Store the priorities
    _priorities.insert(_d.id, Taylor(_d).priority());
}

//Create it with our description
PercentileEngine *TaylorFactory::newPercentileEngine()
{
    return new Taylor(_d);
}

//Created in init()
Descriptor TaylorFactory::descriptor()
{
    return _d;
}

//Returns the engines priorities
int TaylorFactory::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}

//------------------------------------ PERCENTILE ENGINE -------------------------------------

#define QUANTILE_CALC_OPTION "quantilecalc"
#define VAR_CALC_OPTION "varcalc"

//The engine default priority level
const int Taylor::_priority = 10;

//Save the descriptor
Taylor::Taylor (const Descriptor &d) :
    m_pred(0)
{
    _d = d;

    // Way of calculating the quantiles
    addOption(QUANTILE_CALC_OPTION, tr("Way of calculating the quantiles"), tr("0: quantiles calculated with boost. 1: quantiles calculated with the algorithm from http://home.online.no/~pjacklam/notes/invnorm/"),
              QVariant::Int, QVariant(0));

    // Way of calculating the variances
    addOption(VAR_CALC_OPTION, tr("Way of calculating the variances"), tr("Way of calculating the variances"),
              QVariant::Int, QVariant(0));

}

//Create and copy
PercentileEngine *Taylor::clone()
{
    Taylor *instance = new Taylor(_d);
    instance->copyOptions(this);
    return instance;
}

//Given by the factory
Descriptor Taylor::descriptor ()
{
    return _d;
}

int Taylor::priority() const
{
    return _priority;
}

//Never seen this changed
Version Taylor::version ()
{
    return Version("0.1");
}

//Mostly empty
QString Taylor::errorString()
{
    return _errors;
}

//Simply save it
void Taylor::setAnalysis(SharedDrugResponseAnalysis model)
{
    m_pred->setAnalysis(model);
}

SharedDrugResponseAnalysis Taylor::analysis()
{
    return m_pred->analysis();
}

////////////////////////////////////////////////////////////////////////////////////////////////

MeanVarianceMarginalDistributionAtPoint::MeanVarianceMarginalDistributionAtPoint(SharedPrediction _pred,
                                                                                     ParameterSet* params_ind,
                                                                                     SharedMeasure measure)
    : m_pred(0), m_measure(0), m_measuretime(0)
{
    m_pred = _pred;
    m_measure = measure;
    m_measuretime = Duration(0,0,m_pred->treatment()->dosages()->firsttake().secsTo(measure->moment()));
    m_params_ind = params_ind;
    m_local_params_ind = ParameterSet*(new ParameterSet(params_ind));
}

MeanVarianceMarginalDistributionAtPoint::MeanVarianceMarginalDistributionAtPoint(SharedPrediction _pred,
                                                                                     ParameterSet* params_ind)
    : m_pred(0), m_measure(0), m_measuretime(0)
{
    m_pred = _pred;
    m_params_ind = params_ind;
    m_local_params_ind = ParameterSet*(new ParameterSet(params_ind));
}

MeanVarianceMarginalDistributionAtPoint::MeanVarianceMarginalDistributionAtPoint(SharedPrediction _pred,
                                                                                     const QList<ParameterSet*> &params_ind,
                                                                                     SharedMeasure measure,
                                                                                     const Duration& measuretime)
    : m_pred(0), m_measure(0), m_measuretime(0)
{
    m_pred = _pred;
    m_measure = measure;
    m_measuretime = measuretime;
    for (QList<ParameterSet*>::const_iterator it = params_ind.constBegin(); it != params_ind.constEnd(); ++it) {
        if (measuretime < (*it)->duration()) {
            m_params_ind = *it;
            m_local_params_ind = ParameterSet*(new ParameterSet(*m_params_ind));
            return;
        }
    }
}

double MeanVarianceMarginalDistributionAtPoint::calculateLogLikelihood(const QMap<QString, double>& _interindividuals)
{
    reinitialize();
    computeMeanAndVariance(_interindividuals);
    m_loglikelihood = log(m_variance) + pow((m_measure->amount().value() - m_mean), 2) / m_variance;
    return m_loglikelihood;
}

void MeanVarianceMarginalDistributionAtPoint::reinitialize()
{
    m_local_params_ind = ParameterSet*(new ParameterSet(*m_params_ind));
    m_pred->analysis()->setParameters(m_pred, m_local_params_ind, TMP);
    m_pred->treatment()->parameters(m_measuretime, TMP);
    m_mean = m_pred->analysis()->point(m_pred, m_measuretime, TMP).value();
}

void MeanVarianceMarginalDistributionAtPoint::computeMeanAndVariance(const QMap<QString, double>& _interindividuals)
{

    m_variance = 0;
    if (m_params_ind->globalAdditive() != 0) {

        m_variance = pow(m_params_ind->globalAdditive(), 2);
    }

    if (m_params_ind->globalProportional() != 0) {

        m_variance += pow(m_params_ind->globalProportional() * m_pred->analysis()->point(m_pred, m_measuretime, TMP).value(), 2);
    }
    QMap<QString, double> _derivs;
    for (QMap<QString, double>::const_iterator it = _interindividuals.constBegin();
         it != _interindividuals.constEnd(); ++it) {

        if ((*m_params_ind)[it.key()]->deviation() != 0) {

            double _deriv = derivative(0.0, (double)(*m_local_params_ind)[(QString)it.key()]->deviation()/1000000, [=](double x) -> double
            {
                switch((*m_local_params_ind)[(QString)it.key()]->distribution()) {
                    case distribution::LogNormal:
                        (*m_local_params_ind)[(QString)it.key()]->setValue((*m_local_params_ind)[(QString)it.key()]->value() * exp(x));
                        m_pred->analysis()->setParameters(m_pred, m_local_params_ind, TMP);
                        return m_pred->analysis()->point(m_pred, m_measuretime, TMP).value();
                        break;
                    case distribution::Normal:
                        (*m_local_params_ind)[(QString)it.key()]->setValue((*m_local_params_ind)[(QString)it.key()]->value() + x);
                        m_pred->analysis()->setParameters(m_pred, m_local_params_ind, TMP);
                        return m_pred->analysis()->point(SharedPrediction(), m_measuretime, TMP).value();
                        break;
                    default:
                        break;
                }

                return -1.0;
            } );
            _derivs.insert(it.key(),_deriv);
        }
    }

    for (QMap<QString, double>::iterator it = _derivs.begin();
         it != _derivs.end(); ++it) {
        m_mean -= _interindividuals[it.key()] * it.value();
        foreach (QString pid, m_local_params_ind->pids()) {
            if (m_local_params_ind->correlation(it.key(), pid) != 0) {
                m_variance += 2 * it.value() * _derivs[pid] * m_local_params_ind->correlation(it.key(), pid) * (*m_local_params_ind)[pid]->deviation() * (*m_local_params_ind)[it.key()]->deviation();
            }
        }
        m_variance += pow(it.value(),2) * (*m_local_params_ind)[it.key()]->deviation() * (*m_local_params_ind)[it.key()]->deviation();
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////
// http://home.online.no/~pjacklam/notes/invnorm/impl/natarajan/normsinv.h

#include <math.h>

#define  A1  (-3.969683028665376e+01)
#define  A2   2.209460984245205e+02
#define  A3  (-2.759285104469687e+02)
#define  A4   1.383577518672690e+02
#define  A5  (-3.066479806614716e+01)
#define  A6   2.506628277459239e+00

#define  B1  (-5.447609879822406e+01)
#define  B2   1.615858368580409e+02
#define  B3  (-1.556989798598866e+02)
#define  B4   6.680131188771972e+01
#define  B5  (-1.328068155288572e+01)

#define  C1  (-7.784894002430293e-03)
#define  C2  (-3.223964580411365e-01)
#define  C3  (-2.400758277161838e+00)
#define  C4  (-2.549732539343734e+00)
#define  C5   4.374664141464968e+00
#define  C6   2.938163982698783e+00

#define  D1   7.784695709041462e-03
#define  D2   3.224671290700398e-01
#define  D3   2.445134137142996e+00
#define  D4   3.754408661907416e+00

#define P_LOW   0.02425
/* P_high = 1 - p_low*/
#define P_HIGH  0.97575

long double normsinv(long double p)
{
    long double x;
    long double q, r;//, u, e;
    if ((0 < p )  && (p < P_LOW)){
        q = sqrt(-2*log(p));
        x = (((((C1*q+C2)*q+C3)*q+C4)*q+C5)*q+C6) / ((((D1*q+D2)*q+D3)*q+D4)*q+1);
    }
    else{
        if ((P_LOW <= p) && (p <= P_HIGH)){
            q = p - 0.5;
            r = q*q;
            x = (((((A1*r+A2)*r+A3)*r+A4)*r+A5)*r+A6)*q /(((((B1*r+B2)*r+B3)*r+B4)*r+B5)*r+1);
        }
        else{
            if ((P_HIGH < p)&&(p < 1)){
                q = sqrt(-2*log(1-p));
                x = -(((((C1*q+C2)*q+C3)*q+C4)*q+C5)*q+C6) / ((((D1*q+D2)*q+D3)*q+D4)*q+1);
            }
        }
    }

    /* If you are compiling this under UNIX OR LINUX, you may uncomment this block for better accuracy.
if(( 0 < p)&&(p < 1)){
   e = 0.5 * erfc(-x/sqrt(2)) - p;
   u = e * sqrt(2*M_PI) * exp(x*x/2);
   x = x - u/(1 + x*u/2);
}
*/

    return x;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

QMap<int,DataSet> Taylor::percentiles (const QList<int> &values, const Duration &start, const Duration &stop, uint points, const QList<ParameterSet*> &parameters, bool uniform)
{
    //Error : no model!
    if (!m_pred->analysis()) {
        _errors = tr("No model given, cannot compute");
        return QMap<int, DataSet>();
    }

    QList<ParameterSet*> local_parameters;

    for (QList<ParameterSet*>::const_iterator it = parameters.constBegin(); it != parameters.constEnd(); ++it) {

        if (start >= (*it)->duration() && stop >= (*it)->duration()) {
            local_parameters.append(*it);
        }
    }

    //const Duration * _movingstart = &start;
    QMap<int, DataSet> results;
    foreach (int v, values) {
        results.insert(v, DataSet());
    }
    for (QList<ParameterSet*>::iterator it = local_parameters.begin(); it != local_parameters.end(); ++it) {
        QMap<QString, double> _interindividuals;
        foreach (QString pid, (*it)->pids()) {
            _interindividuals.insert(pid, (*(*it))[pid]->value());
        }
        m_pred->analysis()->setParameters(m_pred, *it,TMP);
        //!!!!!!!
        //DataSet _meanset = _model->points(*_movingstart, it.key(), points, TMP, Dosage(), uniform);
        DataSet _meanset = m_pred->analysis()->points(m_pred, start, stop, points, TMP, SharedDosage(new Dosage()), uniform);
        QList<double> meanscheck;
            for (unsigned int i = 0; i < _meanset.size(); ++i) {
                meanscheck.append(_meanset[i]);
            }

        DataSet _variances = taylorize(_meanset,_interindividuals, *it);
        QList<double> varscheck;
            for (unsigned int i = 0; i < _variances.size(); ++i) {
                varscheck.append(_variances[i]);
            }
        //_movingstart = &it.key();

        foreach (int v, values) {
            DataSet tmpds = percentile(v, _meanset, _variances);
            results[v].setUnit(tmpds.unit());
            results[v] = results[v] + tmpds;
        }
    }
    return results;
}

DataSet Taylor::taylorize(DataSet &_meanset, QMap<QString, double> &_interindividuals, const ParameterSet* pset) {
    DataSet _result(_meanset.unit());
    MeanVarianceMarginalDistributionAtPoint mvmd(m_pred, pset);
    int i = 0;
    foreach (CycleData cycle, _meanset.allCycles()) {
        CycleData _newCycle(cycle.size(), cycle.unit());
        _newCycle.setTimes(cycle.startTime(), cycle.endTime(), cycle.type());
        for (unsigned int j = 0; j < _newCycle.size(); j++) {
            mvmd.setMean(_meanset[i]);
            double* times;
            _meanset.getTimes(&times);
            int timept = times[i];
            mvmd.setTimept(timept);
            mvmd.computeMeanAndVariance(_interindividuals);
            _newCycle[j] = mvmd.getVariance();
//            _meanset[i] = mvmd.getMean();
            i++;
        }
        _result.addCycleData(_newCycle);
    }
    return _result;
}

DataSet Taylor::percentile(const int percent, DataSet &_meanset, DataSet &_variances) {

        //Create the dataset for the current percentile
        DataSet _result(_meanset.unit());

        //Compute the points and push them in the result
        double sigma = 0;
        double mu = 0;
        double var = 0;
        int i = 0;
        foreach (CycleData cycle, _meanset.allCycles()) {
            CycleData newCycle(cycle.size(), cycle.unit());
            newCycle.setTimes(cycle.startTime(), cycle.endTime(), cycle.type());

            //Compute the current cycle values
            for (unsigned int j = 0; j < newCycle.size(); j++) {

                var = _variances[i];
                if (_meanset[i] != 0 && var != 0) {
                    sigma = sqrt(log(var / pow(_meanset[i],2) + 1));
                    mu = log(_meanset[i]) - pow(sigma,2)/2;
//                    mu = log(pow(mean[i],2) / sqrt(var + pow(mean[i],2)));
                }

                double tmp = exp(sqrt(2) * sigma * boost::math::erf_inv(2 * percent/100.0 - 1) + mu);// * (1 + var_nor());
                newCycle[j] = tmp;
                //                norm.param(nprmsa);
//                newCycle[j] += var_nor();

                // These lines are truncating the result in order not to freeze the display
                if (newCycle[j] > 10000000.0)
                    newCycle[j] = 10000000.0;
                if (newCycle[j] < 0.0)
                    newCycle[j] = 0.0;

                //Increment the global mean index
                i++;
            }

            //Add the current cycle
            _result.addCycleData(newCycle);
        }
    return _result;
}

//QMap<int,DataSet> Taylor::percentiles (const QList<int> &values, const Duration &start, const Duration &stop, uint points, const QMap<Duration, ParameterSet> &parameters, bool uniform)
//{
//    //Fetching the options
//    int quantile_calc = option(QUANTILE_CALC_OPTION).value.toInt();
//    int var_calc = option(VAR_CALC_OPTION).value.toInt();

//    //Error : no model!
//    if (!_model) {
//        _errors = tr("No model given, cannot compute");
//        return QMap<int, DataSet>();
//    }

////    static boost::mt19937 rng(static_cast<unsigned> (std::time(0)));
////    boost::normal_distribution<> norm(0,1.0);
////    boost::variate_generator<boost::mt19937&,boost::normal_distribution<> > var_nor(rng, norm);

//    QMap<Duration, ParameterSet> local_parameters = parameters;


//    //Get the first parameters set (the one before or at the initial start duration)
//    QMap<Duration, ParameterSet>::const_iterator i = parameters.upperBound(start);

//    // std::cout << "NbSets: " << parameters.size() << std::endl;
//    if (i != parameters.constBegin())
//        i--;

//    ParameterSet* firstParameterSet = parameters.constBegin().value();

//    //Get the current parameters set type
//    ParameterType type = firstParameterSet.type();

//    //   type = APOSTERIORI;

//    //First pass : the normal curve
//    //    _model->setParameters(local_parameters,TMP,start);
////    DataSet mean = _model->points(start,stop,points,type);

//    _model->setParameters(parameters,TMP);
//    DataSet mean = _model->points(start, stop, points, TMP, Dosage(), uniform);
////    QList<double> meanscheck;
////    for (unsigned int i = 0; i < mean.size(); ++i) {
////        meanscheck.append(mean[i]);
////    }

//    //The resulting sets, one for each input value
//    QMap<int, DataSet> results;

//    //For each of the wanted percentile, do the job
//    foreach (int v, values) {

//        qDebug() << QString("Taylor percentile %1 on type %2").arg(v).arg(type);

//        //Clear out the variances
//        double variances[points];
//        for(uint i=0; i<points; i++)
//            variances[i]=0.0;


//        double variances_std[points];
//        for(uint i=0; i<points; i++)
//            variances_std[i]=0.0;



//        //Modify each parameters using Taylor's method
//        foreach (QString pName, firstParameterSet.pids()) {

//            double h=.1; //Magic value : Could this be an option?
//            double derivative;

//            Parameter param = firstParameterSet.get(pName);
//            //if (0)
//            if (param.standard!=0.0) {
//                //Modify the parameter and insert it in the predictor as temporary

//                QMap<Duration, ParameterSet> varParameters;

//                //Get the first parameters set (the one before or at the initial start duration)
//                QMap<Duration, ParameterSet>::const_iterator i = parameters.constBegin();//upperBound(start);

//                while (i != parameters.constEnd() && i.key() < stop) {

//                    //Get the current parameters set
//                    ParameterSet* currentSet = i.value();
//                    Parameter newparam = currentSet.get(pName);
//                    newparam.value += h;
//                    currentSet.set(newparam);
//                    varParameters[i.key()]=currentSet;

//                    i++;
//                }

//                _model->setParameters(varParameters,TMP);


//                //Mean value with the modified parameter
//                DataSet mean_h = _model->points(start, stop, points, TMP, Dosage(), uniform);

//                //Here's the magic!
//                for(uint i=0;i<mean_h.size();i++)
//                {

//                    if ((mean_h[i]>0.00001)||(mean[i]>0.00001)) {

//                        // Normal distribution
//                        derivative=(mean_h[i]-mean[i])/h;
//                        variances_std[i] += param.standard*param.standard*derivative*derivative;
//                    }
//                }
//            }
//            if(param.proportional!=0.0) {

//                QMap<Duration, ParameterSet> newParams;


//                //Get the first parameters set (the one before or at the initial start duration)
//                QMap<Duration, ParameterSet>::const_iterator i = local_parameters.constBegin();

//                while (i != local_parameters.constEnd() && i.key() < stop) {

//                    //Get the current parameters set
//                    ParameterSet* currentSet = i.value();
//                    Parameter newparam = currentSet.get(pName);
//                    switch(var_calc) {
//                    case 0: newparam.value += h;break;
//                    case 1: newparam.value *= exp(h);break;
//                    case 2: newparam.value += h;break;
//                    case 3: newparam.value += h;break;
//                    case 4: newparam.value *= exp(h);break;
//                    case 5: newparam.value *= exp(h);break;
//                    default: WARNING("Unsupported way of calculating the variances");break;
//                    }
//                    currentSet.set(newparam);
//                    newParams[i.key()]=currentSet;

//                    i++;
//                }

//                _model->setParameters(newParams,TMP);

//                //Mean value with the modified parameter
//                DataSet mean_h = _model->points(start, stop, points, TMP, Dosage(), uniform);
////                QList<double> meanshcheck;
////                for (unsigned int i = 0; i < mean_h.size(); ++i) {
////                    meanshcheck.append(mean[i]);
////                }
//                /*
//                for(int i=1;i<points;i++)
//                    if (mean_h[i]>mean_h[i-1])
//                        std::cout << "mean_h[i] : " << mean_h[i] << ". mean_h[i-1] : " << mean_h[i-1] << std::endl;

//                for(int i=1;i<points;i++)
//                    if (mean[i]>mean[i-1])
//                        std::cout << "mean[i] : " << mean_h[i] << ". mean[i-1] : " << mean_h[i-1] << std::endl;
//*/

////                QFile file(QString("/home/ythoma/Desktop/taylor_%1.csv").arg(param.pid));
////                file.open(QFile::WriteOnly);
////                QTextStream out(&file);


//                Duration step = (stop-start)/((int)(mean_h.size()));

//                //Here's the magic!
//                for(int i=0;static_cast<uint>(i)<mean_h.size();i++) {

//                    //Get the first parameters set (the one before or at the initial start duration)
//                    QMap<Duration, ParameterSet>::const_iterator varParamIter = newParams.upperBound(start);

//                    if (varParamIter != newParams.constBegin())
//                        varParamIter--;


//                    QMap<Duration, ParameterSet>::const_iterator stdParamIter = parameters.upperBound(start);

//                    if (stdParamIter != newParams.constBegin())
//                        stdParamIter--;

//                    // YTA: TODO : We have a problem here with parameters changing between cycles, like the PNA

//                    while (varParamIter != newParams.constEnd() && varParamIter.key() < stop) {

//                        Duration next = (varParamIter+1).key();
//                        if ((varParamIter.key()<=(start+step*i)) && (((varParamIter+1)==newParams.constEnd())||(next>(start+step*i)))) {


//                            //Get the current parameters set
//                            Parameter newparam = varParamIter.value().get(pName);
//                            Parameter param = stdParamIter.value().get(pName);


//                            if ((mean_h[i]>0.00001)||(mean[i]>0.00001)) {
//                                // LogNormal distribution. To be still verified
//                                //
//                                switch(var_calc) {
//                                case 0: {
//                                    //                            derivative=(log(mean_h[i])-log(mean[i]))/(log(newparam.value)-log(param.value));
////                                    derivative=(log((long double)(mean_h[i]))-log((long double)(mean[i])))/
////                                            (log((long double)(newparam.value))-log((long double)(param.value)));
//                                    derivative = (mean_h[i] - mean[i]) / h;
//                                } break;
//                                case 1: {
//                                    derivative=(mean_h[i]-mean[i])/(param.value*exp(h)-param.value);
//                                } break;
//                                case 2: {
//                                    derivative=(mean_h[i]-mean[i])/(log(h));
//                                } break;
//                                case 3: {
//                                    derivative=(mean_h[i]-mean[i])/(log(param.value+h)-log(param.value));
//                                } break;
//                                case 4: {
//                                    derivative=(mean_h[i]-mean[i])/(h);
//                                } break;
//                                case 5: {
//                                    derivative=(mean_h[i]-mean[i])/(log(h));
//                                } break;

//                                default: {
//                                    WARNING("No way to calculate the variances");
//                                } break;
//                                }
//                                double var = variances[i];
//                                double pNamevar = (exp(pow(firstParameterSet[pName].deviation(),2)) - 1) * pow(firstParameterSet[pName].value,2);
//                                foreach (QString pid, firstParameterSet.pids()) {
//                                    if (firstParameterSet.correlation(pName, pid) != 0) {
//                                        double pidvar = (exp(pow(firstParameterSet[pid].deviation(),2)) - 1) * pow(firstParameterSet[pid].value,2);
//                                        var += pow(derivative,2) * firstParameterSet.correlation(pName, pid) * sqrt(pidvar) * sqrt(pNamevar);
//                                        mean[i] -= derivative*firstParameterSet[pid].value;
//                                    }
//                                }
//                                var += pow(derivative,2) * pNamevar;
//                                variances[i] = var;
////                                if (v==90) {
////                                    out << mean_h[i] << "\t";
////                                    out << mean_h[i] << "\t";
////                                    out << newparam.value << "\t";
////                                    out << param.value << "\t";
////                                    out << log(newparam.value) << "\t";
////                                    out << log(param.value) << "\t";
////                                    out << derivative << "\t";
////                                    out << param.proportional*derivative*derivative << "\n";
////                                }
//                            }
//                        }

//                        varParamIter++;
//                        stdParamIter++;
//                    }
//                    variances[i] += pow(firstParameterSet.globalAdditive(), 2);
//                    variances[i] += pow(mean[i] * firstParameterSet.globalProportional(), 2);
//                }


//            }
//        }

//        double proportional = firstParameterSet.globalProportional();
//        double additive = firstParameterSet.globalAdditive();

////        boost::random::normal_distribution<>::param_type nprmsp(0,proportional);
////        boost::random::normal_distribution<>::param_type nprmsa(0, additive);

//        double quant;

//        // The inverse of the cumulative function is calculated with the boost
//        // library or using C code developed by a guy called natarajan
//        switch (quantile_calc) {
//        case 0: {
//            boost::math::normal norm(0,1.0);
//            quant = quantile(norm,v/100.0);
//        } break;
//        case 1: {
//            quant = normsinv(v/100.0);
//        } break;
//        default: {
//            WARNING("The quantile calculation is not well defined. Should be 0 or 1");
//            boost::math::normal norm(0,1.0);
//            quant= quantile(norm,v/100.0);
//        } break;
//        }

//        //Create the dataset for the current percentile
//        DataSet result(mean.unit());

//        //Compute the points and push them in the result
//        int i = 0;
//        foreach (CycleData cycle, mean.allCycles()) {
//            CycleData newCycle(cycle.size(), cycle.unit());
//            newCycle.setTimes(cycle.startTime(), cycle.endTime(), cycle.type());

//            //Compute the current cycle values
//            for (unsigned int j = 0; j < newCycle.size(); j++) {
//                double sigma = 0;
//                double mu = 0;
//                double var = variances[i];
//                if (mean[i] != 0 && var != 0) {
//                    sigma = 0.5 * log(var / pow(mean[i],2) + 1);
//                    mu = log(mean[i]) - pow(sigma,2)/2;
////                    mu = log(pow(mean[i],2) / sqrt(var + pow(mean[i],2)));
//                }

//                double tmp = exp(sqrt(2) * sigma * boost::math::erf_inv(2 * v/100.0 - 1) + mu);// * (1 + var_nor());
//                newCycle[j] = tmp;
//                //                norm.param(nprmsa);
////                newCycle[j] += var_nor();

//                // These lines are truncating the result in order not to freeze the display
//                if (newCycle[j] > 10000000.0)
//                    newCycle[j] = 10000000.0;
//                if (newCycle[j] < 0.0)
//                    newCycle[j] = 0.0;

//                //Increment the global mean index
//                i++;
//            }

//            //Add the current cycle
//            result.addCycleData(newCycle);
//        }

//        //Add the result in the results (And yes, that do make sense)
//        results.insert(v,result);
//    }

//    return results;
//}

