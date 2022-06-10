//@@license@@

#include "processingproxy.h"
#include "networkutils.h"
#include <QCoreApplication>
#include "processingrequest.h"
#include "processingresponse.h"

#include <iostream>

using namespace Tucuxi::GuiCore;

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
