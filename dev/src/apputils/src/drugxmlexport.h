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
    bool save(ezechiel::GuiCore::DrugModel *drug, const QString &fileName);

protected:
    QXmlStreamWriter writer;

    bool saveHistory(const ezechiel::GuiCore::DrugModel *drug);

    bool saveEditor(const ezechiel::GuiCore::Editor *editor);

    bool saveDrug(const ezechiel::GuiCore::DrugModel *drug);

    bool saveHead(const ezechiel::GuiCore::DrugModel *drug);

    bool saveAdme(const ezechiel::GuiCore::ADME *adme);

    bool saveHalflife(const ezechiel::GuiCore::Halflife *halflife);

    bool saveConversions(const ezechiel::GuiCore::DrugModel *drug);

    bool saveStandardTreatment(const ezechiel::GuiCore::DrugModel *drug);

    bool saveDosages(const ezechiel::GuiCore::DrugModel *drug);

    bool saveTargets(ezechiel::GuiCore::TargetList *targetSet);

    bool saveCovariates(ezechiel::GuiCore::DrugVariateList * covariates);

    bool saveErrormodel(const ezechiel::GuiCore::ErrorModel* errorModel);

    bool saveParameters(const ezechiel::GuiCore::ParameterSet *parameters);

    bool saveCorrelations(const ezechiel::GuiCore::DrugModel *drug);

    bool saveOperations(const ezechiel::GuiCore::DrugModel *drug);

    bool tagInjector(const QString tagName, QString text, QXmlStreamAttributes *attributes = nullptr);

    bool stringSaverTranslation(ezechiel::GuiCore::TranslatableString *QMapStringTranslation,QString elemName);

    QString typeToQString(QMetaType::Type type,bool &isConvertible);


    void writeNewLine();

};

#endif // DRUGXMLEXPORT_H
