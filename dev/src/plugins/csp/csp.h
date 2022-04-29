//@@license@@

#ifndef CSPREVERSE_H
#define CSPREVERSE_H

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

class Prediction;
/** CSP factory.
 * \ingroup factories
 * Simple factory for the CSP reverse engine
 */
class CSPReverseFactory : public ReverseEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "csp")
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


//! \brief The CSP reverse prediction engine
/*! This reverse prediction engine uses the CSP method to find the root of a function
   to find a dosage corresponding to a value
  */
class CSPReverse : public ReverseEngine
{
    Q_DECLARE_TR_FUNCTIONS(CSPReverse)
public:
   CSPReverse();

   //! \brief Copy constructor
   /*! Fields being copied are : the predictor, the options
     */
   CSPReverse* clone ();

   //! \brief Descriptor
   /*! The id for this engine is : ch.heig-vd.ezechiel.engine.reverse.CSP
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

   //The score tracker
   class ScoreTracker
   {
   public:
       ScoreTracker();
       ScoreTracker(int size);
       ~ScoreTracker();

       void push(double score);
       void pop();

       bool evaluate(double score);
       void clear();

       void setSize(int size);
       int size() const;

   private:
       QList<double> _scores;
       int _size;
   };

   //The engine priority level
   static const int _priority;

   //Sets the targets values
   bool setTargets(const SharedTargetSet &targets);

   //Returns a dosages's score
   double scoring(const Duration &at, const ParameterType type, const SharedDosage &dosage);

   //The predictor to use
   SharedDrugResponseAnalysis _predictor;

   //The score tracker
   ScoreTracker _tracker;

   //The targets weights
   QHash<Target::TargetType, double> _errorWeight;

   //The residual values
   double _residualWanted;
   double _residualMax;
   double _residualMin;

   //The peak values
   double _peakWanted;
   double _peakMin;
   double _peakMax;
   Duration _peakTime;
};

#endif // CSPREVERSE_H
