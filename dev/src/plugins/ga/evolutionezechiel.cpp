//@@license@@

#include "evolutionezechiel.h"
#include <vector>
#include <float.h>

EvolutionEzechiel::EvolutionEzechiel(Population *population, quint32 generationCount, qreal crossoverProbability, qreal mutationProbability, qreal mutationPerBitProbability,
                                     const SharedDosage &currentDosage, const Duration &start, const SharedTargetSet &targets, ParameterType type, SharedDrugResponseAnalysis predictor) :
                                     EvolutionEngine(population, generationCount, crossoverProbability, mutationProbability, mutationPerBitProbability),
                                     dosage(currentDosage),
                                     start(start),
                                     targets(targets),
                                     type(type),
                                     predictor(predictor)
{
    errorWeight.insert(Target::ResidualTarget, 1.0);
    errorWeight.insert(Target::PeakTarget, 1.0);
}

EvolutionEzechiel::~EvolutionEzechiel()
{
}

double EvolutionEzechiel::scoring(const Duration &at, const ParameterType type, const SharedDosage &dosage, const SharedTargetSet &targets, QHash<Target::TargetType, double> errorWeight) const
{
    double residualWanted = -1.0;
    double residualMax;
    double residualMin;
    double peakWanted = -1.0;
    double peakMin;
    double peakMax;
    Duration peakTime;

    foreach (Target* t, *targets.data()){
       switch(t->type()){
       case Target::ResidualTarget:
           residualWanted = t->cbest().value();
           residualMax = t->cmax().value();
           residualMin = t->cmin().value();
           break;
       case Target::PeakTarget:
           peakWanted = t->cbest().value();
           peakMax = t->cmax().value();
           peakMin = t->cmin().value();
           peakTime = t->tbest();
           break;
       default:;
       }
    }

    double peakValue;
    double peakDiff = 0;
    double residualValue;
    double residualDiff = 0;

    //If peak target found
    if (peakWanted != -1.0) {

        //Get the peak concentration and difference
        peakValue = predictor->point(SharedPrediction(), at + peakTime, type, dosage).value();
        peakDiff = fabs(peakValue - peakWanted);

        //Add some penalities if the value is not in interval
        if(peakValue < peakMin || peakValue > peakMax)
            peakDiff *= 100;
    } else //Else set coefficient to 0.0
        errorWeight.insert(Target::PeakTarget, 0.0);

    //If residual target found
    if (residualWanted != -1.0) {

        //Get the residual concentration and value
        residualValue = predictor->point(SharedPrediction(), at + dosage->interval(), type, dosage).value();
        residualDiff = fabs(residualValue - residualWanted);

        /*Add some penalities if the value is not in interval*/
        if(residualValue < residualMin || residualValue > residualMax)
            residualDiff *= 100;
    } else //Else set the coefficient to 0.0
        errorWeight.insert(Target::ResidualTarget, 0.0);

    //ToDo: calcul a score, with coef and diff values
    return (peakDiff * errorWeight.value(Target::PeakTarget, 0.0)) + (residualDiff * errorWeight.value(Target::ResidualTarget, 0.0));
}

bool EvolutionEzechiel::BiggerThan::operator() (PopEntity* a, PopEntity* b) const{
    return a->getFitness() > b->getFitness();
}


bool EvolutionEzechiel::evaluatePopulation(Prediction* _pred, Population *pop, quint32 generation)
{
    QBitArray *genoData;
    Duration steady;
    double score;
    unsigned int intervalI;
    unsigned int dosageI;
    double previousIntervalI = -1.0;
    double diff;
    double fitness;

    vector<PopEntity *> entities =  pop->getAllEntities();

    /*Sort the population by fitness*/
    sort(entities.begin(), entities.end(), biggerThan);

    //qDebug() << "--------Generation " << generation << "--------";

    /*Decode the genome of each entity and calcul and assign new fitness*/
    vector<PopEntity *>::const_iterator itEntity;
    for(itEntity = entities.begin(); itEntity != entities.end(); ++itEntity){
        genoData = (*itEntity)->getGenotype()->getData();

        intervalI = 0;
        dosageI = 0;

        for(unsigned int j=0; j<11; j++){
            intervalI = intervalI << 1;
            intervalI += static_cast<int>((*genoData)[j]);
            //qDebug() << static_cast<int>((*genoData)[j]);
        }

        for(unsigned int j=11; j<21; j++){
            dosageI = dosageI << 1;
            dosageI += static_cast<int>((*genoData)[j]);
            //qDebug() << static_cast<int>((*genoData)[j]);
        }

        Duration interval(0, intervalI > 0 ? intervalI : 1);
        double dose = static_cast<double>(dosageI > 0 ? dosageI : 1);

        dosage->dose().setValue(dose);
        dosage->setInterval(interval);
        steady = predictor->steadyState(_pred, start, type, dosage);

        /*Ignore this dosage if no steady state*/
        if (steady.isEmpty())
            continue;

        score = scoring(steady, type, dosage, targets, errorWeight);

        fitness = score != 0.0 ? 1.0/score : DBL_MAX;

        /*Check and apply a fitness penality if entity is close to previous one*/
        if(previousIntervalI  != -1.0 && generation != generationCount && (diff=fabs(interval.toMinutes() - previousIntervalI)) < 30.0){
            //fitness = fitness/((15.0-diff)/100.0);
            fitness /= 1000;
        }
        previousIntervalI = interval.toMinutes();

        (*itEntity)->setFitness(fitness);

        //qDebug() << "interval " << interval.toMinutes() << " Dose " << dose << " Fitness " << (*itEntity)->getFitness();
    }

    return true;
}
