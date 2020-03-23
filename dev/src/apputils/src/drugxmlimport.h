#ifndef DRUGXMLIMPORT_H
#define DRUGXMLIMPORT_H

#include <QXmlStreamReader>

#include <QTextStream>
#include <QFile>



namespace ezechiel {
namespace core {
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
    ezechiel::core::DrugModel *load(const QString &fileName);

protected:
    QXmlStreamReader reader;

    bool loadHistory(ezechiel::core::DrugModel *drug);

    bool loadRevisions(ezechiel::core::MetaData* metaData);

    bool loadEditor(ezechiel::core::EditorList *editors);

    bool loadDrug(ezechiel::core::DrugModel* drug);

    bool loadHead(ezechiel::core::DrugModel* drug);

    bool loadAdme(ezechiel::core::DrugModel* drug);

    bool loadHalflife(ezechiel::core::DrugModel* drug);
    /**
     * @brief loadConversions
     * @param drug
     *
     * This method takes care of both the concentration and the result part of a drug
     *
     * @return true if everything went well false otherwise, furthermore `drug` isn't modified in case of problems
     */
    bool loadConversions(ezechiel::core::DrugModel* drug);
    /**
     * @brief loadDosages
     * @param drug
     *
     * takes care of doses, intervals and infusions
     *
     * @return
     */
    bool loadDosages(ezechiel::core::DrugModel* drug);

    bool loadStandardTreatment(ezechiel::core::DrugModel* drug);

    bool loadTargets(ezechiel::core::DrugModel* drug);

    bool loadTarget(ezechiel::core::TargetList* targetSet);

    bool loadCovariates(ezechiel::core::DrugModel* drug);

    bool loadCovariate(ezechiel::core::DrugVariateList *drugVariateSet);

    bool loadErrorModel(ezechiel::core::DrugModel* drug);

    bool loadParameters(ezechiel::core::DrugModel* drug);

    bool loadParameter(ezechiel::core::ParameterSet* parameterSet);

    bool loadBsv(ezechiel::core::Parameter* parameter);

    bool loadCorrelations(ezechiel::core::DrugModel *drug);

    bool loadOperations(ezechiel::core::DrugModel* drug);

    bool loadOperation(QMap<QString,ezechiel::core::OperationList*> &mapOperationList);

    bool stringLoaderTranslation(QString elemName, ezechiel::core::TranslatableString *QMapStringTranslation);

    ezechiel::core::TranslatableString* commentsLoader(bool &isOk);

    QString extractor(QXmlStreamAttributes *attributes = nullptr,bool readText = true);

    QMetaType::Type qStringToType(QString stringType,bool &isConvertible);

    QString errorString() const;

    void raiseConversionError();
};

#endif // DRUGXMLIMPORT_H
