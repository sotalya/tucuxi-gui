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


#ifndef FORMULATIONANDROUTETRANSLATOR_H
#define FORMULATIONANDROUTETRANSLATOR_H


#include <QString>
#include <QMap>

#include "tucucore/drugmodel/formulationandroute.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class FormulationAndRouteTranslator
{
public:
    FormulationAndRouteTranslator();
    virtual ~FormulationAndRouteTranslator();

    virtual Tucuxi::Core::FormulationAndRoute restToInternalFormulationAndRoute(QString drugId, QString intake) = 0;
};

class ChuvFormulationAndRouteTranslator : public FormulationAndRouteTranslator
{
public:
    ChuvFormulationAndRouteTranslator();

    virtual Tucuxi::Core::FormulationAndRoute restToInternalFormulationAndRoute(QString drugId, QString intake);

private:
    QMap<QString, QMap<QString, Tucuxi::Core::FormulationAndRoute>> map;
    QMap<QString, Tucuxi::Core::FormulationAndRoute> mapIntake;

};

class ExternalFormulationAndRouteTranslator : public FormulationAndRouteTranslator
{
public:
    ExternalFormulationAndRouteTranslator();

    void setFileName(const QString &fileName);

    virtual Tucuxi::Core::FormulationAndRoute restToInternalFormulationAndRoute(QString drugId, QString intake);

private:
    QString m_fileName;
};


}
}
}

#endif // FORMULATIONANDROUTETRANSLATOR_H
