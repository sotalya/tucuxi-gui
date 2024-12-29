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


#ifndef ADJUSTMENT_H
#define ADJUSTMENT_H

#include <QObject>
#include <QString>
#include "core/utils/autoproperty.h"
#include "predictive.h"
#include "core/dal/dosage.h"
#include "core/dal/entity.h"
#include "core/dal/drug/target.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


/**
 * @brief The TargetEvaluationResult class
 * This class is meant to embed information about the evaluation of a specific target.
 * It is composed of the type of target, the calculated value, and the score
 */
class TargetEvaluationResult : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(TargetEvaluationResult)

    //! Type of target, as there should be only a single one of each type it is sufficient to discriminate
    AUTO_PROPERTY_DECL(TargetMethod*, targetType, TargetType)

    //! Target value calculated by the TargetEvaluator
    AUTO_PROPERTY_DECL(double, value, Value)

    //! Target value calculated by the TargetEvaluator
    AUTO_PROPERTY_DECL(QString, unit, Unit)

    //! Score of the target, calculated by the TargetEvaluator
    AUTO_PROPERTY_DECL(double, score, Score)

public:

    Q_INVOKABLE QString toHtml() const;

public:
    explicit TargetEvaluationResult(AbstractRepository *repository, QObject *parent = nullptr);
};

QML_POINTERLIST_CLASS_DECL(TargetEvaluationResults, TargetEvaluationResult)

class Adjustment : public Predictive
{
    Q_OBJECT
    ENTITY_UTILS(Adjustment)
    AUTO_PROPERTY_DECL(DosageHistory*, dosageHistory, DosageHistory)

    AUTO_PROPERTY_DECL(double, score, Score)

    AUTO_PROPERTY_DECL(TargetEvaluationResults*, targetEvaluationResults, TargetEvaluationResults)

public:
    explicit Adjustment(AbstractRepository *repository, QObject *parent = nullptr);

    bool isValid() {
        if (getDosageHistory()->isValid() && getPredictionData()->isValid()) {
            return true;
        } else {
            return false;
        }
    }
};

QML_POINTERLIST_CLASS_DECL(Adjustments, Adjustment)
}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::TargetEvaluationResult*)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::TargetEvaluationResults*)

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Adjustment*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Adjustment*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Adjustments*)
#endif // ADJUSTMENT_H
