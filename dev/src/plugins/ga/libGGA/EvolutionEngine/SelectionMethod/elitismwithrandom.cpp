/**
 * @file elitismwithrandom.cpp
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
 * @class ElitismWithRandom
 * @brief Select a number of entities of an entity list by sorting the entitiy list
 * and getting the better entities based of their fitness.
 * If the number of entities to be selected is > 1 then one of the selected entities
 * will be randomly selected.
 *
 */
#include "elitismwithrandom.h"

/**
 * @brief ElitismWithRandom::Elitism
 * Constructs a selector based on the elitism principle.
 * If the selector has to select more than a single entity
 * one of the selected entities will be randomly choosed.
 *
 */
ElitismWithRandom::ElitismWithRandom():
    EntitySelection()
{
}

inline bool entityLesserThan(PopEntity* a, PopEntity* b){
    return a->getFitness() < b->getFitness();
}

/**
 * @brief ElitismWithRandom::selectEntities
 * Select a certain number of entities.
 *
 * @param quantity Number of entities to be selected
 * @param entityList The entity list to select from
 * @return A vector of entities selected
 */
vector<PopEntity *> ElitismWithRandom::selectEntities(quint32 quantity, vector<PopEntity *> entityList)
{
    vector<PopEntity *> selectedEntities;
    sort(entityList.begin(), entityList.end(), entityLesserThan);

    for(quint32 i=entityList.size()-1; i > entityList.size()-quantity; i--){
        selectedEntities.push_back(entityList.at(i));
    }

    int pos = RandomGenerator::getGeneratorInstance()->random(0,entityList.size()-1);
    selectedEntities.push_back(entityList.at(pos));

    return selectedEntities;

}
