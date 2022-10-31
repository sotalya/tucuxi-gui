//@@license@@s

#include "iccarequestfileclient.h"
#include "rest/builders/interpretationrequestbuilder.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/core.h"
#include "core/corefactory.h"
#include "apputils/src/appcore.h"
#include "admin/src/dal/partialrequest.h"
#include "admin/src/adminfactory.h"
#include "rest/builders/drugidtranslator.h"
#include "apputils/src/apputilsrepository.h"
#include "admin/src/stdadminrepository.h"
#include "rest/model/replylistxmlmessage.h"
#include "rest/builders/replylistmessagebuilder.h"

using namespace Tucuxi::Gui::Core;
using namespace Tucuxi::Gui::ICCA;

ICCARequestFileClient::ICCARequestFileClient(QObject *parent) : ICCARequestsClientProcessing(parent)
{
}

ICCARequestFileClient::~ICCARequestFileClient()
{

}

void ICCARequestFileClient::setListFile(const QString &fileName)
{
    m_listFileName = fileName;
}

void ICCARequestFileClient::queryList(QDateTime from, QDateTime to, bool state)
{

//    QFile source(m_listFileName);
//    if (!source.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        return;
//    }

//    source.seek(0);

//    QString response = QString(source.readAll());
    QString response;
    QString controlId;
    analyzeList(response, controlId);
}

#include <QDir>
#include <iostream>

void ICCARequestFileClient::queryRequest(const QString &requestId, const QString &patientId, const QString &drugId)
{

    QFile reqFile("vanco_fulldata2.xml");
    QDomDocument doc;
    QDomDocument filtredDoc;

    //TODO (JRP) : Fixed value for testing
    QString id = "156406";

    if (!doc.setContent(&reqFile))
        return;

    QDomElement detailCollectionElement = doc.documentElement().firstChildElement("Tablix1").firstChildElement("Détails_Collection");
    QDomElement detailElement = detailCollectionElement.firstChildElement("Détails");

    QDomElement filtredRootElement = filtredDoc.createElement("Report");
    filtredDoc.appendChild(filtredRootElement);
    QDomElement filtredTabElement = filtredDoc.createElement("Tablix1");
    filtredRootElement.appendChild(filtredTabElement);
    QDomElement filtredDetailCollectionElement = filtredDoc.createElement("Détails_Collection");
    filtredTabElement.appendChild(filtredDetailCollectionElement);

    while (!detailElement.isNull()) {
        if (detailElement.attribute("encounterid") == id) {
            filtredDetailCollectionElement.appendChild(detailElement.cloneNode());
        }

        detailElement = detailElement.nextSiblingElement("Détails");
    }

    QTextStream informer(stdout);

    informer << filtredDoc.toString();
    informer << endl;
    informer.flush();

    analyzeRequest(filtredDoc.toString());
}
