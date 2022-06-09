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
namespace GuiCore {
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
    Q_PROPERTY(ezechiel::GuiCore::DrugModelList* drugs READ getDrugs WRITE setDrugs NOTIFY drugsChanged)
    Q_PROPERTY(ezechiel::GuiCore::TranslatableString*  drugGeneralComments READ getDrugGeneralComments WRITE setDrugGeneralComments NOTIFY drugGeneralCommentsChanged)

    Q_PROPERTY(ezechiel::GuiCore::DrugModel* currentDrug READ getCurrentDrug WRITE setCurrentDrug NOTIFY currentDrugChanged)

    Q_PROPERTY(ezechiel::GuiCore::EditorList* editors READ getEditors WRITE setEditors NOTIFY editorsChanged)
    Q_PROPERTY(ezechiel::GuiCore::TranslatableString* editorComments READ getEditorComments WRITE setEditorComments NOTIFY editorCommentsChanged)

    Q_PROPERTY(ezechiel::GuiCore::DoseList* doses READ getDoses WRITE setDoses NOTIFY dosesChanged)
    Q_PROPERTY(ezechiel::GuiCore::TranslatableString* dosesComments READ getDosesComments WRITE setDosesComments NOTIFY dosesCommentsChanged)
    Q_PROPERTY(ezechiel::GuiCore::IntervalList* intervals READ getIntervals WRITE setIntervals NOTIFY intervalsChanged)
    Q_PROPERTY(ezechiel::GuiCore::InfusionList* infusions READ getInfusions WRITE setInfusions NOTIFY infusionsChanged)

    Q_PROPERTY(ezechiel::GuiCore::TargetList* targets READ getTargets WRITE setTargets NOTIFY targetsChanged)
    Q_PROPERTY(ezechiel::GuiCore::TranslatableString* targetComments READ getTargetComments WRITE setTargetComments NOTIFY targetCommentsChanged)

    Q_PROPERTY(ezechiel::GuiCore::ADME* adme READ getAdme WRITE setAdme NOTIFY admeChanged)
    Q_PROPERTY(ezechiel::GuiCore::TranslatableString* admeComments READ getAdmeComments WRITE setAdmeComments NOTIFY admeCommentsChanged)
    Q_PROPERTY(ezechiel::GuiCore::Halflife* halflife READ getHalflife WRITE setHalflife NOTIFY halflifeChanged)
    Q_PROPERTY(ezechiel::GuiCore::TranslatableString* halflifeComments READ getHalflifeComments WRITE setHalflifeComments NOTIFY halflifeCommentsChanged)
    Q_PROPERTY(ezechiel::GuiCore::ErrorModel* errorModel READ getErrorModel WRITE setErrorModel NOTIFY errorModelChanged)
    Q_PROPERTY(ezechiel::GuiCore::TranslatableString* errorModelComments READ getErrorModelComments WRITE setErrorModelComments NOTIFY errorModelCommentsChanged)


    Q_PROPERTY(ezechiel::GuiCore::Parameters* parameters READ getParameters WRITE setParameters NOTIFY parametersChanged)

    Q_PROPERTY(ezechiel::GuiCore::DrugVariateList* covariates READ getCovariates WRITE setCovariates NOTIFY covariatesChanged)


public:
    explicit UndoRedoController(QWindow* parent = 0);
    //drug
    ezechiel::GuiCore::DrugModelList* getDrugs() { return _drugs; }
    void setDrugs(ezechiel::GuiCore::DrugModelList* dlist) { _drugs = dlist; /*setModified(true);*/ emit drugsChanged(_drugs); }
    //drug general comments
    ezechiel::GuiCore::TranslatableString* getDrugGeneralComments() { return _drugGeneralComments; }
    void setDrugGeneralComments(ezechiel::GuiCore::TranslatableString* cList) { _drugGeneralComments = cList; emit drugGeneralCommentsChanged(_drugGeneralComments); }

    //current drug
    ezechiel::GuiCore::DrugModel* getCurrentDrug(){return  _currentDrug;}
    void setCurrentDrug(ezechiel::GuiCore::DrugModel* newDrug) { _currentDrug = newDrug; }

    //editors
    ezechiel::GuiCore::EditorList* getEditors() { return _editors; }
    void setEditors(ezechiel::GuiCore::EditorList* editors) { _editors = editors; }
    //editors comments
    ezechiel::GuiCore::TranslatableString* getEditorComments() { return _editorComments; }
    void setEditorComments(ezechiel::GuiCore::TranslatableString* comm) { _editorComments = comm; }

    //doses comments
    ezechiel::GuiCore::TranslatableString* getDosesComments() { return _dosesComments; }
    void setDosesComments(ezechiel::GuiCore::TranslatableString* transString){ _dosesComments = transString; emit dosesCommentsChanged(_dosesComments);}
    //doses
    ezechiel::GuiCore::DoseList* getDoses() { return _doses; }
    void setDoses(ezechiel::GuiCore::DoseList* doses) { _doses = doses;    }
    //intervals
    ezechiel::GuiCore::IntervalList* getIntervals() { return _intervals; }
    void setIntervals(ezechiel::GuiCore::IntervalList* intervals) { _intervals = intervals; }
    //infusions
    ezechiel::GuiCore::InfusionList* getInfusions() { return _infusions; }
    void setInfusions(ezechiel::GuiCore::InfusionList* infusions) { _infusions = infusions; }

    //targets
    ezechiel::GuiCore::TargetList* getTargets() { return _targets; }
    void setTargets(ezechiel::GuiCore::TargetList* targets) { _targets = targets; }
    //targetComments
    ezechiel::GuiCore::TranslatableString* getTargetComments() { return _targetComments; }
    void setTargetComments(ezechiel::GuiCore::TranslatableString* comment) { _targetComments = comment; }

    //ADME
    ezechiel::GuiCore::ADME* getAdme() { return _adme; }
    void setAdme(ezechiel::GuiCore::ADME *adme) { _adme = adme; }
    //ADME comments
    ezechiel::GuiCore::TranslatableString* getAdmeComments() { return _admeComments; }
    void setAdmeComments(ezechiel::GuiCore::TranslatableString* comment) { _admeComments = comment; }
    //Halflife
    ezechiel::GuiCore::Halflife*getHalflife() { return _halflife; }
    void setHalflife(ezechiel::GuiCore::Halflife* hl) { _halflife = hl; }
    //halflife comments
    ezechiel::GuiCore::TranslatableString* getHalflifeComments() { return _halflifeComments; }
    void setHalflifeComments(ezechiel::GuiCore::TranslatableString* comment) { _halflifeComments = comment; }
    //error_model
    ezechiel::GuiCore::ErrorModel* getErrorModel() { return _errorModel; }
    void setErrorModel(ezechiel::GuiCore::ErrorModel* errorModel) { _errorModel = errorModel; }
    //error_model comments
    ezechiel::GuiCore::TranslatableString* getErrorModelComments() { return _errorModelComments; }
    void setErrorModelComments(ezechiel::GuiCore::TranslatableString* comment) { _errorModelComments = comment; }

    //parameters
    ezechiel::GuiCore::Parameters* getParameters() { return _parameters; }
    void setParameters(ezechiel::GuiCore::Parameters *parameters) { _parameters = parameters; }

    //covariates
    ezechiel::GuiCore::DrugVariateList* getCovariates() { return _covariates; }
    void setCovariates(ezechiel::GuiCore::DrugVariateList* coavariates) { _covariates = coavariates; }


signals:
    void drugsChanged(ezechiel::GuiCore::DrugModelList*);
    void drugGeneralCommentsChanged(ezechiel::GuiCore::TranslatableString*);

    void currentDrugChanged(ezechiel::GuiCore::DrugModel*);

    void editorsChanged(ezechiel::GuiCore::EditorList*);
    void editorCommentsChanged(ezechiel::GuiCore::TranslatableString*);

    void dosesCommentsChanged(ezechiel::GuiCore::TranslatableString*);
    void dosesChanged(ezechiel::GuiCore::DoseList*);
    void intervalsChanged(ezechiel::GuiCore::IntervalList*);
    void infusionsChanged(ezechiel::GuiCore::InfusionList*);

    void targetsChanged(ezechiel::GuiCore::TargetList*);
    void targetCommentsChanged(ezechiel::GuiCore::TranslatableString*);

    void admeChanged(ezechiel::GuiCore::ADME*);
    void admeCommentsChanged(ezechiel::GuiCore::TranslatableString*);
    void halflifeChanged(ezechiel::GuiCore::Halflife*);
    void halflifeCommentsChanged(ezechiel::GuiCore::TranslatableString*);
    void errorModelChanged(ezechiel::GuiCore::ErrorModel*);
    void errorModelCommentsChanged(ezechiel::GuiCore::TranslatableString*);

    void parametersChanged(ezechiel::GuiCore::Parameters*);

    void covariatesChanged(ezechiel::GuiCore::DrugVariateList*);

public slots:
    void undo();
    void redo();
private:

    ezechiel::GuiCore::DrugModelList* _drugs;
    ezechiel::GuiCore::TranslatableString* _drugGeneralComments;


    ezechiel::GuiCore::EditorList *_editors;
    ezechiel::GuiCore::TranslatableString* _editorComments;

    ezechiel::GuiCore::TranslatableString* _dosesComments;
    ezechiel::GuiCore::DoseList* _doses;
    ezechiel::GuiCore::IntervalList* _intervals;
    ezechiel::GuiCore::InfusionList* _infusions;

    ezechiel::GuiCore::TranslatableString* _targetComments;
    ezechiel::GuiCore::TargetList* _targets;

    ezechiel::GuiCore::ADME* _adme;
    ezechiel::GuiCore::TranslatableString *_admeComments;
    ezechiel::GuiCore::Halflife* _halflife;
    ezechiel::GuiCore::TranslatableString *_halflifeComments;
    ezechiel::GuiCore::ErrorModel* _errorModel;
    ezechiel::GuiCore::TranslatableString *_errorModelComments;

    ezechiel::GuiCore::Parameters *_parameters;

    ezechiel::GuiCore::DrugVariateList* _covariates;


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

    ezechiel::GuiCore::DrugModel *_currentDrug;


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
