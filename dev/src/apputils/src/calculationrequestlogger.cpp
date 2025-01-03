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
