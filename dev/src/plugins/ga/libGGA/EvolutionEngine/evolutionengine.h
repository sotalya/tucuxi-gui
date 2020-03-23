/**
  * @file   evolutionengine.h
  * @author Yvan Da Silva <yvan.dasilva_at_heig-vd.ch>
  * @author ReDS (Reconfigurable and embedded digital systems) <www.reds.ch>
  * @author HEIG-VD (Haute école d'ingénierie et de gestion) <www.heig-vd.ch>
  * @date   06.2012
  * @section LICENSE
  *
  * This application is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 3.0 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
  *
  * @class EvolutionEngine
  *
  * @brief An evolution engine uses an algorithme to pass a population from a generation zero
  * to a generation one and so on. By evolving, mutating and keeping the best individuals
  * the goal of the evolution engine is to provide a better population.
  */

#ifndef EVOLUTIONENGINE_H
#define EVOLUTIONENGINE_H

#include <vector>
#include <QThread>

#include "population.h"
#include "crossover.h"
#include "onepoint.h"
#include "mutate.h"
#include "toggling.h"
#include "entityselection.h"
#include "elitism.h"
#include "elitismwithrandom.h"
#include "rankbasedselection.h"
#include "statisticengine.h"

class Prediction;

class EvolutionEngine
{
public:
    EvolutionEngine(Population *population, quint32 generationCount, qreal crossoverProbability, qreal mutationProbability, qreal mutationPerBitProbability);
    virtual ~EvolutionEngine();
    virtual bool evaluatePopulation(Prediction* _pred, Population* population, quint32 generation) = 0;
    void startEvolution(Prediction* _pred,
                        QMutex *leftLock,
                                            QMutex *rightLock,
                                            quint32 generationCount,
                                            EntitySelection *eliteSelection,
                                            quint32 eliteSelectionCount,
                                            EntitySelection *individualsSelection,
                                            quint32 individualsSelectionCount,
                                            Mutate *mutateMethod, Crossover *crossoverMethod, quint32 cooperatorsCount);
    void setEntitySelector(EntitySelection *eliteSelection, quint32 eliteSelectionCount, EntitySelection * individualsSelection, quint32 individualsSelectionCount);
    void setMutationMethod(Mutate * mutateMethod, quint32 mutationProbability);
    void setCrossoverMethod(Crossover * crossoverMethod);

    void replacePopulation();
    void replacePopulation(Population *population);
    bool isElite(Genotype *genotype);

    StatisticEngine *getStatisticEngine();
    Population *getPopulation();

    vector<EntitySelection *> getEntitySelectors();
    vector<Mutate *> getMutationMethods();
    vector<Crossover *> getCrossoverMethods();

    static QMutex * critMutex;

protected:
    quint32 generationCount;
    StatisticEngine statsEngine;

private:
    void initializePopulation();
    void selectElites();
    void selectIndividuals();
    void crossover();
    void mutate();

    Population *population;
    qreal crossoverProbability;
    qreal mutationProbability;
    qreal mutationPerBitProbability;

    EntitySelection *eliteSelection;
    quint32 eliteSelectionCount;
    EntitySelection *individualsSelection;
    quint32 individualsSelectionCount;
    Mutate *mutateMethod;
    Crossover *crossoverMethod;

    vector<EntitySelection *> entitySelectionMethodList;
    vector<Mutate *> mutateMethodList;
    vector<Crossover *> crossoverMethodList;

    vector<PopEntity *> selectedEntitiesCopy;
    vector<PopEntity *> evolvingEntitiesCopy;
};

#endif // EVOLUTIONENGINE_H
