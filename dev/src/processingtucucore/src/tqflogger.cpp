/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
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
