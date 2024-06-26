#include "flatinterpretationrequestbuilder.h"


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


#include <QMessageBox>

#include "flatinterpretationrequestbuilder.h"
#include "rest/builders/drugidtranslator.h"
#include "rest/builders/routetranslator.h"
#include "rest/builders/formulationandroutetranslator.h"


#include "admin/src/adminfactory.h"

#include "apputils/src/apputilsfactory.h"
#include "apputils/src/apputilsrepository.h"


#include "core/core.h"
#include "core/corefactory.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/uncastedvalue.h"
#include "rest/builders/covariateidtranslator.h"
#include "core/dal/drug/target.h"
#include "guiutils/src/appglobals.h"

using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::Core;

namespace Tucuxi {
namespace Gui {
namespace FlatRequest {


//InterpretationRequestBuilder::InterpretationRequestBuilder(const MessageContent &content) :
FlatInterpretationRequestBuilder::FlatInterpretationRequestBuilder(const QDomDocument &content) :
    content(content)
{
    //Get the control ID
    //    content.setValue("controlId", doc.documentElement().attributeNode("controlId").value());
    reportNode = content.documentElement();

    //Get request data
    //    content.setValue("request.id",    datasetNode.firstChildElement("requestId").firstChild().toText().data());
    //    content.setValue("request.state", datasetNode.firstChildElement("requestState").firstChild().toText().data());

}


FlatInterpretationRequestBuilder::~FlatInterpretationRequestBuilder()
{

}

bool FlatInterpretationRequestBuilder::compareDosage(const Tucuxi::Gui::Core::Dosage* a, const Tucuxi::Gui::Core::Dosage* b)
{
    return (a->getApplied() < b->getApplied());
}

Tucuxi::Gui::Core::Duration FlatInterpretationRequestBuilder::findDuration(const QDomElement &currentElement)
{
    QDomElement element = currentElement.nextSiblingElement("Détails");
    Tucuxi::Gui::Core::Duration duration;

    // Find the first "durée" element, a duration of 0 will be rturned if no duration found
    while(!element.isNull()) {
        if(element.attribute("donnees") == "durée") {
            QString unit = element.attribute("unite");
            QString duree = element.attribute("valeur");

            // Currently only minutes should be used as unit for duration
            if(unit == "min") {
                duration = Tucuxi::Gui::Core::Duration(0, duree.toLongLong());
            }
            break;
        } else {
            element = element.nextSiblingElement("Détails");
        }
    }

    return duration;
}

void FlatInterpretationRequestBuilder::createUncastedIntervalValue(Tucuxi::Gui::Core::Dosage *dosage, int interval_sec)
{
    UncastedValue *uncasted = CoreFactory::createEntity<UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
    uncasted->setField("Interval");
    uncasted->setText(QString::number(interval_sec));
    uncasted->setComment("Interval was computed to 0 and therefore replaced by default value");
    dosage->getUncastedValues()->append(uncasted);

    uncasted = CoreFactory::createEntity<UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
    uncasted->setField("From");
    uncasted->setText(dosage->getApplied().toString("dd/MM/yy hh:mm"));
    uncasted->setComment("Verify if the date is not overlapping another dosage");
    dosage->getUncastedValues()->append(uncasted);

    uncasted = CoreFactory::createEntity<UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
    uncasted->setField("To");
    uncasted->setText(dosage->getEndTime().toString("dd/MM/yy hh:mm"));
    uncasted->setComment("Verify if the date is not overlapping another dosage");
    dosage->getUncastedValues()->append(uncasted);
}

void FlatInterpretationRequestBuilder::createUncastedDosageValue(Tucuxi::Gui::Core::Dosage *dosage, QString field, QString text, QString comment)
{
    UncastedValue *uncasted = CoreFactory::createEntity<UncastedValue>(ABSTRACTREPO, dosage->getUncastedValues());
    uncasted->setField(field);
    uncasted->setText(text);
    uncasted->setComment(comment);
    dosage->getUncastedValues()->append(uncasted);
}

void FlatInterpretationRequestBuilder::splitOverlappingDosage(Tucuxi::Gui::Core::DosageHistory *dosages)
{
    // Build a list of list of Dosages that overlap
    QList<QList<Tucuxi::Gui::Core::Dosage*>> overlapingDosages;

    // Sort the dosages by applied date
    dosages->sort(compareDosage);

    QList<Tucuxi::Gui::Core::Dosage*>::iterator it = dosages->getList().begin();
    QList<Tucuxi::Gui::Core::Dosage*>::iterator next;
    while (it != dosages->getList().end()) {
        QDateTime applied = (*it)->getApplied();
        QDateTime end = applied.addMSecs((*it)->getTinf().mSecs());

        // Check if next dosage exist and overlap
        next = it + 1;
        if((next != dosages->getList().end()) && ((*next)->getApplied() < end)) {
            // The dosages ovelap so we add both to the map and skip
            // the next as we just added it in the map
            QList<Tucuxi::Gui::Core::Dosage*> multipleDosages;
            multipleDosages.append(*it);
            multipleDosages.append(*next);
            overlapingDosages.append(multipleDosages);
            it += 2;
        } else {
            QList<Tucuxi::Gui::Core::Dosage*> singleDosage;
            singleDosage.append(*it);
            overlapingDosages.append(singleDosage);
            ++it;
        }
    }

    // Reset the dosage history list
    dosages->clear();

    // Iterate throught the list and treat overlaping dosage (if any)
    for(QList<QList<Tucuxi::Gui::Core::Dosage*>>::iterator it = overlapingDosages.begin(); it != overlapingDosages.end(); ++it) {
        // Cannot be empty and greater than 2
        Q_ASSERT(!(*it).isEmpty() && ((*it).size() <= 2));

        // Only single dosage
        if((*it).size() == 1) {
            dosages->append((*it).first());
        } else {
            // Multiple dosage overlapping
            // Get the start and end times of the both dosages
            QDateTime applied1 = (*it).first()->getApplied();
            QDateTime end1 = applied1.addMSecs((*it).first()->getTinf().mSecs());
            QDateTime applied2 = (*it).last()->getApplied();
            QDateTime end2 = applied2.addMSecs((*it).last()->getTinf().mSecs());

            // Add the two dosage value for the ovelapping part
            double totalDosageValue = (*it).first()->getQuantity()->getDbvalue() + (*it).last()->getQuantity()->getDbvalue();

            // Applied date are the same
            if(applied1 == applied2) {
                // Dosage are fully overlapping
                if(end1 == end2) {
                    (*it).first()->getQuantity()->setValue(totalDosageValue);

                    // Warning message
                    QString msg = "This dose was automatically computed from 2 dosages given at " +
                                  (*it).first()->getApplied().toString("dd/MM/yy hh:mm");
                    createUncastedDosageValue((*it).first(), "Dose", QString::number(totalDosageValue), msg);

                    dosages->append((*it).first());
                // End of dosage are not the same
                } else {
                    // Find the dosage that end the first
                    Tucuxi::Gui::Core::Dosage* firstDosage;
                    Tucuxi::Gui::Core::Dosage* secondDosage;
                    QDateTime firstDosageEnd;
                    QDateTime secondDosageEnd;


                    if(end1 < end2) {
                        firstDosage = (*it).first();
                        firstDosageEnd = end1;
                        secondDosage = (*it).last();
                        secondDosageEnd = end2;
                    } else {
                        firstDosage = (*it).last();
                        firstDosageEnd = end2;
                        secondDosage = (*it).first();
                        secondDosageEnd = end1;
                    }

                    // The overlapping part amount should be added in the first dosage
                    firstDosage->getQuantity()->setValue(totalDosageValue);

                    // The second dosage applied date and infusion time should be corrected
                    secondDosage->setApplied(firstDosageEnd);
                    secondDosage->setTinf(Duration(0, 0, 0, firstDosageEnd.msecsTo(secondDosageEnd)));

                    // First dosage warning message
                    QString msg = "This dose was automatically computed from 2 dosages given at " +
                                  (*it).first()->getApplied().toString("dd/MM/yy hh:mm");
                    createUncastedDosageValue(firstDosage, "Dose", QString::number(totalDosageValue), msg);
                    //Second dosage warning messag
                    msg = "This applied time was automatically adjusted";
                    createUncastedDosageValue(secondDosage, "From", secondDosage->getApplied().toString("dd/MM/yy hh:mm"), msg);
                    msg = "This infusion time was automatically adjusted";
                    createUncastedDosageValue(secondDosage, "Infusion", secondDosage->getTinf().toString(), msg);

                    dosages->append(firstDosage);
                    dosages->append(secondDosage);
                }
            // Applied date are different
            } else {
                // The first dosage infusion time finish when second dosage begin (overlapping part)
                (*it).first()->setTinf(Duration(0, 0, 0, applied1.msecsTo(applied2)));

                // Keep the quantity of last ending dosage in case a third dosage must be added
                double lastDosageValue = end1 > end2 ? (*it).first()->getQuantity()->getDbvalue() : (*it).last()->getQuantity()->getDbvalue();

                // Correct the second dosage to be the fully overlapping part of both dosages
                (*it).last()->getQuantity()->setValue(totalDosageValue);
                (*it).last()->setTinf(Duration(0, 0, 0, applied2.msecsTo(end1 < end2 ? end1 : end2)));

                // Warning messages
                // First dosage
                QString msg = "This infusion time was automatically adjusted";
                createUncastedDosageValue((*it).first(), "Infusion", (*it).first()->getTinf().toString(), msg);
                // Second dosage
                msg = "This dose was automatically computed from 2 dosages given at " +
                              (*it).last()->getApplied().toString("dd/MM/yy hh:mm");
                createUncastedDosageValue((*it).last(), "Dose", QString::number(totalDosageValue), msg);
                msg = "This infusion time was automatically adjusted";
                createUncastedDosageValue((*it).last(), "Infusion", (*it).last()->getTinf().toString(), msg);

                dosages->append((*it).first());
                dosages->append((*it).last());

                // If end date are not the same a third dosage should be added
                if(end1 != end2) {
                    Tucuxi::Gui::Core::Dosage* dosage = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Dosage>(ABSTRACTREPO, dosages);
                    dosage->setRoute((*it).last()->getRoute());
                    dosage->setIsAtSteadyState((*it).last()->getIsAtSteadyState());

                    dosage->setApplied(end1 < end2 ? end1 : end2);
                    // End date is currently the same as applied date because the end date is corrected later on the xml flat import processus
                    dosage->setEndTime(dosage->getApplied());

                    dosage->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit((*it).last()->getQuantity()->getUnitstring()));
                    dosage->getQuantity()->setValue(lastDosageValue);

                    dosage->setTinf(Duration(0, 0, 0, dosage->getApplied().msecsTo(end1 < end2 ? end2 : end1)));

                    //Warning
                    msg = "This is an automaticaly created dosage representing the end of a previous dosage";
                    createUncastedDosageValue(dosage, "Dose", QString::number(lastDosageValue), msg);
                    createUncastedDosageValue(dosage, "From", dosage->getApplied().toString("dd/MM/yy hh:mm"), msg);
                    createUncastedDosageValue(dosage, "Infusion", dosage->getTinf().toString(), msg);

                    dosages->append(dosage);
                }
            }
        }
    }
}

void FlatInterpretationRequestBuilder::setDosageEndDateInterval(Tucuxi::Gui::Core::DosageHistory* dosages)
{
    //Sort the dosages by applied date
    dosages->sort(compareDosage);
    //Iterate the dosages to corectly set interval time and end date
    QList<Tucuxi::Gui::Core::Dosage*>::iterator next;
    qint64 interval_sec = -1;
    for (QList<Tucuxi::Gui::Core::Dosage*>::iterator it = dosages->getList().begin(); it != dosages->getList().end(); ++it) {
        next = it + 1;
        //The end time and interval is computed using next dosage applied time, if any
        if(next != dosages->getList().end()) {
            (*it)->setEndTime((*next)->getApplied());
            interval_sec = (*it)->getApplied().secsTo((*it)->getEndTime());

            if(interval_sec > 0) {
                (*it)->setInterval(Tucuxi::Gui::Core::Duration(0,0,interval_sec));
            } else {
                createUncastedIntervalValue(*it, interval_sec);
            }

        //If there is no next dosage, check if there is one previous interval
        } else if(interval_sec != -1) {
            //Use last interval to compute end time
            (*it)->setEndTime((*it)->getApplied().addSecs(interval_sec));

            if(interval_sec > 0) {
                (*it)->setInterval(Tucuxi::Gui::Core::Duration(0,0,interval_sec));
            } else {
                createUncastedIntervalValue(*it, interval_sec);
            }
        }
        //else { TODO (JRP) : What to do if only one dosage ?
    }
}

void FlatInterpretationRequestBuilder::groupDosage(Tucuxi::Gui::Core::DosageHistory* dosages)
{
    if(dosages->isEmpty()) {
        return;
    }

    //Build a list for groupped Dosages
    QList<Tucuxi::Gui::Core::Dosage*> groupedDosages;
    QList<Tucuxi::Gui::Core::Dosage*>::iterator next;

    //Sort the dosages by applies date
    dosages->sort(compareDosage);

    //Iterate the dosages to group dosages that follows each otherwith same interval and infusion time
    for (QList<Tucuxi::Gui::Core::Dosage*>::iterator it = dosages->getList().begin(); it != dosages->getList().end(); ++it) {
        //Initiate the groupedDosage with the first dosage
        if(it == dosages->getList().begin()) {
            groupedDosages.append(*it);
        }

        next = it + 1;
        if(next != dosages->getList().end()) {
            //Check if next dosage begin when current dosage end and have the same quantity, interval and infusion time
            if(groupedDosages.last()->getEndTime() == (*next)->getApplied() &&
                groupedDosages.last()->getQuantity()->getDbvalue() == (*next)->getQuantity()->getDbvalue() &&
                groupedDosages.last()->getInterval() == (*next)->getInterval() &&
                groupedDosages.last()->getTinf() == (*next)->getTinf()) {

                groupedDosages.last()->setEndTime((*next)->getEndTime());
            } else {
                groupedDosages.append(*next);
            }
        }
    }

    //Clear and update the dosages list
    dosages->clear();
    for (QList<Tucuxi::Gui::Core::Dosage*>::iterator it = groupedDosages.begin(); it != groupedDosages.end(); ++it) {
        dosages->append(*it);
    }
}

InterpretationRequest* FlatInterpretationRequestBuilder::buildInterpretationRequest()
{
    InterpretationRequest* interpretationRequest = Tucuxi::Gui::Core::CoreFactory::createEntity<InterpretationRequest>(ABSTRACTREPO);
    Tucuxi::Gui::Core::DrugTreatment *treatment = interpretationRequest->getTreatment();
    treatment->setParent(interpretationRequest);


    //Build patient
    SharedPatient shpatient = AdminFactory::createEntity<Patient>(ABSTRACTREPO);
    //Patient data
    Patient* patient = static_cast<Patient*>(shpatient);

    QString activeSubstanceStr = reportNode.attribute("Name");
    QString activeSubstanceId = "";

    //Prediction drug
    if (activeSubstanceStr == "vanco fulldata") {
        activeSubstanceId = "vancomycin";
    } else if (activeSubstanceStr == "cefepime fulldata") {
        activeSubstanceId = "cefepime";
    } else if (activeSubstanceStr == "voriconazole fulldata") {
        activeSubstanceId = "voriconazole";
    }

    //Take the first details element
    //QDomElement detailsCollection = datasetNode.firstChildElement("Tablix1").firstChildElement("Détails_Collection");
    QDomElement detailElement = reportNode.firstChildElement("Tablix1").firstChildElement("Détails_Collection").firstChildElement("Détails");

    //Take the encounter id each details element have the same encounter id for the current interpretation
    patient->externalId(detailElement.attribute("encounterid"));

    //Build dosages to be filled when parsing the file
    Tucuxi::Gui::Core::DosageHistory* dosages = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DosageHistory>(ABSTRACTREPO);

    //Build measure to be filled when parsing the file
    Tucuxi::Gui::Core::CoreMeasureList* measures = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::CoreMeasureList>(ABSTRACTREPO);

    //Build covariate list when parsing the file
    Tucuxi::Gui::Core::PatientVariateList * covariates = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariateList>(ABSTRACTREPO, patient);

    //Iterate through all details element to built an interpretation request
    QString dataType;
    while (!detailElement.isNull()) {
        dataType = detailElement.attribute("donnees");

        if (dataType == "Sexe") {

            patient->person()->gender(detailElement.attribute("valeur").toLower() == "masculin" ? Person::Male : Person::Female);

        } else if (dataType == "DDN") {

            QString dateString = detailElement.attribute("valeur");
            QDate date;

            if (activeSubstanceStr == "vanco fulldata") {
                date = QDateTime::fromString(dateString, "MMM dd yyyy").date();
            } else if (activeSubstanceStr == "cefepime fulldata" || activeSubstanceStr == "voriconazole fulldata") {
                date = QDateTime::fromString(dateString, "MM-dd-yyyy hh:mm:ss").date();
            }

            patient->person()->birthday(date);

        } else if (dataType == "poids") {

            Tucuxi::Gui::Core::PatientVariate* covariate = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariate>(ABSTRACTREPO);
            QString covariateId = "bodyweight";
            covariate->setCovariateId(covariateId);

            QString dateString = detailElement.attribute("horaire");
            QDateTime date = QDateTime::fromString(dateString, Qt::ISODate);
            covariate->setDate(date);

            QString valueString = detailElement.attribute("valeur");
            QString unit = detailElement.attribute("unite", "kg");
            double value = valueString.toDouble();
            covariate->getQuantity()->setValue(value);
            covariate->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit(unit));

            covariate->setType(QMetaType::Double);

            covariates->append(covariate);

        } else if (dataType == "Dosage creat") {

            Tucuxi::Gui::Core::PatientVariate* covariate = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariate>(ABSTRACTREPO);
            QString covariateId = "creatinine";
            covariate->setCovariateId(covariateId);

            QString dateString = detailElement.attribute("horaire");
            QDateTime date = QDateTime::fromString(dateString, Qt::ISODate);
            covariate->setDate(date);

            QString valueString = detailElement.attribute("valeur");
            QString unit = detailElement.attribute("unite", "µmol/l");
            double value = valueString.toDouble();
            covariate->getQuantity()->setValue(value);
            covariate->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit(unit));

            covariate->setType(QMetaType::Double);

            covariates->append(covariate);

        } else if (dataType == "Dosage vanco") {

            Measure * measure = AdminFactory::createEntity<Measure>(ABSTRACTREPO, measures);

            measure->setSdrug(activeSubstanceId);

            QString dateString = detailElement.attribute("horaire");
            QDateTime date = QDateTime::fromString(dateString, Qt::ISODate);
            measure->setMoment(date);
            measure->arrivalDate(date);

            Tucuxi::Gui::Core::IdentifiableAmount * amt = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO, measure);
            QString valueString = detailElement.attribute("valeur");
            valueString.replace(',', '.');
            double value = valueString.toDouble();
            amt->setValue(value);
            amt->setUnit(Tucuxi::Gui::Core::Unit("mg/l"));

            measure->setConcentration(amt);

            measures->append(measure);

        } else if (dataType == "Dosage cefepime" || dataType == "Dosage Residuel cefepime") {

            Measure * measure = AdminFactory::createEntity<Measure>(ABSTRACTREPO, measures);

            measure->setSdrug(activeSubstanceId);

            QString dateString = detailElement.attribute("horaire");
            QDateTime date = QDateTime::fromString(dateString, Qt::ISODate);
            measure->setMoment(date);
            measure->arrivalDate(date);

            Tucuxi::Gui::Core::IdentifiableAmount * amt = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO, measure);
            QString valueString = detailElement.attribute("valeur");
            valueString.replace(',', '.');
            QString unit = detailElement.attribute("unite", "mg/l");
            unit = unit.toLower();
            double value = valueString.toDouble();
            amt->setValue(value);
            amt->setUnit(Tucuxi::Gui::Core::Unit(unit));

            measure->setConcentration(amt);

            measures->append(measure);

        } else if (dataType.startsWith("Tx Vorico")) {

            Measure * measure = AdminFactory::createEntity<Measure>(ABSTRACTREPO, measures);

            measure->setSdrug(activeSubstanceId);

            QString dateString = detailElement.attribute("horaire");
            QDateTime date = QDateTime::fromString(dateString, Qt::ISODate);
            measure->setMoment(date);
            measure->arrivalDate(date);

            Tucuxi::Gui::Core::IdentifiableAmount * amt = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO, measure);
            QString valueString = detailElement.attribute("valeur");
            valueString.replace(',', '.');
            QString unit = detailElement.attribute("unite", "mg/l");
            unit = unit.toLower();
            double value = valueString.toDouble();
            amt->setValue(value);
            amt->setUnit(Tucuxi::Gui::Core::Unit(unit));

            measure->setConcentration(amt);

            measures->append(measure);

        } else if (dataType == "debit") {

            Tucuxi::Gui::Core::Dosage* dosage = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Dosage>(ABSTRACTREPO, dosages);

            Tucuxi::Gui::Core::Admin *admin = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Admin>(ABSTRACTREPO, dosage);
            admin->setRoute(Tucuxi::Gui::Core::Admin::INFUSION);

            Tucuxi::Core::FormulationAndRoute formulationAndRoute(
                    Tucuxi::Core::Formulation::ParenteralSolution,
                    Tucuxi::Core::AdministrationRoute::IntravenousDrip,
                    Tucuxi::Core::AbsorptionModel::Infusion,
                    "");

            admin->setFormulationAndRoute(formulationAndRoute);

            dosage->setRoute(admin);

            QString dateString = detailElement.attribute("horaire");
            QDateTime appl = QDateTime::fromString(dateString, Qt::ISODate);
            dosage->setApplied(appl);

            QDateTime end = appl.addSecs(3599);
            dosage->setEndTime(end);

            //TODO here we should calculate a dosage but we have flow rate, keep it like this for testing import
            QString valueString = detailElement.attribute("valeur");
            double value = valueString.toDouble();
            dosage->getQuantity()->setValue(value);
            dosage->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit("ml/h"));

            // TODO (JRP) : Set 60 minutes infusion and interval time for testing
            dosage->setInterval(Tucuxi::Gui::Core::Duration(0,60));
            dosage->setTinf(Tucuxi::Gui::Core::Duration(0,60));

            //TODO (JRP) : Set at steady state or not ?
            //TODO : To be checked
            dosage->setIsAtSteadyState(false);

            dosages->append(dosage);

        } else if (dataType == "dose") {

            Tucuxi::Gui::Core::Dosage* dosage = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Dosage>(ABSTRACTREPO, dosages);

            Tucuxi::Gui::Core::Admin *admin = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Admin>(ABSTRACTREPO, dosage);
            admin->setRoute(Tucuxi::Gui::Core::Admin::INFUSION);

            Tucuxi::Core::FormulationAndRoute formulationAndRoute(
                    Tucuxi::Core::Formulation::ParenteralSolution,
                    Tucuxi::Core::AdministrationRoute::IntravenousDrip,
                    Tucuxi::Core::AbsorptionModel::Infusion,
                    "");

            admin->setFormulationAndRoute(formulationAndRoute);
            dosage->setRoute(admin);

            QString dateString = detailElement.attribute("horaire");
            QDateTime appl = QDateTime::fromString(dateString, Qt::ISODate);
            dosage->setApplied(appl);

            //QDateTime end = appl.addSecs(10800);
            //dosage->setEndTime(end);
            dosage->setEndTime(appl);

            QString valueString = detailElement.attribute("valeur");
            valueString.replace(',', '.');
            QString unit = detailElement.attribute("unite", "g");
            double value = valueString.toDouble();
            dosage->getQuantity()->setValue(value);
            dosage->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit(unit));

            // Find duration (Tinf corresponding to dosage)
            Tucuxi::Gui::Core::Duration tinf = findDuration(detailElement);
            dosage->setTinf(tinf);

            // If no tinf found add a warning
            if(tinf.isEmpty()) {
                createUncastedDosageValue(dosage, "Infusion", dosage->getTinf().toString(), "No correct infusion time found");
            }

            dosage->setIsAtSteadyState(false);

            dosages->append(dosage);

        }

        detailElement = detailElement.nextSiblingElement("Détails");
    }

    //Prediction patient
    treatment->setPatient(shpatient);
    treatment->getPatient()->setParent(treatment);

    treatment->setActiveSubstanceId(activeSubstanceId);

    splitOverlappingDosage(dosages);
    setDosageEndDateInterval(dosages);

    if(GuiUtils::AppGlobals::getInstance()->getGroupIntake()) {
        groupDosage(dosages);
    }

    //Prediction dosage
    treatment->setDosages(dosages);
    treatment->getDosages()->setParent(treatment);

    //Prediction samples
    treatment->setMeasures(measures);
    treatment->getMeasures()->setParent(treatment);

    //Prediction covariates
    treatment->setCovariates(covariates);

    return interpretationRequest;
}

}
}
}
