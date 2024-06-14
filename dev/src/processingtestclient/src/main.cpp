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


#include "processingproxy.h"
#include "networkutils.h"
#include <QCoreApplication>
#include "processingrequest.h"
#include "processingresponse.h"

#include <iostream>

using namespace Tucuxi::Gui::Core::Core;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);

    ProcessingProxy proxy;
    proxy.connectToHostBlocked(QHostAddress::LocalHost,10000,DEFAULTPORT);

    if (proxy.checkServerVersion())
        std::cout << "Server Version OK" << std::endl;
    else {
        std::cout << "Server version does not match" << std::endl;
        return -1;
    }
    SharedPrediction prediction = SharedPrediction(new Prediction(0));
    SharedPredictionSpec spec = SharedPredictionSpec(new PredictionSpec(0));
    prediction->setSpec(spec);
    spec->setNbPoints(200);
    SharedDrugTreatment treatment = SharedDrugTreatment(new DrugTreatment(0));

    prediction->setTreatment(treatment);

    AprioriTraits paramtrts;
    {
        DataSet data;
        spec->setNbPoints(5);
        proxy.points(prediction->analysis(), PointsTraits(spec->startDate(), spec->endDate(), spec->nbPoints(), &paramtrts, std::vector<double>(0)), data);
        std::cout << "Points received: " ;
        for(int i=0;i<data.size();i++)
            std::cout << data.at(i) << ", ";
        std::cout << std::endl;
    }
    {
        DataSet data;
        spec->setNbPoints(10);
        proxy.points(prediction->analysis(), PointsTraits(spec->startDate(), spec->endDate(), spec->nbPoints(), &paramtrts, std::vector<double>(0)), data);
        std::cout << "Points received: " ;
        for(int i=0;i<data.size();i++)
            std::cout << data.at(i) << ", ";
        std::cout << std::endl;
    }
    {
        DataSet data;
        spec->setNbPoints(15);
        proxy.points(prediction->analysis(), PointsTraits(spec->startDate(), spec->endDate(), spec->nbPoints(), &paramtrts, std::vector<double>(0)), data);
        std::cout << "Points received: " ;
        for(int i=0;i<data.size();i++)
            std::cout << data.at(i) << ", ";
        std::cout << std::endl;
    }
    return 0;
    //    return app.exec();
}
