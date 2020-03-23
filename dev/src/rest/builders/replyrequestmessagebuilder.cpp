#include <QDomDocument>

#include "replyrequestmessagebuilder.h"
#include "rest/model/replyrequestxmlmessage.h"


ReplyRequestMessageBuilder::ReplyRequestMessageBuilder(QObject *parent) :
    XmlMessageBuilder(parent)
{

}

ReplyRequestMessageBuilder::~ReplyRequestMessageBuilder()
{

}

Message *ReplyRequestMessageBuilder::message() const
{
    Q_UNIMPLEMENTED();
    Message * ret = nullptr;
    return ret;
}

MessageContent ReplyRequestMessageBuilder::messageContent() const
{
    Q_ASSERT(_message);

    //ToDo: Add method in subclass to parse and control root tag attributes
    //ToDo: Add the comments
    //ToDo: Segment the code below

    QDomDocument doc;
    if (!doc.setContent(_message->data()))
        qFatal("Unknown error while setting the content of the QDomDocument of the REPLY_REQUEST message");

    MessageContent content;

    //Get the control ID
    content.setValue("controlId", doc.documentElement().attributeNode("controlId").value());
    QDomElement datasetNode = doc.documentElement().firstChildElement("dataset");

    //Get request data
    content.setValue("request.id",    datasetNode.firstChildElement("requestId").firstChild().toText().data());
    content.setValue("request.state", datasetNode.firstChildElement("requestState").firstChild().toText().data());

    //Get drug data
    content.setValue("drug.id",              datasetNode.firstChildElement("drug").firstChildElement("drugId").firstChild().toText().data());
    content.setValue("drug.atc",             datasetNode.firstChildElement("drug").firstChildElement("atc").firstChild().toText().data());
    content.setValue("drug.brandName",       datasetNode.firstChildElement("drug").firstChildElement("brandName").firstChild().toText().data());
    content.setValue("drug.activePrinciple", datasetNode.firstChildElement("drug").firstChildElement("activePrinciple").firstChild().toText().data());

    //Get dosages data
    QDomElement dosageNode = datasetNode.firstChildElement("dosages").firstChildElement("dosage");
    while (!dosageNode.isNull()) {
        MessageContent dosage;

        //Get dosage data
        dosage.setValue("intake", dosageNode.firstChildElement("intake").firstChild().toText().data());

        //Get dosage dates
        dosage.setValue("date.start", dosageNode.firstChildElement("startDate").firstChild().toText().data());
        dosage.setValue("date.last",  dosageNode.firstChildElement("lastDate").firstChild().toText().data());

        //Get dosage dose
        dosage.setValue("dose.value", dosageNode.firstChildElement("dose").firstChildElement("value").firstChild().toText().data());
        dosage.setValue("dose.unit",  dosageNode.firstChildElement("dose").firstChildElement("unit").firstChild().toText().data());

        //Get dosage interval
        dosage.setValue("interval.value", dosageNode.firstChildElement("interval").firstChildElement("value").firstChild().toText().data());
        dosage.setValue("interval.unit",  dosageNode.firstChildElement("interval").firstChildElement("unit").firstChild().toText().data());

        //Get dosage infusion
        dosage.setValue("infusion.value", dosageNode.firstChildElement("infusion").firstChildElement("value").firstChild().toText().data());
        dosage.setValue("infusion.unit",  dosageNode.firstChildElement("infusion").firstChildElement("unit").firstChild().toText().data());

        content.addToList("dosages", dosage);
        dosageNode = dosageNode.nextSiblingElement("dosage");
    }

    //Get samples data
    QDomElement sampleNode = datasetNode.firstChildElement("samples").firstChildElement("sample");
    while (!sampleNode.isNull()) {
        MessageContent sample;

        //Get sample data
        sample.setValue("id", sampleNode.firstChildElement("id").firstChild().toText().data());

        //Get sample dates
        sample.setValue("date.sample",  sampleNode.firstChildElement("sampleDate").firstChild().toText().data());
        sample.setValue("date.arrival", sampleNode.firstChildElement("arrivalDate").firstChild().toText().data());

        QDomElement concentrationNode = sampleNode.firstChildElement("concentrations").firstChildElement("concentration");
        while (!concentrationNode.isNull()) {
            MessageContent concentration;

            //Get concentration data
            concentration.setValue("analyte", concentrationNode.firstChildElement("analyte").firstChild().toText().data());
            concentration.setValue("value",   concentrationNode.firstChildElement("value").firstChild().toText().data());
            concentration.setValue("unit",    concentrationNode.firstChildElement("unit").firstChild().toText().data());

            sample.addToList("concentrations", concentration);
            concentrationNode = concentrationNode.nextSiblingElement("concentration");
        }

        content.addToList("samples", sample);
        sampleNode = sampleNode.nextSiblingElement("sample");
    }

    //Get covariates data
    QDomElement covariateNode = datasetNode.firstChildElement("covariates").firstChildElement("covariate");
    while (!covariateNode.isNull()) {
        MessageContent covariate;

        //Get covariate data
        covariate.setValue("name",   covariateNode.firstChildElement("name").firstChild().toText().data());
        covariate.setValue("date",   covariateNode.firstChildElement("date").firstChild().toText().data());
        covariate.setValue("nature", covariateNode.firstChildElement("nature").firstChild().toText().data());
        covariate.setValue("value",  covariateNode.firstChildElement("value").firstChildElement("value").firstChild().toText().data());
        covariate.setValue("unit",   covariateNode.firstChildElement("value").firstChildElement("unit").firstChild().toText().data());

        content.addToList("covariates", covariate);
        covariateNode = covariateNode.nextSiblingElement("covariate");
    }

    //Get clinicals data
    QDomElement clinicalNode = datasetNode.firstChildElement("clinicals").firstChildElement("clinical");
    while (!clinicalNode.isNull()) {
        MessageContent clinical;

        //Get clinical data
        clinical.setValue("name",  clinicalNode.firstChildElement("name").firstChild().toText().data());
        clinical.setValue("date",  clinicalNode.firstChildElement("date").firstChild().toText().data());
        clinical.setValue("value", clinicalNode.firstChildElement("value").firstChild().toText().data());

        content.addToList("clinicals", clinical);
        clinicalNode = clinicalNode.nextSiblingElement("clinical");
    }

    //Get patient data
    content.setValue("patient.id",          datasetNode.firstChildElement("patient").firstChildElement("patientId").firstChild().toText().data());
    content.setValue("patient.stayNumber",  datasetNode.firstChildElement("patient").firstChildElement("stayNumber").firstChild().toText().data());
    content.setValue("patient.name.first",  datasetNode.firstChildElement("patient").firstChildElement("name").firstChildElement("firstName").firstChild().toText().data());
    content.setValue("patient.name.middle", datasetNode.firstChildElement("patient").firstChildElement("name").firstChildElement("middleName").firstChild().toText().data());
    content.setValue("patient.name.last",   datasetNode.firstChildElement("patient").firstChildElement("name").firstChildElement("lastName").firstChild().toText().data());
    content.setValue("patient.birthDate",   datasetNode.firstChildElement("patient").firstChildElement("birthdate").firstChild().toText().data());
    content.setValue("patient.gender",      datasetNode.firstChildElement("patient").firstChildElement("gender").firstChild().toText().data());

    //Get patient contact data
    content.setValue("patient.contact.address",  datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("address").firstChild().toText().data());
    content.setValue("patient.contact.city",     datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("city").firstChild().toText().data());
    content.setValue("patient.contact.postcode", datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("postcode").firstChild().toText().data());
    content.setValue("patient.contact.state",    datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("state").firstChild().toText().data());
    content.setValue("patient.contact.country",  datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("country").firstChild().toText().data());

    //Get patient emails
    QDomElement patientEmailNode = datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("emails").firstChildElement("email");
    while (!patientEmailNode.isNull()) {
        MessageContent email;

        //Get email data
        email.setValue("email",  patientEmailNode.firstChild().toText().data());
        email.setValue("type",   patientEmailNode.attributeNode("type").value());

        content.addToList("patient.contact.emails", email);
        patientEmailNode = patientEmailNode.nextSiblingElement("email");
    }

    //Get patient phones
    QDomElement patientPhoneNode = datasetNode.firstChildElement("patient").firstChildElement("contact").firstChildElement("phones").firstChildElement("phone");
    while (!patientPhoneNode.isNull()) {
        MessageContent phone;

        //Get phone data
        phone.setValue("phone",  patientPhoneNode.firstChild().toText().data());
        phone.setValue("type",   patientPhoneNode.attributeNode("type").value());

        content.addToList("patient.contact.phones", phone);
        patientPhoneNode = patientPhoneNode.nextSiblingElement("phone");
    }

    //Get patient institute data
    content.setValue("patient.institute.id",   datasetNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("instituteId").firstChild().toText().data());
    content.setValue("patient.institute.name", datasetNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("name").firstChild().toText().data());

    //Get patient institute contact
    content.setValue("patient.institute.contact.address",  datasetNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("contact").firstChildElement("address").firstChild().toText().data());
    content.setValue("patient.institute.contact.city",     datasetNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("contact").firstChildElement("city").firstChild().toText().data());
    content.setValue("patient.institute.contact.postcode", datasetNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("contact").firstChildElement("postcode").firstChild().toText().data());
    content.setValue("patient.institute.contact.state",    datasetNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("contact").firstChildElement("state").firstChild().toText().data());
    content.setValue("patient.institute.contact.country",  datasetNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("contact").firstChildElement("country").firstChild().toText().data());

    //Get patient institute emails
    QDomElement patientInstituteEmailNode = datasetNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("contact").firstChildElement("emails").firstChildElement("email");
    while (!patientInstituteEmailNode.isNull()) {
        MessageContent email;

        //Get email data
        email.setValue("email",  patientInstituteEmailNode.firstChild().toText().data());
        email.setValue("type",   patientInstituteEmailNode.attributeNode("type").value());

        content.addToList("patient.institute.contact.emails", email);
        patientInstituteEmailNode = patientInstituteEmailNode.nextSiblingElement("email");
    }

    //Get patient institute phones
    QDomElement patientInstitutePhoneNode = datasetNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("contact").firstChildElement("phones").firstChildElement("phone");
    while (!patientInstitutePhoneNode.isNull()) {
        MessageContent phone;

        //Get phone data
        phone.setValue("phone",  patientInstitutePhoneNode.firstChild().toText().data());
        phone.setValue("type",   patientInstitutePhoneNode.attributeNode("type").value());

        content.addToList("patient.institute.contact.phones", phone);
        patientInstitutePhoneNode = patientInstitutePhoneNode.nextSiblingElement("phone");
    }

    //Get mandator data
    content.setValue("mandator.id",          datasetNode.firstChildElement("mandator").firstChildElement("practicianId").firstChild().toText().data());
    content.setValue("mandator.title",       datasetNode.firstChildElement("mandator").firstChildElement("title").firstChild().toText().data());
    content.setValue("mandator.name.first",  datasetNode.firstChildElement("mandator").firstChildElement("name").firstChildElement("firstName").firstChild().toText().data());
    content.setValue("mandator.name.middle", datasetNode.firstChildElement("mandator").firstChildElement("name").firstChildElement("middleName").firstChild().toText().data());
    content.setValue("mandator.name.last",   datasetNode.firstChildElement("mandator").firstChildElement("name").firstChildElement("lastName").firstChild().toText().data());
    content.setValue("mandator.birthDate",   datasetNode.firstChildElement("mandator").firstChildElement("birthdate").firstChild().toText().data());
    content.setValue("mandator.gender",      datasetNode.firstChildElement("mandator").firstChildElement("gender").firstChild().toText().data());

    //Get mandator contact data
    content.setValue("mandator.contact.address",  datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("address").firstChild().toText().data());
    content.setValue("mandator.contact.city",     datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("city").firstChild().toText().data());
    content.setValue("mandator.contact.postcode", datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("postcode").firstChild().toText().data());
    content.setValue("mandator.contact.state",    datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("state").firstChild().toText().data());
    content.setValue("mandator.contact.country",  datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("country").firstChild().toText().data());

    //Get mandator emails
    QDomElement mandatorEmailNode = datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("emails").firstChildElement("email");
    while (!mandatorEmailNode.isNull()) {
        MessageContent email;

        //Get email data
        email.setValue("email",  mandatorEmailNode.firstChild().toText().data());
        email.setValue("type",   mandatorEmailNode.attributeNode("type").value());

        content.addToList("mandator.contact.emails", email);
        mandatorEmailNode = mandatorEmailNode.nextSiblingElement("email");
    }

    //Get mandator phones
    QDomElement mandatorPhoneNode = datasetNode.firstChildElement("mandator").firstChildElement("contact").firstChildElement("phones").firstChildElement("phone");
    while (!mandatorPhoneNode.isNull()) {
        MessageContent phone;

        //Get phone data
        phone.setValue("phone",  mandatorPhoneNode.firstChild().toText().data());
        phone.setValue("type",   mandatorPhoneNode.attributeNode("type").value());

        content.addToList("mandator.contact.phones", phone);
        mandatorPhoneNode = mandatorPhoneNode.nextSiblingElement("phone");
    }

    //Get mandator institute data
    content.setValue("mandator.institute.id",   datasetNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("instituteId").firstChild().toText().data());
    content.setValue("mandator.institute.name", datasetNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("name").firstChild().toText().data());

    //Get mandator institute contact
    content.setValue("mandator.institute.contact.address",  datasetNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("address").firstChild().toText().data());
    content.setValue("mandator.institute.contact.city",     datasetNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("city").firstChild().toText().data());
    content.setValue("mandator.institute.contact.postcode", datasetNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("postcode").firstChild().toText().data());
    content.setValue("mandator.institute.contact.state",    datasetNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("state").firstChild().toText().data());
    content.setValue("mandator.institute.contact.country",  datasetNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("country").firstChild().toText().data());

    //Get mandator institute emails
    QDomElement mandatorInstituteEmailNode = datasetNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("emails").firstChildElement("email");
    while (!mandatorInstituteEmailNode.isNull()) {
        MessageContent email;

        //Get email data
        email.setValue("email",  mandatorInstituteEmailNode.firstChild().toText().data());
        email.setValue("type",   mandatorInstituteEmailNode.attributeNode("type").value());

        content.addToList("mandator.institute.contact.emails", email);
        mandatorInstituteEmailNode = mandatorInstituteEmailNode.nextSiblingElement("email");
    }

    //Get mandator institute phones
    QDomElement mandatorInstitutePhoneNode = datasetNode.firstChildElement("mandator").firstChildElement("institute").firstChildElement("contact").firstChildElement("phones").firstChildElement("phone");
    while (!mandatorInstitutePhoneNode.isNull()) {
        MessageContent phone;

        //Get phone data
        phone.setValue("phone",  mandatorInstitutePhoneNode.firstChild().toText().data());
        phone.setValue("type",   mandatorInstitutePhoneNode.attributeNode("type").value());

        content.addToList("mandator.institute.contact.phones", phone);
        mandatorInstitutePhoneNode = mandatorInstitutePhoneNode.nextSiblingElement("phone");
    }

    return content;
}
