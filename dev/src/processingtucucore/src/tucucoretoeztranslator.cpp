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
        ezechiel::GuiCore::TargetEvaluationResult *newTarget)
{
    ezechiel::GuiCore::TargetMethod *method = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TargetMethod>(ABSTRACTREPO, newTarget);
    ezechiel::GuiCore::TargetMethod::TargetType newTargetType;
    switch (target->getTargetType()) {
    case Tucuxi::Core::TargetType::Residual : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::ResidualTarget; break;
    case Tucuxi::Core::TargetType::Peak : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::PeakTarget; break;
    case Tucuxi::Core::TargetType::Mean : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::MeanTarget; break;
    case Tucuxi::Core::TargetType::Auc : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::AUCTarget; break;
    case Tucuxi::Core::TargetType::Auc24 : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::AUC24Target; break;
    case Tucuxi::Core::TargetType::CumulativeAuc : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::CumulativeAUCTarget; break;
    case Tucuxi::Core::TargetType::UnknownTarget : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::UnknownTarget; break;
    case Tucuxi::Core::TargetType::FractionTimeOverMic: newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::FractionTimeOverMicTarget; break;

        // The 4 following target types are not yet supported
    case Tucuxi::Core::TargetType::AucOverMic : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::AUCOverMicTarget; break;
    case Tucuxi::Core::TargetType::Auc24OverMic : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::AUC24OverMicTarget; break;
    case Tucuxi::Core::TargetType::TimeOverMic : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::TimeOverMicTarget; break;
    case Tucuxi::Core::TargetType::AucDividedByMic : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::AUCDividedByMicTarget; break;
    case Tucuxi::Core::TargetType::Auc24DividedByMic : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::AUC24DividedByMicTarget; break;
    case Tucuxi::Core::TargetType::PeakDividedByMic : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::PeakDividedByMicTarget; break;
    case Tucuxi::Core::TargetType::ResidualDividedByMic : newTargetType = ezechiel::GuiCore::TargetMethod::TargetType::ResidualDividedByMicTarget; break;
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

ezechiel::GuiCore::Admin::Route TucucoreToEzTranslator::translateFormulationAndRoute(const Tucuxi::Core::FormulationAndRoute& _formulationAndRoute)
{
    switch (_formulationAndRoute.getAbsorptionModel()) {
    case Tucuxi::Core::AbsorptionModel::Extravascular: return ezechiel::GuiCore::Admin::Route::EXTRA;
    case Tucuxi::Core::AbsorptionModel::ExtravascularLag: return ezechiel::GuiCore::Admin::Route::EXTRALAG;
    case Tucuxi::Core::AbsorptionModel::Infusion: return ezechiel::GuiCore::Admin::Route::INFUSION;
    case Tucuxi::Core::AbsorptionModel::Intravascular: return ezechiel::GuiCore::Admin::Route::BOLUS;
    case Tucuxi::Core::AbsorptionModel::Undefined: return ezechiel::GuiCore::Admin::Route::UNVALID;
    }
    return ezechiel::GuiCore::Admin::Route::UNVALID;
}


ezechiel::GuiCore::Admin::Route TucucoreToEzTranslator::translateAbsorptionModel(Tucuxi::Core::AbsorptionModel _absorptionModel)
{
    switch (_absorptionModel) {
    case Tucuxi::Core::AbsorptionModel::Extravascular: return ezechiel::GuiCore::Admin::Route::EXTRA;
    case Tucuxi::Core::AbsorptionModel::ExtravascularLag: return ezechiel::GuiCore::Admin::Route::EXTRALAG;
    case Tucuxi::Core::AbsorptionModel::Infusion: return ezechiel::GuiCore::Admin::Route::INFUSION;
    case Tucuxi::Core::AbsorptionModel::Intravascular: return ezechiel::GuiCore::Admin::Route::BOLUS;
    case Tucuxi::Core::AbsorptionModel::Undefined: return ezechiel::GuiCore::Admin::Route::UNVALID;
    }
    return ezechiel::GuiCore::Admin::Route::UNVALID;
}

bool TucucoreToEzTranslator::buildDosageHistory(const Tucuxi::Core::DosageHistory & dosageHistory,
                                                  ezechiel::GuiCore::DosageHistory *newHistory)
{

    for (const auto & dosageRange : dosageHistory.getDosageTimeRanges()) {
        ezechiel::GuiCore::Dosage *dosage = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Dosage>(ABSTRACTREPO, newHistory);
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
                ezechiel::GuiCore::Admin *admin = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO, dosage);
                admin->setRoute(translateFormulationAndRoute(lasting->getLastFormulationAndRoute()));
                admin->setFormulationAndRoute(lasting->getLastFormulationAndRoute());
                admin->setDescription(description(lasting->getLastFormulationAndRoute()));
                dosage->setRoute(admin);
                // TODO : Deal with units

                dosage->getQuantity()->setUnit(ezechiel::GuiCore::Unit("mg"));

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
                ezechiel::GuiCore::Admin *admin = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO, dosage);
                admin->setRoute(translateFormulationAndRoute(lasting->getLastFormulationAndRoute()));
                admin->setFormulationAndRoute(lasting->getLastFormulationAndRoute());
                admin->setDescription(description(lasting->getLastFormulationAndRoute()));
                dosage->setRoute(admin);
                // TODO : Deal with units
                dosage->getQuantity()->setUnit(ezechiel::GuiCore::Unit("mg"));

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

ezechiel::GuiCore::DrugModel* TucucoreToEzTranslator::buildLightDrugModel(const Tucuxi::Core::DrugModel *drugModel)
{
    // We do not support multiple active substances
    if (drugModel->getActiveMoieties().size() > 1) {
        return nullptr;
    }

    ezechiel::GuiCore::DrugModel *newModel = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugModel>(ABSTRACTREPO, nullptr);

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

    ezechiel::GuiCore::ActiveSubstance *activeSubstance =  ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ActiveSubstance>(ABSTRACTREPO, newModel);

    const Tucuxi::Core::ActiveMoiety *activeMoiety = drugModel->getActiveMoieties().at(0).get();
    activeSubstance->setSubstanceId(QString::fromStdString(activeMoiety->getActiveMoietyId().toString()));

    ezechiel::GuiCore::TranslatableString *name = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(ABSTRACTREPO, activeSubstance);
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

    ezechiel::GuiCore::ADME *adme = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ADME>(ABSTRACTREPO, newModel);
    ezechiel::GuiCore::Admin *admin = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO, adme);
    admin->setRoute(translateAbsorptionModel(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute().getAbsorptionModel()));
    admin->setFormulationAndRoute(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute());
    admin->setDescription(description(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute()));
    admin->setFormulationAndRoute(drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute());
    adme->setDefaultIntake(admin);

    ezechiel::GuiCore::AdminList *adminList = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::AdminList>(ABSTRACTREPO, adme);
    for(auto & formulation : drugModel->getFormulationAndRoutes()) {
        ezechiel::GuiCore::Admin *admin = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO, adminList);
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
        ezechiel::GuiCore::TranslatableString *studyName = translate(drugModel->getMetadata().getStudyName(), newModel);
        newModel->setStudyNameTranslationMap(studyName);
    }

    {
        ezechiel::GuiCore::TranslatableString *description = translate(drugModel->getMetadata().getDescription(), newModel);
        newModel->setDescriptionsTranslationMap(description);
    }


    {
        ezechiel::GuiCore::TranslatableString *domainName = translate(drugModel->getDomain().getDescription(), newModel);
        newModel->setDomainNameTranslationMap(domainName);
    }



    auto defaultFormulation = drugModel->getFormulationAndRoutes().getDefault();
    {
        // All doses values found on drug model
        ezechiel::GuiCore::ValidDoses *validDoses = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidDoses>(ABSTRACTREPO, newModel);
        ezechiel::GuiCore::IdentifiableAmount *amount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::IdentifiableAmount>(ABSTRACTREPO, validDoses);

        QString uString = QString::fromStdString(defaultFormulation->getValidDoses()->getUnit().toString());
        QList<double> doseList;
        for(const auto &doseValue : defaultFormulation->getValidDoses()->getValues()){
            doseList.append(doseValue);
        }

        validDoses->setDosesList(doseList);
        amount->setUnit(ezechiel::GuiCore::Unit(uString));
        amount->setDbvalue(defaultFormulation->getValidDoses()->getDefaultValue());
        validDoses->setQuantity(amount);

        newModel->setDoses(validDoses);
    }

    if (defaultFormulation->getValidInfusionTimes() != nullptr)
    {
        //All infusions values found on drug model
        ezechiel::GuiCore::ValidInfusions *validInfusions = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidInfusions>(ABSTRACTREPO, newModel);
        ezechiel::GuiCore::IdentifiableAmount *amount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::IdentifiableAmount>(ABSTRACTREPO, validInfusions);

        QString uString = QString::fromStdString(defaultFormulation->getValidInfusionTimes()->getUnit().toString());
        QList<double> infusionsList;
        for(const auto &value : defaultFormulation->getValidInfusionTimes()->getValues()){
            infusionsList.append(value);
        }

        validInfusions->setInfusionsList(infusionsList);
        amount->setUnit(ezechiel::GuiCore::Unit(uString));
        amount->setDbvalue(defaultFormulation->getValidInfusionTimes()->getDefaultValue());
        validInfusions->setQuantity(amount);

        newModel->setInfusions(validInfusions);

    }

    {
        // All intervals values found on drug model
        ezechiel::GuiCore::ValidIntervals *validIntervals = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidIntervals>(ABSTRACTREPO, newModel);
        ezechiel::GuiCore::IdentifiableAmount *amount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::IdentifiableAmount>(ABSTRACTREPO, validIntervals);

        QString uString = QString::fromStdString(defaultFormulation->getValidIntervals()->getUnit().toString());
        QList<double> intervalsList;
        for(const auto &value : defaultFormulation->getValidIntervals()->getValues()){
            intervalsList.append(value);
        }

        validIntervals->setIntervalsList(intervalsList);
        amount->setUnit(ezechiel::GuiCore::Unit(uString));
        amount->setDbvalue(defaultFormulation->getValidIntervals()->getDefaultValue());
        validIntervals->setQuantity(amount);

        newModel->setIntervals(validIntervals);
    }

    ezechiel::GuiCore::DrugVariateList *variateList = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugVariateList>(ABSTRACTREPO, newModel);
    for (const auto &covariate : drugModel->getCovariates()) {
        // The GUI do not need to know about computed covariates
        if (covariate.get()->isComputed()) {
            continue;
        }
        // Do not export dose covariates to the GUI
        if (covariate->getType() == Tucuxi::Core::CovariateType::Dose) {
            continue;
        }
        ezechiel::GuiCore::DrugVariate *variate = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugVariate>(ABSTRACTREPO, variateList);
        variate->setCovariateId(QString::fromStdString(covariate->getId()));
        variate->setVisualNameTranslation(translate(covariate->getName(), variate));
        ezechiel::GuiCore::OperableAmount *amount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::OperableAmount>(ABSTRACTREPO, variate);
        amount->setDbvalue(covariate->getValue());
        QString uString = QString::fromStdString(covariate->getUnit().toString());
        amount->setUnit(ezechiel::GuiCore::Unit(uString));
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


    ezechiel::GuiCore::TargetList *targetList = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TargetList>(ABSTRACTREPO, newModel);
    for(const auto &target : drugModel->getActiveMoieties().at(0)->getTargetDefinitions())
    {
        ezechiel::GuiCore::Target *newTarget = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Target>(ABSTRACTREPO, targetList);
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
        ezechiel::GuiCore::ParameterSet *parameterSet = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ParameterSet>(ABSTRACTREPO, newModel);
        // TODO : Be careful, only valid for a single analyteSet
        const Tucuxi::Core::AnalyteGroupId analyteGroupId = drugModel->getAnalyteSets()[0]->getId();
        const Tucuxi::Core::Formulation formulation = drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute().getFormulation();
        const Tucuxi::Core::AdministrationRoute route = drugModel->getFormulationAndRoutes().getDefault()->getFormulationAndRoute().getAdministrationRoute();

        Tucuxi::Core::ParameterDefinitionIterator it = drugModel->getParameterDefinitions(analyteGroupId, formulation, route);

        it.reset();
        while (!it.isDone()) {
            ezechiel::GuiCore::Parameter *newParameter = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Parameter>(ABSTRACTREPO, parameterSet);
            newParameter->setName(QString::fromStdString((*it)->getId()));
            ezechiel::GuiCore::OperableAmount *amount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::OperableAmount>(ABSTRACTREPO, newParameter);
            amount->setDbvalue((*it)->getValue());
            amount->setUnit(ezechiel::GuiCore::Unit(QString::fromStdString((*it)->getUnit().toString())));
            newParameter->setQuantity(amount);
            parameterSet->getParameters()->append(newParameter);
            it.next();
        }

        newModel->setParameters(parameterSet);
    }

    {

        const Tucuxi::Core::StandardTreatment *oldTreatment = drugModel->getFormulationAndRoutes().getDefault()->getStandardTreatment();
        if (oldTreatment != nullptr) {
            ezechiel::GuiCore::StandardTreatment *standardTreatment = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::StandardTreatment>(ABSTRACTREPO, newModel);
            standardTreatment->setIsFixedDuration(oldTreatment->getIsFixedDuration());
            if (oldTreatment->getIsFixedDuration()) {
                ezechiel::GuiCore::Duration duration;
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

ezechiel::GuiCore::TargetMethod* TucucoreToEzTranslator::translate(Tucuxi::Core::TargetType targetType, ezechiel::GuiCore::Target *newTarget)
{
    ezechiel::GuiCore::TargetMethod *method = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TargetMethod>(ABSTRACTREPO, newTarget);
    switch (targetType) {
    case Tucuxi::Core::TargetType::Auc : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::AUCTarget); break;
    case Tucuxi::Core::TargetType::Auc24 : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::AUC24Target); break;
    case Tucuxi::Core::TargetType::AucDividedByMic : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::AUCDividedByMicTarget); break;
    case Tucuxi::Core::TargetType::Auc24DividedByMic : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::AUC24DividedByMicTarget); break;
    case Tucuxi::Core::TargetType::AucOverMic : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::AUCOverMicTarget); break;
    case Tucuxi::Core::TargetType::Auc24OverMic : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::AUC24OverMicTarget); break;
    case Tucuxi::Core::TargetType::CumulativeAuc : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::CumulativeAUCTarget); break;
    case Tucuxi::Core::TargetType::Mean : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::MeanTarget); break;
    case Tucuxi::Core::TargetType::Peak : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::PeakTarget); break;
    case Tucuxi::Core::TargetType::PeakDividedByMic : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::PeakDividedByMicTarget); break;
    case Tucuxi::Core::TargetType::ResidualDividedByMic : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::ResidualDividedByMicTarget); break;
    case Tucuxi::Core::TargetType::Residual : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::ResidualTarget); break;
    case Tucuxi::Core::TargetType::TimeOverMic : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::TimeOverMicTarget); break;
    case Tucuxi::Core::TargetType::FractionTimeOverMic : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::FractionTimeOverMicTarget); break;
    case Tucuxi::Core::TargetType::UnknownTarget : method->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::UnknownTarget); break;
    }
    return method;
}

ezechiel::GuiCore::OperableAmount *TucucoreToEzTranslator::translate(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, ezechiel::GuiCore::Target *newTarget)
{
    ezechiel::GuiCore::OperableAmount *amount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(subTarget.getValue());
    QString uString = QString::fromStdString(target->getUnit().toString());
    amount->setUnit(ezechiel::GuiCore::Unit(uString));
    return amount;
}

ezechiel::GuiCore::OperableAmount *TucucoreToEzTranslator::translateMic(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Common::TucuUnit micUnit,
                                                                     Tucuxi::Common::TucuUnit newUnit, ezechiel::GuiCore::Target *newTarget)
{
    ezechiel::GuiCore::OperableAmount *amount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(Tucuxi::Common::UnitManager::convertToUnit(subTarget.getValue(), micUnit, newUnit));
    amount->setUnit(ezechiel::GuiCore::Unit(QString(newUnit.toString().c_str())));
    return amount;
}


ezechiel::GuiCore::OperableAmount *TucucoreToEzTranslator::translateTime(const Tucuxi::Core::SubTargetDefinition &subTarget, Tucuxi::Core::TargetDefinition *target, ezechiel::GuiCore::Target *newTarget)
{
    ezechiel::GuiCore::OperableAmount *amount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::OperableAmount>(ABSTRACTREPO, newTarget);
    amount->setDbvalue(subTarget.getValue());
    QString uString = QString::fromStdString(target->getTimeUnit().toString());
    amount->setUnit(ezechiel::GuiCore::Unit(uString));
    return amount;
}

ezechiel::GuiCore::TranslatableString* TucucoreToEzTranslator::translate(const Tucuxi::Common::TranslatableString &str, ezechiel::GuiCore::Entity *parent)
{
    ezechiel::GuiCore::TranslatableString *newString = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(ABSTRACTREPO, parent);
    // Now we only insert the english translation
    newString->insert("en", QString::fromStdString(str.getString("en")));
    return newString;
}
