//@@license@@

#include <QFile>
#include <QFileInfo>

#include "core/utils/version.h"
#include "current_version.h"
#include "exportaction.h"
#include "sphinxmacros.h"
#include "core/core.h"
#include "core/pluginmanager.h"
#include "apputils/src/drugmanager.h"
//#include "core/interfaces/database.h"
#include "core/dal/drug/drug.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/practician.h"
#include "core/corerepository.h"
#include "admin/src/stdadminrepository.h"
//#include "chart.h"
//#include "chartitemgroup.h"
//#include "percentilecurve.h"
//#include "dot.h"
//#include "targetinterval.h"
#include "core/utils/xmlvalidator.h"
#include "apputils/src/appcore.h"
#include "core/corefactory.h"
#include "admin/src/dal/measure.h"
#include "core/utils/logging.h"
#include "errors_cliutils.h"


namespace Tucuxi {
namespace cliutils {
//The error messages
const char *const ExportAction::_INVALID_SUBCOMMAND = QT_TR_NOOP_UTF8("please run the help to display information about them");
const char *const ExportAction::_INVALID_ARGUMENT   = QT_TR_NOOP_UTF8("The argument '%1' is unknown.");
const char *const ExportAction::_INVALID_ARGUMENTS  = QT_TR_NOOP_UTF8("Invalid number of arguments given: the '%1' subcommand requires exactly %2 arguments.");
const char *const ExportAction::_TOO_FEW_ARGUMENTS  = QT_TR_NOOP_UTF8("Invalid number of arguments given: the '%1' subcommand requires at least %2 arguments.");
const char *const ExportAction::_TOO_MANY_ARGUMENTS = QT_TR_NOOP_UTF8("Invalid number of arguments given: the '%1' subcommand requires at most %2 arguments.");
const char *const ExportAction::_INVALID_DATE       = QT_TR_NOOP_UTF8("The date '%1' is invalid or incorretly formatted. Please use the format '%2'.");
const char *const ExportAction::_FILE_OPEN_ERROR    = QT_TR_NOOP_UTF8("The file '%1' could not be opened. Please check if the file and the path are valid or if you have the rights and try again.");

//The shared messages
const char *const ExportAction::_OVERRIDE_FILE      = QT_TR_NOOP_UTF8("The file '%1' already exists. Do you want to override it? (y/n)");

//The action commands
const char *const ExportAction::_ACTION_COMMAND        = "export";
const char *const ExportAction::_CMD_MODELS            = "models";
const char *const ExportAction::_CMD_INTERPRETATION    = "interpretation";
const char *const ExportAction::_CMD_PARAM_LATEX       = "latex";
const char *const ExportAction::_CMD_PARAM_HTML        = "html";
const char *const ExportAction::_CMD_PARAM_SPHINX      = "sphinx";
const char *const ExportAction::_CMD_PARAM_CURVEID     = "curve";
const char *const ExportAction::_CMD_PARAM_PERCENTILES = "percentiles";
const char *const ExportAction::_CMD_PARAM_MANDATORID  = "mandator";
const char *const ExportAction::_CMD_PARAM_ANALYSTID   = "analyst";

QString ExportAction::name()
{
    return _ACTION_COMMAND;
}

QString ExportAction::help()
{
    QString str;
    str = tr("   NAME") + "\n" +
          tr("       EzeCHieL Export Command") + "\n" + "\n" +
          tr("   SYNOPSIS") + "\n" +
             "       " + QString("ezechiel-cli [OPTIONS] %1 %2USERNAME %3PASSWORD SUBCOMMAND").arg(_ACTION_COMMAND, _CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD) + "\n" + "\n" +
          tr("   DESCRIPTION") + "\n" +
          tr("       The EzeCHieL Export Command can be used to export various data such as information about the models. It can also be used to export data about an interpretation and its related data in an export XML file.") + "\n" + "\n" +
          tr("   SUBCOMMANDS") + "\n" +
             "       " + QString("%1 FILE %2|%3|%4").arg(_CMD_MODELS, _CMD_PARAM_HTML, _CMD_PARAM_LATEX, _CMD_PARAM_SPHINX) + "\n" +
          tr("              Export information about the models in the file FILE, using the given format.")  + "\n" + "\n" +
             "       " + QString("%1 DATE PATIENTID FILE [%2=CURVEID [%3=X-Y[,...]]] [%4=MANDATORID] [%5=ANALYSTID] ").arg(_CMD_INTERPRETATION, _CMD_PARAM_CURVEID, _CMD_PARAM_PERCENTILES, _CMD_PARAM_MANDATORID, _CMD_PARAM_ANALYSTID) + "\n" +
          tr("              Export the interpretation corresponding to the given DATE for the given PATIENTID in the given FILE. If the interpretation should contain information about the samples, dosages, covariates, parameters and such, it is necessary to provide the related CURVEID. If 'percentiles' is specified, the percentiles X-Y will also be displayed in the curve's chart. The interpretation may also contain information about the given MANDATORID and/or ANALYSTID.")  + "\n" + "\n" +
          "\n";

    return str;
}

ExportAction::ExportAction()
{

}

bool ExportAction::run(const QStringList &args)
{
    QString subcommand;

    //Copy the arguments
    QStringList wargs = args;

    //Clear the errors
    _errors.clear();

    //Check the arguments
    if (!wargs.size()) {
        _errors = tr("Missing subcommand, %1.").arg(tr(_INVALID_SUBCOMMAND));
        return false;
    }

    //Get subcommand
    subcommand = wargs.takeFirst();

    //Run subcommand
    if (subcommand == _CMD_MODELS)
        return exportModels(wargs);
    if (subcommand == _CMD_INTERPRETATION)
        return exportInterpretation(wargs);

    //Display an error
    _errors = tr("Invalid subcommand '%1' given, %2.").arg(subcommand, tr(_INVALID_SUBCOMMAND));

    return false;
}

bool ExportAction::exportModels(QStringList &args)
{
    bool result = false;

    //Check if enough arguments
    if (args.size() != 2) {
        _errors = tr(_INVALID_ARGUMENTS).arg(_CMD_MODELS, QString::number(2));
        return false;
    }
    QFile file(args.takeFirst());

    //Check if the file exists
    if (!overrideFile(file))
        return true;

    //Open the file in write mode
    if (!file.open(QIODevice::WriteOnly)) {
        _errors = tr(_FILE_OPEN_ERROR).arg(QFileInfo(file).fileName());
        return false;
    }
    QTextStream out(&file);

    //Export in the correct format
    if (args.first() == _CMD_PARAM_HTML)
        result = modelsToHtml(out);
    else if (args.first() == _CMD_PARAM_LATEX)
        result = modelsToLatex(out);
    else if (args.first() == _CMD_PARAM_SPHINX)
        result = modelsToSphinx(out);
    else
        _errors = tr(_INVALID_ARGUMENT).arg(args.first()) + " " + tr("The format can be either '%1', '%2' or '%3'").arg(_CMD_PARAM_HTML, _CMD_PARAM_LATEX, _CMD_PARAM_SPHINX);

    //Close the file
    file.close();

    return result;
}

bool ExportAction::exportInterpretation(QStringList &args)
{
    //The exportation data
    QDateTime date;
    QString filename;
    PercentileList percentiles;
    ident patientId  = invalid_ident;
    ident curveId    = invalid_ident;
    ident mandatorId = invalid_ident;
    ident analystId  = invalid_ident;

    //Initialize the data
    if (!initializeInterpretation(args, date, filename, patientId, curveId, percentiles, mandatorId, analystId))
        return false;

    //Build the dataset
    if (!buildDataset(date, patientId, curveId, percentiles, mandatorId, analystId))
        return false;

    //Export the dataset
    if (!exportDatasets(filename))
        return false;

    return true;
}

bool ExportAction::modelsToHtml(QTextStream &out)
{
    /*
    //Iterate over each models
    foreach (ModelDescriptor model, CORE->pluginManager()->models()) {
        QScopedPointer<DrugModel> m(CORE->pluginManager()->newDrugModelEngine(model.id));

        //Export the current model
        out << "<table border=1>" << endl;
        out << "<tr><td colspan=2>ID</td><td>" << qPrintable(model.id) << "</td></tr>" << endl;
        out << "<tr><td colspan=2>Model ID</td><td>" << qPrintable(model.modelId) << "</td></tr>" << endl;
        out << "<tr><td colspan=2>Model name</td><td>" << qPrintable(model.getName()) << "</td></tr>" << endl;
        out << "<tr><td colspan=2>Model Description</td><td>" << qPrintable(model.getDescription()) << "</td></tr>" << endl;
        out << "<tr><td colspan=3>Parameters</td></tr>" << endl;
        out << "<tr><td>ID</td><td>Name</td><td>Description</td></tr>" << endl;
        foreach (Descriptor desc,m->parametersDescription()) {
            out << "<tr><td>" << qPrintable(desc.id) << "</td><td>" << qPrintable(desc.getName()) << "</td><td>" << qPrintable(desc.getDescription()) << "</td></tr>" << endl;
        }
        out << "</table>" << endl;
        out << "<br />" << endl;
    }
*/
    return true;
}

bool ExportAction::modelsToLatex(QTextStream &out)
{
    /*
    //Setup the implementation table
    out << "\\begin{tabular}{l|l}\\toprule" << endl;
    out << "\\textbf{ModelId} & \\textbf{Implementations} \\\\ \\midrule" << endl;

    //Print the models implementation
    foreach (QString modelId, CORE->pluginManager()->modelIds()) {
        out << qPrintable(modelId) << " & p. ";
        foreach (ModelDescriptor model, CORE->pluginManager()->knownModels(modelId))
            out << "\\pageref{" << model.id << "} " << endl;
        out << "\\\\" << endl;
    }
    out << "\\bottomrule" << endl;
    out << "\\end{tabular}" << endl << endl;

    //Print the models information
    foreach (ModelDescriptor model,CORE->pluginManager()->models()) {
        QScopedPointer<DrugModel> m(CORE->pluginManager()->newDrugModelEngine(model.id));

        //Print the current model
        out << "\\phantomsection\\label{" << model.id << "}" << endl;
        out << "\\begin{tabularx}{\\textwidth}{c|p{3cm}|X}\\toprule" << endl;
        out << "\\multicolumn{3}{c}{\\textbf{Model}} \\\\ \\midrule" << endl;
        out << "\\multicolumn{2}{l|}{ID} & " << qPrintable(model.id) << "\\\\" << endl;
        out << "\\multicolumn{2}{l|}{Model ID} & " << qPrintable(model.modelId) << "\\\\" << endl;
        out << "\\multicolumn{2}{l|}{Name} & " << model.getName().replace("%","\\%") << "\\\\" << endl;
        out << "\\multicolumn{2}{l|}{Description} & " << model.getDescription().replace("%","\\%") << "\\\\ \\midrule" << endl;
        out << "\\multicolumn{3}{c}{\\textbf{Parameters}} \\\\ \\midrule" << endl;
        out << "\\textbf{ID} & \\textbf{Name} & \\textbf{Description} \\\\ \\midrule" << endl;

        //Print the parameters description
        foreach (Descriptor desc, m->parametersDescription())
            out << qPrintable(desc.id) << " & " << desc.getName().replace("%","\\%") << " & " << desc.getDescription().replace("%","\\%") << " \\\\ \\bottomrule" << endl;
        out << "\\end{tabularx}" << endl;
    }
*/
    return true;
}

bool ExportAction::modelsToSphinx(QTextStream &out)
{
    /*
    //Print the title
    out << "******\n"
           "Models\n"
           "******\n"
           "\n"
           ".. contents:: Contents\n"
           "    :depth: 1\n"
           "    :backlinks: top\n"
           "    :local:\n"
           "\n"
           "This chapter presents the different models currently available in EzeCHieL. When creating "
           "a new drug file, you need to choose which model, or family of models, will be used for the "
           "computation of the prediction curves. You can do this by specifying its model ID, as shown "
           "below, in the <modelid> tag of the drug's head section. For more information, please see the "
           "Drug description section of the user manual.\n\n"
           "For each model, you will find information about their ID, Model ID (or the family of the model), "
           "name and description. You will also find a complete listing of the parameters used by the model.\n"
           "\n";

    //Print the models implementation
    out << "Models list\n"
           "===========\n"
           "\n";

    //Set the columns size
    int col1size = 0;
    int col2size = 0;
    int col3size = 0;
    int totalsize = 80;


    foreach (QString modelId, CORE->pluginManager()->modelIds())
        if (modelId.length() > col1size)
            col1size = modelId.length();

    col1size ++; // For the space
    col2size = totalsize - col1size - 3;


    LINE2(col1size,col2size);
    LINECONTENT2(QString("ModelId"),col1size,QString("Implementations"),col2size);

    LINETITLE2(col1size,col2size);

    foreach (QString modelId, CORE->pluginManager()->modelIds()) {

        QString col2 = "";
        foreach (ModelDescriptor model, CORE->pluginManager()->knownModels(modelId)) {
            col2 += "";
        }
        LINECONTENT2(modelId,col1size,col2,col2size);
        LINE2(col1size,col2size);
    }
    out << endl;

    //Print the models information
    foreach (ModelDescriptor model,CORE->pluginManager()->models()) {

        //Print the model name
        QString modelName = model.getName();
        out << modelName << endl << QString(modelName.size(), '=') << endl << endl;

        QScopedPointer<DrugModel> m(CORE->pluginManager()->newDrugModelEngine(model.id));

        //Set the models columns size
        col1size = 20;
        col2size = 0;
        if (model.id.length() > col2size)
            col2size = model.id.length();
        if (model.modelId.length() > col2size)
            col2size = model.modelId.length();
        if (model.getName().length() > col2size)
            col2size = model.getName().length();
        if (model.getDescription().length() > col2size)
            col2size = model.getDescription().length();

        totalsize = col1size + col2size + 3;

        //Print the current model
        LINE1(totalsize-2);
        LINECONTENT1(QString("Model"),totalsize-2);

        LINETITLE2(col1size,col2size);
        LINECONTENT2(QString("ID"),col1size,model.id,col2size);

        LINE2(col1size,col2size);
        LINECONTENT2(QString("Model ID"),col1size,model.modelId,col2size);

        LINE2(col1size,col2size);
        LINECONTENT2(QString("Name"),col1size,model.getName(),col2size);

        LINE2(col1size,col2size);
        LINECONTENT2(QString("Description"),col1size,model.getDescription(),col2size);

        LINE2(col1size,col2size);

        out << endl;

        //Set the parameters columns size
        col1size = 0;
        col2size = 0;
        col3size = 0;

        //Print a blank line
        out << "|\n\n";

        //Print the parameters descriptions
        foreach (Descriptor desc, m->parametersDescription()) {
            if (desc.id.length() > col1size)
                col1size = desc.id.length();
            if (desc.getName().length() > col2size)
                col2size = desc.getName().length();
            if (desc.getDescription().length() > col3size)
                col3size = desc.getDescription().length();
        }

        if (col1size < QString("ID").length())
            col1size = QString("ID").length();

        if (col2size < QString("Name").length())
            col2size = QString("Name").length();

        if (col3size < QString("Description").length())
            col3size = QString("Description").length();

        totalsize = col1size + col2size + col3size + 4;


        LINE1(totalsize-2);
        LINECONTENT1(QString("Parameter"),totalsize-2);
        LINETITLE3(col1size,col2size,col3size);
        LINECONTENT3(QString("ID"),col1size,QString("Name"),col2size,QString("Description"),col3size);
        LINE3(col1size,col2size,col3size);

        foreach (Descriptor desc,m->parametersDescription()) {
            LINECONTENT3(desc.id,col1size,desc.getName(),col2size,desc.getDescription(),col3size);
            LINE3(col1size,col2size,col3size);
        }

        out << endl;
    }
*/
    return true;
}

bool ExportAction::initializeInterpretation(QStringList &args, QDateTime &date, QString &filename, ident &patientId, ident &curveId, PercentileList &percentiles, ident &mandatorId, ident &analystId)
{
    bool check;

    //Check if too few arguments
    if (args.size() < 3) {
        _errors = tr(_TOO_FEW_ARGUMENTS).arg(_CMD_INTERPRETATION, QString::number(3));
        return false;
    }

    //Check if too many arguments
    if (args.size() > 7) {
        _errors = tr(_TOO_MANY_ARGUMENTS).arg(_CMD_INTERPRETATION, QString::number(7));
        return false;
    }

    //Check if the date is valid
    date = QDateTime::fromString(args.at(0), Action::dateTimeFormat());
    if (!date.isValid()) {
        _errors = tr(_INVALID_DATE).arg(args.at(0), Action::dateTimeFormat());
        return false;
    }
    args.removeFirst();

    //Check if the patient ID is valid
    patientId = args.at(0).toInt(&check);
    if (!check || patientId == invalid_ident) {
        _errors = tr("Invalid or unknown patient ID '%1'. Please use the 'list' command to retrieve the ID of an existing patient.").arg(args.at(0));
        return false;
    }
    args.removeFirst();

    //Check if the file exists
    QFile file(filename = args.at(0));
    if (!overrideFile(file))
        return false;

    //Check if the file is writable
    if (!file.open(QIODevice::WriteOnly)) {
        _errors = tr(_FILE_OPEN_ERROR).arg(args.at(0));
        return false;
    }
    file.close();
    args.removeFirst();

    //Get the other parameters
    for (int i = 0; i < args.size(); i++) {

        if (args.at(i).startsWith(QString("%1=").arg(_CMD_PARAM_CURVEID))) {

            //Set the curve ID
            if (!args[i].remove(QString("%1=").arg(_CMD_PARAM_CURVEID)).isEmpty()) {
                curveId = args.at(i).toInt(&check);
                if (!check || curveId == invalid_ident) {
                    _errors = tr("Invalid or unknown curve ID '%1'. Please use the 'list' command to retrieve the ID of an existing curve.").arg(args.at(0));
                    return false;
                }
            }

        } else if (args.at(i).startsWith(QString("%1=").arg(_CMD_PARAM_MANDATORID))) {

            //Set the mandator ID
            if (!args[i].remove(QString("%1=").arg(_CMD_PARAM_MANDATORID)).isEmpty()) {
                mandatorId = args.at(i).toInt(&check);
                if (!check || mandatorId == invalid_ident) {
                    _errors = tr("Invalid or unknown mandator ID '%1'. Please use the 'list' command to retrieve the ID of an existing practician.").arg(args.at(0));
                    return false;
                }
            }

        } else if (args.at(i).startsWith(QString("%1=").arg(_CMD_PARAM_ANALYSTID))) {

            //Set the analyst ID
            if (!args[i].remove(QString("%1=").arg(_CMD_PARAM_ANALYSTID)).isEmpty()) {
                analystId = args.at(i).toInt(&check);
                if (!check || analystId == invalid_ident) {
                    _errors = tr("Invalid or unknown analyst ID '%1'. Please use the 'list' command to retrieve the ID of an existing practician.").arg(args.at(0));
                    return false;
                }
            }

        } else if (args.at(i).startsWith(QString("%1=").arg(_CMD_PARAM_PERCENTILES))) {

            //Set the percentiles
            QStringList rawPercentiles = args[i].remove(QString("%1=").arg(_CMD_PARAM_PERCENTILES)).split(",");

            //Check if empty
            if (rawPercentiles.isEmpty()) {
                _errors = tr("Missing percentiles '%1' values.").arg(args.at(i));
                return false;
            }

            //Extract the percentiles
            foreach (QString current, rawPercentiles) {

                //Get the current values
                QStringList values = current.split("-");
                if (values.size() != 2) {
                    _errors = tr("Invalid percentiles '%1' given.").arg(current);
                    return false;
                }

                int low = values.at(0).toInt(&check);
                if (!check) {
                    _errors = tr("Invalid percentile value '%1' given.").arg(values.at(0));
                    return false;
                }

                int high = values.at(1).toInt(&check);
                if (!check) {
                    _errors = tr("Invalid percentile value '%1' given.").arg(values.at(1));
                    return false;
                }

                //Add the percentiles
                Percentile p((low < high ? low : high), (low < high ? high : low));
                percentiles.append(p);
            }

        } else {

            //Invalid argument
            _errors = tr(_INVALID_ARGUMENT).arg(args.at(i));
            return false;

        }
    }

    return true;
}

bool ExportAction::buildDataset(const QDateTime &date, const ident &patientId, const ident &curveId, const PercentileList &percentiles, const ident &mandatorId, const ident &analystId)
{
    //The build data
    Dataset dataset;
    PatientData patient;
    PracticianData mandator;
    PracticianData analyst;

    //Build the clinicals data
    if (!buildInterpretation(dataset, date, patientId))
        return false;

    //Build the curve data
//    if (!buildCurve(dataset, date, curveId, percentiles))
//        return false;

    //Build the patient data
    if (!buildPatient(patient, patientId))
        return false;

    //Build the mandator data
    if (!buildPractician(mandator, mandatorId))
        return false;

    //Build the analyst data
    if (!buildPractician(analyst, analystId))
        return false;

    //Set the persons
    dataset.setPatient(patient);
    dataset.setMandator(mandator);
    dataset.setAnalyst(analyst);

    //Set the language
    dataset.setLang(CORE->language());

    //Store the dataset
    _document.addDataset(dataset);

    return true;
}

bool ExportAction::buildInterpretation(ExportAction::Dataset &dataset, const QDateTime &date, const ident &patientId)
{
    Tucuxi::GuiCore::Response response;

    //The database objects
    QList<Clinical*> clinicals;

    //Get the clinicals
//    response = DB->getClinicalsByDate(patientId, clinicals);
    response = ADMINREPO->getClinicalsByDate(patientId, clinicals);
    if (response.error != NoError && response.error != NotFound) {
        _errors = tr(_DATABASE_READ_FAILED).arg(tr("clinicals of patient"), tr("ID"), QString::number(patientId), response.message);
        return false;
    }

    //Sort the clinicals
    for (int i = 0; i < clinicals.size(); i++)
        if (clinicals.at(i)->getDate() != date)
            clinicals.removeAt(i);

    //Check the clinicals
    if (!clinicals.size()) {
        _errors = tr("Could not find any clinicals for patient ID '%1' at date '%2'.").arg(QString::number(patientId), date.toString(Action::dateTimeFormat()));
        return false;
    }

    //Build the interpretation
    InterpretationData interpretationData;
    QList<ClinicalData> clinicalsData;

    for (int i = 0; i < clinicals.size(); i++) {
        if (clinicals.at(i)->getName() == "diagnosis")
            interpretationData.setExpectedness(clinicals.at(i)->getValue());
        if (clinicals.at(i)->getName() == "toxicity")
            interpretationData.setWarning(clinicals.at(i)->getValue());
        if (clinicals.at(i)->getName() == "response")
            interpretationData.setSuitability(clinicals.at(i)->getValue());

        ClinicalData clinicalData;
        clinicalData.setDate(clinicals.at(i)->getDate());
        clinicalData.setName(clinicals.at(i)->getName());
        clinicalData.setValue(clinicals.at(i)->getValue());
        clinicalsData.append(clinicalData);
    }
    interpretationData.setDate(date);
    interpretationData.setId("none");

    //Set the interpretation
    dataset.setInterpretation(interpretationData);

    //Set the clinicals
    dataset.setClinicals(clinicalsData);

    return true;
}

//bool ExportAction::buildCurve(ExportAction::Dataset &dataset, const QDateTime &date, const ident &curveId, const PercentileList &percentiles, const uint nbPoints)
//{
//    SharedDrugResponseAnalysis _resp = Tucuxi::GuiCore::CoreFactory::createEntity<DrugResponseAnalysis>(REPO);

//    //Ignore if not requested
//    if (curveId == invalid_ident)
//        return true;

//    //Get the curve
//    if (!_resp->load(curveId)) {
//        _errors = tr(_DATABASE_READ_FAILED).arg(tr("curve"), tr("ID"), QString::number(curveId), _resp->errorString());
//        return false;
//    }

//    //Build the drug
//    DrugData drugData;
//    drugData.setId(_resp->treatment()->drug()->descriptor().id);
//    drugData.setActivePrinciple(_resp->treatment()->drug()->name());
//    drugData.setAtc(APPCORE->drugManager()->drug(_resp->treatment()->drug()->descriptor().id)->atc());

//    //Build the samples
//    QList<SampleData> samplesData;

//    SharedCoreMeasure sample = _resp->treatment()->previousCoreMeasure(date);
//    if (!sample->isValid()) {
//        _errors = tr("Invalid sample: could not find any sample prior to the date '%1'.").arg(date.toString(Action::dateTimeFormat()));
//        return false;
//    }

//    SampleData sampleData;
//    sampleData.setConcentration(sample->amount());
////    sampleData.setId(sample->sampleID());
//    sampleData.setSampleDate(sample->moment());
////    sampleData.setArrivalDate(sample->arrivalDate());
//    samplesData.append(sampleData);

//    QDateTime sampleDate = sample->moment();
//    Duration  sampleTime = Duration(0, 0, 0, _resp->treatment()->dosages()->firsttake().msecsTo(sampleDate));

//    //Build the dosages
//    QList<DosageData> dosagesData;

//    Dosage* dosage = _resp->treatment()->dosages()->currentDosage(sampleDate);
//    if (!dosage->isValid()) {
//        _errors = tr("Invalid dosage: could not find any dosage prior to the date '%1'.").arg(sampleDate.toString(Action::dateTimeFormat()));
//        return false;
//    }

//    DosageData dosageData;
//    dosageData.setDose(dosage->dose());
//    dosageData.setInterval(dosage->interval());
//    dosageData.setInfusion(dosage->tinf());
//    dosageData.setIntake(dosageData.intake());
//    dosageData.setStartDate(dosage->getApplied());
//    dosageData.setLastDate(_resp->treatment()->dosages()->firsttake().addMSecs(_resp->predictions()->first()->prevCycle(sampleTime).mSecs()));
//    dosagesData.append(dosageData);

//    //Build the covariates
//    QList<CovariateData> covariatesData;

//    foreach (SharedPatientVariate covariate, *_resp->treatment()->currentCovariates(sampleDate)) {
//        CovariateData covariateData;
//        covariateData.setName(covariate->name());
//        covariateData.setDate(covariate->date());
//        covariateData.setUnit(covariate->unit());
//        covariateData.setValue(covariate->value());
//        covariateData.setNature("continous");

//        if (!covariateData.date().isValid())
//            covariateData.setDate(QDateTime(QDate(1970, 1, 1)));

//        covariatesData.append(covariateData);
//    }

//    //Compute the interest interval
//    Duration start = _resp->predictions()->first()->prevCycle(sampleDate);
//    Duration stop  = _resp->predictions()->first()->nextCycle(sampleDate);

//    if (start == stop)
//        start = _resp->predictions()->first()->prevCycle(sampleDate.addMSecs(-1));

//    QDateTime startDate = _resp->treatment()->dosages()->firsttake().addMSecs(start.mSecs());
//    QDateTime stopDate  = _resp->treatment()->dosages()->firsttake().addMSecs(stop.mSecs());

//    //Compute the interest points
//    DataSet points = _resp->points(SharedPrediction(_resp->predictions()->first()), start, stop, nbPoints, APOSTERIORI);
//    if (!points.size()) {
//        _errors = tr("Failed to compute the curve points from '%1' to '%2' (%3).").arg(startDate.toString(Action::dateTimeFormat()), stopDate.toString(Action::dateTimeFormat()), _resp->errorString());
//        return false;
//    }

//    //Build the parameters
//    QList<ParameterData> parametersData;

//    ParameterSet* tp = _resp->treatment()->parameters(sampleDate, POPULATION);
//    ParameterSet* pi = _resp->treatment()->parameters(sampleDate, APRIORI);
//    ParameterSet* po = _resp->treatment()->parameters(sampleDate, APOSTERIORI);
//    if (!tp->size() || !pi->size() || !po->size()) {
//        _errors = tr("Invalid parameters: could not find any parameters prior to the date '%1'.").arg(sampleDate.toString(Action::dateTimeFormat()));
//        return false;
//    }
//    if (tp->pids() != pi->pids() || tp->pids() != po->pids()) {
//        _errors = tr("Invalid parameters: the typical, a priori and/or a posteriori parameters are corrupted.").arg(sampleDate.toString(Action::dateTimeFormat()));
//        return false;
//    }

//    foreach (QString pid, tp->pids()) {
//        ParameterData parameterData;
//        parameterData.setName(pid);
//        parameterData.setDate(sampleDate);
//        parameterData.setTypicalValue(tp->get(pid)->value());
//        parameterData.setTypicalUnit(tp->get(pid)->unit());
//        parameterData.setAprioriValue(pi->get(pid)->value());
//        parameterData.setAprioriUnit(pi->get(pid)->unit());
//        parameterData.setAposterioriValue(po->get(pid)->value());
//        parameterData.setAposterioriUnit(po->get(pid)->unit());
//        parametersData.append(parameterData);
//    }

//    //Build the chart
//    QScopedPointer<Chart> chart(new Chart());
//    ChartItemGroup *chartGroup  = new ChartItemGroup();
//    chart->attachGroup(chartGroup);

//    //Build the curve
//    Curve *c_data = new Curve();
//    c_data->setDataset(startDate, stopDate, points);
//    chartGroup->attachItem(c_data);

//    //Build the percentiles
//    QList<int> pList;
//    foreach (Percentile p, percentiles)
//        pList << p.low() << p.high();

//    if (!pList.isEmpty()) {
//        QMap<int, DataSet> ps = _resp->percentiles(SharedPrediction(_resp->predictions()->first()), pList, start, stop, nbPoints, percentiles.type());

//        //Update the view for each of them
//        for (int i = 0; i < percentiles.size(); i++) {
//            Percentile p = percentiles.at(i);
//            PercentileCurve *pc = new PercentileCurve();
//            pc->setDataset(startDate, stopDate, ps.value(p.low()), ps.value(p.high()));
//            chartGroup->attachItem(pc);
//        }
//    }

//    //Show the sample
//    Dot *measureDot = new Dot();
//    measureDot->setPos(sampleDate, sample->amount().valueIn(points.unit()));
//    chartGroup->attachItem(measureDot);

//    //Show the targets
//    foreach (Target* target, *_resp->treatment()->targets().data()) {
//        TargetInterval *targetInterval = new TargetInterval(target->type());
//        targetInterval->setTarget(target);
//        chartGroup->attachItem(targetInterval);
//    }

//    //Configure and replot
//    chart->setTitle(_resp->treatment()->name());
//    chart->insertLegend(0);
//    chart->setAxesNames(tr("Time"), tr("Concentration") + " " + _resp->treatment()->drug()->concentrations()->unit().toString());
//    chart->setRange(startDate, stopDate);

//    //Build the image
//    QString filename = QDir::tempPath() + "/tmpchartexport.png";
//    if (!chart->save(filename, 800, 600)) {
//        _errors = tr("Failed to generate and save the chart image to file '%1'.").arg(filename);
//        return false;
//    }

//    QFile file(filename);
//    if (!file.open(QIODevice::ReadOnly)) {
//        _errors = tr("Failed to open the chart image from file '%1'.").arg(filename);
//        return false;
//    }

//    InterpretationData interpretationData = dataset.interpretation();
//    interpretationData.setCurve(file.readAll());

//    //Set the curve data
//    dataset.setInterpretation(interpretationData);
//    dataset.setDrug(drugData);
//    dataset.setDosages(dosagesData);
//    dataset.setSamples(samplesData);
//    dataset.setCovariates(covariatesData);
//    dataset.setParameters(parametersData);

//    return true;
//}

bool ExportAction::buildPatient(PatientData &patientData, const ident &patientId)
{
    Tucuxi::GuiCore::Response response;

    //The database objects
    SharedPatient patient;
//    Location location;

    //Ignore if not requested
    if (patientId == invalid_ident)
        return true;

    //Get the patient
//    response = DB->getPatient(patientId, patient);
//    if (response.error != NoError) {
//        _errors = tr(_DATABASE_READ_FAILED).arg(tr("patient"), tr("ID"), QString::number(patientId), response.message);
//        return false;
//    }

    response = ADMINREPO->getPatientFromId(patientId, patient);
    if (response.error != NoError) {
        _errors = tr(_DATABASE_READ_FAILED).arg(tr("patient"), tr("ID"), QString::number(patientId), response.message);
        return false;
    }

    //Build the name
    NameData nameData;
    nameData.setFirstname(static_cast<Patient*>(patient)->person()->firstname());
    nameData.setLastname(static_cast<Patient*>(patient)->person()->name());

    //Build the contact
    ContactData contactData;
    SharedPerson person = static_cast<Patient*>(patient)->person();
    if (!buildContact(contactData, person))
        return false;

    //Build the patient
    patientData.setBirthdate(QDateTime(static_cast<Patient*>(patient)->person()->birthday()));
    patientData.setGender(static_cast<Patient*>(patient)->person()->gender() == MALE ? "male" : "female");
    patientData.setId(static_cast<Patient*>(patient)->externalId().isEmpty() ? "unknown" : static_cast<Patient*>(patient)->externalId());
    patientData.setStayNumber(static_cast<Patient*>(patient)->stayNumber());
    patientData.setName(nameData);
    patientData.setContact(contactData);

    return true;
}

bool ExportAction::buildPractician(ExchangeAction::PracticianData &practicianData, const ident &practicianId)
{
    Tucuxi::GuiCore::Response response;

    //Check if valid
    if (practicianId == invalid_ident)
        return true;

    //The database objects
    SharedPractician practician;
//    Location location;

    //Ignore if not requested
    if (practicianId == invalid_ident)
        return true;

    //Get the practician
//    response = DB->getPractician(practicianId, practician);
    response = ADMINREPO->getPracticianFromId(practicianId, practician);
    if (response.error != NoError) {
        _errors = tr(_DATABASE_READ_FAILED).arg(tr("practician"), tr("ID"), QString::number(practicianId), response.message);
        return false;
    }

    //Build the name
    NameData nameData;
    nameData.setFirstname(practician->person()->firstname());
    nameData.setLastname(practician->person()->name());

    //Build the contact
    ContactData contactData;
    SharedPerson person = SharedPerson(practician->person());
    if (!buildContact(contactData, person))
        return false;

    //Build the institute
    InstituteData instituteData;
    SharedInstitute institute = SharedInstitute(practician->institute());
    if (!buildInstitute(instituteData, institute->id()))
        return false;

    //Build the practician
    practicianData.setBirthdate(QDateTime(practician->person()->birthday()));
    practicianData.setGender(practician->person()->gender() == Person::Male ? "male" : "female");
    practicianData.setId(practician->externalId().isEmpty() ? "unknown" : practician->externalId());
    practicianData.setTitle(practician->title());
    practicianData.setName(nameData);
    practicianData.setContact(contactData);
    practicianData.setInstitute(instituteData);

    //Set as valid
    practicianData.setIgnored(false);

    return true;
}

bool ExportAction::buildInstitute(ExchangeAction::InstituteData &instituteData, const ident &instituteId)
{
    Tucuxi::GuiCore::Response response;

    //Check if valid
    if (instituteId == invalid_ident)
        return true;

    //The database objects
    SharedInstitute institute;

    //Ignore if not requested
    if (instituteId == invalid_ident)
        return true;

    //Get the institute
//    response = DB->getInstitute(instituteId, institute);
    response = ADMINREPO->getInstituteFromId(instituteId, institute);
    if (response.error != NoError) {
        _errors = tr(_DATABASE_READ_FAILED).arg(tr("institute"), tr("ID"), QString::number(instituteId), response.message);
        return false;
    }

    //Build the contact
    ContactData contactData;
    // YTA : TODO : refactor
//    if (!buildContact(contactData, location, 0))
//        return false;

    //Build the institute
    instituteData.setId(institute->externalId().isEmpty() ? "unknown" : institute->externalId());
    instituteData.setName(institute->name());
    instituteData.setContact(contactData);

    //Set as valid
    instituteData.setIgnored(false);

    return true;
}

bool ExportAction::buildContact(ExchangeAction::ContactData &contactData, const SharedPerson &person)
{
    //Build the phones
    QList<PhoneData> phonesData;
   /* if (person)
        foreach (SharedPhone phone, person->phones()) {
            PhoneData phoneData;
            phoneData.setNumber(phone->getNumber());
            phoneData.setType(phone->typeToString());
            phonesData.append(phoneData);
        }
*/
    //Build the emails
    QList<EmailData> emailsData;
    if (person)
        foreach (Email* email, person->emails()) {
            EmailData emailData;
            emailData.setEmail(email->getEmail());
            emailData.setType("work");
            emailsData.append(emailData);
        }

    //Build the contact
    Location* location = person->location();
    contactData.setAddress(location->address());
    contactData.setCity(location->city());
    contactData.setCountry(location->country());
    contactData.setPostcode(location->postcode());
    contactData.setState(location->state());
    contactData.setPhones(phonesData);
    contactData.setEmails(emailsData);

    //Set as valid
    contactData.setIgnored(false);

    return true;
}

bool ExportAction::exportDatasets(const QString &filename)
{
    QFile file(filename);

    //Open the XML file
    if (!file.open(QIODevice::WriteOnly)) {
        _errors = tr(_FILE_OPEN_ERROR).arg(filename);
        return false;
    }

    //Export the document
    QTextStream out(&file);
    out << _document.toString();

    //Close the file
    file.close();

    //Validate the file
    if (!XMLVALIDATOR->validate(filename, XmlValidator::Export)) {
        _errors = tr(_INVALID_XML_FILE).arg(filename, QString::number(XMLVALIDATOR->errorLine()), XMLVALIDATOR->errorMessage());
        return false;
    }

    return true;
}

bool ExportAction::overrideFile(const QFile &file)
{
    //Check if the file exists
    if (file.exists()) {
        QTextStream qin(stdin);

        //Prompt the user
        LOG(QtDebugMsg, NOEZERROR, tr(_OVERRIDE_FILE).arg(QFileInfo(file).fileName()));

        //Process the answer
        QString answer;
        do {
            answer = qin.readLine();
        } while (answer != "y" && answer != "yes" && answer != "n" && answer != "no");

        //Cancel if refused
        if (answer == "n" || answer == "no")
            return false;
    }

    return true;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Data set
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExportAction::Dataset::_TAG_DATASET = "dataset";

QDomDocumentFragment ExportAction::Dataset::toXml(QDomDocument doc) const
{
    QDomDocumentFragment fragment = doc.createDocumentFragment();

    //Insert the dataset
    QDomElement dataset = doc.createElement(_TAG_DATASET);
    fragment.appendChild(dataset);

    //Insert the language
    QDomAttr langAttribute = doc.createAttribute(_ATTRIBUTE_LANG);
    langAttribute.setValue(lang());
    dataset.setAttributeNode(langAttribute);

    //Insert the interpretation
    dataset.appendChild(_interpretation.toXml(doc));
    dataset.appendChild(_drug.toXml(doc));
    dataset.appendChild(DosageData::toXml(doc, _dosages));
    dataset.appendChild(SampleData::toXml(doc, _samples));
    dataset.appendChild(CovariateData::toXml(doc, _covariates));
    dataset.appendChild(ParameterData::toXml(doc, _parameters));
    dataset.appendChild(ClinicalData::toXml(doc, _clinicals));
    dataset.appendChild(_patient.toXml(doc));
    dataset.appendChild(_mandator.toXml(doc, PracticianData::_TAG_MANDATOR));
    dataset.appendChild(_analyst.toXml(doc,  PracticianData::_TAG_ANALYST));

    //Return the document fragment
    return fragment;
}

QString ExportAction::Dataset::lang() const
{
    return _lang;
}

void ExportAction::Dataset::setLang(const QString &lang)
{
    _lang = lang;
}

ExportAction::InterpretationData ExportAction::Dataset::interpretation() const
{
    return _interpretation;
}

void ExportAction::Dataset::setInterpretation(const InterpretationData &interpretation)
{
    _interpretation = interpretation;
}

ExportAction::DrugData ExportAction::Dataset::drug() const
{
    return _drug;
}

void ExportAction::Dataset::setDrug(const DrugData &drug)
{
    _drug = drug;
}

ExportAction::PatientData ExportAction::Dataset::patient() const
{
    return _patient;
}

void ExportAction::Dataset::setPatient(const PatientData &patient)
{
    _patient = patient;
}

ExportAction::PracticianData ExportAction::Dataset::mandator() const
{
    return _mandator;
}

ExportAction::PracticianData ExportAction::Dataset::analyst() const
{
    return _analyst;
}

void ExportAction::Dataset::setAnalyst(const PracticianData &analyst)
{
    _analyst = analyst;
}

void ExportAction::Dataset::setMandator(const PracticianData &mandator)
{
    _mandator = mandator;
}

QList<ExportAction::DosageData> ExportAction::Dataset::dosages() const
{
    return _dosages;
}

void ExportAction::Dataset::setDosages(const QList<DosageData> &dosages)
{
    _dosages = dosages;
}

void ExportAction::Dataset::addDosage(const ExportAction::DosageData &dosage)
{
    _dosages.append(dosage);
}

QList<ExportAction::SampleData> ExportAction::Dataset::samples() const
{
    return _samples;
}

void ExportAction::Dataset::setSamples(const QList<SampleData> &samples)
{
    _samples = samples;
}

void ExportAction::Dataset::addSample(const ExportAction::SampleData &sample)
{
    _samples.append(sample);
}

QList<ExportAction::CovariateData> ExportAction::Dataset::covariates() const
{
    return _covariates;
}

void ExportAction::Dataset::setCovariates(const QList<CovariateData> &covariates)
{
    _covariates = covariates;
}

void ExportAction::Dataset::addCovariate(const ExportAction::CovariateData &covariate)
{
    _covariates.append(covariate);
}

QList<ExportAction::ParameterData> ExportAction::Dataset::parameters() const
{
    return _parameters;
}

void ExportAction::Dataset::setParameters(const QList<ParameterData> &parameters)
{
    _parameters = parameters;
}

void ExportAction::Dataset::addParameter(const ExchangeAction::ParameterData &parameter)
{
    _parameters.append(parameter);
}

QList<ExportAction::ClinicalData> ExportAction::Dataset::clinicals() const
{
    return _clinicals;
}

void ExportAction::Dataset::setClinicals(const QList<ClinicalData> &clinicals)
{
    _clinicals = clinicals;
}

void ExportAction::Dataset::addClinical(const ExportAction::ClinicalData &clinical)
{
    _clinicals.append(clinical);
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// Document data
// ////////////////////////////////////////////////////////////////////////////////////////////////

const char *const ExportAction::Document::_TAG_ROOT = "data";
const char *const ExportAction::Document::_ATTRIBUTE_VERSION = "version";
const char *const ExportAction::Document::_ATTRIBUTE_XSI = "xmlns:xsi";
const char *const ExportAction::Document::_ATTRIBUTE_SCHEMA_LOCATION = "xsi:noNamespaceSchemaLocation";

ExportAction::Document::Document()
{

}

QDomDocument ExportAction::Document::toXml() const
{
    //Create the DOM document
    QDomDocument doc;

    //Get the version number
    Version currentVersion(EXPORT_VERSION);

    //Insert the XML declaration
    QDomProcessingInstruction xmlDec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ");
    doc.appendChild(xmlDec);

    //Insert the XML stylesheet
    QDomProcessingInstruction xmlSty = doc.createProcessingInstruction("xml-stylesheet", "href=\"eep.xsl\" type=\"text/xsl\" ");
    doc.appendChild(xmlSty);

    //Insert the root tag
    QDomElement root = doc.createElement(_TAG_ROOT);
    doc.appendChild(root);

    //Set the version attribute
    QDomAttr version = doc.createAttribute(_ATTRIBUTE_VERSION);
    version.setValue(currentVersion.toString(false));
    root.setAttributeNode(version);

    //Set the XSI attribute
    QDomAttr xsi = doc.createAttribute(_ATTRIBUTE_XSI);
    xsi.setValue("http://www.w3.org/2001/XMLSchema-instance");
    root.setAttributeNode(xsi);

    //Set the location attribute
    QDomAttr schemaLoc = doc.createAttribute(_ATTRIBUTE_SCHEMA_LOCATION);
    schemaLoc.setValue("eep.xsd");
    root.setAttributeNode(schemaLoc);

    //Export the data of each dataset
    for (int i = 0; i < _datasets.size(); i++)
        root.appendChild(_datasets.at(i).toXml(doc));

    return doc;
}

QString ExportAction::Document::toString(int indent) const
{
    return toXml().toString(indent);
}

QList<ExportAction::Dataset> ExportAction::Document::datasets() const
{
    return _datasets;
}

void ExportAction::Document::setDatasets(const QList<Dataset> &datasets)
{
    _datasets = datasets;
}

void ExportAction::Document::addDataset(const ExportAction::Dataset &dataset)
{
    _datasets.append(dataset);
}

} //namespace cliutils
} //namespace Tucuxi
