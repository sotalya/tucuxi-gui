/**
  * @file   genotype.cpp
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
  * @class Genotype
  *
  * @brief  A genotype is a serie of bits, the container used is a QBitArray.
  * A genotype is used to describe an entitiy.
  */

#include "genotype.h"

/**
 * @brief Genotype::~Genotype
 * Constructs a genotype with no data (genome).

 *
 */
Genotype::~Genotype(){
    if(data != 0)
        delete data;
}

/**
 * @brief Genotype::Genotype
 * Constructs a genotype with a default data (genome) lenght.
 * A genotype is a serie of bits, the container used is a QBitArray.
 *
 * @param length The lenght of the genome
 */
Genotype::Genotype(quint32 length) : length(length),
    data(new QBitArray(length, false))
{

}

/**
 * @brief Genotype::Genotype
 * Contructs a new copy of a genotype from its genome.
 * A genotype is a serie of bits, the container used is a QBitArray.
 *
 * @param data The genotype data (genome)
 */
Genotype::Genotype(QBitArray *data)
{

    this->data = new QBitArray(*data);
    length = this->data->size();
}

/**
 * @brief Genotype::Genotype
 * Contructs a new copy of a genotype.
 * A genotype is a serie of bits, the container used is a QBitArray.
 *
 * @param genotype The genotype to copy
 */
Genotype::Genotype(Genotype *genotype)
{
    Genotype(genotype->getData());
}

/**
 * @brief Genotype::getCopy
 * Get a copy of this genotype.
 *
 * @return a copy of this genotype
 */
Genotype *Genotype::getCopy(){
    return new Genotype(data);
}

/**
 * @brief Genotype::getLength
 * Get the current lenght of the genotype data.
 *
 * @return The lenght of the genotype data
 */
quint32 Genotype::getLength(){
    return length;
}

/**
 * @brief Genotype::getData
 * Get the genome.
 *
 * @return The data (genome)
 */
QBitArray* Genotype::getData(){
    return data;
}

/**
 * @brief Genotype::getDataCopy
 * Get a copy of the genome.
 *
 * @return A copy of the data (genome)
 */
QBitArray* Genotype::getDataCopy(){
    return new QBitArray(*data);
}

/**
 * @brief Genotype::setData
 * Replace the current genome (data).
 *
 * @param data The new data (genome)
 */
void Genotype::setData(QBitArray *data){
    if(this->data!=0)
        delete data;
    this->data = data;
}
