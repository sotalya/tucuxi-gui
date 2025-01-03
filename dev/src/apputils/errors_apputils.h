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


#ifndef APPUTILSERRORS
#define APPUTILSERRORS

#include "core/utils/logging.h"

namespace Tucuxi {
namespace Gui {
namespace AppUtils {
    EZERROR(APPUTILS, NOEZERROR,"There is no error.")
    EZERROR(APPUTILS, NORIGHTS,"Lack of permission.")
    EZERROR(APPUTILS, ENCRYPTIONFAILED,"Could not encrypt.")
    EZERROR(APPUTILS, NODRUG,"The requested drug cannot be found.")
    EZERROR(APPUTILS, FILECANNOTOPEN,"The drug file could not be opened")
    EZERROR(APPUTILS, INVALIDDRUGOBJ,"The drug object is null")
    EZERROR(APPUTILS, DRUGIDDUPLICATE,"A drug corresponding to this Id is already registered")
    EZERROR(APPUTILS, INVALIDDRUGDESC,"The drug XML descriptor is null or invalid")
    EZERROR(APPUTILS, INVALIDDRUGDOC,"Failed to create the XML document")
    EZERROR(APPUTILS, STRUCTUREERRROOTTAG,"The root element of the drug could not be found")
    EZERROR(APPUTILS, STRUCTUREERRDRUGTAG,"The drug element of the drug could not be found")
    EZERROR(APPUTILS, STRUCTUREERRHDR,"Structure error detected in the drug's header")
    EZERROR(APPUTILS, STRUCTUREERRADME,"Structure error detected in the drug's adme entity")
    EZERROR(APPUTILS, STRUCTUREERRHALFLIFE,"Structure error detected in the drug's half-life entity")
    EZERROR(APPUTILS, STRUCTUREERRCONVERSIONS,"Structure error detected in the drug's conversions entity")
    EZERROR(APPUTILS, STRUCTUREERRDOSAGES,"Structure error detected in the drug's dosages")
    EZERROR(APPUTILS, STRUCTUREERRTARGETS,"Structure error detected in the drug's targets")
    EZERROR(APPUTILS, STRUCTUREERRCOVARIATES,"Structure error detected in the drug's covariates")
    EZERROR(APPUTILS, STRUCTUREERRPARAMETERS,"Structure error detected in the drug's parameters")
    EZERROR(APPUTILS, STRUCTUREERROPERATIONS,"Structure error detected in the drug's operations")
    EZERROR(APPUTILS, DATAERRMODEL,"The model doesn't match any known model")
    EZERROR(APPUTILS, DATAERRPARAM,"The parameter ID '%1' is required by the model but not defined in the drug")
    EZERROR(APPUTILS, SCRIPTERR,"A parameter script can not be evaluated")
}//namespace AppUtils
}//namespace Gui
}//namespace Tucuxi

#endif // APPUTILSERRORS

