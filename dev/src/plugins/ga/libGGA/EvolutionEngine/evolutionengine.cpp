/**
  * @file   evolutionengine.cpp
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

#include "evolutionengine.h"
//#include "../computethread.h"

QMutex * EvolutionEngine::critMutex = new QMutex();

/**
 * @brief EvolutionEngine::EvolutionEngine
 * Constructs an evolution engine.
 *
 * @param population Population to evolve
 * @param generationCount number of generations to evolve
 * @param crossoverProbability probability of reproduction between individuals
 * @param mutationProbability probability of a mutation of an individual.
 * @param mutationPerBitProbability genome bits probability to evolve
 */
EvolutionEngine::EvolutionEngine(Population *population, quint32 generationCount, qreal crossoverProbability, qreal mutationProbability, qreal mutationPerBitProbability) :
    population(population), generationCount(generationCount), crossoverProbability(crossoverProbability), mutationProbability(mutationProbability), mutationPerBitProbability(mutationPerBitProbability)
{
    //entitySelectionMethodList.push_back(new Elitism());
    entitySelectionMethodList.push_back(new ElitismWithRandom());
    entitySelectionMethodList.push_back(new RankBasedSelection());

    crossoverMethodList.push_back(new OnePoint());
    mutateMethodList.push_back(new Toggling());
}

EvolutionEngine::~EvolutionEngine()
{
}

/**
 * @brief EvolutionEngine::startEvolution
 * Starts the evolution of the population.
 *
 * @param leftLock Lock of synchro
 * @param rightLock Lock of synchro
 * @param generationCount Number of generation to evolve
 * @param eliteSelection Selection method of elites
 * @param eliteSelectionCount Number of elites to select
 * @param individualsSelection Selection method of individuals
 * @param individualsSelectionCount Number of individuals to select
 * @param mutateMethod Mutation method
 * @param crossoverMethod Crossover method
 * @param cooperatorsCount Number of cooperators to make available
 */
void EvolutionEngine::startEvolution(Prediction* _pred, QMutex *leftLock, QMutex *rightLock, quint32 generationCount, EntitySelection *eliteSelection, quint32 eliteSelectionCount, EntitySelection *individualsSelection, quint32 individualsSelectionCount, Mutate *mutateMethod, Crossover *crossoverMethod, quint32 cooperatorsCount)
{
    setEntitySelector(eliteSelection,eliteSelectionCount,individualsSelection,individualsSelectionCount);

    this->mutateMethod = mutateMethod;
    this->crossoverMethod = crossoverMethod;

    if(leftLock && rightLock)
        rightLock->lock();
    selectElites();
    population->setRepresentativesCopy(selectedEntitiesCopy, cooperatorsCount);

    // JOIN both evolution
    if(leftLock && rightLock){
        leftLock->unlock();
        rightLock->lock();
        leftLock->unlock();
    }
    if(!evaluatePopulation(_pred, population, 0))
        return;

    // Simple genetic algorithm.
    for(quint32 i = 1; i <= generationCount; i++)
    {
        // Select elites from the population
        selectElites();

        // Set cooperators
        population->setRepresentativesCopy(selectedEntitiesCopy, cooperatorsCount);

        // Select non elite individuals
        selectIndividuals();

        // Crossover
        crossover();

        // Mutate
        mutate();
        population->replace(selectedEntitiesCopy, evolvingEntitiesCopy);

        // Evaluate population
        if(!evaluatePopulation(_pred, population, i))
            break;
    }
}

/**
 * @brief EvolutionEngine::setEntitySelector
 * Set all selectors and quantities.
 *
 * @param eliteSelection Selection method of the elites
 * @param eliteSelectionCount Number of elites to be selected
 * @param entitySelection Selection method of entities
 * @param selectionCount Number of entities to be selected
 */
void EvolutionEngine::setEntitySelector(EntitySelection *eliteSelection, quint32 eliteSelectionCount, EntitySelection *entitySelection, quint32 selectionCount)
{
    this->eliteSelection = eliteSelection;
    this->eliteSelectionCount = eliteSelectionCount;
    this->individualsSelection = entitySelection;
    this->individualsSelectionCount = selectionCount;
}

/**
 * @brief EvolutionEngine::setMutationMethod
 * Sets the mutation method and the mutation probability.
 *
 * @param mutateMethod The mutation method
 * @param mutationProbability The mutation probability
 */
void EvolutionEngine::setMutationMethod(Mutate * mutateMethod, quint32 mutationProbability)
{
    this->mutateMethod = mutateMethod;
    this->mutationProbability = mutationProbability;
}

/**
 * @brief EvolutionEngine::setCrossoverMethod
 * Sets the crossover method.
 *
 * @param crossoverMethod The crossover method
 */
void EvolutionEngine::setCrossoverMethod(Crossover * crossoverMethod)
{
    this->crossoverMethod = crossoverMethod;
}

/**
 * @brief EvolutionEngine::getPopulation
 * Get the current population
 *
 * @return The population
 */
Population *EvolutionEngine::getPopulation(){
    return population;
}

/**
 * @brief EvolutionEngine::getEntitySelectors
 * Get the available entity selectors.
 *
 * @return The available selectors
 */
vector<EntitySelection *> EvolutionEngine::getEntitySelectors()
{
    vector<EntitySelection *> availableSelection;
    for(quint32 i=0; i < entitySelectionMethodList.size(); i++)
        availableSelection.push_back(entitySelectionMethodList.at(i));
    return availableSelection;
}

/**
 * @brief EvolutionEngine::getMutationMethods
 * Get the available mutation methods.
 *
 * @return The available mutation methods.
 */
vector<Mutate *> EvolutionEngine::getMutationMethods()
{
    vector<Mutate *> availableMutation;
    for(quint32 i=0; i < mutateMethodList.size(); i++)
        availableMutation.push_back(mutateMethodList.at(i));
    return availableMutation;
}

/**
 * @brief EvolutionEngine::getCrossoverMethods
 * Get the available crossover methods.
 *
 * @return The available crossover methods.
 */
vector<Crossover *> EvolutionEngine::getCrossoverMethods()
{
    vector<Crossover *> availableCrossover;
    for(quint32 i=0; i < crossoverMethodList.size(); i++)
        availableCrossover.push_back(crossoverMethodList.at(i));
    return availableCrossover;
}

// Private :

/**
 * @brief EvolutionEngine::initializePopulation
 * Initialize the population with random individuals.
 *
 */
void EvolutionEngine::initializePopulation()
{
    population->randomizePopulation();
}

/**
 * @brief EvolutionEngine::selectElites
 * Select elites.
 *
 */
void EvolutionEngine::selectElites()
{
    selectedEntitiesCopy.clear();
    selectedEntitiesCopy = population->getSomeEntityCopy(eliteSelection,eliteSelectionCount);

}

/**
 * @brief EvolutionEngine::isElite
 * Check if a certain genotype is a part of the current elites.
 *
 * @param genotype The genotype
 * @return
 */
bool EvolutionEngine::isElite(Genotype *genotype)
{
    for(quint32 i=0; i < selectedEntitiesCopy.size(); i++){
        if(genotype->getData()->operator ==(*selectedEntitiesCopy.at(i)->getGenotype()->getData()))
            return true;
    }
    return false;
}

/**
 * @brief EvolutionEngine::selectIndividuals
 * Select Individuals.
 *
 */
void EvolutionEngine::selectIndividuals()
{
    evolvingEntitiesCopy.clear();
    evolvingEntitiesCopy = population->getSomeEntityCopy(individualsSelection,individualsSelectionCount);
}

/**
 * @brief EvolutionEngine::crossover
 * Try to reproduce population individuals.
 *
 */
void EvolutionEngine::crossover()
{
    // Missing probability
    // Random list before crossover (To do if better results and not too processing time consuming)
    crossoverMethod->reproducePairOf(evolvingEntitiesCopy, crossoverProbability);
}

/**
 * @brief EvolutionEngine::mutate
 * Try to mutate population individuals.
 *
 */
void EvolutionEngine::mutate()
{

    vector<PopEntity *>::iterator it;
    for(it=evolvingEntitiesCopy.begin(); it!=evolvingEntitiesCopy.end(); it++)
    {
        qreal entitiyLuck = RandomGenerator::getGeneratorInstance()->randomReal(0,1);
        //  if goal < chance OK.
        if(entitiyLuck < mutationProbability){
            mutateMethod->mutateEntity(*it, mutationPerBitProbability);
        }
    }
}

/**
 * @brief EvolutionEngine::getStatisticEngine
 * Provide the current statistic engine.
 *
 * @return The current statistic engine
 */
StatisticEngine *EvolutionEngine::getStatisticEngine(){
    return &statsEngine;
}
