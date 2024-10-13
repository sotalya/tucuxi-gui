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


//#ifndef DRUGXMLEXPORT_H
//#define DRUGXMLEXPORT_H

//#include <QXmlStreamWriter>

//#include <QTextStream>
//#include <QFile>

//#include "core/corefactory.h"
//#include "core/dal/drug/drug.h"
//#include "core/dal/drug/adme.h"
//#include "core/dal/drug/halflife.h"
//#include "core/dal/drug/concentrations.h"
//#include "core/dal/drug/results.h"
//#include "core/dal/drug/doses.h"
//#include "core/dal/drug/intervals.h"
//#include "core/dal/drug/infusions.h"
//#include "core/dal/drug/target.h"
//#include "core/dal/drug/drugvariate.h"
//#include "core/dal/drug/errormodel.h"
//#include "core/dal/drug/bsv.h"
//#include "core/dal/drug/metadata.h"
//#include "core/dal/drug/editors.h"
//#include "core/dal/drug/references.h"
//#include "core/dal/drug/standardtreatment.h"

//#include "core/dal/drug/operation.h"
//#include "core/dal/drug/parameters.h"


//class DrugXmlExport
//{
//public:
//    DrugXmlExport();

//    // The only method to be public
//    bool save(Tucuxi::Gui::Core::DrugModel *drug, const QString &fileName);

//protected:
//    QXmlStreamWriter writer;

//    bool saveHistory(const Tucuxi::Gui::Core::DrugModel *drug);

//    bool saveEditor(const Tucuxi::Gui::Core::Editor *editor);

//    bool saveDrug(const Tucuxi::Gui::Core::DrugModel *drug);

//    bool saveHead(const Tucuxi::Gui::Core::DrugModel *drug);

//    bool saveAdme(const Tucuxi::Gui::Core::ADME *adme);

//    bool saveHalflife(const Tucuxi::Gui::Core::Halflife *halflife);

//    bool saveConversions(const Tucuxi::Gui::Core::DrugModel *drug);

//    bool saveStandardTreatment(const Tucuxi::Gui::Core::DrugModel *drug);

//    bool saveDosages(const Tucuxi::Gui::Core::DrugModel *drug);

//    bool saveTargets(Tucuxi::Gui::Core::TargetList *targetSet);

//    bool saveCovariates(Tucuxi::Gui::Core::DrugVariateList * covariates);

//    bool saveErrormodel(const Tucuxi::Gui::Core::ErrorModel* errorModel);

//    bool saveParameters(const Tucuxi::Gui::Core::ParameterSet *parameters);

//    bool saveCorrelations(const Tucuxi::Gui::Core::DrugModel *drug);

//    bool saveOperations(const Tucuxi::Gui::Core::DrugModel *drug);

//    bool tagInjector(const QString tagName, QString text, QXmlStreamAttributes *attributes = nullptr);

//    bool stringSaverTranslation(Tucuxi::Gui::Core::TranslatableString *QMapStringTranslation,QString elemName);

//    QString typeToQString(QMetaType::Type type,bool &isConvertible);


//    void writeNewLine();

//};

//#endif // DRUGXMLEXPORT_H
