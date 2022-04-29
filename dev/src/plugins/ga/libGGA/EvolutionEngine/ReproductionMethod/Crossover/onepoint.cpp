//@@license@@

/**
  * @file   onepoint.cpp
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
  * @class OnePoint
  *
  * @brief  A one point crossover is a method of reproduction capable of
  * creating two new entities from two parents entitites.
  * One point method consist in cuting in two parts both parents entities
  * by randomly choosing the position of the cut. Then exchange the first
  * part of the parent genome with the second parent genome first part.
  * Two new individuals are then created that are the fruit of a combination
  * of both parents.
  */

#include "onepoint.h"
/**
 * @brief OnePoint::OnePoint
 * Constructs a one point crossover method.
 */
OnePoint::OnePoint() : Crossover()
{
}

/**
 * @brief OnePoint::reproducePairOf
 * Reproduce each pair of entities with a certain probability.
 *
 * @param pairOfEntityList A list of entities to reproduce in pair
 * @param probability The probability of a couple to reproduce
 */
void OnePoint::reproducePairOf(vector<PopEntity *> pairOfEntityList, qreal probability){

    for(quint32 i = 0; i < pairOfEntityList.size()-1; i+=2)
    {
        int j=i+1; // Second entity
        qreal entitiyLuck = RandomGenerator::getGeneratorInstance()->randomReal(0,1);

        //  if goal < chance OK.
        if(entitiyLuck < probability){
            // Never exchange the whole genotype, must be at least 1bit of the other part.
            QBitArray *temp = pairOfEntityList.at(i)->getGenotype()->getDataCopy();
            quint32 cutPoint = RandomGenerator::getGeneratorInstance()->random(1,temp->size()-2);

            for(quint32 k = cutPoint; k < pairOfEntityList.at(i)->getGenotype()->getLength()-1; k++)
            {
                //G2 cut go to temp
                temp->setBit(k,pairOfEntityList.at(j)->getGenotype()->getData()->at(k));
                //G1 cut go to G2
                pairOfEntityList.at(j)->getGenotype()->getData()->setBit(k, pairOfEntityList.at(i)->getGenotype()->getData()->at(k));
                //temp cut go to G1
                pairOfEntityList.at(i)->getGenotype()->getData()->setBit(k, temp->at(k));
            }
            delete temp;
        }

    }
}
