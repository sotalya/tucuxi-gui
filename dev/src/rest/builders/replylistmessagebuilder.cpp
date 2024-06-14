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


#include "rest/builders/replylistmessagebuilder.h"
#include "rest/model/replylistxmlmessage.h"
#include "core/utils/logging.h"
#include "errors_rest.h"

#include <QDomDocument>

namespace Tucuxi {
namespace Gui {
namespace Rest {

ReplyListMessageBuilder::ReplyListMessageBuilder(QObject *parent) :
    XmlMessageBuilder(parent)
{

}

ReplyListMessageBuilder::~ReplyListMessageBuilder()
{

}

Message *ReplyListMessageBuilder::message() const
{
    Q_UNIMPLEMENTED();
    Message * ret = nullptr;
    return ret;
}

MessageContent ReplyListMessageBuilder::messageContent() const
{
    Q_ASSERT(_message);

    //ToDo: Add method in subclass to parse and control root tag attributes
    //ToDo: Add the comments
    //ToDo: Segment the code below

    MessageContent content;

    QDomDocument doc;
    QString qxmlError;
    int errline = -1;
    int columnline = -1;
    if (!doc.setContent(_message->data(),false, &qxmlError, &errline, &columnline)) {
        QString raw = QString::fromLocal8Bit(_message->data());
//        displayError(replyError.isEmpty() ? tr("Could not generate DOM for the 'REPLY_LIST' message") : replyError);
        EXLOG(QtFatalMsg, Tucuxi::rest::NOEZERROR, tr("QXmlError parsing DOM with error: %1, at line: %2, at column: %3, raw data: %4").arg(qxmlError).arg(errline).arg(columnline).arg(raw));
    }

    //Get the control ID
    content.setValue("controlId", doc.documentElement().attributeNode("controlId").value());

    //Get the requests data
    QDomElement requestNode = doc.documentElement().firstChildElement("request");
    while (!requestNode.isNull()) {
        MessageContent request;

        //Get request data
        request.setValue("id",    requestNode.firstChildElement("requestId").firstChild().toText().data());
        request.setValue("state", requestNode.firstChildElement("requestState").firstChild().toText().data());

        //Get patient data
        request.setValue("patient.id",             requestNode.firstChildElement("patient").firstChildElement("patientId").firstChild().toText().data());
        request.setValue("patient.stayNumber",     requestNode.firstChildElement("patient").firstChildElement("stayNumber").firstChild().toText().data());
        request.setValue("patient.name.first",     requestNode.firstChildElement("patient").firstChildElement("name").firstChildElement("firstName").firstChild().toText().data());
        request.setValue("patient.name.middle",    requestNode.firstChildElement("patient").firstChildElement("name").firstChildElement("middleName").firstChild().toText().data());
        request.setValue("patient.name.last",      requestNode.firstChildElement("patient").firstChildElement("name").firstChildElement("lastName").firstChild().toText().data());
        request.setValue("patient.birthDate",      requestNode.firstChildElement("patient").firstChildElement("birthdate").firstChild().toText().data());
        request.setValue("patient.gender",         requestNode.firstChildElement("patient").firstChildElement("gender").firstChild().toText().data());

        //Get patient institute
        request.setValue("patient.institute.id",   requestNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("instituteId").firstChild().toText().data());
        request.setValue("patient.institute.name", requestNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("name").firstChild().toText().data());

        //Get mandator data
        request.setValue("mandator.id",          requestNode.firstChildElement("mandator").firstChildElement("practicianId").firstChild().toText().data());
        request.setValue("mandator.title",       requestNode.firstChildElement("mandator").firstChildElement("title").firstChild().toText().data());
        request.setValue("mandator.name.first",  requestNode.firstChildElement("mandator").firstChildElement("name").firstChildElement("firstName").firstChild().toText().data());
        request.setValue("mandator.name.middle", requestNode.firstChildElement("mandator").firstChildElement("name").firstChildElement("middleName").firstChild().toText().data());
        request.setValue("mandator.name.last",   requestNode.firstChildElement("mandator").firstChildElement("name").firstChildElement("lastName").firstChild().toText().data());
        request.setValue("mandator.birthDate",   requestNode.firstChildElement("mandator").firstChildElement("birthdate").firstChild().toText().data());
        request.setValue("mandator.gender",      requestNode.firstChildElement("mandator").firstChildElement("gender").firstChild().toText().data());

        //Get mandator contact data
        request.setValue("mandator.contact.address",  requestNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("address").firstChild().toText().data());
        request.setValue("mandator.contact.city",     requestNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("city").firstChild().toText().data());
        request.setValue("mandator.contact.postcode", requestNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("postcode").firstChild().toText().data());
        request.setValue("mandator.contact.state",    requestNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("state").firstChild().toText().data());
        request.setValue("mandator.contact.country",  requestNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("country").firstChild().toText().data());

        //Get mandator emails
        QDomElement mandatorEmailNode = requestNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("emails").firstChildElement("email");
        while (!mandatorEmailNode.isNull()) {
            MessageContent email;

            //Get email data
            email.setValue("email",  mandatorEmailNode.firstChild().toText().data());
            email.setValue("type",   mandatorEmailNode.attributeNode("type").value());

            request.addToList("mandator.contact.emails", email);
            mandatorEmailNode = mandatorEmailNode.nextSiblingElement("email");
        }

        //Get mandator phones
        QDomElement mandatorPhoneNode = requestNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("phones").firstChildElement("phone");
        while (!mandatorPhoneNode.isNull()) {
            MessageContent phone;

            //Get phone data
            phone.setValue("phone",  mandatorPhoneNode.firstChild().toText().data());
            phone.setValue("type",   mandatorPhoneNode.attributeNode("type").value());

            request.addToList("mandator.contact.phones", phone);
            mandatorPhoneNode = mandatorPhoneNode.nextSiblingElement("phone");
        }

        //Get mandator institute data
        request.setValue("mandator.institute.id",   requestNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("instituteId").firstChild().toText().data());
        request.setValue("mandator.institute.name", requestNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("name").firstChild().toText().data());

        //Get mandator institute contact
        request.setValue("mandator.institute.address",  requestNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("address").firstChild().toText().data());
        request.setValue("mandator.institute.city",     requestNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("city").firstChild().toText().data());
        request.setValue("mandator.institute.postcode", requestNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("postcode").firstChild().toText().data());
        request.setValue("mandator.institute.state",    requestNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("state").firstChild().toText().data());
        request.setValue("mandator.institute.country",  requestNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("country").firstChild().toText().data());

        //Get mandator institute emails
        QDomElement mandatorInstituteEmailNode = requestNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("emails").firstChildElement("email");
        while (!mandatorInstituteEmailNode.isNull()) {
            MessageContent email;

            //Get email data
            email.setValue("email",  mandatorInstituteEmailNode.firstChild().toText().data());
            email.setValue("type",   mandatorInstituteEmailNode.attributeNode("type").value());

            request.addToList("mandator.institute.contact.emails", email);
            mandatorInstituteEmailNode = mandatorInstituteEmailNode.nextSiblingElement("email");
        }

        //Get mandator institute phones
        QDomElement mandatorInstitutePhoneNode = requestNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("phones").firstChildElement("phone");
        while (!mandatorInstitutePhoneNode.isNull()) {
            MessageContent phone;

            //Get phone data
            phone.setValue("phone",  mandatorInstitutePhoneNode.firstChild().toText().data());
            phone.setValue("type",   mandatorInstitutePhoneNode.attributeNode("type").value());

            request.addToList("mandator.institute.contact.phones", phone);
            mandatorInstitutePhoneNode = mandatorInstitutePhoneNode.nextSiblingElement("phone");
        }

        //Get sample data
        request.setValue("sample.id",           requestNode.firstChildElement("sample").firstChildElement("id").firstChild().toText().data());
        request.setValue("sample.date.sample",  requestNode.firstChildElement("sample").firstChildElement("sampleDate").firstChild().toText().data());
        request.setValue("sample.date.arrival", requestNode.firstChildElement("sample").firstChildElement("arrivalDate").firstChild().toText().data());

        QDomElement concentrationNode = requestNode.firstChildElement("sample").firstChildElement("concentrations").firstChildElement("concentration");
        while (!concentrationNode.isNull()) {
            MessageContent concentration;

            //Get concentration data
            concentration.setValue("analyte", concentrationNode.firstChildElement("analyte").firstChild().toText().data());
            concentration.setValue("value",   concentrationNode.firstChildElement("value").firstChild().toText().data());
            concentration.setValue("unit",    concentrationNode.firstChildElement("unit").firstChild().toText().data());

            request.addToList("sample.concentrations", concentration);
            concentrationNode = concentrationNode.nextSiblingElement("concentration");
        }

        //Get drug data
        request.setValue("drug.id",              requestNode.firstChildElement("drug").firstChildElement("drugId").firstChild().toText().data());
        request.setValue("drug.atc",             requestNode.firstChildElement("drug").firstChildElement("atc").firstChild().toText().data());
        request.setValue("drug.brandName",       requestNode.firstChildElement("drug").firstChildElement("brandName").firstChild().toText().data());
        request.setValue("drug.activePrinciple", requestNode.firstChildElement("drug").firstChildElement("activePrinciple").firstChild().toText().data());

        content.addToList("request", request);
        requestNode = requestNode.nextSiblingElement("request");
    }

    return content;
}

}
}
}
