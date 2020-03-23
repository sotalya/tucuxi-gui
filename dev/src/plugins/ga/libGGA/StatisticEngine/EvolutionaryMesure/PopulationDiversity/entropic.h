#ifndef ENTROPIC_H
#define ENTROPIC_H

#include "populationdiversity.h"

class Entropic : public PopulationDiversity
{
public:
    Entropic(QList<Individual> *individualList);
};

#endif // ENTROPIC_H
