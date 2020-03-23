#include "core/utils/logging.h"
#include <QFile>


// Instanciation of static data members
QFile ezechiel::logging::Logger::logFile;


void ezechiel::logging::Logger::initLogFile(QString appDirectory)
{
    logFile.setFileName(appDirectory + "/log.txt");
    logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Unbuffered);
}

