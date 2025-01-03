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


#ifndef FLATREQUESTPARAMETERS_H
#define FLATREQUESTPARAMETERS_H

#include <QString>


class FlatRequestParameters
{
public:
    static FlatRequestParameters *getInstance();

    QString reportNameXml();
    QString reportsNameXml();
    QString fullDataNameXml();
    QString detailsListNameXml();
    QString siteNameXmls();
    QString drugNameXml();
    QString timeNameXml();
    QString unitNameXml();
    QString valueNameXml();
    QString dataNameXml();
    QString encounteridNameXml();
    QString detailsNameXml();


private:
    FlatRequestParameters();
    static FlatRequestParameters* instance;
};

#endif // FLATREQUESTPARAMETERS_H
