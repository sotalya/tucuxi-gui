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
    bool save(ezechiel::core::DrugModel *drug, const QString &fileName);

protected:
    QXmlStreamWriter writer;

    bool saveHistory(const ezechiel::core::DrugModel *drug);

    bool saveEditor(const ezechiel::core::Editor *editor);

    bool saveDrug(const ezechiel::core::DrugModel *drug);

    bool saveHead(const ezechiel::core::DrugModel *drug);

    bool saveAdme(const ezechiel::core::ADME *adme);

    bool saveHalflife(const ezechiel::core::Halflife *halflife);

    bool saveConversions(const ezechiel::core::DrugModel *drug);

    bool saveStandardTreatment(const ezechiel::core::DrugModel *drug);

    bool saveDosages(const ezechiel::core::DrugModel *drug);

    bool saveTargets(ezechiel::core::TargetList *targetSet);

    bool saveCovariates(ezechiel::core::DrugVariateList * covariates);

    bool saveErrormodel(const ezechiel::core::ErrorModel* errorModel);

    bool saveParameters(const ezechiel::core::ParameterSet *parameters);

    bool saveCorrelations(const ezechiel::core::DrugModel *drug);

    bool saveOperations(const ezechiel::core::DrugModel *drug);

    bool tagInjector(const QString tagName, QString text, QXmlStreamAttributes *attributes = nullptr);

    bool stringSaverTranslation(ezechiel::core::TranslatableString *QMapStringTranslation,QString elemName);

    QString typeToQString(QMetaType::Type type,bool &isConvertible);


    void writeNewLine();

};

#endif // DRUGXMLEXPORT_H
