#ifndef DRUGXMLEXPORT_H
#define DRUGXMLEXPORT_H

#include <QXmlStreamWriter>

#include <QTextStream>
#include <QFile>

#include "core/corefactory.h"
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
#include "core/dal/drug/metadata.h"
#include "core/dal/drug/editors.h"
#include "core/dal/drug/references.h"
#include "core/dal/drug/standardtreatment.h"

#include "core/dal/drug/operation.h"
#include "core/dal/drug/parameters.h"


class DrugXmlExport
{
public:
    DrugXmlExport();

    // The only method to be public
    bool save(Tucuxi::GuiCore::DrugModel *drug, const QString &fileName);

protected:
    QXmlStreamWriter writer;

    bool saveHistory(const Tucuxi::GuiCore::DrugModel *drug);

    bool saveEditor(const Tucuxi::GuiCore::Editor *editor);

    bool saveDrug(const Tucuxi::GuiCore::DrugModel *drug);

    bool saveHead(const Tucuxi::GuiCore::DrugModel *drug);

    bool saveAdme(const Tucuxi::GuiCore::ADME *adme);

    bool saveHalflife(const Tucuxi::GuiCore::Halflife *halflife);

    bool saveConversions(const Tucuxi::GuiCore::DrugModel *drug);

    bool saveStandardTreatment(const Tucuxi::GuiCore::DrugModel *drug);

    bool saveDosages(const Tucuxi::GuiCore::DrugModel *drug);

    bool saveTargets(Tucuxi::GuiCore::TargetList *targetSet);

    bool saveCovariates(Tucuxi::GuiCore::DrugVariateList * covariates);

    bool saveErrormodel(const Tucuxi::GuiCore::ErrorModel* errorModel);

    bool saveParameters(const Tucuxi::GuiCore::ParameterSet *parameters);

    bool saveCorrelations(const Tucuxi::GuiCore::DrugModel *drug);

    bool saveOperations(const Tucuxi::GuiCore::DrugModel *drug);

    bool tagInjector(const QString tagName, QString text, QXmlStreamAttributes *attributes = nullptr);

    bool stringSaverTranslation(Tucuxi::GuiCore::TranslatableString *QMapStringTranslation,QString elemName);

    QString typeToQString(QMetaType::Type type,bool &isConvertible);


    void writeNewLine();

};

#endif // DRUGXMLEXPORT_H
