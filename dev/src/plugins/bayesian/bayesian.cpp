#include "core.h"
#include "core_errors.h"
#include "coremessagehandler.h"

#include "bayesian.h"
#include "prediction.h"

#include <QtPlugin>
#include <limits>
#include <cmath>

//------------------------------------ FACTORY -------------------------------------

//Save it, and create our descriptor
void BayesianFactory::init(Core *core)
{
    //Set the core
    Core::setInstance(core);

    //Oh yes we are
    _d.id = BAYESIAN_ID;
    _d.name = "Bayesian";
    _d.description = "Approximate patient parameters using a maximum of likelihood approach.";

    //Store the priorities
    _priorities.insert(_d.id, Bayesian(_d).priority());
}

//Create it with our description
PostEngine *BayesianFactory::newPostEngine()
{
    return new Bayesian(_d);
}

//Created in init()
Descriptor BayesianFactory::descriptor()
{
    return _d;
}

//Returns an engine priority
int BayesianFactory::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}

//------------------------------------ PERCENTILE ENGINE -------------------------------------

//The engine default priority level
const int Bayesian::_priority = 10;

//Save the descriptor, setup the options
Bayesian::Bayesian (const Descriptor &d) :
    m_pred(0),
    eta(0),
    nbVariableParameters(0)
{
    _d = d;

    // Gradient descent parameters
    addOption(IGNOREPARAMETERS, tr("Ignore parameters"), tr("Ignore the parameters variability during the process"),
              QVariant::Bool, QVariant(false));
    addOption(IGNORERESIDUAL, tr("Ignore residual"), tr("Ignore the residual error during the process"),
              QVariant::Bool, QVariant(false));
    addOption(IGNORECORRELATIONS, tr("Ignore correlations"), tr("Ignore the parameters correlations"),
              QVariant::Bool, QVariant(false));
    addOption(EPSILON, tr("Epsilon"), tr("Initial Epsilon for gradient steps"),
              QVariant::Double, QVariant(1.));
    addOption(DELTA_EPSILON, tr("Epsilon delta"), tr("Epsilon multiplicator (should be <= 1)"),
              QVariant::Double, QVariant(0.5));
    addOption(PRECISION, tr("Precision (xE-6)"), tr("Precision (stop searching for the minima when (X1 - X2 < precision*E-6))"),
              QVariant::Double, QVariant(10));
    addOption(CALCULIMP, tr("Calculus implementation"), tr("Defines the algo used. 0: Yann. 1: Lionel. 2:With correlation"),
              QVariant::Int, QVariant(2));

    addOption(RESIDERRORIMP, tr("Residual error implementation"), tr("Defines the algo used for calculating the residual error impact. 0: Laplace. 1: Additive"),
              QVariant::Int, QVariant(0));


    addOption(EXPORTSTATS, tr("Export computation details into files"), tr("If checked, then all the calculus points are exported to files that can be viewed in Octave/Matlab."),
              QVariant::Bool, QVariant(false));

    //Name of the directory where the files are saved
    addOption(STATSFILE_OPTION, tr("Directory name for export"), tr("All the files will be generated into this directory."),
              qMetaTypeId<DirName>(), QVariant::fromValue(DirName("")));

}

//Create and copy
PostEngine *Bayesian::clone()
{
    Bayesian *instance = new Bayesian(_d);
    instance->copyOptions(this);
    return instance;
}

//Given by the factory
Descriptor Bayesian::descriptor ()
{
    return _d;
}

int Bayesian::priority() const
{
    return _priority;
}

//0.3 : Because
Version Bayesian::version ()
{
    return Version("0.3");
}

//Mostly empty
QString Bayesian::errorString()
{
    return _errors;
}

//Simply save it
void Bayesian::setPrediction(SharedPrediction pred)
{
    m_pred = pred;
}



//We're talking about wizards, but not the ones showed in GUI. The ones that do magic!
ParameterSet* Bayesian::aposteriori_lionel(const ParameterSet* &input, const QList<SharedMeasure> &measures, const Duration &, const Duration &)
{
    //Cache of moment times from start (instead of QDateTime)
    QVector<Duration> measureTimes(measures.size());
    int cnt = 0;
    QDateTime firstTake = m_pred->treatment()->dosages()->firsttake();
    foreach (SharedMeasure m, measures)
        measureTimes[cnt++] = Duration(0,0,firstTake.secsTo(m->moment()));

    // Phi function data
    double phi_new, phi_old;

    // Nothing to do
    if (measures.isEmpty()) {
        return input;
    }

    // Gradient descent parameters
    double eps = option(EPSILON).value.toDouble();
    double deps = option(DELTA_EPSILON).value.toDouble();
    double precision = option(PRECISION).value.toDouble();

    //////////////////// FIXME: Input fields limited to 4 numbers...
    precision /= 1.E6;

    if (precision == 0)
    {
        precision = 1.E-300;
    }
    ////////////////////////////////////////////////////////////////

    // Use the given parameters
    ParameterSet* params_pre = input;

    // First iteration : start from a priori values
    ParameterSet* params_ind(params_pre);

    // Set phi_new to the maximum value possible, as we are minimizing the function.
    phi_new = std::numeric_limits<double>::max();

    //Scoubi-
    do {
//        qDebug() << QString("deps: %1. eps: %2").arg(deps).arg(eps);
        do { // ! Says Homer
            double phi0;

            // Clone the parameters as we will be adapting them individually, starting
            // from the base parameters
            ParameterSet* params_loop(params_ind);

            // Gradient descent
            phi_old = phi_new;

            /* Algo: 1) calculate phi at the current position
             *       2) for each parameter which has a deviation,
             *          2.1) calculate the derivative,
             *          2.2) then phi_i
             *          2.3) update the parameter value by incrementing it by phi_i*epsilon
             *       3) compute the new phi value, from the updated parameters
             */
            phi0 = phi(measures, measureTimes.data(), params_loop, params_pre);

            foreach (QString pName, params_pre->pids()) {
                SharedParameter ppop = params_pre->get(pName);

                if (ppop->standard() != 0) {
//                if ((ppop.standard != 0)||(ppop.proportional != 0)) {
                    double phi_i;
                    const double H = 0.000000001;

                    // Clone parameter list from the clean one
                    ParameterSet* params_tmp(params_loop);

                    // Modify the current parameter being derivated
                    (*params_tmp)[ppop->name()]->setValue((*params_loop)[ppop->name()]->value() + H);

                    //Check for division by 0
                    Q_ASSERT(H != 0.0);

                    // Compute its derivation
                    phi_i = (phi(measures,measureTimes.data(), params_tmp, params_pre) - phi0) / H;

                    // update the current parameter value, in the final parameter list
                    // Multiply the step by the median pop value, to accomodate order-of-magnitutde differences.
                    (*params_ind)[ppop->name()]->setValue(phi_i*eps);

                    // We have to ensure that the parameter stays positive, for
                    // the Log to be potentially applied
                    if ((*params_ind)[ppop->name()]->value()<=0.0)
                        (*params_ind)[ppop->name()]->setValue(0.0000000001);
              //      qDebug() << QString("phi_i*eps: %1").arg(phi_i*eps);
                }
            }

            // Compute the new phi, with all the parameters adapted.
            phi_new = phi(measures,measureTimes.data(), params_ind, params_pre);
        //    qDebug() << QString("phi_new: %1").arg(phi_new);

        } while((phi_old - phi_new) > precision);

        // reduce the espilon step by the delta given
        eps *= deps;
    } while(eps > precision);

    qDebug() << QString("********************************");
    qDebug() << QString("phi_old %1 phi_new %2 delta %3 epsilon %4)
          .arg(phi_old)
          .arg(phi_new)
          .arg(phi_old - phi_new)
          .arg(eps)
          );

    return params_ind;
}


ParameterSet* Bayesian::aposteriori(const ParameterSet* &input, const QList<SharedMeasure> &measures, const Duration &intervalStart, const Duration &intervalStop)
{
    Q_UNUSED(intervalStop);

    int calc_imp=option(CALCULIMP).value.toInt();
    switch(calc_imp) {
    case 0:default: return aposteriori_yann(input,measures,intervalStart,intervalStart);
    case 1: return aposteriori_lionel(input,measures,intervalStart,intervalStart);
    //case 2: return aposteriori_correlation(input,measures,intervalStart,intervalStart);
    }

}


//We're talking about wizards, but not the ones showed in GUI. The ones that do magic!
ParameterSet* Bayesian::aposteriori_yann(const ParameterSet* &input, const QList<SharedMeasure> &measures, const Duration &, const Duration &)
{
    //Cache of moment times from start (instead of QDateTime)
    QVector<Duration> measureTimes(measures.size());
    int cnt = 0;
    QDateTime firstTake = m_pred->treatment()->dosages()->firsttake();
    foreach (SharedMeasure m, measures)
        measureTimes[cnt++] = Duration(0,0,firstTake.secsTo(m->moment()));

    // Phi function data
    double phi_new, phi_old;

    // Nothing to do
    if (measures.isEmpty()) {
        return input;
    }

    // Gradient descent parameters
    double eps = option(EPSILON).value.toDouble();
    double deps = option(DELTA_EPSILON).value.toDouble();
    double precision = option(PRECISION).value.toDouble();
    ignoreParameters = option(IGNOREPARAMETERS).value.toBool();
    ignoreResidual = option(IGNORERESIDUAL).value.toBool();
    residualErrorType = option(RESIDERRORIMP).value.toInt();

    //////////////////// FIXME: Input fields limited to 4 numbers...
    precision /= 1.E6;

    if (precision == 0)
    {
        precision = 1.E-300;
    }
    ////////////////////////////////////////////////////////////////

    // Use the given parameters
    ParameterSet* params_pre = input;

    // First iteration : start from a priori values
    ParameterSet* params_ind(params_pre);

    // Set phi_new to the maximum value possible, as we are minimizing the function.
    phi_new = std::numeric_limits<double>::max();

    double deltas[params_pre->size()];
    for(int i=0;i<params_pre->size();i++)
        deltas[i]=0.0;

    int nb_iter;
    int nb_tot;
    int nb_loops = 0;

    double last_diff;

    //Scoubi-
    do {
        nb_loops++;
//        qDebug() << QString("deps: %1. eps: %2").arg(deps).arg(eps);
        nb_tot=0;
        do { // ! Says Homer
            double phi0;

            // Clone the parameters as we will be adapting them individually, starting
            // from the base parameters
            ParameterSet* params_loop(params_ind);

            // Gradient descent
            phi_old = phi_new;

            /* Algo: 1) calculate phi at the current position
             *       2) for each parameter which has a deviation,
             *          2.1) calculate the derivative,
             *          2.2) then phi_i
             *          2.3) update the parameter value by incrementing it by phi_i*epsilon
             *       3) compute the new phi value, from the updated parameters
             */
            phi0 = phi(measures, measureTimes.data(), params_loop, params_pre);

            int i=0;
            foreach (QString pName, params_pre->pids()) {
                SharedParameter ppop = params_pre->get(pName);

                if ((ppop->standard() != 0)||(ppop->proportional() != 0)) {
                    double phi_i;
                    const double H = 0.000000001;

                    // Clone parameter list from the clean one
                    ParameterSet* params_tmp(params_loop);

                    // Modify the current parameter being derivated
                    (*params_tmp)[ppop->name()]->setValue((*params_loop)[ppop->name()]->value() + H);

                    //Check for division by 0
                    Q_ASSERT(H != 0.0);

                    // Compute its derivation
                    phi_i = (phi(measures,measureTimes.data(), params_tmp, params_pre) - phi0) / H;

                    deltas[i] = -phi_i*eps;
              //      qDebug() << QString("phi_i*eps: %1").arg(phi_i*eps);
                }
                i++;
            }

            last_diff=std::numeric_limits<double>::max();

            // We follow the gradient while we go down
            nb_iter=0;
            phi_new=phi0;

            ParameterSet* last_params;

            do {

                last_params->copy(params_ind);

                int i=0;
                nb_iter++;
                foreach (QString pName, params_pre->pids()) {
                    SharedParameter ppop = params_pre->get(pName);


                    // update the current parameter value, in the final parameter list
                    // Multiply the step by the median pop value, to accomodate order-of-magnitutde differences.
                    (*params_ind)[ppop->name()]->setValue((*params_ind)[ppop->name()]->value() + deltas[i]);

                    // We have to ensure that the parameter stays positive, for
                    // the Log to be potentially applied
                    if ((*params_ind)[ppop->name()]->value() <= 0.0)
                        (*params_ind)[ppop->name()]->setValue(0.0000000001);

                    i++;
                }
                last_diff=phi_old-phi_new;
                phi_old=phi_new;
                // Compute the new phi, with all the parameters adapted.
                phi_new = phi(measures,measureTimes.data(), params_ind, params_pre);
            } while ((phi_new<phi_old)&&(fabs(phi_new-phi_old)>.0001));

            // We go back for 1 step
            {
                int i=0;
                foreach (QString pName, params_pre->pids()) {
                    SharedParameter ppop = params_pre->get(pName);
                    (*params_ind)[ppop->name()]->setValue((*params_ind)[ppop->name()]->value() - deltas[i]);

                    // We have to ensure that the parameter stays positive, for
                    // the Log to be potentially applied
                    if ((*params_ind)[ppop->name()]->value() <= 0.0)
                        (*params_ind)[ppop->name()]->setValue(0.0000000001);

                    i++;
                }

                phi_new = phi_old;
            }

            params_ind->copy(last_params);
         //   qDebug() << QString("nb iter: %1").arg(nb_iter);

         //   qDebug() << QString("phi_new: %1").arg(phi_new);
         //   qDebug() << QString("eps: %1").arg(eps);

         //   qDebug() << QString("lastdiff: %1").arg(last_diff);

            nb_tot++;
            if (nb_tot==10) {
                nb_tot=0;
                eps/=deps;
            }

        } while((nb_iter!=1)&&(fabs(last_diff) > precision));


        // reduce the espilon step by the delta given
        eps *= deps;

    } while((eps > precision)&&(nb_loops<1000));//&&(fabs(last_diff) > precision));

//    foreach (QString pName, params_pre.pids()) {
//        Parameter ppop = params_pre.get(pName);
//        Parameter pind = params_ind.get(pName);
//        qDebug() << QString("Param %1: %2, %3. Diff: %4)
//              .arg(pName)
//              .arg(ppop.value)
//              .arg(pind.value)
//              .arg(ppop.value-pind.value));
//    }

//    foreach (QString pName, params_ind.pids()) {
//        if (pName.compare("CL")==0){
//            Parameter ppop = params_ind.get(pName);
//            //params_ind[ppop.pid].value =12.2;
//        }
//    }

    return params_ind;
}

//Magic, I suppose
double Bayesian::phi(const QList<SharedMeasure> &measures, Duration *measureTimes, ParameterSet* params_ind, ParameterSet* params_p0)
{
    double phi = 0;

    if (!ignoreResidual) {
        double Cpreds[measures.size()];

        double Sprop = params_ind->globalProportional();
        double Sadd = params_ind->globalAdditive();

        // Compute Cpreds only if Sprop and/or Sadd are defined.
        if((Sprop != 0.0) || (Sadd != 0.0))
        {
            m_pred->analysis()->setParameters(m_pred, params_ind, TMP);
            for( int i = 0; i < measures.size(); i++)
                Cpreds[i] = m_pred->analysis()->point(m_pred, measureTimes[i], TMP).value();
        }

        // Ignore Sprop and/or Sadd if they are null.
        if (Sprop != 0)
        {
            for(int i = 0; i < measures.size(); i++)
            {
                if ((measures[i]->amount().value()>0.0) && (Cpreds[i]>0.0)) {
                    double c = (log(measures[i]->amount().value()) - log(Cpreds[i]));
                    phi += (c*c)/(Sprop);
                }
            }
        }

        if (Sadd != 0)
        {
            for( int i = 0; i < measures.size(); i++)
            {
                double d = (measures[i]->amount().value() - Cpreds[i]);
                phi += (d * d)/(Sadd);
            }
        }
    }
//    qDebug() << QString("phi1 :: %1").arg(phi);

    if (!ignoreParameters) {
        // Compute phi, ignore any parameters whose standard deviation is null.
        foreach (QString pName, params_ind->pids()) {
            SharedParameter pi = params_ind->get(pName);
            SharedParameter p0 = params_p0->get(pName);


            if (pi->value() > 0.0)
                if (p0->proportional() != 0.0) {
                    phi += 2*log(pi->value());
                }
      //      qDebug() << QString("phi2 :: %1").arg(phi);

            if ( p0->value() != -1.0 && p0->standard() != 0.0 ) {
                double a = 0.0;
                //distribution::Normal:
                a = (p0->value() - pi->value());
                phi += ((a * a) / (p0->standard()));
        //        qDebug() << QString("phi: %1,%2").arg(a).arg(p0.standard);

            }

          //  qDebug() << QString("phi3 :: %1").arg(phi);

            if ( p0->value() != -1.0 && p0->proportional() != 0.0 ) {
                double a = 0.0;
                //distribution::LogNormal:
                if ((p0->value() > 0.0) && (pi->value() > 0.0)) {
                    a = (log(p0->value()) - log(pi->value()));

                    phi += ((a * a) / (p0->proportional()));
                }
            }
         //   qDebug() << QString("phi4 :: %1").arg(phi);

        }
    }
   // qDebug() << QString("phi5 :: %1").arg(phi);

    return phi;
}

