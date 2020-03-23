#ifndef STATISTICLOGGER_H
#define STATISTICLOGGER_H

#include "logger.h"

class StatisticLogger : public Logger
{
public:
    StatisticLogger(EvolutionEngine *evolutionEngine, QFile *file, LoggerLevel loggerLevel = NONE);
    void log();
    void saveLogs();

private:
    QFile *file;
};

#endif // STATISTICLOGGER_H
