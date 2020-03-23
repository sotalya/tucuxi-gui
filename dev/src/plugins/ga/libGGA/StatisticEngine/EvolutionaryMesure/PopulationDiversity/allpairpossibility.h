#ifndef ALLPAIRPOSSIBILITY_H
#define ALLPAIRPOSSIBILITY_H

#include "populationdiversity.h"

class AllPairPossibility : public PopulationDiversity
{
public:
    AllPairPossibility(QList<Individual> *individualList);
};

#endif // ALLPAIRPOSSIBILITY_H
