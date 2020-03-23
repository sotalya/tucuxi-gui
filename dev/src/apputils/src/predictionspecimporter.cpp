#include "predictionspecimporter.h"

#include "core/corefactory.h"
#include "core/core.h"
#include "core/dal/drug/drug.h"
#include "core/dal/drug/adme.h"
#include "core/dal/drug/halflife.h"
#include "core/dal/drug/concentrations.h"
#include "core/dal/drug/results.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/target.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/drug/errormodel.h"
#include "core/dal/drug/bsv.h"
#include "core/dal/drug/operation.h"
#include "core/dal/drug/metadata.h"
#include "core/dal/drug/editors.h"
#include "core/dal/drug/references.h"
#include "core/dal/drug/standardtreatment.h"

#include "core/dal/drug/parameters.h"
#include "core/dal/predictionspec.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/coremeasure.h"
#include "core/dal/covariate.h"
#include "core/dal/dosage.h"

#include "core/utils/duration.h"

#include <iostream>

#define WHILE_NOT_END_ELEM(elem)     while(isOk && !reader.hasError() && (!reader.isEndElement() || reader.name() != elem))


PredictionSpecImporter::PredictionSpecImporter()
{

}

ezechiel::core::PredictionSpec *PredictionSpecImporter::load(const QByteArray &data)
{
    isOk = true;

    ezechiel::core::PredictionSpec *predictionSpec = nullptr;

    reader.addData(data);
    reader.readNext();
    while(!reader.atEnd() && isOk){
        QString name = reader.name().toString();

        switch(reader.tokenType()){
        case QXmlStreamReader::StartElement :
            if (name == "predictionSpec") {
                predictionSpec = loadPredictionSpec("predictionSpec");
            }
            break;
        default:
            break;
        }
        reader.readNext();
    }

    if(reader.hasError() || !isOk){
        std::cout <<"reader error " << qPrintable(errorString()) << std::endl;
        std::cout <<"reader error " << qPrintable(_errorString) << std::endl;
        delete predictionSpec;
        predictionSpec = nullptr;
    }

    return predictionSpec;
}

void PredictionSpecImporter::checkValidity(bool condition)
{
    if (!condition) {
        /*****************
         * TODO: store information about the error
         *****************/
        isOk = false;
        _errorString = errorString();
        std::cout <<"reader error " << qPrintable(errorString()) << std::endl;
    }
}

void PredictionSpecImporter::checkReaderError()
{
    if (reader.hasError()) {
        /*****************
         * TODO: store information about the error
         *****************/
        isOk = false;
        _errorString = errorString();
        std::cout <<"reader error " << qPrintable(errorString()) << std::endl;
    }

}

QDateTime PredictionSpecImporter::extractDate(bool essential)
{
    QDateTime date = QDateTime::fromString(extractor(),"yyyy-MM-dd. hh:mm:ss");
    if (essential)
        checkValidity(date.isValid());
    return date;
}

double PredictionSpecImporter::extractDouble()
{
    QString value = extractor();
    bool ok;
    double result;
    result = value.toDouble(&ok);
    checkValidity(ok);
    return result;
}

bool PredictionSpecImporter::extractBool()
{
    QString value = extractor();
    if (value.compare("true") == 0)
        return true;
    if (value.compare("false") == 0)
        return false;
    // There is an error
    checkValidity(false);

    return false;
}

ezechiel::core::PredictionSpec *PredictionSpecImporter::loadPredictionSpec(const QString &tagName, QObject *parent)
{

    ezechiel::core::PredictionSpec *predictionSpec = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionSpec>(ABSTRACTREPO, parent);

    bool typeSet = false;
    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "calculationType") {
                predictionSpec->setCalculationType(predictionSpec->stringToCalculationType(extractor()));
                typeSet = true;
            }
            if (name == "startDate") {
                predictionSpec->setStartDate(extractDate());
            }
            else if (name == "endDate") {
                predictionSpec->setEndDate(extractDate());
            }
            else if (name == "steadyState") {
                predictionSpec->setSteadyState(extractBool());
            }
            else if (name == "nbPoints") {
                predictionSpec->setNbPoints(extractor().toInt());
            }
            else if (name == "paramsType") {
                predictionSpec->setParamsType(ezechiel::core::fromString(extractor()));
            }
            else if (name == "adjustmentDate") {
                bool required;
                if (typeSet)
                    required = (predictionSpec->getCalculationType() == predictionSpec->CalculationType::RevPred);
                else {
                    required = false;
                    checkValidity(false);
                }
                predictionSpec->setAdjustmentDate(extractDate(required));
            }
            else if (name == "percentiles") {
                predictionSpec->setPercentileList(loadPercentiles("percentiles", predictionSpec));
            }
            else if(name == "drugResponseAnalysis"){
                predictionSpec->setAnalysis(loadDrugResponseAnalysis("drugResponseAnalysis", predictionSpec));
            }
        }
    }
    return predictionSpec;
}


std::vector<double> PredictionSpecImporter::loadPercentiles(const QString &tagName, QObject *parent)
{
    std::vector<double> percentiles;

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "p") {
                percentiles.push_back(extractDouble());
            }
        }
    }

    return percentiles;
}

ezechiel::core::DrugResponseAnalysis * PredictionSpecImporter::loadDrugResponseAnalysis(const QString &tagName, QObject *parent)
{
    ezechiel::core::DrugResponseAnalysis *analysis = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugResponseAnalysis>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "treatment") {
                analysis->setTreatment(loadDrugTreatment("treatment", analysis));
            }
            else if (name == "drugModel") {
                analysis->setDrugModel(loadDrugModel("drugModel", analysis));
            }
        }
    }

    return analysis;
}


ezechiel::core::DrugTreatment * PredictionSpecImporter::loadDrugTreatment(const QString &tagName, QObject *parent)
{

    ezechiel::core::DrugTreatment *treatment = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugTreatment>(ABSTRACTREPO, parent);


    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "measures") {
                ezechiel::core::CoreMeasureList *list =  ezechiel::core::CoreFactory::createEntity<ezechiel::core::CoreMeasureList>(ABSTRACTREPO, treatment);

                WHILE_NOT_END_ELEM("measures"){

                    if(isOk && reader.readNextStartElement()){
                        QString name = reader.name().toString();

                        if (name == "measure") {
                            ezechiel::core::CoreMeasure *measure =  ezechiel::core::CoreFactory::createEntity<ezechiel::core::CoreMeasure>(ABSTRACTREPO, list);

                            WHILE_NOT_END_ELEM("measure"){

                                if(isOk && reader.readNextStartElement()){
                                    QString name = reader.name().toString();

                                    if (name == "concentration") {
                                        measure->setConcentration(loadIdentifiableAmount("concentration", measure));
                                    }
                                    else if (name == "moment") {
                                        measure->setMoment(extractDate());
                                    }
                                    else if (name == "uncastedValues") {

                                    }
                                }
                            }

                            list->append(measure);
                        }
                    }
                }
                treatment->setMeasures(list);
            }
            else if (name == "patientVariates") {
                treatment->setCovariates(loadPatientVariates("patientVariates", treatment));
            }
            else if (name == "dosageHistory") {
                treatment->setDosages(loadDosageHistory("dosageHistory", treatment));
            }
            else if (name == "targets") {
                treatment->setTargets(loadTargets("targets", treatment));
            }
        }
    }

    return treatment;
}

ezechiel::core::PatientVariateList *PredictionSpecImporter::loadPatientVariates(const QString &tagName, QObject *parent)
{
    ezechiel::core::PatientVariateList *patientVariateSet = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PatientVariateList>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){
        /******************************************
         * TODO : Check the tag name : covariate
         *****************************************/
        if(reader.readNextStartElement() && isOk){
            patientVariateSet->append(loadPatientVariate("patientVariate", patientVariateSet));
        }
    }
    checkReaderError();

    return patientVariateSet;
}

ezechiel::core::PatientVariate *PredictionSpecImporter::loadPatientVariate(const QString &tagName, QObject *parent)
{
    ezechiel::core::PatientVariate *patientVariate = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PatientVariate>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "name")
                patientVariate->setCovariateId(extractor());
            else if (name == "date")
                patientVariate->setDate(extractDate());
            else if (name == "quantity")
                patientVariate->setQuantity(loadOperableAmount("quantity", patientVariate));
            else if (name == "type") {
                /********************************
                 * TODO: Manage the type. There should be an already existing method somewhere
                 ********************************/
                //     patientVariate->setType();
            }
            else if (name == "forced") {
                patientVariate->setForced(extractBool());
            }
        }
    }

    checkReaderError();

    return patientVariate;
}

ezechiel::core::DosageHistory *PredictionSpecImporter::loadDosageHistory(const QString &tagName, QObject *parent)
{
    ezechiel::core::DosageHistory *history = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DosageHistory>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){
        /******************************************
         * TODO : Check the tag name : dosage
         *****************************************/
        if(reader.readNextStartElement() && isOk){
            history->append(loadDosage("dosage", history));
        }
    }

    checkReaderError();

    return history;
}

ezechiel::core::Dosage *PredictionSpecImporter::loadDosage(const QString &tagName, QObject *parent)
{
    ezechiel::core::Dosage *dosage = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Dosage>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "dbinterval")
                dosage->setDbinterval(extractDouble());
            else if (name == "dbtinf")
                dosage->setDbtinf(extractDouble());
            else if (name == "route") {
                ezechiel::core::Admin *admin = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Admin>(ABSTRACTREPO, dosage);
                admin->setLabel(extractor());
                dosage->setRoute(admin);
            }
            else if (name == "quantity") {
                dosage->setQuantity(loadIdentifiableAmount("quantity", dosage));
            }
            else if (name == "applied")
                dosage->setApplied(extractDate());
            else if (name == "endTime")
                dosage->setEndTime(extractDate());
            else if (name == "tinf") {
                dosage->setTinf(ezechiel::core::Duration(0, extractDouble()));
            }
            else if (name == "interval") {
                dosage->setInterval(ezechiel::core::Duration(0, extractDouble()));
            }
            else if (name == "uncastedValues") {
                // Handle uncasted values
            }
        }
    }
    checkReaderError();

    return dosage;
}



ezechiel::core::DrugModel *PredictionSpecImporter::loadDrugModel(const QString &tagName, QObject *parent)
{

    ezechiel::core::DrugModel *drugModel = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugModel>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if (name == "drugModelId")
                drugModel->setDrugModelId(extractor());
            if (name == "pkModelId")
                drugModel->setPkModelId(extractor());
            else if(name == "adme"){
                drugModel->setAdme(loadAdme("adme", drugModel));
                //            }else if(name == "halflife"){
                //                isOk = loadHalflife(drugModel);
                //            }else if(name == "conversions"){
                //                isOk = loadConversions(drugModel);
            }else if(name == "standardTreatment"){
                drugModel->setStandardTreatment(loadStandardTreatment("standardTreatement", drugModel));
            }else if(name == "dosages"){

                WHILE_NOT_END_ELEM("dosages") {
                    if(isOk && reader.readNextStartElement()) {
                        QString name = reader.name().toString();
                        if(name == "validDoses") {
                            drugModel->setDoses(loadValidDoses("validDoses", drugModel));
                        } else if(name == "validIntervals"){
                            drugModel->setIntervals(loadValidIntervals("validIntervals", drugModel));
                        } else if(name == "validInfusions"){
                            drugModel->setInfusions(loadValidInfusions("validInfusions", drugModel));
                        }
                    }
                }

            }else if(name == "targets"){
                drugModel->setTargets(loadTargets("targets", drugModel));
            }else if(name == "drugVariates"){
                drugModel->setCovariates(loadCovariates("drugVariates", drugModel));
            }else if(name == "errorModel"){
                drugModel->setErrorModel(loadErrorModel("errorModel", drugModel));
            }else if(name == "parameterSet"){
                drugModel->setParameters(loadParameters("parameterSet", drugModel));
                // }else if(name == "operations"){
                //     drugModel->setOperations(loadOperations("operations", drugModel));
            }
        }
    }

    return drugModel;
}


ezechiel::core::StandardTreatment* PredictionSpecImporter::loadStandardTreatment(const QString &tagName, QObject* parent)
{

    bool isConvertible = true;
    ezechiel::core::StandardTreatment* treatment = ezechiel::core::CoreFactory::createEntity<ezechiel::core::StandardTreatment>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "isFixedDuration"){
                treatment->setIsFixedDuration(extractor() == "true" ? true : false);
            } else if (name == "duration"){
                QString unit;
                double value;
                //parses the doses
                WHILE_NOT_END_ELEM("duration"){
                    if(isConvertible && reader.readNextStartElement() ){
                        if(reader.name() == "unit"){
                            unit = extractor();
                        }
                        else if (reader.name() == "value") {
                            value = extractor().toDouble(&isConvertible);
                        }
                    } else {
                        break;
                    }
                }
                if (unit == "h") {
                    treatment->setDuration(ezechiel::core::Duration(value));
                }
                else if (unit == "d") {
                    treatment->setDuration(ezechiel::core::Duration(24 * value));
                }
                else if (unit == "m") {
                    treatment->setDuration(ezechiel::core::Duration(((double) (value))/60));
                }
                else {
                    raiseConversionError();
                }
            }
        } else if (!isConvertible){
            raiseConversionError();
        }
    }

    checkReaderError();

    return treatment;
}

ezechiel::core::ADME *PredictionSpecImporter::loadAdme(const QString &tagName, QObject *parent)
{
    ezechiel::core::ADME* adme = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ADME>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if (name == "defaultIntake") {
                ezechiel::core::Admin* def = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Admin>(ABSTRACTREPO, adme);
                def->setLabel(extractor());
                adme->setDefaultIntake(def);
            }
            else if (name == "intakes") {
                ezechiel::core::AdminList* intakes = ezechiel::core::CoreFactory::createEntity<ezechiel::core::AdminList>(ABSTRACTREPO, adme);


                WHILE_NOT_END_ELEM("intakes"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "intake"){
                            ezechiel::core::Admin* def = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Admin>(ABSTRACTREPO, adme);
                            def->setLabel(extractor());
                            intakes->append(def);
                        }
                    }
                }

                adme->setIntakes(intakes);
            } else if(name == "distribution"){
                adme->setDistribution(extractor());
            } else if(name == "elimination"){
                adme->setElimination(extractor());
            }
        }
    }

    checkReaderError();

    return adme;
}



ezechiel::core::ValidDoses *PredictionSpecImporter::loadValidDoses(const QString &tagName, QObject *parent)
{
    ezechiel::core::ValidDoses* doses = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidDoses>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if (name == "anyDose")
                doses->setAnyDose(extractBool());
            else if (name == "defaultDose")
                doses->setQuantity(loadIdentifiableAmount("defaultDose", doses));
            else if (name == "doses") {

                WHILE_NOT_END_ELEM("doses"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "dose"){
                            ezechiel::core::ValidDose* dose = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidDose>(ABSTRACTREPO,doses);
                            WHILE_NOT_END_ELEM("dose") {
                                if(reader.readNextStartElement() ){
                                    name = reader.name().toString();
                                    if (name == "route") {
                                        ezechiel::core::Admin* admin = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Admin>(ABSTRACTREPO,dose);
                                        admin->setLabel(extractor());
                                        dose->setRoute(admin);
                                    }
                                    else if (name == "quantity")
                                        dose->setQuantity(loadIdentifiableAmount("quantity", dose));
                                }
                            }
                            doses->append(dose);
                        }
                    }
                }
            } else {
                break;
            }
        }
    }

    checkReaderError();

    return doses;

}

ezechiel::core::ValidIntervals *PredictionSpecImporter::loadValidIntervals(const QString &tagName, QObject *parent)
{
    ezechiel::core::ValidIntervals* intervals = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidIntervals>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if (name == "any")
                intervals->setAny(extractBool());
            else if (name == "defaultInterval")
                intervals->setQuantity(loadIdentifiableAmount("defaultInterval", intervals));
            else if (name == "intervals") {

                WHILE_NOT_END_ELEM("intervals"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "interval"){
                            ezechiel::core::ValidInterval* interval = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidInterval>(ABSTRACTREPO,intervals);
                            WHILE_NOT_END_ELEM("interval") {
                                if(reader.readNextStartElement() ){
                                    name = reader.name().toString();
                                    if (name == "quantity")
                                        interval->setQuantity(loadIdentifiableAmount("quantity", interval));
                                }
                            }
                            intervals->append(interval);
                        }
                    }
                }
            } else {
                break;
            }
        }
    }

    checkReaderError();

    return intervals;
}

ezechiel::core::ValidInfusions *PredictionSpecImporter::loadValidInfusions(const QString &tagName, QObject *parent)
{
    ezechiel::core::ValidInfusions* infusions = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidInfusions>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if (name == "any")
                infusions->setAny(extractBool());
            else if (name == "defaultInfusion")
                infusions->setQuantity(loadIdentifiableAmount("defaultInfusion", infusions));
            else if (name == "infusions") {

                WHILE_NOT_END_ELEM("infusions"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "infusion"){
                            ezechiel::core::ValidInfusion* infusion = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidInfusion>(ABSTRACTREPO,infusions);
                            WHILE_NOT_END_ELEM("infusion") {
                                if(reader.readNextStartElement() ){
                                    name = reader.name().toString();
                                    if (name == "quantity")
                                        infusion->setQuantity(loadIdentifiableAmount("quantity", infusion));
                                }
                            }
                            infusions->append(infusion);
                        }
                    }
                }
            } else {
                break;
            }
        }
    }

    checkReaderError();

    return infusions;
}

ezechiel::core::TargetList *PredictionSpecImporter::loadTargets(const QString &tagName, QObject *parent)
{
    ezechiel::core::TargetList* targetSet = ezechiel::core::CoreFactory::createEntity<ezechiel::core::TargetList>(ABSTRACTREPO, parent);
    WHILE_NOT_END_ELEM("targets"){
        if(reader.readNextStartElement() && isOk){
            QString name = reader.name().toString();
            if(name == "target"){
                targetSet->append(loadTarget("target", targetSet));
            }
        }
    }

    checkReaderError();

    return targetSet;
}

ezechiel::core::Target *PredictionSpecImporter::loadTarget(const QString &tagName, QObject *parent)
{
    ezechiel::core::Target* target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Target>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "method"){
                target->getType()->setLabel(extractor());
            } else if(name == "concentrations"){
                QXmlStreamAttributes attributes ;
                extractor(&attributes,false);
                ezechiel::core::Unit unit;
                WHILE_NOT_END_ELEM("concentrations"){
                    if(reader.readNextStartElement() ){
                        name = reader.name().toString();
                        if(name == "cmin"){
                            target->setCmin(loadOperableAmount("cmin", target));
                        } else if(name == "cmax"){
                            target->setCmax(loadOperableAmount("cmax", target));
                        } else if(name == "cbest"){
                            target->setCbest(loadOperableAmount("cbest", target));
                        }
                    }
                }
            } else if(name == "times"){
                QXmlStreamAttributes attributes ;
                extractor(&attributes,false);
                ezechiel::core::Unit unit;
                WHILE_NOT_END_ELEM("times"){
                    if(reader.readNextStartElement() ){
                        name = reader.name().toString();
                        if(name == "tmin"){
                            target->setTmin(loadOperableAmount("tmin", target));
                        } else if(name == "tmax"){
                            target->setTmax(loadOperableAmount("tmax", target));
                        } else if(name == "tbest"){
                            target->setTbest(loadOperableAmount("tbest", target));
                        }
                    }
                }
            }
        }
    }

    checkReaderError();

    return target;
}

ezechiel::core::DrugVariateList *PredictionSpecImporter::loadCovariates(const QString &tagName, QObject *parent)
{
    ezechiel::core::DrugVariateList *drugVariateSet = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugVariateList>(ABSTRACTREPO, parent);

    /*******************************
     * TODO: Totally wrong down there
     *******************************/
    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement()){
            QString name = reader.name().toString();
            if (name == "drugVariate") {
                drugVariateSet->append(loadCovariate("drugVariate", drugVariateSet));
            }
        }
    }
    checkReaderError();

    return drugVariateSet;
}

ezechiel::core::DrugVariate *PredictionSpecImporter::loadCovariate(const QString &tagName, QObject *parent)
{
    ezechiel::core::DrugVariate* drugVariate = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugVariate>(ABSTRACTREPO, parent);

    ezechiel::core::TranslatableString *nameTranslation, *descriptionTranslation;

    nameTranslation = ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(ABSTRACTREPO,drugVariate);
    descriptionTranslation = ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(ABSTRACTREPO,drugVariate);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "name"){
                drugVariate->setCovariateId(extractor());
            } else if (name == "quantity"){
                drugVariate->setQuantity(loadOperableAmount("quantity", drugVariate));
            } else if (name == "type"){
                drugVariate->setType(extractType());
            }
        }
    }

    checkReaderError();

    return drugVariate;
}

ezechiel::core::ErrorModel *PredictionSpecImporter::loadErrorModel(const QString &tagName, QObject *parent)
{
    ezechiel::core::ErrorModel* errorModel = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ErrorModel>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "additive"){
                errorModel->setAdditive(extractDouble());
            } else if (name == "proportional"){
                errorModel->setProportional(extractDouble());
            }
        }
    }

    checkReaderError();

    return errorModel;
}

ezechiel::core::ParameterSet *PredictionSpecImporter::loadParameters(const QString &tagName, QObject *parent)
{

    ezechiel::core::ParameterSet* parameterSet = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ParameterSet>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "parameters"){

                WHILE_NOT_END_ELEM("parameters"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "parameter"){
                            parameterSet->set(loadParameter("parameter", parameterSet));
                        }
                    }
                }

            }
            else if (name == "correlations") {

                QString param1, param2;
                double value;
                WHILE_NOT_END_ELEM("correlations"){
                    if(isOk && reader.readNextStartElement()  && reader.name() == "correlation"){
                        WHILE_NOT_END_ELEM("correlation"){
                            if(reader.readNextStartElement() ){
                                QString name = reader.name().toString();
                                if(name == "param1"){
                                    param1 = extractor();
                                } else if (name == "param2"){
                                    param2 = extractor();
                                } else if(name == "value"){
                                    value = extractDouble();
                                }
                            }
                        }
                        //check and load
                        if(isOk){
                            parameterSet->setCorrelation(param1,param2,value);
                        }

                    }
                }

            }
            else if (name =="time") {
                /*****************************************
                 * TODO: Check this. It seems to be empty
                 ****************************************/
                // parameterSet->setTime(extractDate());
            }
        }
    }

    checkReaderError();

    return parameterSet;
}

ezechiel::core::Parameter *PredictionSpecImporter::loadParameter(const QString &tagName, QObject *parent)
{
    ezechiel::core::Parameter* parameter = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Parameter>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "name"){
                parameter->setName(extractor());
            } else if (name == "unit") {//FIXME unkown unit in fileout
                QString extract = extractor();
                ezechiel::core::Unit unit(extract);
                bool isValid = unit.isValid();
                parameter->getQuantity()->setUnit(extract);
            } else if (name == "value") {
                parameter->getQuantity()->setValue(extractDouble());
            } else if (name == "bsv") {
                parameter->setBsv(loadBsv("bsv", parameter));
            } else if (name == "min") {
                parameter->setMin(extractDouble());
            } else if (name == "max") {
                parameter->setMax(extractDouble());
            } else if (name == "step") {//FIXME crazy steps in fileout
                parameter->setStep(extractDouble());
            }
            else if (name == "operations") {
                parameter->setOperations(loadOperations("operations", parameter));
            }
        }
    }

    checkReaderError();

    return parameter;
}

ezechiel::core::OperationList *PredictionSpecImporter::loadOperations(const QString &tagName, QObject *parent)
{
    ezechiel::core::OperationList *operations = ezechiel::core::CoreFactory::createEntity<ezechiel::core::OperationList>(ABSTRACTREPO,0);

    QMap<QString,ezechiel::core::OperationList*> mapOperationLists;

    WHILE_NOT_END_ELEM(tagName){
        if(reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "operation"){
                ezechiel::core::Operation* operation = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Operation>(ABSTRACTREPO);
                WHILE_NOT_END_ELEM("operation"){
                    if(reader.readNextStartElement() ){
                        QString name = reader.name().toString();
                        if(name == "type"){
                            QString t = extractor();
                            if (t == "Hardcoded")
                                operation->setType(ezechiel::core::OperationType::HARDCODED);
                            else if (t == "Imported")
                                operation->setType(ezechiel::core::OperationType::IMPORTED);
                            else
                                // TODO: There is an error. It should be reported
                                operation->setType(ezechiel::core::OperationType::NOOP);
                        }
                        else if(name == "formula"){
                            QString formula = extractor();
                            operation->setFormula(formula);
                        }
                    }
                }
                if (isOk)
                    operations->append(operation);
            }
        }
    }

    checkReaderError();

    return operations;
}


ezechiel::core::Bsv *PredictionSpecImporter::loadBsv(const QString &tagName, QObject *parent)
{
    ezechiel::core::Bsv* bsv = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Bsv>(ABSTRACTREPO,parent);

    WHILE_NOT_END_ELEM("bsv"){
        if(isOk && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "additive"){
                bsv->setStandard(extractDouble());
            } else if ( name == "proportional"){
                bsv->setProportional(extractDouble());
            }
        }
    }


    checkReaderError();

    return bsv;
}


QString PredictionSpecImporter::extractor(QXmlStreamAttributes *attributes,bool readText)
{
    QString data;
    if(attributes != nullptr){
        *attributes = reader.attributes();
    }
    if(readText){
        data = reader.readElementText();
    }
    return data;
}

QMetaType::Type PredictionSpecImporter::extractType()
{
    QString s = extractor();
    bool isConvertible = true;
    QMetaType::Type t = qStringToType(s, isConvertible);
    checkValidity(isConvertible);
    return t;
}

QMetaType::Type PredictionSpecImporter::qStringToType(QString stringType,bool &isConvertible)
{//FIXME how many different types are there
    QMetaType::Type type;
    if(stringType == "double"){
        type = QMetaType::Type::Double;
    } else if(stringType == "int"){
        type = QMetaType::Type::Int;
    } else if (stringType == "bool"){
        type = QMetaType::Type::Bool;
    }else{
        isConvertible = false;
    }

    return type;
}


QString PredictionSpecImporter::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(reader.errorString())
            .arg(reader.lineNumber())
            .arg(reader.columnNumber());
}

void PredictionSpecImporter::raiseConversionError()
{
    reader.raiseError("syntaxe error, probably text instead of double");
}


ezechiel::core::IdentifiableAmount * PredictionSpecImporter::loadIdentifiableAmount(const QString &tagName, QObject *parent)
{

    ezechiel::core::IdentifiableAmount* amount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::IdentifiableAmount>(ABSTRACTREPO, parent);
    QString parameter;
    WHILE_NOT_END_ELEM(tagName){
        if(isOk && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "amountId"){
                amount->setAmountId(extractor());
            }else if(name == "value"){
                amount->setDbvalue(extractDouble());
            } else if (name == "unit"){
                ezechiel::core::Unit unit(extractor());
                checkValidity(unit.isValid());
                amount->setUnit(unit);
            }
        }
    }

    checkReaderError();

    return amount;
}

ezechiel::core::OperableAmount *PredictionSpecImporter::loadOperableAmount(const QString &tagName, QObject *parent)
{

    ezechiel::core::OperableAmount* amount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::OperableAmount>(ABSTRACTREPO, parent);

    WHILE_NOT_END_ELEM(tagName){
        if(isOk && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "amountId"){
                amount->setAmountId(extractor());
            }else if(name == "value"){
                amount->setDbvalue(extractDouble());
            } else if (name == "unit"){
                ezechiel::core::Unit unit(extractor());
                checkValidity(unit.isValid());
                amount->setUnit(unit);
            }
            else if (name == "operations") {
                amount->setOperations(loadOperations("operations", amount));
            }
        }
    }

    checkReaderError();

    return amount;
}
