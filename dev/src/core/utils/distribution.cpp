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


#include "distribution.h"

//#include <QObject>

#include "core/utils/ezutils.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



//Array of possible distributions
QList<Descriptor> descs;

//Fill the descriptor list
// Each value needs to be appened in the same order as the DistributionType enumeration
void createDescs()
{
   descs.append(Descriptor("uniform",Descriptor::tr("Uniform"),Descriptor::tr("Uniform distribution")));
   descs.append(Descriptor("logNormal",Descriptor::tr("Log Normal"),Descriptor::tr("Log normal distribution")));
   descs.append(Descriptor("normal",Descriptor::tr("Normal"),Descriptor::tr("Normal distribution")));
}


// Create the entries if not already done
QList<Descriptor> distribution::descriptions()
{
   if (descs.isEmpty())
      createDescs();
   return descs;
}

// Create the entries if not already done, and search for the right one on them
distribution::DistributionType distribution::fromString(QString str)
{
   if (descs.isEmpty())
      createDescs();

   //Simple search
   for (int i=0; i<descs.size(); i++)
      if (descs.at(i).id == str)
         return DistributionType(i);
   return Invalid;
}

//Simple lookup
QString distribution::toString (DistributionType type)
{
   if (descs.isEmpty())
      createDescs();

   if (type >= descs.size())
      return "invalid";
   return descs.at(type).id;
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

