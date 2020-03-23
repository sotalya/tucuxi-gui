#include "gradient.h"

#include "coremessagehandler.h"
#include "core_errors.h"
#include "units.h"
#include "dataset.h"
#include "prediction.h"

#include <math.h>
#include <limits>
#include <stdlib.h>
#include <QtPlugin>
#include <cfloat>
#include <QDebug>

#define SQRT5 2.236067977499789696
#define ERRORWRESTARGET 1.0
#define ERRORWPEAKTARGET 1.0

void GradientReverseFactory::init(Core *core)
{
    //Set the core
    Core::setInstance(core);

    //Store the engines priorities
    _priorities.insert(descriptor().id, GradientReverse().priority());
}

ReverseEngine *GradientReverseFactory::newReverseEngine()
{
    return new GradientReverse();
}

Descriptor GradientReverseFactory::descriptor()
{
    return Descriptor(GRADIENT_ID,
                      tr("Continuous CSP"),
                      tr("Continuous reverse calculation using the Gradient descent method."));
}

int GradientReverseFactory::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}


//--------------------------------------- ENGINE -----------------------------------------


// Options OID definition
#define KEEP_THE_N_BEST "0n_best"

//The engine default priority level
const int GradientReverse::_priority = 20;

GradientReverse::GradientReverse(): args(NULL), alpha(0.5 * (3.0 - SQRT5))
{
    addOption(KEEP_THE_N_BEST, tr("Keep n best dosage"), tr(""), QVariant::Int, QVariant(4));
}

// New instance, and copy the options if asked
GradientReverse * GradientReverse::clone()
{
   //New instance
   GradientReverse *instance = new GradientReverse();

   //Copy the options
   instance->copyOptions(this);

   return instance;
}

Descriptor GradientReverse::descriptor()
{
   return Descriptor("ch.heig-vd.ezechiel.gradient",
                     tr("Continuous CSP"),
                     tr("Continuous reverse calculation using the Gradient descent method."));
}

int GradientReverse::priority() const
{
    return _priority;
}

Version GradientReverse::version()
{
   return Version("0.1");
}

QString GradientReverse::errorString ()
{
    return QString();
}

void GradientReverse::setAnalysis(SharedDrugResponseAnalysis model)
{
    predictor = model;
}

double GradientReverse::scoring(const Duration &at, const ParameterType type, const SharedDosage &dosage, const SharedTargetSet &targets, QHash<Target::TargetType, double> errorWeight)
{
    double residualWanted = -1.0;
    double peakWanted = -1.0;
    Duration peakTime;

    // ToDo ////////////////////////////////////////////////////////
    // Below, we need to return an "invalid" or bad score if the at
    // value, which corresponds to the steady state for the current
    // dosage being tested, is null (no steady state found). We do
    // not know what score to return to indicate this state, and
    // returning a max value seems to have an impact on the value of
    // the next test dosages (very high value), which lead to a crash
    // in the curve model (NaN detected). This need to be fixed.
    // /////////////////////////////////////////////////////////////

    /*Return worst possible score if at is empty (no steady state)*/
    if (at.isEmpty())
        return std::numeric_limits<double>::max();

    foreach (Target* t, *targets.data()){
       switch(t->type()){
       case Target::ResidualTarget:
           residualWanted = t->cbest().value();
           break;
       case Target::PeakTarget:
           peakWanted = t->cbest().value();
           peakTime = t->tbest();
           break;
       default:;
       }
    }

    double peakValue;
    double peakDiff = 0.0;
    double residualValue;
    double residualDiff = 0.0;

    //If peak target found
    if (peakWanted != -1.0) {

        //Get the peak concentration and difference
        peakValue = predictor->point(SharedPrediction(), at + peakTime, type, dosage).value();
        peakDiff = fabs(peakValue - peakWanted);

        //Test for peak value not in interval, applic penality
        //if(peakValue > peakMax || peakValue < peakMin)
            //peakDiff *= 4;
    } else //Else set the coefficient to 0.0
        errorWeight.insert(Target::PeakTarget, 0.0);

    //If residual target found
    if (residualWanted != -1.0) {

        //Get the residual concentration and difference
        residualValue = predictor->point(SharedPrediction(), at + dosage->interval(), type, dosage).value();
        residualDiff = fabs(residualValue - residualWanted);

        //Test for residual value not in interval, applic penality
        //if(residualValue > residualMax || residualValue < residualMin)
            //residualDiff *= 4;
    } else //Else set the coefficient to 0.0
        errorWeight.insert(Target::ResidualTarget, 0.0);

    //ToDo: calcul a score, with coef and diff values
    return (peakDiff * errorWeight.value(Target::PeakTarget, 0.0)) + (residualDiff * errorWeight.value(Target::ResidualTarget, 0.0));
}

bool GradientReverse::checkForValidSolution(const SharedDosage &solution, double residualMin, double residualMax, double peakMin, double peakMax, const Duration &peakTime) {

    /*If no valid dosage, not a valid solution*/
    if(solution->interval().toSeconds() < 0.0 || solution->dose().value() < 0.0)
        return false;

    /*If no correct interval, not a valid solution*/
    if((residualMin < 0.0 || residualMax < 0.0) && (peakMin < 0.0 || peakMax < 0.0))
        return false;

    double peakValue;
    double residualValue;

    Duration steady = predictor->steadyState(m_pred, start, type, solution);

    /*If no steady state, not a valid solution*/
    if (steady.isEmpty())
        return false;

    //Test for peak in interval
    if (peakMin >= 0.0 && peakMax >= 0.0) {

        //Get the peak cencentration
        peakValue = predictor->point(m_pred, steady + peakTime, type, solution).value();

        //Test for peak value in interval
        if (peakValue > peakMax || peakValue < peakMin)
            return false;
    }

    //Test for residual in interval
    if(residualMin >= 0.0 && residualMax >= 0.0){

        //Get the residual concentration
        residualValue = predictor->point(m_pred, steady + solution->interval(), type, solution).value();

        //Test for residual in interval
        if(residualValue > residualMax || residualValue < residualMin)
            return false;
    }

    return true;
}

QMap<int, SharedDosage> GradientReverse::reverseDosages(Prediction* _pred, const SharedDosage &currentDosage, const Duration &start, const SharedTargetSet &targets, const QList<Duration> &, const QList<Duration> &availableInfusions, const QList<Amount> &, ParameterType type, int nbIntakes)
{
    Q_UNUSED(availableInfusions);
    Q_UNUSED(nbIntakes);

    /*Fetch the options*/
    const int keepNBest = option(KEEP_THE_N_BEST).value.toInt();
    m_pred = _pred;
    QMap<int,SharedDosage> result;

    /*Get the targets min max limit*/
    double residualMax = -1.0;
    double residualMin = -1.0;
    double peakMin = -1.0;
    double peakMax = -1.0;
    Duration peakTime;
    foreach (Target* t, *targets.data()){
       switch(t->type()){
       case Target::ResidualTarget:
           residualMax = t->cmax().value();
           residualMin = t->cmin().value();
           break;
       case Target::PeakTarget:
           peakMax = t->cmax().value();
           peakMin = t->cmin().value();
           peakTime = t->tbest();
           break;
       default:;
       }
    }
    //If no correct type target is defined return empty result
    if((residualMin < 0.0 || residualMax < 0.0) && (peakMin < 0.0 || peakMax < 0.0))
        return result;

    /*For debugging purpose*/
    /*
    QFile file("Gradient.txt");
    QTextStream fileStream(&file);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    fileStream << "Dosage,Interval,Fitness" << endl;
    */

    QMap<double, SharedDosage> foundPosologies;

    //Error weight for fitness comouting
    QHash<Target::TargetType, double> errorWeight;
    errorWeight.insert(Target::ResidualTarget, ERRORWRESTARGET);
    errorWeight.insert(Target::PeakTarget, ERRORWPEAKTARGET);

    double a[2] = {1.0,1.0};
    double fa;
    double dfa[2];
    Duration steady;

    this->type = type;
    this->start = start;
    dosage = currentDosage;
    this->targets = targets;

    dosage->setApplied(predictor->treatment()->dosages()->firsttake().addSecs(start.seconds()));

    for(int time = 5; time <= 25; time+=5)
        for(int dose = 100; dose <= 1000; dose+=200){
            modelCall = 0;
            a[0] = static_cast<double>(dose);
            a[1] = static_cast<double>(time);

            /*Initial calcul of f(a[])*/
            dosage->dose().setValue(a[0]);
            dosage->setInterval(Duration(time));
            steady = predictor->steadyState(_pred, start, type, dosage);

            /*Ignore if no steady state*/
            if (steady.isEmpty())
                continue;

            modelCall++;
            fa = scoring(steady, this->type, dosage, targets, errorWeight);


            /*error =*/ Steepest_Descent(&GradientReverse::funToOptimizeWraper, &GradientReverse::gradientFun,
                                     &GradientReverse::stoppingRule, a, &fa, dfa, 1.0,
                                     1.0, 0.0001, 2);


            dosage->dose().setValue(a[0]);

            double interval = a[1];

            qint64 h = static_cast<qint64>(floor(interval));
            interval = (interval - h)*60;
            qint64 m = static_cast<qint64>(floor(interval));
            qint64 s = static_cast<qint64>(floor((interval  - m) * 60));
            qint64 ms = 0;

            dosage->setInterval(Duration(h, m, s, ms));

            /*Keep solution if she fit the interval*/
            if(checkForValidSolution(dosage, residualMin, residualMax, peakMin, peakMax, peakTime))
                foundPosologies.insertMulti(fa, dosage);

            //qDebug() << "Return code :" << error;
            //qDebug() << "Dosage : " << a[0] << " Interval : " << a[1] << " Fitness : " << fa << "Model_Call : " << modelCall << endl;
            //fileStream << a[0] << ',' << a[1] << ',' << fa << endl;
        }

    //file.close();

    int kept = 0;
    /*Use an iterator to loop through the map*/
    QMap<double, SharedDosage>::ConstIterator i = foundPosologies.constBegin();
    while(i != foundPosologies.constEnd()){
        //Keep the n best SharedDosage to return
        if (kept < keepNBest)
            result.insert(kept + 1, i.value());
        else
            break;

        kept++;
        ++i;
    }

    return result;
}

double GradientReverse::funToOptimizeWraper(double *x){
    QHash<Target::TargetType, double> errorWeight;
    errorWeight.insert(Target::ResidualTarget, ERRORWRESTARGET);
    errorWeight.insert(Target::PeakTarget, ERRORWPEAKTARGET);

    //qDebug() << "USAGE Dose :" << x[0] << " Interval :" << x[1];

    SharedDosage locDosage = dosage;

    locDosage->dose().setValue(x[0]);

    double interval = x[1];

    if (interval < 1.0)
        interval = 1.0;
    if (locDosage->dose().value() < 1.0)
        locDosage->dose().setValue(1.0);

    qint64 h = static_cast<qint64>(floor(interval));
    interval = (interval - h)*60;
    qint64 m = static_cast<qint64>(floor(interval));
    qint64 s = static_cast<qint64>(floor((interval  - m) * 60));
    qint64 ms = 0;

    locDosage->setInterval(Duration(h, m, s, ms));

    Duration steady = predictor->steadyState(m_pred, start, type, locDosage);

    modelCall++;
    double score = scoring(steady, type, locDosage, targets, errorWeight);

    return score;
}

void GradientReverse::gradientFun(double *x, double *grad){

    Duration steady;
    SharedDosage locDosage = dosage;

    QHash<Target::TargetType, double> errorWeight;
    errorWeight.insert(Target::ResidualTarget, ERRORWRESTARGET);
    errorWeight.insert(Target::PeakTarget, ERRORWPEAKTARGET);

    locDosage->dose().setValue(x[0]);

    double interval = x[1];

    if (interval < 1.0)
        interval = 1.0;
    if (locDosage->dose().value() < 1.0)
        locDosage->dose().setValue(1.0);

    qint64 h = static_cast<qint64>(floor(interval));
    interval = (interval - h)*60;
    qint64 m = static_cast<qint64>(floor(interval));
    qint64 s = static_cast<qint64>(floor((interval  - m) * 60));
    qint64 ms = 0;

    locDosage->setInterval(Duration(h, m, s, ms));

    const Duration keepInterval = locDosage->interval();

    //Central difference method to evaluate first derivative and the gradient

    //Evalutate the derivative for the interval
    locDosage->interval().addMinutes(1);
    steady = predictor->steadyState(m_pred, start, type, locDosage);
    modelCall++;
    double f1 = scoring(steady, type, locDosage, targets, errorWeight);

    locDosage->setInterval(Duration(0, locDosage->interval().minutes() - 2, 0, 0));
    steady = predictor->steadyState(m_pred, start, type, locDosage);
    modelCall++;
    double fm1 = scoring(steady, type, locDosage, targets, errorWeight);

    double dfx2 = (f1 - fm1)/2;

    //Evaluate the derivative for the dosage
    locDosage->setInterval(keepInterval);
    locDosage->dose().setValue(locDosage->dose().value() + 1);
    steady = predictor->steadyState(m_pred, start, type, locDosage);
    modelCall++;
    f1 = scoring(steady, type, locDosage, targets, errorWeight);

    locDosage->dose().setValue(locDosage->dose().value() - 2);
    steady = predictor->steadyState(m_pred, start, type, locDosage);
    modelCall++;
    fm1 = scoring(steady, type, locDosage, targets, errorWeight);

    double dfx1 = (f1 - fm1)/2;

    grad[0] = dfx1;
    grad[1] = dfx2;
}

int GradientReverse::stoppingRule(double *, double funValue, double *, double newFunValue, double *, int iteration , int ){
    //qDebug() << "iteration : " << iteration << " funValue : " << funValue << "newFunValue : " << newFunValue;

    if (fabs(funValue - newFunValue) < 0.001)
        return 1;
    if(iteration >= 500)
        return 2;

    return 0;
}

#if 1

/*----------------------------------Steepest_descent---------------------------------------*/
/*The following code is from Mathematics Source Library C & ASM (www.mymathlib.com)*/
/*Modified to fit this class and C++ (JRP, IICT)*/


////////////////////////////////////////////////////////////////////////////////
//  int  Steepest_Descent(double (*f)(double *),                              //
//          void (*df)(double *, double *),                                   //
//          int (*stopping_rule)(double*, double, double*, double, double*,   //
//                                                                 int, int), //
//          double a[], double *fa, double *dfa, double cutoff,               //
//          double cutoff_scale_factor, double tolerance, int n)              //
//                                                                            //
//  Description:                                                              //
//     Given a nonlinear function f:R^n -> R, the optimal steepest descent    //
//     method searches for the minimum of f by starting at a predetermined    //
//     point x and then following the line from x in the direction opposite   //
//     to that given by grad f evaluated at x until a minimum on the line is  //
//     reached.  The process is repeated until halted by the stopping rule.   //
//     The steepest descent method is best used when the point x is far away  //
//     from the minimum.  Near the minimum, the norm of grad f is small and   //
//     convergence is slow and another method should be used accelerate       //
//     convergence to the final approximation of the minimum.                 //
//                                                                            //
//     It is possible that if the initial point happens to be the location of //
//     a local maximum or a saddle point, the gradient will vanish and the    //
//     procedure will fail.                                                   //
//     It is also possible that the method will converge to a saddle point    //
//     so that the result should be checked and if it is a saddle point then  //
//     try a different initial point.                                         //
//                                                                            //
//  Arguments:                                                                //
//     double (*f)(double *)                                                  //
//        Pointer to a user-defined function of a real n-dimensional vector   //
//        returning a real number (type double).                              //
//     void   (*df)(double *, double *)                                       //
//        Pointer to a user-defined function which returns the gradient of the//
//        function f, above, in the second argument evaluated at the point    //
//        in the first argument.  I.e. df(x,dfx), where dfx = grad f(x).      //
//     int (*stopping_rule)(double*, double, double*, double, double*, int,   //
//                                                                      int)  //
//        Pointer to a user-defined function which controls the iteration of  //
//        the steepest descent method.  If the stopping rule is non-zero the  //
//        process is halted and if zero then the process continues iterating. //
//        Steepest descent converges slowly in the neighborhood of an         //
//        extremum and another method is then called to converge to the final //
//        solution.                                                           //
//        The arguments are: original point, the value of the function f      //
//        evaluated at the original point, the new point, the value of the    //
//        function evaluated at the new point, the gradient of the function   //
//        at the new point, the total number of iterations performed and the  //
//        dimension, n, if a point and the gradient.                          //
//     double *a                                                              //
//        On input, a is the initial point to start the descent.  On output,` //
//        a is the final point before the iteration is halted.                //
//     double *fa                                                             //
//        On input and output, *fa is the value of f(a[]).                    //
//     double *dfa                                                            //
//        Working storage used to hold the gradient of f at a. Should be      //
//        dimensioned at least n in the calling routine.                      //
//     double cutoff                                                          //
//        The upper bound of the parameter p during the line search where     //
//        the line is x - pv, v is the gradient of f at x, 0 <= p <= cutoff.  //
//     double cutoff_scale_factor                                             //
//        A parameter which limits the displacement in any single step during //
//        the parabolic extrapolation phase of the line search.               //
//     double tolerance                                                       //
//        A parameter which controls the termination of the line search.      //
//        The line search is terminated when the relative length of the       //
//        interval of uncertainty to the magnitude of its mid-point is less   //
//        than or equal to tolerance.  If a nonpositive number is passed,     //
//        tolerance is set to sqrt(DBL_EPSILON).                              //
//     int    n                                                               //
//        On input, n is the dimension of a and dfa.                          //
//                                                                            //
//  Return Values:                                                            //
//     0   Success                                                            //
//    -1   In the line search three points are collinear.                     //
//    -2   In the line search the extremum of the parabola through the three  //
//         points is a maximum.                                               //
//    -3   Int the line search the initial points failed to satisfy the       //
//         condition that x1 < x2 < x3 and fx1 > fx2 < fx3.                   //
//    -4   Not enough memory.                                                 //
//    -5   The gradient evaluated at the initial point vanishes.              //
//   Other user specified return from stopping_rule().                        //
//                                                                            //
//  Example:                                                                  //
//     extern double f(double*);                                              //
//     extern void df(double*, double*);                                      //
//     extern int Stopping_Rule(double*, double, double*, double, double*,    //
//                                                                 int, int); //
//     #define N                                                              //
//     double cutoff, cutoff_scale_factor, tolerance;                         //
//     double a[N], dfa[N];                                                   //
//                                                                            //
//     (your code to initialize the vector a, set cutoff, )                   //
//     (cutoff_scale_factor, and tolerance.               )                   //
//                                                                            //
//     err = Steepest_Descent( f, df, Stopping_Rule, a, fa, dfa, cutoff,      //
//                cutoff_scale_factor, tolerance, N);                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int  GradientReverse::Steepest_Descent(double (GradientReverse::*f)(double *), void (GradientReverse::*df)(double *, double *),
      int (GradientReverse::*stopping_rule)(double*, double, double*, double, double*, int, int),
                          double a[], double *fa, double *dfa, double cutoff,
                          double cutoff_scale_factor, double tolerance, int n)
{
   double* x;
   double* initial_a = a;
   double* tmp;
   double fx;
   double p;
   int err = 0;
   int iteration = 0;

   x = (double *) malloc(n * sizeof(double));
   if ( x == NULL) { err = -3; }

   (this->*df)(a, dfa);
   if (Vector_Max_Norm(dfa,n) == 0.0) err = -5;
   while (!err) {
      if (Vector_Max_Norm(dfa,n) == 0.0) break;
      err = Minimize_Down_the_Line(f, a, *fa, &p, dfa, x, cutoff,
                                            cutoff_scale_factor, tolerance, n);
      if ( err ) break;
      fx = (this->*f)(x);
      iteration++;
      (this->*df)(x, dfa);
      err = (this->*stopping_rule)( a, *fa, x, fx, dfa, iteration, n);
      *fa = fx;
      tmp = a;
      a = x;
      x = tmp;
   }
   if (a != initial_a ) {
      Copy_Vector(initial_a, a, n);
      x = a;
   }
   free(x);
   return err;
}

void GradientReverse::Copy_Vector(double *d, double *s, int n)
{
   for (; n > 0; n--) *d++ = *s++;
}

////////////////////////////////////////////////////////////////////////////////
//  double Vector_Max_Norm(double v[], int n)                                 //
//                                                                            //
//  Description:                                                              //
//     Given an n-d real vector space V with a basis consisting of vectors    //
//     with unit norm, the max norm on the vector space V is the maximum of   //
//     the absolute values of the components of a vector v with respect to    //
//     that basis i.e. for v = v[0]e[0] + ... + v[n-1]e[n-1], where e[0], ...,//
//     e[n-1] are the basis vectors for which                                 //
//     || e[0] || = ... = || e[n-1] || = 1, then                              //
//                   || v || = Max( |v[0]|, ..., |v[n-1]| ).                  //
//                                                                            //
//  Arguments:                                                                //
//     double v[]  Pointer to the first element of the vector v[n].           //
//     int     n   The number of elements of the vector v[].                  //
//                                                                            //
//  Return Values:                                                            //
//     max norm of v.                                                         //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double v[N], norm;                                                     //
//                                                                            //
//     (your code to initialize the vector v)                                 //
//     norm = Vector_Max_Norm(v, N);                                          //
//     printf(" || v || = %12.6f\n", norm);                                   //
//           ...                                                              //
////////////////////////////////////////////////////////////////////////////////
double GradientReverse::Vector_Max_Norm(double v[], int n)
{
   double norm = 0.0;
   double x;
   int i;

   for (i = 0; i < n; i++) if (norm < ( x = fabs( v[i] ) ) ) norm = x;

   return norm;
}

////////////////////////////////////////////////////////////////////////////////
//  int Minimize_Down_the_Line(double (*f)(double *), double x[], double fx,  //
//                       double *p, double v, double y[], double cutoff,      //
//                       double cutoff_scale_factor, double tolerance, int n) //
//                                                                            //
//  Description:                                                              //
//     Given a function f:R^n -> R, a point x in R^n, and a direction v       //
//     this function minimizes g(p) = f(x - pv) for 0 < p <= cutoff.  In      //
//     particular, if v is the gradient of f, then as long as v != 0, this    //
//     function searches for a minimum of f in the direction opposite to that //
//     of the direction of the grad f.                                        //
//                                                                            //
//     This routine uses parabolic extrapolation to bracket a local minimum   //
//     followed by parabolic interpolation to refine the estimate.            //
//                                                                            //
//  Arguments:                                                                //
//     double (*f)(double *)                                                  //
//        Pointer to a user-defined function of a real n-dimensional vector   //
//        returning a real number (type double).                              //
//     double x[]                                                             //
//        On input, x[] is the initial point to begin the search for a        //
//        minimum of f(x - pv) with respect to p for p > 0.  The array x      //
//        should be dimensioned n in the calling routine.                     //
//     double fx                                                              //
//        On input, fx is the value of f(x[]).                                //
//     double *p                                                              //
//        On output, *p is the value of p which minimizes f(x - pv) for p > 0.//
//     double v[]                                                             //
//        On input, v is an "unnormalized" direction.  The search starts at   //
//        the point x in the direction -v.  The array v should be dimensioned //
//        n in the calling routine.                                           //
//     double y[]                                                             //
//        On output, y is the point x - pv where 0 < p <= cutoff is the value //
//        of p which minimizes f(x - pv).  The array y should be dimensioned  //
//        n in the calling routine.                                           //
//     double cutoff                                                          //
//        The upper bound of the parameter p.                                 //
//     double cutoff_scale_factor                                             //
//        A parameter which limits the displacement in any single step during //
//        the parabolic extrapolation phase.                                  //
//     double tolerance                                                       //
//        A parameter which controls the termination of the procedure.        //
//        The procedure is terminated when the relative length of the interval//
//        of uncertainty to the magnitude of its mid-point is less than or    //
//        equal to tolerance.  If a nonpositive number is passed, tolerance   //
//        is set to sqrt(DBL_EPSILON).                                        //
//     int    n                                                               //
//        On input, n is the dimension of x, v, and y.                        //
//                                                                            //
//  Return Values:                                                            //
//                                                                            //
//  Example:                                                                  //
//     extern double f(double*);                                              //
//     #define N                                                              //
//     double x[N], double v[N], double y[N];                                 //
//     double p, fx;                                                          //
//     double cutoff, cutoff_scale_factor, tolerance;                         //
//                                                                            //
//     (your code to create vectors x, v, evaluate fx = f(x), set cutoff,)    //
//     (cutoff_scale_factor and the tolerance.                           )    //
//                                                                            //
//     Minimize_Down_the_Line(f, x, fx, &p, v, y, cutoff, cutoff_scale_factor,//
//                                                             tolerance, n); //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
int GradientReverse::Minimize_Down_the_Line(double (GradientReverse::*f)(double *), double x[], double fx,
                          double *p, double v[], double y[], double cutoff,
                          double cutoff_scale_factor, double tolerance, int n)
{
   struct arg_list g_args;
   struct arg_list *old_args;
   double lambda[3];
   double g_lambda[3];
   int k = 0;

   g_args.x = x;
   g_args.v = v;
   g_args.f = f;
   g_args.y = y;
   g_args.n = n;

                            // Bracket the minimum

   lambda[0] = 0.0;
   lambda[2] = 1.0;
   old_args = args;
   args = &g_args;
   g_lambda[0] = fx;
   g_lambda[2] = g(lambda[2]);
   *p = (g_lambda[0] < g_lambda[2]) ? 0.0 : 1.0;
   args = &g_args;
   k = Min_Search_Parabolic_Extrapolation(&GradientReverse::g, lambda, g_lambda, 0.0, cutoff, cutoff_scale_factor);
   if ( k < 0 ) k = 0;
   else {
                        // Interpolate for the minimum
     // The minimum is located between lambda[0] < lambda[1] < lambda[2],
     // where g(lambda[1]) < g(lambda[0]) and g(lambda[1]) < g(lambda[2]),
     // i.e. f(x - lambda[0]*v) > f(x - lambda[1]*v) < f(x - lambda[2]*v).

      if (tolerance <= 0.0) tolerance = sqrt(DBL_EPSILON);
      args = &g_args;
      k = Min_Search_Parabolic_Interpolation(&GradientReverse::g, lambda, g_lambda, tolerance );
      *p = 0.5 * (lambda[0] + lambda[2]);
   }

   Ray_in_n_Space(y, x, v, -(*p), n );
   args = old_args;
   return k;
}


////////////////////////////////////////////////////////////////////////////////
//  static double g( double lambda )                                          //
//                                                                            //
//  Description:                                                              //
//                                                                            //
//  Return Values:                                                            //
//     g(lambda) = f(x - lambda * v)                                          //
////////////////////////////////////////////////////////////////////////////////
double GradientReverse::g( double lambda )
{
   Ray_in_n_Space(args->y, args->x, args->v, -lambda, args->n );

   return (this->*args->f)(args->y);
}

////////////////////////////////////////////////////////////////////////////////
//  int Min_Search_Parabolic_Extrapolation(double (*f)(double), double x[],   //
//            double fx[], double a, double b,  double cut_off_scale_factor)  //
//                                                                            //
//  Description:                                                              //
//     This routine can be used to bracket the minimum of a unimodal function //
//     or a local minimum of a multimodal function within a preassigned       //
//     closed interval.                                                       //
//                                                                            //
//     Given points (x[0], f(x[0])) and (x[2], f(x[2])) with                  //
//     a <= x[0] < x[2] <= b, the parabolic extrapolation method brackets a   //
//     local minimum of f(x).                                                 //
//                                                                            //
//  Arguments:                                                                //
//     double (*f)(double)                                                    //
//        Pointer to a user-defined function of a real variable (type double) //
//        returning a real number (type double).                              //
//     double x[]                                                             //
//        On input, x[0] and x[2] are defined as two initial starting points  //
//        where a < x[0], x[2] < b.  On output, x[0] is the lower endpoint of //
//        the final subinterval which contains the local minimum, x[2] is the //
//        upper endpoint and x[1] is a point between x[0] and x[2].           //
//     double fx[]                                                            //
//        On input, fx[0] is the value of f(x) evaluated at x[0] and fx[2] is //
//        the value of f(x) evaluated at x[2].  On output, fx[i] is the value //
//        of f(x) evaluated at the final value of x[i], i = 0,1,2.            //
//     double a                                                               //
//        The lower bound of the interval on which the function f is defined. //
//        If a is -infinity, then set a = -DBL_MAX where DBL_MAX is defined   //
//        in <float.h>.                                                       //
//     double b                                                               //
//        The upper bound of the interval on which the function f is defined. //
//        If b is infinity, then set b = DBL_MAX where DBL_MAX is defined in  //
//        <float.h>.                                                          //
//     double cutoff_scale_factor                                             //
//        A parameter which limits the displacement in any single step.       //
//                                                                            //
//  Return Values:                                                            //
//      0  Success                                                            //
//     -1  Process failed to find a value x such that a local minimum is      //
//         found in the interval [a, b], i.e. the minimum is probably located //
//         on the boundary.                                                   //
//     -2  Process failed, the initial values of x[0] and x[2] are equal.     //
//                                                                            //
//  Example:                                                                  //
//     #include <float.h>                                                     //
//     extern double f(double);                                               //
//     double a = -DBL_MAX;                                                   //
//     double b = DBL_MAX;                                                    //
//     double x[3];                                                           //
//     double fx[3];                                                          //
//     double cut_off_scale_factor = 10.0;                                    //
//     int err;                                                               //
//                                                                            //
//     (set x[0], x[2] )                                                      //
//                                                                            //
//     fx[0] = f(x[0]);                                                       //
//     fx[2] = f(x[2]);                                                       //
//                                                                            //
//     err = Min_Search_Parabolic_Extrapolation(f, x, fx, a, b,               //
//                                                     cut_off_scale_factor)  //
//     if (err < 0 ) printf("No local minimum found\n");                      //
//     else printf("local minimum is between (%12.6e,%12.6e) and              //
//                                (%12.6e,%12.6e)\n", x[0],fx[0],x[2],fx[2]); //
////////////////////////////////////////////////////////////////////////////////

int GradientReverse::Min_Search_Parabolic_Extrapolation(double (GradientReverse::*f)(double), double x[],
            double fx[], double a, double b,  double cut_off_scale_factor)
{
   double xmin, ymin;
   int return_code = 0;

   min_displacement = sqrt(DBL_EPSILON);

                          // Verify that x[0] < x[2]

   if ( x[0] == x[2] ) return -2;
   if ( x[0] > x[2] ) {
      x[1] = x[0]; fx[1] = fx[0];
      x[0] = x[2]; fx[0] = fx[2];
      x[2] = x[1]; fx[2] = fx[1];
   }

                   // Set x[1] such that x[0] < x[1] < x[2]

   if ( fx[0] < fx[2] ) x[1] = x[0] + alpha * (x[2] - x[0]);
      x[1] = x[2] - alpha * (x[2] - x[0]);
   fx[1] = (this->*f)(x[1]);

              // Iterate until either f(x[0]) > f(x[1]) < f(x[2])
              // or x[0] == a or x[2] == b.

   while ( ( fx[0] <= fx[1] ) || ( fx[2] <= fx[1] ) ) {
      xmin = New_Test_Point( x, fx, a, b, cut_off_scale_factor );
      if ( (xmin == a) || (xmin == b) ) { return_code = -1; break; }
      ymin = (this->*f)(xmin);
      if ( xmin < x[0] ) {
         x[2] = x[1]; fx[2] = fx[1];
         x[1] = x[0]; fx[1] = fx[0];
         x[0] = xmin; fx[0] = ymin;
      }
      else if ( xmin > x[2] ) {
         x[0] = x[1]; fx[0] = fx[1];
         x[1] = x[2]; fx[1] = fx[2];
         x[2] = xmin; fx[2] = ymin;
      }
      else if ( xmin < x[1] )
         if  (( ymin >= fx[0] ) && ( ymin <= fx[1] )) {
            x[2] = x[1]; fx[2] = fx[1];
            x[1] = xmin; fx[1] = ymin;
         }
         else if ( ymin > fx[1] ) { x[0] = xmin; fx[0] = ymin; break; }
         else { x[2] = x[1]; fx[2] = fx[1]; x[1] = xmin; fx[1] = ymin; break; }
      else
         if  (( ymin >= fx[2] ) && ( ymin <= fx[1] )) {
            x[0] = x[1]; fx[0] = fx[1];
            x[1] = xmin; fx[1] = ymin;
         }
         else if ( ymin > fx[1] ) { x[2] = xmin; fx[2] = ymin; break; }
         else if ( ymin < fx[2] ) { x[1] = xmin; fx[1] = ymin; break; }
   }

   return return_code;
}


////////////////////////////////////////////////////////////////////////////////
//  static double New_Test_Point( double x[], double y[], double a, double b, //
//                     double min_displacement, double cut_off_scale_factor ) //
//                                                                            //
//  Description:                                                              //
//     Given three points (x[0],y[0]), (x[1],y[1]), and (x[2],y[2]) such that //
//     x[0] < x[1] < x[2] find the location of the next test point, normally  //
//     the minimum, xmin, of the parabola through these points.               //
//                                                                            //
//     For C compilers which support the 'inline' type qualifier, this routine//
//     could be declared with the 'inline' qualifier.                         //
//                                                                            //
//  Return Values:                                                            //
//     The next test point.                                                   //
////////////////////////////////////////////////////////////////////////////////
double GradientReverse::New_Test_Point( double x[], double y[], double a, double b,
                                                 double cut_off_scale_factor )
{
   double xmin;
   double eta;

   xmin = Parabolic_Minimum( x, y, cut_off_scale_factor );

                 // Compare the location of the minimum of the
                 // parabolic fit to the left-most node.

   eta = ( x[0] == 0.0 ) ? 1.0 : fabs(x[0]);
   if ( fabs(xmin - x[0]) < (min_displacement * eta) )
      xmin -= 100.0 * min_displacement * eta;
   else {
                 // Compare the location of the minimum of the
                 // parabolic fit to the center node.

      eta = ( x[1] == 0.0 ) ? 1.0 : fabs(x[1]);
      if ( fabs(xmin - x[1]) < (min_displacement * eta) )
         if ( y[2] < y[0] )
            xmin = x[1] + alpha * (x[2] - x[1]);
         else
            xmin = x[1] - alpha * (x[1] - x[0]);
      else {
                 // Compare the location of the minimum of the
                 // parabolic fit to the right-most node.

         eta = ( x[2] == 0.0 ) ? 1.0 : fabs(x[2]);
         if ( fabs(x[2] - xmin) < (min_displacement * eta) )
            xmin += 100.0 * min_displacement * eta;
      }
   }

   if (xmin < a) xmin = a;
   if (xmin > b) xmin = b;

   return xmin;
}


////////////////////////////////////////////////////////////////////////////////
//  static double Parabolic_Minimum( double x[], double y[],                  //
//                                            double cut_off_scale_factor )   //
//                                                                            //
//  Description:                                                              //
//     Given three points (x[0],y[0]), (x[1],y[1]), and (x[2],y[2]) such that //
//     x[0] < x[1] < x[2] and y[1] > the line through (x[0],y[0]) and         //
//     (x[2],y[2]) at x[1] find the location of the minimum, xmin, of the     //
//      parabola through these points.                                        //
//                                                                            //
//     For C compilers which support the 'inline' type qualifier, this routine//
//     could be declared with the 'inline' qualifier.                         //
//                                                                            //
//  Return Values:                                                            //
//     The location of the minimum.                                           //
////////////////////////////////////////////////////////////////////////////////
double GradientReverse::Parabolic_Minimum( double x[], double y[],
                                                  double cut_off_scale_factor )
{
   double d1 = (x[2] - x[1]) * (y[1] - y[0]);
   double d2 = (x[1] - x[0]) * (y[2] - y[1]);
   double denominator = 2.0 * (d1 - d2);
   double numerator = (x[2] + x[1]) * d1 - (x[1] + x[0]) * d2;
   double x_lb = x[0] - cut_off_scale_factor * (x[1] - x[0]);
   double x_ub = x[2] + cut_off_scale_factor * (x[2] - x[1]);
   double ymax =( y[2] - y[0]) * (x[1] - x[0]) / (x[2] - x[0]) + y[0];
   double xmin;

   if (y[1] >= ymax) xmin = ( y[0] < y[2] ) ? x_lb : x_ub;
   else {
      if ( x_lb * denominator <= numerator ) xmin = x_lb;
      else if ( x_ub * denominator >= numerator ) xmin = x_ub;
      else  xmin = numerator / denominator;
   }
   return xmin;
}

////////////////////////////////////////////////////////////////////////////////
//  int Min_Search_Parabolic_Interpolation(double (*f)(double), double* x0,   //
//                                            double* fx0, double tolerance ) //
//                                                                            //
//  Description:                                                              //
//     This routine can be used to locate the minimum of a unimodal function  //
//     within a preassigned tolerance or a local minimum of a multimodal      //
//     function within a preassigned tolerance.  The function should not be   //
//     linear on any closed and bounded subinterval with nonempty interior.   //
//                                                                            //
//     Initially 5 points are defined (x[i], fx[i]), i = 0,1,2,3,4 by setting //
//     x[0] = x[1] = x0[0], x[2] = x0[1], x[3] = x[4] = x0[2] and             //
//     fx[i] = f(x[i]).  The procedure then fits a parabola through the three //
//     interior points.                                                       //
//                                                                            //
//     Given points (x1, f(x1)), (x2, f(x2)), (x3, f(x3)) with x1 < x2 < x3   //
//     and f(x1) > f(x2) < f(x3), the parabolic interpolation method          //
//     estimates a local minimum of f(x) for x1 < x < x3 by repeated          //
//     estimation of the minimum of the parabola through three points.  The   //
//     three points being two points which bound the minimum and the current  //
//     location of the observed minimum.  The iteration is terminated when    //
//     the relative length of the interval bounding the minimum is less than  //
//     or equal to a preassigned tolerance.                                   //
//                                                                            //
//  Arguments:                                                                //
//     double (*f)(double)                                                    //
//        Pointer to a user-defined function of a real variable (type double) //
//        returning a real number (type double).                              //
//     double x0[]                                                            //
//        On input, x0[0] is the initial lower endpoint and x0[2] is the upper//
//        endpoint of an interval which contains a local minimum. x0[1] is    //
//        an interior point between x0[0] and x0[2] such that                 //
//        f(x0[1]) < f(x0[0]) and f(x0[1]) < f(x0[2]).  On output, x0[0] is   //
//        the lower endpoint and x0[2] is the upper endpoint of the final     //
//        subinterval which contains the local minimum.  x0[1] is the current //
//        best estimate of the location of the local minimum.                 //
//     double fx0[]                                                           //
//        On input and on output, fx0[i] is the value of f(x) evaluated at    //
//        x0[i], i=1,2,3.                                                     //
//     double tolerance                                                       //
//        A parameter which controls the termination of the procedure.        //
//        When the magnitude of x[3] - x[1] is less than "tolerance" *        //
//        ((x[3] + x[1]) / 2 + eta), the procedure is terminated, where eta   //
//        is 1 if *x3 + *x1 <= 1 and 0 otherwise.  "tolerance' should be      //
//        is strictly positive number.  If a nonpositive number is passed,    //
//        then tolerance is set to sqrt(DBL_EPSILON).                         //
//                                                                            //
//  Return Values:                                                            //
//     0   Success                                                            //
//    -1   The three points are collinear.                                    //
//    -2   The extremum of the parabola through the three points is a maximum.//
//    -3   The initial points failed to satisfy the condition that            //
//         x1 < x2 < x3 and fx1 > fx2 < fx3.                                  //
//                                                                            //
//  Example:                                                                  //
//     extern double f(double);                                               //
//     double x[] = { -1.0, 0.0, 1.0 };                                       //
//     double fx = { f(x[0]), f(x[1]), f(x[2]) };                             //
//     double tolerance = 1.0e-2;                                             //
//     int err;                                                               //
//                                                                            //
//             (verify fx[1] < fx[2] and fx[1] < fx[0] otherwise              //
//                                 choose different initial points)           //
//                                                                            //
//     err = Min_Search_Parabolic_Interpolation(f, x, fx, tolerance )         //
//     if (err < 0 ) printf("Error in input data\n");                         //
//     else printf("local minimum is between (%12.6e,%12.6e) and              //
//                               (%12.6e,%12.6e)\n", x[0],fx[0], x[2],fx[2]); //
////////////////////////////////////////////////////////////////////////////////

int GradientReverse::Min_Search_Parabolic_Interpolation(double (GradientReverse::*f)(double), double* x0,
                                               double* fx0, double tolerance )
{
   double x[5] = { x0[0], x0[0], x0[1], x0[2], x0[2] };
   double y[5] = { fx0[0], fx0[0], fx0[1], fx0[2], fx0[2] };
   double min_displacement = 0.01 * tolerance;
   double epsilon = 0.333333 * tolerance;
   double xmin, ymin;
   int err = 0;

       // Verify that the input data satisfy the conditions:
       // x1 < x2 < x3 and fx1 >= fx2 =< fx3 but not fx1 = fx2 = fx3.

   if ( (x[0] > x[2]) || (x[4] < x[2]) ) return -3;
   if ( (y[0] < y[2]) || (y[2] > y[4]) ) return -3;
   if ( (y[0] == y[2]) && (y[2] == y[4]) ) return -3;

       // If the input tolerance is nonpositive, set it to the default.

   if (tolerance <= 0.0) tolerance = sqrt(DBL_EPSILON);

                      // Find the three interior points.

   Left_Interior_Point( f, x, y );
   if ( Stopping_Rule(x, tolerance) ) return 0;
   Right_Interior_Point( f, x, y );

                 // Iterate until the location of the minimum
                 // is found within the specified tolerance.

   while (!Stopping_Rule(x, tolerance ) ) {
      err = Parabolic_Minimum( x, y, &xmin);
      if (err) break;
      Check_New_Test_Point( x, &xmin, min_displacement, epsilon);
      ymin = (this->*f)(xmin);
      if ( xmin < x[2] ) {
         if ( ymin >= y[1] ) {
            x[0] = xmin; y[0] = ymin; Left_Interior_Point( f, x, y );
         }
         else if ( ymin >= y[2] ) {
            x[0] = x[1]; y[0] = y[1]; x[1] = xmin; y[1] = ymin;
         }
         else {
            x[4] = x[3]; y[4] = y[3]; x[3] = x[2]; y[3] = y[2];
            x[2] = xmin; y[2] = ymin;
         }
      }
      else {
         if ( ymin >= y[3] ) {
            x[4] = xmin; y[4] = ymin; Right_Interior_Point( f, x, y );
         }
         else if ( ymin >= y[2] ) {
            x[4] = x[3]; y[4] = y[3]; x[3] = xmin; y[3] = ymin;
         }
         else {
            x[0] = x[1]; y[0] = y[1]; x[1] = x[2]; y[1] = y[2];
            x[2] = xmin; y[2] = ymin;
         }
      }
   }

                 // If no errs were detected, return the final
                 // interval and an internal point together with
                 // the value of the function evaluated there.

   if (err == 0) {
      x0[0] = x[1];
      x0[1] = x[2];
      x0[2] = x[3];
      fx0[0] = y[1];
      fx0[1] = y[2];
      fx0[2] = y[3];
   }

   return err;
}


////////////////////////////////////////////////////////////////////////////////
//  static int Stopping_Rule(double x[], double tolerance)                    //
//                                                                            //
//  Description:                                                              //
//     Given an ordered array x[0] < x[1] < ... < x[4], this routine returns  //
//     TRUE (1) if  x[3] - x[1] < epsilon * (|xm| + eta) or FALSE (0)         //
//     otherwise, where f(x[2]) is the minimum among f(x[i]), i = 0,...,4,    //
//     epsilon = tolerance, and eta = 1.0 if xm <= 1.0 otherwise              //
//     eta = 0.0.                                                             //
//                                                                            //
//     For C compilers which support the 'inline' type qualifier, this routine//
//     could be declared with the 'inline' qualifier.                         //
//                                                                            //
//  Return Values:                                                            //
//     0  Bound is not within the specified tolerance.                        //
//     1  Bound is within the specified tolerance.                            //
////////////////////////////////////////////////////////////////////////////////
int GradientReverse::Stopping_Rule(double x[], double tolerance)
{
   double xmid = 0.5 * fabs( x[3] + x[1] );
   double eta = (xmid <= 1.0) ? 1.0 : fabs(xmid);

   return ( ( x[3] - x[1] ) <= tolerance * eta ) ? 1 : 0;
}


////////////////////////////////////////////////////////////////////////////////
//  static int Parabolic_Minimum( double x[], double y[], double *xmin)       //
//                                                                            //
//  Description:                                                              //
//     Given three noncollinear points (x[1],y[1]), (x[2],y[2]), and          //
//     (x[3],y[3]) such that x[1] < x[2] < x[3] and y[1] > y[2] < y[3] find   //
//     the location of the minimum, xmin, of the parabola through these       //
//     points.                                                                //
//                                                                            //
//     For C compilers which support the 'inline' type qualifier, this routine//
//     could be declared with the 'inline' qualifier.                         //
//                                                                            //
//  Return Values:                                                            //
//     0  Success                                                             //
//    -1  The three points are collinear.                                     //
//    -2  The extremum of the parabola through the three points is a maximum. //
////////////////////////////////////////////////////////////////////////////////
int GradientReverse::Parabolic_Minimum( double x[], double y[], double *xmin)
{
   double d1 = (x[3] - x[2]) * (y[1] - y[2]);
   double d2 = (x[2] - x[1]) * (y[3] - y[2]);
   double denominator = d1 + d2;
   double numerator = (x[3] + x[2]) * d1 + (x[2] + x[1]) * d2;

   if (denominator == 0.0) {                   // the points are collinear.
      *xmin = (y[1] < y[3]) ?  x[1] : x[3];
      return -1;
   }
   if (denominator < 0.0) {                    // local maximum
      *xmin = (y[1] < y[3]) ?  x[1] : x[3];
      return -2;
   }
   *xmin = 0.5 * numerator / denominator;
   return 0;
}


////////////////////////////////////////////////////////////////////////////////
// static void Check_New_Test_Point( double x[], double *xmin,                //
//                                 double min_displacement, double epsilon )  //
//                                                                            //
//  Description:                                                              //
//     This routine alters the test point, *xmin, in the event that *xmin is  //
//     within min_displacement of one of the nodes x[1], x[2], and x[3].      //
//                                                                            //
//  Return Value:                                                             //
//     void                                                                   //
////////////////////////////////////////////////////////////////////////////////
void GradientReverse::Check_New_Test_Point( double x[], double *xmin,
                                     double min_displacement, double epsilon )
{
   double eta;
   double d1, d2;

                 // Compare the location of the minimum of the
                 // parabolic fit to the center node.

   eta = ( x[2] == 0.0 ) ? 1.0 : fabs(x[2]);
   if ( fabs(*xmin - x[2]) < (min_displacement * eta) ) {
      if ( *xmin == x[2] )
         if ( (x[3] - x[2]) > ( x[2] - x[1] ) )
            *xmin = x[2] + alpha * (x[3] - x[2]);
         else *xmin = x[2] - alpha * (x[2] - x[1]);
      else if ( *xmin < x[2] )
         if ( x[2] - x[1] < epsilon * eta )
            *xmin = x[2] + (x[2] - x[1]);
         else {
            d1 = epsilon * eta;
            d2 = alpha * (x[2] - x[1]);
            *xmin = x[2] - ( (d1 < d2) ? d1 : d2 );
         }
      else
         if ( x[3] - x[2] < epsilon * eta )
            *xmin = x[2] - (x[3] - x[2]);
         else {
            d1 = epsilon * eta;
            d2 = alpha * (x[3] - x[2]);
            *xmin = x[2] + ( (d1 < d2) ? d1 : d2 );
         }
      return;
   }

                 // Compare the location of the minimum of the
                 // parabolic fit to the left-most node.

   eta = ( x[1] == 0.0 ) ? 1.0 : fabs(x[1]);
   if ( (*xmin - x[1]) < (min_displacement * eta)) {
      d1 = epsilon * eta;
      d2 = alpha * (x[2] - x[1]);
      *xmin = x[1] + ( (d1 < d2) ? d1 : d2 );
      return;
   }

                 // Compare the location of the minimum of the
                 // parabolic fit to the right-most node.

   eta = ( x[3] == 0.0 ) ? 1.0 : fabs(x[3]);
   if ( (x[3] - *xmin) < (min_displacement * eta)) {
      d1 = epsilon * eta;
      d2 = alpha * (x[3] - x[2]);
      *xmin = x[3] - ( (d1 < d2) ? d1 : d2 );
      return;
   }

   return;
}


////////////////////////////////////////////////////////////////////////////////
//  static void Left_Interior_Point( double (*f)(double), double x[],         //
//                                                              double y[] )  //
//                                                                            //
//  Description:                                                              //
//     Select the new test point using the points (x[0],y[0]), (x[2],y[2]),   //
//     and (x[4],y[4]),  where x[0] < x[2] < x[4] and y[0] > y[2] < y[4].     //
//     Upon return the points (x[0],y[0]), (x[1],y[1]), (x[2],y[2]),          //
//     (x[4],y[4]) are defined with x[0] < x[1] < x[2] < x[4] and             //
//     y[0] > y[1] > y[2] < y[4], where y[i] = f(x[i]).                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void GradientReverse::Left_Interior_Point( double (GradientReverse::*f)(double), double x[], double y[] )
{
   int not_found = 1;

   while (not_found) {
      x[1] = x[2] - alpha * (x[2] - x[0]);
      y[1] = (this->*f)(x[1]);
      if ( y[1] >= y[0] ) { x[0] = x[1]; y[0] = y[1]; }
      else if ( y[1] >= y[2] ) not_found = 0;
      else {
         x[4] = x[3]; y[4] = y[3];
         x[3] = x[2]; y[3] = y[2];
         x[2] = x[1]; y[2] = y[1];
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
//  static void Right_Interior_Point( double (*f)(double), double x[],        //
//                                                              double y[] )  //
//                                                                            //
//  Description:                                                              //
//     Given four points (x[0],y[0]), (x[1],y[1]), (x[2],y[2]), and           //
//     (x[4],y[4]), such that x[0] < x[1] < x[2] < x[4] and y[0] > y[1] >     //
//     y[2] < y[4] find points (x[0],y[0]), (x[1],y[1]), (x[2],y[2]),         //
//     (x[3],y[3]) and (x[4],y[4]), such that x[0] < x[1] < x[2] < x[3] < x[4]//
//     y[0] > y[1] > y[2] < y[3] < y[4], where y[i] = f(x[i]).                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void GradientReverse::Right_Interior_Point( double (GradientReverse::*f)(double), double x[], double y[] )
{
   int not_found = 1;

   while (not_found) {
      x[3] = x[2] + alpha * (x[4] - x[2]);
      y[3] = (this->*f)(x[3]);
      if ( y[3] >= y[4] ) { x[4] = x[3]; y[4] = y[3]; }
      else if ( y[3] >= y[2] ) not_found = 0;
      else {
         x[0] = x[1]; y[0] = y[1];
         x[1] = x[2]; y[1] = y[2];
         x[2] = x[3]; y[2] = y[3];
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
//  void Ray_in_n_Space(double v[], double initial_pt[], double direction[],  //
//                                                    double distance, int n) //
//                                                                            //
//  Description:                                                              //
//     Calculates the vector v = initial_pt + distance * direction.           //
//     If direction has unit norm, the v lies at a distance |distance| from   //
//     the initial_pt, i.e. || v - initial_pt || = | distance |.              //
//     In general, || v - initial_pt || = |distance| || direction ||.         //
//     The argument distance can be positive or negative.  The argument       //
//     direction need not have unit norm.                                     //
//                                                                            //
//  Arguments:                                                                //
//     double v[]                                                             //
//            Pointer to the first element of the resultant vector v[n].      //
//     double initial_pt[]                                                    //
//            Pointer to the first element of the vector initial_pt[n].       //
//     double direction[]                                                     //
//            Pointer to the first element of the vector direction[n].        //
//     double distance                                                        //
//            Distance to travel from the initial point, initial_pt, along    //
//            a line with direction 'direction', assuming that 'direction'    //
//            has unit norm.                                                  //
//     int    n                                                               //
//            The dimension of the vectors, v, initial_pt, and direction.     //
//                                                                            //
//  Return Values:                                                            //
//     address to v.                                                          //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double v[N], init_pt[N], d[N], distance;                               //
//     int i;                                                                 //
//                                                                            //
//     (your code to intialize the vectors init_pt and d, and the scalar)     //
//     (distance.                                                       )     //
//                                                                            //
//     Ray_in_n_Space(v, init_pt, d, distance, N);                            //
//     for (i = 0; i < N; i++)                                                //
//        printf(" v[%3d] = %12.6f\n", i,v[i]);                               //
//           ...                                                              //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void GradientReverse::Ray_in_n_Space(double v[], double initial_pt[], double direction[],
                                                      double distance, int n )
{
   for (--n; n >= 0; n--)
     v[n]  = initial_pt[n] + distance * direction[n];
}

#endif
