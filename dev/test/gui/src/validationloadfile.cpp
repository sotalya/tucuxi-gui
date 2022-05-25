//@@lisence@@

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
#include <Spix/Events/KeyCodes.h>

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

//include test
#include "admin/src/dal/practician.h"
#include "admin/src/dal/interpretationrequest.h"

extern SpixGTest* srv;

TEST(ValidationLoadFile, Test1)
{
    int waitTime1       = 4;
    int waitTimeLong    = 30;

    // LOADING interpretation from Xml file

    QString loadName = "save_1c"; // "save_1.tui";

    srv->loadInterpretation(loadName);

    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/patientButton"));
    srv->waitPeriod(waitTime1);

//--------------------------------------------------
//    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("patientFirstName");
//    auto patientFirstName = item->property("text").toString();

//    QFile MyFile("save_1b.tui");
//    MyFile.open(QIODevice::ReadWrite);
//    QTextStream in (&MyFile);
//    const QString content = in.readAll();
//    if(content.contains("<firstname>"+patientFirstName+"</firstname>"))
//    {
//        qInfo() << "Chaine de caractere trouvé";
//    }
//    else
//    {
//        qInfo() << "Chaine de caractere PAS trouve";
//    }
//    MyFile.close();


//--------------------------------------------------
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->waitPeriod(waitTimeLong*100);
    std::cout << "End of program ..." << std::endl;
}
