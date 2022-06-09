#include "drugxmlimport.h"


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



#define WHILE_NOT_END_ELEM(elem)     while(!reader.hasError() && (!reader.isEndElement() || reader.name() != #elem))


DrugXmlImport::DrugXmlImport(){}

ezechiel::GuiCore::DrugModel *DrugXmlImport::load(const QString &fileName)
{
    QTextStream out(stdout);

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)){
        out<<"can not open file"<<endl;
        return nullptr;
    }
    
    ezechiel::GuiCore::DrugModel *drug = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugModel>(ABSTRACTREPO);
    
    reader.setDevice(&file);
    reader.readNext();
    bool isOk = true;
    while(!reader.atEnd() && isOk){
        QString name = reader.name().toString();
        
        switch(reader.tokenType()){
        case QXmlStreamReader::StartElement :
            if(name == "history"){
                isOk = loadHistory(drug);
            } else if (name == "drug"){
                isOk = loadDrug(drug);
            } else if(name == "comments"){

                //TODO comments in every tokens

            }
            break;
        default:
            break;
        }
        reader.readNext();
    }
    
    if(reader.hasError() || !isOk){
        out<<"reader error " << errorString() <<endl;
        delete drug;
        drug = nullptr;
    }

    file.close();

    return drug;
}
bool DrugXmlImport::loadHistory(ezechiel::GuiCore::DrugModel *drug)
{
    ezechiel::GuiCore::MetaData* metaData = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::MetaData>(ABSTRACTREPO,drug);
    ezechiel::GuiCore::EditorList* editors = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::EditorList>(ABSTRACTREPO,metaData);
    bool isConvertible = true;
    WHILE_NOT_END_ELEM(history){
        if (isConvertible && reader.readNextStartElement()) {
            QString name = reader.name().toString();
            if (name == "creator") {
                isConvertible = loadEditor(editors);
                metaData->setEditors(editors);
            } else if (name == "revisions") {
                isConvertible = loadRevisions(metaData);
            }
        } else if (!isConvertible){
            raiseConversionError();
        }
    }
    isConvertible = reader.hasError() ? false : isConvertible;

    if(isConvertible){
        drug->setMetaData(metaData);
    }
    return isConvertible;
}

bool DrugXmlImport::loadRevisions(ezechiel::GuiCore::MetaData* metaData)
{
    ezechiel::GuiCore::EditorList *editors = metaData->getEditors();
    bool isConvertible = true;

    WHILE_NOT_END_ELEM(revisions){
        if (isConvertible && reader.readNextStartElement()) {
            QString name = reader.name().toString();
            if(name == "revision"){
                isConvertible = loadEditor(editors);
            }
        } else if(!isConvertible){
            raiseConversionError();
        }
    }
    isConvertible = reader.hasError() ? false : isConvertible;

    return isConvertible;
}

bool DrugXmlImport::loadEditor(ezechiel::GuiCore::EditorList* editors)
{
    ezechiel::GuiCore::Editor *editor = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Editor>(ABSTRACTREPO, editors);
    bool isConvertible = true;

    QString container = reader.name().toString();

    while(!reader.isEndElement() || reader.name() != container){
        if(isConvertible && reader.readNextStartElement()){
            QString name = reader.name().toString();
            if(name == "name"){
                editor->setName(extractor());
            } else if(name == "institution"){
                editor->setInstitution(extractor());
            } else if(name == "email"){
                editor->setEmail(extractor());
            } else if (name == "date"){
                editor->setDate(QDateTime::fromString(extractor()));
            } else if (name == "comments"){
                ezechiel::GuiCore::TranslatableString *comments = commentsLoader(isConvertible);
                editor->setComments(comments);
                comments->setParent(editor);
            }
        } else if(!isConvertible){
            raiseConversionError();
        }
    }
    isConvertible = reader.hasError() ? false : isConvertible;

    if(isConvertible){
        editors->append(editor);
    }
    return isConvertible;
}

bool DrugXmlImport::loadDrug(ezechiel::GuiCore::DrugModel* drug)
{
    bool isOk=true;

    WHILE_NOT_END_ELEM(drug){

        if(isOk && reader.readNextStartElement()){
            QString name = reader.name().toString();

            if(name == "head"){
                isOk = loadHead(drug);
            }else if(name == "adme"){
                isOk = loadAdme(drug);
            }else if(name == "halflife"){
                isOk = loadHalflife(drug);
            }else if(name == "conversions"){
                isOk = loadConversions(drug);
            }else if(name == "standardTreatment"){
                isOk = loadStandardTreatment(drug);
            }else if(name == "dosages"){
                isOk = loadDosages(drug);
            }else if(name == "targets"){
                isOk = loadTargets(drug);
            }else if(name == "covariates"){
                isOk = loadCovariates(drug);
            }else if(name == "errormodel"){
                isOk = loadErrorModel(drug);
            }else if(name == "parameters"){
                isOk = loadParameters(drug);
            }else if(name == "correlations"){
                isOk = loadCorrelations(drug);
            }else if(name == "operations"){
                isOk = loadOperations(drug);
            } else if (name == "comments"){
                ezechiel::GuiCore::TranslatableString *comments = commentsLoader(isOk);
                drug->setGeneralCommentsTranslationMap(commentsLoader(isOk));
                comments->setParent(drug);
            }
        }
    }

    return isOk;
}

#include <iostream>

bool DrugXmlImport::loadHead(ezechiel::GuiCore::DrugModel* drug)
{
    bool isConvertible = true;
    QString drugId, pkModelId, drugModelId, domainName, studyName,description;
    QStringList atcs;
    QStringList brands;
    ezechiel::GuiCore::ReferenceList *references = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ReferenceList>(ABSTRACTREPO,drug->getMetaData());
    QXmlStreamAttributes referencesAttributes;
    QString name;
//    QMap<QString,QString> drugNameTranslation,domainNameTranslation,studyNameTranslation,descriptionTranslation,headComments;
    ezechiel::GuiCore::TranslatableString *drugNameTranslation, *domainNameTranslation,*studyNameTranslation,*descriptionTranslation,*headComments;

    drugNameTranslation = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(ABSTRACTREPO,drug);
    domainNameTranslation = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(ABSTRACTREPO,drug);
    studyNameTranslation = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(ABSTRACTREPO,drug);
    descriptionTranslation = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(ABSTRACTREPO,drug);


    WHILE_NOT_END_ELEM(head) {
        if (isConvertible && reader.readNextStartElement()) {
            name = reader.name().toString();
            if (name == "atc") {
                atcs << extractor();
            } else if (name == "drugid") {
                drugId = extractor();
            } else if (name == "drugmodelid") {
                drugModelId = extractor();
            } else if (name == "pkmodelid") {
                pkModelId = extractor();
            } else if (name == "drugnames") {
                isConvertible = stringLoaderTranslation("name",drugNameTranslation);
            } else if (name == "domainnames") {
                isConvertible = stringLoaderTranslation("name",domainNameTranslation);
            } else if (name == "studynames") {
                isConvertible = stringLoaderTranslation("name",studyNameTranslation);
            } else if(name == "references") {
                WHILE_NOT_END_ELEM(references) {
                    if (isConvertible && reader.readNextStartElement()) {
                        if (reader.name() == "reference") {
                            ezechiel::GuiCore::Reference *reference = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Reference>(ABSTRACTREPO,drug);
                            reference->setText(extractor(&referencesAttributes));
                            reference->setType(referencesAttributes.value("type").toString());
                            references->append(reference);
                        }
                    }

                }
            } else if (name == "descs") {
                isConvertible = stringLoaderTranslation("desc",descriptionTranslation);
            } else if (name == "brands") {
                WHILE_NOT_END_ELEM(brands) {
                    if (isConvertible && reader.readNextStartElement()) {
                        if (reader.name() == "brand") {
                            brands.append(extractor());
                        }
                    }
                }
            } else if (name == "comments"){
                headComments = commentsLoader(isConvertible);
            }
        }
    }

//    std::cout << "Drug ID: " << qPrintable(drugId) << std::endl;

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        // TODO: Really use ActiveSubstance, and
        // Store it in the drugxmlexport.cpp
        drug->getActiveSubstance()->setAtc(atcs);
//        drug->getActiveSubstance()->setMolarMass();
        drug->getActiveSubstance()->setSubstanceId(drugId);
        drug->getActiveSubstance()->setBrands(brands);
        drug->getActiveSubstance()->setName(drugNameTranslation);

        // This should be different after refactoring about this ID
        drug->setDrugModelId(drugModelId);
        drug->setPkModelId(pkModelId);
        drug->setName(drugNameTranslation);
        drug->setDomainNameTranslationMap(domainNameTranslation);
        drug->setStudyNameTranslationMap(studyNameTranslation);
        drug->getMetaData()->setReferences(references);
        drug->setDescriptionsTranslationMap(descriptionTranslation);
        drug->setHeadCommentsTranslationMap(headComments);
        headComments->setParent(drug);
    }

    return isConvertible;
}


bool DrugXmlImport::loadAdme(ezechiel::GuiCore::DrugModel* drug)
{
    ezechiel::GuiCore::ADME* adme = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ADME>(ABSTRACTREPO,drug);

    bool isConvertible=true;
    WHILE_NOT_END_ELEM(adme){
        if( isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "intake"){
//                QList<ezechiel::GuiCore::Admin*> intakes;
                ezechiel::GuiCore::AdminList* intakes = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::AdminList>(ABSTRACTREPO, adme);
                QString intake = extractor();
                if (intake == "bolus") {
                    intakes->add();
                    intakes->last()->setRoute(ezechiel::GuiCore::Admin::BOLUS);
                    // If bolus model, should also be compatible with infusion
                    intakes->add();
                    intakes->last()->setRoute(ezechiel::GuiCore::Admin::INFUSION);
                } else if (intake == "infu") {
                    intakes->add();
                    intakes->last()->setRoute(ezechiel::GuiCore::Admin::INFUSION);
                    // If infusion model, should also be compatible with bolus
                    intakes->add();
                    intakes->last()->setRoute(ezechiel::GuiCore::Admin::BOLUS);
                } else {
                    intakes->add();
                    intakes->last()->setRoute(ezechiel::GuiCore::Admin::EXTRA);
                }
                adme->setIntakes(intakes);

//                adme->setIntakes(intakes);
                ezechiel::GuiCore::Admin* def = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO, adme);
                if (intake == "bolus") {
                    def->setRoute(ezechiel::GuiCore::Admin::BOLUS);
                } else if (intake == "infu") {
                    def->setRoute(ezechiel::GuiCore::Admin::INFUSION);
                } else {
                    def->setRoute(ezechiel::GuiCore::Admin::EXTRA);
                }
                adme->setDefaultIntake(def);
            } else if(name == "distribution"){
                adme->setDistribution(extractor());
            } else if(name == "elimination"){
                adme->setElimination(extractor());
            } else if(name == "comments"){
                ezechiel::GuiCore::TranslatableString *comments = commentsLoader(isConvertible);
                adme->setComments(comments);
                comments->setParent(adme);
            }
        }
    }

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        drug->setAdme(adme);
    }

    return isConvertible;
}
bool DrugXmlImport::loadHalflife(ezechiel::GuiCore::DrugModel* drug)
{
    ezechiel::GuiCore::Halflife* halflife = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Halflife>(ABSTRACTREPO,drug);

    bool isConvertible=true;
    WHILE_NOT_END_ELEM(halflife){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "unit"){
                halflife->setUnit(ezechiel::GuiCore::Unit(extractor()));
            } else if (name == "value"){
                halflife->setValue(extractor().toDouble(&isConvertible));
            } else if (name == "multiplier"){
                halflife->setMultiplier(extractor().toInt(&isConvertible));
            } else if (name == "comments"){
                ezechiel::GuiCore::TranslatableString *comments = commentsLoader(isConvertible);
                halflife->setComments(comments);
                comments->setParent(halflife);
            }
        } else if (!isConvertible){
            raiseConversionError();
        }
    }

    if(isConvertible){
        drug->setHalflife(halflife);
    }

    return isConvertible;
}


bool DrugXmlImport::loadConversions(ezechiel::GuiCore::DrugModel* drug)
{
    ezechiel::GuiCore::Concentrations* concentrations = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Concentrations>(ABSTRACTREPO,drug);
    ezechiel::GuiCore::Results* results = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Results>(ABSTRACTREPO,drug);
    ezechiel::GuiCore::Amount molarMass(0, ezechiel::GuiCore::Unit(""));

    bool isConvertible=true;
    WHILE_NOT_END_ELEM(conversions){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "units"){
                WHILE_NOT_END_ELEM(units){
                    if(reader.readNextStartElement()){
                        name = reader.name().toString();
                        if(name == "concentration"){
                            concentrations->getQuantity().setUnit(ezechiel::GuiCore::Unit(extractor()));
                        } else if (name == "result"){
                            results->setUnit(ezechiel::GuiCore::Unit(extractor()));
                        }
                    }

                }
            } else if(name=="factors"){
                WHILE_NOT_END_ELEM(factors){
                    if(isConvertible && reader.readNextStartElement() ){
                        name = reader.name().toString();
                        if(name == "concentration"){
                            concentrations->getQuantity().setValue(extractor().toDouble(&isConvertible));
                        }else if(name == "result"){
                            results->setFactor(extractor().toDouble(&isConvertible));
                        }
                    } else if (!isConvertible){
                        break;
                    }
                }
            } else if(name == "molarmass"){
                molarMass.setValue(extractor().toDouble(&isConvertible));
            } else if(name == "comments"){
                ezechiel::GuiCore::TranslatableString *comments = commentsLoader(isConvertible);
                drug->setConversionsCommentsTranslationMap(comments);
                comments->setParent(drug);
            }
        } else if (!isConvertible){
            raiseConversionError();
        }
    }

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        drug->setResults(results);
        drug->setConcentrations(concentrations);
        drug->setMolarMass(molarMass);
    }
    return isConvertible;
}


bool DrugXmlImport::loadStandardTreatment(ezechiel::GuiCore::DrugModel* drug)
{

    bool isConvertible = true;
    ezechiel::GuiCore::StandardTreatment* treatment = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::StandardTreatment>(ABSTRACTREPO,drug);

    WHILE_NOT_END_ELEM(standardTreatment){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "isFixedDuration"){
                treatment->setIsFixedDuration(extractor() == "true" ? true : false);
            } else if (name == "duration"){
                QString unit;
                double value;
                //parses the doses
                WHILE_NOT_END_ELEM(duration){
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
                    treatment->setDuration(ezechiel::GuiCore::Duration(value));
                }
                else if (unit == "d") {
                    treatment->setDuration(ezechiel::GuiCore::Duration(24 * value));
                }
                else if (unit == "m") {
                    treatment->setDuration(ezechiel::GuiCore::Duration(((double) (value))/60));
                }
                else {
                    raiseConversionError();
                }
            }
        } else if (!isConvertible){
            raiseConversionError();
        }
    }

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        drug->setStandardTreatment(treatment);
    }
    return isConvertible;
}

bool DrugXmlImport::loadDosages(ezechiel::GuiCore::DrugModel *drug)
{
    ezechiel::GuiCore::ValidDoses* doses = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidDoses>(ABSTRACTREPO,drug);
    ezechiel::GuiCore::ValidIntervals* intervals = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidIntervals>(ABSTRACTREPO,drug);
    ezechiel::GuiCore::ValidInfusions* infusions = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidInfusions>(ABSTRACTREPO,drug);


    bool isConvertible=true;
    WHILE_NOT_END_ELEM(dosages) {
        if(isConvertible && reader.readNextStartElement()) {
            QString name = reader.name().toString();
            if(name == "doses") {
                //parses the attributes
                QXmlStreamAttributes attributes = reader.attributes();
                for(QXmlStreamAttribute attribute : attributes){
                    if(isConvertible) {
                        name = attribute.name().toString();
                        if(name == "default"){
                            doses->getQuantity()->setValue(attribute.value().toDouble(&isConvertible));
                        } else if (name == "unit"){
                            doses->getQuantity()->setUnit(attribute.value().toString());
                        } else if (name == "any"){
                            doses->setAnyDose(attribute.value().toString().toLower() == "true" ? true : false);
                        } else if (name == "from"){
                            doses->getFromDose()->setValue(attribute.value().toDouble(&isConvertible));
                        } else if (name == "to"){
                            doses->getToDose()->setValue(attribute.value().toDouble(&isConvertible));
                        } else if (name == "step"){
                            doses->getStepDose()->setValue(attribute.value().toDouble(&isConvertible));
                        }
                    } else{
                        break;
                    }
                }
                //parses the doses
                WHILE_NOT_END_ELEM(doses){
                    if(isConvertible && reader.readNextStartElement() ){
                        if(reader.name() == "dose"){
                            ezechiel::GuiCore::ValidDose* dose = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidDose>(ABSTRACTREPO,doses);
                            dose->getQuantity()->setUnit(doses->getQuantity()->unit());
                            dose->getQuantity()->setValue(extractor().toDouble(&isConvertible));
                            dose->getRoute()->setRoute(drug->getAdme()->getDefaultIntake()->getRoute());
                            doses->append(dose);
                        }
                    } else {
                        break;
                    }
                }
            } else if(name == "intervals"){
                //parses the attributes
                QXmlStreamAttributes attributes = reader.attributes();
                for(QXmlStreamAttribute attribute : attributes){
                    if(isConvertible){
                        name = attribute.name().toString();
                        if(name == "default"){
                            intervals->getQuantity()->setValue(attribute.value().toDouble(&isConvertible));
                        } else if (name == "unit"){
                            intervals->getQuantity()->setUnit(ezechiel::GuiCore::Unit(attribute.value().toString()));
                        } else if (name == "any"){
                            intervals->setAny(attribute.value().toString().toLower() == "true" ? true : false);
                        }
                    }else{
                        break;
                    }
                }
                //parses the intervals
                WHILE_NOT_END_ELEM(intervals){
                    if(isConvertible && reader.readNextStartElement() ){
                        if(reader.name() == "interval"){
                            ezechiel::GuiCore::ValidInterval* interval = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidInterval>(ABSTRACTREPO,intervals);
                            interval->getQuantity()->setUnit(intervals->getQuantity()->unit().toString());
                            interval->getQuantity()->setValue(extractor().toDouble(&isConvertible));
                            intervals->append(interval);
                        }
                    } else if (!isConvertible){
                        break;
                    }
                }
            } else if(name == "infusions"){
                //parses the attributes
                QXmlStreamAttributes attributes = reader.attributes();
                for(QXmlStreamAttribute attribute : attributes){
                    if(isConvertible){
                        name = attribute.name().toString();
                        if(name == "default"){
                            infusions->getQuantity()->setValue(attribute.value().toDouble(&isConvertible));
                        } else if (name == "unit"){
                            infusions->getQuantity()->setUnit(attribute.value().toString());
                        } else if (name == "any"){
                            infusions->setAny(attribute.value().toString().toLower() == "true" ? true : false);
                        }
                    } else{
                        break;
                    }
                }
                WHILE_NOT_END_ELEM(infusions){
                    if(isConvertible && reader.readNextStartElement() ){
                        if(reader.name() == "infusion"){
                            ezechiel::GuiCore::ValidInfusion* infusion = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidInfusion>(ABSTRACTREPO,infusions);
                            infusion->getQuantity()->setUnit(infusions->getQuantity()->getUnitstring());
                            infusion->getQuantity()->setValue(extractor().toDouble(&isConvertible));
                            infusions->append(infusion);
                        }
                    } else if(!isConvertible){
                        break;
                    }
                }
            } else if (name == "comments"){
                ezechiel::GuiCore::TranslatableString *comments = commentsLoader(isConvertible);
                doses->setComments(comments);
                comments->setParent(doses);
            }
        } else if (!isConvertible){
            raiseConversionError();
        }
    }

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        drug->setDoses(doses);
        drug->setIntervals(intervals);
        drug->setInfusions(infusions);
    }

    return isConvertible;
}

bool DrugXmlImport::loadTargets(ezechiel::GuiCore::DrugModel* drug)
{
    bool isOk = true;
    ezechiel::GuiCore::TargetList* targetSet = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TargetList>(ABSTRACTREPO, drug);
    WHILE_NOT_END_ELEM(targets){
        if(reader.readNextStartElement() && isOk){
            QString name = reader.name().toString();
            if(name == "target"){
                isOk = loadTarget(targetSet);
            }
        }
    }

    isOk = reader.hasError() ? false : isOk;
    if(isOk){
        drug->setTargets(targetSet);
    }

    return isOk;
}

bool DrugXmlImport::loadTarget(ezechiel::GuiCore::TargetList* targetSet)
{
    bool isConvertible = true;
    ezechiel::GuiCore::Target* target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Target>(ABSTRACTREPO, targetSet);

    WHILE_NOT_END_ELEM(target){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "type"){
                QString type = extractor();
                if (type == "residual"){
                    target->getType()->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::ResidualTarget);
                }else  if(type == "peak"){
                    target->getType()->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::PeakTarget);
                } else if(type == "mean"){
                    target->getType()->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::MeanTarget);
                } else if(type == "auc"){
                    target->getType()->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::AUCTarget);
                } else if(type == "cumulativeauc"){
                    target->getType()->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::CumulativeAUCTarget);
                } else if(type == "unknown"){
                    target->getType()->setTargetType(ezechiel::GuiCore::TargetMethod::TargetType::UnknownTarget);
                }
            } else if(name == "concentrations"){
                QXmlStreamAttributes attributes ;
                extractor(&attributes,false);
                ezechiel::GuiCore::Unit unit;
                if(attributes.front().name() == "unit"){
                    unit = ezechiel::GuiCore::Unit(attributes.front().value().toString());
                } else {
                    isConvertible = false;
                }
                WHILE_NOT_END_ELEM(concentrations){
                    if(isConvertible && reader.readNextStartElement() ){
                        name = reader.name().toString();
                        if(name == "min"){
                            reader.readNextStartElement();//implies that value is always the first token to follow
                            if (reader.name().toString() == "value") {
                                target->getCmin()->setValue(extractor().toDouble());
                                target->getCmin()->setUnit(unit);
                            }
                        } else if(name == "max"){
                            reader.readNextStartElement();//implies that value is always the first token to follow
                            if (reader.name().toString() == "value") {
                                target->getCmax()->setValue(extractor().toDouble());
                                target->getCmax()->setUnit(unit);
                            }
                        } else if(name == "best"){
                            reader.readNextStartElement();//implies that value is always the first token to follow
                            if (reader.name().toString() == "value") {
                                target->getCbest()->setValue(extractor().toDouble());
                                target->getCbest()->setUnit(unit);
                            }
                        }
                    } else if (!isConvertible){
                        break;
                    }
                }
            } else if(name == "times"){
                QXmlStreamAttributes attributes ;
                extractor(&attributes,false);
                ezechiel::GuiCore::Unit unit;
                if(attributes.front().name() == "unit"){
                    unit = ezechiel::GuiCore::Unit(attributes.front().value().toString());
                } else {
                    isConvertible = false;
                }
                WHILE_NOT_END_ELEM(times){
                    if(isConvertible && reader.readNextStartElement() ){
                        name = reader.name().toString();
                        if(name == "min"){
                            reader.readNextStartElement();//implies that value is always the first token to follow
                            target->getTmin()->setValue(extractor().toDouble(&isConvertible));
                            target->getTmin()->setUnit(unit);
                        } else if(name == "max"){
                            reader.readNextStartElement();//implies that value is always the first token to follow
                            target->getTmax()->setValue(extractor().toDouble(&isConvertible));
                            target->getTmax()->setUnit(unit);
                        } else if(name == "best"){
                            reader.readNextStartElement();//implies that value is always the first token to follow
                            target->getTbest()->setValue(extractor().toDouble(&isConvertible));
                            target->getTbest()->setUnit(unit);
                        }
                    } else if(!isConvertible){
                        break;
                    }
                }
            } else if(name == "comments"){
                ezechiel::GuiCore::TranslatableString *comments = commentsLoader(isConvertible);
                target->setComments(comments);
                comments->setParent(target);
            }
        } else if (!isConvertible){
            raiseConversionError();
        }
    }

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        targetSet->append(target);
    }
    return isConvertible;
}

bool DrugXmlImport::loadCovariates(ezechiel::GuiCore::DrugModel* drug)
{
    bool isOk = true;
    ezechiel::GuiCore::DrugVariateList *drugVariateSet = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugVariateList>(ABSTRACTREPO, drug);

    WHILE_NOT_END_ELEM(covariates){
        if(reader.readNextStartElement() && isOk){
            isOk = loadCovariate(drugVariateSet);
        }
    }
    isOk = reader.hasError() ? false : isOk;
    if(isOk){
        drug->setCovariates(drugVariateSet);
    }
    return isOk;
}

#include <iostream>

bool DrugXmlImport::loadCovariate(ezechiel::GuiCore::DrugVariateList *drugVariateSet)
{
    bool isConvertible = true;
    ezechiel::GuiCore::DrugVariate* drugVariate = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugVariate>(ABSTRACTREPO,drugVariateSet);

    ezechiel::GuiCore::TranslatableString *nameTranslation, *descriptionTranslation;

    nameTranslation = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(ABSTRACTREPO,drugVariate);
    descriptionTranslation = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(ABSTRACTREPO,drugVariate);

    WHILE_NOT_END_ELEM(covariate){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "id"){
                drugVariate->setCovariateId(extractor());
            } else if(name == "names"){
                isConvertible = stringLoaderTranslation("name",nameTranslation);
                drugVariate->setVisualNameTranslation(nameTranslation);
            }else if(name == "descs"){
                isConvertible = stringLoaderTranslation("desc",descriptionTranslation);
                drugVariate->setDescriptionTranslation(descriptionTranslation);
            } else if (name == "unit"){
                QString s = extractor();
//                std::cout << "Raw Unit: \"" << qPrintable(s) << "\"" << std::endl;
                drugVariate->getQuantity()->setUnit(s);
            } else if (name == "type"){
                drugVariate->setType(qStringToType(extractor(),isConvertible));
            }else if (name == "defaultvalue"){
                drugVariate->getQuantity()->setValue(extractor().toDouble());
            } else if (name == "comments"){
                ezechiel::GuiCore::TranslatableString *comments = commentsLoader(isConvertible);
                drugVariate->setComments(comments);
                comments->setParent(drugVariate);
            }
        } else if (!isConvertible){
            raiseConversionError();
        }
    }


//    std::cout << "Unit: \"" << qPrintable(drugVariate->getQuantity()->getUnitstring()) << "\"" << std::endl;
//    std::cout << "Unit: \"" << qPrintable(drugVariate->getQuantity()->unit().name()) << "\"" << std::endl;
//    std::cout << "Unit: \"" << qPrintable(drugVariate->getQuantity()->unit().toString()) << "\"" << std::endl;


    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        drugVariateSet->append(drugVariate);
    }
    return isConvertible;
}

bool DrugXmlImport::loadErrorModel(ezechiel::GuiCore::DrugModel* drug)
{
    bool isConvertible = true;
    ezechiel::GuiCore::ErrorModel* errorModel = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ErrorModel>(ABSTRACTREPO,drug);

    WHILE_NOT_END_ELEM(errormodel){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "additive"){
                errorModel->setAdditive(extractor().toDouble(&isConvertible));
            } else if (name == "proportional"){
                errorModel->setProportional(extractor().toDouble(&isConvertible));
            } else if (name == "comments"){
                ezechiel::GuiCore::TranslatableString *comments = commentsLoader(isConvertible);
                errorModel->setComments(comments);
                comments->setParent(errorModel);
            }
        } else if (!isConvertible){
            raiseConversionError();
        }
    }

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        drug->setErrorModel(errorModel);
    }
    return isConvertible;
}

bool DrugXmlImport::loadParameters(ezechiel::GuiCore::DrugModel* drug)
{
    bool isConvertible = true;

    ezechiel::GuiCore::ParameterSet* parameterSet = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ParameterSet>(ABSTRACTREPO,drug);

    WHILE_NOT_END_ELEM(parameters){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "parameter"){
                isConvertible = loadParameter(parameterSet);
            }
        }
    }

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        drug->setParameters(parameterSet);
    }
    return isConvertible;
}

bool DrugXmlImport::loadParameter(ezechiel::GuiCore::ParameterSet* parameterSet)
{//FIXME few bugs appeared
    bool isConvertible = true;
    ezechiel::GuiCore::Parameter* parameter = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Parameter>(ABSTRACTREPO,parameterSet);

    WHILE_NOT_END_ELEM(parameter){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "id"){
                parameter->setName(extractor());
            } else if (name == "unit") {//FIXME unkown unit in fileout
                QString extract = extractor();
                ezechiel::GuiCore::Unit unit(extract);
                bool isValid = unit.isValid();
                parameter->getQuantity()->setUnit(extract);
            } else if (name == "value") {
                parameter->getQuantity()->setValue(extractor().toDouble(&isConvertible));
            } else if (name == "bsv") {
                isConvertible = loadBsv(parameter);
            } else if (name == "min") {
                parameter->setMin(extractor().toDouble(&isConvertible));
            } else if (name == "max") {
                parameter->setMax(extractor().toDouble(&isConvertible));
            } else if (name == "step") {//FIXME crazy steps in fileout
                QString extract = extractor();
                double dou = extract.toDouble(&isConvertible);
                parameter->setStep(dou);
            } else if (name == "comments") {
                ezechiel::GuiCore::TranslatableString *comments = commentsLoader(isConvertible);
                parameter->setComments(comments);
                comments->setParent(parameter);
            }
        } else if(!isConvertible) {
            raiseConversionError();
        }
    }

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        parameterSet->set(parameter);
    }

    return isConvertible;
}

bool DrugXmlImport::loadBsv(ezechiel::GuiCore::Parameter* parameter)
{
    bool isConvertible = true;
    ezechiel::GuiCore::Bsv* bsv = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Bsv>(ABSTRACTREPO,parameter);

    WHILE_NOT_END_ELEM(bsv){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "additive"){
                bsv->setStandard(reader.readElementText().toDouble(&isConvertible));
            } else if ( name == "proportional"){
                bsv->setProportional(reader.readElementText().toDouble(&isConvertible));
            }
        } else if (!isConvertible){
            raiseConversionError();
        }
    }

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        parameter->setBsv(bsv);
    }

    return isConvertible;
}

bool DrugXmlImport::loadCorrelations(ezechiel::GuiCore::DrugModel *drug)
{
    bool isConvertible= true;
    ezechiel::GuiCore::ParameterSet* parameterSet = drug->getParameters();
    QString param1, param2;
    double value = 0;
    ezechiel::GuiCore::TranslatableString *comments;
    WHILE_NOT_END_ELEM(correlations){
        if(isConvertible && reader.readNextStartElement()  && reader.name() == "correlation"){
            WHILE_NOT_END_ELEM(correlation){
                if(isConvertible && reader.readNextStartElement() ){
                    QString name = reader.name().toString();
                    if(name == "param1"){
                        param1 = reader.readElementText();
                    } else if (name == "param2"){
                        param2 = reader.readElementText();
                    } else if(name == "value"){
                        value = reader.readElementText().toDouble(&isConvertible);
                    } else if (name == "comments"){
                        comments = commentsLoader(isConvertible);
                    }
                } else if (!isConvertible){
                    raiseConversionError();
                }
            }
            //check and load
            isConvertible = reader.hasError() ? false : isConvertible;
            if(isConvertible){
                parameterSet->setCorrelation(param1,param2,value);
                parameterSet->setCorrelationComments(param1,param2,comments);
            }

        }
    }

    return isConvertible;
}

bool DrugXmlImport::loadOperations(ezechiel::GuiCore::DrugModel* drug)
{
    bool isConvertible = true;
    QMap<QString,ezechiel::GuiCore::OperationList*> mapOperationLists;

    WHILE_NOT_END_ELEM(operations){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "operation"){
                isConvertible = loadOperation(mapOperationLists);
            }
        }
    }

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        for(QString pid : drug->getParameters()->pids()){
            if(mapOperationLists.contains(pid)){
                drug->getParameters()->get(pid)->setOperations(mapOperationLists.value(pid));
                mapOperationLists.value(pid)->setParent(drug->getParameters()->get(pid));
            } else {
                drug->getParameters()->get(pid)->setOperations(ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::OperationList>(ABSTRACTREPO,drug->getParameters()->get(pid)));
            }
        }
    }


    return isConvertible;
}

bool DrugXmlImport::loadOperation(QMap<QString,ezechiel::GuiCore::OperationList*> &mapOperationList) /// no parents set
{

    ezechiel::GuiCore::Operation* operation = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Operation>(ABSTRACTREPO);
    bool isConvertible = true;
    QString parameter;
    WHILE_NOT_END_ELEM(operation){
        if(isConvertible && reader.readNextStartElement() ){
            QString name = reader.name().toString();
            if(name == "parameter"){
                parameter = extractor();
            }else if(name == "formula"){
                QString formula = extractor();
                operation->setFormula(formula);
            } else if (name == "comments"){
                ezechiel::GuiCore::TranslatableString *comments = commentsLoader(isConvertible);
                operation->setComments(comments);
                comments->setParent(operation);
            }
        }
    }

    isConvertible = reader.hasError() ? false : isConvertible;
    if(isConvertible){
        ezechiel::GuiCore::OperationList* operationList;
        operation->setType(ezechiel::GuiCore::OperationType::IMPORTED);
        if(mapOperationList.contains(parameter)){
            operationList = mapOperationList.take(parameter);
        }else{
            operationList = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::OperationList>(ABSTRACTREPO);
        }
        operationList->append(operation);
        mapOperationList.insert(parameter,operationList);
    }
    return isConvertible;
}


bool DrugXmlImport::stringLoaderTranslation(QString elemName,ezechiel::GuiCore::TranslatableString *QMapStringTranslation)
{
    bool isOk= true;
    QString container = reader.name().toString();

    while(!reader.isEndElement() || reader.name() != container){
        if(reader.readNextStartElement() && reader.name() == elemName){
            QString key =reader.attributes().value("lang").toString() ;
            QMapStringTranslation->insert(key,extractor());
            //            QMapStringTranslation.insert(reader.attributes().value("lang").toString(),reader.readElementText());
        }
    }

    return isOk;
}

ezechiel::GuiCore::TranslatableString* DrugXmlImport::commentsLoader(bool &isOk){

    ezechiel::GuiCore::TranslatableString *comments = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(ABSTRACTREPO); //no parents
    isOk = stringLoaderTranslation("comment",comments);


    return comments;
}

QString DrugXmlImport::extractor(QXmlStreamAttributes *attributes,bool readText)
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

QMetaType::Type DrugXmlImport::qStringToType(QString stringType,bool &isConvertible)
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


QString DrugXmlImport::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(reader.errorString())
            .arg(reader.lineNumber())
            .arg(reader.columnNumber());
}

void DrugXmlImport::raiseConversionError()
{
    reader.raiseError("syntaxe error, probably text instead of double");
}
