/**
 * @file randomgenerator.cpp
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
 * @class RandomGenerator
 * @brief A random generator
 *
 * RandomGenerator is a random generator, thread safe.
 * It uses the Qt function qsrand and qrand to be multi-platform
 * compatible.
 * RangomGenerator works as a singleton and by so
 * no pointer of it should be kept on the user code.
 */

#include "randomgenerator.h"
#include <cstdlib>
QMutex RandomGenerator::mutex;
RandomGenerator* RandomGenerator::randomGenerator = NULL;
const qreal RandomGenerator::RANDMAX_PLUSONE = (qreal)RAND_MAX+1;

/**
 * @brief RandomGenerator::RandomGenerator
 * Constructs a random generator.
 *
 */
RandomGenerator::RandomGenerator() :
    QThread()
{
    resetSeed();
}

/**
 * @brief RandomGenerator::getGeneratorInstance
 * Get an instance of the random generator.
 *
 * @return The instance of the random generator
 */
RandomGenerator *RandomGenerator::getGeneratorInstance(){
    //Thread safety, automatically realeased at the end of the function.
    QMutexLocker locker(&RandomGenerator::mutex);

    if(randomGenerator == NULL)
        randomGenerator = new RandomGenerator();

    return randomGenerator;
}

/**
 * @brief RandomGenerator::random
 * Get a random number between min and max included.
 *
 * @param min The minimum value of the random
 * @param max The maximum value of the random
 * @return The random value
 */
qint32 RandomGenerator::random(qint32 min, qint32 max){
    if(min > max){
        return (qrand() / RANDMAX_PLUSONE) * (min-max+1);
    }else{
        return (qrand() / RANDMAX_PLUSONE) * (max+1-min);
    }
}

/**
 * @brief RandomGenerator::randomReal
 * Get a random number in qreal between min and max included.
 *
 * @param min The minimum value of the random
 * @param max The maximum value of the random
 * @return The random value
 */
qreal RandomGenerator::randomReal(qreal min, qreal max){
    if(min > max){
        return ((qreal)qrand() / (qreal)RANDMAX_PLUSONE) * (min-max);
    }else{
        return ((qreal)qrand() / (qreal)RANDMAX_PLUSONE) * (max-min);
    }
}

/**
 * @brief RandomGenerator::randomNoRandMax
 * Get a random value without using the RANDMAX algorithm.
 * @param min The minimum value of the random
 * @param max The maximum value of the random
 * @return The random value generated without RANDMAX
 */
qint32 RandomGenerator::randomNoRandMax(qint32 min, qint32 max){
    if(min < max){
        return qrand()%(max-min+1) + min;
    }else{
        return qrand()%(min-max+1) + max;
    }
}

/**
 * @brief RandomGenerator::resetSeed
 * Reset the seed of the random generator.
 *
 */
void RandomGenerator::resetSeed(){
    qsrand(QTime::currentTime().msec());
}
