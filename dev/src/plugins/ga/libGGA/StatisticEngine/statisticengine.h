/**
 * @file statisticengine.h
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
 * @class StatisticEngine
 * @brief Manage the statistics for a population,
 * different statistics can be set like the minimum fitness of the population,
 * the mean, the maximum etc.
 * Numerical informations relatated to the whole population can be found in this class.
 *
 */
#ifndef STATISTICENGINE_H
#define STATISTICENGINE_H

#include <QList>
#include <vector>
#include "evolutionarymesure.h"
#include "math.h"
class StatisticEngine
{
public:
    StatisticEngine();
    void addFitness(qreal fitness);
    void buildStats();
    void reset();

    qreal getMinFitness();
    qreal getMaxFitness();
    qreal getMeanFitness();
    qreal getStandardDeviation();

private:
    QList<qreal> fitnesslist;
    qreal minfitness;
    qreal maxfitness;
    qreal meanfitness;
    qreal standardDeviation;

    QList<EvolutionaryMesure> evolutionaryMesures;
};

#endif // STATISTICENGINE_H
