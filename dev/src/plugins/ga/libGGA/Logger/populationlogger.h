#ifndef POPULATIONLOGGER_H
#define POPULATIONLOGGER_H

#include <vector>
#include <QTextStream>

#include "logger.h"
#include "popentity.h"
#include "qbitarrayutility.h"
class PopulationLogger : public Logger
{
public:
    PopulationLogger(EvolutionEngine *evolutionEngine,  QFile *file, LoggerLevel loggerLevel = NONE);
    void log();
    void addInformationToCurrentGeneration(QString columnName, QString content);
    void saveLogs();
    void clearLogs();
private:
    QFile *file;
    vector<vector<PopEntity *> > generationsLogs;
};

#endif // POPULATIONLOGGER_H
