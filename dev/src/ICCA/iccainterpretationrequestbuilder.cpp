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
        activeSubstanceId = "ch.heig-vd.ezechiel.vancomycin.adult.1CP";
    } else if (activeSubstanceStr == "cefepime fulldata") {
        activeSubstanceId = "ch.tucuxi.cefepime.buclin2020";
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
            // QDate date = QDateTime::fromString(dateString, Qt::ISODate).date();
            QDate date = QDateTime::fromString(dateString, "MM-dd-yyyy hh:mm:ss").date();
            patient->person()->birthday(date);

        } else if (dataType == "poids") {

            Tucuxi::Gui::Core::PatientVariate* covariate = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariate>(ABSTRACTREPO);
            QString covariateId = "bodyweight";
            covariate->setCovariateId(covariateId);

            QString dateString = detailElement.attribute("horaire");
            QDateTime date = QDateTime::fromString(dateString, Qt::ISODate);
            covariate->setDate(date);

            QString valueString = detailElement.attribute("valeur");
            QString unit = detailElement.attribute("unite");
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
            QString unit = detailElement.attribute("unite");
            double value = valueString.toDouble();
            covariate->getQuantity()->setValue(value);
            covariate->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit(unit));

            covariate->setType(QMetaType::Double);

            covariates->append(covariate);

        } else if (dataType == "Dosage vanco") {

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

            QString valueString = detailElement.attribute("valeur");
            valueString.replace(',', '.');
            double value = valueString.toDouble();
            dosage->getQuantity()->setValue(value);

            // TODO (JRP) : Set 60 minutes infusion and interval time for testing
            dosage->setInterval(Tucuxi::Gui::Core::Duration(0,60));
            dosage->setTinf(Tucuxi::Gui::Core::Duration(0,60));

            //TODO (JRP) : No dosage interval ? cf. perfusion

            //TODO (JRP) : Set at steady state or not ?
            //TODO : To be checked
            dosage->setIsAtSteadyState(false);

            dosages->append(dosage);

        } else if (dataType == "concentration") {

            Measure * measure = AdminFactory::createEntity<Measure>(ABSTRACTREPO, measures);

            measure->setSdrug(activeSubstanceId);

            QString dateString = detailElement.attribute("horaire");
            QDateTime date = QDateTime::fromString(dateString, Qt::ISODate);
            measure->setMoment(date);
            measure->arrivalDate(date);

            Tucuxi::Gui::Core::IdentifiableAmount * amt = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO, measure);
            QString valueString = detailElement.attribute("valeur");
            double value = valueString.toDouble();
            amt->setValue(value);
            amt->setUnit(Tucuxi::Gui::Core::Unit("mmol/l"));

            measure->setConcentration(amt);

            measures->append(measure);

        } else if (dataType == "Dosage Residuel cefepime") {

            Measure * measure = AdminFactory::createEntity<Measure>(ABSTRACTREPO, measures);

            measure->setSdrug(activeSubstanceId);

            QString dateString = detailElement.attribute("horaire");
            QDateTime date = QDateTime::fromString(dateString, Qt::ISODate);
            measure->setMoment(date);
            measure->arrivalDate(date);

            Tucuxi::Gui::Core::IdentifiableAmount * amt = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO, measure);
            QString valueString = detailElement.attribute("valeur");
            QString unit = detailElement.attribute("unite");
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

            QDateTime end = appl.addSecs(10800);
            dosage->setEndTime(end);

            QString valueString = detailElement.attribute("valeur");
            QString unit = detailElement.attribute("unite");
            double value = valueString.toDouble();
            dosage->getQuantity()->setValue(value);
            dosage->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit(unit));

            //TODO (JRP) : interval and duration should be deducted from the XML
            dosage->setInterval(Tucuxi::Gui::Core::Duration(8));
            dosage->setTinf(Tucuxi::Gui::Core::Duration(0,180));

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
    if (activeSubstanceStr == "vanco fulldata") {
        treatment->setActiveSubstanceId("vancomycin");
    } else if (activeSubstanceStr == "cefepime fulldata") {
        treatment->setActiveSubstanceId("cefepime");
    }

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
