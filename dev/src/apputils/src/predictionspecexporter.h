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


#ifndef PREDICTIONSPECEXPORTER_H
#define PREDICTIONSPECEXPORTER_H

#include <QXmlStreamWriter>

namespace Tucuxi {
namespace Gui {
namespace Core {


class PredictionSpec;
class DrugResponseAnalysis;
class DrugTreatment;
class DrugModel;
class DosageHistory;
class PatientVariateList;
class TargetList;
class CoreMeasureList;
class IdentifiableAmount;
class OperableAmount;
class UncastedValueList;
class ADME;
class ValidDoses;
class ValidInfusions;
class ValidIntervals;
class DrugVariateList;
class ParameterSet;
class OperationList;

}
}
}

class PredictionSpecExporter
{
public:
    PredictionSpecExporter();

    QString writeDate(QDateTime date);
    bool save(Tucuxi::Gui::Core::PredictionSpec *spec, QByteArray &data);

    void save(Tucuxi::Gui::Core::PredictionSpec *spec);
    void save(Tucuxi::Gui::Core::DrugResponseAnalysis *analysis);
    void save(Tucuxi::Gui::Core::DrugTreatment *treatment);
    void save(Tucuxi::Gui::Core::DrugModel *model);
    void save(Tucuxi::Gui::Core::DosageHistory *history);
    void save(Tucuxi::Gui::Core::PatientVariateList *list);
    void save(Tucuxi::Gui::Core::TargetList *list);
    void save(Tucuxi::Gui::Core::CoreMeasureList *list);
    void save(Tucuxi::Gui::Core::UncastedValueList *list);
    void save(Tucuxi::Gui::Core::ADME *adme);
    void save(Tucuxi::Gui::Core::ValidDoses *doses);
    void save(Tucuxi::Gui::Core::ValidInfusions *infusions);
    void save(Tucuxi::Gui::Core::ValidIntervals *intervals);
    void save(Tucuxi::Gui::Core::DrugVariateList *list);
    void save(Tucuxi::Gui::Core::ParameterSet *set);
    void save(Tucuxi::Gui::Core::OperationList *list);
    void saveIdentifiableAmount(const QString &tagName, Tucuxi::Gui::Core::IdentifiableAmount *amount);
    void saveOperableAmount(const QString &tagName, Tucuxi::Gui::Core::OperableAmount *amount);

protected:

    QXmlStreamWriter *writer;
};

#endif // PREDICTIONSPECEXPORTER_H
