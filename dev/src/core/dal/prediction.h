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


#ifndef PREDICTION_H
#define PREDICTION_H

#include <QObject>

#include "core/dal/entity.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"
#include "core/dal/drug/parameters.h"

Q_MOC_INCLUDE("core/dal/drugresponseanalysis.h")
Q_MOC_INCLUDE("core/dal/predictionspec.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class DrugResponseAnalysis;
class DrugTreatment;
class PredictionSpec;

enum CurveType {
    Continous,  //! A continuous curve is a curve that starts with an intial dosage (the first take) and an initial concentration of 0.
    Partial     //! A partial curve is a curve that starts directly at steady state given an initial dosage and observation date or measure.
};

class Prediction : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Prediction)

    AUTO_PROPERTY_DECL(QString, name, Name)
    AUTO_PROPERTY_DECL(QString, comment, Comment)
    AUTO_PROPERTY_DECL(CurveType, curveType, CurveType)
    AUTO_PROPERTY_DECL(ParameterType, paramsType, ParamsType)
    AUTO_PROPERTY_DECL(DrugResponseAnalysis*, analysis, Analysis)

    AUTO_PROPERTY_DECL(PredictionSpec*, spec, Spec)

//    Q_ENUMS(CurveType)


protected:


    Q_INVOKABLE Prediction(AbstractRepository *repository = nullptr, QObject* parent = 0);
    Q_INVOKABLE Prediction(AbstractRepository *repository, QObject* parent, DrugResponseAnalysis*);

public:

//    QDateTime firsttake() const;
    void setFirsttake(const QDateTime &firsttake);

    DrugTreatment *getTreatment() const;

signals:

private:

};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Prediction*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Prediction*>)

#endif // PREDICTION_H
