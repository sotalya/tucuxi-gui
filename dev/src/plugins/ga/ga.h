//@@license@@

#ifndef GAREVERSE_H
#define GAREVERSE_H

#include "ezutils.h"
#include "medical.h"
#include "drugresponseanalysis.h"
#include "drugtreatment.h"
#include "optionable.h"
#include "reverseengine.h"
#include "reverseenginefactory.h"
#include "core.h"
#include "prediction.h"

#include <QCoreApplication>
#include <QObject>


/** GA factory.
 * \ingroup factories
 * Simple factory for the GA reverse engine
 */
class GAReverseFactory : public ReverseEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ga")
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


//! \brief The GA reverse prediction engine
/*! This reverse prediction engine uses the Genetic Algorithm evolution to find the root of a function
   to find a dosage corresponding to a value
  */
class GAReverse : public ReverseEngine
{
    Q_DECLARE_TR_FUNCTIONS(GAReverse)
public:
   GAReverse();

   //! \brief Copy constructor
   /*! Fields being copied are : the predictor, the options
     */
   GAReverse* clone ();

   //! \brief Descriptor
   /*! The id for this engine is : ch.heig-vd.ezechiel.engine.reverse.ga
     */
   Descriptor descriptor ();

   int priority() const;

   Version version();

   QString errorString ();

   void setAnalysis (SharedDrugResponseAnalysis model);

   QMap<int,SharedDosage> reverseDosages (Prediction* _pred, const SharedDosage &currentDosage,
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

   bool checkForValidSolution(Prediction* _pred, const Duration &start, ParameterType type, const SharedDosage &solution, double residualMin, double residualMax, double peakMin, double peakMax, const Duration &peakTime) const;
};

#endif // GAREVERSE_H
