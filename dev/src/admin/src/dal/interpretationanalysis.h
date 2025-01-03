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


#ifndef INTERPRETATIONANALYSIS_H
#define INTERPRETATIONANALYSIS_H

#include "core/dal/entity.h"
#include "core/dal/chartdata.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

class InterpretationAnalysis : public Tucuxi::Gui::Core::Entity
{

    Q_OBJECT
    ADMIN_ENTITY_UTILS(InterpretationAnalysis)

    //! the interpretation of the normality of the result by the analyst
    AUTO_PROPERTY_DECL(QString, expectedness, Expectedness)

    //! the interpretation of the appropriateness of drug exposure by the analyst
    AUTO_PROPERTY_DECL(QString, suitability, Suitability)

    AUTO_PROPERTY_DECL(Tucuxi::Gui::Core::ChartData*, chartData, ChartData)

    //! the recommendation of dosage adjustment by the analyst
    AUTO_PROPERTY_DECL(QString, prediction, Prediction)

    //! the recommendation for future monitoring by the analyst
    AUTO_PROPERTY_DECL(QString, remonitoring, Remonitoring)

    //! Some cautionary statement by the analyst
    AUTO_PROPERTY_DECL(QString, warning, Warning)

    AUTO_PROPERTY_DECL(QDateTime, nextControl, NextControl)

    AUTO_PROPERTY_DECL(QString, dosageDescription, DosageDescription)
protected:

    Q_INVOKABLE InterpretationAnalysis(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);

public:

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::InterpretationAnalysis*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::InterpretationAnalysis*>)

#endif // INTERPRETATIONANALYSIS_H

