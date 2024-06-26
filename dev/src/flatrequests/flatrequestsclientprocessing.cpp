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


#include "flatrequestsclientprocessing.h"
#include "flatinterpretationrequestbuilder.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/core.h"
#include "core/corefactory.h"
#include "admin/src/dal/partialrequest.h"
#include "admin/src/adminfactory.h"
#include "rest/builders/drugidtranslator.h"
#include "apputils/src/apputilsrepository.h"
#include "admin/src/stdadminrepository.h"
#include "rest/model/replylistxmlmessage.h"
#include "rest/builders/replylistmessagebuilder.h"

#include <qmessagebox.h>

#include "cli/rlutil.h"
#include "core/dal/drugtreatment.h"

using namespace Tucuxi::Gui::Core;
using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::Rest;
using namespace Tucuxi::Gui::FlatRequest;


FlatRequestsClientProcessing::FlatRequestsClientProcessing(QObject *parent) :
    RequestsClient(parent)
{

}

int FlatRequestsClientProcessing::analyzeList(const QString &xmlList, QString &controlId)
{
    QDomDocument doc;

    if (!doc.setContent(xmlList))
        return 0;

    QDomElement detailCollectionElement = doc.documentElement().firstChildElement("Tablix1").firstChildElement("Détails_Collection");
    QDomElement detailElement = detailCollectionElement.firstChildElement("Détails");

    QString substanceStr = doc.documentElement().attribute("Name");
    QString substanceID = "";
    QString measureTagName = "";

    // TODO (JRP) : use a dictionay (cf INI files)
    if (substanceStr == "vanco fulldata") {
        substanceID = "vancomycin";
        measureTagName = "Dosage vanco";
    } else if (substanceStr == "cefepime fulldata") {
        substanceID = "cefepime";
        measureTagName = "Dosage cefepime"; //"Dosage Residuel cefepime";
    } else if (substanceStr == "voriconazole fulldata") {
        substanceID = "voriconazole";
        measureTagName = "Tx Vorico";
    }

    QList<SharedPartialRequest> requests;
    SharedPartialRequest request;
    Patient* patient;
    Tucuxi::Gui::Core::ActiveSubstance* substance = nullptr;
    QString patientID;
    QString lastPatientID = "None";

    while (!detailElement.isNull()) {
        patientID = detailElement.attribute("encounterid");

        // Verify if patient already have been parsed. Also imply that patient details are grouped by encouderid in the XML.
        if (patientID != lastPatientID) {
            lastPatientID = patientID;

            request = AdminFactory::createEntity<PartialRequest>(ABSTRACTREPO);
            request->requestId(patientID);

            patient = static_cast<Patient*>(request->patient());
            patient->externalId(patientID);
            patient->person()->name(patient->externalId());

            substance = nullptr;
            APPUTILSREPO->getActiveSubstanceOfId(substanceID, substance);

            //If no substance found, certainly mean that the drug modes is missing
            if(substance == nullptr) {
                QMessageBox::warning(nullptr, "Error while loading file", "The active substance (" + substanceID + ") cannot be found, drug model is certainly missing");
                return 0;
            }

            request->drug(substance);

            // Init measure default, useful if no measure found
            QDomElement detailElementCurrentPatient = detailElement;
            QString currentPatientID = patientID;
            QString sampleID = "nosample";
            double concentration = 0.0;
            QString unit = "µmol/l";
            // bool sampleFound = false;
            QDateTime sampleDate = QDateTime::currentDateTime();

            // Find first concentration element for measure (if any)
            while (!detailElementCurrentPatient.isNull() && currentPatientID == patientID/* && !sampleFound*/) {
                if (detailElementCurrentPatient.attribute("donnees").startsWith(measureTagName) ||
                    detailElementCurrentPatient.attribute("donnees").startsWith("Dosage Residuel cefepime")) {
                    QString valueString = detailElementCurrentPatient.attribute("valeur");
                    valueString.replace(',', '.');
                    concentration = valueString.toDouble();
                    unit = detailElementCurrentPatient.attribute("unite", "mg/l");
                    unit = unit.toLower();
                    sampleDate = QDateTime::fromString(detailElementCurrentPatient.attribute("horaire"), Qt::ISODate);
                    sampleID = currentPatientID;
                    // sampleFound = true;
                }

                detailElementCurrentPatient = detailElementCurrentPatient.nextSiblingElement("Détails");
                if (!detailElementCurrentPatient.isNull())
                    currentPatientID = detailElementCurrentPatient.attribute("encounterid");
            }

            Measure* measure = static_cast<Measure*>(request->sample());
            measure->sampleID(sampleID);
            measure->setMoment(sampleDate);
            measure->arrivalDate(sampleDate);
            measure->setConcentration(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO, measure));
            measure->getConcentration()->setValue(concentration);
            measure->getConcentration()->setUnit(Unit(unit));

            requests.append(request);
            ADMINREPO->setPartialRequest(request);
        }

        detailElement = detailElement.nextSiblingElement("Détails");
    }

    emit requestListReady(requests);
    return 1;
}

InterpretationRequest* SimpleBuilder::buildRequest(const QString &xmlRequest)
{
    QDomDocument doc;
    if (!doc.setContent(xmlRequest)) {

        return nullptr;
    }

    FlatInterpretationRequestBuilder irBuilder(doc);

    InterpretationRequest* ir = irBuilder.buildInterpretationRequest();
    if (!ir) {
        EXLOG(QtWarningMsg, Tucuxi::Gui::Core::NOEZERROR, tr("Could not build interpretationRequest from Pending Request. Aborting."));
        return nullptr;
    }

    ir->setSource(xmlRequest);

    return ir;
}

int FlatRequestsClientProcessing::analyzeRequest(const QString &xmlRequest)
{
    SimpleBuilder builder;
    InterpretationRequest* ir = builder.buildRequest(xmlRequest);
    if (ir == nullptr) {
        return 0;
    }

    emit requestReady(ir);

    return 1;
}

//}
//}
//}
