#include "csp.h"

#include "coremessagehandler.h"
#include "core_errors.h"
#include "units.h"
#include "dataset.h"
#include "prediction.h"

#include <math.h>
#include <QtPlugin>

void CSPReverseFactory::init(Core *core)
{
    //Set the core
    Core::setInstance(core);

    //Store the priorities
    _priorities.insert(descriptor().id, CSPReverse().priority());
}

ReverseEngine *CSPReverseFactory::newReverseEngine()
{
    return new CSPReverse();
}

Descriptor CSPReverseFactory::descriptor()
{
    return Descriptor(CSP_ID,
                      tr("Point CSP"),
                      tr("Reverse calculation using an exhaustive method."));
}

//Returns an engine priority
int CSPReverseFactory::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}


//--------------------------------------- ENGINE -----------------------------------------

// Options OID definition
#define KEEP_THE_N_BEST "00n_best"
#define FORCE_ANY_DOSE_OPTION "01any_dose"
#define MIN_POSOLOGY_DOSE_OPTION "02min_dose"
#define MAX_POSOLOGY_DOSE_OPTION "03max_dose"
#define POSOLOGY_DOSE_STEP_OPTION "04dose_step"
#define FORCE_ANY_INFUSION_OPTION "05any_infusion"
#define MIN_INFUSION_TIME_OPTION "06min_infusion"
#define MAX_INFUSION_TIME_OPTION "07max_infusion"
#define INFUSION_STEP_OPTION "08infusion_step"
#define OPTIMISATION_CHECK_OPTION "09optimisation_check"
#define OPTIMISATION_QUEUE_OPTION "10optimisation_queue"

//The engine default priority level
const int CSPReverse::_priority = 10;

CSPReverse::CSPReverse() :
    _predictor(0),
    _tracker(5),
    _errorWeight(),
    _residualWanted(-1),
    _residualMin(-1),
    _residualMax(-1),
    _peakWanted(-1),
    _peakMin(-1),
    _peakMax(-1),
    _peakTime()
{
    //Set the targets weights
    _errorWeight.insert(Target::ResidualTarget, 1.0);
    _errorWeight.insert(Target::PeakTarget, 1.0);

    //Set the options defaults
    addOption(KEEP_THE_N_BEST, tr("Keep n best dosage"), tr(""), QVariant::Int, QVariant(4));
    addOption(FORCE_ANY_DOSE_OPTION, tr("Force any dose"), tr(""), QVariant::Bool, QVariant(false));
    addOption(MIN_POSOLOGY_DOSE_OPTION, tr("Minimum dose"), tr(""), QVariant::Double, QVariant(1));
    addOption(MAX_POSOLOGY_DOSE_OPTION, tr("Maximum dose"), tr(""), QVariant::Double, QVariant(1000));
    addOption(POSOLOGY_DOSE_STEP_OPTION, tr("Dose step"), tr(""), QVariant::Double, QVariant(1));
    addOption(FORCE_ANY_INFUSION_OPTION, tr("Force any infusion time"), tr(""), QVariant::Bool, QVariant(false));
    addOption(MIN_INFUSION_TIME_OPTION, tr("Minimum infusion time (minutes)"), tr(""), QVariant::Double, QVariant(5));
    addOption(MAX_INFUSION_TIME_OPTION, tr("Maximum infusion time (minutes)"), tr(""), QVariant::Double, QVariant(60));
    addOption(INFUSION_STEP_OPTION, tr("Infusion time step (minutes)"), tr(""), QVariant::Double, QVariant(5));
    addOption(OPTIMISATION_CHECK_OPTION, tr("Enable optimisation"), tr(""), QVariant::Bool, QVariant(true));
    addOption(OPTIMISATION_QUEUE_OPTION, tr("Optimisation queue"), tr(""), QVariant::Int, QVariant(5));
}

// New instance, and copy the options if asked
CSPReverse * CSPReverse::clone()
{
   //New instance
   CSPReverse *instance = new CSPReverse();

   //Copy the options
   instance->copyOptions(this);

   return instance;
}

Descriptor CSPReverse::descriptor()
{
   return Descriptor("ch.heig-vd.ezechiel.csp",
                     tr("Point CSP"),
                     tr("Reverse calculation using an exhaustive method."));
}

int CSPReverse::priority() const
{
    return _priority;
}

Version CSPReverse::version()
{
   return Version("0.1");
}

QString CSPReverse::errorString ()
{
    return QString();
}

void CSPReverse::setAnalysis(SharedDrugResponseAnalysis model)
{
    _predictor = model;
}

QMap<int, SharedDosage> CSPReverse::reverseDosages(SharedPrediction _pred, const SharedDosage &currentDosage, const Duration &start, const SharedTargetSet &targets, const QList<Duration> &availableIntervals, const QList<Duration> &availableInfusions, const QList<Amount> &availableDoses, ParameterType type, int nbIntakes)
{
    //The number of doses to return
    int keepNBest = option(KEEP_THE_N_BEST).value.toInt();

    //The dose options
    bool anyDose    = option(FORCE_ANY_DOSE_OPTION).value.toBool();
    double minDose  = option(MIN_POSOLOGY_DOSE_OPTION).value.toDouble();
    double maxDose  = option(MAX_POSOLOGY_DOSE_OPTION).value.toDouble();
    double doseStep = option(POSOLOGY_DOSE_STEP_OPTION).value.toDouble();

    //The infusion options
    bool anyInfusionTime    = option(FORCE_ANY_INFUSION_OPTION).value.toBool();
    double minInfusionTime  = option(MIN_INFUSION_TIME_OPTION).value.toDouble();
    double maxInfusionTime  = option(MAX_INFUSION_TIME_OPTION).value.toDouble();
    double infusionTimeStep = option(INFUSION_STEP_OPTION).value.toDouble();

    //The optimisation options
    bool enableOptimisation = option(OPTIMISATION_CHECK_OPTION).value.toBool();
    int optimisationQueue   = option(OPTIMISATION_QUEUE_OPTION).value.toInt();

    //The dosages
    SharedDosage dosageToTest = currentDosage;
    SharedDosage bestDosage   = currentDosage;
    QList<Amount> doses = availableDoses;
    QMap<double, SharedDosage> validDosages;

    //The durations
    Duration targetedTime;
    QList<Duration> intervals = availableIntervals;
    QList<Duration> infusions = availableInfusions;

    //The scores
    double score;
    double bestScore = -1;

    //The the optimisation values
    _tracker.setSize(!enableOptimisation || optimisationQueue <= 0 ? 0 : optimisationQueue);

    //Set the targets values and weight
    if (!setTargets(targets))
        return QMap<int, SharedDosage>();

    //Set the dosage to test's applied date
    dosageToTest->setApplied(_predictor->treatment()->dosages()->firsttake().addSecs(start.seconds()));

    //Set the infusion list if necessary
    if (infusions.isEmpty())
        infusions.append(dosageToTest->tinf());

    //Set the any dose check if necessary
    for (int i = 0; i < doses.count() && !anyDose; i++) {
        if (doses.at(i).value() == -1)
            anyDose = true;
    }

    //Set the doses list for any dosage
    if (anyDose) {
        doses.clear();
        for (double doseValue = minDose; doseValue <= maxDose; doseValue += doseStep)
            doses.append(Amount(doseValue, currentDosage->dose().unit()));
    }

    //Set the infusions list for any infusion
    if (anyInfusionTime) {
        infusions.clear();
        for (double infusionTime = minInfusionTime; infusionTime <= maxInfusionTime; infusionTime += infusionTimeStep)
            infusions.append(Duration(int(infusionTime) / 60, int(infusionTime) % 60));
    }

    //Iterate through the intervals
    for (int i = 0; i < intervals.count(); ++i) {
        dosageToTest->setInterval(intervals.at(i));

        //Iterate through the infusion times
        for (int j = 0; j < infusions.count(); ++j) {
            dosageToTest->setTinf(infusions.at(j));

            //Reset the score tracker
            _tracker.clear();

            //Iterate through the doses
            for (int k = 0; k < doses.count(); ++k) {
                dosageToTest->setDose(doses.at(k));

                if (nbIntakes > 0)
                    targetedTime = Duration(0, 0, 0, _predictor->treatment()->dosages()->firsttake().msecsTo(dosageToTest->applied().addMSecs(nbIntakes * dosageToTest->interval().mSecs())));
                else
                    targetedTime = _predictor->steadyState(_pred, start, type, dosageToTest);

                if (targetedTime.isEmpty())
                    continue;

                score = scoring(targetedTime, type, dosageToTest);

                if (score < 0) {
                    if (!_tracker.evaluate(score))
                        break;

                    _tracker.push(score);
                } else {
                    if (bestScore == -1 || score < bestScore) {
                        bestScore  = score;
                        bestDosage = dosageToTest;
                    }
                    validDosages.insertMulti(score, dosageToTest);

                    _tracker.clear();
                }
            }
        }
    }

    //Check if dosages were found
    if(bestScore == -1)
        return QMap<int, SharedDosage>();

    //Keep the N best dosages
    int kept = 0;
    QMap<int,SharedDosage> result;

    for (QMap<double, SharedDosage>::ConstIterator i = validDosages.constBegin(); i != validDosages.constEnd() && kept < keepNBest; ++i)
            result.insert(++kept, i.value());

    return result;
}

bool CSPReverse::setTargets(const SharedTargetSet &targets)
{
    //Reset the targets
    _residualWanted = -1;
    _residualMax    = -1;
    _residualMin    = -1;
    _peakWanted     = -1;
    _peakMax        = -1;
    _peakMin        = -1;
    _peakTime       = Duration();

    //Set the targets values
    foreach (Target* t, *targets.data()){
       switch(t->type()){
       case Target::ResidualTarget:
           _residualWanted = t->cbest().value();
           _residualMax    = t->cmax().value();
           _residualMin    = t->cmin().value();

           //Reset if incomplete
           if (_residualMin == -1 || _residualMax == -1)
               _residualWanted = -1;
           break;
       case Target::PeakTarget:
           _peakWanted = t->cbest().value();
           _peakMax    = t->cmax().value();
           _peakMin    = t->cmin().value();
           _peakTime   = t->tbest();

           //Reset if incomplete
           if (_peakMin == -1 || _peakMax == -1 || _peakTime.isEmpty())
               _peakWanted = -1;
           break;
       default:
           ;
       }
    }

    //Check if a target was found
    if(_residualWanted == -1 && _peakWanted == -1)
        return false;

    return true;
}

double CSPReverse::scoring(const Duration &at, const ParameterType type, const SharedDosage &dosage)
{
    double residualDiff = 0.0;
    double peakDiff     = 0.0;

    //Compute the residual score
    if (_residualWanted != -1.0) {
        double residualValue;

        //Get the residual concentration
        residualValue = _predictor->point(SharedPrediction(), at + dosage->interval(), type, dosage).value();

        //Compute the score (>= 0 if valid, < 0 otherwise)
        if (residualValue <= _residualMax && residualValue >= _residualMin)
            residualDiff = fabs(residualValue - _residualWanted);
        else
            residualDiff = -(_residualMin > residualValue ? _residualMin - residualValue : residualValue - _residualMax);
    } else
        _errorWeight.insert(Target::ResidualTarget, 0.0);

    //Compute the peak score
    if (_peakWanted != -1.0) {
        double peakValue;

        //Get the peak concentration
        peakValue = _predictor->point(SharedPrediction(), at + _peakTime, type, dosage).value();

        //Compute the score (>= 0 if valid, < 0 otherwise)
        if (peakValue <= _peakMax && peakValue >= _peakMin)
            peakDiff = fabs(peakValue - _peakWanted);
        else
            peakDiff = -(_peakMin > peakValue ? _peakMin - peakValue : peakValue - _peakMax);
    } else
        _errorWeight.insert(Target::PeakTarget, 0.0);

    //Return the first representable value less than zero
    if (_residualWanted != -1.0 && _peakWanted != -1.0 && ((residualDiff < 0 && peakDiff >= 0) || (residualDiff >= 0 && peakDiff < 0)))
        return std::nextafter(0.0, -1.0);

    //Return the score
    return (peakDiff * _errorWeight.value(Target::PeakTarget, 0.0)) + (residualDiff * _errorWeight.value(Target::ResidualTarget, 0.0));
}

CSPReverse::ScoreTracker::ScoreTracker() :
    _scores(), _size(0)
{

}

CSPReverse::ScoreTracker::ScoreTracker(int size) :
    _scores(), _size(size)
{

}

CSPReverse::ScoreTracker::~ScoreTracker()
{

}

void CSPReverse::ScoreTracker::push(double score)
{
    if (_size <= 0)
        return;

    if (_scores.count() == _size)
        _scores.removeLast();

    _scores.prepend(fabs(score));
}

void CSPReverse::ScoreTracker::pop()
{
    if (!_scores.isEmpty())
        _scores.removeLast();
}

bool CSPReverse::ScoreTracker::evaluate(double score)
{
    if (_size <= 0 || _size > _scores.count())
        return true;

    double current = fabs(score);

    for (int i = 0; i < _scores.count(); ++i) {
        if (current <= _scores.at(i))
            return true;
        current = _scores.at(i);
    }

    return false;
}

void CSPReverse::ScoreTracker::clear()
{
    _scores.clear();
}

void CSPReverse::ScoreTracker::setSize(int size)
{
    _size = size;
}

int CSPReverse::ScoreTracker::size() const
{
    return _size;
}
