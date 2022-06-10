//@@license@@

#include "restlogger.h"

#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include "core/core.h"
#include "core/utils/logging.h"
#include "errors_rest.h"

RestLogger::RestLogger()
{
    path = ".";
    currentPath = path;

}

RestLogger *RestLogger::getInstance()
{
    static RestLogger *logger = new RestLogger();
    return logger;
}

void RestLogger::setPath(const QString &path)
{
    this->path = path;
    currentPath = path + "/";
}

void RestLogger::logList(const QString& text)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    currentPath = path + QString("/req_%1/").arg(currentTime.toString("yyyy_MM_dd_hh_mm_ss"));
    QDir currentDir = QDir(currentPath);
    if (!currentDir.exists()) {
        if (!currentDir.mkpath(currentPath)) {

            EXLOG(QtWarningMsg, Tucuxi::rest::LOGERROR, QObject::tr("Can not create the LOG directory: %1.").arg(currentPath));
            return;
        }
    }
    QString baseFileName = QString("%1List_%2").arg(currentPath).arg(currentTime.toString("yyyy_MM_dd_hh_mm_ss"));
    QString fileName = baseFileName + ".xml";
    int i=0;
    while (QFileInfo::exists(fileName)) {
        fileName = QString("%1_%2.xml").arg(baseFileName).arg(i);
    }

    QFile f(fileName);
    if (!f.open(QFile::WriteOnly)) {
        EXLOG(QtWarningMsg, Tucuxi::rest::LOGERROR, QObject::tr("Can not log REST responses."));
        return;
    }

    f.write(text.toUtf8());
    f.close();
}

void RestLogger::logRequest(const QString& text)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString baseFileName = QString("%1Req_%2").arg(currentPath).arg(currentTime.toString("yyyy_MM_dd_hh_mm_ss"));
    QString fileName = baseFileName + ".xml";
    int i=0;
    while (QFileInfo::exists(fileName)) {
        fileName = QString("%1_%2.xml").arg(baseFileName).arg(i);
        i++;
    }

    QFile f(fileName);
    if (!f.open(QFile::WriteOnly)) {
        EXLOG(QtWarningMsg, Tucuxi::rest::LOGERROR, QObject::tr("Can not log REST responses."));
        return;
    }

    f.write(text.toUtf8());
    f.close();
}
