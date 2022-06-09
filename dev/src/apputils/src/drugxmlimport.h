#ifndef DRUGXMLIMPORT_H
#define DRUGXMLIMPORT_H

#include <QXmlStreamReader>

#include <QTextStream>
#include <QFile>



namespace ezechiel {
namespace GuiCore {
class DrugModel;
class MetaData;
class EditorList;
class TargetList;
class DrugVariate;
class DrugVariateList;
class ParameterSet;
class Parameter;
class OperationList;
class TranslatableString;
}
}

class DrugXmlImport
{
public:
    DrugXmlImport();
    // The only method to be public
    ezechiel::GuiCore::DrugModel *load(const QString &fileName);

protected:
    QXmlStreamReader reader;

    bool loadHistory(ezechiel::GuiCore::DrugModel *drug);

    bool loadRevisions(ezechiel::GuiCore::MetaData* metaData);

    bool loadEditor(ezechiel::GuiCore::EditorList *editors);

    bool loadDrug(ezechiel::GuiCore::DrugModel* drug);

    bool loadHead(ezechiel::GuiCore::DrugModel* drug);

    bool loadAdme(ezechiel::GuiCore::DrugModel* drug);

    bool loadHalflife(ezechiel::GuiCore::DrugModel* drug);
    /**
     * @brief loadConversions
     * @param drug
     *
     * This method takes care of both the concentration and the result part of a drug
     *
     * @return true if everything went well false otherwise, furthermore `drug` isn't modified in case of problems
     */
    bool loadConversions(ezechiel::GuiCore::DrugModel* drug);
    /**
     * @brief loadDosages
     * @param drug
     *
     * takes care of doses, intervals and infusions
     *
     * @return
     */
    bool loadDosages(ezechiel::GuiCore::DrugModel* drug);

    bool loadStandardTreatment(ezechiel::GuiCore::DrugModel* drug);

    bool loadTargets(ezechiel::GuiCore::DrugModel* drug);

    bool loadTarget(ezechiel::GuiCore::TargetList* targetSet);

    bool loadCovariates(ezechiel::GuiCore::DrugModel* drug);

    bool loadCovariate(ezechiel::GuiCore::DrugVariateList *drugVariateSet);

    bool loadErrorModel(ezechiel::GuiCore::DrugModel* drug);

    bool loadParameters(ezechiel::GuiCore::DrugModel* drug);

    bool loadParameter(ezechiel::GuiCore::ParameterSet* parameterSet);

    bool loadBsv(ezechiel::GuiCore::Parameter* parameter);

    bool loadCorrelations(ezechiel::GuiCore::DrugModel *drug);

    bool loadOperations(ezechiel::GuiCore::DrugModel* drug);

    bool loadOperation(QMap<QString,ezechiel::GuiCore::OperationList*> &mapOperationList);

    bool stringLoaderTranslation(QString elemName, ezechiel::GuiCore::TranslatableString *QMapStringTranslation);

    ezechiel::GuiCore::TranslatableString* commentsLoader(bool &isOk);

    QString extractor(QXmlStreamAttributes *attributes = nullptr,bool readText = true);

    QMetaType::Type qStringToType(QString stringType,bool &isConvertible);

    QString errorString() const;

    void raiseConversionError();
};

#endif // DRUGXMLIMPORT_H
