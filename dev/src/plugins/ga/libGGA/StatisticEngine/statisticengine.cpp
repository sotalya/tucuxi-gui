/**
 * @file statisticengine.cpp
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
#include "statisticengine.h"

/**
 * @brief StatisticEngine::StatisticEngine
 * Constructs a statistics engine with all informations set to 0.0
 *
 */
StatisticEngine::StatisticEngine() : minfitness(0.0), maxfitness(0.0), meanfitness(0.0), standardDeviation(0.0)
{
}

/**
 * @brief StatisticEngine::addFitness
 * Record a fitness in the statistic engine.
 *
 * @param fitness The fitness to be recorded
 */
void StatisticEngine::addFitness(qreal fitness){
    if(fitness > maxfitness)
        maxfitness = fitness;
    if(fitness < minfitness || minfitness == 0.0)
        minfitness = fitness;

    fitnesslist.append(fitness);
    meanfitness += fitness;
}

/**
 * @brief StatisticEngine::buildStats
 * Build stats with all the recorded fitnesses.
 *
 */
void StatisticEngine::buildStats(){

    meanfitness /= (qreal)fitnesslist.size();

    qreal temp;
    for(int i = 0; i < fitnesslist.size(); i++){
        temp = fitnesslist.at(i) -  meanfitness;
        standardDeviation += (temp*temp);
    }
    standardDeviation /= (qreal)fitnesslist.size();
    standardDeviation = sqrt(standardDeviation);

}

/**
 * @brief StatisticEngine::reset
 * Reset all the information in the statistic engine
 *
 */
void StatisticEngine::reset(){
    meanfitness = 0.0;
    standardDeviation = 0.0;
    minfitness = 0.0;
    maxfitness = 0.0;
    fitnesslist.clear();
}

/**
 * @brief StatisticEngine::getMinFitness
 * Get the minimum fitness recorded.
 *
 * @return The lower fitness recorded
 */
qreal StatisticEngine::getMinFitness(){
    return minfitness;
}

/**
 * @brief StatisticEngine::getMaxFitness
 * Get the maximum recorded fitness.
 *
 * @return The maximum fitness recorded
 */
qreal StatisticEngine::getMaxFitness(){
    return maxfitness;
}

/**
 * @brief StatisticEngine::getMeanFitness
 * Get the current mean of the recorded fitnesses.
 * A build of the statistics need to be run before disposing of this information.
 *
 * @return The mean fitness
 */
qreal StatisticEngine::getMeanFitness(){
    return meanfitness;
}

/**
 * @brief StatisticEngine::getStandardDeviation
 * Get the standard deviation of the recorded fitnesses.
 * A build of the statistics need to be run before disposing of this information.
 *
 * @return The standard deviation
 */
qreal StatisticEngine::getStandardDeviation(){
    return standardDeviation;
}
