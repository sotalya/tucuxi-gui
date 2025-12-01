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


#ifndef PERCENTILE_H
#define PERCENTILE_H

#include <QObject>
#include "core/dal/drug/parameters.h"

namespace Tucuxi {
namespace Gui {
namespace Core {

class Percentile
{

public:
    Percentile(int low, int high);

    [[nodiscard]] int low() const;
    [[nodiscard]] int high() const;

    void setLow(int value);
    void setHigh(int value);

private:
    //The percentile lower bound
    int _low;

    //The percentile higher bound
    int _high;
};

typedef Percentile* SharedPercentile;
/** A percentiles list of a given type.
 * \ingroup utils
 */
class PercentileList : public QList<Percentile>
{

public:
    PercentileList();
    PercentileList(const QList<Percentile> &other);
    PercentileList(const PercentileList &other);
    explicit PercentileList(ParameterType type);

    [[nodiscard]] ParameterType type() const;
    void setType(ParameterType type);

private:
    //The percentiles type
    ParameterType _type;
};


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // PERCENTILE_H
