#include "iccainterpretationrequestbuilder.h"


//@@license@@

#include <QMessageBox>

#include "iccainterpretationrequestbuilder.h"
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

using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::Core;

namespace Tucuxi {
namespace Gui {
namespace ICCA {


//InterpretationRequestBuilder::InterpretationRequestBuilder(const MessageContent &content) :
ICCAInterpretationRequestBuilder::ICCAInterpretationRequestBuilder(const QDomDocument &content) :
    content(content)
{
    //Get the control ID
    //    content.setValue("controlId", doc.documentElement().attributeNode("controlId").value());
    reportNode = content.documentElement();

    //Get request data
    //    content.setValue("request.id",    datasetNode.firstChildElement("requestId").firstChild().toText().data());
    //    content.setValue("request.state", datasetNode.firstChildElement("requestState").firstChild().toText().data());

}


ICCAInterpretationRequestBuilder::~ICCAInterpretationRequestBuilder()
{

}

bool ICCAInterpretationRequestBuilder::compareDosage(const Tucuxi::Gui::Core::Dosage* a, const Tucuxi::Gui::Core::Dosage* b)
{
    return (a->getApplied() < b->getApplied());
}

void ICCAInterpretationRequestBuilder::createUncastedIntervalValue(Tucuxi::Gui::Core::Dosage *dosage, int interval_sec)
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
#if 0
void ICCAInterpretationRequestBuilder::concatenateDosages(Tucuxi::Gui::Core::DosageHistory *dosages)
{
    // Build a map of dosages, map key is the dosage applied time, (by default QMap are sorted by asc order)
    QMap<QDateTime, QList<Tucuxi::Gui::Core::Dosage*>> dosagebyTime;

    for (QList<Tucuxi::Gui::Core::Dosage*>::iterator it = dosages->getList().begin(); it != dosages->getList().end(); ++it) {
        QDateTime applied = (*it)->getApplied();

        QList<Tucuxi::Gui::Core::Dosage*> currentDosages;

        if(dosagebyTime.contains(applied)) {
            currentDosages = dosagebyTime.value(applied);
        }

        currentDosages.append(*it);
        dosagebyTime.insert(applied, currentDosages);
    }

    // Reset the dosage history list
    dosages->clear();
    // Tucuxi::Gui::Core::DosageHistory* resultDosages = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DosageHistory>(ABSTRACTREPO);

    // Iterate throught the map by key order
    for(const QDateTime key : dosagebyTime.keys()) {
        QList<Tucuxi::Gui::Core::Dosage*> currentDosages = dosagebyTime.value(key);

        // Cannot be empty for a given QDateTime (key)
        Q_ASSERT(!currentDosages.isEmpty());

        if(currentDosages.size() == 1) {
            dosages->append(currentDosages.first());
        } else {
            bool okToAdd = true;
            double totalDosageValue = 0.0;
            Duration previousTinf = currentDosages.first()->getTinf();
            QString previousUnit = currentDosages.first()->getQuantity()->getUnitstring();
            Tucuxi::Gui::Core::Dosage* lastDosage = nullptr;

            // Check if dosage in the list for the given time can be added to have only one dosage at the end
            // Add dosages values
            for(QList<Tucuxi::Gui::Core::Dosage*>::iterator it = currentDosages.begin(); it != currentDosages.end(); ++it) {
                okToAdd = okToAdd && (previousTinf == (*it)->getTinf()) && (previousUnit == (*it)->getQuantity()->getUnitstring());
                previousTinf = (*it)->getTinf();
                previousUnit = (*it)->getQuantity()->getUnitstring();

                totalDosageValue += (*it)->getQuantity()->getDbvalue();

                lastDosage = (*it);
            }

            // If dosage can be added, create the resulting dosage
            if(okToAdd) {
                Q_ASSERT(lastDosage != nullptr);
                lastDosage->getQuantity()->setValue(totalDosageValue);

                UncastedValue *uncasted = CoreFactory::createEntity<UncastedValue>(ABSTRACTREPO, lastDosage->getUncastedValues());
                uncasted->setField("Dose");
                uncasted->setText(QString::number(totalDosageValue));
                QString msg = "This dose was automatically computed from " + QString::number(currentDosages.size()) +
                              " dosages given at " + lastDosage->getApplied().toString("dd/MM/yy hh:mm");
                uncasted->setComment(msg);
                lastDosage->getUncastedValues()->append(uncasted);

                dosages->append(lastDosage);
            } else {
                // If dosage cannot be added then keep them as it is
                for(QList<Tucuxi::Gui::Core::Dosage*>::iterator it = currentDosages.begin(); it != currentDosages.end(); ++it) {
                    dosages->append(*it);
                }
            }
        }
    }
}
#endif
void ICCAInterpretationRequestBuilder::concatenateDosages(Tucuxi::Gui::Core::DosageHistory *dosages)
{
    // Build a map of dosages, map key is the dosage applied time, (by default QMap are sorted by asc order)
    QMap<QDateTime, QList<Tucuxi::Gui::Core::Dosage*>> dosagebyTime;
    // Build a list of list of Dosages that overlap
    QList<QList<Tucuxi::Gui::Core::Dosage*>> overlapingDosages;

    for (QList<Tucuxi::Gui::Core::Dosage*>::iterator it = dosages->getList().begin(); it != dosages->getList().end(); ++it) {
        QDateTime applied = (*it)->getApplied();

        QList<Tucuxi::Gui::Core::Dosage*> currentDosages;

        if(dosagebyTime.contains(applied)) {
            currentDosages = dosagebyTime.value(applied);
        }

        currentDosages.append(*it);
        dosagebyTime.insert(applied, currentDosages);
    }

    // Reset the dosage history list
    dosages->clear();
    // Tucuxi::Gui::Core::DosageHistory* resultDosages = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DosageHistory>(ABSTRACTREPO);

    // Iterate throught the map by key order
    for(const QDateTime key : dosagebyTime.keys()) {
        QList<Tucuxi::Gui::Core::Dosage*> currentDosages = dosagebyTime.value(key);

        // Cannot be empty for a given QDateTime (key)
        Q_ASSERT(!currentDosages.isEmpty());

        if(currentDosages.size() == 1) {
            dosages->append(currentDosages.first());
        } else {
            bool okToAdd = true;
            double totalDosageValue = 0.0;
            Duration previousTinf = currentDosages.first()->getTinf();
            QString previousUnit = currentDosages.first()->getQuantity()->getUnitstring();
            Tucuxi::Gui::Core::Dosage* lastDosage = nullptr;

            // Check if dosage in the list for the given time can be added to have only one dosage at the end
            // Add dosages values
            for(QList<Tucuxi::Gui::Core::Dosage*>::iterator it = currentDosages.begin(); it != currentDosages.end(); ++it) {
                okToAdd = okToAdd && (previousTinf == (*it)->getTinf()) && (previousUnit == (*it)->getQuantity()->getUnitstring());
                previousTinf = (*it)->getTinf();
                previousUnit = (*it)->getQuantity()->getUnitstring();

                totalDosageValue += (*it)->getQuantity()->getDbvalue();

                lastDosage = (*it);
            }

            // If dosage can be added, create the resulting dosage
            if(okToAdd) {
                Q_ASSERT(lastDosage != nullptr);
                lastDosage->getQuantity()->setValue(totalDosageValue);

                UncastedValue *uncasted = CoreFactory::createEntity<UncastedValue>(ABSTRACTREPO, lastDosage->getUncastedValues());
                uncasted->setField("Dose");
                uncasted->setText(QString::number(totalDosageValue));
                QString msg = "This dose was automatically computed from " + QString::number(currentDosages.size()) +
                              " dosages given at " + lastDosage->getApplied().toString("dd/MM/yy hh:mm");
                uncasted->setComment(msg);
                lastDosage->getUncastedValues()->append(uncasted);

                dosages->append(lastDosage);
            } else {
                // If dosage cannot be added then keep them as it is
                for(QList<Tucuxi::Gui::Core::Dosage*>::iterator it = currentDosages.begin(); it != currentDosages.end(); ++it) {
                    dosages->append(*it);
                }
            }
        }
    }
}

void ICCAInterpretationRequestBuilder::setDosageEndDateInterval(Tucuxi::Gui::Core::DosageHistory* dosages)
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

InterpretationRequest* ICCAInterpretationRequestBuilder::buildInterpretationRequest()
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
    //TODO JRP : use a dictionary of config file
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

            patient->person()->gender(detailElement.attribute("valeur") == "Masculin" ? Person::Male : Person::Female);

        } else if (dataType == "DDN") {

            QString dateString = detailElement.attribute("valeur");
            QDate date;

            // TODO (JRP) : The date should have the same format for all XML files
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

//        } else if (dataType == "Dosage vanco") {

//            Tucuxi::Gui::Core::Dosage* dosage = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Dosage>(ABSTRACTREPO, dosages);

//            Tucuxi::Gui::Core::Admin *admin = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Admin>(ABSTRACTREPO, dosage);
//            admin->setRoute(Tucuxi::Gui::Core::Admin::INFUSION); //TODO perfusion/infusion, use rate to compute dosage

//            Tucuxi::Core::FormulationAndRoute formulationAndRoute(
//                    Tucuxi::Core::Formulation::ParenteralSolution,
//                    Tucuxi::Core::AdministrationRoute::IntravenousDrip,
//                    Tucuxi::Core::AbsorptionModel::Infusion,
//                    "");

//            admin->setFormulationAndRoute(formulationAndRoute);

//            dosage->setRoute(admin);

//            QString dateString = detailElement.attribute("horaire");
//            QDateTime appl = QDateTime::fromString(dateString, Qt::ISODate);
//            dosage->setApplied(appl);

//            QString valueString = detailElement.attribute("valeur");
//            valueString.replace(',', '.');
//            double value = valueString.toDouble();
//            dosage->getQuantity()->setValue(value);

//            // TODO (JRP) : Set 60 minutes infusion and interval time for testing
//            dosage->setInterval(Tucuxi::Gui::Core::Duration(0,60));
//            dosage->setTinf(Tucuxi::Gui::Core::Duration(0,60));

//            //TODO (JRP) : No dosage interval ? cf. perfusion

//            //TODO (JRP) : Set at steady state or not ?
//            //TODO : To be checked
//            dosage->setIsAtSteadyState(false);

//            dosages->append(dosage);

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
            admin->setRoute(Tucuxi::Gui::Core::Admin::INFUSION); //TODO perfusion/infusion, use rate to compute dosage

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

            //TODO (JRP) : interval and duration should be deducted from the XML
            if (activeSubstanceStr == "cefepime fulldata") {
                //dosage->setInterval(Tucuxi::Gui::Core::Duration(8));
                dosage->setTinf(Tucuxi::Gui::Core::Duration(0,180));
            } else if (activeSubstanceStr == "voriconazole fulldata") {
                //dosage->setInterval(Tucuxi::Gui::Core::Duration(12));
                dosage->setTinf(Tucuxi::Gui::Core::Duration(0,120));
            }

            //TODO (JRP) : Set at steady state or not ?
            //TODO : To be checked
            dosage->setIsAtSteadyState(false);

            dosages->append(dosage);

        }

        detailElement = detailElement.nextSiblingElement("Détails");
    }

    //Prediction patient
    treatment->setPatient(shpatient);
    treatment->getPatient()->setParent(treatment);

    //TODO JRP : use a dictionary of config file
//    if (activeSubstanceStr == "vanco fulldata") {
//        treatment->setActiveSubstanceId("vancomycin");
//    } else if (activeSubstanceStr == "cefepime fulldata") {
//        treatment->setActiveSubstanceId("cefepime");
//    }

    treatment->setActiveSubstanceId(activeSubstanceId);

    concatenateDosages(dosages);
    setDosageEndDateInterval(dosages);

    //Prediction dosage
    treatment->setDosages(dosages);
    treatment->getDosages()->setParent(treatment);

    //Prediction samples
    treatment->setMeasures(measures);
    treatment->getMeasures()->setParent(treatment);

    //Prediction covariates
    treatment->setCovariates(covariates);

    //interpretationRequest->setClinicals(buildClinical("clinicals"));

    //targets

    return interpretationRequest;
}

}
}
}
