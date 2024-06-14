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


#include "QApplication"
#include "tcpclient.h"
#include <QDateTime>
#include <QString>
#include <QtGlobal>
#include <iostream>

#ifdef Q_WS_WIN
    #include <windows.h>
    #define sleep(s) Sleep((s)*1000)
#else
    #include <unistd.h>
    #define sleep(s) sleep(s)
#endif

//Initial sleep time
#define INITIAL_SLEEP 10

//Sleep time between commands
#define INTERVAL_SLEEP 5

/* Starts a simple client test
  */
int main(int argc, char *argv[])
{
    //Application
    QApplication app(argc, argv);

    TcpClient client;
    if (!client.initialize())
    {
        std::cout << "Could not connect to the server. Aborting ..." << std::endl;
        return 0;
    }

    QDate d = QDate::currentDate();

    /*****************************************************************************************
     *                              TCP Demonstrator                                         *
     *                              ----------------                                         *
     *                                                                                       *
     * You can find below an example of data sent to Tucuxi. This data consists of measures*
     * and posology adaptations for a prediction made for John Doe using the Imatinib drug.  *
     * In order to have the best results, please follow the steps below in Tucuxi before   *
     * launching this TCP client.                                                            *
     *                                                                                       *
     * 1. Make sure the measures and covariates are emtpy for the patient John Doe.          *
     * 2. Make sure that there isn't any saved curve with the name 'Imatinib (John Doe)'     *
     * 3. Create a curve for John Doe with the following data:                               *
     *    a) Name:          Imatinib (John Doe)                                              *
     *    b) Patient:       John Doe                                                         *
     *    c) Drug:          Imatinib                                                         *
     *    d) Route:         Extravascular                                                    *
     *    e) Type:          A posteriori                                                     *
     *    f) Percentiles:   Optional, but better with 25-75 for A Posteriori percentile type *
     *    g) Date:          Today                                                            *
     *    h) Time:          08:00                                                            *
     *    i) Amount:        200 mg                                                           *
     *    j) Interval:      12h00                                                            *
     *    k) Model:         Analytic                                                         *
     * 4. Save the curve                                                                     *
     * 5. Display 12 cycles, or 20 if you do not want a center on new dosages                *
     * 6. Show the measures widget instead of the dosage widget                              *
     *                                                                                       *
     * You can now launch the TCP client to view the results. Don't forget to destroy the    *
     * curve that you saved, as well as the measures, before running the demonstration again.*
     *****************************************************************************************/

    //Initial sleep time
    sleep(INITIAL_SLEEP);

    //Add measure test
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)), 420);
    sleep(INTERVAL_SLEEP);
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)).addDays(1), 970);
    sleep(INTERVAL_SLEEP);
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)).addDays(2), 1130);
    sleep(INTERVAL_SLEEP);
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)).addDays(3), 1245);
    sleep(INTERVAL_SLEEP);
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)).addDays(4), 1270);
    sleep(INTERVAL_SLEEP);
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)).addDays(5), 1295);
    sleep(INTERVAL_SLEEP);

    //Replace measure test
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)), 410);
    sleep(INTERVAL_SLEEP);
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)).addDays(1), 980);
    sleep(INTERVAL_SLEEP);
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)).addDays(2), 1120);
    sleep(INTERVAL_SLEEP);
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)).addDays(3), 1255);
    sleep(INTERVAL_SLEEP);
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)).addDays(4), 1260);
    sleep(INTERVAL_SLEEP);
    client.sendMeasure("John", "Doe", "Imatinib", QDateTime(d, QTime(12, 0)).addDays(5), 1305);
    sleep(INTERVAL_SLEEP);

    //Add dose test
    client.sendDose("John", "Doe", "Imatinib", QDateTime(d, QTime(8, 0)).addDays(6), 100, 3600*6);
    sleep(INTERVAL_SLEEP);

    //Replace dose test
    client.sendDose("John", "Doe", "Imatinib", QDateTime(d, QTime(8, 0)).addDays(6), 150, 3600*10);
    sleep(INTERVAL_SLEEP);

    //client.close();

    return app.exec();
    return 0;
}

