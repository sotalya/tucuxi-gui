//@@license@@

/**
 * @file toggling.cpp
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
 * @class Toggling
 * @brief Toggling is a method of mutation that act on the genotype
 * of a population entity. It alternates the states of one or more
 * bits composing the genotype depending on a mutation and mutation per bit
 * probability.
 *
 */

#include "toggling.h"

/**
 * @brief Toggling::Toggling
 * Constructs a method of toggling.
 *
 */
Toggling::Toggling()
{
}

/**
 * @brief Toggling::mutateEntity
 * Mutate an entity by applying a per bit probability if probability is not zero.
 * The mutation will give a chance to each bit to toggle (1 if it was 0, 0 if it was 1).
 * In case mutation probability per bit was set to zero, a random bit in the genome is
 * choosen and toggled.
 *
 * @param entity The entity to be mutated
 * @param mutationPerBitProbability The probability of each bit to be mutated >= 0
 */
void Toggling::mutateEntity(PopEntity *entity, qreal mutationPerBitProbability)
{
    // TODO : DONE: Check if not too CPU consuming : Care of per bit mutation probability
    QBitArray *genotypeData = entity->getGenotype()->getData();

    if(mutationPerBitProbability != 0){
        qreal luck = 0.0;
        for(int i=0; i < genotypeData->size(); i++){
            luck = RandomGenerator::getGeneratorInstance()->randomReal(0,1);
            if(luck < mutationPerBitProbability){
                genotypeData->toggleBit(i);
            }
        }
    }else{
        quint32 togglingPos = RandomGenerator::getGeneratorInstance()->random(0,genotypeData->size()-1);
        genotypeData->toggleBit(togglingPos);
    }

}
