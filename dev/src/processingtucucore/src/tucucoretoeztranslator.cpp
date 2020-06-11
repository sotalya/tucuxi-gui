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
        ezechiel::core::TargetEvaluationResult *newTarget)
{
    ezechiel::core::TargetMethod *method = ezechiel::core::CoreFactory::createEntity<ezechiel::core::TargetMethod>(ABSTRACTREPO, newTarget);
    ezechiel::core::TargetMethod::TargetType newTargetType;
    switch (target->getTargetType()) {
    case Tucuxi::Core::TargetType::Residual : newTargetType = ezechiel::core::TargetMethod::TargetType::ResidualTarget; break;
    case Tucuxi::Core::TargetType::Peak : newTargetType = ezechiel::core::TargetMethod::TargetType::PeakTarget; break;
    case Tucuxi::Core::TargetType::Mean : newTargetType = ezechiel::core::TargetMethod::TargetType::MeanTarget; break;
    case Tucuxi::Core::TargetType::Auc : newTargetType = ezechiel::core::TargetMethod::TargetType::AUCTarget; break;
    case Tucuxi::Core::TargetType::Auc24 : newTargetType = ezechiel::core::TargetMethod::TargetType::AUC24Target; break;
    case Tucuxi::Core::TargetType::CumulativeAuc : newTargetType = ezechiel::core::TargetMethod::TargetType::CumulativeAUCTarget; break;
    case Tucuxi::Core::TargetType::UnknownTarget : newTargetType = ezechiel::core::TargetMethod::TargetType::UnknownTarget; break;

        // The 4 following target types are not yet supported
    case Tucuxi::Core::TargetType::AucOverMic : newTargetType = ezechiel::core::TargetMethod::TargetType::AUCOverMicTarget; break;
    case Tucuxi::Core::TargetType::Auc24OverMic : newTargetType = ezechiel::core::TargetMethod::TargetType::AUC24OverMicTarget; break;
    case Tucuxi::Core::TargetType::TimeOverMic : newTargetType = ezechiel::core::TargetMethod::TargetType::TimeOverMicTarget; break;
    case Tucuxi::Core::TargetType::AucDividedByMic : newTargetType = ezechiel::core::TargetMethod::TargetType::AUCDividedByMicTarget; break;
    case Tucuxi::Core::TargetType::Auc24DividedByMic : newTargetType = ezechiel::core::TargetMethod::TargetType::AUC24DividedByMicTarget; break;
    case Tucuxi::Core::TargetType::PeakDividedByMic : newTargetType = ezechiel::core::TargetMethod::TargetType::PeakDividedByMicTarget; break;
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

ezechiel::core::Admin::Route TucucoreToEzTranslator::translateFormulationAndRoute(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute)
{
    switch (_formulationAndRoute.getAbsorptionModel()) {
    case Tucuxi::Core::AbsorptionModel::Extravascular: return ezechiel::core::Admin::Route::EXTRA;
    case Tucuxi::Core::AbsorptionModel::ExtravascularLag: return ezechiel::core::Admin::Route::EXTRALAG;
    case Tucuxi::Core::AbsorptionModel::Infusion: return ezechiel::core::Admin::Route::INFUSION;
    case Tucuxi::Core::AbsorptionModel::Intravascular: return ezechiel::core::Admin::Route::BOLUS;
    case Tucuxi::Core::AbsorptionModel::Undefined: return ezechiel::core::Admin::Route::UNVALID;
    }
    return ezechiel::core::Admin::Route::UNVALID;
}


ezechiel::core::Admin::Route TucucoreToEzTranslator::translateAbsorptionModel(Tucuxi::Core::AbsorptionModel _absorptionModel)
{
    switch (_absorptionModel) {
    case Tucuxi::Core::AbsorptionModel::Extravascular: return ezechiel::core::Admin::Route::EXTRA;
    case Tucuxi::Core::AbsorptionModel::ExtravascularLag: return ezechiel::core::Admin::Route::EXTRALAG;
    case Tucuxi::Core::AbsorptionModel::Infusion: return ezechiel::core::Admin::Route::INFUSION;
    case Tucuxi::Core::AbsorptionModel::Intravascular: return ezechiel::core::Admin::Route::BOLUS;
    case Tucuxi::Core::AbsorptionModel::Undefined: return ezechiel::core::Admin::Route::UNVALID;
    }
    return ezechiel::core::Admin::Route::UNVALID;
}

bool TucucoreToEzTranslator::buildDosageHistory(const Tucuxi::Core::DosageHistory & dosageHistory,
                                                  ezechiel::core::DosageHistory *newHistory)
{

    for (const auto & dosageRange : dosageHistory.getDosageTimeRanges()) {
        ezechiel::core::Dosage *dosage = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Dosage>(ABSTRACTREPO, newHistory);
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
                ezechiel::core::Admin *admin = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Admin>(ABSTRACTREPO, dosage);
                admin->setRoute(translateFormulationAndRoute(lasting->getLastFormulationAndRoute()));
                admin->setDescription(description(lasting->getLastFormulationAndRoute()));
                dosage->setRoute(admin);
                // TODO : Deal with units

                dosage->getQuantity()->setUnit(ezechiel::core::Unit("mg"));

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
                ezechiel::core::Admin *admin = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Admin>(ABSTRACTREPO, dosage);
                admin->setRoute(translateFormulationAndRoute(lasting->getLastFormulationAndRoute()));
                admin->setDescription(description(lasting->getLastFormulationAndRoute()));
                dosage->setRoute(admin);
                // TODO : Deal with units
                dosage->getQuantity()->setUnit(ezechiel::core::Unit("mg"));

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

ezechiel::core::DrugModel* TucucoreToEzTranslator::buildLightDrugModel(const Tucuxi::Core::DrugModel *drugModel)
{
    // We do not support multiple active substances
    if (drugModel->getActiveMoieties().size() > 1) {
        return nullptr;
    }

    ezechiel::core::DrugModel *newModel = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugModel>(ABSTRACTREPO, nullptr);

    //
    // General fields
    //

    newModel->setDrugModelId(QString::fromStdString(drugModel->getDrugModelId()));

    newModel->setName(translate(drugModel->getMetadata().getDrugName(), newModel));

    newModel->setStudyAuthors(QString::fromStdString(drugModel->getMetadata().getAuthorName()));


    //
    // Active substance
    //

    // For the active substance we only need its Id and its name in english

    ezechiel::core::ActiveSubstance *activeSubstance =  ezechiel::core::CoreFactory::createEntity<ezechiel::core::ActiveSubstance>(ABSTRACTREPO, newModel);

    const Tucuxi::Core::ActiveMoiety *activeMoiety = drugModel->getActiveMoieties().at(0).get();
    activeSubstance->setSubstanceId(QString::fromStdString(activeMoiety->getActiveMoietyId().toString()));

    ezechiel::core::TranslatableString *name = ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(ABSTRACTREPO, activeSubstance);
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

    ezechiel::core::ADME *adme = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ADME>(ABSTRACTREPO, newModel);
    ezechiel::core::Admin *admin = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Admin>(ABSTRACTREPO, adme);
    admin->setRoute(translateAbsorptionModel(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute().getAbsorptionModel()));
    admin->setDescription(description(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute()));
    adme->setDefaultIntake(admin);

    ezechiel::core::AdminList *adminList = ezechiel::core::CoreFactory::createEntity<ezechiel::core::AdminList>(ABSTRACTREPO, adme);
    for(auto & formulation : drugModel->getFormulationAndRoutes()) {
        ezechiel::core::Admin *admin = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Admin>(ABSTRACTREPO, adminList);
        admin->setRoute(translateAbsorptionModel(formulation->getFormulationAndRoute().getAbsorptionModel()));
        admin->setDescription(description(formulation->getFormulationAndRoute()));
        adminList->append(admin);
    }
    adme->setIntakes(adminList);

    adme->setDistribution(QString::fromStdString(drugModel->getMetadata().getDistribution().getString()));
    adme->setElimination(QString::fromStdString(drugModel->getMetadata().getElimination().getString()));

    newModel->setAdme(adme);

    {
        ezechiel::core::TranslatableString *studyName = translate(drugModel->getMetadata().getStudyName(), newModel);
        newModel->setStudyNameTranslationMap(studyName);
    }

    {
        ezechiel::core::TranslatableString *description = translate(drugModel->getMetadata().getDescription(), newModel);
        newModel->setDescriptionsTranslationMap(description);
    }


    {
        ezechiel::core::TranslatableString *domainName = translate(drugModel->getDomain().getDescription(), newModel);
        newModel->setDomainNameTranslationMap(domainName);
    }



    auto defaultFormulation = drugModel->getFormulationAndRoutes().getDefault();
    {
        // Only the default dose
        ezechiel::core::ValidDoses *validDoses = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidDoses>(ABSTRACTREPO, newModel);
        ezechiel::core::IdentifiableAmount *amount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::IdentifiableAmount>(ABSTRACTREPO, validDoses);
        amount->setDbvalue(defaultFormulation->getValidDoses()->getDefaultValue());
        QString uString = QString::fromStdString(defaultFormulation->getValidDoses()->getUnit().toString());
        amount->setUnit(ezechiel::core::Unit(uString));
        validDoses->setQuantity(amount);
        newModel->setDoses(validDoses);
    }

    if (defaultFormulation->getValidInfusionTimes() != nullptr)
    {
        // Only the default infusion time
        ezechiel::core::ValidInfusions *validInfusions = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidInfusions>(ABSTRACTREPO, newModel);
        ezechiel::core::IdentifiableAmount *amount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::IdentifiableAmount>(ABSTRACTREPO, validInfusions);
        amount->setDbvalue(defaultFormulation->getValidInfusionTimes()->getDefaultValue());
        QString uString = QString::fromStdString(defaultFormulation->getValidInfusionTimes()->getUnit().toString());
        amount->setUnit(ezechiel::core::Unit(uString));
        validInfusions->setQuantity(amount);
        newModel->setInfusions(validInfusions);
    }

    {
        // Only the default interval
        ezechiel::core::ValidIntervals *validIntervals = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidIntervals>(ABSTRACTREPO, newModel);
        ezechiel::core::IdentifiableAmount *amount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::IdentifiableAmount>(ABSTRACTREPO, validIntervals);
        amount->setDbvalue(defaultFormulation->getValidIntervals()->getDefaultValue());
        QString uString = QString::fromStdString(defaultFormulation->getValidIntervals()->getUnit().toString());
        amount->setUnit(ezechiel::core::Unit(uString));
        validIntervals->setQuantity(amount);
        newModel->setIntervals(validIntervals);
    }

    ezechiel::core::DrugVariateList *variateList = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugVariateList>(ABSTRACTREPO, newModel);
    for (const auto &covariate : drugModel->getCovariates()) {
        // The GUI do not need to know about computed covariates
        if (covariate.get()->isComputed()) {
            continue;
        }
        ezechiel::core::DrugVariate *variate = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugVariate>(ABSTRACTREPO, variateList);
        variate->setCovariateId(QString::fromStdString(covariate->getId()));
        variate->setVisualNameTranslation(translate(covariate->getName(), variate));
        ezechiel::core::OperableAmount *amount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::OperableAmount>(ABSTRACTREPO, variate);
        amount->setDbvalue(covariate->getValue());
        QString uString = QString::fromStdString(covariate->getUnit().toString());
        amount->setUnit(ezechiel::core::Unit(uString));
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


    ezechiel::core::TargetList *targetList = ezechiel::core::CoreFactory::createEntity<ezechiel::core::TargetList>(ABSTRACTREPO, newModel);
    for(const auto &target : drugModel->getActiveMoieties().at(0)->getTargetDefinitions())
    {
        ezechiel::core::Target *newTarget = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Target>(ABSTRACTREPO, targetList);
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
        ezechiel::core::ParameterSet *parameterSet = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ParameterSet>(ABSTRACTREPO, newModel);
        // TODO : Be careful, only valid for a single analyteSet
        const Tucuxi::Core::AnalyteGroupId analyteGroupId = drugModel->getAnalyteSets()[0]->getId();
        const Tucuxi::Core::Formulation formulation = drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute().getFormulation();
        const Tucuxi::Core::AdministrationRoute route = drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute().getAdministrationRoute();

        Tucuxi::Core::ParameterDefinitionIterator it = drugModel->getParameterDefinitions(analyteGroupId, formulation, route);

        it.reset();
        while (!it.isDone()) {
            ezechiel::core::Parameter *newParameter = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Parameter>(ABSTRACTREPO, parameterSet);
            newParameter->setName(QString::fromStdString((*it)->getId()));
            ezechiel::core::OperableAmount *amount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::OperableAmount>(ABSTRACTREPO, newParameter);
            amount->setDbvalue((*it)->getValue());
            amount->setUnit(ezechiel::core::Unit(QString::fromStdString((*it)->getUnit().toString())));
            newParameter->setQuantity(amount);
            parameterSet->getParameters()->append(newParameter);
            it.next();
        }

        newModel->setParameters(parameterSet);
    }

    {

        const Tucuxi::Core::StandardTreatment *oldTreatment = drugModel->getFormulationAndRoutes().getDefault()->getStandardTreatment();
        if (oldTreatment != nullptr) {
            ezechiel::core::StandardTreatment *standardTreatment = ezechiel::core::CoreFactory::createEntity<ezechiel::core::StandardTreatment>(ABSTRACTREPO, newModel);
            standardTreatment->setIsFixedDuration(oldTreatment->getIsFixedDuration());
            if (oldTreatment->getIsFixedDuration()) {
                ezechiel::core::Duration duration;
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

ezechiel::core::TargetMethod* TucucoreToEzTranslator::translate(Tucuxi::Core::TargetType targetType, ezechiel::core::Target *newTarget)
{
    ezechiel::core::TargetMethod *method = ezechiel::core::CoreFactory::createEntity<ezechiel::core::TargetMethod>(ABSTRACTREPO, newTarget);
    switch (targetType) {
    case Tucuxi::Core::TargetType::Auc : method->setTargetType(ezechiel::core::TargetMethod::TargetType::AUCTarget); break;
    case Tucuxi::Core::TargetType::Auc24 : method->setTargetType(ezechiel::core::TargetMethod::TargetType::AUC24Target); break;
    case Tucuxi::Core::TargetType::AucDividedByMic : method->setTargetType(ezechiel::core::TargetMethod::TargetType::AUCDividedByMicTarget); break;
    case Tucuxi::Core::TargetType::Auc24DividedByMic : method->setTargetType(ezechiel::core::TargetMethod::TargetType::AUC24DividedByMicTarget); break;
    case Tucuxi::Core::TargetType::AucOverMic : method->setTargetType(ezechiel::core::TargetMethod::TargetType::AUCOverMicTarget); break;
    case Tucuxi::Core::TargetType::Auc24OverMic : method->setTargetType(ezechiel::core::TargetMethod::TargetType::AUC24OverMicTarget); break;
    case Tucuxi::Core::TargetType::CumulativeAuc : method->setTargetType(ezechiel::core::TargetMethod::TargetType::CumulativeAUCTarget); break;
    case Tucuxi::Core::TargetType::Mean : method->setTargetType(ezechiel::core::TargetMethod::TargetType::MeanTarget); break;
    case Tucuxi::Core::TargetType::Peak : method->setTargetType(ezechiel::core::TargetMethod::TargetType::PeakTarget); break;
    case Tucuxi::Core::TargetType::PeakDividedByMic : method->setTargetType(ezechiel::core::TargetMethod::TargetType::PeakDividedByMicTarget); break;
    case Tucuxi::Core::TargetType::Residual : method->setTargetType(ezechiel::core::TargetMethod::TargetType::ResidualTarget); break;
    case Tucuxi::Core::TargetType::TimeOverMic : method->setTargetType(ezechiel::core::TargetMethod::TargetType::TimeOverMicTarget); break;
    case Tucuxi::Core::TargetType::UnknownTarget : method->setTargetType(ezechiel::core::TargetMethod::TargetType::UnknownTarget); break;
    }
    return method;
}

ezechiel::core::OperableAmount *TucucoreToEzTranslator::translate(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, ezechiel::core::Target *newTarget)
{
    ezechiel::core::OperableAmount *amount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(subTarget.getValue());
    QString uString = QString::fromStdString(target->getUnit().toString());
    amount->setUnit(ezechiel::core::Unit(uString));
    return amount;
}

ezechiel::core::OperableAmount *TucucoreToEzTranslator::translateMic(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::Unit micUnit,
                                                                     Tucuxi::Core::Unit newUnit, ezechiel::core::Target *newTarget)
{
    ezechiel::core::OperableAmount *amount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(Tucuxi::Core::translateToUnit(subTarget.getValue(), micUnit, newUnit));
    amount->setUnit(ezechiel::core::Unit(QString(newUnit.toString().c_str())));
    return amount;
}


ezechiel::core::OperableAmount *TucucoreToEzTranslator::translateTime(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, ezechiel::core::Target *newTarget)
{
    ezechiel::core::OperableAmount *amount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(subTarget.getValue());
    QString uString = QString::fromStdString(target->getTimeUnit().toString());
    amount->setUnit(ezechiel::core::Unit(uString));
    return amount;
}

ezechiel::core::TranslatableString* TucucoreToEzTranslator::translate(const Tucuxi::Common::TranslatableString &str, ezechiel::core::Entity *parent)
{
    ezechiel::core::TranslatableString *newString = ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(ABSTRACTREPO, parent);
    // Now we only insert the english translation
    newString->insert("en", QString::fromStdString(str.getString("en")));
    return newString;
}
