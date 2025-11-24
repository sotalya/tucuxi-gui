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


#include <QBuffer>

#include "interpretationxmlexport.h"
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
#include "core/dal/drug/parameters.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/drug.h"
#include "core/dal/coremeasure.h"
#include "core/dal/drug/target.h"
#include "core/dal/covariate.h"
#include "core/dal/dosage.h"
#include "core/dal/uncastedvalue.h"
#include "core/dal/drug/adme.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/drug/parameters.h"

#include "admin/src/dal/practician.h"

using namespace Tucuxi::Gui::Admin;

InterpretationXmlExport::InterpretationXmlExport() = default;



QString interpretationTypeToString(Interpretation::InterpretationType t)
{
    switch (t) {
    case Interpretation::InterpretationType::FromRequest : return "fromRequest";
    case Interpretation::InterpretationType::NoRequest : return "noRequest";
    case Interpretation::InterpretationType::Undefined : return "undefined";
    }
    return "undefined";
}

QString InterpretationXmlExport::toXml(Interpretation *interpretation)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    writer.setDevice(&buffer);
    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0",false);

    QString namespaceUri = "http://www.w3.org/2001/XMLSchema-instance";
    writer.writeStartElement("interpretation");
    writer.writeNamespace(namespaceUri,"xsi");
    writer.writeAttribute("type","interpretation");
    writer.writeAttribute("version","0.2");
//    writer.writeAttribute(namespaceUri, "noNamespaceSchemaLocation", "eep.xsd");

    bool ok = true;

    writer.writeTextElement("interpretationType", interpretationTypeToString(interpretation->getInterpretationType()));

    ok &= save(interpretation->getAnalysis());
    ok &= save(interpretation->getRequest());
    ok &= save(interpretation->getDrugResponseAnalysis());
    ok &= save(interpretation->getAnalyst(), "analyst");
    ok &= save(interpretation->getValidationStatus());

    writer.writeStartElement("adjustment");
    writer.writeTextElement("adjustmentDateIsSet", interpretation->getAdjustmentDateIsSet() ? "true" : "false");
    writer.writeTextElement("adjustmentDate", interpretation->getAdjustmentDate().toString(Qt::ISODate));
    writer.writeTextElement("adjustmentWithLoadingDose", interpretation->getAdjustmentWithLoadingDose() ? "true" : "false");
    writer.writeTextElement("adjustmentWithRestPeriod", interpretation->getAdjustmentWithRestPeriod() ? "true" : "false");
    writer.writeEndElement(); // adjustment

    writer.writeTextElement("interpretationStartDate", interpretation->getStartInterpretationTime().toString(Qt::ISODate));
    if (interpretation->getValidateInterpretationTime().isValid())
        writer.writeTextElement("interpretationValidationDate", interpretation->getValidateInterpretationTime().toString(Qt::ISODate));

    writer.writeEndElement(); // interpretation

    writer.writeEndDocument();

    return QString(byteArray);
}



bool InterpretationXmlExport::save(InterpretationAnalysis *analysis)
{
    writer.writeStartElement("analysis");

    writer.writeTextElement("nextControl", analysis->getNextControl().toString(Qt::ISODate));
    writer.writeTextElement("dosageDescription", analysis->getDosageDescription());

    // We use a field tag in order to be more generic for future requirements

    writer.writeStartElement("field");
    writer.writeTextElement("name", "expectedness");
    writer.writeTextElement("value", analysis->getExpectedness());
    writer.writeEndElement(); // field

    writer.writeStartElement("field");
    writer.writeTextElement("name", "suitability");
    writer.writeTextElement("value", analysis->getSuitability());
    writer.writeEndElement(); // field

    writer.writeStartElement("field");
    writer.writeTextElement("name", "prediction");
    writer.writeTextElement("value", analysis->getPrediction());
    writer.writeEndElement(); // field

    writer.writeStartElement("field");
    writer.writeTextElement("name", "remonitoring");
    writer.writeTextElement("value", analysis->getRemonitoring());
    writer.writeEndElement(); // field

    writer.writeStartElement("field");
    writer.writeTextElement("name", "warning");
    writer.writeTextElement("value", analysis->getWarning());
    writer.writeEndElement(); // field

    // We do not save chartData

    writer.writeEndElement(); // analysis

    // There is no reason something would go wrong here
    return true;
}

bool InterpretationXmlExport::save(InterpretationRequest *request)
{
    writer.writeStartElement("interpretationRequest");
    bool ok = true;
    // TODO We just save the clinicals now. To be more complete later
    ok &= save(request->getClinicals());
    save(request->getPractician(), "physician");
    writer.writeEndElement(); // interpretationRequest

    return true;

}

bool InterpretationXmlExport::save(ClinicalSet *clinicals)
{
    writer.writeStartElement("clinicals");
    foreach (Clinical *clinical, *clinicals) {

        // We do not save patient or patient_id. These fields could maybe be removed

        writer.writeStartElement("clinical");
        writer.writeTextElement("name", clinical->getName());
        writer.writeTextElement("date", clinical->getDate().toString(Qt::ISODate));
        writer.writeTextElement("value", clinical->getValue());
        writer.writeTextElement("comment", clinical->getComment());
        writer.writeEndElement(); // clinical
    }
    writer.writeEndElement(); // clinicals

    return true;

}

bool InterpretationXmlExport::save(Tucuxi::Gui::Core::DrugResponseAnalysis *drugResponseAnalysis)
{
    writer.writeStartElement("drugResponseAnalysis");
    bool ok = true;
    ok &= save(drugResponseAnalysis->getTreatment());

    // With only save the drugModelId here. To be checked if everything goes well with that

    writer.writeTextElement("drugModelId", drugResponseAnalysis->getDrugModel()->getDrugModelId());
    writer.writeEndElement(); // drugResponseAnalysis

    return true;
}


bool InterpretationXmlExport::save(Practician *practician, QString name)
{
    writer.writeStartElement(name);
    writer.writeTextElement("personId", QString::number(practician->person_id()));
    writer.writeTextElement("externalId", practician->externalId());
    writer.writeTextElement("title", practician->title());
    if (name == "analyst")
        writer.writeTextElement("role", practician->role());
    writer.writeTextElement("address", practician->institute()->location()->address());
    writer.writeTextElement("affiliation", practician->institute()->name());
    save(practician->person(), name);
    writer.writeEndElement(); // practician

    return true;

}

QString InterpretationXmlExport::stepToString(int step)
{
    switch(step) {
    case StepType::Patient : return "patient";
    case StepType::Drug : return "drug";
    case StepType::Dosage : return "dosage";
    case StepType::Covariate : return "covariate";
    case StepType::Measure : return "measure";
    case StepType::Target : return "target";
    case StepType::Adjustment : return "adjustment";
    case StepType::Validation : return "validation";
    case StepType::Report : return "report";
    }

    // TODO : Generate an error
    return "unknown";
}

QString InterpretationXmlExport::toStringValidation(ValidationStatus::ValidationStatusType status)
{
    switch(status) {
    case ValidationStatus::ValidationStatusType::UnValidated : return "unvalidated";
    case ValidationStatus::ValidationStatusType::Validated : return "validated";
    }

    // TODO : Generate an error
    return "unknown";
}

bool InterpretationXmlExport::save(ValidationStatus *validationStatus)
{
    writer.writeStartElement("validationStatus");

    for(auto step = StepType::first; step <= StepType::last; step ++) {
        writer.writeStartElement("stepStatus");
        writer.writeTextElement("step", stepToString(step));
        QString s = toStringValidation(validationStatus->getValidationStatus(StepType::convert(step)));
        writer.writeTextElement("status", s);
        writer.writeEndElement(); // stepStatus
    }

    writer.writeEndElement(); // validationStatus
    return false;

}



QString InterpretationXmlExport::writeDate(QDateTime date)
{
    return date.toString(Qt::ISODate);
}

bool InterpretationXmlExport::saveIdentifiableAmount(const QString &tagName, Tucuxi::Gui::Core::IdentifiableAmount *amount)
{
    writer.writeStartElement(tagName);
    writer.writeTextElement("amountId", amount->getAmountId());
    writer.writeTextElement("value", QString("%1").arg(amount->getDbvalue()));
    writer.writeTextElement("unit", amount->getUnitstring());
    writer.writeEndElement();
    return true;
}

bool InterpretationXmlExport::saveOperableAmount(const QString &tagName, Tucuxi::Gui::Core::OperableAmount *amount)
{
    writer.writeStartElement(tagName);
    writer.writeTextElement("amountId", amount->getAmountId());
    writer.writeTextElement("value", QString("%1").arg(amount->getDbvalue()));
    writer.writeTextElement("unit", amount->getUnitstring());
    save(amount->getOperations());
    writer.writeEndElement();
    return true;
}

bool InterpretationXmlExport::save(Tucuxi::Gui::Core::OperationList *list)
{
    writer.writeStartElement("operations");
    foreach(Tucuxi::Gui::Core::Operation *operation, list->getList()) {
        writer.writeStartElement("operation");
        writer.writeTextElement("type",
                                 (operation->getType() == Tucuxi::Gui::Core::OperationType::HARDCODED) ? "Hardcoded" :
                                                                                                      (operation->getType() == Tucuxi::Gui::Core::OperationType::IMPORTED) ? "Imported" :
                                                                                                                                                                          "Noop");
        writer.writeTextElement("formula", operation->getFormula());
        writer.writeEndElement();
    }
    writer.writeEndElement();
    return true;
}


bool InterpretationXmlExport::save(Tucuxi::Gui::Core::DrugTreatment *treatment)
{
    writer.writeStartElement("treatment");

    writer.writeTextElement("activeSubstanceId", treatment->getActiveSubstanceId());
    if (treatment->getPatient() != nullptr)
        save(static_cast<Patient* >(treatment->getPatient()));
    save(treatment->getDosages());
    save(treatment->getCovariates());
    save(treatment->getMeasures());
    save(treatment->getTargets());
    save(treatment->getAdjustments(), "adjustments");
    writer.writeEndElement();
    return true;
}

bool InterpretationXmlExport::save(Patient *patient)
{
    writer.writeStartElement("patient");
    writer.writeTextElement("personId", QString::number(patient->person_id()));
    writer.writeTextElement("externalId", patient->externalId());
    writer.writeTextElement("stayNumber", patient->stayNumber());
    save(patient->person(), "patient");
    writer.writeEndElement(); // patient
    return true;
}


bool InterpretationXmlExport::save(Person *person, QString name)
{
    writer.writeStartElement("person");
    writer.writeTextElement("name", person->name());
    writer.writeTextElement("firstname", person->firstname());
    if (name == "patient"){
        // Only save the birthday if it is valid
        if (person->birthday().isValid()) {
            writer.writeTextElement("birthday", person->birthday().toString(Qt::ISODate));
        }
        writer.writeTextElement("gender", (person->gender() == Person::Male) ? "male" : "female");
        writer.writeTextElement("address", person->location()->address());
    }
    writer.writeTextElement("city", person->location()->city());
    writer.writeTextElement("postcode", person->location()->postcode());
    writer.writeTextElement("state", person->location()->state());
    writer.writeTextElement("country", person->location()->country());
    if (name == "analyst")
        save(person->getPhones());
    writer.writeEndElement(); // patient
    return true;
}

bool InterpretationXmlExport::save(PhoneList *list)
{
    writer.writeStartElement("phoneNumber");
    foreach(Phone *phone, list->getList()) {
        writer.writeTextElement("number", phone->getNumber());
    }
    writer.writeEndElement();
    return true;
}


bool InterpretationXmlExport::save(Tucuxi::Gui::Core::CoreMeasureList *list)
{
    writer.writeStartElement("measures");
    foreach (Tucuxi::Gui::Core::CoreMeasure *coreMeasure, list->getList()) {
        auto* measure = dynamic_cast<Measure *>(coreMeasure);
        // TODO : Do something in case it is not a measure?
        writer.writeStartElement("measure");
        writer.writeTextElement("analyteId", measure->getAnalyteId());
        writer.writeTextElement("enable", measure->getEnable() ? "true" : "false");
        writer.writeTextElement("sampleId", measure->sampleID());
        writer.writeTextElement("arrivalDate", writeDate(measure->arrivalDate()));
        writer.writeTextElement("moment", writeDate(measure->getMoment()));
        saveIdentifiableAmount("concentration", measure->getConcentration());
        save(measure->getUncastedValues());
        writer.writeEndElement();
    }
    writer.writeEndElement();
    return true;
}

bool InterpretationXmlExport::save(Tucuxi::Gui::Core::UncastedValueList *list)
{
    writer.writeStartElement("uncastedValues");
    foreach(Tucuxi::Gui::Core::UncastedValue *value, list->getList()) {
        writer.writeStartElement("uncastedValue");
        writer.writeTextElement("field", value->getField());
        writer.writeTextElement("value", value->getText());
        writer.writeEndElement();
    }
    writer.writeEndElement();
    return true;
}



QString toString(Tucuxi::Core::Formulation _formulation) {
    static std::map<Tucuxi::Core::Formulation, std::string> map = {
        {Tucuxi::Core::Formulation::Undefined, "undefined"},
        {Tucuxi::Core::Formulation::ParenteralSolution,"parenteralSolution"},
        {Tucuxi::Core::Formulation::OralSolution, "oralSolution"},
        {Tucuxi::Core::Formulation::Test, "test"}
    };
    return QString::fromStdString(map.at(_formulation));
}

QString toString(Tucuxi::Core::AdministrationRoute _admin) {

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

bool InterpretationXmlExport::save(Tucuxi::Gui::Core::AdjustmentDosage *dosage, const QString &tagName)
{
    writer.writeStartElement(tagName);
    writer.writeTextElement("type", dosage->getType());
    writer.writeTextElement("dbinterval", QString("%1").arg(dosage->getDbinterval()));
    writer.writeTextElement("dbtinf", QString("%1").arg(dosage->getDbtinf()));
//    writer.writeTextElement("route", dosage->getRoute()->getLabel());
    writer.writeStartElement("formulationAndRoute");
    writer.writeTextElement("formulation", toString(dosage->getRoute()->getFormulationAndRoute().getFormulation()));
    writer.writeTextElement("administrationName", QString::fromStdString(dosage->getRoute()->getFormulationAndRoute().getAdministrationName()));
    writer.writeTextElement("administrationRoute", toString(dosage->getRoute()->getFormulationAndRoute().getAdministrationRoute()));
    writer.writeEndElement();
    saveIdentifiableAmount("quantity", dosage->getQuantity());
    writer.writeTextElement("applied", writeDate(dosage->getApplied()));
    writer.writeTextElement("endTime", writeDate(dosage->getEndTime()));
    writer.writeTextElement("tinf", QString("%1").arg(dosage->getTinf().toMinutes()));
    writer.writeTextElement("interval", QString("%1").arg(dosage->getInterval().toMinutes()));
    save(dosage->getUncastedValues());
    writer.writeEndElement();
    return true;
}

bool InterpretationXmlExport::save(Tucuxi::Gui::Core::Dosage *dosage, const QString &tagName)
{
    writer.writeStartElement(tagName);
    writer.writeTextElement("dbinterval", QString("%1").arg(dosage->getDbinterval()));
    writer.writeTextElement("dbtinf", QString("%1").arg(dosage->getDbtinf()));
    //    writer.writeTextElement("route", dosage->getRoute()->getLabel());
    writer.writeStartElement("formulationAndRoute");
    writer.writeTextElement("formulation", toString(dosage->getRoute()->getFormulationAndRoute().getFormulation()));
    writer.writeTextElement("administrationName", QString::fromStdString(dosage->getRoute()->getFormulationAndRoute().getAdministrationName()));
    writer.writeTextElement("administrationRoute", toString(dosage->getRoute()->getFormulationAndRoute().getAdministrationRoute()));
    writer.writeEndElement();
    saveIdentifiableAmount("quantity", dosage->getQuantity());
    writer.writeTextElement("applied", writeDate(dosage->getApplied()));
    writer.writeTextElement("endTime", writeDate(dosage->getEndTime()));
    writer.writeTextElement("isAtSteadyState", dosage->getIsAtSteadyState() ? "true" : "false");
    writer.writeTextElement("steadyStateLastDoseTime", writeDate(dosage->getSteadyStateLastDoseDate()));
    writer.writeTextElement("tinf", QString("%1").arg(dosage->getTinf().toMinutes()));
    writer.writeTextElement("interval", QString("%1").arg(dosage->getInterval().toMinutes()));
    save(dosage->getUncastedValues());
    writer.writeEndElement();
    return true;
}

bool InterpretationXmlExport::save(Tucuxi::Gui::Core::DosageHistory *history, const QString &tagName)
{
    writer.writeStartElement(tagName);
    foreach(Tucuxi::Gui::Core::Dosage *dosage, history->getList()) {
        if (dynamic_cast<Tucuxi::Gui::Core::AdjustmentDosage*>(dosage))
            save(dynamic_cast<Tucuxi::Gui::Core::AdjustmentDosage*>(dosage));
        else
            save(dosage);
    }
    writer.writeEndElement();
    return true;
}

bool InterpretationXmlExport::save(Tucuxi::Gui::Core::PatientVariateList *list)
{
    writer.writeStartElement("patientVariates");
    foreach(Tucuxi::Gui::Core::PatientVariate *variate, list->getList()) {
        writer.writeStartElement("patientVariate");
        writer.writeTextElement("name", variate->getCovariateId());
        writer.writeTextElement("date", writeDate(variate->getDate()));
        writer.writeTextElement("valueAsString", variate->getValueAsString());
        if (variate->getCovariateId() != "birthdate")
            saveOperableAmount("quantity", variate->getQuantity());
        writer.writeTextElement("type", QMetaType::typeName(variate->getType()));
        writer.writeTextElement("forced", variate->getForced() ? "true" : "false");
        save(variate->getUncastedValues());
        writer.writeEndElement();
    }
    writer.writeEndElement();
    return true;
}

bool InterpretationXmlExport::save(Tucuxi::Gui::Core::TargetList *list)
{
    writer.writeStartElement("targets");
    foreach(Tucuxi::Gui::Core::Target *target, list->getList()) {
        writer.writeStartElement("target");
        writer.writeTextElement("method", target->getType()->getLabel());
        writer.writeStartElement("concentrations");
        saveOperableAmount("cmin", target->getCmin());
        saveOperableAmount("cmax", target->getCmax());
        saveOperableAmount("cbest", target->getCbest());
        writer.writeEndElement();
        writer.writeStartElement("times");
        saveOperableAmount("tmin", target->getTmin());
        saveOperableAmount("tmax", target->getTmax());
        saveOperableAmount("tbest", target->getTbest());
        writer.writeEndElement();
        saveOperableAmount("mic", target->getMic());
        writer.writeEndElement();
    }
    writer.writeEndElement();
    return true;
}

bool InterpretationXmlExport::save(Tucuxi::Gui::Core::ADME *adme)
{
    writer.writeStartElement("adme");
    writer.writeTextElement("defaultIntake", adme->getDefaultIntake()->getLabel());
    writer.writeTextElement("distribution", adme->getDistribution());
    writer.writeTextElement("elimination", adme->getElimination());
    writer.writeStartElement("intakes");
    foreach(Tucuxi::Gui::Core::DMAdmin *admin, adme->getIntakes()->getList()) {
        writer.writeTextElement("intake", admin->getLabel());
    }
    writer.writeEndElement();
    writer.writeEndElement();
    return true;
}
