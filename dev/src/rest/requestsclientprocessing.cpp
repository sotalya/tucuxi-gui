//@@license@@

#include "requestsclientprocessing.h"
#include "rest/builders/interpretationrequestbuilder.h"
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


using namespace Tucuxi::GuiCore;

RequestsClientProcessing::RequestsClientProcessing(QObject *parent) :
    RequestsClient(parent)
{

}

int RequestsClientProcessing::analyzeList(const QString &xmlList, QString &controlId)
{
    QDomDocument reply_request;
    if (!reply_request.setContent(xmlList)) {
        //    internalError(response, "Unkown convertion error");
        return 0;
    }



    ReplyListXmlMessage replyListMessage;
    replyListMessage.setData(xmlList.toUtf8());

    if (!replyListMessage.isValid()) {
        EXLOG(QtWarningMsg, NOEZERROR, tr("The content of the requests list is invalid (%1)").arg(replyListMessage.errorString()));
        return 0;
    }

    ReplyListMessageBuilder* _replyListBuilder = new ReplyListMessageBuilder(this);
    _replyListBuilder->setMessage(&replyListMessage);
    MessageContent content = _replyListBuilder->messageContent();

    //ToDo: use interpretation request builder instead

    QList<MessageContent> requestsContent = content.list("request");
    QList<SharedPartialRequest> requests;


    for (int i = 0; i < requestsContent.size(); ++i) {
        MessageContent content = requestsContent.at(i);
        // Q_ASSERT(content.listCount("sample.concentrations") > 0);

        SharedPartialRequest request = AdminFactory::createEntity<PartialRequest>(ABSTRACTREPO);

        //ToDo: complete the share request data
        //ToDo: uncomment the drug value below and remove imatinib

        //The request data
        request->requestId(content.value("id"));
        request->setId(request->requestId().toInt());
        request->requestCpcl(content.value("state"));

        //The patient data
        Patient* patient = static_cast<Patient*>(request->patient());
        patient->externalId(content.value("patient.id"));
        patient->stayNumber(content.value("patient.stayNumber"));
        patient->person()->firstname(content.value("patient.name.first"));
        patient->person()->name(content.value("patient.name.last"));
        patient->person()->birthday(QDateTime::fromString(content.value("patient.birthDate"), Qt::ISODate).date());
        patient->person()->gender(content.value("patient.gender") == "male" ? Person::Male : Person::Female);

        //The patient institute
        request->institute()->externalId(content.value("patient.institute.id"));
        request->institute()->name(content.value("patient.institute.name"));

        //The sample data
        if (content.listCount("sample.concentrations") > 0) {
            Measure* measure = static_cast<Measure*>(request->sample());
            measure->sampleID(content.value("sample.id"));
            measure->setMoment(QDateTime::fromString(content.value("sample.date.sample"), Qt::ISODate));
            measure->arrivalDate(QDateTime::fromString(content.value("sample.date.arrival"), Qt::ISODate));
            measure->setConcentration(Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::IdentifiableAmount>(ABSTRACTREPO, measure));
            measure->getConcentration()->setValue(content.list("sample.concentrations").first().value("value").toDouble());
            measure->getConcentration()->setUnit(content.list("sample.concentrations").first().value("unit"));
        }
        else {
            Measure* measure = static_cast<Measure*>(request->sample());
            measure->sampleID("nosample");
            measure->setMoment(QDateTime::currentDateTime());
            measure->arrivalDate(QDateTime::currentDateTime());
            measure->setConcentration(Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::IdentifiableAmount>(ABSTRACTREPO, measure));
            measure->getConcentration()->setValue(0);
            measure->getConcentration()->setUnit(Unit("mg/l"));
        }

        //The drug data
        Tucuxi::GuiCore::ActiveSubstance* substance = nullptr;


        //        this->queryRequest(content.value("id"),content.value("patient.id"),content.value("drug.id"));

#ifdef CONFIG_DEMO
        DummyDrugIdTranslator *translator = new DummyDrugIdTranslator();
#else
        ExternalDrugIdTranslator *translator = new ExternalDrugIdTranslator();
        translator->setFileName(QCoreApplication::applicationDirPath() + "/drugidtranslations.ini");
#endif
        QString restDrugId = content.value("drug.id");
        QString drugId = translator->restToInternalId(restDrugId);
        delete translator;

        if (drugId.isEmpty())
        {
            EXLOG(QtWarningMsg, Tucuxi::GuiCore::NOEZERROR, tr("Cannot build interpretationRequest from Pending Request. The drug id %1 is unknown. Aborting this request.").arg(restDrugId));
            continue;
        }


        if (drugId.compare("ch.heig-vd.ezechiel.unknown") == 0) {
            request->uncastedSourceSubstance(restDrugId);
        }

        APPUTILSREPO->getActiveSubstanceOfId(drugId, substance);
        request->drug(substance);


        QTextStream informer(stdout);
        rlutil::setColor(rlutil::GREEN);
        informer << "RequestId:\t"; informer.flush();
        rlutil::resetColor();
        informer << request->requestId(); informer.flush();
        rlutil::setColor(rlutil::GREEN);
        informer << "\tPatientId:\t"; informer.flush();
        rlutil::resetColor();
        informer << request->patient()->externalId(); informer.flush();
        rlutil::setColor(rlutil::GREEN);
        informer << "\tDrugId:\t"; informer.flush();
        rlutil::resetColor();
        if (!request->drug()) {
            rlutil::setColor(rlutil::RED);
            informer << "DRUG NOT FOUND: " << content.value("drug.id") << endl;
            EXLOG(QtWarningMsg, Tucuxi::GuiCore::NOEZERROR, tr("Cannot build interpretationRequest from Pending Request. Aborting this request."));
            rlutil::resetColor();
            continue;
        } else {
            informer << request->drug()->getSubstanceId();
        }
        rlutil::resetColor();
        informer << endl;

        if (!request->drug()) {
            EXLOG(QtWarningMsg, Tucuxi::GuiCore::NOEZERROR, tr("Cannot build interpretationRequest from Pending Request. Aborting."));
            continue;
            return 0;
        }
        requests.append(request);
        ADMINREPO->setPartialRequest(request);
    }

    controlId = content.value("controlId");

    rlutil::resetColor();

    emit requestListReady(requests);
    return 1;
}

InterpretationRequest* SimpleBuilder::buildRequest(const QString &xmlRequest)
{
    QDomDocument doc;
    if (!doc.setContent(xmlRequest)) {

        return nullptr;
    }

    InterpretationRequestBuilder irBuilder(doc);

    InterpretationRequest* ir = irBuilder.buildInterpretationRequest();
    if (!ir) {
        EXLOG(QtWarningMsg, Tucuxi::GuiCore::NOEZERROR, tr("Could not build interpretationRequest from Pending Request. Aborting."));
        return nullptr;
    }

    ir->setSource(xmlRequest);

    return ir;
}

int RequestsClientProcessing::analyzeRequest(const QString &xmlRequest)
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
