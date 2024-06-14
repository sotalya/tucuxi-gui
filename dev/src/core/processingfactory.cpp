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


#include "processingfactory.h"

#ifdef COMPILE_WITH_TUCUCORE
#include "processingtucucore/src/processingtucucore.h"
#ifdef COMPILE_WITH_COMPARATOR
#include "processingcomparator/src/processingcomparator.h"
#endif // COMPILE_WITH_COMPARATOR
#else
#include "processing/src/processingmath.h"
#endif // COMPILE_WITH_TUCUCORE

ProcessingFactory::ProcessingFactory()
{

}

Tucuxi::Gui::Core::ProcessingInterface *ProcessingFactory::createProcessing()
{
#ifdef COMPILE_WITH_TUCUCORE
#ifdef COMPILE_WITH_COMPARATOR
    return new ProcessingComparator();
#else
    return new Tucuxi::Gui::Processing::ProcessingTucucore();
#endif // COMPILE_WITH_COMPARATOR
#else
    return new ProcessingMath();
#endif // COMPILE_WITH_TUCUCORE
}


Tucuxi::Gui::Core::ProcessingInterface *ProcessingFactory::createProcessing(std::string what)
{
    if (what == "comparator") {
#ifdef COMPILE_WITH_TUCUCORE
#ifdef COMPILE_WITH_COMPARATOR
        return new ProcessingComparator();
#else
        return nullptr;
#endif // COMPILE_WITH_COMPARATOR
#else
        return nullptr;
#endif // COMPILE_WITH_TUCUCORE
    }
    else if (what == "tucucore") {
#ifdef COMPILE_WITH_TUCUCORE
        return new Tucuxi::Gui::Processing::ProcessingTucucore();
#else
        return nullptr;
#endif // COMPILE_WITH_TUCUCORE
    }
    else if (what == "ezechiel") {
        return nullptr;
//        return new ProcessingMath();
    }
}
