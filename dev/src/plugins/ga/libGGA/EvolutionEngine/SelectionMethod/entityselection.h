#ifndef ENTITYSELECTION_H
#define ENTITYSELECTION_H

#include <vector>
#include <Qt>
#include <QList>

#include "popentity.h"
#include <algorithm>
#include "randomgenerator.h"

class EntitySelection
{
public:
    EntitySelection();
    virtual vector<PopEntity *> selectEntities(quint32 quantity, vector<PopEntity *> entityList) = 0;
private:

};

#endif // ENTITYSELECTION_H
