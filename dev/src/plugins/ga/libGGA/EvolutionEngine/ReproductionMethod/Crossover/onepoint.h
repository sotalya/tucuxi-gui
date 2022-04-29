//@@license@@

/**
  * @file   onepoint.h
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

#ifndef ONEPOINT_H
#define ONEPOINT_H

#include "crossover.h"
#include "Utility/randomgenerator.h"
class OnePoint : public Crossover
{
public:
    OnePoint();
    void reproducePairOf(vector<PopEntity *> pairOfEntityList, qreal probability);
};

#endif // ONEPOINT_H
