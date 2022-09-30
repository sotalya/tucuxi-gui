//@@license@@

#include "guitotucucoretranslator.h"

#include "core/dal/predictionspec.h"
#include "core/dal/predictive.h"
#include "core/dal/predictiondata.h"
#include "core/dal/predictionresult.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/dosage.h"
#include "core/dal/covariate.h"
#include "core/dal/coremeasure.h"
#include "core/dal/drug/target.h"
#include "core/dal/drug/drug.h"
#include "core/interfaces/processingtraits.h"
#include "core/core.h"

#include "tucucommon/utils.h"

#include "tucucore/drugmodel/drugmodel.h"
#include "tucucore/drugtreatment/drugtreatment.h"
#include "tucucore/definitions.h"
#include "tucucore/dosage.h"
#include "tucucore/drugmodelrepository.h"
#include "tucucore/drugmodel/formulationandroute.h"


GuiToTucucoreTranslator::GuiToTucucoreTranslator()
{
}


DateTime GuiToTucucoreTranslator::buildDateTime(const QDateTime &qDate)
{
//    if (!qDate.isValid()) {
//        return DateTime();
//    }
//    return DateTime(std::chrono::milliseconds(qDate.toMSecsSinceEpoch()));

    DateTime d = DateTime(qDate.toString("yyyy-MM-dd-hh-mm-ss").toStdString(),"%Y-%m-%d-%H-%M-%S");

//    std::cout << "DateTime in : " << qDate.toString().toStdString() << std::endl;
//    std::cout << "DateTime in : " << qDate.toString("yyyy-MM-dd-hh-mm-ss").toStdString() << std::endl;
//    std::cout << "DateTime out: " << d << std::endl;

    return d;
}


Tucuxi::Common::TucuUnit GuiToTucucoreTranslator::buildUnit(const QString &_strUnit)
{
    return Tucuxi::Common::TucuUnit(_strUnit.toStdString());
}

Tucuxi::Core::PredictionParameterType GuiToTucucoreTranslator::buildParameterType(const Tucuxi::Gui::Core::ParamTraits *traits)
{
    switch (traits->ptype()) {
    case Tucuxi::Gui::Core::ParameterType::POPULATION  : return Tucuxi::Core::PredictionParameterType::Population;
    case Tucuxi::Gui::Core::ParameterType::APRIORI     : return Tucuxi::Core::PredictionParameterType::Apriori;
    case Tucuxi::Gui::Core::ParameterType::APOSTERIORI : return Tucuxi::Core::PredictionParameterType::Aposteriori;
    default                                         : return Tucuxi::Core::PredictionParameterType::Population;
    }
}

Tucuxi::Core::DosageTimeRange *GuiToTucucoreTranslator::buildTimeRange(const Tucuxi::Gui::Core::Dosage *_ezDosage)
{
    Tucuxi::Core::FormulationAndRoute formulationAndRoute = _ezDosage->getRoute()->getFormulationAndRoute();
    Tucuxi::Core::LastingDose lastingDose(_ezDosage->getQuantity()->getDbvalue(),
                                          Tucuxi::Common::TucuUnit(_ezDosage->getQuantity()->getUnitstring().toStdString()),
                                          formulationAndRoute,
                                          std::chrono::seconds(static_cast<int>(_ezDosage->getDbtinf()*60.0)),
                                          std::chrono::seconds(static_cast<int>(_ezDosage->getDbinterval()*3600.0)));

    Tucuxi::Common::DateTime appliedDate = buildDateTime(_ezDosage->getApplied());


    Tucuxi::Common::DateTime endDate = buildDateTime(_ezDosage->getEndTime());

    if (appliedDate == endDate) {
        if (_ezDosage->getDbinterval() > 0.0) {
            // Seems that the user wants a single dose, so add the interval
            endDate = appliedDate + Duration(std::chrono::seconds(static_cast<int>(_ezDosage->getDbinterval()*3600.0)));
        }
        else {
            // Don't know how to manage a 0 time interval with no end time
            return nullptr;
        }
    }
    else if (appliedDate > endDate) {
        // Something wrong here
        return nullptr;
    }


    if (_ezDosage->getIsAtSteadyState()) {
        Tucuxi::Core::DosageSteadyState steadyState(lastingDose, buildDateTime(_ezDosage->getApplied()));
        return new Tucuxi::Core::DosageTimeRange(
            appliedDate,    // startDate,
            endDate,    // endDate,
            steadyState);                                 // dosage
    }
    else {
        Tucuxi::Core::DosageLoop loop(lastingDose);
        return new Tucuxi::Core::DosageTimeRange(
            appliedDate,    // startDate,
            endDate,    // endDate,
            loop);                                 // dosage
    }
}


Tucuxi::Core::DrugTreatment *GuiToTucucoreTranslator::buildTreatment(const Tucuxi::Gui::Core::DrugTreatment *_ezTreatment, QDateTime adjTime)
{

    Tucuxi::Core::DrugTreatment *newTreatment = new Tucuxi::Core::DrugTreatment();
    QList<Tucuxi::Gui::Core::Dosage*> dosageList = _ezTreatment->getDosages()->getList();
    QList<Tucuxi::Gui::Core::Dosage*>::iterator itDosages = dosageList.begin();
    while (itDosages != dosageList.end()) {
        Tucuxi::Gui::Core::Dosage *dosage = *itDosages++;

        Tucuxi::Core::DosageTimeRange *timeRange = buildTimeRange(dosage);
        if (timeRange != nullptr) {
            newTreatment->addDosageTimeRange(std::unique_ptr<Tucuxi::Core::DosageTimeRange>(timeRange));
        }
    }


    if (adjTime.isValid() && _ezTreatment->getAdjustments()->size() > 0) {

        foreach(Tucuxi::Gui::Core::Dosage* dosage, _ezTreatment->getAdjustments()->getList())
        {
            if (dosage->getApplied() >= adjTime)
            {

                Tucuxi::Core::DosageTimeRange *timeRange = buildTimeRange(dosage);
                if (timeRange != nullptr) {
                    newTreatment->mergeDosageTimeRange(std::unique_ptr<Tucuxi::Core::DosageTimeRange>(timeRange));
                }
            }
        }
    }


    QList<Tucuxi::Gui::Core::PatientVariate*> covariateList = _ezTreatment->getCovariates()->getList();
    QList<Tucuxi::Gui::Core::PatientVariate*>::iterator itCovariates = covariateList.begin();
    while (itCovariates != covariateList.end()) {
        Tucuxi::Gui::Core::PatientVariate *covariate = *itCovariates++;
        if (covariate->getCovariateId() == "birthdate") {
            QDateTime birthdate;
            birthdate = QDateTime::fromString(covariate->getValueAsString(), Qt::ISODate);
            // Do not use setSecsSinceEpoch as Qt5.7 does not support it
//            birthdate.setMSecsSinceEpoch(covariate->getQuantity()->getDbvalue() * 1000);

            newTreatment->addCovariate(std::make_unique<Tucuxi::Core::PatientCovariate>(
                covariate->getCovariateId().toStdString(),                    // _id,
                Tucuxi::Common::Utils::varToString(buildDateTime(birthdate)),    // _value,
                Tucuxi::Core::DataType::Date,                              // _dataType,
                Tucuxi::Common::TucuUnit(),      // _unit,
                buildDateTime(covariate->getDate())));                     // _date
        }
        else {
        newTreatment->addCovariate(std::make_unique<Tucuxi::Core::PatientCovariate>(
            covariate->getCovariateId().toStdString(),                    // _id,
            std::to_string(covariate->getQuantity()->getDbvalue()),    // _value,
            Tucuxi::Core::DataType::Double,                                          // _dataType,
            buildUnit(covariate->getQuantity()->getUnitstring()),      // _unit,
            buildDateTime(covariate->getDate())));                     // _date
        }
    }

    // TODO : Be careful her, we use the active substance ID
    std::string analyteId = _ezTreatment->getActiveSubstanceId().toStdString();
    Tucuxi::Core::ActiveMoietyId activeMoietyId =
            Tucuxi::Core::ActiveMoietyId(_ezTreatment->getActiveSubstanceId().toStdString());

    QList<Tucuxi::Gui::Core::CoreMeasure*> sampleList = _ezTreatment->getMeasures()->getList();
    QList<Tucuxi::Gui::Core::CoreMeasure*>::iterator itSamples = sampleList.begin();
    while (itSamples != sampleList.end()) {
        Tucuxi::Gui::Core::CoreMeasure *sample = *itSamples++;
        newTreatment->addSample(std::make_unique<Tucuxi::Core::Sample>(
            buildDateTime(sample->getMoment()),                     // date,
            Tucuxi::Core::AnalyteId(analyteId),                     // analyteId,
            sample->getConcentration()->getDbvalue(),               // value,
            buildUnit(sample->getConcentration()->getUnitstring())  // unit
        ));
    }

    QList<Tucuxi::Gui::Core::Target*> targetList = _ezTreatment->getTargets()->getList();
    QList<Tucuxi::Gui::Core::Target*>::iterator itTargets = targetList.begin();
    while (itTargets != targetList.end()) {
        Tucuxi::Gui::Core::Target *target = *itTargets++;
        Tucuxi::Core::TargetType targetType;
        switch (target->getType()->getTargetType()) {
        case Tucuxi::Gui::Core::TargetMethod::TargetType::ResidualTarget:             targetType = Tucuxi::Core::TargetType::Residual; break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::PeakTarget:                 targetType = Tucuxi::Core::TargetType::Peak;     break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::MeanTarget:                 targetType = Tucuxi::Core::TargetType::Mean;     break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::AUCTarget:                  targetType = Tucuxi::Core::TargetType::Auc;      break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::AUC24Target:                targetType = Tucuxi::Core::TargetType::Auc24;      break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::CumulativeAUCTarget :       targetType = Tucuxi::Core::TargetType::CumulativeAuc; break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::AUCOverMicTarget :          targetType = Tucuxi::Core::TargetType::AucOverMic; break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::AUC24OverMicTarget :        targetType = Tucuxi::Core::TargetType::Auc24OverMic; break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::TimeOverMicTarget :         targetType = Tucuxi::Core::TargetType::TimeOverMic; break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::AUCDividedByMicTarget :     targetType = Tucuxi::Core::TargetType::AucDividedByMic; break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::AUC24DividedByMicTarget :   targetType = Tucuxi::Core::TargetType::Auc24DividedByMic; break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::PeakDividedByMicTarget :    targetType = Tucuxi::Core::TargetType::PeakDividedByMic; break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::UnknownTarget :             targetType = Tucuxi::Core::TargetType::UnknownTarget; break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::ResidualDividedByMicTarget: targetType = Tucuxi::Core::TargetType::ResidualDividedByMic; break;
        case Tucuxi::Gui::Core::TargetMethod::TargetType::FractionTimeOverMicTarget:  targetType = Tucuxi::Core::TargetType::FractionTimeOverMic; break;
        }
        if (target->getTbest()->getUnitstring() == "h") {
            newTreatment->addTarget(std::make_unique<Tucuxi::Core::Target>(
                                        activeMoietyId,
                                        targetType,
                                        target->getCmin()->getUnitstring().toStdString(),
                                        target->getCmin()->getDbvalue(),
                                        target->getCbest()->getDbvalue(),
                                        target->getCmax()->getDbvalue(),
                                        target->getMic()->getDbvalue(),
                                        target->getMic()->getUnitstring().toStdString(),
                                        // Here we consider the target times to be in hours
                                        std::chrono::seconds(static_cast<int>(60*60*target->getTmin()->getDbvalue())),
                                        std::chrono::seconds(static_cast<int>(60*60*target->getTbest()->getDbvalue())),
                                        std::chrono::seconds(static_cast<int>(60*60*target->getTmax()->getDbvalue()))));
        }
        else if (target->getTbest()->getUnitstring() == "m") {
            newTreatment->addTarget(std::make_unique<Tucuxi::Core::Target>(
                                        activeMoietyId,
                                        targetType,
                                        target->getCmin()->getUnitstring().toStdString(),
                                        target->getCmin()->getDbvalue(),
                                        target->getCbest()->getDbvalue(),
                                        target->getCmax()->getDbvalue(),
                                        target->getMic()->getDbvalue(),
                                        target->getMic()->getUnitstring().toStdString(),
                                        // Here we consider the target times to be in minutes
                                        std::chrono::seconds(static_cast<int>(60*target->getTmin()->getDbvalue())),
                                        std::chrono::seconds(static_cast<int>(60*target->getTbest()->getDbvalue())),
                                        std::chrono::seconds(static_cast<int>(60*target->getTmax()->getDbvalue()))));
        }
        else if (target->getTbest()->getUnitstring() == "s") {
            newTreatment->addTarget(std::make_unique<Tucuxi::Core::Target>(
                                        activeMoietyId,
                                        targetType,
                                        target->getCmin()->getUnitstring().toStdString(),
                                        target->getCmin()->getDbvalue(),
                                        target->getCbest()->getDbvalue(),
                                        target->getCmax()->getDbvalue(),
                                        target->getMic()->getDbvalue(),
                                        target->getMic()->getUnitstring().toStdString(),
                                        // Here we consider the target times to be in seconds
                                        std::chrono::seconds(static_cast<int>(target->getTmin()->getDbvalue())),
                                        std::chrono::seconds(static_cast<int>(target->getTbest()->getDbvalue())),
                                        std::chrono::seconds(static_cast<int>(target->getTmax()->getDbvalue()))));
        }
        else {
            // Something went wrong
        }

    }

    return newTreatment;
}

#include "drugs2manager.h"

Tucuxi::Core::DrugModel *GuiToTucucoreTranslator::buildDrugModel(const Tucuxi::Gui::Core::DrugModel *_drugModel)
{
    Tucuxi::Core::DrugModel *pDrugModel = nullptr;

    pDrugModel = Tucuxi::Gui::Processing::Drugs2Manager::getInstance()->getTucucoreById(_drugModel->getDrugModelId().toStdString());
    if (pDrugModel == nullptr) {
        std::cout << "Can not import the drugs2 drug model" << std::endl;
    }

    return pDrugModel;
}


