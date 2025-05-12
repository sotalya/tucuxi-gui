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

#include "interpretationtorequestxmlexport.h"
#include "dal/interpretation.h"
#include "dal/interpretationanalysis.h"
#include "dal/interpretationrequest.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/drug.h"
#include "core/dal/coremeasure.h"
#include "dal/measure.h"
#include "core/dal/covariate.h"
#include "dal/clinical.h"
#include "dal/patient.h"

#include "core/dal/drug/translatablestring.h"

using namespace Tucuxi::Gui::Admin;

InterpretationToRequestXmlExport::InterpretationToRequestXmlExport()
{
}

QString InterpretationToRequestXmlExport::toXml(Interpretation *interpretation)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    writer.setDevice(&buffer);
    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0",false);

    QString namespaceUri = "http://www.w3.org/2001/XMLSchema-instance";
    writer.writeStartElement("data");
    writer.writeNamespace(namespaceUri,"xsi");
    writer.writeAttribute("controlId","cid");
    writer.writeAttribute("errorCondition","OK");
    writer.writeAttribute("lang","FRA");
    writer.writeAttribute("type","reply_request");
    writer.writeAttribute("version","0.2");
    writer.writeAttribute(namespaceUri, "noNamespaceSchemaLocation", "eep.xsd");

    writer.writeStartElement("dataset");
    writer.writeTextElement("requestId", "12345678");
    writer.writeTextElement("requestState","open");

    bool ok = true;
    // Todo : something about it
//    ok &= saveActiveSubstance(interpretation->getRequest()->getTreatment()->getDrug());
    ok &= saveDosages(interpretation->getRequest()->getTreatment()->getDosages());
    ok &= saveSamples(interpretation->getRequest()->getTreatment()->getMeasures());
    ok &= saveCovariates(interpretation->getRequest()->getTreatment()->getCovariates());
    ok &= saveClinicals(interpretation->getRequest()->getClinicals());
    ok &= savePatient(dynamic_cast<Patient*>(interpretation->getRequest()->getTreatment()->getPatient()));


    writer.writeEndElement(); // dataset

    writer.writeEndElement(); // data

    writer.writeEndDocument();

    QString result = QString(byteArray);

    std::cout << "----------------------------------" << std::endl;
    std::cout << qPrintable(result) << std::endl;
    std::cout << "----------------------------------" << std::endl;


    return QString(byteArray);

}

bool InterpretationToRequestXmlExport::saveActiveSubstance(Tucuxi::Gui::Core::ActiveSubstance *activeSubstance)
{
    writer.writeStartElement("drug");
    writer.writeTextElement("drugId", activeSubstance->getSubstanceId());
    QStringList atcList = activeSubstance->getAtc();
    // We troncate the ATC list. Could be modified once
    if (atcList.isEmpty())
        writer.writeEmptyElement("atc");
    else
        writer.writeTextElement("atc", atcList.first());

    QStringList brandList = activeSubstance->getBrands();
    // We troncate the brand name list. Could be modified once
    if (brandList.isEmpty())
        writer.writeEmptyElement("brandName");
    else
        writer.writeTextElement("brandName", brandList.first());

    writer.writeTextElement("activePrinciple", activeSubstance->getName()->value());
    writer.writeEmptyElement("comments");

    writer.writeEndElement(); // drug

    return true;
}

bool InterpretationToRequestXmlExport::saveDosages(Tucuxi::Gui::Core::DosageHistory *dosageHistory)
{
    writer.writeStartElement("dosages");
    for (int i = 0; i < dosageHistory->size(); i++) {
        Tucuxi::Gui::Core::Dosage *dosage = dosageHistory->at(i);
        writer.writeStartElement("dosage");
        writer.writeTextElement("startDate", dosage->getApplied().toString(Qt::ISODate));
        writer.writeTextElement("lastDate", dosage->getEndTime().toString(Qt::ISODate));
        writer.writeStartElement("dose");
        writer.writeTextElement("value", QString::number(dosage->getQuantity()->value()));
        writer.writeTextElement("unit", dosage->getQuantity()->unit().name());
        writer.writeEndElement(); // dose
        writer.writeStartElement("interval");
        writer.writeTextElement("value", QString::number(dosage->getInterval().toHours()));
        writer.writeTextElement("unit", "h");
        writer.writeEndElement(); // interval
        writer.writeStartElement("infusion");
        writer.writeTextElement("value", QString::number(dosage->getTinf().toMinutes()));
        writer.writeTextElement("unit", "m");
        writer.writeEndElement(); // infusion
        // TODO: Rewrite with FormulationAndRoute
        // However not used
        // writer.writeTextElement("intake", dosage->getRoute()->getLabel());
        writer.writeEmptyElement("comments");
        writer.writeEndElement(); // dosage
    }
    writer.writeEndElement(); // dosages

    return true;
}

bool InterpretationToRequestXmlExport::saveSamples(Tucuxi::Gui::Core::CoreMeasureList *samples)
{
    writer.writeStartElement("samples");
    for (int i = 0; i < samples->size(); i++) {
        Measure *sample = dynamic_cast<Measure*>(samples->at(i));
        writer.writeStartElement("sample");
        writer.writeTextElement("id", sample->sampleID());
        writer.writeTextElement("sampleDate", sample->getMoment().toString(Qt::ISODate));
        writer.writeTextElement("arrivalDate", sample->arrivalDate().toString(Qt::ISODate));
        writer.writeStartElement("concentrations");
        // We only support one analyte now...
        writer.writeStartElement("concentration");
        writer.writeTextElement("analyte", sample->getSdrug());
        writer.writeTextElement("value", QString::number(sample->getConcentration()->value()));
        writer.writeTextElement("unit", sample->getConcentration()->unit().name());

        writer.writeEndElement(); // concentration
        writer.writeEndElement(); // concentrations
        writer.writeEmptyElement("comments");
        writer.writeEndElement(); // sample
    }
    writer.writeEndElement(); // samples

    return true;
}

bool InterpretationToRequestXmlExport::saveCovariates(Tucuxi::Gui::Core::PatientVariateList *covariates)
{
    writer.writeStartElement("covariates");
    for (int i = 0; i < covariates->size(); i++) {
        Tucuxi::Gui::Core::PatientVariate *covariate = covariates->at(i);
        writer.writeStartElement("covariate");
        // Or covariate->getName() ... To be solved
        writer.writeTextElement("name", covariate->getCovariateId());
        writer.writeTextElement("date", covariate->getDate().toString(Qt::ISODate));
        writer.writeStartElement("value");
        writer.writeTextElement("value", QString::number(covariate->getQuantity()->value()));
        writer.writeTextElement("unit", covariate->getQuantity()->unit().name());
        writer.writeEndElement(); // value
        // Not handled yet
        writer.writeTextElement("nature", "continuous");
        writer.writeEmptyElement("comments");
        writer.writeEndElement(); // covariate
    }
    writer.writeEndElement(); // covariates

    return true;
}

bool InterpretationToRequestXmlExport::saveClinicals(ClinicalSet *clinicals)
{
    if (clinicals == nullptr) {
        writer.writeEmptyElement("clinicals");
        return true;
    }
    writer.writeStartElement("clinicals");
    for (int i = 0; i < clinicals->size(); i++) {
        Clinical *clinical = clinicals->at(i);
        writer.writeStartElement("clinical");
        writer.writeTextElement("name", clinical->getName());
        writer.writeTextElement("date", clinical->getDate().toString(Qt::ISODate));
        writer.writeTextElement("value", clinical->getValue());
        writer.writeEmptyElement("comments");
        writer.writeEndElement(); // clinical
    }
    writer.writeEndElement(); // clinicals

    return true;
}

bool InterpretationToRequestXmlExport::savePatient(Patient *patient)
{
    writer.writeStartElement("patient");
    writer.writeStartElement("name");
    writer.writeTextElement("firstName", patient->person()->firstname());
    // Not yet supported by the dal
    writer.writeTextElement("middleName", "");
    writer.writeTextElement("lastName", patient->person()->name());
    writer.writeEndElement(); // name
    writer.writeTextElement("patientId", patient->externalId());
    writer.writeTextElement("stayNumber", patient->stayNumber());
    writer.writeTextElement("birthdate", patient->person()->birthday().toString(Qt::ISODate));
    writer.writeTextElement("gender", patient->person()->gender() == Person::Male ? "male" : "female");
    writer.writeEmptyElement("comments");
    writer.writeEndElement(); // patient

    return true;

}
