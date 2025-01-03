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


#include <QQuickWindow>

#include <QTest>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/Identifiers.h>

#include "guitest.h"

#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/target.h"


extern SpixGTest* srv;

TEST(CacheTest, Test1)
{
    for (int testIndex = 0; testIndex < 10; testIndex ++) {

        srv->startNewPatient();

        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
        srv->waitPeriod();
        srv->selectDrugInList("Cefepime", 0);
        srv->waitPeriod();

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

        srv->waitPeriod(20);

        srv->enterKey(spix::ItemPath("mainWindow/flowView/chartArea"), spix::KeyCodes::Right, 0);
        srv->waitPeriod(5);
        srv->enterKey(spix::ItemPath("mainWindow/flowView/chartArea"), spix::KeyCodes::Left, 0);
        srv->waitPeriod(5);
        srv->enterKey(spix::ItemPath("mainWindow/flowView/chartArea"), spix::KeyCodes::Left, 0);
        srv->waitPeriod(5);
        srv->enterKey(spix::ItemPath("mainWindow/flowView/chartArea"), spix::KeyCodes::Right, 0);
        srv->waitPeriod(5);
    }
    srv->synchronize();

    CHECKSPIXERRORS;
}


TEST(CacheTest, TestFarAway)
{

    srv->startNewPatient();

    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod();
    srv->selectDrugInList("Cefepime", 0);
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

    srv->waitPeriod(20);

    for(int iteration = 0; iteration < 10; iteration++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/chartArea"), spix::KeyCodes::Right, 0);
        srv->waitPeriod();
    }
    srv->synchronize();

    CHECKSPIXERRORS;
}
