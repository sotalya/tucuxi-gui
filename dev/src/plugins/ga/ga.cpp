#include "ga.h"

#include "coremessagehandler.h"
#include "core_errors.h"
#include "units.h"
#include "dataset.h"

#include <QtPlugin>
#include <QMutex>

#include "evolutionengine.h"
#include "population.h"
#include "evolutionezechiel.h"

void GAReverseFactory::init(Core *core)
{
    //Set the core
    Core::setInstance(core);

    //Store the engines priorities
    _priorities.insert(descriptor().id, GAReverse().priority());
}

ReverseEngine *GAReverseFactory::newReverseEngine()
{
    return new GAReverse();
}

Descriptor GAReverseFactory::descriptor()
{
    return Descriptor(GA_ID,
                      tr("Genetic Algorithm"),
                      tr("Reverse calculation using an genetic algorithm."));
}

int GAReverseFactory::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}


//--------------------------------------- ENGINE -----------------------------------------


// Options OID definition
#define KEEP_THE_N_BEST "0n_best"
#define POP_SIZE "1pop_size"
#define NB_GEN "2nb_gen"
#define CROSS_P "3cross_p"
#define MUT_P "4mut_p"
#define MUT_BIT_P "5mut_bit_p"
#define ELITE_SELECT_NUM "6elite_select_num"
#define INDIVIDUAL_SELECT_NUM "7individual_select_num"

//The engine default priority level
const int GAReverse::_priority = 30;

GAReverse::GAReverse()
{
    addOption(KEEP_THE_N_BEST, tr("Keep n best dosage"), tr(""), QVariant::Int, QVariant(20));
    addOption(POP_SIZE, tr("Population size"), tr(""), QVariant::Int, QVariant(200));
    addOption(NB_GEN, tr("Number of generation to envolve"), tr(""), QVariant::Int, QVariant(40));
    addOption(CROSS_P, tr("Crossover propability"), tr(""), QVariant::Double, QVariant(1.0));
    addOption(MUT_P, tr("Mutation probability"), tr(""), QVariant::Double, QVariant(0.25));
    addOption(MUT_BIT_P, tr("Mutation per bit probability"), tr(""), QVariant::Double, QVariant(0.10));
    addOption(ELITE_SELECT_NUM, tr("Number of elite to select"), tr(""), QVariant::Int, QVariant(2));
    addOption(INDIVIDUAL_SELECT_NUM, tr("Number of individuals to select"), tr(""), QVariant::Int, QVariant(190));
}

// New instance, and copy the options if asked
GAReverse * GAReverse::clone()
{
   //New instance
   GAReverse *instance = new GAReverse();

   //Copy the options
   instance->copyOptions(this);

   return instance;
}

Descriptor GAReverse::descriptor()
{
   return Descriptor("ch.heig-vd.ezechiel.ga",
                     tr("Genetic Algorithm"),
                     tr("Reverse calculation using an genetic algorithm."));
}

int GAReverse::priority() const
{
    return _priority;
}

Version GAReverse::version()
{
   return Version("0.1");
}

QString GAReverse::errorString ()
{
    return QString();
}

void GAReverse::setAnalysis(SharedDrugResponseAnalysis model)
{
    predictor = model;
}

bool GAReverse::checkForValidSolution(Prediction* _pred, const Duration &start, ParameterType type, const SharedDosage &solution, double residualMin, double residualMax, double peakMin, double peakMax, const Duration &peakTime) const {

    //If no valid dosage, not a valid solution
    if(solution->interval().toSeconds() < 0.0 || solution->dose().value() < 0.0)
        return false;

    //If no correct interval, not a valid solution
    if((residualMin < 0.0 || residualMax < 0.0) && (peakMin < 0.0 || peakMax < 0.0))
        return false;

    double peakValue;
    double residualValue;

    Duration steady = predictor->steadyState(_pred, start, type, solution);
    
    //If no steady state, not a valid solution
    if (steady.isEmpty())
        return false;

    //Test for peak in interval
    if (peakMin >= 0.0 && peakMax >= 0.0) {

        //Get the peak concentration
        peakValue = predictor->point(SharedPrediction(), steady + peakTime, type, solution).value();

        //Test for peak value in interval
        if(peakValue > peakMax || peakValue < peakMin)
            return false;
    }

    //Test for residual in interval
    if (residualMin >= 0.0 && residualMax >= 0.0) {

        //Get the residual concentration
        residualValue = predictor->point(SharedPrediction(), steady + solution->interval(), type, solution).value();

        //Test for residual in interval
        if(residualValue > residualMax || residualValue < residualMin)
            return false;
    }

    return true;
}

QMap<int, SharedDosage> GAReverse::reverseDosages(Prediction* _pred, const SharedDosage &currentDosage, const Duration &start, const SharedTargetSet &targets, const QList<Duration> &, const QList<Duration> &availableInfusions, const QList<Amount> &, ParameterType type, int nbIntakes)
{
    Q_UNUSED(availableInfusions);
    Q_UNUSED(nbIntakes);

    /*Fetch the options*/
    const int keepNBest = option(KEEP_THE_N_BEST).value.toInt();
    const int popSize = option(POP_SIZE).value.toInt();
    const int nbGen = option(NB_GEN).value.toInt();
    const double crossP = option(CROSS_P).value.toDouble();
    const double mutP = option(MUT_P).value.toDouble();
    const double mutBitP = option(MUT_BIT_P).value.toDouble();
    const int eliteSelectNum = option(ELITE_SELECT_NUM).value.toInt();
    const int individualSelectNum = option(INDIVIDUAL_SELECT_NUM).value.toInt();

    QMap<int, SharedDosage> result;

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

    SharedDosage dosage = currentDosage;
    dosage->setApplied(predictor->treatment()->dosages()->firsttake().addSecs(start.seconds()));

    Elitism eliteSelection;
    RankBasedSelection individualsSelection;
    Toggling mutateMethod;
    OnePoint crossoverMethod;

    /*Name, size, individual genome length*/
    Population pop("Ezechiel", popSize, 21);
    /*population, number of generation, crossoverP, mutationP, mutationPerbitP*/
    EvolutionEzechiel evoEngine(&pop, nbGen, crossP, mutP, mutBitP, dosage, start, targets, type, predictor);
    /*leftLock, rightLock, number of generation, elite selection, number of elite to select, individual selection, number of individual to select, mutation, crossover, number of cooperators*/
    evoEngine.startEvolution(_pred, NULL, NULL, nbGen, &eliteSelection, eliteSelectNum, &individualsSelection, individualSelectNum, &mutateMethod, &crossoverMethod, 1);

    /*Get last population*/
    Population *popResult = evoEngine.getPopulation();

    /******* Log last generation for debuging purpose *******/
    /*
    QFile file("GA.txt");
    QTextStream fileStream(&file);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    fileStream << "Dosage,Interval,Fitness" << endl;
    */
    /********************************************************/

    /*Get nbest entities in the population*/
    vector<PopEntity*> bestEntities = popResult->getSomeEntity(&eliteSelection, keepNBest);

    QBitArray *genoData;
    unsigned int intervalI;
    unsigned int dosageI;

    /*Decode the genome*/
    for(unsigned int i=0; i<bestEntities.size(); i++){
        genoData = bestEntities.at(i)->getGenotype()->getData();

        intervalI = 0;
        dosageI = 0;

        for(unsigned int j=0; j<11; j++){
            intervalI = intervalI << 1;
            intervalI += static_cast<int>(genoData->at(j));
        }
        for(unsigned int j=11; j<21; j++){
            dosageI = dosageI << 1;
            dosageI += static_cast<int>(genoData->at(j));
        }

        Duration interval(0, intervalI);
        double dose = static_cast<double>(dosageI);

        //qDebug() << "Found :  " << "interval " << interval.toMinutes() << " Dose " << dose << " Fitness " << bestEntities.at(i)->getFitness();
        //fileStream << dose << ',' << interval.toMinutes() << ',' << 1.0/bestEntities.at(i)->getFitness() << endl;

        dosage->dose().setValue(dose);
        dosage->setInterval(interval);

        /*Last check for valid solution*/
        if(checkForValidSolution(_pred, start, type, dosage, residualMin, residualMax, peakMin, peakMax, peakTime))
            result.insert(i + 1, dosage);
    }

    //file.close();
    return result;
}
