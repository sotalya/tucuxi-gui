/**
 * @file popentity.cpp
 * @author Yvan Da Silva <yvan.m.silva AT gmail.com>
 * @author IICT Institute for Information and Communication<www.iict.ch>
 * @author HEIG-VD (Haute école d'inénierie et de gestion) <www.heig-vd.ch>
 * @date 02.2012
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
 * @class PopEntity
 * @brief A PopEntity is an entity that is part of a population,
 * it is a population entity that have a fitness that makes sense inside
 * a certain population.
 *
 */
#include "popentity.h"

/**
 * @brief PopEntity::PopEntity
 * Creates a population entity with a default lenght for the genome.
 *
 * @param lenght The genome lenght
 */
PopEntity::PopEntity(quint32 lenght) :
    genotype(new Genotype(lenght)),
    fitness(0)
{
}

/**
 * @brief PopEntity::~PopEntity
 * Cleanly destroys a population entity.
 *
 */
PopEntity::~PopEntity(){
    if(genotype != 0)
        delete genotype;
}

/**
 * @brief PopEntity::PopEntity
 * Creates a new population entity with a specified genotype.
 *
 * @param genotype The genotype to be used
 */
PopEntity::PopEntity(Genotype *genotype) :
             genotype(genotype->getCopy()),
             fitness(0)
{
}

/**
 * @brief PopEntity::PopEntity
 * Copy a population entity.
 *
 * @param popEntity The population entity to be copied
 */
PopEntity::PopEntity(PopEntity *popEntity) :
             genotype(new Genotype(popEntity->getGenotype()->getData())),
             fitness(popEntity->getFitness())
{
}

/**
 * @brief PopEntity::operator <
 * Operator '<' to be used for sort operations.
 *
 * @param rEntity entity to compare with
 * @return true if right entity as a beter fitness than this population entity
 */
bool PopEntity::operator< (PopEntity *rEntity)
{
        return getFitness() < rEntity->getFitness();
}

/**
 * @brief PopEntity::setFitness
 * Change this population entity fitness.
 *
 * @param fitness The new fitness
 */
void PopEntity::setFitness(qreal fitness){
    this->fitness = fitness;
}

/**
 * @brief PopEntity::getFitness
 * Get the fitness.
 *
 * @return The current fitness
 */
qreal PopEntity::getFitness()
{
    return fitness;
}

/**
 * @brief PopEntity::getGenotype
 * Get the genotype.
 *
 * @return The current genotype
 */
Genotype *PopEntity::getGenotype()
{
    return genotype;
}

/**
 * @brief PopEntity::getCopy
 * Constructs a new copy of this entity.
 *
 * @return The a copy of this entity.
 */
PopEntity *PopEntity::getCopy()
{
    return new PopEntity(this);
}
