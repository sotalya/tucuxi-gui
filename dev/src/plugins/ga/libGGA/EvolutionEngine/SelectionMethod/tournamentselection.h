#ifndef TOURNAMENTSELECTION_H
#define TOURNAMENTSELECTION_H

#include "entityselection.h"
#include "Utility/randomgenerator.h"

class TournamentSelection : public EntitySelection
{
public:
    TournamentSelection();
     vector<PopEntity *> selectEntities(quint32 quantity, vector<PopEntity *> entityList);
};

#endif // TOURNAMENTSELECTION_H
