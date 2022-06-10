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

namespace Tucuxi {
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
    Q_PROPERTY(Tucuxi::GuiCore::DrugModelList* drugs READ getDrugs WRITE setDrugs NOTIFY drugsChanged)
    Q_PROPERTY(Tucuxi::GuiCore::TranslatableString*  drugGeneralComments READ getDrugGeneralComments WRITE setDrugGeneralComments NOTIFY drugGeneralCommentsChanged)

    Q_PROPERTY(Tucuxi::GuiCore::DrugModel* currentDrug READ getCurrentDrug WRITE setCurrentDrug NOTIFY currentDrugChanged)

    Q_PROPERTY(Tucuxi::GuiCore::EditorList* editors READ getEditors WRITE setEditors NOTIFY editorsChanged)
    Q_PROPERTY(Tucuxi::GuiCore::TranslatableString* editorComments READ getEditorComments WRITE setEditorComments NOTIFY editorCommentsChanged)

    Q_PROPERTY(Tucuxi::GuiCore::DoseList* doses READ getDoses WRITE setDoses NOTIFY dosesChanged)
    Q_PROPERTY(Tucuxi::GuiCore::TranslatableString* dosesComments READ getDosesComments WRITE setDosesComments NOTIFY dosesCommentsChanged)
    Q_PROPERTY(Tucuxi::GuiCore::IntervalList* intervals READ getIntervals WRITE setIntervals NOTIFY intervalsChanged)
    Q_PROPERTY(Tucuxi::GuiCore::InfusionList* infusions READ getInfusions WRITE setInfusions NOTIFY infusionsChanged)

    Q_PROPERTY(Tucuxi::GuiCore::TargetList* targets READ getTargets WRITE setTargets NOTIFY targetsChanged)
    Q_PROPERTY(Tucuxi::GuiCore::TranslatableString* targetComments READ getTargetComments WRITE setTargetComments NOTIFY targetCommentsChanged)

    Q_PROPERTY(Tucuxi::GuiCore::ADME* adme READ getAdme WRITE setAdme NOTIFY admeChanged)
    Q_PROPERTY(Tucuxi::GuiCore::TranslatableString* admeComments READ getAdmeComments WRITE setAdmeComments NOTIFY admeCommentsChanged)
    Q_PROPERTY(Tucuxi::GuiCore::Halflife* halflife READ getHalflife WRITE setHalflife NOTIFY halflifeChanged)
    Q_PROPERTY(Tucuxi::GuiCore::TranslatableString* halflifeComments READ getHalflifeComments WRITE setHalflifeComments NOTIFY halflifeCommentsChanged)
    Q_PROPERTY(Tucuxi::GuiCore::ErrorModel* errorModel READ getErrorModel WRITE setErrorModel NOTIFY errorModelChanged)
    Q_PROPERTY(Tucuxi::GuiCore::TranslatableString* errorModelComments READ getErrorModelComments WRITE setErrorModelComments NOTIFY errorModelCommentsChanged)


    Q_PROPERTY(Tucuxi::GuiCore::Parameters* parameters READ getParameters WRITE setParameters NOTIFY parametersChanged)

    Q_PROPERTY(Tucuxi::GuiCore::DrugVariateList* covariates READ getCovariates WRITE setCovariates NOTIFY covariatesChanged)


public:
    explicit UndoRedoController(QWindow* parent = 0);
    //drug
    Tucuxi::GuiCore::DrugModelList* getDrugs() { return _drugs; }
    void setDrugs(Tucuxi::GuiCore::DrugModelList* dlist) { _drugs = dlist; /*setModified(true);*/ emit drugsChanged(_drugs); }
    //drug general comments
    Tucuxi::GuiCore::TranslatableString* getDrugGeneralComments() { return _drugGeneralComments; }
    void setDrugGeneralComments(Tucuxi::GuiCore::TranslatableString* cList) { _drugGeneralComments = cList; emit drugGeneralCommentsChanged(_drugGeneralComments); }

    //current drug
    Tucuxi::GuiCore::DrugModel* getCurrentDrug(){return  _currentDrug;}
    void setCurrentDrug(Tucuxi::GuiCore::DrugModel* newDrug) { _currentDrug = newDrug; }

    //editors
    Tucuxi::GuiCore::EditorList* getEditors() { return _editors; }
    void setEditors(Tucuxi::GuiCore::EditorList* editors) { _editors = editors; }
    //editors comments
    Tucuxi::GuiCore::TranslatableString* getEditorComments() { return _editorComments; }
    void setEditorComments(Tucuxi::GuiCore::TranslatableString* comm) { _editorComments = comm; }

    //doses comments
    Tucuxi::GuiCore::TranslatableString* getDosesComments() { return _dosesComments; }
    void setDosesComments(Tucuxi::GuiCore::TranslatableString* transString){ _dosesComments = transString; emit dosesCommentsChanged(_dosesComments);}
    //doses
    Tucuxi::GuiCore::DoseList* getDoses() { return _doses; }
    void setDoses(Tucuxi::GuiCore::DoseList* doses) { _doses = doses;    }
    //intervals
    Tucuxi::GuiCore::IntervalList* getIntervals() { return _intervals; }
    void setIntervals(Tucuxi::GuiCore::IntervalList* intervals) { _intervals = intervals; }
    //infusions
    Tucuxi::GuiCore::InfusionList* getInfusions() { return _infusions; }
    void setInfusions(Tucuxi::GuiCore::InfusionList* infusions) { _infusions = infusions; }

    //targets
    Tucuxi::GuiCore::TargetList* getTargets() { return _targets; }
    void setTargets(Tucuxi::GuiCore::TargetList* targets) { _targets = targets; }
    //targetComments
    Tucuxi::GuiCore::TranslatableString* getTargetComments() { return _targetComments; }
    void setTargetComments(Tucuxi::GuiCore::TranslatableString* comment) { _targetComments = comment; }

    //ADME
    Tucuxi::GuiCore::ADME* getAdme() { return _adme; }
    void setAdme(Tucuxi::GuiCore::ADME *adme) { _adme = adme; }
    //ADME comments
    Tucuxi::GuiCore::TranslatableString* getAdmeComments() { return _admeComments; }
    void setAdmeComments(Tucuxi::GuiCore::TranslatableString* comment) { _admeComments = comment; }
    //Halflife
    Tucuxi::GuiCore::Halflife*getHalflife() { return _halflife; }
    void setHalflife(Tucuxi::GuiCore::Halflife* hl) { _halflife = hl; }
    //halflife comments
    Tucuxi::GuiCore::TranslatableString* getHalflifeComments() { return _halflifeComments; }
    void setHalflifeComments(Tucuxi::GuiCore::TranslatableString* comment) { _halflifeComments = comment; }
    //error_model
    Tucuxi::GuiCore::ErrorModel* getErrorModel() { return _errorModel; }
    void setErrorModel(Tucuxi::GuiCore::ErrorModel* errorModel) { _errorModel = errorModel; }
    //error_model comments
    Tucuxi::GuiCore::TranslatableString* getErrorModelComments() { return _errorModelComments; }
    void setErrorModelComments(Tucuxi::GuiCore::TranslatableString* comment) { _errorModelComments = comment; }

    //parameters
    Tucuxi::GuiCore::Parameters* getParameters() { return _parameters; }
    void setParameters(Tucuxi::GuiCore::Parameters *parameters) { _parameters = parameters; }

    //covariates
    Tucuxi::GuiCore::DrugVariateList* getCovariates() { return _covariates; }
    void setCovariates(Tucuxi::GuiCore::DrugVariateList* coavariates) { _covariates = coavariates; }


signals:
    void drugsChanged(Tucuxi::GuiCore::DrugModelList*);
    void drugGeneralCommentsChanged(Tucuxi::GuiCore::TranslatableString*);

    void currentDrugChanged(Tucuxi::GuiCore::DrugModel*);

    void editorsChanged(Tucuxi::GuiCore::EditorList*);
    void editorCommentsChanged(Tucuxi::GuiCore::TranslatableString*);

    void dosesCommentsChanged(Tucuxi::GuiCore::TranslatableString*);
    void dosesChanged(Tucuxi::GuiCore::DoseList*);
    void intervalsChanged(Tucuxi::GuiCore::IntervalList*);
    void infusionsChanged(Tucuxi::GuiCore::InfusionList*);

    void targetsChanged(Tucuxi::GuiCore::TargetList*);
    void targetCommentsChanged(Tucuxi::GuiCore::TranslatableString*);

    void admeChanged(Tucuxi::GuiCore::ADME*);
    void admeCommentsChanged(Tucuxi::GuiCore::TranslatableString*);
    void halflifeChanged(Tucuxi::GuiCore::Halflife*);
    void halflifeCommentsChanged(Tucuxi::GuiCore::TranslatableString*);
    void errorModelChanged(Tucuxi::GuiCore::ErrorModel*);
    void errorModelCommentsChanged(Tucuxi::GuiCore::TranslatableString*);

    void parametersChanged(Tucuxi::GuiCore::Parameters*);

    void covariatesChanged(Tucuxi::GuiCore::DrugVariateList*);

public slots:
    void undo();
    void redo();
private:

    Tucuxi::GuiCore::DrugModelList* _drugs;
    Tucuxi::GuiCore::TranslatableString* _drugGeneralComments;


    Tucuxi::GuiCore::EditorList *_editors;
    Tucuxi::GuiCore::TranslatableString* _editorComments;

    Tucuxi::GuiCore::TranslatableString* _dosesComments;
    Tucuxi::GuiCore::DoseList* _doses;
    Tucuxi::GuiCore::IntervalList* _intervals;
    Tucuxi::GuiCore::InfusionList* _infusions;

    Tucuxi::GuiCore::TranslatableString* _targetComments;
    Tucuxi::GuiCore::TargetList* _targets;

    Tucuxi::GuiCore::ADME* _adme;
    Tucuxi::GuiCore::TranslatableString *_admeComments;
    Tucuxi::GuiCore::Halflife* _halflife;
    Tucuxi::GuiCore::TranslatableString *_halflifeComments;
    Tucuxi::GuiCore::ErrorModel* _errorModel;
    Tucuxi::GuiCore::TranslatableString *_errorModelComments;

    Tucuxi::GuiCore::Parameters *_parameters;

    Tucuxi::GuiCore::DrugVariateList* _covariates;


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

    Tucuxi::GuiCore::DrugModel *_currentDrug;


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
