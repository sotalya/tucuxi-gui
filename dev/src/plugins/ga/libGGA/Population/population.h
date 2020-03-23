/**
 * @file population.h
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
 * @class Population
 * @brief A population is described by a name and a number of entities (individuals)
 * that are part of the population.
 * A population can receive and give entities to others but has no control on the
 * individuals themselves.
 *
 */
#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <QMutexLocker>
#include <QDebug>

#include "popentity.h"
#include "entityselection.h"
#include "randomgenerator.h"

using namespace std;
class Population
{
public:
    Population(QString name, quint32 size, quint32 individualsLength);
    Population(Population *population, QString name = QString());
    ~Population();

    quint32 getSize();
    void replace(vector<PopEntity *> newPopulation);
    void replace(vector<PopEntity *> base, vector<PopEntity *> generatedPartPopulation);
    void randomizePopulation();

    void setRepresentativesCopy(vector<PopEntity *> representatives, int quantity);
    vector<PopEntity *> getRepresentativesCopy();
    QString getName();

    PopEntity *getEntityCopyFrom(quint32 pos);
    PopEntity *getEntityAt(quint32 pos);

    vector<PopEntity *> getSomeEntityCopy(EntitySelection *entitySelection, quint32 count);
    vector<PopEntity *> getSomeEntity(EntitySelection *entitySelection, quint32 count);
    vector<PopEntity *> getAllEntitiesCopy();
    vector<PopEntity *> getAllEntities();
private:
    vector<PopEntity *> representatives;
    vector<PopEntity *> entityList;
    QMutex mutex;
    QString name;

};

#endif // POPULATION_H
