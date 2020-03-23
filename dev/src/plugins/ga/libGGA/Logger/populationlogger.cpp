#include "populationlogger.h"

PopulationLogger::PopulationLogger(EvolutionEngine *evolutionEngine, QFile *file, LoggerLevel loggerLevel) : Logger(evolutionEngine, loggerLevel),
    file(file)
{
}
void PopulationLogger::log(){
    generationsLogs.push_back(evolutionEngine->getPopulation()->getAllEntitiesCopy());
}

void addInformationToCurrentGeneration(QString /*columnName*/, QString /*content*/){

}

void PopulationLogger::saveLogs(){
    if (file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(file);
        stream << "Genotype," << "Fitness," << "Elite" << endl;

        for (vector< vector<PopEntity *> >::size_type u = 0; u < generationsLogs.size(); u++) {
            for (vector<PopEntity *>::size_type v = 0; v < generationsLogs[u].size(); v++) {
                QString gene = QBitArrayUtility::bitArray2String(generationsLogs[u][v]->getGenotype()->getDataCopy());
                bool isElite = evolutionEngine->isElite(generationsLogs[u][v]->getGenotype());
                stream << gene << "," << generationsLogs[u][v]->getFitness() << "," << (isElite? "1" : "0") << endl;
            }
        }
        file->close();
    }
}

void PopulationLogger::clearLogs(){
    generationsLogs.clear();
}
