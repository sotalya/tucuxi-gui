//@@license@@

#ifndef EVOLUTIONEZECHIEL_H
#define EVOLUTIONEZECHIEL_H

#include "evolutionengine.h"
#include "../dal/parameters.h"
#include "drugresponseanalysis.h"
#include "drugtreatment.h"
#include "prediction.h"


class EvolutionEzechiel : public EvolutionEngine
{
public:
    EvolutionEzechiel(Population *population, quint32 generationCount, qreal crossoverProbability, qreal mutationProbability, qreal mutationPerBitProbability,
                      const SharedDosage &currentDosage, const Duration &start, const SharedTargetSet &targets, ParameterType type, SharedDrugResponseAnalysis predictor);
    virtual ~EvolutionEzechiel();
    virtual bool evaluatePopulation(Prediction* _pred, Population* population, quint32 generation);

private:
    SharedDosage dosage;
    const Duration &start;
    const SharedTargetSet &targets;
    ParameterType type;
    SharedDrugResponseAnalysis predictor;

    QHash<Target::TargetType, double> errorWeight;

    double scoring(const Duration &at, const ParameterType type, const SharedDosage &dosage, const SharedTargetSet &targets, QHash<Target::TargetType, double> errorWeight) const;

    class BiggerThan{
    public:
        bool operator() (PopEntity* a, PopEntity* b) const;
    } biggerThan;
};

#endif // EVOLUTIONEZECHIEL_H
