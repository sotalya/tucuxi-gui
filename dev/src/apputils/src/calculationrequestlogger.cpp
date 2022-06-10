
#include <QFileInfo>
#include <QFile>
#include <QMutexLocker>

#include "calculationrequestlogger.h"

#include "core/dal/predictionspec.h"
#include "apputils/src/predictionspecexporter.h"
#include "apputils/src/predictionspecimporter.h"

CalculationRequestLogger::CalculationRequestLogger()
{
    enabled = false;
    path = "./log";
    currentPath = path + "/";

}

CalculationRequestLogger *CalculationRequestLogger::getInstance()
{
    static CalculationRequestLogger *logger = new CalculationRequestLogger();
    return logger;
}

void CalculationRequestLogger::setPath(const QString &path)
{
    this->path = path;
    currentPath = path + "/";
}

void CalculationRequestLogger::setEnable(bool enable)
{
    this->enabled = enable;
}


void CalculationRequestLogger::log(Tucuxi::Gui::Core::PredictionSpec *spec)
{
    QMutexLocker locker(&mutex);
    if (!enabled)
        return;
    QDateTime currentTime = QDateTime::currentDateTime();
    QString baseFileName = QString("%1CalcReq_%2").arg(currentPath).arg(currentTime.toString("yyyy_MM_dd_hh_mm_ss"));
    QString fileName = baseFileName + ".xml";
    int i=0;
    while (QFileInfo::exists(fileName)) {
        fileName = QString("%1_%2.xml").arg(baseFileName).arg(i);
        i++;
    }

    QFile f(fileName);
    if (!f.open(QFile::WriteOnly)) {
        EXLOG(QtWarningMsg, Tucuxi::Gui::Core::ACCESSDENIED, QObject::tr("Can not log processing requests."));
        return;
    }

    PredictionSpecExporter exporter;
    QByteArray data;
    exporter.save(spec, data);
    f.write(data);
    f.close();

    // The next lines are only for testing the importer. It should
    // generate a second file identical to the first one.
    if (false)
    {
        Tucuxi::Gui::Core::PredictionSpec *newSpec;
        PredictionSpecImporter importer;
        newSpec = importer.load(data);

        fileName += ".2.xml";
        QFile f(fileName);
        if (!f.open(QFile::WriteOnly)) {
            EXLOG(QtWarningMsg, Tucuxi::Gui::Core::ACCESSDENIED, QObject::tr("Can not log processing requests."));
            return;
        }

        PredictionSpecExporter exporter;
        QByteArray data;
        exporter.save(newSpec, data);
        f.write(data);
        f.close();

    }

}
