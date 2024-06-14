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


#include "drugtabcontroller.h"
#include "core/dal/drug/drug.h"
#include "guiutils/src/drugtohtml.h"

using namespace Tucuxi::Gui::GuiUtils;

STD_PROPERTY_IMPL(DrugTabController, QString, drugHtmlDescription, DrugHtmlDescription)

DrugTabController::DrugTabController(QObject *parent) : AbstractViewController(parent),
    _drugHtmlDescription(tr("No drug selected"))
{

}


void DrugTabController::setDrugInfo(Tucuxi::Gui::Core::ActiveSubstance *substance)
{
    DrugToHtml translator;
    setDrugHtmlDescription(translator.activeSubstanceToHtml(substance));

}

void DrugTabController::setDrugModelInfo(Tucuxi::Gui::Core::DrugModel *drugModel)
{
    DrugToHtml translator;
    setDrugHtmlDescription(translator.drugToHtml(drugModel));
}
