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


#include "tucucoretoguitranslator.h"

#include "core/dal/adjustment.h"
#include "core/dal/drug/target.h"
#include "core/dal/dosage.h"
#include "core/dal/drug/standardtreatment.h"

#include "tucucore/targetevaluationresult.h"
#include "tucucore/dosage.h"
#include "tucucore/drugmodel/parameterdefinition.h"
#include "tucucore/drugmodel/formulationandroute.h"

using namespace Tucuxi::Gui::Processing;

namespace Tucuxi {
namespace Gui {
namespace Processing {

TucucoreToGuiTranslator::TucucoreToGuiTranslator()
{

}

QDateTime TucucoreToGuiTranslator::buildDateTime(const Tucuxi::Common::DateTime &date)
{
    QDateTime result(QDate(date.year(), date.month(), date.day()), QTime(date.hour(), date.minute(), date.second()));
    return result;
}


bool TucucoreToGuiTranslator::buildTargetEvaluation(
        const Tucuxi::Core::TargetEvaluationResult *target,
        Tucuxi::Gui::Core::TargetEvaluationResult *newTarget)
{
    Tucuxi::Gui::Core::TargetMethod *method = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::TargetMethod>(ABSTRACTREPO, newTarget);
    Tucuxi::Gui::Core::TargetMethod::TargetType newTargetType;
    switch (target->getTargetType()) {
    case Tucuxi::Core::TargetType::Residual : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::ResidualTarget; break;
    case Tucuxi::Core::TargetType::Peak : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::PeakTarget; break;
    case Tucuxi::Core::TargetType::Mean : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::MeanTarget; break;
    case Tucuxi::Core::TargetType::Auc : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::AUCTarget; break;
    case Tucuxi::Core::TargetType::Auc24 : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::AUC24Target; break;
    case Tucuxi::Core::TargetType::CumulativeAuc : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::CumulativeAUCTarget; break;
    case Tucuxi::Core::TargetType::UnknownTarget : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::UnknownTarget; break;
    case Tucuxi::Core::TargetType::FractionTimeOverMic: newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::FractionTimeOverMicTarget; break;

        // The 4 following target types are not yet supported
    case Tucuxi::Core::TargetType::AucOverMic : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::AUCOverMicTarget; break;
    case Tucuxi::Core::TargetType::Auc24OverMic : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::AUC24OverMicTarget; break;
    case Tucuxi::Core::TargetType::TimeOverMic : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::TimeOverMicTarget; break;
    case Tucuxi::Core::TargetType::AucDividedByMic : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::AUCDividedByMicTarget; break;
    case Tucuxi::Core::TargetType::Auc24DividedByMic : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::AUC24DividedByMicTarget; break;
    case Tucuxi::Core::TargetType::PeakDividedByMic : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::PeakDividedByMicTarget; break;
    case Tucuxi::Core::TargetType::ResidualDividedByMic : newTargetType = Tucuxi::Gui::Core::TargetMethod::TargetType::ResidualDividedByMicTarget; break;
    }

    method->setTargetType(newTargetType);
    newTarget->setTargetType(method);
    newTarget->setScore(target->getScore());
    newTarget->setValue(target->getValue());
    newTarget->setUnit(QString::fromStdString(target->getUnit().toString()));

    return true;
}

QString TucucoreToGuiTranslator::description(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute)
{
    QString result;
    switch(_formulationAndRoute.getAdministrationRoute()) {
    case Tucuxi::Core::AdministrationRoute::Intramuscular : result = "Intramuscular"; break;
    case Tucuxi::Core::AdministrationRoute::IntravenousBolus : result = "Intravenous bolus"; break;
    case Tucuxi::Core::AdministrationRoute::IntravenousDrip : result = "Intravenous drip"; break;
    case Tucuxi::Core::AdministrationRoute::Nasal : result = "Nasal"; break;
    case Tucuxi::Core::AdministrationRoute::Oral : result = "Oral"; break;
    case Tucuxi::Core::AdministrationRoute::Rectal : result = "Rectal"; break;
    case Tucuxi::Core::AdministrationRoute::Subcutaneous : result = "Subcutaneous"; break;
    case Tucuxi::Core::AdministrationRoute::Sublingual : result = "Sublingual"; break;
    case Tucuxi::Core::AdministrationRoute::Transdermal : result = "Transdermal"; break;
    default: result = "Undefined"; break;
    }
    return result;
}

}
}
}
/*
Tucuxi::Gui::Core::DMAdmin::Route TucucoreToGuiTranslator::translateFormulationAndRoute(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute)
{
    switch (_formulationAndRoute.getAbsorptionModel()) {
    case Tucuxi::Core::AbsorptionModel::Extravascular: return Tucuxi::Gui::Core::DMAdmin::Route::EXTRA;
    case Tucuxi::Core::AbsorptionModel::ExtravascularLag: return Tucuxi::Gui::Core::DMAdmin::Route::EXTRALAG;
    case Tucuxi::Core::AbsorptionModel::Infusion: return Tucuxi::Gui::Core::DMAdmin::Route::INFUSION;
    case Tucuxi::Core::AbsorptionModel::Intravascular: return Tucuxi::Gui::Core::DMAdmin::Route::BOLUS;
    case Tucuxi::Core::AbsorptionModel::Undefined: return Tucuxi::Gui::Core::DMAdmin::Route::UNVALID;
    }
    return Tucuxi::Gui::Core::DMAdmin::Route::UNVALID;
}
*/

Tucuxi::Gui::Core::DMAdmin::Route TucucoreToGuiTranslator::translateAbsorptionModel(Tucuxi::Core::AbsorptionModel _absorptionModel)
{
    switch (_absorptionModel) {
    case Tucuxi::Core::AbsorptionModel::Extravascular: return Tucuxi::Gui::Core::DMAdmin::Route::EXTRA;
    case Tucuxi::Core::AbsorptionModel::ExtravascularLag: return Tucuxi::Gui::Core::DMAdmin::Route::EXTRALAG;
    case Tucuxi::Core::AbsorptionModel::Infusion: return Tucuxi::Gui::Core::DMAdmin::Route::INFUSION;
    case Tucuxi::Core::AbsorptionModel::Intravascular: return Tucuxi::Gui::Core::DMAdmin::Route::BOLUS;
    case Tucuxi::Core::AbsorptionModel::Undefined: return Tucuxi::Gui::Core::DMAdmin::Route::UNVALID;
    }
    return Tucuxi::Gui::Core::DMAdmin::Route::UNVALID;
}

bool TucucoreToGuiTranslator::buildDosageHistory(const Tucuxi::Core::DosageHistory & dosageHistory,
                                                  Tucuxi::Gui::Core::DosageHistory *newHistory)
{

    for (const auto & dosageRange : dosageHistory.getDosageTimeRanges()) {
        Tucuxi::Gui::Core::Dosage *dosage = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Dosage>(ABSTRACTREPO, newHistory);
        dosage->setApplied(buildDateTime(dosageRange->getStartDate()));
        dosage->setEndTime(buildDateTime(dosageRange->getEndDate()));
        const Tucuxi::Core::Dosage * d = dosageRange->getDosage();

        {
            const Tucuxi::Core::DosageRepeat *r = dynamic_cast<const Tucuxi::Core::DosageRepeat*>(d);
            if (r) {
                const Tucuxi::Core::LastingDose *lasting = dynamic_cast<const Tucuxi::Core::LastingDose*>(r->getDosage());

                if (lasting == nullptr) {
                    std::cout << "ouch" << std::endl;
                }
                dosage->setDbinterval(lasting->getTimeStep().toHours());
                dosage->setDbtinf(lasting->getInfusionTime().toMinutes());
                dosage->getQuantity()->setValue(lasting->getDose() );
                Tucuxi::Gui::Core::Admin *admin = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Admin>(ABSTRACTREPO, dosage);
                //admin->setRoute(translateFormulationAndRoute(lasting->getLastFormulationAndRoute()));
                Tucuxi::Core::FormulationAndRoute dmf(
                    lasting->getLastFormulationAndRoute().getFormulation(),
                    lasting->getLastFormulationAndRoute().getAdministrationRoute(),
                    lasting->getLastFormulationAndRoute().getAdministrationName());
                admin->setFormulationAndRoute(dmf);
                admin->setDescription(description(lasting->getLastFormulationAndRoute()));
                dosage->setRoute(admin);
                // TODO : Deal with units

                dosage->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit("mg"));

                // Adjustments are not at steady state
                dosage->setIsAtSteadyState(false);

                newHistory->append(dosage);
            }
        }
        {
            const Tucuxi::Core::DosageLoop *r = dynamic_cast<const Tucuxi::Core::DosageLoop*>(d);
            if (r) {
                const Tucuxi::Core::SingleDose *lasting = dynamic_cast<const Tucuxi::Core::SingleDose*>(r->getDosage());

                if (lasting == nullptr) {
                    std::cout << "ouch" << std::endl;
                }
                dosage->setDbinterval(lasting->getTimeStep().toHours());
                dosage->setDbtinf(lasting->getInfusionTime().toMinutes());
                dosage->getQuantity()->setValue(lasting->getDose() );
                Tucuxi::Gui::Core::Admin *admin = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Admin>(ABSTRACTREPO, dosage);
                //admin->setRoute(translateFormulationAndRoute(lasting->getLastFormulationAndRoute()));
                Tucuxi::Core::FormulationAndRoute dmf(
                    lasting->getLastFormulationAndRoute().getFormulation(),
                    lasting->getLastFormulationAndRoute().getAdministrationRoute(),
                    lasting->getLastFormulationAndRoute().getAdministrationName());
                admin->setFormulationAndRoute(dmf);
                admin->setDescription(description(lasting->getLastFormulationAndRoute()));
                dosage->setRoute(admin);
                // TODO : Deal with units
                dosage->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit("mg"));

                // Adjustments are not at steady state
                dosage->setIsAtSteadyState(false);

                newHistory->append(dosage);
            }
        }
    }
    return true;
}


#include "core/dal/drug/drug.h"
#include "core/dal/drug/translatablestring.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/drugvariate.h"

#include "tucucore/drugmodel/drugmodel.h"
#include "tucucommon/translatablestring.h"
#include "tucucore/drugmodel/covariatedefinition.h"

Tucuxi::Gui::Core::DrugModel* TucucoreToGuiTranslator::buildLightDrugModel(const Tucuxi::Core::DrugModel *drugModel,
                                                                          const QString filePath)
{
    // We do not support multiple active substances
    if (drugModel->getActiveMoieties().size() > 1) {
        return nullptr;
    }

    Tucuxi::Gui::Core::DrugModel *newModel = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugModel>(ABSTRACTREPO, nullptr);

    //
    // General fields
    //

    newModel->setFilePath(filePath);

    newModel->setDrugModelId(QString::fromStdString(drugModel->getDrugModelId()));

    newModel->setName(translate(drugModel->getMetadata().getDrugName(), newModel));

    newModel->setStudyAuthors(QString::fromStdString(drugModel->getMetadata().getAuthorName()));

    newModel->setDescription(QString::fromStdString(drugModel->getMetadata().getDescription().getString()));

    newModel->setDefaultUnit(QString::fromStdString(drugModel->getActiveMoieties()[0]->getUnit().toString()));


    //
    // Active substance
    //

    // For the active substance we only need its Id and its name in english

    Tucuxi::Gui::Core::ActiveSubstance *activeSubstance =  Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ActiveSubstance>(ABSTRACTREPO, newModel);

    const Tucuxi::Core::ActiveMoiety *activeMoiety = drugModel->getActiveMoieties().at(0).get();
    activeSubstance->setSubstanceId(QString::fromStdString(activeMoiety->getActiveMoietyId().toString()));

    Tucuxi::Gui::Core::TranslatableString *name = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::TranslatableString>(ABSTRACTREPO, activeSubstance);
    // We only support english now
    name->insert("en", QString::fromStdString(activeMoiety->getActiveMoietyName().getString()));
    activeSubstance->setName(name);

    {
        QStringList atcs;
        for(auto atc : drugModel->getMetadata().getAtcs()) {
            atcs << QString::fromStdString(atc);
        }
        activeSubstance->setAtc(atcs);
    }


    // Create analyte list and populate it
    auto analyteList = Core::CoreFactory::createEntity<Tucuxi::Gui::Core::AnalyteList>(ABSTRACTREPO);
    for (const auto& analytePtr : activeMoiety->getAnalyteIds()) {
        auto analyte = Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Analyte>(ABSTRACTREPO);
        analyte->setAnalyteId(QString::fromStdString(analytePtr.toString()));
        analyteList->append(analyte);
    }
    activeSubstance->setAnalytes(analyteList);

    newModel->setActiveSubstance(activeSubstance);

    //
    // ADME
    //

    Tucuxi::Gui::Core::ADME *adme = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ADME>(ABSTRACTREPO, newModel);
    Tucuxi::Gui::Core::DMAdmin *admin = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DMAdmin>(ABSTRACTREPO, adme);
    admin->setRoute(translateAbsorptionModel(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute().getAbsorptionModel()));
    admin->setFormulationAndRoute(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute());
    admin->setDescription(description(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute().getTreatmentFormulationAndRoute()));
    admin->setFormulationAndRoute(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute());
    adme->setDefaultIntake(admin);

    Tucuxi::Gui::Core::DMAdminList *adminList = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DMAdminList>(ABSTRACTREPO, adme);
    for(auto & formulation : drugModel->getFormulationAndRoutes()) {
        Tucuxi::Gui::Core::DMAdmin *admin = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DMAdmin>(ABSTRACTREPO, adminList);
        admin->setRoute(translateAbsorptionModel(formulation->getFormulationAndRoute().getAbsorptionModel()));
        admin->setFormulationAndRoute(formulation->getFormulationAndRoute());
        auto f = formulation->getFormulationAndRoute();
        Tucuxi::Core::DMFormulationAndRoute dmf(
            f.getFormulation(),
            f.getAdministrationRoute(),
            f.getAbsorptionModel(),
            f.getAdministrationName());
        admin->setFormulationAndRoute(dmf);

        admin->setDescription(description(formulation->getFormulationAndRoute().getTreatmentFormulationAndRoute()));
        admin->setFormulationAndRoute(formulation->getFormulationAndRoute());
        adminList->append(admin);
    }
    adme->setIntakes(adminList);

    adme->setDistribution(QString::fromStdString(drugModel->getMetadata().getDistribution().getString()));
    adme->setElimination(QString::fromStdString(drugModel->getMetadata().getElimination().getString()));

    newModel->setAdme(adme);

    {
        Tucuxi::Gui::Core::TranslatableString *studyName = translate(drugModel->getMetadata().getStudyName(), newModel);
        newModel->setStudyNameTranslationMap(studyName);
    }

    {
        Tucuxi::Gui::Core::TranslatableString *description = translate(drugModel->getMetadata().getDescription(), newModel);
        newModel->setDescriptionsTranslationMap(description);
    }


    {
        Tucuxi::Gui::Core::TranslatableString *domainName = translate(drugModel->getDomain().getDescription(), newModel);
        newModel->setDomainNameTranslationMap(domainName);
    }



    auto defaultFormulation = drugModel->getFormulationAndRoutes().getDefault();
    {
        // All doses values found on drug model
        Tucuxi::Gui::Core::ValidDoses *validDoses = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ValidDoses>(ABSTRACTREPO, newModel);
        Tucuxi::Gui::Core::IdentifiableAmount *amount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO, validDoses);

        QString uString = QString::fromStdString(defaultFormulation->getValidDoses()->getUnit().toString());
        QList<double> doseList;
        for(const auto &doseValue : defaultFormulation->getValidDoses()->getValues()){
            doseList.append(doseValue);
        }

        validDoses->setDosesList(doseList);
        amount->setUnit(Tucuxi::Gui::Core::Unit(uString));
        amount->setDbvalue(defaultFormulation->getValidDoses()->getDefaultValue());
        validDoses->setQuantity(amount);

        newModel->setDoses(validDoses);
    }

    if (defaultFormulation->getValidInfusionTimes() != nullptr)
    {
        //All infusions values found on drug model
        Tucuxi::Gui::Core::ValidInfusions *validInfusions = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ValidInfusions>(ABSTRACTREPO, newModel);
        Tucuxi::Gui::Core::IdentifiableAmount *amount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO, validInfusions);

        QString uString = QString::fromStdString(defaultFormulation->getValidInfusionTimes()->getUnit().toString());
        QList<double> infusionsList;
        for(const auto &value : defaultFormulation->getValidInfusionTimes()->getValues()){
            infusionsList.append(Common::UnitManager::convertToUnit(value, TucuUnit(uString.toStdString()), TucuUnit("min")));
        }

        validInfusions->setInfusionsList(infusionsList);
        amount->setUnit(Tucuxi::Gui::Core::Unit("min"));
        amount->setDbvalue(Common::UnitManager::convertToUnit(defaultFormulation->getValidInfusionTimes()->getDefaultValue(), TucuUnit(uString.toStdString()), TucuUnit("min")));
        //amount->setUnit(Tucuxi::Gui::Core::Unit(uString));
        //amount->setDbvalue(defaultFormulation->getValidInfusionTimes()->getDefaultValue());
        validInfusions->setQuantity(amount);

        newModel->setInfusions(validInfusions);

    }

    {
        // All intervals values found on drug model
        Tucuxi::Gui::Core::ValidIntervals *validIntervals = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ValidIntervals>(ABSTRACTREPO, newModel);
        Tucuxi::Gui::Core::IdentifiableAmount *amount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO, validIntervals);

        QString uString = QString::fromStdString(defaultFormulation->getValidIntervals()->getUnit().toString());
        QList<double> intervalsList;
        for(const auto &value : defaultFormulation->getValidIntervals()->getValues()){
            intervalsList.append(Common::UnitManager::convertToUnit(value, TucuUnit(uString.toStdString()), TucuUnit("h")));
        }

        validIntervals->setIntervalsList(intervalsList);
        amount->setUnit(Tucuxi::Gui::Core::Unit("h"));
        amount->setDbvalue(Common::UnitManager::convertToUnit(defaultFormulation->getValidIntervals()->getDefaultValue(), TucuUnit(uString.toStdString()), TucuUnit("h")));
        //amount->setUnit(Tucuxi::Gui::Core::Unit(uString));
        //amount->setDbvalue(defaultFormulation->getValidIntervals()->getDefaultValue());
        validIntervals->setQuantity(amount);

        newModel->setIntervals(validIntervals);
    }

    Tucuxi::Gui::Core::DrugVariateList *variateList = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugVariateList>(ABSTRACTREPO, newModel);
    for (const auto &covariate : drugModel->getCovariates()) {
        // The GUI do not need to know about computed covariates
        if (covariate.get()->isComputed()) {
            continue;
        }
        // Do not export dose covariates to the GUI
        if (covariate->getType() == Tucuxi::Core::CovariateType::Dose) {
            continue;
        }
        Tucuxi::Gui::Core::DrugVariate *variate = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugVariate>(ABSTRACTREPO, variateList);
        variate->setCovariateId(QString::fromStdString(covariate->getId()));
        variate->setVisualNameTranslation(translate(covariate->getName(), variate));
        Tucuxi::Gui::Core::OperableAmount *amount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperableAmount>(ABSTRACTREPO, variate);
        amount->setDbvalue(covariate->getValue());
        QString uString = QString::fromStdString(covariate->getUnit().toString());
        amount->setUnit(Tucuxi::Gui::Core::Unit(uString));
        variate->setQuantity(amount);

        Tucuxi::Gui::Core::CovariateType cType = Tucuxi::Gui::Core::CovariateType::Standard;
        switch (covariate->getType()) {
        case Tucuxi::Core::CovariateType::Standard: cType = Tucuxi::Gui::Core::CovariateType::Standard; break;
        case Tucuxi::Core::CovariateType::AgeInYears: cType = Tucuxi::Gui::Core::CovariateType::AgeInYears; break;
        case Tucuxi::Core::CovariateType::AgeInDays: cType = Tucuxi::Gui::Core::CovariateType::AgeInDays; break;
        case Tucuxi::Core::CovariateType::AgeInWeeks: cType = Tucuxi::Gui::Core::CovariateType::AgeInWeeks; break;
        case Tucuxi::Core::CovariateType::AgeInMonths: cType = Tucuxi::Gui::Core::CovariateType::AgeInMonths; break;
        case Tucuxi::Core::CovariateType::Sex: cType = Tucuxi::Gui::Core::CovariateType::Sex; break;
        case Tucuxi::Core::CovariateType::Dose: cType = Tucuxi::Gui::Core::CovariateType::Dose; break;
        case Tucuxi::Core::CovariateType::TimeFromStartInDays: cType = Tucuxi::Gui::Core::CovariateType::TimeFromStartInDays; break;
        case Tucuxi::Core::CovariateType::TimeFromStartInWeeks: cType = Tucuxi::Gui::Core::CovariateType::TimeFromStartInWeeks; break;
        case Tucuxi::Core::CovariateType::TimeFromStartInMonths: cType = Tucuxi::Gui::Core::CovariateType::TimeFromStartInMonths; break;
        case Tucuxi::Core::CovariateType::TimeFromStartInYears: cType = Tucuxi::Gui::Core::CovariateType::TimeFromStartInYears; break;
        case Tucuxi::Core::CovariateType::TimeFromStartInHours: cType = Tucuxi::Gui::Core::CovariateType::TimeFromStartInHours; break;
        }
        variate->setCovariateType(cType);

        QMetaType::Type newType;
        switch (covariate->getDataType()) {
        case Tucuxi::Core::DataType::Bool : newType = QMetaType::Type::Bool; break;
        case Tucuxi::Core::DataType::Int : newType = QMetaType::Type::Int; break;
        case Tucuxi::Core::DataType::Double : newType = QMetaType::Type::Double; break;
        case Tucuxi::Core::DataType::Date : newType = QMetaType::Type::QDateTime; break; // This is unsupported
        }
        if ((covariate->getType() != Tucuxi::Core::CovariateType::Standard)
                && (covariate->getType() != Tucuxi::Core::CovariateType::Sex)) {
            variate->setAutomatic(true);
        }

        variate->setType(newType);
        variateList->append(variate);
    }
    newModel->setCovariates(variateList);


    Tucuxi::Gui::Core::TargetList *targetList = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::TargetList>(ABSTRACTREPO, newModel);
    for(const auto &target : drugModel->getActiveMoieties().at(0)->getTargetDefinitions())
    {
        Tucuxi::Gui::Core::Target *newTarget = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Target>(ABSTRACTREPO, targetList);
        newTarget->setCmin(translate(target->getCMin(), target.get(), newTarget));
        newTarget->setCmax(translate(target->getCMax(), target.get(), newTarget));
        newTarget->setCbest(translate(target->getCBest(), target.get(), newTarget));
        newTarget->setTmin(translateTime(target->getTMin(), target.get(), newTarget));
        newTarget->setTmax(translateTime(target->getTMax(), target.get(), newTarget));
        newTarget->setTbest(translateTime(target->getTBest(), target.get(), newTarget));
        newTarget->setMic(translateMic(target->getMic(), target->getMicUnit(), target->getMicUnit(), newTarget));
        newTarget->setType(translate(target->getTargetType(), newTarget));
        targetList->append(newTarget);
    }
    newModel->setTargets(targetList);

    {
        Tucuxi::Gui::Core::ParameterSet *parameterSet = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ParameterSet>(ABSTRACTREPO, newModel);
        // TODO : Be careful, only valid for a single analyteSet
        const Tucuxi::Core::AnalyteGroupId analyteGroupId = drugModel->getAnalyteSets()[0]->getId();
        const Tucuxi::Core::Formulation formulation = drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute().getFormulation();
        const Tucuxi::Core::AdministrationRoute route = drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute().getAdministrationRoute();

        Tucuxi::Core::ParameterDefinitionIterator it = drugModel->getParameterDefinitions(analyteGroupId, formulation, route);

        it.reset();
        while (!it.isDone()) {
            Tucuxi::Gui::Core::Parameter *newParameter = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Parameter>(ABSTRACTREPO, parameterSet);
            newParameter->setName(QString::fromStdString((*it)->getId()));
            Tucuxi::Gui::Core::OperableAmount *amount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperableAmount>(ABSTRACTREPO, newParameter);
            amount->setDbvalue((*it)->getValue());
            amount->setUnit(Tucuxi::Gui::Core::Unit(QString::fromStdString((*it)->getUnit().toString())));
            newParameter->setQuantity(amount);
            parameterSet->getParameters()->append(newParameter);
            it.next();
        }

        newModel->setParameters(parameterSet);
    }

    {

        const Tucuxi::Core::StandardTreatment *oldTreatment = drugModel->getFormulationAndRoutes().getDefault()->getStandardTreatment();
        if (oldTreatment != nullptr) {
            Tucuxi::Gui::Core::StandardTreatment *standardTreatment = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::StandardTreatment>(ABSTRACTREPO, newModel);
            standardTreatment->setIsFixedDuration(oldTreatment->getIsFixedDuration());
            if (oldTreatment->getIsFixedDuration()) {
                Tucuxi::Gui::Core::Duration duration;
                if (oldTreatment->getUnit().toString() == "d") {
                    duration.addHours(24 * oldTreatment->getDuration());
                }
                else if (oldTreatment->getUnit().toString() == "h") {
                    duration.addHours(oldTreatment->getDuration());
                }
                else if (oldTreatment->getUnit().toString() == "m") {
                    duration.addMinutes(oldTreatment->getDuration());
                }
                else {
                    // Should not happen. To be checked
                }
                standardTreatment->setDuration(duration);
            }
            newModel->setStandardTreatment(standardTreatment);
        }
    }

    return newModel;
}

Tucuxi::Gui::Core::TargetMethod* TucucoreToGuiTranslator::translate(Tucuxi::Core::TargetType targetType, Tucuxi::Gui::Core::Target *newTarget)
{
    Tucuxi::Gui::Core::TargetMethod *method = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::TargetMethod>(ABSTRACTREPO, newTarget);
    switch (targetType) {
    case Tucuxi::Core::TargetType::Auc : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::AUCTarget); break;
    case Tucuxi::Core::TargetType::Auc24 : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::AUC24Target); break;
    case Tucuxi::Core::TargetType::AucDividedByMic : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::AUCDividedByMicTarget); break;
    case Tucuxi::Core::TargetType::Auc24DividedByMic : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::AUC24DividedByMicTarget); break;
    case Tucuxi::Core::TargetType::AucOverMic : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::AUCOverMicTarget); break;
    case Tucuxi::Core::TargetType::Auc24OverMic : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::AUC24OverMicTarget); break;
    case Tucuxi::Core::TargetType::CumulativeAuc : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::CumulativeAUCTarget); break;
    case Tucuxi::Core::TargetType::Mean : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::MeanTarget); break;
    case Tucuxi::Core::TargetType::Peak : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::PeakTarget); break;
    case Tucuxi::Core::TargetType::PeakDividedByMic : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::PeakDividedByMicTarget); break;
    case Tucuxi::Core::TargetType::ResidualDividedByMic : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::ResidualDividedByMicTarget); break;
    case Tucuxi::Core::TargetType::Residual : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::ResidualTarget); break;
    case Tucuxi::Core::TargetType::TimeOverMic : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::TimeOverMicTarget); break;
    case Tucuxi::Core::TargetType::FractionTimeOverMic : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::FractionTimeOverMicTarget); break;
    case Tucuxi::Core::TargetType::UnknownTarget : method->setTargetType(Tucuxi::Gui::Core::TargetMethod::TargetType::UnknownTarget); break;
    }
    return method;
}

Tucuxi::Gui::Core::OperableAmount *TucucoreToGuiTranslator::translate(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, Tucuxi::Gui::Core::Target *newTarget)
{
    Tucuxi::Gui::Core::OperableAmount *amount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(subTarget.getValue());
    QString uString = QString::fromStdString(target->getUnit().toString());
    amount->setUnit(Tucuxi::Gui::Core::Unit(uString));
    return amount;
}

Tucuxi::Gui::Core::OperableAmount *TucucoreToGuiTranslator::translateMic(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Common::TucuUnit micUnit,
                                                                     Tucuxi::Common::TucuUnit newUnit, Tucuxi::Gui::Core::Target *newTarget)
{
    Tucuxi::Gui::Core::OperableAmount *amount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(Tucuxi::Common::UnitManager::convertToUnit(subTarget.getValue(), micUnit, newUnit));
    amount->setUnit(Tucuxi::Gui::Core::Unit(QString(newUnit.toString().c_str())));
    return amount;
}


Tucuxi::Gui::Core::OperableAmount *TucucoreToGuiTranslator::translateTime(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, Tucuxi::Gui::Core::Target *newTarget)
{
    Tucuxi::Gui::Core::OperableAmount *amount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(subTarget.getValue());
    QString uString = QString::fromStdString(target->getTimeUnit().toString());
    amount->setUnit(Tucuxi::Gui::Core::Unit(uString));
    return amount;
}

Tucuxi::Gui::Core::TranslatableString* TucucoreToGuiTranslator::translate(const Tucuxi::Common::TranslatableString &str, Tucuxi::Gui::Core::Entity *parent)
{
    Tucuxi::Gui::Core::TranslatableString *newString = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::TranslatableString>(ABSTRACTREPO, parent);
    // Now we only insert the english translation
    newString->insert("en", QString::fromStdString(str.getString("en")));
    return newString;
}

