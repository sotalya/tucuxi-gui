#include "processingserver.h"
#include <QCoreApplication>

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);

    if (argc==2){
        if ((strcmp(argv[1],"-help")==0)||
            (strcmp(argv[1],"--help")==0)||
            (strcmp(argv[1],"-")==0)){
            std::cout << "EzeCHIeL processing server." << std::endl;
        }
    }
    ProcessingServer server;
    server.initialize();
    return app.exec();
}
