#ifndef EVOLUTIONARYMESURE_H
#define EVOLUTIONARYMESURE_H

#include <QList>

#include "individual.h"

class EvolutionaryMesure
{
public:
    EvolutionaryMesure(QList<Individual> *individualList);

protected:
    QList<Individual> *individualList;

private:
};

#endif // EVOLUTIONARYMESURE_H
