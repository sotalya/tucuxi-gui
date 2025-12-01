/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "drugs2manager.h"

#include "core/utils/logging.h"

#include "tucucore/drugmodelimport.h"
#include "processingtucucore/src/tucucoretoguitranslator.h"
#include "tucucore/drugmodel/drugmodel.h"
#include "tucucore/drugmodelchecker.h"
#include "tucucore/pkmodel.h"

#ifdef CONFIG_SIGN
#include "tucusign/signvalidator.h"
#include "tucusign/signparser.h"

#include "core/dal/drug/drug.h"
#endif // CONFIG_SIGN

namespace Tucuxi {
namespace Gui {
namespace Processing {

Drugs2Manager::Drugs2Manager() = default;


Drugs2Manager *Drugs2Manager::getInstance()
{
    static auto *manager = new Drugs2Manager();
    return manager;
}


const std::vector<Tucuxi::Core::DrugModel *> & Drugs2Manager::getAllTucucoreDrugModels() const
{
    return m_tucuxiDrugModels;
}

const std::vector<Tucuxi::Gui::Core::DrugModel *> & Drugs2Manager::getAllGuiDrugModels() const
{
    return m_guiDrugModels;
}

Tucuxi::Core::DrugModel * Drugs2Manager::getTucucoreById(std::string id) const
{
    for(auto model : m_tucuxiDrugModels) {
        if (model->getDrugModelId() == id) {
            return model;
        }
    }
    return nullptr;
}

#ifdef CONFIG_SIGN
Tucuxi::Sign::Signer /*bool*/ Drugs2Manager::checkSign(std::string fileName)
{
#ifdef CONFIG_SIGN
    std::string signedDrugfilePath = fileName;
    Tucuxi::Sign::Signature signature;

    // load signature from drug file
    Tucuxi::Sign::ParsingError parsingResponse =
            Tucuxi::Sign::SignParser::loadSignature(signedDrugfilePath, signature);

    Tucuxi::Sign::Signer signer;

    if (parsingResponse == Tucuxi::Sign::ParsingError::UNABLE_TO_LOAD_DRUGFILE)
        std::cout << "Unable to load drug file" << std::endl;

    if (parsingResponse == Tucuxi::Sign::ParsingError::SIGNATURE_OK) {
        // validate signature
        Tucuxi::Sign::SignatureError signatureResponse =
                Tucuxi::Sign::SignValidator::validateSignature(signature);
        std::cout << "Signature OK" << std::endl;

        if (true /*signatureResponse == Tucuxi::Sign::SignatureError::SIGNATURE_VALID*/) {
            //get signer info
            signer = Tucuxi::Sign::SignValidator::loadSigner(signature.getUserCert());
            // print signer info
            std::cout << "\nThe drug file has been signed by: \n"
                      << signer /*Tucuxi::Sign::SignValidator::loadSigner(signature.getUserCert())*/ << std::endl;

            //             return signer;
        }
    }
    else {
        std::cout << "Signature NOK !" << std::endl;
    }

    return signer;

#else // CONFIG_SIGN
    return false;

#endif // CONFIG_SIGN
}
#endif // CONFIG_SIGN

#ifdef CONFIG_SIGN

Tucuxi::Sign::Signer Drugs2Manager::checkSign(const Tucuxi::Gui::Core::DrugModel* drugModel)
{
    return checkSign(drugModel->getFilePath().toUtf8().toStdString());
}

#endif // CONFIG_SIGN

Tucuxi::Core::DrugModel* Drugs2Manager::scanDrug(const QString & fileName)
{
    Tucuxi::Core::DrugModelImport importer;

    //result = importer.importFromFile(pDrugModel, fileName.toStdString());

    // We read the file and use the std::string for the importer, as the
    // file could be a resource. If this is the case, we have to use Qt
    // files to open it, as it is not supported by the standard library
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly)) {
        return nullptr;
    }
    std::string content = f.readAll().data();
    std::unique_ptr<Tucuxi::Core::DrugModel> uniqueDrugModel;
    Tucuxi::Common::IImport::Status result = importer.importFromString(uniqueDrugModel, content);
    Tucuxi::Core::DrugModel *pDrugModel = uniqueDrugModel.release();

    if (result != Tucuxi::Common::IImport::Status::Ok) {
        return nullptr;
    }

    Tucuxi::Core::PkModelCollection pkCollection;

    if (!defaultPopulate(pkCollection)) {
        //logHelper.error("Could not populate the Pk models collection. No model will be available");
        return nullptr;
    }

    Tucuxi::Core::DrugModelChecker checker;
    Tucuxi::Core::DrugModelChecker::CheckerResult_t checkerResult = checker.checkDrugModel(pDrugModel, &pkCollection);
    if (!checkerResult.m_ok) {
        //logHelper.error("The drug file {} has internal errors : {}", _fileName, checkerResult.errorMessage);
        return nullptr;
    }
    else {
        //logHelper.info("Successfully loaded drug model : {}", drugModel->getDrugModelId());
        return pDrugModel;
    }

    return nullptr;
}


//Scans a directory recursively
void Drugs2Manager::scanDirectory(const QDir &directory)
{
    //    LOG(QtDebugMsg, Tucuxi::Gui::AppUtils::NOEZERROR, tr("Scanning directory '%1'").arg(directory.absolutePath()));

    //For each files in the directory
    foreach (QFileInfo entry, directory.entryInfoList(QDir::Files)) {
        Tucuxi::Core::DrugModel *drug = nullptr;

        //Ignore non-XML files
        if (entry.suffix().toLower() != "tdd") {
            //LOG(QtWarningMsg, NOEZERROR, tr(_FILE_BAD_EXTENSION).arg(entry.absoluteFilePath()) + tr(_IGNORING));
            continue;
        }

        //Ignore unreadable files
        if (!entry.isReadable()) {
            //           LOG(QtWarningMsg, NOEZERROR, tr(_FILE_UNREADABLE).arg(entry.absoluteFilePath()) + tr(_IGNORING));
            continue;
        }

        //Scan the current drug XML file
        if (!(drug = scanDrug(entry.filePath())))
            continue;

        //Check if the drug does not already exists
        if (m_tucuxiDrugModelsByIds.count(drug->getDrugModelId()) != 0) {
            //           LOG(QtWarningMsg, NOEZERROR, tr(_DRUG_DUPLICATED).arg(entry.filePath(), drug->drugId()) + tr(_IGNORING));
            continue;
        }

        //        std::cout << drug->getDrugId() << std::endl;

        TucucoreToGuiTranslator translator;
        Tucuxi::Gui::Core::DrugModel *newModel = translator.buildLightDrugModel(drug, entry.filePath());
        if (newModel != nullptr) {
            // Store the Tucuxi and ezechiel drug models
            m_tucuxiDrugModelsByIds[drug->getDrugModelId()] = drug;
            m_tucuxiDrugModels.push_back(drug);
            m_guiDrugModels.push_back(newModel);
        }

        //----------------------------------------------------------------------------------------------------------------

        //        std::cout << drug->getDrugModelId() << std::endl;

        //        std::string localDrugModelsPath = "/home/julien/Documents/tucuxi-drugs/drugfiles/";
        //        localDrugModelsPath += drug->getDrugModelId() + ".tdd";
        //        std::cout << localDrugModelsPath << std::endl;
        //        Tucuxi::Sign::Signer signer = this->checkSign(localDrugModelsPath + drug->getDrugModelId() + "tdd");

        //----------------------------------------------------------------------------------------------------------------

    }

    //Recursively scan each subdirectories
    foreach (QFileInfo subdir, directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
        scanDirectory(QDir(subdir.absoluteFilePath()));
}

#include <QDir>

void Drugs2Manager::buildAllDrugModels(std::string dirName)
{
    std::vector<Tucuxi::Gui::Core::DrugModel *> list;

    QList<QDir> _directories;
    _directories << QDir(QString::fromStdString(dirName));

    //Scan each directories
    foreach (QDir directory, _directories) {

        //Ignore unreadable directories
        if (!directory.isReadable()) {
            //    LOG(QtDebugMsg, NOEZERROR, tr(_DIRECTORY_UNREADABLE).arg(directory.absolutePath()) + tr(_IGNORING));
            continue;
        }

        //Scan the directory
        scanDirectory(directory);
    }

#ifdef DRUGMODELTESTS
    addDrugModelTests();
#endif // DRUGMODELTESTS

}

}
}
}

#ifdef DRUGMODELTESTS

#include "tucucore/../../test/tucucore/drugmodels/buildconstantelimination.h"
#include "tucucore/../../test/tucucore/drugmodels/buildpkasymptotic.h"
#include "tucucore/../../test/tucucore/drugmodels/buildmultianalytessingleactivemoiety.h"
// #include "tucucore/../../test/tucucore/drugmodels/buildmultianalytesmultiactivemoieties.h"

void Tucuxi::Gui::Processing::Drugs2Manager::addDrugModelTests()
{
    {

        BuildConstantElimination builder;
        auto drug = builder.buildDrugModel(
                    ResidualErrorType::PROPORTIONAL,std::vector<Value>({0.2}));
        // Add targets
        TargetDefinition *target = new TargetDefinition(TargetType::Residual,
                                                        Unit("mg/l"),
                                                        ActiveMoietyId("analyte"),
                                                        std::make_unique<SubTargetDefinition>("cMin", 750.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("cMax", 1500.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("cBest", 1000.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("mic", 0.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("tMin", 1000.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("tMax", 1000.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("tBest", 1000.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("toxicity", 10000.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("inefficacy", 000.0, nullptr));

        drug->m_activeMoieties[0]->addTarget(std::unique_ptr<TargetDefinition>(target));

        TucucoreToGuiTranslator translator;
        Tucuxi::Gui::Core::DrugModel *newModel = translator.buildLightDrugModel(drug.get(),"");
        if (newModel != nullptr) {
            // Store the Tucuxi and ezechiel drug models
            m_tucuxiDrugModelsByIds[drug->getDrugModelId()] = drug.get();
            m_tucuxiDrugModels.push_back(drug.release());
            m_guiDrugModels.push_back(newModel);
        }
    }
    {

        BuildPkAsymptotic builder;
        auto drug = builder.buildDrugModel(
                    ResidualErrorType::PROPORTIONAL,std::vector<Value>({0.2}));
        // Add targets
        TargetDefinition *target = new TargetDefinition(TargetType::Residual,
                                                        Unit("mg/l"),
                                                        ActiveMoietyId("analyte"),
                                                        std::make_unique<SubTargetDefinition>("cMin", 750.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("cMax", 1500.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("cBest", 1000.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("mic", 0.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("tMin", 1000.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("tMax", 1000.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("tBest", 1000.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("toxicity", 10000.0, nullptr),
                                                        std::make_unique<SubTargetDefinition>("inefficacy", 000.0, nullptr));

        drug->m_activeMoieties[0]->addTarget(std::unique_ptr<TargetDefinition>(target));

        TucucoreToGuiTranslator translator;
        Tucuxi::Gui::Core::DrugModel *newModel = translator.buildLightDrugModel(drug.get(),"");
        if (newModel != nullptr) {
            // Store the Tucuxi and ezechiel drug models
            m_tucuxiDrugModelsByIds[drug->getDrugModelId()] = drug.get();
            m_tucuxiDrugModels.push_back(drug.release());
            m_guiDrugModels.push_back(newModel);
        }
    }
    {
        BuildMultiAnalytesSingleActiveMoiety builder;
        auto drug = builder.buildDrugModel(0.3,
                                           0.7,
                                           ResidualErrorType::ADDITIVE,
        {100.0},
                                           Tucuxi::Core::ParameterVariabilityType::Additive,
                                           Tucuxi::Core::ParameterVariabilityType::None,
                                           Tucuxi::Core::ParameterVariabilityType::None,
                                           Tucuxi::Core::ParameterVariabilityType::None,
                                           10.0,
                                           0.0,
                                           0.0,
                                           0.0);

        TucucoreToGuiTranslator translator;
        Tucuxi::Gui::Core::DrugModel *newModel = translator.buildLightDrugModel(drug.get(),"");
        if (newModel != nullptr) {
            // Store the Tucuxi and ezechiel drug models
            m_tucuxiDrugModelsByIds[drug->getDrugModelId()] = drug.get();
            m_tucuxiDrugModels.push_back(drug.release());
            m_guiDrugModels.push_back(newModel);
        }
    }
}
#endif // DRUGMODELTESTS
