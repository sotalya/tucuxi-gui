/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "restlogger.h"

#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include "core/core.h"
#include "core/utils/logging.h"
#include "errors_rest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


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

}
}
}
