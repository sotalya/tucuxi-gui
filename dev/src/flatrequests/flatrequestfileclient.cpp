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

#include <qmessagebox.h>
#include <QProcess>
#include <QDir>

#include "flatrequestfileclient.h"
#include "flatrequestparameters.h"
#include "LoginDialog.h"

using namespace Tucuxi::Gui::FlatRequest;

FlatRequestFileClient::FlatRequestFileClient(QObject *parent) : FlatRequestsClientProcessing(parent)
{
}

FlatRequestFileClient::~FlatRequestFileClient() = default;

void FlatRequestFileClient::constructFileFromDB()
{
    QProcess process;
    QString username;
    QString password;

    // Currently deactivated, can be use if we need a login
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
        constructFileFromDB();
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

void FlatRequestFileClient::queryRequest(const QString &requestId, const QString &patientId, const QString &drugId)
{

    QFile reqFile(m_listFileName);
    QDomDocument doc;
    QDomDocument filtredDoc;

    FlatRequestParameters* flatRequestParam = FlatRequestParameters::getInstance();

    if (!doc.setContent(&reqFile))
        return;

    QDomElement reportElement = doc.documentElement().firstChildElement(flatRequestParam->reportNameXml());
    QString reportName;

    //Find the correct report for the selected drugId
    while (!reportElement.isNull()) {
        reportName = reportElement.attribute(flatRequestParam->fullDataNameXml());
        if (reportName == drugId) {
            break;
        }

        reportElement = reportElement.nextSiblingElement(flatRequestParam->reportNameXml());
    }

    QDomElement detailCollectionElement = reportElement.firstChildElement(flatRequestParam->detailsListNameXml());

    QDomElement detailElement = detailCollectionElement.firstChildElement(flatRequestParam->detailsNameXml());

    // Construct a filtred xml doc containing only the seleted patient, by using patienId as criteria
    QDomElement filtredRootElement = filtredDoc.createElement(flatRequestParam->reportNameXml());
    filtredRootElement.setAttribute(flatRequestParam->fullDataNameXml(), reportName);
    filtredDoc.appendChild(filtredRootElement);

    QDomElement filtredTabElement = filtredRootElement;

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

    //Print the imported XML
    informer << filtredDoc.toString();
    informer << Qt::endl;
    informer.flush();

    analyzeRequest(filtredDoc.toString());
}
