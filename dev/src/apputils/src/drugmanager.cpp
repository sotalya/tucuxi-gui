//#include "core/interfaces/genericdatabase.h"
#include "apputils/src/apputilsrepository.h"
#include "core/corerepository.h"
#include "core/utils/errorenums.h"

#include "apputils/src/drugmanager.h"
//#include "coremessagehandler.h"
//#include "core_errors.h"
#include "core/core.h"
#include "core/drugxmldescriptor.h"

// All drug related DAL objects
#include "core/dal/drug/drug.h"
#include "core/dal/drug/parameters.h"
#include "core/dal/drug/drugvariate.h"

#include "core/pluginmanager.h"
#include "core/definitionmanager.h"

#include "core/corefactory.h"
#include "core/utils/logging.h"
#include "errors_apputils.h"

#include <QFile>
#include <QIODevice>
#include <QDebug>


#include <iostream>
#include <QtGlobal>


using namespace Tucuxi::Gui::Core;

namespace Tucuxi {
namespace Gui {
namespace AppUtils {

//Constructor
DrugManager::DrugManager()
{
//#ifndef COMPILE_PURE_TUCUCORE
//    scanDrugs();
//#endif // COMPILE_PURE_TUCUCORE
}

//Destructor
DrugManager::~DrugManager()
{
    resetDrugs();
}

//Checks if a drug exists
bool DrugManager::exists(const QString &drugId)
{
    return _drugIdToDrugDesc.contains(drugId);
}

//Scans a drug XML file
const DrugXmlDescriptor *DrugManager::scanDrug(const QString &filePath)
{
    //Parse the drug XML file
    QScopedPointer<const DrugXmlDescriptor> xmlDesc(_lister.scanDrug(filePath));

    //Return it only if valid
    if (!xmlDesc.isNull() && xmlDesc->isValid())
        return xmlDesc.take();

    //Set the error message otherwise
    _error = _lister.errorMessage();

    return nullptr;
}

bool DrugManager::tryToAddDrugModelToRepo(Tucuxi::Gui::Core::DrugModel *drugModel)
{
    // Added this to correct usage in InterpretationImport
    _drugIdToDrugObj.insert(drugModel->getDrugModelId(), drugModel);

    Response r;
    bool canAdd = true;
    QList<Tucuxi::Gui::Core::DrugModel* > existingList;
    if (ABSTRACTREPO == nullptr) {exit(4);}
    if (APPUTILSREPO == nullptr) {exit(5);}
    r = APPUTILSREPO->getDrugsList(existingList);
    if (r.error == NoError) {
        for(int i=0; i < existingList.size(); i++) {
            if (existingList.at(i)->getDrugModelId() == drugModel->getDrugModelId()) {
                LOG(QtWarningMsg, DRUGIDDUPLICATE, QString("The Id is : '%1'").arg(drugModel->getDrugModelId()));
                canAdd = false;
            }
        }
    }
    if (canAdd) {
        Response r = APPUTILSREPO->setDrug(drugModel);
        if (r.error != NoError) {
            LOG(QtWarningMsg, NOEZERROR, "Couldnt set drug, this error should be db error not here.")
                    return 0;
        }
    }
    return false;
}

//Returns the description of a drug
Descriptor DrugManager::description(const QString &drugId)
{
    return _drugIdToDrugDesc.value(drugId);
}

//Returns all the drugs descriptions
QList<Descriptor> DrugManager::descriptions()
{
    return _drugIdToDrugDesc.values();
}

//Returns all the models that can be used with this drug
QList<QString> DrugManager::models(const QString &drugId)
{
    return _drugIdToModelId.values(drugId);
}

//Returns all the drugs descriptions compatible with the given model
QList<Descriptor> DrugManager::descriptionsForModel(const QString &modelId)
{
    return _modelIdToDrugDesc.values(modelId);
}

//Returns the ATC of a drug
QString DrugManager::atc(const QString &drugId)
{
    return _drugIdToDrugAtc.value(drugId);
}

//Returns all the drugs description matching the given ATC
QList<Descriptor> DrugManager::descriptionsForAtc(const QString &atc)
{
    return _atcToDrugDesc.values(atc);
}

//Returns the domain of a drug
QString DrugManager::domain(const QString &drugId)
{
    return _drugIdToDrugDomain.value(drugId);
}

//Returns the study of a drug
QString DrugManager::study(const QString &drugId)
{
    return _drugIdToDrugStudy.value(drugId);
}

//Returns a drug
DrugModel* DrugManager::drug(const QString &drugId)
{
    QString _drugId = drugId;
    //if (!drugId.contains("ch.heig-vd.ezechiel")) {
        //change this.
    //    _drugId = "ch.heig-vd.ezechiel." + drugId.toLower();
    //}

    //Get the corresponding drug
    DrugModel* drug = _drugIdToDrugObj.value(_drugId);
    if (drug)
        return drug;

        OWNLOG(QtDebugMsg, NODRUG, tr("Drug not found with id %1").arg(drugId));
        return nullptr;
}

//Removes a drug from the manager
void DrugManager::removeDrug(const QString &drugId)
{
    //Get and delete the drug
    DrugModel* drug = _drugIdToDrugObj.take(drugId);
    Q_UNUSED(drug);

    //Get the drug descriptor
    Descriptor desc = _drugIdToDrugDesc.value(drugId);

    //Get the drug ATC
    QString atc = _drugIdToDrugAtc.value(drugId);

    //Get the drug models
    QList<QString> models = _drugIdToModelId.values(drugId);

    //Remove it from the models
    foreach (QString modelId, models)
        _modelIdToDrugDesc.remove(modelId, desc);

    //Remove it from the ATCs
    _atcToDrugDesc.remove(atc, desc);

    //Remove it from the maps
    _drugIdToDrugDomain.remove(drugId);
    _drugIdToDrugStudy.remove(drugId);
    _drugIdToDrugDesc.remove(drugId);
    _drugIdToDrugAtc.remove(drugId);
    _drugIdToModelId.remove(drugId);

    //Remove if from the lister
    _lister.removeDrug(drugId);
}

//Returns the directories list
QList<QDir> DrugManager::directories() const
{
    return _lister.directories();
}


//Returns the last error message
QString DrugManager::errorMessage() const
{
    return _error;
}


//Validate the model
bool DrugManager::validateModel(const DrugModel* drug)
{
    //Build the models list
    QList<QString> models;
    models.append(drug->getPkModelId());

    //Check if the model exists
    if (!CORE->pluginManager()->filter(models).isEmpty())
        return true;

    //Set the error message
    OWNLOG(QtWarningMsg, DATAERRMODEL, tr("ModelId: %1").arg(drug->getPkModelId()))

    return false;
}

//Validate the parameters
bool DrugManager::validateParameters(const DrugModel* drug)
{
    //Build the models list
    QList<QString> models;
    models.append(drug->getPkModelId());

    //Get the drug parameters set
    ParameterSet* parameters = drug->getParameters();
    Q_UNUSED(parameters);

    //Get the first available model
    //QString modelId = CORE->pluginManager()->filter(models).first();
/*
    //Construct the model
    SharedDrugModel model = SharedDrugModel(CORE->pluginManager()->newDrugModelEngine(modelId));

    //Check if parameters are available
    foreach (Descriptor desc, model->parametersDescription()) {
        if (!parameters->contains(desc.id)) {
            OWNLOG(QtWarningMsg, DATAERRPARAM, tr("DescId: %1").arg(desc.id));
            return false;
        }
    }
*/
    return true;
}

#if 0
bool DrugManager::validateScripts(const DrugModel *drug)
{
    bool resultOk = true;
    ParameterSet* parameters = drug->getParameters();
    DrugVariateList* covariates = drug->getCovariates();

    foreach(QString pid, parameters->pids()) {
        Parameter *parameter = parameters->get(pid);
        for (int i = 0; i < parameter->getOperations()->size(); ++i) {
            Operation *operation = parameter->getOperations()->at(i);

            QScriptEngine m_engine;    /*!< js interpreter for evaluating operations*/

            // Function to evaluate
            QString _formula = "function func () { " + operation->getFormula() + "} func();";

            QScriptSyntaxCheckResult validation = m_engine.checkSyntax(_formula);
            if (validation.state() != QScriptSyntaxCheckResult::Valid) {
                QString error = QString("\n\tDrug: %1. Parameter: %2. Script line: %3. Script column: %4. Message: %5")
                        .arg(drug->getDrugModelId())
                        .arg(parameter->getName())
                        .arg(validation.errorLineNumber())
                        .arg(validation.errorColumnNumber())
                        .arg(validation.errorMessage());
                OWNLOG(QtWarningMsg, SCRIPTERR, error);
                resultOk = false;
            }
            else {


                // Populate the parameters
                foreach(QString parid, parameters->pids()) {
                    Parameter *param = parameters->get(parid);
                    m_engine.globalObject().setProperty("parameter_" + param->getName(), param->getQuantity()->value());
                }

                // Populate the covariates
                for(int cov = 0; cov < covariates->size() ; cov++) {
                    DrugVariate *covariate = covariates->at(cov);
                    m_engine.globalObject().setProperty("covariate_" + covariate->getCovariateId(), covariate->getQuantity()->value());
                }


                //Call the newly created function
                QScriptValue _value = m_engine.evaluate(_formula);
                if (m_engine.hasUncaughtException()) {
                    QString error = QString("Drug: %1. Parameter: %2").arg(drug->getDrugModelId()).arg(parameter->getName());
                    OWNLOG(QtWarningMsg, SCRIPTERR, error);
                    resultOk = false;
                }
            }
        }
    }
    return resultOk;
}
#endif // 0

//Deletes the existing drugs
void DrugManager::resetDrugs()
{
    foreach (const DrugModel* drug, _drugIdToDrugObj.values())
        delete drug;
}

void DrugManager::analyzeCovariates()
{
    QMap<QString, int> covariatesMap;
    QList<Tucuxi::Gui::Core::DrugModel* > allDrugs;
    APPUTILSREPO->getDrugsList(allDrugs);
    foreach (Tucuxi::Gui::Core::DrugModel *drug, allDrugs) {
        foreach(Tucuxi::Gui::Core::DrugVariate *variate, drug->getCovariates()->getList()) {
            std::cout << qPrintable(variate->getCovariateId()) << std::endl;
            covariatesMap.insert(variate->getCovariateId(), covariatesMap.value(variate->getCovariateId(),0) + 1);
        }
    }

    std::cout << "Synthetic covariate maps:" << std::endl;
    QMapIterator<QString, int> i(covariatesMap);
      while (i.hasNext()) {
          i.next();
          std::cout << qPrintable(i.key()) << ": " << i.value() << std::endl;
      }
      std::cout << "End of synthetic covariate maps" << std::endl;
}

void DrugManager::clearErrorMessage()
{
    _error = "";
}

} //namespace Apputils
} //namespace Gui
} //namespace Tucuxi
