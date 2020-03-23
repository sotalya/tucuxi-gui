/**
 * @file popentity.h
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

#ifndef POPENTITY_H
#define POPENTITY_H

#include <memory>
#include <Qt>

#include "genotype.h"
#include "randomgenerator.h"

class PopEntity
{
public:
    PopEntity();
    virtual ~PopEntity();
public:
    PopEntity(quint32 lenght);
    PopEntity(Genotype *genotype);
    PopEntity(PopEntity *popEntity);
    virtual bool operator< (PopEntity *rEntity);

    void setFitness(qreal fitness);
    qreal getFitness();
    virtual Genotype *getGenotype();
    virtual PopEntity *getCopy();

protected:
    Genotype *genotype;
    RandomGenerator *randomGenerator;
    qreal fitness;
};

#endif // POPENTITY_H
