#include "statisticlogger.h"

StatisticLogger::StatisticLogger(EvolutionEngine *evolutionEngine, QFile *file, LoggerLevel loggerLevel) : Logger(evolutionEngine, loggerLevel),
    file(file)
{
}
void StatisticLogger::log(){

}
void StatisticLogger::saveLogs(){

}
