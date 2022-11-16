//@@license@@

#include "iccarequestsclientprocessing.h"
#include "iccainterpretationrequestbuilder.h"
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

#include "cli/rlutil.h"
#include "core/dal/drugtreatment.h"

using namespace Tucuxi::Gui::Core;
using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::Rest;
using namespace Tucuxi::Gui::ICCA;


ICCARequestsClientProcessing::ICCARequestsClientProcessing(QObject *parent) :
    RequestsClient(parent)
{

}

int ICCARequestsClientProcessing::analyzeList(const QString &xmlList, QString &controlId)
{
    QDomDocument doc;

    if (!doc.setContent(xmlList))
        return 0;

    QDomElement detailCollectionElement = doc.documentElement().firstChildElement("Tablix1").firstChildElement("Détails_Collection");
    QDomElement detailElement = detailCollectionElement.firstChildElement("Détails");

    QString substanceStr = doc.documentElement().attribute("Name");

    QString substanceID = "";

    // TODO (JRP) : use a dictionay (cf INI files)
    if (substanceStr == "vanco fulldata") {
        substanceID = "vancomycin";
    } else if (substanceStr == "cefepime fulldata") {
        substanceID = "cefepime";
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
            request->drug(substance);

            // Init measure default, useful if no measure found
            QDomElement detailElementCurrentPatient = detailElement;
            QString currentPatientID = patientID;
            QString sampleID = "nosample";
            double concentration = 0.0;
            QString unit = "µmol/l";
            bool sampleFound = false;
            QDateTime sampleDate = QDateTime::currentDateTime();

            // Find first concentration element for measure (if any)
            while (!detailElementCurrentPatient.isNull() && currentPatientID == patientID && !sampleFound) {
                if (detailElementCurrentPatient.attribute("donnees") == "concentration") {
                    concentration = detailElementCurrentPatient.attribute("valeur").toDouble();
                    unit = detailElementCurrentPatient.attribute("unite");
                    sampleDate = QDateTime::fromString(detailElementCurrentPatient.attribute("horaire"), Qt::ISODate);
                    sampleID = currentPatientID;
                    sampleFound = true;
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

    ICCAInterpretationRequestBuilder irBuilder(doc);

    InterpretationRequest* ir = irBuilder.buildInterpretationRequest();
    if (!ir) {
        EXLOG(QtWarningMsg, Tucuxi::Gui::Core::NOEZERROR, tr("Could not build interpretationRequest from Pending Request. Aborting."));
        return nullptr;
    }

    ir->setSource(xmlRequest);

    return ir;
}

int ICCARequestsClientProcessing::analyzeRequest(const QString &xmlRequest)
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
