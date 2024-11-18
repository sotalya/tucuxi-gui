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


#ifndef FLATINTERPRETATIONREQUESTBUILDER_H
#define FLATINTERPRETATIONREQUESTBUILDER_H


#include <QDomDocument>

#include "core/dal/dosage.h"
#include "admin/src/dal/interpretationrequest.h"
#include "flatrequestparameters.h"
#include "flatrequestnametranslator.h"


namespace Tucuxi {
namespace Gui {
namespace FlatRequest {

class FlatInterpretationRequestBuilder
{

public:
//    explicit InterpretationRequestBuilder(const MessageContent &content);
    explicit FlatInterpretationRequestBuilder(const QDomDocument &content);
    ~FlatInterpretationRequestBuilder();

    Tucuxi::Gui::Admin::InterpretationRequest* buildInterpretationRequest();

private:
//    MessageContent content;
    QDomDocument content;
    QDomElement reportNode;

    FlatRequestParameters* flatRequestParameters;
    FlatrequestNameTranslator* nameTranslator;

    static bool compareDosage(const Tucuxi::Gui::Core::Dosage* a, const Tucuxi::Gui::Core::Dosage* b);
    Tucuxi::Gui::Core::Duration findDuration(const QDomElement &currentElement);
    void createUncastedIntervalValue(Tucuxi::Gui::Core::Dosage* dosage, int interval_sec);
    void createUncastedDosageValue(Tucuxi::Gui::Core::Dosage *dosage, QString field, QString text, QString comment);
    void splitOverlappingDosage(Tucuxi::Gui::Core::DosageHistory* dosages);
    void setDosageEndDateInterval(Tucuxi::Gui::Core::DosageHistory* dosages);
    void groupDosage(Tucuxi::Gui::Core::DosageHistory* dosages);
};

}
}
}


#endif // FLATINTERPRETATIONREQUESTBUILDER_H
