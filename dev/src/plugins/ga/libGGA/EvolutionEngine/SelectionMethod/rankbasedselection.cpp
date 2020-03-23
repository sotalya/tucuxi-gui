/**
 * @file rankbasedselection.cpp
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
 * @class RankBasedSelection
 * @brief Select a number of entities of an entity list by :
 * 1) Choosing a random 10 percent of the entity list entities.
 * 2) Applying an elitism based selection of those 10 percent to acquire one entity.
 * 3) Restarting the process until the required number of entities selected is reached.
 *
 */

#include "rankbasedselection.h"

/**
 * @brief RankBasedSelection::RankBasedSelection
 * Constructs a selection method based on the ranking selection algorithm.
 *
 */
RankBasedSelection::RankBasedSelection() : EntitySelection()
{
}

/**
 * @brief RankBasedSelection::selectEntities
 * Select a certain number of entities.
 *
 * @param quantity Number of entities to be selected
 * @param entityList The entity list to select from
 * @return A vector of entities selected
 */
vector<PopEntity *> RankBasedSelection::selectEntities(quint32 quantity, vector<PopEntity *> entityList)
{
    // THERE IS A SMALL CHANCE, that we get multiple copy of the same individual.

    vector<PopEntity *> selectedEntities;

    PopEntity *tempEntity;
    quint32 pos;
    qreal bestfitness;
    qreal fitness;
    for(quint32 q = 0; q < quantity; q++){
        bestfitness = 0.0;
        for(quint32 i = 0; i < entityList.size()/10; i++){
            pos = RandomGenerator::getGeneratorInstance()->random(0,entityList.size()-1);
            tempEntity = entityList.at(pos);
            fitness = tempEntity->getFitness();

            if(fitness > bestfitness){
                bestfitness = fitness;
                tempEntity = entityList.at(pos);
            }
        }
        selectedEntities.push_back(tempEntity);
    }

    return selectedEntities;

}
