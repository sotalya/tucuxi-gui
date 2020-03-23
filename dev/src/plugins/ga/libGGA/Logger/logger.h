#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>

#include "evolutionengine.h"

class Logger
{
public:
    enum LoggerLevel {EVERYTHING, SATS_ONLY, STATS_AND_GENES, GENES_ONLY, NONE};
    Logger(EvolutionEngine* evolutionEngine, LoggerLevel loggerLevel = NONE);

    virtual void log() = 0;
    virtual void saveLogs() = 0;
    virtual void clearLogs() = 0;
protected:
    EvolutionEngine* evolutionEngine;
    LoggerLevel loggerLevel;

private:

};

#endif // LOGGER_H
