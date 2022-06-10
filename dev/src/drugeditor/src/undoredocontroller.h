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
namespace Gui {
namespace Core {


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
}



class UndoRedoController : public QQuickView
{

    Q_OBJECT
    Q_PROPERTY(Tucuxi::Gui::Core::DrugModelList* drugs READ getDrugs WRITE setDrugs NOTIFY drugsChanged)
    Q_PROPERTY(Tucuxi::Gui::Core::TranslatableString*  drugGeneralComments READ getDrugGeneralComments WRITE setDrugGeneralComments NOTIFY drugGeneralCommentsChanged)

    Q_PROPERTY(Tucuxi::Gui::Core::DrugModel* currentDrug READ getCurrentDrug WRITE setCurrentDrug NOTIFY currentDrugChanged)

    Q_PROPERTY(Tucuxi::Gui::Core::EditorList* editors READ getEditors WRITE setEditors NOTIFY editorsChanged)
    Q_PROPERTY(Tucuxi::Gui::Core::TranslatableString* editorComments READ getEditorComments WRITE setEditorComments NOTIFY editorCommentsChanged)

    Q_PROPERTY(Tucuxi::Gui::Core::DoseList* doses READ getDoses WRITE setDoses NOTIFY dosesChanged)
    Q_PROPERTY(Tucuxi::Gui::Core::TranslatableString* dosesComments READ getDosesComments WRITE setDosesComments NOTIFY dosesCommentsChanged)
    Q_PROPERTY(Tucuxi::Gui::Core::IntervalList* intervals READ getIntervals WRITE setIntervals NOTIFY intervalsChanged)
    Q_PROPERTY(Tucuxi::Gui::Core::InfusionList* infusions READ getInfusions WRITE setInfusions NOTIFY infusionsChanged)

    Q_PROPERTY(Tucuxi::Gui::Core::TargetList* targets READ getTargets WRITE setTargets NOTIFY targetsChanged)
    Q_PROPERTY(Tucuxi::Gui::Core::TranslatableString* targetComments READ getTargetComments WRITE setTargetComments NOTIFY targetCommentsChanged)

    Q_PROPERTY(Tucuxi::Gui::Core::ADME* adme READ getAdme WRITE setAdme NOTIFY admeChanged)
    Q_PROPERTY(Tucuxi::Gui::Core::TranslatableString* admeComments READ getAdmeComments WRITE setAdmeComments NOTIFY admeCommentsChanged)
    Q_PROPERTY(Tucuxi::Gui::Core::Halflife* halflife READ getHalflife WRITE setHalflife NOTIFY halflifeChanged)
    Q_PROPERTY(Tucuxi::Gui::Core::TranslatableString* halflifeComments READ getHalflifeComments WRITE setHalflifeComments NOTIFY halflifeCommentsChanged)
    Q_PROPERTY(Tucuxi::Gui::Core::ErrorModel* errorModel READ getErrorModel WRITE setErrorModel NOTIFY errorModelChanged)
    Q_PROPERTY(Tucuxi::Gui::Core::TranslatableString* errorModelComments READ getErrorModelComments WRITE setErrorModelComments NOTIFY errorModelCommentsChanged)


    Q_PROPERTY(Tucuxi::Gui::Core::Parameters* parameters READ getParameters WRITE setParameters NOTIFY parametersChanged)

    Q_PROPERTY(Tucuxi::Gui::Core::DrugVariateList* covariates READ getCovariates WRITE setCovariates NOTIFY covariatesChanged)


public:
    explicit UndoRedoController(QWindow* parent = 0);
    //drug
    Tucuxi::Gui::Core::DrugModelList* getDrugs() { return _drugs; }
    void setDrugs(Tucuxi::Gui::Core::DrugModelList* dlist) { _drugs = dlist; /*setModified(true);*/ emit drugsChanged(_drugs); }
    //drug general comments
    Tucuxi::Gui::Core::TranslatableString* getDrugGeneralComments() { return _drugGeneralComments; }
    void setDrugGeneralComments(Tucuxi::Gui::Core::TranslatableString* cList) { _drugGeneralComments = cList; emit drugGeneralCommentsChanged(_drugGeneralComments); }

    //current drug
    Tucuxi::Gui::Core::DrugModel* getCurrentDrug(){return  _currentDrug;}
    void setCurrentDrug(Tucuxi::Gui::Core::DrugModel* newDrug) { _currentDrug = newDrug; }

    //editors
    Tucuxi::Gui::Core::EditorList* getEditors() { return _editors; }
    void setEditors(Tucuxi::Gui::Core::EditorList* editors) { _editors = editors; }
    //editors comments
    Tucuxi::Gui::Core::TranslatableString* getEditorComments() { return _editorComments; }
    void setEditorComments(Tucuxi::Gui::Core::TranslatableString* comm) { _editorComments = comm; }

    //doses comments
    Tucuxi::Gui::Core::TranslatableString* getDosesComments() { return _dosesComments; }
    void setDosesComments(Tucuxi::Gui::Core::TranslatableString* transString){ _dosesComments = transString; emit dosesCommentsChanged(_dosesComments);}
    //doses
    Tucuxi::Gui::Core::DoseList* getDoses() { return _doses; }
    void setDoses(Tucuxi::Gui::Core::DoseList* doses) { _doses = doses;    }
    //intervals
    Tucuxi::Gui::Core::IntervalList* getIntervals() { return _intervals; }
    void setIntervals(Tucuxi::Gui::Core::IntervalList* intervals) { _intervals = intervals; }
    //infusions
    Tucuxi::Gui::Core::InfusionList* getInfusions() { return _infusions; }
    void setInfusions(Tucuxi::Gui::Core::InfusionList* infusions) { _infusions = infusions; }

    //targets
    Tucuxi::Gui::Core::TargetList* getTargets() { return _targets; }
    void setTargets(Tucuxi::Gui::Core::TargetList* targets) { _targets = targets; }
    //targetComments
    Tucuxi::Gui::Core::TranslatableString* getTargetComments() { return _targetComments; }
    void setTargetComments(Tucuxi::Gui::Core::TranslatableString* comment) { _targetComments = comment; }

    //ADME
    Tucuxi::Gui::Core::ADME* getAdme() { return _adme; }
    void setAdme(Tucuxi::Gui::Core::ADME *adme) { _adme = adme; }
    //ADME comments
    Tucuxi::Gui::Core::TranslatableString* getAdmeComments() { return _admeComments; }
    void setAdmeComments(Tucuxi::Gui::Core::TranslatableString* comment) { _admeComments = comment; }
    //Halflife
    Tucuxi::Gui::Core::Halflife*getHalflife() { return _halflife; }
    void setHalflife(Tucuxi::Gui::Core::Halflife* hl) { _halflife = hl; }
    //halflife comments
    Tucuxi::Gui::Core::TranslatableString* getHalflifeComments() { return _halflifeComments; }
    void setHalflifeComments(Tucuxi::Gui::Core::TranslatableString* comment) { _halflifeComments = comment; }
    //error_model
    Tucuxi::Gui::Core::ErrorModel* getErrorModel() { return _errorModel; }
    void setErrorModel(Tucuxi::Gui::Core::ErrorModel* errorModel) { _errorModel = errorModel; }
    //error_model comments
    Tucuxi::Gui::Core::TranslatableString* getErrorModelComments() { return _errorModelComments; }
    void setErrorModelComments(Tucuxi::Gui::Core::TranslatableString* comment) { _errorModelComments = comment; }

    //parameters
    Tucuxi::Gui::Core::Parameters* getParameters() { return _parameters; }
    void setParameters(Tucuxi::Gui::Core::Parameters *parameters) { _parameters = parameters; }

    //covariates
    Tucuxi::Gui::Core::DrugVariateList* getCovariates() { return _covariates; }
    void setCovariates(Tucuxi::Gui::Core::DrugVariateList* coavariates) { _covariates = coavariates; }


signals:
    void drugsChanged(Tucuxi::Gui::Core::DrugModelList*);
    void drugGeneralCommentsChanged(Tucuxi::Gui::Core::TranslatableString*);

    void currentDrugChanged(Tucuxi::Gui::Core::DrugModel*);

    void editorsChanged(Tucuxi::Gui::Core::EditorList*);
    void editorCommentsChanged(Tucuxi::Gui::Core::TranslatableString*);

    void dosesCommentsChanged(Tucuxi::Gui::Core::TranslatableString*);
    void dosesChanged(Tucuxi::Gui::Core::DoseList*);
    void intervalsChanged(Tucuxi::Gui::Core::IntervalList*);
    void infusionsChanged(Tucuxi::Gui::Core::InfusionList*);

    void targetsChanged(Tucuxi::Gui::Core::TargetList*);
    void targetCommentsChanged(Tucuxi::Gui::Core::TranslatableString*);

    void admeChanged(Tucuxi::Gui::Core::ADME*);
    void admeCommentsChanged(Tucuxi::Gui::Core::TranslatableString*);
    void halflifeChanged(Tucuxi::Gui::Core::Halflife*);
    void halflifeCommentsChanged(Tucuxi::Gui::Core::TranslatableString*);
    void errorModelChanged(Tucuxi::Gui::Core::ErrorModel*);
    void errorModelCommentsChanged(Tucuxi::Gui::Core::TranslatableString*);

    void parametersChanged(Tucuxi::Gui::Core::Parameters*);

    void covariatesChanged(Tucuxi::Gui::Core::DrugVariateList*);

public slots:
    void undo();
    void redo();
private:

    Tucuxi::Gui::Core::DrugModelList* _drugs;
    Tucuxi::Gui::Core::TranslatableString* _drugGeneralComments;


    Tucuxi::Gui::Core::EditorList *_editors;
    Tucuxi::Gui::Core::TranslatableString* _editorComments;

    Tucuxi::Gui::Core::TranslatableString* _dosesComments;
    Tucuxi::Gui::Core::DoseList* _doses;
    Tucuxi::Gui::Core::IntervalList* _intervals;
    Tucuxi::Gui::Core::InfusionList* _infusions;

    Tucuxi::Gui::Core::TranslatableString* _targetComments;
    Tucuxi::Gui::Core::TargetList* _targets;

    Tucuxi::Gui::Core::ADME* _adme;
    Tucuxi::Gui::Core::TranslatableString *_admeComments;
    Tucuxi::Gui::Core::Halflife* _halflife;
    Tucuxi::Gui::Core::TranslatableString *_halflifeComments;
    Tucuxi::Gui::Core::ErrorModel* _errorModel;
    Tucuxi::Gui::Core::TranslatableString *_errorModelComments;

    Tucuxi::Gui::Core::Parameters *_parameters;

    Tucuxi::Gui::Core::DrugVariateList* _covariates;


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

    Tucuxi::Gui::Core::DrugModel *_currentDrug;


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
