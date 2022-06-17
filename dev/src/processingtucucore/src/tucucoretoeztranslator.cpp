//@@license@@

#include "tucucoretoeztranslator.h"

#include "core/dal/adjustment.h"
#include "core/dal/drug/target.h"
#include "core/dal/dosage.h"
#include "core/dal/drug/standardtreatment.h"

#include "tucucore/targetevaluationresult.h"
#include "tucucore/dosage.h"
#include "tucucore/drugmodelimport.h"
#include "tucucore/drugmodel/parameterdefinition.h"
#include "tucucore/drugmodel/formulationandroute.h"

using namespace Tucuxi::Gui::Processing;

namespace Tucuxi {
namespace Gui {
namespace Processing {

TucucoreToEzTranslator::TucucoreToEzTranslator()
{

}

QDateTime TucucoreToEzTranslator::buildDateTime(const Tucuxi::Common::DateTime &date)
{
    QDateTime result(QDate(date.year(), date.month(), date.day()), QTime(date.hour(), date.minute(), date.second()));
    return result;
}


bool TucucoreToEzTranslator::buildTargetEvaluation(
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

QString TucucoreToEzTranslator::description(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute)
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

Tucuxi::Gui::Core::Admin::Route TucucoreToEzTranslator::translateFormulationAndRoute(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute)
{
    switch (_formulationAndRoute.getAbsorptionModel()) {
    case Tucuxi::Core::AbsorptionModel::Extravascular: return Tucuxi::Gui::Core::Admin::Route::EXTRA;
    case Tucuxi::Core::AbsorptionModel::ExtravascularLag: return Tucuxi::Gui::Core::Admin::Route::EXTRALAG;
    case Tucuxi::Core::AbsorptionModel::Infusion: return Tucuxi::Gui::Core::Admin::Route::INFUSION;
    case Tucuxi::Core::AbsorptionModel::Intravascular: return Tucuxi::Gui::Core::Admin::Route::BOLUS;
    case Tucuxi::Core::AbsorptionModel::Undefined: return Tucuxi::Gui::Core::Admin::Route::UNVALID;
    }
    return Tucuxi::Gui::Core::Admin::Route::UNVALID;
}


Tucuxi::Gui::Core::Admin::Route TucucoreToEzTranslator::translateAbsorptionModel(Tucuxi::Core::AbsorptionModel _absorptionModel)
{
    switch (_absorptionModel) {
    case Tucuxi::Core::AbsorptionModel::Extravascular: return Tucuxi::Gui::Core::Admin::Route::EXTRA;
    case Tucuxi::Core::AbsorptionModel::ExtravascularLag: return Tucuxi::Gui::Core::Admin::Route::EXTRALAG;
    case Tucuxi::Core::AbsorptionModel::Infusion: return Tucuxi::Gui::Core::Admin::Route::INFUSION;
    case Tucuxi::Core::AbsorptionModel::Intravascular: return Tucuxi::Gui::Core::Admin::Route::BOLUS;
    case Tucuxi::Core::AbsorptionModel::Undefined: return Tucuxi::Gui::Core::Admin::Route::UNVALID;
    }
    return Tucuxi::Gui::Core::Admin::Route::UNVALID;
}

bool TucucoreToEzTranslator::buildDosageHistory(const Tucuxi::Core::DosageHistory & dosageHistory,
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
                admin->setRoute(translateFormulationAndRoute(lasting->getLastFormulationAndRoute()));
                admin->setFormulationAndRoute(lasting->getLastFormulationAndRoute());
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
                admin->setRoute(translateFormulationAndRoute(lasting->getLastFormulationAndRoute()));
                admin->setFormulationAndRoute(lasting->getLastFormulationAndRoute());
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
#include "core/dal/amount.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/drugvariate.h"

#include "tucucore/drugmodel/drugmodel.h"
#include "tucucore/drugmodel/activesubstance.h"
#include "tucucommon/translatablestring.h"
#include "tucucore/drugmodel/covariatedefinition.h"

Tucuxi::Gui::Core::DrugModel* TucucoreToEzTranslator::buildLightDrugModel(const Tucuxi::Core::DrugModel *drugModel)
{
    // We do not support multiple active substances
    if (drugModel->getActiveMoieties().size() > 1) {
        return nullptr;
    }

    Tucuxi::Gui::Core::DrugModel *newModel = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugModel>(ABSTRACTREPO, nullptr);

    //
    // General fields
    //


    newModel->setDrugModelId(QString::fromStdString(drugModel->getDrugModelId()));

    newModel->setName(translate(drugModel->getMetadata().getDrugName(), newModel));

    newModel->setStudyAuthors(QString::fromStdString(drugModel->getMetadata().getAuthorName()));

    newModel->setDescription(QString::fromStdString(drugModel->getMetadata().getDescription().getString()));


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

    newModel->setActiveSubstance(activeSubstance);

    //
    // ADME
    //

    Tucuxi::Gui::Core::ADME *adme = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ADME>(ABSTRACTREPO, newModel);
    Tucuxi::Gui::Core::Admin *admin = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Admin>(ABSTRACTREPO, adme);
    admin->setRoute(translateAbsorptionModel(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute().getAbsorptionModel()));
    admin->setFormulationAndRoute(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute());
    admin->setDescription(description(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute()));
    admin->setFormulationAndRoute(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute());
    adme->setDefaultIntake(admin);

    Tucuxi::Gui::Core::AdminList *adminList = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::AdminList>(ABSTRACTREPO, adme);
    for(auto & formulation : drugModel->getFormulationAndRoutes()) {
        Tucuxi::Gui::Core::Admin *admin = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Admin>(ABSTRACTREPO, adminList);
        admin->setRoute(translateAbsorptionModel(formulation->getFormulationAndRoute().getAbsorptionModel()));
        admin->setFormulationAndRoute(formulation->getFormulationAndRoute());
        admin->setDescription(description(formulation->getFormulationAndRoute()));
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
            infusionsList.append(value);
        }

        validInfusions->setInfusionsList(infusionsList);
        amount->setUnit(Tucuxi::Gui::Core::Unit(uString));
        amount->setDbvalue(defaultFormulation->getValidInfusionTimes()->getDefaultValue());
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
            intervalsList.append(value);
        }

        validIntervals->setIntervalsList(intervalsList);
        amount->setUnit(Tucuxi::Gui::Core::Unit(uString));
        amount->setDbvalue(defaultFormulation->getValidIntervals()->getDefaultValue());
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

Tucuxi::Gui::Core::TargetMethod* TucucoreToEzTranslator::translate(Tucuxi::Core::TargetType targetType, Tucuxi::Gui::Core::Target *newTarget)
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

Tucuxi::Gui::Core::OperableAmount *TucucoreToEzTranslator::translate(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, Tucuxi::Gui::Core::Target *newTarget)
{
    Tucuxi::Gui::Core::OperableAmount *amount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(subTarget.getValue());
    QString uString = QString::fromStdString(target->getUnit().toString());
    amount->setUnit(Tucuxi::Gui::Core::Unit(uString));
    return amount;
}

Tucuxi::Gui::Core::OperableAmount *TucucoreToEzTranslator::translateMic(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Common::TucuUnit micUnit,
                                                                     Tucuxi::Common::TucuUnit newUnit, Tucuxi::Gui::Core::Target *newTarget)
{
    Tucuxi::Gui::Core::OperableAmount *amount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(Tucuxi::Common::UnitManager::convertToUnit(subTarget.getValue(), micUnit, newUnit));
    amount->setUnit(Tucuxi::Gui::Core::Unit(QString(newUnit.toString().c_str())));
    return amount;
}


Tucuxi::Gui::Core::OperableAmount *TucucoreToEzTranslator::translateTime(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, Tucuxi::Gui::Core::Target *newTarget)
{
    Tucuxi::Gui::Core::OperableAmount *amount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(subTarget.getValue());
    QString uString = QString::fromStdString(target->getTimeUnit().toString());
    amount->setUnit(Tucuxi::Gui::Core::Unit(uString));
    return amount;
}

Tucuxi::Gui::Core::TranslatableString* TucucoreToEzTranslator::translate(const Tucuxi::Common::TranslatableString &str, Tucuxi::Gui::Core::Entity *parent)
{
    Tucuxi::Gui::Core::TranslatableString *newString = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::TranslatableString>(ABSTRACTREPO, parent);
    // Now we only insert the english translation
    newString->insert("en", QString::fromStdString(str.getString("en")));
    return newString;
}

