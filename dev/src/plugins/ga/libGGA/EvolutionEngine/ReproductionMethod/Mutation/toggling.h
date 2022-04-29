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

#ifndef TOGGLING_H
#define TOGGLING_H

#include "mutate.h"
#include "randomgenerator.h"
#include <QDebug>

class Toggling : public Mutate
{
public:
    Toggling();
    void mutateEntity(PopEntity *entity, qreal mutationPerBitProbability);
};

#endif // TOGGLING_H
