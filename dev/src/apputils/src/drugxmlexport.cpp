//#include "drugxmlexport.h"

//DrugXmlExport::DrugXmlExport()
//{
//}

//bool DrugXmlExport::save(Tucuxi::Gui::Core::DrugModel *drug, const QString &fileName)
//{

//    QTextStream out(stdout);
//    // To be implemented with QXmlStreamWriter
//    QFile file(fileName);
//    if(!file.open(QFile::WriteOnly)){
//        out<<"can not open file"<<Qt::endl;
//        return false;
//    }
//    bool isOk = true;
//    writer.setDevice(&file);
//    writer.setAutoFormatting(true);
//    writer.writeStartDocument("1.0",false);
//    writer.writeProcessingInstruction("xml-stylesheet", "href=\"drugsmodel.xsl\" type=\"text/xsl\"");


//    QString namespaceUri = "http://www.w3.org/2001/XMLSchema-instance";
//    writer.writeStartElement("model");
//    writer.writeNamespace(namespaceUri,"xsi");
//    writer.writeAttribute("version","0.6");
//    writer.writeAttribute(namespaceUri, "noNamespaceSchemaLocation", "drug.xsd");

//    saveHistory(drug);
//    writeNewLine();
//    saveDrug(drug);


//    writer.writeEndElement();//model

//    writer.writeEndDocument();
//    file.close();
//    return isOk;
//}

//bool DrugXmlExport::saveHistory(const Tucuxi::Gui::Core::DrugModel *drug)
//{
//    bool isOk = true;

//    Tucuxi::Gui::Core::EditorList *editors = drug->getMetaData()->getEditors();
//    writer.writeComment(" Drug history ");
//    writeNewLine();
//    writer.writeStartElement("history");
//    writer.writeStartElement("creator");
//    saveEditor(editors->first());
//    writer.writeEndElement();//creator
//    writer.writeStartElement("revisions");
//    for (int i = 1; i < editors->size(); i++) {
//        Tucuxi::Gui::Core::Editor *editor = editors->at(i);
//        saveEditor(editor);
//        writer.writeEndElement();//revision
//    }


//    writer.writeEndElement();//revisions
//    writer.writeEndElement();//history
//    return isOk;
//}

//bool DrugXmlExport::saveEditor(const Tucuxi::Gui::Core::Editor *editor){
//    bool isOk = true;

//    tagInjector("name",editor->getName());
//    tagInjector("institution",editor->getInstitution());
//    tagInjector("email", editor->getEmail());
//    tagInjector("date", editor->getDate().toString());

//    writer.writeStartElement("comments");
//    stringSaverTranslation(editor->getComments(),"comment");
//    writer.writeEndElement();//comments

//    return isOk;
//}

//bool DrugXmlExport::saveDrug(const Tucuxi::Gui::Core::DrugModel *drug)
//{
//    bool isOk = true;
//    writer.writeComment(" Drug data ");
//    writeNewLine();
//    writer.writeStartElement("drug");
////    writeNewLine();
//    isOk = isOk ? saveHead(drug) : false;
//    writeNewLine();
//    isOk = isOk ? saveAdme(drug->getAdme()) : false;
//    writeNewLine();
//    isOk = isOk ? saveHalflife(drug->getHalflife()) : false;
//    writeNewLine();
//    isOk = isOk ? saveConversions(drug) : false;
//    writeNewLine();
//    isOk = isOk ? saveStandardTreatment(drug) : false;
//    writeNewLine();
//    isOk = isOk ? saveDosages(drug) : false;
//    writeNewLine();
//    isOk = isOk ? saveTargets(drug->getTargets()) : false;
//    writeNewLine();
//    isOk = isOk ? saveCovariates(drug->getCovariates()) : false;
//    writeNewLine();
//    isOk = isOk ? saveErrormodel(drug->getErrorModel()) : false;
//    writeNewLine();
//    isOk = isOk ? saveParameters(drug->getParameters()) : false;
//    writeNewLine();
//    isOk = isOk ? saveCorrelations(drug) : false;
//    writeNewLine();
//    isOk = isOk ? saveOperations(drug) : false; //TODO still refactoring operations
//    writeNewLine();
//    if(drug->getGeneralCommentsTranslationMap()->size() > 0) {
//        writer.writeComment(" Drug general comments ");
//        writeNewLine();
//        writer.writeStartElement("comments");
//        stringSaverTranslation(drug->getGeneralCommentsTranslationMap(),"comment");
//        writer.writeEndElement();//comments
//        writeNewLine();
//    }

//    writer.writeEndElement();//drug
//    return isOk;
//}

//bool DrugXmlExport::saveHead(const Tucuxi::Gui::Core::DrugModel *drug)
//{
//    bool isOk = true;
//    writer.writeComment(" Drug description ");
//    writeNewLine();

//    writer.writeStartElement("head");

//    // Here we only write a single ATC. To be modified when XML will change
//    //writer.writeTextElement("atc",drug->getAtc().at(0));
//    writer.writeTextElement("atc",drug->getActiveSubstance()->getAtc().at(0));

//    // This should be a drugModelId
//    writer.writeTextElement("drugid",drug->getActiveSubstance()->getSubstanceId());
//    writer.writeTextElement("drugmodelid",drug->getDrugModelId());
//    writer.writeTextElement("pkmodelid",drug->getPkModelId());

//    writer.writeStartElement("drugnames");
//    stringSaverTranslation(drug->getName(),"name");
//    writer.writeEndElement();//drugnames

//    writer.writeStartElement("domainnames");
//    stringSaverTranslation(drug->getDomainNameTranslationMap(),"name");
//    writer.writeEndElement();//domainnames

//    writer.writeStartElement("studynames");
//    stringSaverTranslation(drug->getStudyNameTranslationMap(),"name");
//    writer.writeEndElement();//studynames

//    writer.writeStartElement("references");
//    //FIXME once references are good
//    for(Tucuxi::Gui::Core::Reference *reference : drug->getMetaData()->getReferences()->getList()) {
//        writer.writeStartElement("reference");
//        if(!reference->getType().isEmpty()){
//            writer.writeAttribute("type",reference->getType());
//        }
//        writer.writeCharacters(reference->getText());
//        writer.writeEndElement();//reference
//    }
//    writer.writeEndElement();//references

//    writer.writeStartElement("descs");
//    stringSaverTranslation(drug->getDescriptionsTranslationMap(),"desc");
//    writer.writeEndElement();//descs

//    writer.writeStartElement("brands");
//    for(QString brand : drug->getActiveSubstance()->getBrands()){
//        tagInjector("brand",brand);
//    }
//    writer.writeEndElement();//brands

//    writer.writeStartElement("comments");
//    stringSaverTranslation(drug->getHeadCommentsTranslationMap(),"comment");
//    writer.writeEndElement();//comments

//    writer.writeEndElement();//head
//    return isOk;
//}

//bool DrugXmlExport::saveAdme(const Tucuxi::Gui::Core::ADME *adme)
//{
//    bool isOk = true;
//    writer.writeComment(" Drug adme and intake ");
//    writeNewLine();

//    writer.writeStartElement("adme");

//    if (adme->getDefaultIntake()->getRoute() == Tucuxi::Gui::Core::Admin::BOLUS) {
//        tagInjector("intake","bolus");
//    } else if (adme->getDefaultIntake()->getRoute() == Tucuxi::Gui::Core::Admin::INFUSION) {
//        tagInjector("intake","infu");
//    } else {
//        tagInjector("intake","extra");
//    }
//    tagInjector("distribution",adme->getDistribution());
//    tagInjector("elimination",adme->getElimination());

//    writer.writeStartElement("comments");
//    stringSaverTranslation(adme->getComments(),"comment");
//    writer.writeEndElement();//comments

//    writer.writeEndElement();//adme
//    return isOk;
//}

//bool DrugXmlExport::saveHalflife(const Tucuxi::Gui::Core::Halflife *halflife)
//{
//    bool isOk = true;
//    writer.writeComment(" Drug half-life ");
//    writeNewLine();

//    writer.writeStartElement("halflife");
//    tagInjector("unit",halflife->getUnit().name());
//    tagInjector("value", QString::number(halflife->getValue()));
//    tagInjector("multiplier",QString::number(halflife->getMultiplier()));

//    writer.writeStartElement("comments");
//    stringSaverTranslation(halflife->getComments(), "comment");
//    writer.writeEndElement();//comments

//    writer.writeEndElement();//halflife
//    return isOk;
//}


//bool DrugXmlExport::saveConversions(const Tucuxi::Gui::Core::DrugModel *drug)
//{
//    bool isOk = true;
//    writer.writeComment(" Drug units and conversions ");
//    writeNewLine();

//    writer.writeStartElement("conversions");

//    writer.writeStartElement("units");
//    tagInjector("concentration",drug->getConcentrations()->getQuantity().unit().name());
//    tagInjector("result", drug->getResults()->getUnit().name());
//    writer.writeEndElement();//units

//    writer.writeStartElement("factors");
//    tagInjector("concentration",QString::number(drug->getConcentrations()->getQuantity().value()));
//    tagInjector("result", QString::number(drug->getResults()->getFactor()));
//    writer.writeEndElement();//factors

//    tagInjector("molarmass",QString::number(drug->getMolarMass().value()));

//    writer.writeStartElement("comments");
//    stringSaverTranslation(drug->getConversionsCommentsTranslationMap(),"comment");
//    writer.writeEndElement();//comments

//    writer.writeEndElement();//conversions

//    return isOk;
//}

//bool DrugXmlExport::saveStandardTreatment(const Tucuxi::Gui::Core::DrugModel *drug)
//{
//    Tucuxi::Gui::Core::StandardTreatment *treatment = drug->getStandardTreatment();

//    bool isOk = true;
//    writer.writeComment(" Standard treatment ");
//    writeNewLine();
//    writer.writeStartElement("standardTreatment");
//    writer.writeTextElement("isFixedDuration", treatment->getIsFixedDuration() ? "true" : "false");
//    writer.writeStartElement("duration");
//    writer.writeTextElement("unit", "m");
//    writer.writeTextElement("value", QString::number(treatment->getDuration().toMinutes()));
//    writer.writeEndElement(); // duration
//    writer.writeEndElement(); // standardTreatment

//    return isOk;
//}

//bool DrugXmlExport::saveDosages(const Tucuxi::Gui::Core::DrugModel *drug)
//{
//    bool isOk = true;
//    writer.writeComment(" Drug dosages ");
//    writeNewLine();
//    writer.writeStartElement("dosages");

//    writer.writeStartElement("doses");

//    Tucuxi::Gui::Core::ValidDoses *doses = drug->getDoses();
//    QXmlStreamAttributes dosesAttributes;

//    dosesAttributes.append("default", QString::number(doses->getQuantity()->value()));
//    dosesAttributes.append("unit", doses->getQuantity()->unit().name());
//    dosesAttributes.append("any",doses->getAnyDose() ? "true" : "false");
//    if (doses->getAnyDose()) {
//        dosesAttributes.append("from", QString::number(doses->getFromDose()->value()));
//        dosesAttributes.append("to", QString::number(doses->getToDose()->value()));
//        dosesAttributes.append("step", QString::number(doses->getStepDose()->value()));
//    }
//    writer.writeAttributes(dosesAttributes);

//    for(double dose: doses->doses()) {
//        tagInjector("dose",QString::number(dose));
//    }

//    writer.writeEndElement();//doses
//    writer.writeStartElement("intervals");

//    Tucuxi::Gui::Core::ValidIntervals *intervals = drug->getIntervals();
//    QXmlStreamAttributes intervalAttributes;
//    intervalAttributes.append("default", QString::number(intervals->getQuantity()->value()));
//    intervalAttributes.append("unit", intervals->getQuantity()->unit().name());
//    intervalAttributes.append("any",intervals->getAny() ? "true" : "false");
//    writer.writeAttributes(intervalAttributes);

//    for(double interval : intervals->intervals()) {
//        tagInjector("interval", QString::number(interval));
//    }

//    writer.writeEndElement();//intervals
//    writer.writeStartElement("infusions");

//    Tucuxi::Gui::Core::ValidInfusions *infusions = drug->getInfusions();
//    QXmlStreamAttributes infusionAttributes;
//    infusionAttributes.append("default", QString::number(infusions->getQuantity()->getDbvalue()));
//    infusionAttributes.append("unit", infusions->getQuantity()->getUnitstring());
//    infusionAttributes.append("any",infusions->getAny() ? "true" : "false");
//    writer.writeAttributes(infusionAttributes);

//    for(double infusion : infusions->infusions()) {
//        tagInjector("infusion", QString::number(infusion));
//    }

//    writer.writeEndElement();//infusions

//    writer.writeStartElement("comments");
//    stringSaverTranslation(doses->getComments(),"comment");
//    writer.writeEndElement();//comments

//    writer.writeEndElement();//dosages

//    return isOk;
//}

//bool DrugXmlExport::saveTargets(Tucuxi::Gui::Core::TargetList* targets)
//{
//    bool isOk = true;

//    writer.writeComment(" Drug targets ");
//    writeNewLine();

//    writer.writeStartElement("targets");
//    for(Tucuxi::Gui::Core::Target *target : targets->getList()){
//        writer.writeStartElement("target");

//        switch(target->getType()->getTargetType()){
//        case Tucuxi::Gui::Core::TargetMethod::TargetType::ResidualTarget :
//            tagInjector("type","residual");
//            break;
//        case Tucuxi::Gui::Core::TargetMethod::TargetType::PeakTarget :
//            tagInjector("type","peak");
//            break;
//        case  Tucuxi::Gui::Core::TargetMethod::TargetType::MeanTarget :
//            tagInjector("type","mean");
//            break;
//        case  Tucuxi::Gui::Core::TargetMethod::TargetType::AUCTarget :
//            tagInjector("type","auc");
//            break;
//        case  Tucuxi::Gui::Core::TargetMethod::TargetType::CumulativeAUCTarget :
//            tagInjector("type","cumulativeauc");
//            break;
//        case Tucuxi::Gui::Core::TargetMethod::TargetType::UnknownTarget :
//            tagInjector("type", "unknown");
//            break;
//        // No default, as we cover every possible state
//        //default :
//        //    break;
//        }

//        writer.writeStartElement("concentrations");

//        //TODO: target object changed, need to update for units
////        writer.writeAttribute("unit",target->getCunit().name());

//        writer.writeStartElement("min");
//        tagInjector("value",QString::number(target->getCmin()->value()));
//        tagInjector("formula","");//TODO YMI: formula not taken into account yet
//        writer.writeEndElement();//min

//        writer.writeStartElement("max");
//        tagInjector("value",QString::number(target->getCmax()->value()));
//        tagInjector("formula","");
//        writer.writeEndElement();//max

//        writer.writeStartElement("best");
//        tagInjector("value",QString::number(target->getCbest()->value()));
//        tagInjector("formula","");
//        writer.writeEndElement();//best

//        writer.writeEndElement();//concentrations

//        writer.writeStartElement("times");

////        writer.writeAttribute("unit", target->getTunit().name());

//        writer.writeStartElement("min");
//        tagInjector("value",QString::number(target->getTmin()->value()));
//        tagInjector("formula","");//TODO formulas
//        writer.writeEndElement();//min

//        writer.writeStartElement("max");
//        tagInjector("value",QString::number(target->getTmax()->value()));
//        tagInjector("formula","");
//        writer.writeEndElement();//max

//        writer.writeStartElement("best");
//        tagInjector("value",QString::number(target->getTbest()->value()));
//        tagInjector("formula","");
//        writer.writeEndElement();//best


//        writer.writeEndElement();//times

//        writer.writeStartElement("comments");
//        stringSaverTranslation(target->getComments(),"comment");
//        writer.writeEndElement();//comments

//        writer.writeEndElement();//target
//    }


//    writer.writeEndElement();//targets

//    return isOk;
//}

//bool DrugXmlExport::saveCovariates(Tucuxi::Gui::Core::DrugVariateList * covariates)
//{
//    bool isOk = true;

//    writer.writeComment(" Drug covariates ");
//    writeNewLine();

//    writer.writeStartElement("covariates");

//    for(Tucuxi::Gui::Core::DrugVariate* covariate : covariates->getList()){
//        writer.writeStartElement("covariate");
//        tagInjector("id",covariate->getCovariateId());

//        writer.writeStartElement("names");
//        stringSaverTranslation(covariate->getVisualNameTranslation(),"name");
//        writer.writeEndElement();//names

//        writer.writeStartElement("descs");
//        stringSaverTranslation(covariate->getDescriptionTranslation(),"desc");
//        writer.writeEndElement();//descs

//        tagInjector("unit",covariate->getQuantity()->unit().name());
//        tagInjector("type",typeToQString(covariate->getType(),isOk)); //FIXME gotta create some typeToString method analog to the one in drugxmlimport.cpp
//        tagInjector("defaultvalue",QString::number(covariate->getQuantity()->value()));

//        writer.writeStartElement("comments");
//        stringSaverTranslation(covariate->getComments(),"comment");
//        writer.writeEndElement();//comments

//        writer.writeEndElement();//covariate
//    }


//    writer.writeEndElement();//covariates
//    return isOk;
//}

//bool DrugXmlExport::saveErrormodel(const Tucuxi::Gui::Core::ErrorModel* errorModel)
//{
//    bool isOk = true;

//    writer.writeComment(" Drug error model ");
//    writeNewLine();

//    writer.writeStartElement("errormodel");

//    tagInjector("additive",QString::number(errorModel->getAdditive()));
//    tagInjector("proportional",QString::number(errorModel->getProportional()));

//    writer.writeStartElement("comments");
//    stringSaverTranslation(errorModel->getComments(),"comment");
//    writer.writeEndElement();//comments

//    writer.writeEndElement();//errormodel
//    return isOk;
//}

//bool DrugXmlExport::saveParameters(const Tucuxi::Gui::Core::ParameterSet *parameters)
//{
//    bool isOk = true;

//    writer.writeComment(" Drug parameters ");
//    writeNewLine();

//    writer.writeStartElement("parameters");

//    for(QString paramId : parameters->pids()){

//        Tucuxi::Gui::Core::Parameter *parameter = parameters->get(paramId);
//        writer.writeStartElement("parameter");

//        tagInjector("id",parameter->getName());
//        tagInjector("unit",parameter->getQuantity()->unit().name());
//        tagInjector("value",QString::number(parameter->getQuantity()->value()));

//        Tucuxi::Gui::Core::Bsv *bsv = parameter->getBsv();
//        writer.writeStartElement("bsv");
//        tagInjector("additive",QString::number(bsv->getStandard()));
//        tagInjector("proportional",QString::number(bsv->getProportional()));
//        writer.writeEndElement();//bsv

//        tagInjector("min",QString::number(parameter->getMin()));
//        tagInjector("max",QString::number(parameter->getMax()));
//        double getStep = parameter->getStep();
//        QString step = QString::number(getStep);
//        tagInjector("step",step);

//        writer.writeStartElement("comments");

//        stringSaverTranslation(parameter->getComments(),"comment");
//        writer.writeEndElement();//comments

//        writer.writeEndElement();//parameter
//    }

//    writer.writeEndElement();//parameters

//    return isOk;
//}

//bool DrugXmlExport::saveCorrelations(const Tucuxi::Gui::Core::DrugModel *drug)
//{
//    bool isOk = true;

//    writer.writeComment(" Drug correlations ");
//    writeNewLine();

//    writer.writeStartElement("correlations");


//    for(QString key : drug->getParameters()->correlationsKeys()){
//        writer.writeStartElement("correlation");
//        QStringList keys = key.split("__");
//        QString param1 = keys.at(0),
//                param2 = keys.at(1);

//        for (int i = 0; i < keys.size(); ++i) {
//            tagInjector("param" + QString::number(i+1),keys.at(i));
//        }
//        tagInjector("value", QString::number(drug->getParameters()->correlation(param1,param2)));
//        //TODO comments

//        writer.writeStartElement("comments");
//        stringSaverTranslation(drug->getParameters()->correlationComments(param1,param2),"comment");
//        writer.writeEndElement();//comments

//        writer.writeEndElement();//correlation
//    }

//    writer.writeEndElement();//correlations


//    return isOk;
//}

//bool DrugXmlExport::saveOperations(const Tucuxi::Gui::Core::DrugModel *drug)
//{
//    bool isOk = true;
//    writer.writeComment(" Drug operations ");
//    writeNewLine();

//    writer.writeStartElement("operations");

//    for(QString paramPids : drug->getParameters()->pids()){

//        Tucuxi::Gui::Core::Parameter *parameter = drug->getParameters()->get(paramPids);
//        Tucuxi::Gui::Core::OperationList *operations = parameter->getOperations();


//        for(Tucuxi::Gui::Core::Operation *operation : operations->getList()){

//            writer.writeStartElement("operation");
//            tagInjector("parameter",parameter->getName());

//            writer.writeStartElement("formula");
//            writer.writeCDATA(operation->getFormula());
//            writer.writeEndElement();//formula


//            writer.writeStartElement("comments");
//            stringSaverTranslation(operation->getComments(),"comment");
//            writer.writeEndElement();//comments

//            writer.writeEndElement();//operation
//        }


//    }
//    writer.writeEndElement();//operations
//    return isOk;
//}

//bool DrugXmlExport::tagInjector(const QString tagName, QString text, QXmlStreamAttributes *attributes){

//    writer.writeStartElement(tagName);

//    if(attributes != nullptr){
//        writer.writeAttributes(*attributes);
//    }
//    if(text.size() > 0){
//        //got to split the text as QXmlStreamWriter escapes forbiden characters such as `"`
//        QStringList textSplitted = text.split('"');

//        while (!textSplitted.isEmpty()) {
//            writer.writeCharacters(textSplitted.takeFirst());
//            if(!textSplitted.isEmpty()) {
//                writer.device()->putChar('"');
//            }
//        }
//    }

//    writer.writeEndElement();//tagName
//    return true;
//}

//bool DrugXmlExport::stringSaverTranslation(Tucuxi::Gui::Core::TranslatableString *QMapStringTranslation,QString elemName)
//{
//    for(QString langKey : QMapStringTranslation->keys()){
//        QXmlStreamAttributes attributes;

//        if(!langKey.isEmpty()) {
//            attributes.append("lang",langKey);
//        }
//        tagInjector(elemName,QMapStringTranslation->value(langKey),&attributes);

//    }
//    return true;
//}

//QString DrugXmlExport::typeToQString(QMetaType::Type type,bool &isConvertible)
//{
//    QString stringType;

//    if(type == QMetaType::Type::Double){
//        stringType = "double";
//    } else if(type == QMetaType::Type::Int){
//        stringType = "int";
//    } else if (type ==  QMetaType::Type::Bool){
//        stringType = "bool";
//    }else{
//        isConvertible = false;
//    }

//return stringType;
//}


//void DrugXmlExport::writeNewLine()
//{
//    writer.device()->putChar('\n');
//}

