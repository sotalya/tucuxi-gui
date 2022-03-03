// File title       :   realisticpatienttest.cpp
// Test title       :   RealisticPatientTest
// Author           :   Julien Rosset
// Creation date    :   16.02.2022
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
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/target.h"

#include <QDebug>                       // allows to print text in console : qInfo() << "text", or qDebug()"

extern SpixGTest* srv;



//_______________________________________________________________________________________________________________________________________________________

TEST(RealisticPatientTest, Test1)
{

    int waitTime1 = 1;                          // wait time in [s/2]

    //__Launch view : start a new patient________________________________________________________________________________________________________________

    srv->waitPeriod(waitTime1);                 // wait (4*1/2) [s]

    srv->startNewPatient();                     // from guitest.cpp
    srv->waitPeriod(waitTime1);

    // Runs ok

    //__Drug tab_________________________________________________________________________________________________________________________________________

    // concatenates path_ & drugName
    QString drugName = "Cefepime";

    QString drugPath = QString("mainWindow/flowView/drugList_%1").arg(drugName.data());
    qInfo() << drugPath;

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath(drugPath.toStdString()));    //QString to std string
    srv->waitPeriod(waitTime1);

    // Runs ok, but will be updated with use of functions to set chosen drug in list


    //__Dosage tab_______________________________________________________________________________________________________________________________________

    // Dose, Interval, Route, Infusion / - , Steady state ? y / n , Last dose / From & To.

    // clicks on Dosage tab

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod(waitTime1);

    // adding new Dose (will open Dosage dialog window)

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod(waitTime1);

    // Runs ok

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // fills in Dose value, in [µg]

    srv->synchronize();
    {       // isolates item, allowing to re-use it
        auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("doseSpinBox");
        item->setProperty("value", 400.0);
        srv->waitPeriod(waitTime1);
    }

    // fills in Interval value, in [h / 100]

    srv->synchronize();
    {
        auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("intervalSpinBox");
        item->setProperty("value", 1200);
        srv->waitPeriod(waitTime1);
    }

    // Runs ok

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // reads text of "Route :" text entry

    srv->synchronize();
    {
        auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("routeText");
        auto routeText = item->property("text").toString();
        qInfo() << "Route : " << routeText;

        if (routeText == "Intravenous drip")
        {
            // Infusion parameter only to be filled if Route = Intravenous drip != Oral
            srv->synchronize();
            {
                qInfo() << "Infusion value to be filled in";
                auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("infusionSpinBox");
                item->setProperty("value", 1000);                 // infusion value in [min / 100]
                srv->waitPeriod(waitTime1);
            }
        }
        else if (routeText == "Oral")
        {
            // do nothing ..?
            qInfo() << "No infusion to be done";
        }
    }

    // Runs ok

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->synchronize();
    {
        auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("atSteadyState");
        item->setProperty("checked", false);                    // sets steady state value
        srv->waitPeriod(waitTime1);
        auto steadyStateYorN = item->property("checked");       // gets value of steady state
        srv->waitPeriod(waitTime1);

        srv->synchronize();

        srv->mouseClick(spix::ItemPath("dosageDialog/LastDoseOrFromDateInput/wholeDate"));
        srv->waitPeriod(waitTime1);
        for(int i = 0; i < 10; i++) {
            srv->enterKey(spix::ItemPath("dosageDialog/LastDoseOrFromDateInput/wholeDate"), spix::KeyCodes::Backspace, 0);
            //srv->waitPeriod();
        }
        srv->inputText(spix::ItemPath("dosageDialog/LastDoseOrFromDateInput/wholeDate"), "01/01/2022");
        srv->waitPeriod(waitTime1);

        {
            auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("LastDoseOrFromTimeInput");
            item->setProperty("text", "23:59");
        }
        srv->waitPeriod(waitTime1);

        // Runs ok

        if (steadyStateYorN == false)
        {
            qInfo() << "At steady state? NO";

            srv->synchronize();

            // double click on second date entry (for loop necessary ?)
            srv->mouseClick(spix::ItemPath("dosageDialog/stoppedDateInput/wholeDate"));
            srv->mouseClick(spix::ItemPath("dosageDialog/stoppedDateInput/wholeDate"));
            srv->waitPeriod(waitTime1);

            for(int i = 0; i < 10; i++) {
                srv->enterKey(spix::ItemPath("dosageDialog/stoppedDateInput/wholeDate"), spix::KeyCodes::Backspace, 0);
                //srv->waitPeriod(waitTime1);
            }//appliedDateInput
            srv->inputText(spix::ItemPath("dosageDialog/stoppedDateInput/wholeDate"), "01/03/2022");
            srv->waitPeriod(waitTime1);

            // Runs ok

            srv->synchronize();

            {
                auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("stoppedTimeInput");
                item->setProperty("text", "12:35");
            }
            srv->waitPeriod(waitTime1);

        }
        else qInfo() << "At steady state? YES";
    }

    // Runs ok

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // closes Dosage dialog window when editing is done

    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));
    srv->waitPeriod(waitTime1);

    // Runs ok

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->waitPeriod(waitTime1*10);

    // editing existing Dose (will open Dosage dialog window)

    srv->synchronize();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod(waitTime1*6);

    srv->synchronize();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/editDosage"));
    srv->waitPeriod(waitTime1);


    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-


//    QDateTime dateTime;
//    dateTime.setDate(QDate(2022,02,17));
//    dateTime.setTime(QTime(9, 50, 0));
//    srv->addMeasure(100.0, dateTime);


    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    qInfo() << "End of test...";

    srv->waitPeriod(40);


}
