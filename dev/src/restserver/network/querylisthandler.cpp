
#include <QFile>
#include <QString>
#include <QVariant>


#include "querylisthandler.h"
#include "restserver/utils/settings.h"

#include "core/utils/xmlvalidator.h"

#include "rest/model/querylistxmlmessage.h"
#include "rest/utils/controlid.h"

QueryListHandler::QueryListHandler(QObject *parent) :
    HttpRequestHandler(parent),
    ErrorHandler()
{

}

QueryListHandler::~QueryListHandler()
{

}

void QueryListHandler::service(HttpRequest &request, HttpResponse &response)
{
    QueryListXmlMessage message;
    message.setData(request.getBody());
    if (!message.isValid()) {
        badRequest(response, "Malformed request");
        return;
    }

    QFile source(Settings::getInstance().get("source").toString());
    if (!source.open(QIODevice::ReadOnly | QIODevice::Text)) {
        noContent(response);
        return;
    }


#ifdef ARTIFICIAL_TEST
    Tucuxi::Gui::Core::XmlValidator validator;
    if (!validator.validate(&source, Tucuxi::Gui::Core::XmlValidator::Reply_Request)) {
        internalError(response, QString("Malformed source (%1)").arg(validator.errorMessage()));
        return;
    }

    source.seek(0);
    QDomDocument reply_request;
    if (!reply_request.setContent(&source)) {
        internalError(response, "Unkown convertion error");
        return;
    }

    QDomDocument reply_list = buildReply(reply_request);
    if (!validator.validate(reply_list.toByteArray(), Tucuxi::Gui::Core::XmlValidator::Reply_List)) {
        internalError(response,QString("Malformed response (%1)").arg(validator.errorMessage()));
        return;
    }

    response.setHeader("Content-Type", "text/xml");
    response.write(reply_list.toByteArray(2), true);

#else
    response.setHeader("Content-Type", "text/xml");
    response.write(source.readAll(), true);

#endif // ARTIFICIAL_TEST

}

QString QueryListHandler::handlerName() const
{
    return metaObject()->className();
}

QDomDocument QueryListHandler::buildReply(const QDomDocument &source) const
{
    QDomDocument result;
    result.setContent(QString("<data xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' controlId='210' errorCondition='OK' lang='fr' type='reply_list' version='0.2' xsi:noNamespaceSchemaLocation='eep.xsd'> <request> <requestId>1699991114</requestId> <requestState>open</requestState> <patient> <name> <firstName>ALBERT4</firstName> <middleName/> <lastName>LE VERT</lastName> </name> <institute> <instituteId>SYLC</instituteId> <name/> </institute> <patientId>3011256</patientId> <stayNumber>1160083939</stayNumber> <birthdate>1985-09-10T00:00:00</birthdate> <gender>female</gender> <comments/> </patient> <mandator> <name> <firstName>GER</firstName> <middleName/> <lastName>GER1</lastName> </name> <contact> <address/> <city/> <postcode/> <state/> <country/> <emails></emails> <phones></phones> </contact> <institute> <instituteId>Consultation deriatrie</instituteId> <name/> <contact> <address/> <city/> <postcode/> <state/> <country/> <emails></emails> <phones></phones> </contact> </institute> <practicianId>GER1</practicianId> <title/> <birthdate/> <gender/> <comments/> </mandator> <sample> <id>813004135301</id> <sampleDate>2016-10-14T09:58:00</sampleDate> <arrivalDate>2016-10-14T10:01:37</arrivalDate> <concentrations> <concentration> <analyte>Gentamicine (taux au pic)</analyte> <value>2.7</value> <unit>mg/l</unit> </concentration> </concentrations> <comments/> </sample> <drug> <drugId>GNTP</drugId> <atc/> <brandName>Gentalline</brandName> <activePrinciple>Gentamicine (taux au pic)</activePrinciple> <comments/> </drug> </request> <request> <requestId>1610190698</requestId> <requestState>open</requestState> <patient> <name> <firstName>ROGER</firstName> <middleName/> <lastName>NEUENSCHWANDER</lastName> </name> <institute> <instituteId>16SC</instituteId> <name/> </institute> <patientId>827497</patientId> <stayNumber>316106701</stayNumber> <birthdate>1957-07-07T00:00:00</birthdate> <gender>male</gender> <comments/> </patient> <mandator> <name> <firstName>CAR</firstName> <middleName/> <lastName>CARH</lastName> </name> <contact> <address/> <city/> <postcode/> <state/> <country/> <emails></emails> <phones></phones> </contact> <institute> <instituteId>UNITE SOINS CONTINUS CARDIOLOGIE</instituteId> <name/> <contact> <address/> <city/> <postcode/> <state/> <country/> <emails></emails> <phones></phones> </contact> </institute> <practicianId>16SC</practicianId> <title/> <birthdate/> <gender/> <comments/> </mandator> <sample> <id>813004240210</id> <sampleDate>2016-10-19T06:00:00</sampleDate> <arrivalDate>2016-10-19T07:18:20</arrivalDate> <concentrations> <concentration> <analyte>Tacrolimus</analyte> <value>5.0</value> <unit>g/l</unit> </concentration> </concentrations> <comments/> </sample> <drug> <drugId>TACR</drugId> <atc/> <brandName>Prograf</brandName> <activePrinciple>Tacrolimus</activePrinciple> <comments/> </drug> </request> <request> <requestId>1699991127</requestId> <requestState>open</requestState> <patient> <name> <firstName>ALBERT4</firstName> <middleName/> <lastName>LE VERT</lastName> </name> <institute> <instituteId>SYLC</instituteId> <name/> </institute> <patientId>3011256</patientId> <stayNumber>1160083939</stayNumber> <birthdate>1985-09-10T00:00:00</birthdate> <gender>female</gender> <comments/> </patient> <mandator> <name> <firstName>GER</firstName> <middleName/> <lastName>GER1</lastName> </name> <contact> <address/> <city/> <postcode/> <state/> <country/> <emails></emails> <phones></phones> </contact> <institute> <instituteId>Consultation deriatrie</instituteId> <name/> <contact> <address/> <city/> <postcode/> <state/> <country/> <emails></emails> <phones></phones> </contact> </institute> <practicianId>GER1</practicianId> <title/> <birthdate/> <gender/> <comments/> </mandator> <sample> <id>813004275601</id> <sampleDate>2016-10-19T10:30:00</sampleDate> <arrivalDate>2016-10-19T10:35:11</arrivalDate> <concentrations> <concentration> <analyte>Amikacine (taux au pic)</analyte> <value>28.2</value> <unit>mg/l</unit> </concentration> </concentrations> <comments/> </sample> <drug> <drugId>AMIKP</drugId> <atc/> <brandName>Amiklin</brandName> <activePrinciple>Amikacine (taux au pic)</activePrinciple> <comments/> </drug> </request> <request> <requestId>1699991128</requestId> <requestState>open</requestState> <patient> <name> <firstName>ALBERT4</firstName> <middleName/> <lastName>LE VERT</lastName> </name> <institute> <instituteId>SYLC</instituteId> <name/> </institute> <patientId>3011256</patientId> <stayNumber>1160083939</stayNumber> <birthdate>1985-09-10T00:00:00</birthdate> <gender>female</gender> <comments/> </patient> <mandator> <name> <firstName>GER</firstName> <middleName/> <lastName>GER1</lastName> </name> <contact> <address/> <city/> <postcode/> <state/> <country/> <emails></emails> <phones></phones> </contact> <institute> <instituteId>Consultation deriatrie</instituteId> <name/> <contact> <address/> <city/> <postcode/> <state/> <country/> <emails></emails> <phones></phones> </contact> </institute> <practicianId>GER1</practicianId> <title/> <birthdate/> <gender/> <comments/> </mandator> <sample> <id>813004276301</id> <sampleDate>2016-10-19T10:42:00</sampleDate> <arrivalDate>2016-10-19T10:46:08</arrivalDate> <concentrations> <concentration> <analyte>Gentamicine (taux au pic)</analyte> <value>11.2</value> <unit>mg/l</unit> </concentration> </concentrations> <comments/> </sample> <drug> <drugId>GNTP</drugId> <atc/> <brandName>Gentalline</brandName> <activePrinciple>Gentamicine (taux au pic)</activePrinciple> <comments/> </drug> </request> </data> "));
//    //The <data> node
//    QDomElement rootNode = result.createElement("data");
//    result.appendChild(rootNode);

//    QDomAttr versionAttr = result.createAttribute("version");
//    versionAttr.setNodeValue("0.2");
//    rootNode.setAttributeNode(versionAttr);

//    QDomAttr langAttr = result.createAttribute("lang");
//    langAttr.setNodeValue("en");
//    rootNode.setAttributeNode(langAttr);

//    QDomAttr typeAttr = result.createAttribute("type");
//    typeAttr.setNodeValue("reply_list");
//    rootNode.setAttributeNode(typeAttr);

//    QDomAttr controlIdAttr = result.createAttribute("controlId");
//    controlIdAttr.setNodeValue(QString::number(ControlId::getInstance().next()));
//    rootNode.setAttributeNode(controlIdAttr);

//    QDomAttr errorConditionAttr = result.createAttribute("errorCondition");
//    errorConditionAttr.setNodeValue("OK");
//    rootNode.setAttributeNode(errorConditionAttr);

//    QDomAttr xsiAttr = result.createAttribute("xmlns:xsi");
//    xsiAttr.setNodeValue("http://www.w3.org/2001/XMLSchema-instance");
//    rootNode.setAttributeNode(xsiAttr);

//    QDomAttr nnslAttr = result.createAttribute("xsi:noNamespaceSchemaLocation");
//    nnslAttr.setNodeValue("eep.xsd");
//    rootNode.setAttributeNode(nnslAttr);

//    //The <request> node
//    QDomElement requestNode = result.createElement("request");
//    rootNode.appendChild(requestNode);

//    //The <requestId> node
//    QDomElement requestIdNode = source.documentElement().firstChildElement("dataset").firstChildElement("requestId");
//    requestNode.appendChild(requestIdNode);

//    //The <requestState> node
//    QDomElement requestStateNode = source.documentElement().firstChildElement("dataset").firstChildElement("requestState");
//    requestNode.appendChild(requestStateNode);

//    //The <patient> node
//    QDomElement patientNode = result.createElement("patient");
//    requestNode.appendChild(patientNode);

//    QDomElement nameNode = source.documentElement().firstChildElement("dataset").firstChildElement("patient").firstChildElement("name");
//    patientNode.appendChild(nameNode);

//    QDomElement instituteNode = result.createElement("institute");
//    patientNode.appendChild(instituteNode);

//    QDomElement instituteIdNode = source.documentElement().firstChildElement("dataset").firstChildElement("patient").firstChildElement("institute").firstChildElement("instituteId");
//    instituteNode.appendChild(instituteIdNode);

//    QDomElement instituteNameNode = source.documentElement().firstChildElement("dataset").firstChildElement("patient").firstChildElement("institute").firstChildElement("name");
//    instituteNode.appendChild(instituteNameNode);

//    QDomElement patientIdNode = source.documentElement().firstChildElement("dataset").firstChildElement("patient").firstChildElement("patientId");
//    patientNode.appendChild(patientIdNode);

//    QDomElement stayNumberNode = source.documentElement().firstChildElement("dataset").firstChildElement("patient").firstChildElement("stayNumber");
//    patientNode.appendChild(stayNumberNode);

//    QDomElement birthdateNode = source.documentElement().firstChildElement("dataset").firstChildElement("patient").firstChildElement("birthdate");
//    patientNode.appendChild(birthdateNode);

//    QDomElement genderNode = source.documentElement().firstChildElement("dataset").firstChildElement("patient").firstChildElement("gender");
//    patientNode.appendChild(genderNode);

//    QDomElement commentNode = source.documentElement().firstChildElement("dataset").firstChildElement("patient").firstChildElement("comments");
//    patientNode.appendChild(commentNode);

//    //The <mandator> node
//    QDomElement mandatorNode = source.documentElement().firstChildElement("dataset").firstChildElement("mandator");
//    requestNode.appendChild(mandatorNode);

//    //The <sample> node
//    QDomElement sampleNode = source.documentElement().firstChildElement("dataset").firstChildElement("samples").firstChildElement("sample");
//    requestNode.appendChild(sampleNode);

//    //The <drug> node
//    QDomElement drugNode = source.documentElement().firstChildElement("dataset").firstChildElement("drug");
//    requestNode.appendChild(drugNode);

    return result;
}
