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
