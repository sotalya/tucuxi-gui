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

#include "demorequestfileclient.h"
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
using namespace Tucuxi::Gui::GuiUtils;

DemoRequestFileClient::DemoRequestFileClient(QObject *parent) : RequestsClientProcessing(parent)
{
}

DemoRequestFileClient::~DemoRequestFileClient()
{

}

void DemoRequestFileClient::setListFile(const QString &fileName)
{
    m_listFileName = fileName;
}

void DemoRequestFileClient::queryList(QDateTime from, QDateTime to, bool state)
{

    QFile source(m_listFileName);
    if (!source.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    source.seek(0);

    QString response = QString(source.readAll());

    QString controlId;
    analyzeList(response, controlId);
}

#include <QDir>
#include <iostream>

void DemoRequestFileClient::queryRequest(const QString &requestId, const QString &patientId, const QString &drugId)
{

    // We look for the request file in the same directory as the list of requests.
    // We look for the file with the same request ID as the one asked by the client
    // and send the raw file data.

    QFileInfo fi(m_listFileName);
    QDir dir = fi.dir();

    QStringList filters;
    filters << "Req_*.xml" << "*.prf";
    dir.setNameFilters(filters);

    QStringList reqFiles = dir.entryList();

    foreach (QString fileName, reqFiles) {
        QFile reqFile(dir.absoluteFilePath(fileName));
        QDomDocument doc;
        if (!doc.setContent(&reqFile)) {
            return;}

            QString requestId1 = doc.documentElement().firstChildElement("dataset").firstChildElement("requestId").firstChild().toText().data();
            if (requestId1.compare(requestId) == 0) {
                std::cout << "Matched Request id : " << qPrintable(requestId) << std::endl;
                // Got it, thats the right file
                reqFile.seek(0);
                QString response = reqFile.readAll();

                analyzeRequest(response);

                return;
            }
    }
}
