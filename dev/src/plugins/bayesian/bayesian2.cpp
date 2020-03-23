#include "core.h"
#include "core_errors.h"
#include "coremessagehandler.h"

#include "bayesian.h"
#include "prediction.h"

#include <QtPlugin>
#include <limits>
#include <cmath>

namespace ublas = boost::numeric::ublas;



// The following code was taken here:
// https://gist.github.com/lilac/2464434

/* Matrix inversion routine.
Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */
template<class T>
bool InvertMatrix (const ublas::matrix<T>& input, ublas::matrix<T>& inverse) {
    using namespace boost::numeric::ublas;
    typedef permutation_matrix<std::size_t> pmatrix;
    // create a working copy of the input
    matrix<T> A(input);
    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());

    // perform LU-factorization
    int res = lu_factorize(A,pm);
    if( res != 0 ) return false;

    // create identity matrix of "inverse"
    inverse.assign(ublas::identity_matrix<T>(A.size1()));

    // backsubstitute to get the inverse
    lu_substitute(A, pm, inverse);

    return true;
}

#include <QFile>
#include <QTextStream>




void Bayesian::updateEta(double deltas[],bool positive)
{
//    std::cout << "OldEtas" << std::endl;
//    for(int i=0;i<nbVariableParameters;i++)
//        std::cout << i << " : " << eta[i] << std::endl;

//    std::cout << "Deltas" << std::endl;
//    for(int i=0;i<nbVariableParameters;i++)
//        std::cout << i << " : " << deltas[i] << std::endl;
    int i=0;
    foreach (QString pName, keysOfVariableParameters) {
        if (positive)
            eta[i] += deltas[i];
        else
            eta[i] -= deltas[i];

        i++;
    }
}


void Bayesian::moveVariableParameters(ParameterSet* &parameters,double deltas[],bool positive)
{
    int i=0;
    foreach (QString pName, keysOfVariableParameters) {

        // update the current parameter value, in the final parameter list
        // Multiply the step by the median pop value, to accomodate order-of-magnitutde differences.
        if (positive)
            (*parameters)[pName]->setValue((*parameters)[pName]->value() + deltas[i]);
        else
            (*parameters)[pName]->setValue((*parameters)[pName]->value() - deltas[i]);

        // We have to ensure that the parameter stays positive, for
        // the Log to be potentially applied
        Q_ASSERT((*parameters)[pName]->value() > 0.0);
        //if (parameters[pName].value<=0.0)
        //    parameters[pName].value=0.0000000001;

        i++;
    }
}


void Bayesian::generatePhiExplorer(const QString& fileName)
{
    QFile fileMatlab(fileName);
    fileMatlab.open(QFile::WriteOnly);
    QTextStream outMatlab(&fileMatlab);

    outMatlab << "phiparam=load('travelphiparam.csv');\n";
    outMatlab << "phiobs=load('travelphiobs.csv');\n";
    outMatlab << "figure(1);\n";
    outMatlab << "clf;\n";
    outMatlab << "title(\"Evolution of Phi function. Blue for Obs impact, red for Params impact, green for total\");\n";
    outMatlab << "hold on;\n";
    outMatlab << "plot(phiparam,'r');\n";
    outMatlab << "plot(phiobs,'b');\n";
    outMatlab << "phitot = phiparam + phiobs;\n";
    outMatlab << "plot(phitot,  'g');\n";
    outMatlab << "hold off;\n";
    outMatlab << "\n";
    outMatlab << "figure(2);\n";
    outMatlab << "clf;\n";
    outMatlab << "hold on;\n";
    outMatlab << "tx=load('tx.csv');\n";
    outMatlab << "ty=load('ty.csv');\n";
    outMatlab << "phis=csvread('phi.csv');\n";
    outMatlab << "mesh(tx,ty,phis);\n";
    outMatlab << "travelp1=load('travelp1.csv');\n";
    outMatlab << "travelp2=load('travelp2.csv');\n";
    outMatlab << "travelphi=load('travelphi.csv');\n";
    outMatlab << "travelphi = travelphi + .01;\n";
    outMatlab << "plot3(travelp1,travelp2,travelphi,'*');\n";
    outMatlab << "h=plot3(travelp1,travelp2,travelphi,'1');\n";
    outMatlab << "set (h(1),'linewidth', 2);\n";
    outMatlab << "title(\"Travel into the search space.\");\n";
    outMatlab << "xlabel(\"Parameter 1\");\n";
    outMatlab << "ylabel(\"Parameter 2\");\n";
    outMatlab << "zlabel(\"Phi\");\n";
    outMatlab << "hold off;\n";

    fileMatlab.close();
}

void Bayesian::exportPhiSpace(const SharedMeasureSet measures,
                              Duration *measureTimes,
                              QList<ParameterSet*> const &params_pre,
                              const double pmin[2],const double pmax[2])
{
    logPhiCalls = false;

    double old_etas[nbVariableParameters];
    for(int i=0;i<nbVariableParameters;i++)
        old_etas[i] = eta[i];


    QFile file(exportStatsDir + "/phi.csv");
    QFile filex(exportStatsDir + "/tx.csv");
    QFile filey(exportStatsDir + "/ty.csv");
    QFile fileallx(exportStatsDir + "/alltx.csv");
    QFile fileally(exportStatsDir + "/allty.csv");
    if (!file.open(QFile::WriteOnly))
    {
        WARNING("The file for exporting points could not be opened");
        return;
    }
    if (!filex.open(QFile::WriteOnly))
    {
        WARNING("The file for exporting points could not be opened");
        return;
    }
    if (!filey.open(QFile::WriteOnly))
    {
        WARNING("The file for exporting points could not be opened");
        return;
    }
    if (!fileallx.open(QFile::WriteOnly))
    {
        WARNING("The file for exporting points could not be opened");
        return;
    }
    if (!fileally.open(QFile::WriteOnly))
    {
        WARNING("The file for exporting points could not be opened");
        return;
    }
    QTextStream out(&file);
    QTextStream outx(&filex);
    QTextStream outy(&filey);
    QTextStream outallx(&fileallx);
    QTextStream outally(&fileally);

    QList<ParameterSet*> params_ind(params_pre);

    double p1min = pmin[0];//-1;
    double p1max = pmax[0];//+2;
    double p2min = pmin[1];//-100;
    double p2max = pmax[1];//+100;
    unsigned nbp1points = 20;
    unsigned nbp2points = 20;

    //Check for division by 0
    Q_ASSERT(nbp1points - 1 != 0);
    Q_ASSERT(nbp2points - 1 != 0);

    double p1delta = (p1max-p1min)/((double)(nbp1points-1));
    double p2delta = (p2max-p2min)/((double)(nbp2points-1));

    for(float peo=0;peo<nbp2points;peo++) {
        eta[1] = p2min + p2delta * peo;
        outy << eta[1] << "\t";
        for(unsigned int po=0;po<nbp1points;po++) {
            eta[0] = p1min + p1delta * po;
            aPosterioriParameters(params_pre,params_ind);
            // YTA : TODO : This is wrong
            double phi0 = phiWithCorrelation(measures, measureTimes, params_ind);

            out << phi0 ;
            outallx << eta[0];
            outally << eta[1];
            if (po!=nbp1points-1)
            {
                out << ",";
                outallx << ",";
                outally << ",";
            }
            if (peo==0)
                outx << eta[0] << "\t";

        }
        out << "\n";
        outallx << "\n";
        outally << "\n";
    }
    file.close();
    filex.close();
    filey.close();
    fileallx.close();
    fileally.close();


    for(int i=0;i<nbVariableParameters;i++)
        eta[i] = old_etas[i];
}



bool Bayesian::calculateEta (QList<SharedParameterSet> const &aprioriParams,
                           SharedMeasureSet const &measures)
{
    nbVariableParameters = 0;
    keysOfVariableParameters.clear();    
    ParameterSet* first = aprioriParams.first();
    foreach(QString pName, first->pids()) {
        SharedParameter param = first->get(pName);
        if (param->isVariable()) {
            keysOfVariableParameters << pName;
            nbVariableParameters ++;
        }
    }
    if (eta != 0)
        delete[] eta;
    if (nbVariableParameters > 0)
        eta = new double[nbVariableParameters];

    for(int i=0;i<nbVariableParameters;i++)
        eta[i] = 0.0;


    //Cache of moment times from start (instead of QDateTime)
    QVector<Duration> measureTimes(measures->size());
    int cnt = 0;
    QDateTime firstTake = m_pred->treatment()->dosages()->firsttake();
    foreach (SharedMeasure m, *measures.data())
        measureTimes[cnt++] = Duration(0,0,firstTake.secsTo(m->moment()));

    // Phi function data
    double phi_new, phi_old;

    // Nothing to do
    if (measures->isEmpty()) {
        return true;
    }

    // Gradient descent parameters
    double eps = option(EPSILON).value.toDouble();
    eps = .01;
    double deps = option(DELTA_EPSILON).value.toDouble();
    double precision = option(PRECISION).value.toDouble();
    ignoreParameters = option(IGNOREPARAMETERS).value.toBool();
    ignoreResidual = option(IGNORERESIDUAL).value.toBool();
    ignoreCorrelations = option(IGNORECORRELATIONS).value.toBool();
    residualErrorType = option(RESIDERRORIMP).value.toInt();
    exportStats = option(EXPORTSTATS).value.toBool();
    exportStatsDir = option(STATSFILE_OPTION).value.value<DirName>();

    /*
    exportStats = false;
    exportStatsDir = DirName("/home/ythoma/docs/ezekiel/git/dev/src/dist/export");
*/
/*
    exportStatsDir = DirName("/home/ythoma/Desktop/ezechiel");
    exportStats = true;
*/

    //////////////////// FIXME: Input fields limited to 4 numbers...
    precision /= 1.E6;

    precision /= 1.E3;

    if (precision == 0)
    {
        precision = 1.E-300;
    }
    ////////////////////////////////////////////////////////////////

    // Calculation of the covariance matrix

    // Calculate the size of the covariance matrix
    // We create the smallest covariance matrix possible.
    // It will only contain the the parameters that do not have a 0 variance
    // Here we also store the Ids of these parameters, for future use
    covMatrixSize=nbVariableParameters;

    // We build the covariance matrix
    cov_matrix = boost::numeric::ublas::identity_matrix<DBL>(covMatrixSize);

    // Initialization of the mu vector, that will then be used during
    // computation
    mu = boost::numeric::ublas::vector<DBL>(covMatrixSize);

    ParameterSet* firstSet = aprioriParams.first();

    // Populate the covariance matrix
    int param_index=0;
    foreach(QString pName, keysOfVariableParameters) {
        SharedParameter param = firstSet->get(pName);

        if (!ignoreCorrelations) {
            int param_index2=0;
            foreach(QString pName2, keysOfVariableParameters) {
                SharedParameter param2 = firstSet->get(pName2);
                if (firstSet->correlation(pName,pName2)!=0.0) {
                    cov_matrix(param_index,param_index2)=firstSet->correlation(pName,pName2)*param->deviation()*param2->deviation();
                }
                param_index2++;
            }
        }
        // The diagonal contains the variances
        cov_matrix(param_index,param_index)=param->variance();
        mu(param_index) = param->value();
        param_index++;
    }



    // Compute the inverse of the covariance matrix, used during processing
    InvCovMatrix = boost::numeric::ublas::matrix<DBL, ORI> (covMatrixSize,covMatrixSize);
    if (!InvertMatrix(cov_matrix,InvCovMatrix))
    {
        qDebug() << QString("Impossible to inverte the covariance matrix");
        return false;
    }

/*
    std::cout << "Covariance matrix: " << std::endl;
    std::cout << cov_matrix << std::endl;

    std::cout << "Inverse of the covariance matrix: " << std::endl;
    std::cout << InvCovMatrix << std::endl;

    std::cout << "mu: " << std::endl;
    std::cout << mu << std::endl;
*/


    // First iteration : start from a priori values
    QList<ParameterSet*> params_ind(aprioriParams);

    // Set phi_new to the maximum value possible, as we are minimizing the function.
    phi_new = std::numeric_limits<double>::max();

    double deltas[covMatrixSize];
    for(int i=0;i<covMatrixSize;i++)
        deltas[i]=0.0;

    //int nb_iter;
    int nb_tot;
    int nb_loops = 0;

    QFile fileTravelP1(exportStatsDir + "/travelp1.csv");
    QFile fileTravelP2(exportStatsDir + "/travelp2.csv");
    QFile fileTravelPhi(exportStatsDir + "/travelphi.csv");
    if (exportStats)
    {
        fileTravelP1.open(QFile::WriteOnly);
        fileTravelP2.open(QFile::WriteOnly);
        fileTravelPhi.open(QFile::WriteOnly);

        generatePhiExplorer(exportStatsDir + "/phiexplorer.m");
    }
    QTextStream outTravelP1(&fileTravelP1);
    QTextStream outTravelP2(&fileTravelP2);
    QTextStream outTravelPhi(&fileTravelPhi);

    QFile fileTravelPhiParam;
    QFile fileTravelPhiObs;

    if (exportStats)
    {
        fileTravelPhiParam.setFileName(exportStatsDir + "/travelphiparam.csv");
        fileTravelPhiObs.setFileName(exportStatsDir + "/travelphiobs.csv");
        fileTravelPhiParam.open(QFile::WriteOnly);
        fileTravelPhiObs.open(QFile::WriteOnly);

        outPhiParamTravel.setDevice(&fileTravelPhiParam);
        outPhiObsTravel.setDevice(&fileTravelPhiObs);
    }


    aPosterioriParameters(aprioriParams, params_ind);
    phi_old = phiWithCorrelation(measures, measureTimes.data(), params_ind );


    double diff;

    double pmin[2];
    double pmax[2];
    pmin[0] = std::numeric_limits<double>::max();
    pmin[1] = std::numeric_limits<double>::max();
    pmax[0] = std::numeric_limits<double>::min();
    pmax[1] = std::numeric_limits<double>::min();

    nb_tot = 0;
    //Scoubi-
    do {
        nb_loops++;
//        qDebug() << QString("deps: %1. eps: %2").arg(deps).arg(eps);
        int nb_iter = 0;
        do { // ! Says Homer

            nb_iter++;
            nb_tot++;
            // Gradient descent

            /* Algo: 1) calculate phi at the current position
             *       2) for each parameter which has a deviation,
             *          2.1) calculate the derivative,
             *          2.2) then phi_i
             *          2.3) update the parameter value by incrementing it by phi_i*epsilon
             *       3) compute the new phi value, from the updated parameters
             */


            if (exportStats)
            {
                double p1value = eta[0];
                double p2value = eta[1];
                outTravelP1 << p1value << "\t";
                outTravelP2 << p2value << "\t";
                outTravelPhi << phi_old << "\t";
                if (p1value < pmin[0])
                    pmin[0] = p1value;
                if (p2value < pmin[1])
                    pmin[1] = p2value;
                if (p1value > pmax[0])
                    pmax[0] = p1value;
                if (p2value > pmax[1])
                    pmax[1] = p2value;
            }

            calculateGradient(measures, measureTimes.data(),aprioriParams, phi_old,eps, deltas);

            // Now deltas[] contains the gradient to follow

            updateEta(deltas,true);

            aPosterioriParameters(aprioriParams,params_ind);

            // Compute the new phi, with all the parameters adapted.
       //     std::cout << "Etas" << std::endl;
       //     for(int i=0;i<nbVariableParameters;i++)
       //         std::cout << i << " : " << eta[i] << std::endl;
            phi_new = phiWithCorrelation(measures, measureTimes.data(), params_ind);

//            if (phi_new < phi_old)
//                std::cout << "x" << std::endl;
//            else
//                std::cout << "-" << std::endl;

            diff = phi_new - phi_old;
            phi_old=phi_new;

            if ((exportStats) && (diff < 0.0))
            {
                logPhiCalls = true;
                phi_new = phiWithCorrelation(measures, measureTimes.data(), params_ind);
                logPhiCalls = false;
            }


/*
            qDebug() << QString(" phi_new: %1").arg(phi_new);
            qDebug() << QString(" diff: %1").arg(diff);
            qDebug() << QString(" eps: %1").arg(eps);
            qDebug() << QString(" eps: %1").arg(eps);
            qDebug() << QString(" diff: %1").arg(diff);
            qDebug() << QString(" delta0: %1").arg(deltas[0]);
            qDebug() << QString(" delta1: %1").arg(deltas[1]);
*/
        } while((diff < 0.0)&&(nb_iter < 100000));


        updateEta(deltas,false);

        aPosterioriParameters(aprioriParams,params_ind);
        phi_old = phiWithCorrelation(measures, measureTimes.data(), params_ind);


        // reduce the espilon step by the delta given
        eps *= deps;

    } while((eps > 0.0000001/*precision*/)&&(nb_tot<10000000));//&&(fabs(last_diff) > precision));

    // Potential export of all points into a .csv file.
    // This file can be useful for Matlab/Octave further analysis
    if (exportStats) {
        exportPhiSpace(measures, measureTimes.data(), aprioriParams, pmin, pmax);
    }

    /*
     *
    //Scoubi-
    do {
        nb_loops++;
//        qDebug() << QString("deps: %1. eps: %2").arg(deps).arg(eps);
        nb_tot=0;
        do { // ! Says Homer
            double phi0;

            // Gradient descent
            phi_old = phi_new;

            // Algo: 1) calculate phi at the current position
            //       2) for each parameter which has a deviation,
            //          2.1) calculate the derivative,
            //          2.2) then phi_i
            //          2.3) update the parameter value by incrementing it by phi_i*epsilon
            //       3) compute the new phi value, from the updated parameters
            //
            phi0 = phi_correlation(measures, measureTimes, params_ind, params_pre);

            if (exportStats)
            {
                outTravelP1 << params_ind.get(keysOfVariableParameters.at(0)).value << "\t";
                outTravelP2 << params_ind.get(keysOfVariableParameters.at(1)).value << "\t";
                outTravelPhi << phi0 << "\t";
            }

            calculateGradient(measures,measureTimes,params_ind,params_pre,phi0,eps,deltas);

            // Now deltas[] contains the gradient to follow

            last_diff=std::numeric_limits<double>::max();

            phi_new=phi0;

            nb_iter=0;

            // We follow the gradient while we go down
            do {
                nb_iter++;
                moveVariableParameters(params_ind,deltas,true);

                if (exportStats)
                {
                    outTravelP1 << params_ind.get(keysOfVariableParameters.at(0)).value << "\t";
                    outTravelP2 << params_ind.get(keysOfVariableParameters.at(1)).value << "\t";
                    outTravelPhi << phi0 << "\t";
                }

                last_diff=phi_old-phi_new;
                phi_old=phi_new;
                // Compute the new phi, with all the parameters adapted.
                phi_new = phi_correlation(measures,measureTimes,params_ind, params_pre);
                std::cout << "Last diff: " << last_diff << std::endl;
            } while ((phi_new<phi_old)&&(fabs(phi_new-phi_old)>.0001));

            // We go back for 1 step
            moveVariableParameters(params_ind,deltas,false);

            if (exportStats)
            {
                outTravelP1 << params_ind.get(keysOfVariableParameters.at(0)).value << "\t";
                outTravelP2 << params_ind.get(keysOfVariableParameters.at(1)).value << "\t";
                outTravelPhi << phi0 << "\t";
            }

            phi_new = phi_old;

         //   qDebug() << QString("nb iter: %1").arg(nb_iter);

         //   qDebug() << QString("phi_new: %1").arg(phi_new);
         //   qDebug() << QString("eps: %1").arg(eps);

         //   qDebug() << QString("lastdiff: %1").arg(last_diff);

            nb_tot++;
            if (nb_tot==10) {
                nb_tot=0;
                eps/=deps;
            }

            std::cout << "Last diff: " << last_diff << std::endl;
        } while((nb_iter!=1)&&(fabs(last_diff) > precision));


        // reduce the espilon step by the delta given
        eps *= deps;

    } while((eps > precision)&&(nb_loops<1000));//&&(fabs(last_diff) > precision));*/

    if (exportStats)
    {
        fileTravelPhi.close();
        fileTravelP1.close();
        fileTravelP2.close();
        fileTravelPhiParam.close();
        fileTravelPhiObs.close();
    }

    return true;
}

ParameterSet* Bayesian::aPosterioriParameters (ParameterSet* const &aprioriParams)
{
    ParameterSet* aposterioriParams = ParameterSet*(new ParameterSet(aprioriParams));
    aposterioriParams->clear();
    int index = 0;
    foreach (QString pid,aprioriParams->pids()) {
        SharedParameter newParameter(aprioriParams->get(pid));
        if (newParameter->proportional() != 0) {
            newParameter->setValue(newParameter->value() * exp(eta[index]));
            index ++;
        }
        else if (newParameter->standard() != 0) {
            newParameter->setValue(newParameter->value() + eta[index]);
            index ++;
        }
        aposterioriParams->set(newParameter);
    }
    return aposterioriParams;
}

void Bayesian::aPosterioriParameters (QList<ParameterSet*> const &aprioriParams,
                                        QList<ParameterSet*> &aposterioriParams)
{
    aposterioriParams.clear();

    QList<ParameterSet*>::const_iterator i = aprioriParams.constBegin();
    while (i != aprioriParams.constEnd()) {
        aposterioriParams.append(aPosterioriParameters(*i));
        ++i;
    }
}


#include <boost/math/tools/roots.hpp>

double SSprop;
double SSadd;
double Sz;
double Smu;

class Test {
public:
    double operator()(const double x) {
        return SSprop *(Sz - exp(x))*exp(x) - SSadd * (x - Smu);
    }
};

// For PI
#include <boost/math/constants/constants.hpp>

//Magic, I suppose
double Bayesian::phiWithCorrelation(const SharedMeasureSet measures,
                                 Duration *measureTimes,
                                 const QList<ParameterSet*> &params_ind)
{
    double phi = 0;

    if (!ignoreResidual) {

        ParameterSet* firstSet = params_ind.first();

        double Cpreds[measures->size()];

        double Sprop = firstSet->globalProportional();
        double Sadd = firstSet->globalAdditive();
        // Actually we need covariances, not standard deviations...
        Sprop = Sprop * Sprop;
        Sadd = Sadd * Sadd;

        // Compute Cpreds only if Sprop and/or Sadd are defined.
        if((Sprop != 0.0) || (Sadd != 0.0))
        {
            m_pred->analysis()->setParameters(m_pred, params_ind, TMP);
            for( int i = 0; i < measures->size(); i++)
                Cpreds[i] = m_pred->analysis()->point(m_pred, measureTimes[i], TMP).value();
        }

        // Calculation proposed by Aziz
        if (((Sprop != 0.0) && (Sadd != 0.0)) && (residualErrorType == 1))
        {
            for(int i = 0; i < measures->size(); i++)
            {
                Test t;
                typedef std::pair<double, double> Result;
                boost::uintmax_t max_iter=5000;
                boost::math::tools::eps_tolerance<double> tol(30);

                SSprop = Sprop;
                SSadd = Sadd;
                Smu = log(Cpreds[i]);
                Sz = (*measures.data())[i]->amount().value();

            //    std::cout << "Z: " << Sz << ". Smu: " << Smu << std::endl;

                Result r1 = boost::math::tools::toms748_solve(t, Smu-10.0, Smu + 10.0, tol, max_iter);
                double u0 = r1.first;

                double M = 1/(2*SSprop*SSadd);
                double gu = -SSadd * (u0 - Smu) * (u0 - Smu) - SSprop * (Sz - exp(Smu)) * (Sz - exp(Smu));

                double toAdd = M * gu;

                double gprimprimu = 2*(SSprop * exp(u0) * (Sz - 2* exp(u0)) - SSadd);
                double gprimu = 2*(SSprop * (Sz - exp(u0)) * exp(u0) - SSadd * (u0 - Smu));
                Q_ASSERT(gprimu < 0.000001);
                double absgprimprimu = std::abs(gprimprimu);

                toAdd += -log(absgprimprimu) / 2;
            //    std::cout << "To Add : " << toAdd << std::endl;

                phi -= toAdd;


            //    std::cout << "root bracketed: [ " << r1.first << " , " << r1.second <<  " ]" << std::endl;
            //    std::cout << "f("<< r1.first << ")=" << t(r1.first) << std::endl;
            //    std::cout << "f("<< r1.second << ")=" << t(r1.second) << std::endl;
            //    std::cout << "max_iter=" << max_iter << std::endl;
            }
        }
        else
        {

            // Ignore Sprop and/or Sadd if they are null.
            if (Sprop != 0)
            {
                for(int i = 0; i < measures->size(); i++)
                {
                    if (((*measures.data())[i]->amount().value()>0.0) && (Cpreds[i]>0.0)) {
                        double c = (log((*measures.data())[i]->amount().value()) - log(Cpreds[i]));
                        phi += (c*c)/(2*Sprop);
                    }
                }
            }

            if (Sadd != 0)
            {
                for( int i = 0; i < measures->size(); i++)
                {
                    double d = ((*measures.data())[i]->amount().value() - Cpreds[i]);
                    phi += (d * d)/(2*Sadd);
                }
            }
        }
    }

    if (exportStats && logPhiCalls)
        outPhiObsTravel << phi << "\t";

    if (!ignoreParameters) {

        double phiParam = 0.0;

        // First create the mu vector, containing the differences between
        // the individual parameters and the a priori parameters
        // The calculation depends on the type of variability.
        int paramIndex = 0;
        foreach (QString pName, keysOfVariableParameters) {
            SharedParameter pi = params_ind.first()->get(pName);
            if (pi->isVariable()) {
            //    if (pi.standard != 0.0)
                    mu(paramIndex) = eta[paramIndex];
            //    else
            //        mu(paramIndex) = (1-1/exp(eta[paramIndex])) * pi.value;
            }

            paramIndex ++;
        }

//        printInfos();

        // Computes mu^t * invCovMatrix * mu
        boost::numeric::ublas::vector<DBL> first_prod(covMatrixSize);
        first_prod = prod(InvCovMatrix,mu);

        for(int i=0;i<covMatrixSize;i++) {
            phiParam += mu(i) * first_prod(i);
        //    if (mu(i) > 0.0)
        //        phiParam += mu(i);
        //    else
        //        phiParam -= mu(i);

        }

        if (exportStats && logPhiCalls)
            outPhiParamTravel << phiParam << "\t";
        phi += phiParam;
    }

    return phi;
}

void Bayesian::printInfos()
{
    std::cout << "Cov matrix:" << std::endl;
    for(int i=0;i<nbVariableParameters;i++)
    {
        for(int j=0;j<nbVariableParameters;j++)
        {
            std::cout << cov_matrix(j,i) << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "Inv Cov matrix:" << std::endl;
    for(int i=0;i<nbVariableParameters;i++)
    {
        for(int j=0;j<nbVariableParameters;j++)
        {
            std::cout << InvCovMatrix(j,i) << "\t";
        }
        std::cout << std::endl;
    }


    std::cout << "eta:" << std::endl;
    for(int i=0;i<nbVariableParameters;i++)
    {
        std::cout << eta[i] << "\t";
    }
    std::cout << std::endl;

    std::cout << "mu:" << std::endl;
    for(int i=0;i<nbVariableParameters;i++)
    {
        std::cout << mu(i) << "\t";
    }
}


void Bayesian::calculateGradient(const SharedMeasureSet measures,
                                 Duration *measureTimes,
                                 const QList<ParameterSet*> &params_pre,
                                 double phi0,
                                 double eps,
                                 double *deltas)
{
    int i=0;
    double length=0.0;

    foreach (QString pName, keysOfVariableParameters) {

        double phi_i;
        const double H = 0.0000001;

        // Clone parameter list from the clean one
        QList<ParameterSet*> params_tmp;//(params_ind);

        // Modify the current parameter being derivated
        eta[i] += H;
        aPosterioriParameters(params_pre, params_tmp);

        //Check for division by 0
        Q_ASSERT(H != 0.0);

        // Compute its derivation

        phi_i = (phiWithCorrelation(measures,measureTimes,params_tmp) - phi0) / H;
        deltas[i] = -phi_i;

        length += phi_i*phi_i;

        eta[i] -= H;

        i++;
    }

    //Check for length validity
    Q_ASSERT(length >= 0.0);

    length = sqrt(length);

    double deltaFactor;
    if (length > 0.0)
        deltaFactor = eps/length;
    else
        deltaFactor = 0.0;

    for(int i=0;i<keysOfVariableParameters.size();i++)
    {
        deltas[i] = deltas[i]*deltaFactor;
    }
}
