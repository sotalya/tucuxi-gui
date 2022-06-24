//@@license@@

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //Setup the application
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Tucuxi-DBUpdater");
    QCoreApplication::setOrganizationName("REDS");
    QCoreApplication::setOrganizationDomain("http://www.ezechiel.ch/");

    //Setup the main window
    MainWindow w;
    w.show();
    
    return a.exec();
}
