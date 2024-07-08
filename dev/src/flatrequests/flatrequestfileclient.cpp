//@@license@@

#include "flatrequestfileclient.h"
#include "flatrequestparameters.h"
#include "LoginDialog.h"
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

#include <qmessagebox.h>
#include <QProcess>

using namespace Tucuxi::Gui::Core;
using namespace Tucuxi::Gui::FlatRequest;

FlatRequestFileClient::FlatRequestFileClient(QObject *parent) : FlatRequestsClientProcessing(parent)
{
}

FlatRequestFileClient::~FlatRequestFileClient()
{

}

void FlatRequestFileClient::constructFileFromDB()
{
    QProcess process;
    QString username;
    QString password;

    // TODO (JRP) : Currently deactivated, to be removed when we can assert that will be never needed
#if 0
    LoginDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        username = dialog.getUsername();
        password = dialog.getPassword();
    }
#endif

    QString pythonCommand = "dbConnect.exe -o import.xml -d cefepime -u " + username + " -p " + password;
    // QString pythonCommand = "python main.py -r -o import.xml -d cefepime -u " + username + " -p " + password;

    process.startCommand(pythonCommand);
    process.waitForFinished();

    QTextStream informer(stdout);

    informer << "Python script output: " << process.readAllStandardOutput();
    informer << Qt::endl;
    informer.flush();

    m_listFileName = "import.xml";
}

void FlatRequestFileClient::setListFile(const QString &fileName)
{
    m_listFileName = fileName;
}

void FlatRequestFileClient::queryList(QDateTime from, QDateTime to, bool state)
{
    FlatRequestParameters* flatRequestParam = FlatRequestParameters::getInstance();

    // If no file is already set then construc a file from a DB request
    if(m_listFileName.isEmpty()) {
        flatRequestParam->setFrenchTag(false);
        constructFileFromDB();
    } else {
        flatRequestParam->setFrenchTag(true);
    }

    QFile source(m_listFileName);
    if (!source.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error while loading file", "The xml file could not be loaded");
        return;
    }

    source.seek(0);

    QString response = QString(source.readAll());
    QString controlId;
    analyzeList(response, controlId);
}

#include <QDir>
#include <iostream>

void FlatRequestFileClient::queryRequest(const QString &requestId, const QString &patientId, const QString &drugId)
{

    QFile reqFile(m_listFileName);
    QDomDocument doc;
    QDomDocument filtredDoc;

    FlatRequestParameters* flatRequestParam = FlatRequestParameters::getInstance();

    if (!doc.setContent(&reqFile))
        return;

    QDomElement detailCollectionElement;

    if(flatRequestParam->getIsFrenchTag()) {
        detailCollectionElement = doc.documentElement().firstChildElement("Tablix1").firstChildElement(flatRequestParam->detailsListNameXml());
    } else {
        detailCollectionElement = doc.documentElement().firstChildElement(flatRequestParam->detailsListNameXml());
    }
    QDomElement detailElement = detailCollectionElement.firstChildElement(flatRequestParam->detailsNameXml());

    QString reportName = doc.documentElement().attribute(flatRequestParam->fullDataNameXml());

    // Construct a filtred xml doc containing only the seleted patient, by using patienId as criteria
    QDomElement filtredRootElement = filtredDoc.createElement(flatRequestParam->reportNameXml());
    filtredRootElement.setAttribute(flatRequestParam->fullDataNameXml(), reportName);
    filtredDoc.appendChild(filtredRootElement);

    QDomElement filtredTabElement;
    if(flatRequestParam->getIsFrenchTag()) {
        filtredTabElement = filtredDoc.createElement("Tablix1");
        filtredRootElement.appendChild(filtredTabElement);
    } else {
        filtredTabElement = filtredRootElement;
    }

    QDomElement filtredDetailCollectionElement = filtredDoc.createElement(flatRequestParam->detailsListNameXml());
    filtredTabElement.appendChild(filtredDetailCollectionElement);

    // Take only details element with the selected encounterid (=patientId)
    while (!detailElement.isNull()) {
        if (detailElement.attribute(flatRequestParam->encounteridNameXml()) == patientId) {
            filtredDetailCollectionElement.appendChild(detailElement.cloneNode());
        }

        detailElement = detailElement.nextSiblingElement(flatRequestParam->detailsNameXml());
    }

    QTextStream informer(stdout);

    // TODO JRP : For debug, to be removed
    informer << filtredDoc.toString();
    informer << Qt::endl;
    informer.flush();

    analyzeRequest(filtredDoc.toString());
}
