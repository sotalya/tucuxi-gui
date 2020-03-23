/**
 * @file randomgenerator.h
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

#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H
#include <QThread>
#include <QMutexLocker>
#include <QMutex>
#include <QTime>
#include <QLinkedList>
#include <QThread>
#include <memory>

using namespace std;
class RandomGenerator : public QThread
{
    Q_OBJECT
public:
    /**
      * Get a random generator instance.
      *
      * This function is thread safe.
      *
      * @return the RandomGenerator singleton.
      */
    static RandomGenerator *getGeneratorInstance();

    /**
      * Get a random number.
      *
      * The return number is a number between min and max,
      *
      * @param startValue
      *     a starting value for the random.
      * @param endValue
      *     an ending value for the random.
      * @return random
      *     a random value.
      */
    qint32 random(qint32 min, qint32 max);

    /**
      * Get a random real number.
      *
      * The return number is a number between min and max,
      *
      * @param startValue
      *     a starting value for the random.
      * @param endValue
      *     an ending value for the random.
      * @return random
      *     a random value.
      */
    qreal randomReal(qreal min, qreal max);


    /**
      * Get a random number.
      *
      * The return number is a number between min and max, without using RAND_MAX.
      * Less precise but more compatible with low bit architectures(16bit/32bit).
      *
      * @param startValue
      *     a starting value for the random.
      * @param endValue
      *     an ending value for the random.
      * @return random
      *     a random value.
      */
    qint32 randomNoRandMax(qint32 min, qint32 max);

    /**
      * Reset the seed.
      *
      * Reinitialize the current seed.
      */
    void resetSeed();

protected:
    RandomGenerator();

private:
    //static auto_ptr<RandomGenerator> randomGenerator;
    static RandomGenerator* randomGenerator;

    static const qreal RANDMAX_PLUSONE;
    static QMutex mutex;
};

#endif // RANDOMGENERATOR_H
