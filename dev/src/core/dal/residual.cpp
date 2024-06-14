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


#include "core/dal/residual.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


//Residual::Residual(QObject *parent) : QObject(parent)
//{

//}

Residual::Residual(double value) : value(value){
}

Residual::Residual() : value(0.0){
}

//----------------------- RESIDUALS --------------------------

Residuals::Residuals()
{

}

Residuals::Residuals(int size) : residuals(size)
{
}

const Residual &Residuals::at(int i)
{
    //Resize the vector if position do not exist
    if(i+1 > residuals.size())
        residuals.resize(i+1);

    return residuals.at(i);
}

Residual &Residuals::operator[](int i)
{
    //Resize the vector if position do not exist
    if(i+1 > residuals.size())
        residuals.resize(i+1);

    return residuals[i];
}

int Residuals::size() const
{
    return residuals.size();
}

void Residuals::clear()
{
    residuals.clear();
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
