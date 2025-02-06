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


#include <iostream>

#include <QBuffer>
#include <QSettings>

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
        {Tucuxi::Core::Formulation::ParenteralSolution,"parenteralSolution"},
        {Tucuxi::Core::Formulation::OralSolution, "oralSolution"},
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

QString DataXmlExport::toCdssXml(Interpretation *interpretation, QString substanceId, QString drugId)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    writer.setDevice(&buffer);
    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0", false);

    QString namespaceUri = "http://www.w3.org/2001/XMLSchema-instance";
    writer.writeStartElement("query");
    writer.writeNamespace(namespaceUri,"xsi");
    writer.writeAttribute("type", "data");
    writer.writeAttribute("version", "0.2");
    writer.writeTextElement("date", writeDate(QDateTime::currentDateTime()));

    if (interpretation->getDrugResponseAnalysis()->getTreatment() != nullptr) {
        saveAdminData(interpretation);
        save(interpretation->getDrugResponseAnalysis());

        // Save the request.
        writer.writeStartElement("requests");
        writer.writeStartElement("xpertRequest");
        writer.writeTextElement("drugId", substanceId);
        writer.writeTextElement("configId", drugId);

        writer.writeStartElement("output");
        writer.writeTextElement("format", "html");
        writer.writeTextElement("language", "en");
        writer.writeEndElement(); // End of output

        writer.writeTextElement("adjustmentDate", interpretation->getAdjustmentDate().toString(Qt::ISODate));

        writer.writeStartElement("options");

        if (interpretation->getAdjustmentWithLoadingDose()) {
            writer.writeTextElement("loadingOption", "loadingDoseAllowed");
        } else {
            writer.writeTextElement("loadingOption", "noLoadingDose");
        }
        if (interpretation->getAdjustmentWithRestPeriod()) {
            writer.writeTextElement("restPeriodOption", "restPeriodAllowed");
        } else {
            writer.writeTextElement("restPeriodOption", "noRestPeriod");
        }
        writer.writeEndElement(); // End of options
        writer.writeEndElement(); // End of xpertRequest
        writer.writeEndElement(); // End of requests
    }

    writer.writeEndElement(); // End of query
    writer.writeEndDocument();

    return QString(byteArray);
}

bool DataXmlExport::save(Patient *patient)
{
    writer.writeTextElement("clientId", patient->externalId());
    return true;
}

bool DataXmlExport::saveAdminData(Interpretation *interpretation)
{
    writer.writeStartElement("admin");
    writer.writeStartElement("mandator");
    Practician *practician = static_cast<Practician* >(interpretation->getAnalyst());
    if (practician != nullptr) {
        writer.writeStartElement("person");
        writer.writeTextElement("id", practician->externalId());
        writer.writeTextElement("title", practician->title());
        if (practician->person() != nullptr) {
            writer.writeTextElement("firstName", practician->person()->firstname());
            writer.writeTextElement("lastName", practician->person()->name());
            writer.writeStartElement("address");
            writer.writeTextElement("street", practician->person()->location()->address());
            writer.writeTextElement("postalCode", practician->person()->location()->postcode());
            writer.writeTextElement("city", practician->person()->location()->city());
            writer.writeTextElement("state", practician->person()->location()->state());
            writer.writeTextElement("country", practician->person()->location()->country());
            writer.writeEndElement(); // End of address
            writer.writeStartElement("phone");
            if (!practician->person()->getPhones()->isEmpty()){
                writer.writeTextElement("number", practician->person()->getPhones()->at(0)->getNumber());
                if (practician->person()->getPhones()->at(0)->getType() == PhoneType::Private) {
                    writer.writeTextElement("type", "private");
                } else if (practician->person()->getPhones()->at(0)->getType() == PhoneType::Professional) {
                    writer.writeTextElement("type", "professional");
                } else {
                    writer.writeEmptyElement("type");
                }

            } else {
                writer.writeEmptyElement("number");
                writer.writeEmptyElement("type");
            }
            writer.writeEndElement(); // End of phone
            writer.writeStartElement("email");
            if(!practician->person()->emails().empty()) {
                writer.writeTextElement("address", practician->person()->emails().at(0)->getEmail());
                if (practician->person()->emails().at(0)->getType() == Type::Private) {
                    writer.writeTextElement("type", "private");
                } else if (practician->person()->emails().at(0)->getType() == Type::Professional) {
                    writer.writeTextElement("type", "professional");
                } else {
                    writer.writeEmptyElement("type");
                }
            } else {
                writer.writeEmptyElement("address");
                writer.writeEmptyElement("type");
            }
            writer.writeEndElement(); // End of email
        }
        writer.writeEndElement(); // End of person

        writer.writeStartElement("institute");
        if (practician->institute() != nullptr) {
            writer.writeTextElement("id", practician->institute()->externalId());
            writer.writeTextElement("name", practician->institute()->name());
            if (practician->institute()->location() != nullptr) {
                writer.writeStartElement("address");
                writer.writeTextElement("street", practician->institute()->location()->address());
                writer.writeTextElement("postalCode", practician->institute()->location()->postcode());
                writer.writeTextElement("city", practician->institute()->location()->city());
                writer.writeTextElement("state", practician->institute()->location()->state());
                writer.writeTextElement("country", practician->institute()->location()->country());
                writer.writeEndElement(); // End of address
                writer.writeStartElement("phone");
                if (!practician->institute()->phones().empty()){
                    writer.writeTextElement("number", practician->institute()->phones().at(0)->getNumber());
                    if (practician->institute()->phones().at(0)->getType() == PhoneType::Private) {
                        writer.writeTextElement("type", "private");
                    } else if (practician->institute()->phones().at(0)->getType() == PhoneType::Professional) {
                        writer.writeTextElement("type", "professional");
                    } else {
                        writer.writeEmptyElement("type");
                    }
                } else {
                    writer.writeEmptyElement("number");
                    writer.writeEmptyElement("type");
                }
                writer.writeEndElement(); // End of phone
                writer.writeStartElement("email");
                if(!practician->institute()->emails().empty()){
                    writer.writeTextElement("address", practician->institute()->emails().at(0)->getEmail());
                    if(practician->institute()->emails().at(0)->getType() == Type::Private){
                        writer.writeTextElement("type", "private");
                    } else if (practician->institute()->emails().at(0)->getType() == Type::Professional) {
                        writer.writeTextElement("type", "professional");
                    } else {
                        writer.writeEmptyElement("type");
                    }
                } else {
                    writer.writeEmptyElement("address");
                    writer.writeEmptyElement("type");
                }
                writer.writeEndElement(); // End of email
            }
        }
        writer.writeEndElement(); // End of institute
    }
    writer.writeEndElement(); // End of mandator

    Patient *patient = static_cast<Patient* >(interpretation->getDrugResponseAnalysis()->getTreatment()->getPatient());
    writer.writeStartElement("patient");
    if (interpretation->getDrugResponseAnalysis()->getTreatment()->getPatient() != nullptr) {
        Person *person = static_cast<Person* >(patient->person());
        if (person != nullptr) {
            writer.writeStartElement("person");
            writer.writeTextElement("id", patient->externalId());

            Person::GenderType gender = person->gender();
            if (gender == Person::GenderType::Female)
                writer.writeTextElement("title", "Mme");
            else {
                if (gender == Person::GenderType::Male) {
                    writer.writeTextElement("title", "Mr");
                } else {
                    writer.writeTextElement("title", "Ind");
                }
            }

            writer.writeTextElement("firstName", person->firstname());
            writer.writeTextElement("lastName", person->name());
            writer.writeStartElement("address");
            if (person->location() != nullptr) {
                writer.writeTextElement("street", person->location()->address());
                writer.writeTextElement("postalCode", person->location()->postcode());
                writer.writeTextElement("city", person->location()->city());
                writer.writeTextElement("state", person->location()->state());
                writer.writeTextElement("country", person->location()->country());
            }
            writer.writeEndElement(); // End of address

            // writer.writeStartElement("phone");
            // writer.writeTextElement("number", person->getPhones()->at(0)->getNumber());
            // PhoneType phoneType(person->getPhones()->at(0)->getType());
            // if (phoneType == PhoneType::Private) {
            //     writer.writeTextElement("type", "private");
            // } else {
            //     if (phoneType == PhoneType::Professional) {
            //         writer.writeTextElement("type", "professional");
            //     } else {
            //         if (phoneType == PhoneType::Cell) {
            //             writer.writeTextElement("type", "cellphone");
            //         } else {
            //             writer.writeTextElement("type", "unknown");
            //         }
            //     }
            // }
            // writer.writeEndElement(); // End of phone

            // writer.writeStartElement("email");
            // writer.writeTextElement("address", person->emails().at(0)->getEmail());
            // Type emailType(person->emails().at(0)->getType());
            // if (emailType == Type::Private) {
            //     writer.writeTextElement("type", "private");
            // } else {
            //     if (emailType == Type::Professional) {
            //         writer.writeTextElement("type", "professional");
            //     } else {
            //         writer.writeTextElement("type", "unknown");
            //     }
            // }
            // writer.writeEndElement(); // End of email
            writer.writeEndElement(); // End of person
        }
    }
    writer.writeEndElement(); // End of patient
    writer.writeEndElement(); // End of admin

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
    writer.writeTextElement("activePrinciple", substance->getSubstanceId());
    writer.writeTextElement("brandName", "somebrand");
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
        if (variate->getCovariateId() != "age") {
            writer.writeStartElement("covariate");
            writer.writeTextElement("covariateId", variate->getCovariateId());
            writer.writeTextElement("date", writeDate(variate->getDate()));
            if (variate->getCovariateId() != "birthdate") {
                writer.writeTextElement("unit", variate->getQuantity()->getUnitstring());
                writer.writeTextElement("value", QString("%1").arg(variate->getQuantity()->getDbvalue()));
                writer.writeTextElement("dataType", QMetaType::typeName(variate->getType()));
            } else {
                writer.writeTextElement("value", variate->getValueAsString());
                writer.writeTextElement("dataType", "date");
            }
            writer.writeTextElement("nature", "discrete");
            writer.writeEndElement(); // End of covariate
        }
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

    // TODO : That's wrong, just a test here
    writer.writeTextElement("interval", QString("%1:00:00").arg(dosage->getDbinterval()));

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
        writer.writeStartElement("dosageTimeRange");
        writer.writeTextElement("start", writeDate(dosage->getApplied()));
        writer.writeTextElement("end", writeDate(dosage->getEndTime()));
        save(dosage);
        writer.writeEndElement(); // End of dosageTimeRange
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
