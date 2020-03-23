#ifndef SAVEENGINE_H
#define SAVEENGINE_H

#include "logger.h"

class SaveEngine
{
public:
    SaveEngine(Logger *logger);
private:
    Logger *logger;
};

#endif // SAVEENGINE_H
