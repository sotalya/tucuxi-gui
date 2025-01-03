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


//#ifndef DRUGXMLIMPORT_H
//#define DRUGXMLIMPORT_H

//#include <QXmlStreamReader>

//#include <QTextStream>
//#include <QFile>



//namespace Tucuxi {
//namespace Gui {
//namespace Core {

//class DrugModel;
//class MetaData;
//class EditorList;
//class TargetList;
//class DrugVariate;
//class DrugVariateList;
//class ParameterSet;
//class Parameter;
//class OperationList;
//class TranslatableString;
//}
//}
//}

//class DrugXmlImport
//{
//public:
//    DrugXmlImport();
//    // The only method to be public
//    Tucuxi::Gui::Core::DrugModel *load(const QString &fileName);

//protected:
//    QXmlStreamReader reader;

//    bool loadHistory(Tucuxi::Gui::Core::DrugModel *drug);

//    bool loadRevisions(Tucuxi::Gui::Core::MetaData* metaData);

//    bool loadEditor(Tucuxi::Gui::Core::EditorList *editors);

//    bool loadDrug(Tucuxi::Gui::Core::DrugModel* drug);

//    bool loadHead(Tucuxi::Gui::Core::DrugModel* drug);

//    bool loadAdme(Tucuxi::Gui::Core::DrugModel* drug);

//    bool loadHalflife(Tucuxi::Gui::Core::DrugModel* drug);
//    /**
//     * @brief loadConversions
//     * @param drug
//     *
//     * This method takes care of both the concentration and the result part of a drug
//     *
//     * @return true if everything went well false otherwise, furthermore `drug` isn't modified in case of problems
//     */
//    bool loadConversions(Tucuxi::Gui::Core::DrugModel* drug);
//    /**
//     * @brief loadDosages
//     * @param drug
//     *
//     * takes care of doses, intervals and infusions
//     *
//     * @return
//     */
//    bool loadDosages(Tucuxi::Gui::Core::DrugModel* drug);

//    bool loadStandardTreatment(Tucuxi::Gui::Core::DrugModel* drug);

//    bool loadTargets(Tucuxi::Gui::Core::DrugModel* drug);

//    bool loadTarget(Tucuxi::Gui::Core::TargetList* targetSet);

//    bool loadCovariates(Tucuxi::Gui::Core::DrugModel* drug);

//    bool loadCovariate(Tucuxi::Gui::Core::DrugVariateList *drugVariateSet);

//    bool loadErrorModel(Tucuxi::Gui::Core::DrugModel* drug);

//    bool loadParameters(Tucuxi::Gui::Core::DrugModel* drug);

//    bool loadParameter(Tucuxi::Gui::Core::ParameterSet* parameterSet);

//    bool loadBsv(Tucuxi::Gui::Core::Parameter* parameter);

//    bool loadCorrelations(Tucuxi::Gui::Core::DrugModel *drug);

//    bool loadOperations(Tucuxi::Gui::Core::DrugModel* drug);

//    bool loadOperation(QMap<QString,Tucuxi::Gui::Core::OperationList*> &mapOperationList);

//    bool stringLoaderTranslation(QString elemName, Tucuxi::Gui::Core::TranslatableString *QMapStringTranslation);

//    Tucuxi::Gui::Core::TranslatableString* commentsLoader(bool &isOk);

//    QString extractor(QXmlStreamAttributes *attributes = nullptr,bool readText = true);

//    QMetaType::Type qStringToType(QString stringType,bool &isConvertible);

//    QString errorString() const;

//    void raiseConversionError();
//};

//#endif // DRUGXMLIMPORT_H
