//@@license@@


#include "tqflogger.h"

#include <QDateTime>
#include <QFileInfo>
#include <QMutexLocker>


TqfLogger* TqfLogger::getInstance()
{
    static TqfLogger* tqfLogger = new TqfLogger();
    return tqfLogger;
}

void TqfLogger::setPath(const QString &path)
{
    this->path = path;
    currentPath = path + "/";
}

void TqfLogger::setEnable(bool enable)
{
    this->enabled = enable;
}



void TqfLogger::log(const Tucuxi::Core::ComputingRequest& _request)
{
    QMutexLocker locker(&mutex);
    if (!enabled)
        return;
    QDateTime currentTime = QDateTime::currentDateTime();
    QString baseFileName = QString("%1ComputingRequest_%2").arg(currentPath, currentTime.toString("yyyy_MM_dd_hh_mm_ss"));
    QString fileName = baseFileName + ".tqf";
    int i = 1;
    while (QFileInfo::exists(fileName)) {
        fileName = QString("%1_%2.tqf").arg(baseFileName).arg(i);
        i++;
    }
    Tucuxi::Core::ComputingRequestXmlExport exporter;
    exporter.exportToFile(_request, fileName.toStdString());
}
