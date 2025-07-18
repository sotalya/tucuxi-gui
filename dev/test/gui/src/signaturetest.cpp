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


// File title       :   signaturetest.cpp
// Test title       :   SignatureTest
// Author           :   Julien Rosset
// Creation date    :   26.07.2022
// Purpose          :
// ...              :   ...

#include <QQuickWindow>

#include <QTest>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/Identifiers.h>

#include "guitest.h"

#include <QDebug>
#include <QMessageBox>


using namespace Tucuxi::Gui::Admin;

extern SpixGTest* srv;

TEST(SignatureTest, Test1)
{
/*
    //    srv->startNewPatient();
    //    std::string fileName = "/home/julien/Documents/tucuxi-drugs/drugfiles/ch.tucuxi.imatinib.gotta2012.tdd";
    //    std::string fileName = "/home/julien/Documents/tucuxi-drugs/drugfiles/ch.tucuxi.vancomycin.colin2019.tdd";
    std::string fileName = "/home/julien/Documents/tucuxi-drugs/drugfiles/ch.tucuxi.caffeine.seng2009.tdd";

    Tucuxi::Gui::Processing::Drugs2Manager drugManager;

    Tucuxi::Sign::Signer signer = drugManager.checkSign(fileName);

    //    std::cout << signer << std::endl;             // Runs OK

    //        std::string name = signer.getName();

    std::cout << "Signer name : " << signer.getName() << std::endl;
    std::cout << "Organizzzation : " << signer.getOrganizationName() << std::endl;
    std::cout << "Locality : " << signer.getLocality() << std::endl;


    srv->waitPeriod(20);

    std::cout << "End of test" << std::endl;
*/
}
