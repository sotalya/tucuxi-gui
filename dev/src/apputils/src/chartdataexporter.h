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


#ifndef CHARTDATAEXPORTER_H
#define CHARTDATAEXPORTER_H

#include <QString>

namespace Tucuxi {
namespace Gui {
namespace Core {

class ChartData;
class PredictionResult;
}
}
}


#define EXPORT_APRIORI      0x1
#define EXPORT_APOSTERIORI  0x2
#define EXPORT_POPULATION   0x4
#define EXPORT_ADJUSTMENTS  0x8
#define EXPORT_ALL          0xF

class ChartDataExporter
{
public:
    ChartDataExporter();

    QString exportData(const Tucuxi::Gui::Core::ChartData *chartData, int what = EXPORT_ALL);

    QString exportPrediction(Tucuxi::Gui::Core::PredictionResult *pred);
};

class ChartDataHtmlExporter
{
public:
    ChartDataHtmlExporter();

    QString exportData(const Tucuxi::Gui::Core::ChartData *chartData, int what);

    QString exportPrediction(Tucuxi::Gui::Core::PredictionResult *pred);
};

#endif // CHARTDATAEXPORTER_H
