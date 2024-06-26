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

#include "flatrequestfileclient.h"
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

using namespace Tucuxi::Gui::Core;
using namespace Tucuxi::Gui::FlatRequest;

FlatRequestFileClient::FlatRequestFileClient(QObject *parent) : FlatRequestsClientProcessing(parent)
{
}

FlatRequestFileClient::~FlatRequestFileClient()
{

}

void FlatRequestFileClient::setListFile(const QString &fileName)
{
    m_listFileName = fileName;
}

void FlatRequestFileClient::queryList(QDateTime from, QDateTime to, bool state)
{

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

    if (!doc.setContent(&reqFile))
        return;

    QDomElement detailCollectionElement = doc.documentElement().firstChildElement("Tablix1").firstChildElement("Détails_Collection");
    QDomElement detailElement = detailCollectionElement.firstChildElement("Détails");

    QString reportName = doc.documentElement().attribute("Name");

    // Construct a filtred xml doc containing only the seleted patient, by using patienId as criteria
    QDomElement filtredRootElement = filtredDoc.createElement("Report");
    filtredRootElement.setAttribute("Name", reportName);
    filtredDoc.appendChild(filtredRootElement);
    QDomElement filtredTabElement = filtredDoc.createElement("Tablix1");
    filtredRootElement.appendChild(filtredTabElement);
    QDomElement filtredDetailCollectionElement = filtredDoc.createElement("Détails_Collection");
    filtredTabElement.appendChild(filtredDetailCollectionElement);

    // Take only details element with the selected encounterid (=patientId)
    while (!detailElement.isNull()) {
        if (detailElement.attribute("encounterid") == patientId) {
            filtredDetailCollectionElement.appendChild(detailElement.cloneNode());
        }

        detailElement = detailElement.nextSiblingElement("Détails");
    }

    QTextStream informer(stdout);

    // TODO JRP : For debug, to be removed
    informer << filtredDoc.toString();
    informer << Qt::endl;
    informer.flush();

    analyzeRequest(filtredDoc.toString());
}
