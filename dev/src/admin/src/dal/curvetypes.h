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


#ifndef CURVETYPES_H
#define CURVETYPES_H

#include <QObject>

namespace Tucuxi {
namespace Gui {
namespace Admin {

class CurveType : public QObject
{
    Q_OBJECT
public:
    enum Enum
    {
        PopulationPrediction = 0,
        PopulationPercentiles,
        APrioriPrediction,
        AprioriPercentiles,
        APosterioriPrediction,
        APosterioriPercentiles,
        Measure,
        Target,
        PossibleAdjustments,
        SelectedAdjustments
    };
    Q_ENUM(CurveType::Enum)

    static const int first = Enum::PopulationPrediction;
    static const int last = Enum::SelectedAdjustments;
    static const int size = last + 1;
    inline static Enum convert(int value) { return static_cast<Enum>(value); }
    inline static bool isValid(int value) { return (value >= 0 && value<size); }
};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::CurveType::Enum)

#endif // CURVETYPES_H
