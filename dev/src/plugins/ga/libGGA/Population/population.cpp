/**
 * @file population.cpp
 * @author Yvan Da Silva <yvan.m.silva AT gmail.com>
 * @author IICT Institute for Information and Communication<www.iict.ch>
 * @author HEIG-VD (Haute école d'inénierie et de gestion) <www.heig-vd.ch>
 * @date 06.2012
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
 * @class Population
 * @brief A population is described by a name and a number of entities (individuals)
 * that are part of the population.
 * A population can receive and give entities to others but has no control on the
 * individuals themselves.
 *
 */
#include "population.h"

/**
 * @brief Population::~Population
 * Destroys a population and all of its Individuals.
 *
 */
Population::~Population(){
    for(unsigned int i = 0; i < entityList.size(); i++)
        delete entityList.at(i);
    entityList.clear();
}

/**
 * @brief Population::Population
 * Constructs a population with a given name, size and genome lenght for each individual.
 *
 * @param name The name of the population
 * @param size The size of the population (Number of individuals)
 * @param individualsLength The genome lenght of each individual
 */
Population::Population(QString name, quint32 size, quint32 individualsLength) : name(name)
{
    for(unsigned int i = 0; i < entityList.size(); i++)
        delete entityList.at(i);
    entityList.clear();
    for(quint32 i = 0; i < size; i++)
    {
        entityList.push_back(new PopEntity(individualsLength));
    }
    randomizePopulation();
}

/**
 * @brief Population::Population
 * Constructs a population from an existing population and asign to it a new name.
 * (Works like a copy constructor, but allows to change the population name)
 *
 * @param population The population to be copied
 * @param name The new name of the population
 */
Population::Population(Population *population, QString name)
{
    for(unsigned int i = 0; i < entityList.size(); i++)
        delete entityList.at(i);
    entityList.clear();
    for(quint32 i = 0; i < population->getSize(); i++)
    {
        entityList.push_back(population->getEntityCopyFrom(i));
    }
    this->name = name;
}

/**
 * @brief Population::setRepresentativesCopy
 * Copy the given representatives and set them as new representatives for the population.
 *
 * @param representatives The entities to be copied
 * @param quantity The number of entities to be copied
 */
void Population::setRepresentativesCopy(vector<PopEntity *> representatives, int quantity){
    int nbCooperator = quantity;
    QMutexLocker locker(&mutex);

    // Delete old entities.
    for(unsigned int i = 0; i < this->representatives.size(); i++)
        delete this->representatives[i];
    this->representatives.clear();

    // Random Cooperator (Working but not needed cooperators are the best ones only)
    //        if(nbCooperator > 1){
    //            nbCooperator--;
    //            int pos = RandomGenerator::getGeneratorInstance()->random(0,entityList.size()-1);
    //            this->representatives.push_back(new PopEntity(entityList.at(pos)));
    //        }


    vector<PopEntity *>::iterator it;
    for(it=representatives.begin(); it!= representatives.begin()+nbCooperator; it++){
        this->representatives.push_back(new PopEntity(*it));
    }

}

/**
 * @brief Population::getRepresentativesCopy
 * Copies of the current representatives.
 *
 * @return Copies of the current representatives
 */
vector<PopEntity *> Population::getRepresentativesCopy(){
    QMutexLocker locker(&mutex);
    vector<PopEntity *> temp;
    vector<PopEntity *>::iterator it;
    for(it=representatives.begin(); it!=representatives.end(); it++)
        temp.push_back(new PopEntity(*it));
    return temp;
}

/**
 * @brief Population::getName
 * The current population name.
 *
 * @return Population name
 */
QString Population::getName(){
    return name;
}

/**
 * @brief Population::replace
 * Replace the existing population with a new one.
 *
 * @param newPopulation The new population to replace this
 */
void Population::replace(vector<PopEntity *> newPopulation)
{
    QMutexLocker locker(&mutex);
    for(unsigned int i = 0; i < entityList.size(); i++)
        delete entityList.at(i);
    entityList.clear();

    vector<PopEntity *>::iterator it;
    for(it=newPopulation.begin(); it!=newPopulation.end(); it++)
    {
        entityList.push_back(*it);
    }
}

/**
 * @brief Population::replace
 * Replace the population with two different populations.
 *
 * @param base First part of the population
 * @param generatedPartPopulation Second part of the population
 */
void Population::replace(vector<PopEntity *> base, vector<PopEntity *> generatedPartPopulation)
{
    replace(generatedPartPopulation);

    vector<PopEntity *>::iterator it;
    for(it=base.begin(); it!=base.end(); it++)
    {
        entityList.push_back(*it);
    }
}

/**
 * @brief Population::getSize
 * The size of the population.
 *
 * @return The size of the population
 */
quint32 Population::getSize()
{
    return entityList.size();
}

/**
 * @brief Population::randomizePopulation
 * Randomize each individual of the population by generating random genomes.
 *
 */
void Population::randomizePopulation()
{
    QBitArray *data;
    bool status = false;
    for(quint32 i = 0; i < getSize(); i++)
    {
        entityList.at(i)->setFitness(0.f);
        data = entityList.at(i)->getGenotype()->getData();
        for(int j=0; j < data->size(); j++)
        {
            status = RandomGenerator::getGeneratorInstance()->random(0,1);
            data->setBit(j,status);
        }
    }
}

/**
 * @brief Population::getEntityCopyFrom
 * Get a copy of an entity at a certain position of the population.
 *
 * @param pos The pos in the population structure
 * @return A copy of the Entity (Individual)
 */
PopEntity *Population::getEntityCopyFrom(quint32 pos)
{
    return getEntityAt(pos)->getCopy();
}

/**
 * @brief Population::getEntityAt
 * Get an entity at a certain position of the population.
 *
 * @param pos The pos in the population structure
 * @return The entity (Individual)
 */
PopEntity *Population::getEntityAt(quint32 pos)
{
    return entityList.at(pos);
}

/**
 * @brief Population::getSomeEntityCopy
 * Get few entities copy by using a specific method of selection.
 *
 * @param entitySelection The method of selection
 * @param count The number of entities to be selected
 * @return A vector of copies of the entities selected
 */
vector<PopEntity *> Population::getSomeEntityCopy(EntitySelection *entitySelection, quint32 count)
{
    vector<PopEntity *>::iterator it;
    vector<PopEntity *> temp;
    vector<PopEntity *> selected = entitySelection->selectEntities(count, entityList);

    for(it=selected.begin(); it!=selected.end(); it++){
        temp.push_back(new PopEntity(*it));
    }
    return temp;
}

/**
 * @brief Population::getSomeEntity
 * Get few entities by using a specific method of selection.
 *
 * @param entitySelection The method of selection
 * @param count The number of entities to be selected
 * @return A vector of the entities selected
 */
vector<PopEntity *> Population::getSomeEntity(EntitySelection *entitySelection, quint32 count)
{
    return entitySelection->selectEntities(count, entityList);
}

/**
 * @brief Population::getAllEntitiesCopy
 * Get a Copy of all entites of the population.
 *
 * @return The copy of all entities of the population
 */
vector<PopEntity *> Population::getAllEntitiesCopy()
{
    vector<PopEntity *>::iterator it;
    vector<PopEntity *> temp;
    for(it=entityList.begin(); it!=entityList.end(); it++){
        temp.push_back(new PopEntity(*it));
    }
    return temp;
}

/**
 * @brief Population::getAllEntities
 * Get the entites of the population.
 *
 * @return The entities of the population
 */
vector<PopEntity *> Population::getAllEntities()
{
    return entityList;
}
