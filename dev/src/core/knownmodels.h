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


#ifndef KNOWNMODELS_H
#define KNOWNMODELS_H


#define MODEL_LINEAR_1COMP_BOLUS_K_ID "ezechiel.model.linear.1comp.bolus.k"
#define MODEL_LINEAR_1COMP_INFUS_K_ID "ezechiel.model.linear.1comp.infus.k"
#define MODEL_LINEAR_1COMP_EXTRA_K_ID "ezechiel.model.linear.1comp.extra.k"
#define MODEL_LINEAR_2COMP_BOLUS_K_ID "ezechiel.model.linear.2comp.bolus.k"
#define MODEL_LINEAR_2COMP_INFUS_K_ID "ezechiel.model.linear.2comp.infus.k"
#define MODEL_LINEAR_2COMP_EXTRA_K_ID "ezechiel.model.linear.2comp.extra.k"
#define MODEL_LINEAR_3COMP_BOLUS_K_ID "ezechiel.model.linear.3comp.bolus.k"
#define MODEL_LINEAR_3COMP_INFUS_K_ID "ezechiel.model.linear.3comp.infus.k"
#define MODEL_LINEAR_3COMP_EXTRA_K_ID "ezechiel.model.linear.3comp.extra.k"

#define MODEL_LINEAR_1COMP_BOLUS_CLV_ID "ezechiel.model.linear.1comp.bolus.clv"
#define MODEL_LINEAR_1COMP_EXTRA_CLV_ID "ezechiel.model.linear.1comp.extra.clv"
#define MODEL_LINEAR_1COMP_INFUS_CLV_ID "ezechiel.model.linear.1comp.infus.clv"
#define MODEL_LINEAR_2COMP_BOLUS_CLV_ID "ezechiel.model.linear.2comp.bolus.clv"
#define MODEL_LINEAR_2COMP_INFUS_CLV_ID "ezechiel.model.linear.2comp.infus.clv"
#define MODEL_LINEAR_2COMP_EXTRA_CLV_ID "ezechiel.model.linear.2comp.extra.clv"
#define MODEL_LINEAR_3COMP_BOLUS_CLV_ID "ezechiel.model.linear.3comp.bolus.clv"
#define MODEL_LINEAR_3COMP_INFUS_CLV_ID "ezechiel.model.linear.3comp.infus.clv"
#define MODEL_LINEAR_3COMP_EXTRA_CLV_ID "ezechiel.model.linear.3comp.extra.clv"

#define MODEL_MM_1COMP_BOLUS_ID "ezechiel.model.mm.1comp.bolus"
#define MODEL_MM_1COMP_INFUS_ID "ezechiel.model.mm.1comp.infus"
#define MODEL_MM_1COMP_EXTRA_ID "ezechiel.model.mm.1comp.extra"
#define MODEL_MM_1COMP_EXTRAINF_ID "ezechiel.model.mm.1comp.extrainf"

#define MODEL_MM_2COMP_BOLUS_K_ID "ezechiel.model.mm.2comp.bolus.k"
#define MODEL_MM_2COMP_BOLUS_CLV_ID "ezechiel.model.mm.2comp.bolus.clv"
#define MODEL_MM_2COMP_INFUS_K_ID "ezechiel.model.mm.2comp.infus.k"
#define MODEL_MM_2COMP_INFUS_CLV_ID "ezechiel.model.mm.2comp.infus.clv"
#define MODEL_MM_2COMP_EXTRA_K_ID "ezechiel.model.mm.2comp.extra.k"
#define MODEL_MM_2COMP_EXTRA_CLV_ID "ezechiel.model.mm.2comp.extra.clv"

#define MODEL_MM_3COMP_BOLUS_K_ID "ezechiel.model.mm.3comp.bolus.k"
#define MODEL_MM_3COMP_BOLUS_CLV_ID "ezechiel.model.mm.3comp.bolus.clv"
#define MODEL_MM_3COMP_INFUS_K_ID "ezechiel.model.mm.3comp.infus.k"
#define MODEL_MM_3COMP_INFUS_CLV_ID "ezechiel.model.mm.3comp.infus.clv"
#define MODEL_MM_3COMP_EXTRA_K_ID "ezechiel.model.mm.3comp.extra.k"
#define MODEL_MM_3COMP_EXTRA_CLV_ID "ezechiel.model.mm.3comp.extra.clv"


#define MODEL_MM_1COMP_BOLUS_VMOVERV_ID "ezechiel.model.mm.1comp.bolus.vmoverv"
#define MODEL_MM_1COMP_INFUS_VMOVERV_ID "ezechiel.model.mm.1comp.infus.vmoverv"
#define MODEL_MM_1COMP_EXTRA_VMOVERV_ID "ezechiel.model.mm.1comp.extra.vmoverv"
#define MODEL_MM_2COMP_BOLUS_VMOVERV_ID "ezechiel.model.mm.2comp.bolus.vmoverv"
#define MODEL_MM_2COMP_INFUS_VMOVERV_ID "ezechiel.model.mm.2comp.infus.vmoverv"
#define MODEL_MM_2COMP_EXTRA_VMOVERV_ID "ezechiel.model.mm.2comp.extra.vmoverv"
#define MODEL_MM_3COMP_BOLUS_VMOVERV_ID "ezechiel.model.mm.3comp.bolus.vmoverv"
#define MODEL_MM_3COMP_INFUS_VMOVERV_ID "ezechiel.model.mm.3comp.infus.vmoverv"
#define MODEL_MM_3COMP_EXTRA_VMOVERV_ID "ezechiel.model.mm.3comp.extra.vmoverv"


#define MODEL_TEST_CONSTANT_ID "ezechiel.model.test.constant"
#define MODEL_TEST_PROPORTIONAL_ID "ezechiel.model.test.proportional"
#define MODEL_TEST_CONSTANT_SUM_ID "ezechiel.model.test.constantsum"

#define MODEL_NONLINEAR_1COMP_GAMMA_EXTRA_K_ID "ezechiel.model.nonlinear.1comp.gamma.extra.k"
#define MODEL_NONLINEAR_1COMP_GAMMA_EXTRA_CLV_ID "ezechiel.model.nonlinear.1comp.gamma.extra.clv"

#endif // KNOWNMODELS_H
