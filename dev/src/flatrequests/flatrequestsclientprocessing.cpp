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

#include <set>

#include <qmessagebox.h>

#include "flatrequestsclientprocessing.h"
#include "flatinterpretationrequestbuilder.h"
#include "flatrequestparameters.h"
#include "flatrequestnametranslator.h"
#include "core/core.h"
#include "core/corefactory.h"
#include "admin/src/dal/partialrequest.h"
#include "admin/src/adminfactory.h"
#include "apputils/src/apputilsrepository.h"
#include "admin/src/stdadminrepository.h"


#include "core/dal/drugtreatment.h"

using namespace Tucuxi::Gui::Core;
using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::FlatRequest;


FlatRequestsClientProcessing::FlatRequestsClientProcessing(QObject *parent) :
    RequestsClient(parent)
{

}

int FlatRequestsClientProcessing::analyzeList(const QString &xmlList, QString &controlId)
{
    QDomDocument doc;
    QDomElement reportElement;
    QList<SharedPartialRequest> requests;
    FlatRequestParameters* flatRequestParam = FlatRequestParameters::getInstance();
    FlatrequestNameTranslator* nameTranslator = FlatrequestNameTranslator::getInstance();

    if (!doc.setContent(xmlList))
        return 0;

    reportElement = doc.documentElement().firstChildElement(flatRequestParam->reportNameXml());

    // XML file can have more than one report, one report is for one specific drug
    while(!reportElement.isNull()) {
        QDomElement detailCollectionElement = reportElement.firstChildElement(flatRequestParam->detailsListNameXml());
        QDomElement detailElement = detailCollectionElement.firstChildElement(flatRequestParam->detailsNameXml());

        QString substanceID = reportElement.attribute(flatRequestParam->fullDataNameXml());
        Tucuxi::Gui::Core::ActiveSubstance* substance = nullptr;

        std::set<QString> notFound;
        std::set<QString> patientsVisited;

        // Go through elements to identify each patient and search for a first dosage (for each patient)
        while (!detailElement.isNull()) {
            QString patientID = detailElement.attribute(flatRequestParam->encounteridNameXml());

            // Verify if patient already have been parsed. Also imply that patient details are grouped by encouderid in the XML.
            if (!patientsVisited.contains(patientID)) {
                patientsVisited.insert(patientID);

                auto request = AdminFactory::createEntity<PartialRequest>(ABSTRACTREPO);
                request->requestId(patientID);

                auto* patient = static_cast<Patient*>(request->patient());
                patient->externalId(patientID);
                patient->person()->name(patient->externalId());

                substance = nullptr;
                APPUTILSREPO->getActiveSubstanceOfId(substanceID, substance);

                //If no substance found, certainly mean that the drug modes is missing
                if(substance == nullptr) {
                    if (!notFound.count(substanceID)) {
                        QMessageBox::warning(nullptr, "Error while loading file", "The active substance (" + substanceID + ") cannot be found, drug model is certainly missing");
                        notFound.insert(substanceID);
                    }
                    continue;
                    //return 0;
                }

                request->drug(substance);

                // Init measure default, useful if no measure found
                QDomElement detailElementCurrentPatient = detailElement;
                QString sampleID = "nosample";
                double concentration = 0.0;
                QString unit = "µmol/l";
                QDateTime sampleDate = QDateTime::currentDateTime();
                QDateTime lastSampleDate = QDateTime::currentDateTime().addYears(-50);

                // Find first concentration element for measure (if any)
                while (!detailElementCurrentPatient.isNull()) { // && currentPatientID == patientID) {
                    if (detailElementCurrentPatient.attribute(flatRequestParam->encounteridNameXml()) == patientID) {
                        if (nameTranslator->nameToInternalId(detailElementCurrentPatient.attribute(flatRequestParam->dataNameXml())) == "DRUG_MEASURE") {
                            sampleDate = QDateTime::fromString(detailElementCurrentPatient.attribute(flatRequestParam->timeNameXml()), Qt::ISODate);
                            if (sampleDate > lastSampleDate) {
                                lastSampleDate = sampleDate;
                                QString valueString = detailElementCurrentPatient.attribute(flatRequestParam->valueNameXml());
                                valueString.replace(',', '.');
                                concentration = valueString.toDouble();
                                unit = detailElementCurrentPatient.attribute(flatRequestParam->unitNameXml(), "mg/l");
                                unit = unit.toLower();
                                sampleID = patientID;
                            }
                        }
                    }


                    detailElementCurrentPatient = detailElementCurrentPatient.nextSiblingElement(flatRequestParam->detailsNameXml());
                }

                auto measure = static_cast<Measure*>(request->sample());
                measure->sampleID(sampleID);
                measure->setMoment(sampleDate);
                measure->arrivalDate(sampleDate);
                measure->setConcentration(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO, measure));
                measure->getConcentration()->setValue(concentration);
                measure->getConcentration()->setUnit(Unit(unit));

                requests.append(request);
                ADMINREPO->setPartialRequest(request);
            }

            detailElement = detailElement.nextSiblingElement(flatRequestParam->detailsNameXml());
        }

        reportElement = reportElement.nextSiblingElement(flatRequestParam->reportNameXml());
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
