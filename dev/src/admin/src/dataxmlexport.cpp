//@@license@@

#include <iostream>

#include <QBuffer>

#include "dataxmlexport.h"
#include "dal/interpretation.h"
#include "dal/interpretationanalysis.h"
#include "dal/interpretationrequest.h"
#include "dal/measure.h"
#include "dal/clinical.h"
#include "dal/patient.h"
#include "dal/validationstatus.h"
#include "dal/steptypes.h"

#include "core/dal/covariate.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drug/drug.h"
#include "core/dal/coremeasure.h"
#include "core/dal/predictionspec.h"
#include "core/dal/drug/parameters.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/drug.h"
#include "core/dal/coremeasure.h"
#include "core/dal/drug/target.h"
#include "core/dal/covariate.h"
#include "core/dal/dosage.h"
#include "core/dal/amount.h"
#include "core/dal/uncastedvalue.h"
#include "core/dal/drug/adme.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/drug/bsv.h"
#include "core/dal/drug/errormodel.h"
#include "core/dal/drug/parameters.h"

#include "admin/src/dal/practician.h"


using namespace Tucuxi::Gui::Admin;

DataXmlExport::DataXmlExport()
{}

QString _toString(Tucuxi::Core::Formulation _formulation) {
    static std::map<Tucuxi::Core::Formulation, std::string> map = {
        {Tucuxi::Core::Formulation::Undefined, "undefined"},
        {Tucuxi::Core::Formulation::ParenteralSolution,"parenteral solution"},
        {Tucuxi::Core::Formulation::OralSolution, "oral solution"},
        {Tucuxi::Core::Formulation::Test, "test"}
    };
    return QString::fromStdString(map.at(_formulation));
}

QString _toString(Tucuxi::Core::AdministrationRoute _admin) {

    static std::map<Tucuxi::Core::AdministrationRoute, std::string> m =
    {
        {Tucuxi::Core::AdministrationRoute::Undefined, "undefined"},
        {Tucuxi::Core::AdministrationRoute::Intramuscular, "intramuscular"},
        {Tucuxi::Core::AdministrationRoute::IntravenousBolus, "intravenousBolus"},
        {Tucuxi::Core::AdministrationRoute::IntravenousDrip, "intravenousDrip"},
        {Tucuxi::Core::AdministrationRoute::Nasal, "nasal"},
        {Tucuxi::Core::AdministrationRoute::Oral, "oral"},
        {Tucuxi::Core::AdministrationRoute::Rectal, "rectal"},
        {Tucuxi::Core::AdministrationRoute::Subcutaneous, "subcutaneous"},
        {Tucuxi::Core::AdministrationRoute::Sublingual, "sublingual"},
        {Tucuxi::Core::AdministrationRoute::Transdermal, "transdermal"},
        {Tucuxi::Core::AdministrationRoute::Vaginal, "vaginal"}
    };
    return QString::fromStdString(m.at(_admin));
}

QString _toString(Tucuxi::Core::AbsorptionModel _model) {
    static std::map<Tucuxi::Core::AbsorptionModel, std::string> m =
    {
        {Tucuxi::Core::AbsorptionModel::Undefined, "undefined"},
        {Tucuxi::Core::AbsorptionModel::Intravascular, "bolus"},
        {Tucuxi::Core::AbsorptionModel::Extravascular, "extra"},
        {Tucuxi::Core::AbsorptionModel::ExtravascularLag, "extra.lag"},
        {Tucuxi::Core::AbsorptionModel::Infusion, "infusion"}
    };
    return QString::fromStdString(m.at(_model));
}

QString DataXmlExport::writeDate(QDateTime date)
{
    return date.toString(Qt::ISODate);
}

QString DataXmlExport::toXml(Interpretation *interpretation)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    writer.setDevice(&buffer);
    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0", false);

    QString namespaceUri = "http://www.w3.org/2001/XMLSchema-instance";
    writer.writeStartElement("data");
    writer.writeNamespace(namespaceUri,"xsi");
    writer.writeAttribute("type", "data");
    writer.writeAttribute("version", "0.2");

    if (interpretation->getDrugResponseAnalysis()->getTreatment() != nullptr)
        save(static_cast<Patient* >(interpretation->getDrugResponseAnalysis()->getTreatment()->getPatient()));
    writer.writeTextElement("date", writeDate(QDateTime::currentDateTime()));

    bool ok = true;
    ok &= save(interpretation->getDrugResponseAnalysis());

    writer.writeEndElement(); // End of data
    writer.writeEndDocument();

    return QString(byteArray);
}

bool DataXmlExport::save(Patient *patient)
{
    writer.writeTextElement("clientId", patient->externalId());
    return true;
}

bool DataXmlExport::save(Tucuxi::Gui::Core::DrugResponseAnalysis *drugResponseAnalysis)
{
    writer.writeStartElement("drugTreatment");
    writer.writeStartElement("patient");

    bool ok = true;
    ok &= save(drugResponseAnalysis->getTreatment()->getCovariates());

    writer.writeEndElement(); // End of patient

    writer.writeStartElement("drugs");
    writer.writeStartElement("drug");

    ok &= save(drugResponseAnalysis->getDrugModel()->getActiveSubstance());
    ok &= save(drugResponseAnalysis->getTreatment());

    writer.writeEndElement(); // End of drug
    writer.writeEndElement(); // End of drugs

    writer.writeEndElement(); // End of drugTreatment
    return true;
}

bool DataXmlExport::save(Tucuxi::Gui::Core::ActiveSubstance *substance)
{
    writer.writeTextElement("drugId", substance->getSubstanceId());
    writer.writeTextElement("atc", substance->getAtc()[0]);
    return true;
}

bool DataXmlExport::save(Tucuxi::Gui::Core::DrugTreatment *treatment)
{
    save(treatment->getDosages());
    save(treatment->getMeasures());

    return true;
}

bool DataXmlExport::save(Tucuxi::Gui::Core::PatientVariateList *list)
{
    writer.writeStartElement("covariates");
    foreach(Tucuxi::Gui::Core::PatientVariate *variate, list->getList()) {
        writer.writeStartElement("covariate");
        writer.writeTextElement("covariateId", variate->getCovariateId());
        writer.writeTextElement("date", writeDate(variate->getDate()));
        if (variate->getCovariateId() != "birthdate") {
            writer.writeTextElement("unit", variate->getQuantity()->getUnitstring());
            writer.writeTextElement("value", QString("%1").arg(variate->getQuantity()->getDbvalue()));
        } else {
            writer.writeTextElement("value", variate->getValueAsString());
        }
        writer.writeTextElement("dataType", QMetaType::typeName(variate->getType()));
        writer.writeEndElement(); // End of covariate
    }
    writer.writeEndElement(); // End of covariates
    return true;
}

bool DataXmlExport::save(Tucuxi::Gui::Core::CoreMeasureList *list)
{
    writer.writeStartElement("samples");
    foreach (Tucuxi::Gui::Core::CoreMeasure *coreMeasure, list->getList()) {
        Measure *measure = dynamic_cast<Measure *>(coreMeasure);
        writer.writeStartElement("sample");
        writer.writeTextElement("sampleId", measure->sampleID());
        writer.writeTextElement("sampleDate", writeDate(measure->getMoment()));
        saveIdentifiableAmount("concentration", measure->getConcentration());
        writer.writeEndElement(); // End of sample
    }
    writer.writeEndElement(); // End of samples
    return true;
}

bool DataXmlExport::save(Tucuxi::Gui::Core::Dosage *dosage)
{
    writer.writeStartElement("dosage");
    writer.writeStartElement("dosageLoop");
    writer.writeStartElement("lastingDosage");

    writer.writeTextElement("interval", QString("%1").arg(dosage->getDbinterval()));

    writer.writeStartElement("dose");
    writer.writeTextElement("value", QString("%1").arg(dosage->getQuantity()->getDbvalue()));
    writer.writeTextElement("unit", dosage->getQuantity()->getUnitstring());
    writer.writeTextElement("infusionTimeInMinutes", QString("%1").arg(dosage->getDbtinf()));
    writer.writeEndElement(); // End of dose

    writer.writeStartElement("formulationAndRoute");
    writer.writeTextElement("formulation", _toString(dosage->getRoute()->getFormulationAndRoute().getFormulation()));
    writer.writeTextElement("administrationName", QString::fromStdString(dosage->getRoute()->getFormulationAndRoute().getAdministrationName()));
    writer.writeTextElement("administrationRoute", _toString(dosage->getRoute()->getFormulationAndRoute().getAdministrationRoute()));
    writer.writeTextElement("absorptionModel", _toString(dosage->getRoute()->getFormulationAndRoute().getAbsorptionModel()));
    writer.writeEndElement(); // End of formulationAndRoute

    writer.writeEndElement(); // End of lastingDosage
    writer.writeEndElement(); // End of dosageLoop
    writer.writeEndElement(); // End of dosage
    return true;
}

bool DataXmlExport::save(Tucuxi::Gui::Core::DosageHistory *history)
{
    writer.writeStartElement("treatment");
    writer.writeStartElement("dosageHistory");
    foreach(Tucuxi::Gui::Core::Dosage *dosage, history->getList()) {
        writer.writeTextElement("start", writeDate(dosage->getApplied()));
        writer.writeTextElement("end", writeDate(dosage->getEndTime()));
        save(dosage);
    }
    writer.writeEndElement(); // End of dosageHistory
    writer.writeEndElement(); // End of treatment
    return true;
}

bool DataXmlExport::saveIdentifiableAmount(const QString &tagName, Tucuxi::Gui::Core::IdentifiableAmount *amount)
{
    writer.writeStartElement(tagName);
    writer.writeTextElement("amountId", amount->getAmountId());
    writer.writeTextElement("value", QString("%1").arg(amount->getDbvalue()));
    writer.writeTextElement("unit", amount->getUnitstring());
    writer.writeEndElement();
    return true;
}
