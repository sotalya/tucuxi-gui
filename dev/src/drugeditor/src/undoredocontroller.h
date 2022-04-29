//@@license@@

#ifndef UNDOREDOCONTROLLER_H
#define UNDOREDOCONTROLLER_H

//#include "QObject"
#include "QUndoStack"
#include "QQuickView"


//TODO clean following uneeded classes
class CommentListModel;
class CovariateListModel;
class CovariateModel;
class DomainListModel;
class DoseListModel;
class DrugListModel;
class StudyListModel;
class ValidationModel;
class EditorModel;
class EditorListModel;

class SortFilterEntityListProxyModel;
class ProxyModelFilter;

class RequestsController;

class Report;
class QPrinter;
class PersistentFileDialog;

class InterpretationRequest;
class Interpretation;

namespace ezechiel {
namespace core {
class DrugModel;
class CorePatient;
class DrugModelList;
class Editor;
class EditorList;
class TranslatableString;

class ValidDoses;
class DoseList;
class InfusionList;
class IntervalList;
class TargetList;

class ADME;
class Halflife;
class ErrorModel;

class Parameters;

class DrugVariateList;
class DrugVariate;

class Entity;
}
}



class UndoRedoController : public QQuickView
{

    Q_OBJECT
    Q_PROPERTY(ezechiel::core::DrugModelList* drugs READ getDrugs WRITE setDrugs NOTIFY drugsChanged)
    Q_PROPERTY(ezechiel::core::TranslatableString*  drugGeneralComments READ getDrugGeneralComments WRITE setDrugGeneralComments NOTIFY drugGeneralCommentsChanged)

    Q_PROPERTY(ezechiel::core::DrugModel* currentDrug READ getCurrentDrug WRITE setCurrentDrug NOTIFY currentDrugChanged)

    Q_PROPERTY(ezechiel::core::EditorList* editors READ getEditors WRITE setEditors NOTIFY editorsChanged)
    Q_PROPERTY(ezechiel::core::TranslatableString* editorComments READ getEditorComments WRITE setEditorComments NOTIFY editorCommentsChanged)

    Q_PROPERTY(ezechiel::core::DoseList* doses READ getDoses WRITE setDoses NOTIFY dosesChanged)
    Q_PROPERTY(ezechiel::core::TranslatableString* dosesComments READ getDosesComments WRITE setDosesComments NOTIFY dosesCommentsChanged)
    Q_PROPERTY(ezechiel::core::IntervalList* intervals READ getIntervals WRITE setIntervals NOTIFY intervalsChanged)
    Q_PROPERTY(ezechiel::core::InfusionList* infusions READ getInfusions WRITE setInfusions NOTIFY infusionsChanged)

    Q_PROPERTY(ezechiel::core::TargetList* targets READ getTargets WRITE setTargets NOTIFY targetsChanged)
    Q_PROPERTY(ezechiel::core::TranslatableString* targetComments READ getTargetComments WRITE setTargetComments NOTIFY targetCommentsChanged)

    Q_PROPERTY(ezechiel::core::ADME* adme READ getAdme WRITE setAdme NOTIFY admeChanged)
    Q_PROPERTY(ezechiel::core::TranslatableString* admeComments READ getAdmeComments WRITE setAdmeComments NOTIFY admeCommentsChanged)
    Q_PROPERTY(ezechiel::core::Halflife* halflife READ getHalflife WRITE setHalflife NOTIFY halflifeChanged)
    Q_PROPERTY(ezechiel::core::TranslatableString* halflifeComments READ getHalflifeComments WRITE setHalflifeComments NOTIFY halflifeCommentsChanged)
    Q_PROPERTY(ezechiel::core::ErrorModel* errorModel READ getErrorModel WRITE setErrorModel NOTIFY errorModelChanged)
    Q_PROPERTY(ezechiel::core::TranslatableString* errorModelComments READ getErrorModelComments WRITE setErrorModelComments NOTIFY errorModelCommentsChanged)


    Q_PROPERTY(ezechiel::core::Parameters* parameters READ getParameters WRITE setParameters NOTIFY parametersChanged)

    Q_PROPERTY(ezechiel::core::DrugVariateList* covariates READ getCovariates WRITE setCovariates NOTIFY covariatesChanged)


public:
    explicit UndoRedoController(QWindow* parent = 0);
    //drug
    ezechiel::core::DrugModelList* getDrugs() { return _drugs; }
    void setDrugs(ezechiel::core::DrugModelList* dlist) { _drugs = dlist; /*setModified(true);*/ emit drugsChanged(_drugs); }
    //drug general comments
    ezechiel::core::TranslatableString* getDrugGeneralComments() { return _drugGeneralComments; }
    void setDrugGeneralComments(ezechiel::core::TranslatableString* cList) { _drugGeneralComments = cList; emit drugGeneralCommentsChanged(_drugGeneralComments); }

    //current drug
    ezechiel::core::DrugModel* getCurrentDrug(){return  _currentDrug;}
    void setCurrentDrug(ezechiel::core::DrugModel* newDrug) { _currentDrug = newDrug; }

    //editors
    ezechiel::core::EditorList* getEditors() { return _editors; }
    void setEditors(ezechiel::core::EditorList* editors) { _editors = editors; }
    //editors comments
    ezechiel::core::TranslatableString* getEditorComments() { return _editorComments; }
    void setEditorComments(ezechiel::core::TranslatableString* comm) { _editorComments = comm; }

    //doses comments
    ezechiel::core::TranslatableString* getDosesComments() { return _dosesComments; }
    void setDosesComments(ezechiel::core::TranslatableString* transString){ _dosesComments = transString; emit dosesCommentsChanged(_dosesComments);}
    //doses
    ezechiel::core::DoseList* getDoses() { return _doses; }
    void setDoses(ezechiel::core::DoseList* doses) { _doses = doses;    }
    //intervals
    ezechiel::core::IntervalList* getIntervals() { return _intervals; }
    void setIntervals(ezechiel::core::IntervalList* intervals) { _intervals = intervals; }
    //infusions
    ezechiel::core::InfusionList* getInfusions() { return _infusions; }
    void setInfusions(ezechiel::core::InfusionList* infusions) { _infusions = infusions; }

    //targets
    ezechiel::core::TargetList* getTargets() { return _targets; }
    void setTargets(ezechiel::core::TargetList* targets) { _targets = targets; }
    //targetComments
    ezechiel::core::TranslatableString* getTargetComments() { return _targetComments; }
    void setTargetComments(ezechiel::core::TranslatableString* comment) { _targetComments = comment; }

    //ADME
    ezechiel::core::ADME* getAdme() { return _adme; }
    void setAdme(ezechiel::core::ADME *adme) { _adme = adme; }
    //ADME comments
    ezechiel::core::TranslatableString* getAdmeComments() { return _admeComments; }
    void setAdmeComments(ezechiel::core::TranslatableString* comment) { _admeComments = comment; }
    //Halflife
    ezechiel::core::Halflife*getHalflife() { return _halflife; }
    void setHalflife(ezechiel::core::Halflife* hl) { _halflife = hl; }
    //halflife comments
    ezechiel::core::TranslatableString* getHalflifeComments() { return _halflifeComments; }
    void setHalflifeComments(ezechiel::core::TranslatableString* comment) { _halflifeComments = comment; }
    //error_model
    ezechiel::core::ErrorModel* getErrorModel() { return _errorModel; }
    void setErrorModel(ezechiel::core::ErrorModel* errorModel) { _errorModel = errorModel; }
    //error_model comments
    ezechiel::core::TranslatableString* getErrorModelComments() { return _errorModelComments; }
    void setErrorModelComments(ezechiel::core::TranslatableString* comment) { _errorModelComments = comment; }

    //parameters
    ezechiel::core::Parameters* getParameters() { return _parameters; }
    void setParameters(ezechiel::core::Parameters *parameters) { _parameters = parameters; }

    //covariates
    ezechiel::core::DrugVariateList* getCovariates() { return _covariates; }
    void setCovariates(ezechiel::core::DrugVariateList* coavariates) { _covariates = coavariates; }


signals:
    void drugsChanged(ezechiel::core::DrugModelList*);
    void drugGeneralCommentsChanged(ezechiel::core::TranslatableString*);

    void currentDrugChanged(ezechiel::core::DrugModel*);

    void editorsChanged(ezechiel::core::EditorList*);
    void editorCommentsChanged(ezechiel::core::TranslatableString*);

    void dosesCommentsChanged(ezechiel::core::TranslatableString*);
    void dosesChanged(ezechiel::core::DoseList*);
    void intervalsChanged(ezechiel::core::IntervalList*);
    void infusionsChanged(ezechiel::core::InfusionList*);

    void targetsChanged(ezechiel::core::TargetList*);
    void targetCommentsChanged(ezechiel::core::TranslatableString*);

    void admeChanged(ezechiel::core::ADME*);
    void admeCommentsChanged(ezechiel::core::TranslatableString*);
    void halflifeChanged(ezechiel::core::Halflife*);
    void halflifeCommentsChanged(ezechiel::core::TranslatableString*);
    void errorModelChanged(ezechiel::core::ErrorModel*);
    void errorModelCommentsChanged(ezechiel::core::TranslatableString*);

    void parametersChanged(ezechiel::core::Parameters*);

    void covariatesChanged(ezechiel::core::DrugVariateList*);

public slots:
    void undo();
    void redo();
private:

    ezechiel::core::DrugModelList* _drugs;
    ezechiel::core::TranslatableString* _drugGeneralComments;


    ezechiel::core::EditorList *_editors;
    ezechiel::core::TranslatableString* _editorComments;

    ezechiel::core::TranslatableString* _dosesComments;
    ezechiel::core::DoseList* _doses;
    ezechiel::core::IntervalList* _intervals;
    ezechiel::core::InfusionList* _infusions;

    ezechiel::core::TranslatableString* _targetComments;
    ezechiel::core::TargetList* _targets;

    ezechiel::core::ADME* _adme;
    ezechiel::core::TranslatableString *_admeComments;
    ezechiel::core::Halflife* _halflife;
    ezechiel::core::TranslatableString *_halflifeComments;
    ezechiel::core::ErrorModel* _errorModel;
    ezechiel::core::TranslatableString *_errorModelComments;

    ezechiel::core::Parameters *_parameters;

    ezechiel::core::DrugVariateList* _covariates;


    void resetStaticModels();
    void resetDynamicModels();

    void initProxyModels();
    void initRootContext();
    void initQmlViews();
    void initConnexions();

    void refreshCurrentView();
    void undoPushator(QUndoCommand *cmd);

    QUndoStack *undoRedoStack;

    //models
    EditorModel         *editorModel;
    EditorListModel     *editorListModel;
    CommentListModel    *editorCommentsModel;
    DrugListModel       *drugListModel;
    CommentListModel    *drugCommentsModel;
    DomainListModel         *domainListModel;
    StudyListModel          *studyListModel;
    DoseListModel         *doseListModel;
    CommentListModel        *doseCommentsModel;


    SortFilterEntityListProxyModel *drugListProxyModel;
    SortFilterEntityListProxyModel *editorListProxyModel;
    SortFilterEntityListProxyModel *doseListProxyModel;


    QObject *mainView;
    QObject *flowView;
    QObject *drugsView;
    QObject *drugCommentsView;
    QObject *editorView;
    QObject *editorCommentsView;
    QObject *dosageView;
    QObject *dosageCommentsView;
    QObject *targetView;
    QObject *conversionView;
    QObject *parameterView;
    QObject *covariateView;

    ezechiel::core::DrugModel *_currentDrug;


    void covariatesSignal() { emit covariatesChanged(_covariates); }


private slots:

    void currentIndexChanged(QString view, int index);

    void drugIndexChanged(int index);
    void editorIndexChanged(int index);
    void dosageIndexChanged(int index);
    void targetIndexChanged(int index);

    void currentDrugChanged(QString view, int index);
    void currentEditorChanged(QString view, int index);
    void currentDosageChanged(QString view, int index);

    void drugEntityChanged(QString listViewName, int index);
    void editorEntityChanged(QString listViewName, int index);
    void dosageEntityChanged(QString listViewName, int index);
    void targetEntityChanged(QString listViewName, int index);
    void conversionEntityChanged(QString listViewName, int index);
    void parameterEntityChanged(QString listViewName, int index);
    void covariateEntityChanged(QString listViewName, int index);

    void drugDataChanged(QString propertyName, QVariant newText);
    void editorDataChanged(QString propertyName, QVariant newText);
    void dosageDataChanged(QString propertyName, QVariant newText);
    void targetDataChanged(QString propertyName, QVariant newText);
    void conversionDataChanged(QString propertyName, QVariant newText);

    void covariateDataChanged(QString propertyName, QVariant newText);

    void drugDataChanged();
    void editorDataChanged();
    void dosageDataChanged();

};

#endif // UNDOREDOCONTROLLER_H
