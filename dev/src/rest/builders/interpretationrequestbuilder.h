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


#ifndef INTERPRETATIONREQUESTBUILDER_H
#define INTERPRETATIONREQUESTBUILDER_H

#include <QDomDocument>

#include "core/dal/drug/drug.h"
#include "core/dal/covariate.h"
#include "core/dal/dosage.h"
#include "admin/src/dal/institute.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/clinical.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/measure.h"
#include "admin/src/dal/interpretationrequest.h"


namespace Tucuxi {
namespace Gui {
namespace Rest {

class InterpretationRequestBuilder
{

public:
//    explicit InterpretationRequestBuilder(const MessageContent &content);
    explicit InterpretationRequestBuilder(const QDomDocument &content);
    ~InterpretationRequestBuilder();

    Tucuxi::Gui::Admin::InterpretationRequest* buildInterpretationRequest();

private:
    QString buildDrug(const QString &rootKey);
    Tucuxi::Gui::Core::DosageHistory* buildDosages(const QString &rootKey);

    Tucuxi::Gui::Core::PatientVariateList * buildCovariates(const QString &rootKey, const SharedPatient patient);
    Tucuxi::Gui::Admin::ClinicalSet* buildClinical(const QString &rootKey);
//    bool buildClinicals(const QString &rootKey, InterpretationRequest *InterpretationRequest);

    SharedPatient buildPatient(const QString &rootKey);
    SharedPractician buildPractician(const QString &rootKey);
    SharedInstitute buildInstitute(const QString &rootKey);

    Tucuxi::Gui::Core::CoreMeasureList* buildSamples(const QString &rootKey, const SharedPatient patient, const QString &activeSubstance);
    QList<SharedPhone> buildPhones(const QString &rootKey, const QString&);
    Tucuxi::Gui::Admin::PhoneList* buildPhoneList(const QString &rootKey, const QString&);
    QList<Tucuxi::Gui::Admin::Email*> buildEmails(const QString &rootKey, const QString&);

    Tucuxi::Gui::Admin::PhoneType toPhoneType(const QString &type);
    Tucuxi::Gui::Admin::EmailType toEmailType(const QString &type);

//    MessageContent content;
    QDomDocument content;
    QDomElement datasetNode;
};

}
}
}

#endif // INTERPRETATIONBUILDER_H
