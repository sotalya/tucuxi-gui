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


// File title       :   validationLoadFile.cpp
// Test title       :   validationLoadFile
// Author           :   Mattia Crasi
// Creation date    :   10.05.2022
// Purpose          :
// ...              :   ...

#include <QQuickWindow>

#include <QTest>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/Identifiers.h>

#include "core/dal/drug/drug.h"
#include "guitest.h"

#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/target.h"

#include <QDebug>
#include <QMessageBox>

#include "admin/src/interpretationxmlexport.h"
#include "admin/src/interpretationtorequestxmlexport.h"
#include "admin/src/interpretationxmlimport.h"

#include "core/corefactory.h"
#include "core/core.h"

using namespace Tucuxi::Gui::Admin;

extern SpixGTest* srv;

TEST(ValidationLoadFile, DISABLED_Test1)
{
    int waitTime1       = 4;
    int waitTimeLong    = 120;

    // LOADING interpretation from Xml file

    QString loadName = "scenario8"; // "save_1.tui";

    srv->loadInterpretation(loadName);

    InterpretationXmlExport interpretationXmlExport;
    Interpretation *interpretationLoad = srv->m_mainWindowController->getInterpretationController()->getInterpretation();

    QString status = interpretationXmlExport.toStringValidation(interpretationLoad->getValidationStatus()->getValidationStatus(StepType::Validation));

//    if (status == "validated")
//    {
//        srv->synchronize();
//        srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
//        srv->waitPeriod(waitTime1);
//        srv->synchronize();
//        srv->waitForSync();
//        srv->validateInterpretation();
//    }

//--------------------------------------------------
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->waitPeriod(waitTimeLong);
    std::cout << "End of program ..." << std::endl;
}
