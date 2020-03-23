#include "predictionspecexporter.h"

#include "core/dal/predictionspec.h"
#include "core/dal/drug/parameters.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/drug.h"
#include "core/dal/coremeasure.h"
#include "core/dal/drug/target.h"
#include "core/dal/covariate.h"
#include "core/dal/dosage.h"
#include "core/dal/amount.h"
#include "core/dal/uncastedvalue.h"
#include "core/dal/drug/adme.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/drug/bsv.h"
#include "core/dal/drug/errormodel.h"
#include "core/dal/drug/parameters.h"
#include "core/dal/drug/standardtreatment.h"

PredictionSpecExporter::PredictionSpecExporter()
{

}

#include <QTextStream>

bool PredictionSpecExporter::save(ezechiel::core::PredictionSpec *spec, QByteArray &data)
{
    writer = new QXmlStreamWriter(&data);

    writer->setAutoFormatting(true);
    writer->writeStartDocument("1.0",false);

    /*
    writer.writeProcessingInstruction("xml-stylesheet", "href=\"drugsmodel.xsl\" type=\"text/xsl\"");


    QString namespaceUri = "http://www.w3.org/2001/XMLSchema-instance";
    writer.writeStartElement("model");
    writer.writeNamespace(namespaceUri,"xsi");
    writer.writeAttribute("version","0.6");
    writer.writeAttribute(namespaceUri, "noNamespaceSchemaLocation", "drug.xsd");
    */

    save(spec);


    delete writer;
    return true;
}

QString PredictionSpecExporter::writeDate(QDateTime date)
{
    return date.toString("yyyy-MM-dd. hh:mm:ss");
}

void PredictionSpecExporter::save(ezechiel::core::PredictionSpec *spec)
{
    writer->writeStartElement("predictionSpec");
    writer->writeTextElement("calculationType", spec->calculationTypeToString(spec->getCalculationType()));
    writer->writeTextElement("startDate", writeDate(spec->getStartDate()));
    writer->writeTextElement("endDate", writeDate(spec->getEndDate()));
    writer->writeTextElement("steadyState", spec->getSteadyState() ? "true" : "false");
    writer->writeTextElement("nbPoints", QString("%1").arg(spec->getNbPoints()));
    writer->writeTextElement("paramsType", toString(spec->getParamsType()));
    writer->writeTextElement("adjustmentDate", writeDate(spec->getAdjustmentDate()));
    writer->writeStartElement("percentiles");
    for(unsigned int i=0;i< spec->getPercentileList().size();i++)
        writer->writeTextElement("p", QString("%1").arg(spec->getPercentileList()[i]));
    writer->writeEndElement();
    save(spec->getAnalysis());
    writer->writeEndElement();
}

void PredictionSpecExporter::saveIdentifiableAmount(const QString &tagName, ezechiel::core::IdentifiableAmount *amount)
{
    writer->writeStartElement(tagName);
    writer->writeTextElement("amountId", amount->getAmountId());
    writer->writeTextElement("value", QString("%1").arg(amount->getDbvalue()));
    writer->writeTextElement("unit", amount->getUnitstring());
    writer->writeEndElement();
}

void PredictionSpecExporter::saveOperableAmount(const QString &tagName, ezechiel::core::OperableAmount *amount)
{
    writer->writeStartElement(tagName);
    writer->writeTextElement("amountId", amount->getAmountId());
    writer->writeTextElement("value", QString("%1").arg(amount->getDbvalue()));
    writer->writeTextElement("unit", amount->getUnitstring());
    save(amount->getOperations());
    writer->writeEndElement();
}

void PredictionSpecExporter::save(ezechiel::core::OperationList *list)
{
    writer->writeStartElement("operations");
    if (list != nullptr) {
        foreach(ezechiel::core::Operation *operation, list->getList()) {
            writer->writeStartElement("operation");
            writer->writeTextElement("type",
                                     (operation->getType() == ezechiel::core::OperationType::HARDCODED) ? "Hardcoded" :
                                                                                                          (operation->getType() == ezechiel::core::OperationType::IMPORTED) ? "Imported" :
                                                                                                                                                                              "Noop");
            writer->writeTextElement("formula", operation->getFormula());
            writer->writeEndElement();
        }
    }
    writer->writeEndElement();
}

void PredictionSpecExporter::save(ezechiel::core::DrugResponseAnalysis *analysis)
{
    writer->writeStartElement("drugResponseAnalysis");
    save(analysis->getTreatment());
    save(analysis->getDrugModel());
    writer->writeEndElement();
}


void PredictionSpecExporter::save(ezechiel::core::DrugTreatment *treatment)
{
    writer->writeStartElement("treatment");
    save(treatment->getDosages());
    save(treatment->getCovariates());
    save(treatment->getMeasures());
    save(treatment->getTargets());
    writer->writeEndElement();
}

void PredictionSpecExporter::save(ezechiel::core::CoreMeasureList *list)
{
    writer->writeStartElement("measures");
    foreach (ezechiel::core::CoreMeasure *measure, list->getList()) {
        writer->writeStartElement("measure");
        writer->writeTextElement("moment", writeDate(measure->getMoment()));
        saveIdentifiableAmount("concentration", measure->getConcentration());
        save(measure->getUncastedValues());
        writer->writeEndElement();
    }
    writer->writeEndElement();
}

void PredictionSpecExporter::save(ezechiel::core::UncastedValueList *list)
{
    writer->writeStartElement("uncastedValues");
    foreach(ezechiel::core::UncastedValue *value, list->getList()) {
        writer->writeStartElement("uncastedValue");
        writer->writeTextElement("field", value->getField());
        writer->writeTextElement("value", value->getText());
        writer->writeEndElement();
    }
    writer->writeEndElement();
}


void PredictionSpecExporter::save(ezechiel::core::DosageHistory *history)
{
    writer->writeStartElement("dosageHistory");
    foreach(ezechiel::core::Dosage *dose, history->getList()) {
        writer->writeStartElement("dosage");
        writer->writeTextElement("dbinterval", QString("%1").arg(dose->getDbinterval()));
        writer->writeTextElement("dbtinf", QString("%1").arg(dose->getDbtinf()));
        writer->writeTextElement("route", dose->getRoute()->getLabel());
        saveIdentifiableAmount("quantity", dose->getQuantity());
        writer->writeTextElement("applied", writeDate(dose->getApplied()));
        writer->writeTextElement("endTime", writeDate(dose->getEndTime()));
        writer->writeTextElement("tinf", QString("%1").arg(dose->getTinf().toMinutes()));
        writer->writeTextElement("interval", QString("%1").arg(dose->getInterval().toMinutes()));
        save(dose->getUncastedValues());
        writer->writeEndElement();
    }
    writer->writeEndElement();
}

void PredictionSpecExporter::save(ezechiel::core::PatientVariateList *list)
{
    writer->writeStartElement("patientVariates");
    foreach(ezechiel::core::PatientVariate *variate, list->getList()) {
        writer->writeStartElement("patientVariate");
        writer->writeTextElement("name", variate->getCovariateId());
        writer->writeTextElement("date", writeDate(variate->getDate()));
        saveOperableAmount("quantity", variate->getQuantity());
        writer->writeTextElement("type", QMetaType::typeName(variate->getType()));
        writer->writeTextElement("forced", variate->getForced() ? "true" : "false");
        save(variate->getUncastedValues());
        writer->writeEndElement();
    }
    writer->writeEndElement();
}

void PredictionSpecExporter::save(ezechiel::core::TargetList *list)
{
    writer->writeStartElement("targets");
    foreach(ezechiel::core::Target *target, list->getList()) {
        writer->writeStartElement("target");
        writer->writeTextElement("method", target->getType()->getLabel());
        writer->writeStartElement("concentrations");
        saveOperableAmount("cmin", target->getCmin());
        saveOperableAmount("cmax", target->getCmax());
        saveOperableAmount("cbest", target->getCbest());
        writer->writeEndElement();
        writer->writeStartElement("times");
        saveOperableAmount("tmin", target->getTmin());
        saveOperableAmount("tmax", target->getTmax());
        saveOperableAmount("tbest", target->getTbest());
        writer->writeEndElement();
        writer->writeEndElement();
    }
    writer->writeEndElement();
}


void PredictionSpecExporter::save(ezechiel::core::DrugModel *model)
{
    writer->writeStartElement("drugModel");
    // Activesubstance?

    writer->writeTextElement("drugModelId", model->getDrugModelId());
    writer->writeTextElement("pkModelId", model->getPkModelId());
    save(model->getAdme());
    // Halflife?

    // Concentrations?

    // Results?


    writer->writeStartElement("standardTreatment");
    writer->writeTextElement("isFixedDuration", model->getStandardTreatment()->getIsFixedDuration() ? "true" : "false");
    writer->writeStartElement("duration");
    writer->writeTextElement("unit", "m");
    writer->writeTextElement("value", QString::number(model->getStandardTreatment()->getDuration().toMinutes()));
    writer->writeEndElement(); // duration
    writer->writeEndElement(); // standardTreatment

    // We group the dosage elements in one single group
    writer->writeStartElement("dosages");
    save(model->getDoses());
    save(model->getInfusions());
    save(model->getIntervals());
    writer->writeEndElement();

    save(model->getCovariates());
    save(model->getTargets());

    // Operations?

    save(model->getParameters());
    //save(model->getErrorModel());

    writer->writeStartElement("errorModel");
    writer->writeTextElement("additive", QString("%1").arg(model->getErrorModel()->getAdditive()));
    writer->writeTextElement("proportional", QString("%1").arg(model->getErrorModel()->getProportional()));
    writer->writeEndElement();

    writer->writeEndElement();
}

void PredictionSpecExporter::save(ezechiel::core::ADME *adme)
{
    writer->writeStartElement("adme");
    writer->writeTextElement("defaultIntake", adme->getDefaultIntake()->getLabel());
    writer->writeTextElement("distribution", adme->getDistribution());
    writer->writeTextElement("elimination", adme->getElimination());
    writer->writeStartElement("intakes");
    foreach(ezechiel::core::Admin *admin, adme->getIntakes()->getList()) {
        writer->writeTextElement("intake", admin->getLabel());
    }
    writer->writeEndElement();
    writer->writeEndElement();
}

void PredictionSpecExporter::save(ezechiel::core::ValidDoses *doses)
{
    writer->writeStartElement("validDoses");
    writer->writeTextElement("anyDose", doses->getAnyDose() ? "true" : "false");
    // Should be renamed "defaultDose"
    saveIdentifiableAmount("defaultDose", doses->getQuantity());
    writer->writeStartElement("doses");
    for (int i=0; i< doses->size() ; i++) {
        ezechiel::core::ValidDose *dose = doses->at(i);
        writer->writeStartElement("dose");
        writer->writeTextElement("route", dose->getRoute()->getLabel());
        saveIdentifiableAmount("quantity", dose->getQuantity());
        writer->writeEndElement();
    }
    writer->writeEndElement();
    writer->writeEndElement();
}


void PredictionSpecExporter::save(ezechiel::core::ValidInfusions *infusions)
{
    writer->writeStartElement("validInfusions");
    writer->writeTextElement("any", infusions->getAny() ? "true" : "false");
    saveIdentifiableAmount("defaultInfusion", infusions->getQuantity());
    writer->writeStartElement("infusions");
    for (int i=0; i< infusions->size() ; i++) {
        writer->writeStartElement("infusion");
        saveIdentifiableAmount("quantity", infusions->at(i)->getQuantity());
        writer->writeEndElement();
    }
    writer->writeEndElement();
    writer->writeEndElement();
}


void PredictionSpecExporter::save(ezechiel::core::ValidIntervals *intervals)
{
    writer->writeStartElement("validIntervals");
    writer->writeTextElement("any", intervals->getAny() ? "true" : "false");
    // Should be renamed "defaultInterval"
    saveIdentifiableAmount("defaultInterval", intervals->getQuantity());
    writer->writeStartElement("intervals");
    for (int i=0; i< intervals->size() ; i++) {
        writer->writeStartElement("interval");
        saveIdentifiableAmount("quantity", intervals->at(i)->getQuantity());
        writer->writeEndElement();
    }
    writer->writeEndElement();
    writer->writeEndElement();
}

void PredictionSpecExporter::save(ezechiel::core::DrugVariateList *list)
{
    writer->writeStartElement("drugVariates");
    foreach(ezechiel::core::DrugVariate *variate, list->getList()) {
        writer->writeStartElement("drugVariate");
        writer->writeTextElement("name", variate->getCovariateId());
        saveOperableAmount("quantity", variate->getQuantity());
        writer->writeTextElement("type", QMetaType::typeName(variate->getType()));
        writer->writeEndElement();
    }

    writer->writeEndElement();
}

void PredictionSpecExporter::save(ezechiel::core::ParameterSet *set)
{
    writer->writeStartElement("parameterSet");
    writer->writeTextElement("time", writeDate(set->getTime()));
    writer->writeStartElement("parameters");
    foreach(ezechiel::core::Parameter *p, set->getParameters()->getList()) {
        writer->writeStartElement("parameter");
        writer->writeTextElement("name", p->getName());
        saveOperableAmount("quantity", p->getQuantity());
        writer->writeStartElement("bsv");
        writer->writeTextElement("standard", QString("%1").arg(p->getBsv()->getStandard()));
        writer->writeTextElement("proportional", QString("%1").arg(p->getBsv()->getProportional()));
        writer->writeEndElement();
        writer->writeTextElement("min", QString("%1").arg(p->getMin()));
        writer->writeTextElement("max", QString("%1").arg(p->getMax()));
        writer->writeTextElement("step", QString("%1").arg(p->getStep()));
        save(p->getOperations());
        writer->writeEndElement();
    }
    writer->writeEndElement();
    writer->writeStartElement("correlations");
    foreach(QString pkey, set->correlationsKeys()) {
        writer->writeStartElement("correlation");
        QStringList params = pkey.split("__");
        writer->writeTextElement("param1", params[0]);
        writer->writeTextElement("param2", params[1]);
        writer->writeTextElement("value", QString("%1").arg(set->correlation(params[0], params[1])));
        writer->writeEndElement();
    }

    writer->writeEndElement();
    writer->writeEndElement();
}
