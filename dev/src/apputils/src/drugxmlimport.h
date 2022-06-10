#ifndef DRUGXMLIMPORT_H
#define DRUGXMLIMPORT_H

#include <QXmlStreamReader>

#include <QTextStream>
#include <QFile>



namespace Tucuxi {
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
    Tucuxi::GuiCore::DrugModel *load(const QString &fileName);

protected:
    QXmlStreamReader reader;

    bool loadHistory(Tucuxi::GuiCore::DrugModel *drug);

    bool loadRevisions(Tucuxi::GuiCore::MetaData* metaData);

    bool loadEditor(Tucuxi::GuiCore::EditorList *editors);

    bool loadDrug(Tucuxi::GuiCore::DrugModel* drug);

    bool loadHead(Tucuxi::GuiCore::DrugModel* drug);

    bool loadAdme(Tucuxi::GuiCore::DrugModel* drug);

    bool loadHalflife(Tucuxi::GuiCore::DrugModel* drug);
    /**
     * @brief loadConversions
     * @param drug
     *
     * This method takes care of both the concentration and the result part of a drug
     *
     * @return true if everything went well false otherwise, furthermore `drug` isn't modified in case of problems
     */
    bool loadConversions(Tucuxi::GuiCore::DrugModel* drug);
    /**
     * @brief loadDosages
     * @param drug
     *
     * takes care of doses, intervals and infusions
     *
     * @return
     */
    bool loadDosages(Tucuxi::GuiCore::DrugModel* drug);

    bool loadStandardTreatment(Tucuxi::GuiCore::DrugModel* drug);

    bool loadTargets(Tucuxi::GuiCore::DrugModel* drug);

    bool loadTarget(Tucuxi::GuiCore::TargetList* targetSet);

    bool loadCovariates(Tucuxi::GuiCore::DrugModel* drug);

    bool loadCovariate(Tucuxi::GuiCore::DrugVariateList *drugVariateSet);

    bool loadErrorModel(Tucuxi::GuiCore::DrugModel* drug);

    bool loadParameters(Tucuxi::GuiCore::DrugModel* drug);

    bool loadParameter(Tucuxi::GuiCore::ParameterSet* parameterSet);

    bool loadBsv(Tucuxi::GuiCore::Parameter* parameter);

    bool loadCorrelations(Tucuxi::GuiCore::DrugModel *drug);

    bool loadOperations(Tucuxi::GuiCore::DrugModel* drug);

    bool loadOperation(QMap<QString,Tucuxi::GuiCore::OperationList*> &mapOperationList);

    bool stringLoaderTranslation(QString elemName, Tucuxi::GuiCore::TranslatableString *QMapStringTranslation);

    Tucuxi::GuiCore::TranslatableString* commentsLoader(bool &isOk);

    QString extractor(QXmlStreamAttributes *attributes = nullptr,bool readText = true);

    QMetaType::Type qStringToType(QString stringType,bool &isConvertible);

    QString errorString() const;

    void raiseConversionError();
};

#endif // DRUGXMLIMPORT_H
