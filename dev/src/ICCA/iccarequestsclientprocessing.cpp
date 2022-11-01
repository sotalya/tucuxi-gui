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

    QList<SharedPartialRequest> requests;
    SharedPartialRequest request;
    Patient* patient;
    Tucuxi::Gui::Core::ActiveSubstance* substance = nullptr;
    QString patientID;
    QString lastPatientID = "None";

    while (!detailElement.isNull()) {
        patientID = detailElement.attribute("encounterid");

        if (patientID != lastPatientID) {
            lastPatientID = patientID;

            request = AdminFactory::createEntity<PartialRequest>(ABSTRACTREPO);
            request->requestId(patientID);

            patient = static_cast<Patient*>(request->patient());
            patient->externalId(patientID);
            patient->person()->name(patient->externalId());

            substance = nullptr;
            APPUTILSREPO->getActiveSubstanceOfId("vancomycin", substance);
            request->drug(substance);

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

    QTextStream informer(stdout);
    rlutil::setColor(rlutil::CYAN);
    informer <<  "Received Pending Request from Remote Server" << endl;
    informer <<  "*******************************************" << endl;
    rlutil::setColor(rlutil::GREEN);
    informer << "PatientId:\t"; informer.flush();
    rlutil::resetColor();
    informer << static_cast<Patient*>(ir->getTreatment()->getPatient())->externalId(); informer.flush();
    rlutil::setColor(rlutil::GREEN);
    informer << "\tDrugId:\t"; informer.flush();
    rlutil::resetColor();
    informer << ir->getTreatment()->getActiveSubstanceId();
    rlutil::resetColor();
    informer << endl;

    emit requestReady(ir);

    return 1;

}

//}
//}
//}
