#include "newtonreverse.h"

#include "coremessagehandler.h"
#include "core_errors.h"
#include "units.h"
#include "dataset.h"
#include "prediction.h"

#include <math.h>

#include <QtPlugin>

void NewtonReverseFactory::init(Core *core)
{
    //Set the core
    Core::setInstance(core);

    //Store the engines priorities
    _priorities.insert(descriptor().id, NewtonReverse(descriptor()).priority());
}

ReverseEngine *NewtonReverseFactory::newReverseEngine()
{
    return new NewtonReverse(this->descriptor());
}

Descriptor NewtonReverseFactory::descriptor()
{
    return Descriptor(NEWTON_ID,
                      tr("Newton"),
                      tr("Reverse calculation using the Newton's method to find the root of the function.\nThis engine uses only the residual target, if not given, the result will be the same as the given value."));
}

int NewtonReverseFactory::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}

// Options OID definition
#define MAX_ITER_OPTION "max_iter"
#define DELTA_OPTION "delta"
#define STOP_DIFF_OPTION "stop"

//The engine default priority level
const int NewtonReverse::_priority = 50;

NewtonReverse::NewtonReverse(const Descriptor &d)
{
    _desc = d;

    // Create the options
    //Distance between the two point when doing the numerical derivation
    addOption(DELTA_OPTION, tr("Delta"), tr("Difference between two point for the numerical derivation, should be small"),
              QMetaType::Double, QVariant(10.0));

    //If the difference between the result and the wanted concentration goes under this value, stop the search
    addOption(STOP_DIFF_OPTION, tr("Maximal difference (%)"), tr("Maximal difference between the wanted concentration and the concentration found with the algorithm, in percent"),
              QMetaType::Double, QVariant(1.0));

    //Maximum number of iteration that the algorithm can do, if the maximum is reached, a warning is shown
    addOption(MAX_ITER_OPTION, tr("Maximum number of iteration"), tr("In each iteration, the algorithm will make some prediction and see if it matches the wanted concentration, to then adapt the dosage. This value permits to fix a maximal number of iteration to do, avoiding an infinite loop"),
              QMetaType::Int, QVariant(10));
}

// New instance, and copy the options
ReverseEngine * NewtonReverse::clone ()
{
    //New instance
    NewtonReverse *instance = new NewtonReverse(_desc);

    //Copy the options
    instance->copyOptions(this);

    return instance;
}

Descriptor NewtonReverse::descriptor()
{
    return Descriptor("ch.heig-vd.ezechiel.newton",
                      tr("Newton"),
                      tr("Reverse calculation using the Newton's method to find the root of the function.\nThis engine uses only the residual target, if not given, the result will be the same as the given value."));
}

int NewtonReverse::priority() const
{
    return _priority;
}

Version NewtonReverse::version()
{
    return Version("0.1");
}

QString NewtonReverse::errorString ()
{
    return _errors;
}

//Simple save
void NewtonReverse::setAnalysis (SharedDrugResponseAnalysis model)
{
    _analysis = model;
}

QMap<int, SharedDosage> NewtonReverse::reverseDosages(Prediction* _pred, const SharedDosage &currentDosage, const Duration &start, const SharedTargetSet &targets, const QList<Duration> &, const QList<Duration> &availableInfusions, const QList<Amount> &availableDosages, ParameterType type, int nbIntakes)
{
    Q_UNUSED(availableInfusions);
    Q_UNUSED(nbIntakes);

    QMap<int, SharedDosage> ds;

    //Use the residual target as goal
    Amount wanted;
    wanted.setValue(-1.0);
    foreach (Target* t, *targets.data()) {
        t->setType(Target::ResidualTarget);
        wanted = t->cbest();
    }

    //If no target is found, return an empty map
    if (wanted.value() == -1.0) {
        _errors = tr("No residual target found, cannot compute.");
        return ds;
    }

    //Fetch the options
    int max_iter = option(MAX_ITER_OPTION).value.toInt();
    int stop_diff = option(STOP_DIFF_OPTION).value.toDouble();
    double delta = option(DELTA_OPTION).value.toDouble();

    //Dose, filled with the current dosage, and applied at the right time
    SharedDosage dose = currentDosage;
    dose->setApplied(_analysis->treatment()->dosages()->firsttake().addSecs(start.seconds()));
    //The moment used corresponds to the end of the steady state (minus a second, we want the end, not the begining)
    Duration moment(0,0,start.seconds() + dose->interval().seconds() - 1);
    //Dose with the delta (not yet, you fool!)
    SharedDosage deltaDose = dose;
    //Difference between the wanted concentration and the calculated final concentration
    Amount concentration;
    /* The Newton method find the root of a function, but we do not want the root but a specific value.
       And so the function is moved from the value of the wanted concentration
      */
    //Limit the maximum number of iteration
    for (int i=0; i<max_iter; i++) {
        //If the maximum iteration is reached
        if (i == max_iter-1)
            WARNING(tr("Maximum number of iteration reached when searching for the dosage, the result might be wrong"));

        //Dose + delta
        deltaDose->dose().setValue(dose->dose().value() + delta);
        //The two points for the numerical derivative
        Amount one = _analysis->point(SharedPrediction(), moment,type,dose);
        Amount two = _analysis->point(SharedPrediction(), moment,type,deltaDose);
        //The concentration corresponds to the first points minus the wanted final concentration
        concentration = one - wanted;

        //If the new concentration is good enough, quit the loop
        if (fabs(concentration.value()) < (wanted.value()*(stop_diff/100.0)))
            break;

        /* Calculation of the new dose.
          The method used is the Newton one.
          As the Newton method use the first derivative of the function, but as we don't know its algebrical value, a numerical derivation is done.
         */
        dose->setDose(dose->dose() - concentration / ( (two.value() - one.value()) / delta ));
    }

    //Fill the map with the best value found
    double ideal = dose->dose().value();
    double diff = -1.0;
    foreach (Amount a, availableDosages) {
        //If -1.0 is given, save the found value and quit
        if (a.value() == -1.0) {
            dose->dose().setValue(ideal);
            break;
        }
        //Check if the difference between the ideal and the current is better than the one found until here
        double newDiff = fabs(a.value() - ideal);
        if (newDiff < diff || diff == -1.0) {
            diff = newDiff;
            dose->setDose(a);
        }
    }

    //Save the given value with 100% in the map and return it
    ds.insert(100,dose);
    return ds;
}
