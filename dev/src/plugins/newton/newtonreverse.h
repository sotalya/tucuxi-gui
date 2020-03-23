#ifndef NEWTONREVERSE_H
#define NEWTONREVERSE_H

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

class Prediction;
/** Newton factory.
 * \ingroup factories
 * Simple factory for the newton reverse engine
 */
class NewtonReverseFactory : public ReverseEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "newton")
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

/** \brief The Newton reverse prediction engine.
 * \ingroup reengines
 * This reverse prediction engine uses the Netwon's method to find the root of a function to find a dosage corresponding to a value
 */
class NewtonReverse : public ReverseEngine
{
    Q_DECLARE_TR_FUNCTIONS(NewtonReverse)
public:
    NewtonReverse(const Descriptor &d);

    ReverseEngine* clone ();

    //! \brief Descriptor
    /*! The id for this engine is : ch.heig-vd.ezechiel.newton
     */
    Descriptor descriptor ();

    int priority() const;

    Version version ();

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

    //The model to use
    SharedDrugResponseAnalysis _analysis;

    //Our error string
    QString _errors;

    //Our description, given by our factory
    Descriptor _desc;
};

#endif // NEWTONREVERSE_H
