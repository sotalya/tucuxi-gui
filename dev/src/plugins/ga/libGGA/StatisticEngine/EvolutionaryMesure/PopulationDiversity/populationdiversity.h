#ifndef POPULATIONDIVERSITY_H
#define POPULATIONDIVERSITY_H

#include "evolutionarymesure.h"

class PopulationDiversity : public EvolutionaryMesure
{
public:
    PopulationDiversity(QList<Individual> *individualList);
};

#endif // POPULATIONDIVERSITY_H
