//@@lisence@@

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
#include <Spix/Events/KeyCodes.h>

#include "guitest.h"

#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "admin/src/dal/interpretation.h"

#include <QDebug>
#include <QMessageBox>

#include "admin/src/interpretationxmlexport.h"
#include "admin/src/interpretationtorequestxmlexport.h"
#include "admin/src/interpretationxmlimport.h"

#include "core/corefactory.h"
#include "core/core.h"
#include "processingtucucore/src/drugs2manager.h"

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
