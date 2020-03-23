#include "interpretationbuildermodule.h"
#include "treatmentbuildermodule.h"
#include "dosagebuildermodule.h"
#include "samplebuildermodule.h"
#include "concentrationsbuildermodule.h"
#include "covariatesbuildermodule.h"
#include "parametersbuildermodule.h"
#include "teambuildermodule.h"
#include "patientbuildermodule.h"

#include "reportdata.h"
#include "identifiers.h"
#include "interpretation.h"
#include "interpretationrequest.h"
#include "drugresponseanalysis.h"
#include "drugtreatment.h"
#include "predictionspec.h"

#include "interfaces/processinginterface.h"
#include "interfaces/processingtraits.h"

using namespace ezechiel::core;

AUTO_PROPERTY_IMPL(InterpretationBuilderModule, Interpretation*, interpretation, Interpretation)
AUTO_PROPERTY_IMPL(InterpretationBuilderModule, PredictionSpec*, prediction, Prediction)
AUTO_PROPERTY_IMPL(InterpretationBuilderModule, Measure*, sample, Sample)

InterpretationBuilderModule::InterpretationBuilderModule(Interpretation* interpretation, PredictionSpec* prediction, Measure * sample, QObject *parent) :
    _interpretation(interpretation),
    _prediction(prediction),
    _sample(sample)
{

}

bool InterpretationBuilderModule::setData(ReportData *data)
{
//    if (_prediction == 0 || !_sample->isValid() || _prediction->firsttake() > _sample->getMoment())
//        return true;

    Amount popamt;
    PopulationTraits popTraits;
    PointTraits popPtTraits(_prediction->getStartDate(),
                              _prediction->getEndDate(),
                              _sample->getMoment(),
                              &popTraits);

    Prediction2* pred = CoreFactory::createEntity<Prediction2>(ABSTRACTREPO);
    CORE->getProcessingInterface()->point(_prediction->getAnalysis(),
                                           popPtTraits,
                                           popamt, *pred);

    Amount apramt;
    AprioriTraits aprTraits;
    PointTraits aprPtTraits(_prediction->getStartDate(),
                              _prediction->getEndDate(),
                              _sample->getMoment(),
                              &aprTraits);

    CORE->getProcessingInterface()->point(_prediction->getAnalysis(),
                                           aprPtTraits,
                                           apramt, *pred);

    //Build the extrapolated concentration
    Amount apoamt;
    AposterioriTraits apoTraits;
    PointTraits apoPtTraits(_prediction->getStartDate(),
                              _prediction->getEndDate(),
                              _sample->getMoment(),
                              &apoTraits);

    CORE->getProcessingInterface()->point(_prediction->getAnalysis(),
                                           apoPtTraits,
                                           apoamt, *pred);

    QList<QPair<Amount, QDateTime> > concentrations;
    concentrations.append(QPair<Amount, QDateTime>(apoamt, _sample->getMoment()));
//    Duration nextIntake = _prediction->nextCycle(_sample->getMoment().addMSecs(1));
//    QList<QPair<Amount, QDateTime> > concentrations;
//    concentrations.append(QPair<Amount, QDateTime>(_prediction->analysis()->point(_prediction, nextIntake, APOSTERIORI), _prediction->firsttake().addMSecs(nextIntake.mSecs())));

    //Build the last dose from the dosage
    DosageHistory* istoire = _prediction->getAnalysis()->getTreatment()->getDosages();
    Dosage* dosage = istoire->getList().last();

//    Duration prevIntake = _prediction->prevCycle(_sample->getMoment());
//    Dosage* dosage = _prediction->treatment()->dosages()->currentDosage(_sample->getMoment());
//    dosage->setApplied(_prediction->firsttake().addMSecs(prevIntake.mSecs()));

    //Set the sample and treatment data
    if (!TreatmentBuilderModule(_prediction->getAnalysis()->getTreatment(), this).setData(data)) {
        unsetData(data);
        return false;
    }
    if (!PatientBuilderModule(static_cast<Patient*>(_prediction->getAnalysis()->getTreatment()->getPatient()), this).setData(data)) {
        unsetData(data);
        return false;
    }
    if (!DosageBuilderModule(dosage, this).setData(data)) {
        unsetData(data);
        return false;
    }
    if (!SampleBuilderModule(_sample, this).setData(data)) {
        unsetData(data);
        return false;
    }
    if (!ConcentrationsBuilderModule(concentrations, this).setData(data)) {
        unsetData(data);
        return false;
    }
    if (!CovariatesBuilderModule(_prediction->getAnalysis()->getTreatment()->getDrug()->getCovariates(), _prediction->getAnalysis()->getTreatment()->getCovariates(), this).setData(data)) {
        unsetData(data);
        return false;
    }
//    if (!TeamBuilderModule(_interpretation->getAnalyst(), this).setData(data)) {
//        unsetData(data);
//        return false;
//    }
//    if (!ParametersBuilderModule(_prediction->treatment()->parameters(_sample->getMoment(), POPULATION)).setData(data)) {
//        unsetData(data);
//        return false;
//    }
    if (!ParametersBuilderModule(apoTraits.pset, this).setData(data)) {
        unsetData(data);
        return false;
    }
    if (!ParametersBuilderModule(aprTraits.pset, this).setData(data)) {
        unsetData(data);
        return false;
    }
    if (!ParametersBuilderModule(popTraits.pset, this).setData(data)) {
        unsetData(data);
        return false;
    }

    return true;
}

void InterpretationBuilderModule::unsetData(ReportData *data)
{
    TreatmentBuilderModule().unsetData(data);
//    DosageBuilderModule().unsetData(data);
//    SampleBuilderModule().unsetData(data);
    ConcentrationsBuilderModule().unsetData(data);
    CovariatesBuilderModule().unsetData(data);
    ParametersBuilderModule().unsetData(data);
}
