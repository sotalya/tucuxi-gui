#ifndef GradientREVERSE_H
#define GradientREVERSE_H

#include "ezutils.h"
#include "medical.h"
#include "drugresponseanalysis.h"
#include "drugtreatment.h"
#include "optionable.h"
#include "reverseengine.h"
#include "reverseenginefactory.h"
#include "core.h"

#include <QCoreApplication>
#include <QObject>
#include <QDateTime>

#include <QFile>
#include <QTextStream>

/** Gradient factory.
 * \ingroup factories
 * Simple factory for the Gradient reverse engine
 */
class GradientReverseFactory : public ReverseEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "gradient")
    Q_INTERFACES(ReverseEngineFactory)
public:
   void init(Core *core);

   ReverseEngine* newReverseEngine ();

   Descriptor descriptor ();

   int priority(const QString &id) const;

private:
   //Map of <Engine ID, Engine priority>
   QMap<QString, int> _priorities;
};


//! \brief The Gradient reverse prediction engine
/*! This reverse prediction engine uses the Gradient descent method to find the root of a function
   to find a dosage corresponding to a value
  */
class GradientReverse : public ReverseEngine
{
    Q_DECLARE_TR_FUNCTIONS(GradientReverse)
public:
   GradientReverse();

   //! \brief Copy constructor
   /*! Fields being copied are : the predictor, the options
     */
   GradientReverse* clone ();

   //! \brief Descriptor
   /*! The id for this engine is : ch.heig-vd.ezechiel.engine.reverse.gradient
     */
   Descriptor descriptor ();

   int priority() const;

   Version version();

   QString errorString ();

   void setAnalysis (SharedDrugResponseAnalysis model);

   QMap<int,SharedDosage> reverseDosages (Prediction* _pred,
                                          const SharedDosage &currentDosage,
                                    const Duration &start,
                                    const SharedTargetSet &targets,
                                    const QList<Duration> &availableIntervals,
                                    const QList<Duration> &availableInfusions,
                                    const QList<Amount> &availableDosages,
                                    ParameterType type,
                                    int nbIntakes = 0);

private:
   //The engine priority level
   static const int _priority;

   // The predictor to use
   SharedDrugResponseAnalysis predictor;
   Prediction* m_pred;
   //The type for predictor
   ParameterType type;

   //The current used dosage
   SharedDosage dosage;

   //The time correspondig to beginning of prediction
   Duration start;

   //Target for fitness computing
   SharedTargetSet targets;

   //Model call number for debuging
   unsigned int modelCall;

   double scoring(const Duration &at, const ParameterType type, const SharedDosage &dosage, const SharedTargetSet &targets, QHash<Target::TargetType, double> errorWeight);
   bool checkForValidSolution(const SharedDosage &solution, double residualMin, double residualMax, double peakMin, double peakMax, const Duration &peaktTime);

   double funToOptimizeWraper(double *x);
   void gradientFun(double *x, double *grad);
   int stoppingRule(double *OriginalPoint, double funValue, double *newPoint, double newFunValue, double *newGrad, int iteration, int n);

   /*----------------------------------Steepest_descent---------------------------------------*/
   struct arg_list {
      double* x;
      double* v;
      double (GradientReverse::*f)(double*);
      double *y;
      int n;
   };
   struct arg_list *args;

   const double alpha;
#if 1
   double min_displacement;

   int  Steepest_Descent(double (GradientReverse::*f)(double *), void (GradientReverse::*df)(double *, double *),
         int (GradientReverse::*stopping_rule)(double*, double, double*, double, double*, int, int),
                             double a[], double *fa, double *dfa, double cutoff,
                             double cutoff_scale_factor, double tolerance, int n);

   void Copy_Vector(double *d, double *s, int n);

   double Vector_Max_Norm(double v[], int n);

   int Minimize_Down_the_Line(double (GradientReverse::*f)(double *), double x[], double fx,
                            double *p, double v[], double y[], double cutoff,
                            double cutoff_scale_factor, double tolerance, int n);

   int Min_Search_Parabolic_Extrapolation(double (GradientReverse::*f)(double), double x[],
                  double fx[], double a, double b, double cut_off_scale_factor);

   int Min_Search_Parabolic_Interpolation(double (GradientReverse::*f)(double), double x[],
                                                double fx[], double tolerance );

   void Ray_in_n_Space(double v[], double initial_pt[], double direction[],
                                                       double distance, int n );

   double g( double lambda );

   double Parabolic_Minimum( double x[], double y[],
                                                   double cut_off_scale_factor );

   double New_Test_Point( double x[], double y[], double a, double b,
                                                   double cut_off_scale_factor );

   int Stopping_Rule(double x[], double tolerance);

   int Parabolic_Minimum( double x[], double y[], double *xmin);
   void Check_New_Test_Point( double x[], double *xmin,
                                        double min_displacement, double epsilon );

   void Left_Interior_Point( double (GradientReverse::*f)(double), double x[], double y[] );

   void Right_Interior_Point( double (GradientReverse::*f)(double), double x[], double y[] );
#endif

};

#endif // GradientREVERSE_H
