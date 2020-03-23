#include "undoredocontroller.h"

#include "apputils/src/appcore.h"
#include "apputils/src/apputilsrepository.h"

#include "models/sortfilterentitylistproxymodel.h"
#include "models/commentlistmodel.h"

#include "models/editormodel.h"
#include "models/editorlistmodel.h"
#include "models/druglistmodel.h"
#include "models/domainlistmodel.h"
#include "models/studylistmodel.h"
#include "models/doselistmodel.h"

#include "core/dal/drug/editors.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/errormodel.h"


#include "apputils/src/drugxmlexport.h"
#include "apputils/src/drugxmlimport.h"
#include "undocommands.h"

#include "errors_drugeditor.h"

#include "core/utils/connect.h"

#include "QMap"

#include "QQmlContext"
#include "QQuickItem"

UndoRedoController::UndoRedoController(QWindow *parent) :
    QQuickView(parent),
    _currentDrug(ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugModel>(ABSTRACTREPO, this)),
    _editors(ezechiel::core::CoreFactory::createEntity<ezechiel::core::EditorList>(ABSTRACTREPO,this)),
    _editorComments(ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(ABSTRACTREPO,this)),
    editorModel(new EditorModel(this)),
    editorListModel(new EditorListModel(this)),
    editorCommentsModel(new CommentListModel(this)),
    _doses(ezechiel::core::CoreFactory::createEntity<ezechiel::core::DoseList>(ABSTRACTREPO,this)),
    //    _doseList(ezechiel::core::CoreFactory::createEntity<ezechiel::core::DoseList>(ABSTRACTREPO,this)),
    _dosesComments(ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(ABSTRACTREPO,this)),
    _intervals(ezechiel::core::CoreFactory::createEntity<ezechiel::core::IntervalList>(ABSTRACTREPO,this)),
    _infusions(ezechiel::core::CoreFactory::createEntity<ezechiel::core::InfusionList>(ABSTRACTREPO,this)),
    drugListModel(new DrugListModel(this)),
    drugCommentsModel(new CommentListModel(this)),
    _drugGeneralComments(ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(ABSTRACTREPO, this)),
    _drugs(ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugModelList>(ABSTRACTREPO, this)),
    _targetComments(ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(ABSTRACTREPO, this)),
    _targets(ezechiel::core::CoreFactory::createEntity<ezechiel::core::TargetList>(ABSTRACTREPO, this)),
    _admeComments(ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(ABSTRACTREPO, this)),
    _adme(ezechiel::core::CoreFactory::createEntity<ezechiel::core::ADME>(ABSTRACTREPO,this)),
    _halflifeComments(ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(ABSTRACTREPO, this)),
    _halflife(ezechiel::core::CoreFactory::createEntity<ezechiel::core::Halflife>(ABSTRACTREPO,this)),
    _errorModelComments(ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(ABSTRACTREPO, this)),
    _errorModel(ezechiel::core::CoreFactory::createEntity<ezechiel::core::ErrorModel>(ABSTRACTREPO,this)),
    _parameters(ezechiel::core::CoreFactory::createEntity<ezechiel::core::Parameters>(ABSTRACTREPO,this)),
    _covariates(ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugVariateList>(ABSTRACTREPO,this)),
    domainListModel(new DomainListModel(this)),
    studyListModel(new StudyListModel(this)),
    doseListModel(new DoseListModel(this)),
    doseCommentsModel(new CommentListModel(this)),
    drugListProxyModel(new SortFilterEntityListProxyModel(this)),
    editorListProxyModel(new SortFilterEntityListProxyModel(this)),
    doseListProxyModel(new SortFilterEntityListProxyModel(this)),
    undoRedoStack(new QUndoStack(this))
{

    resetStaticModels();
    resetDynamicModels();

    initProxyModels();
    initRootContext();
    initQmlViews();
    initConnexions();


    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(drugsView, "reset"));
}



void UndoRedoController::resetStaticModels(){
    if (APPUTILSREPO->getDrugsList(_drugs->getList()).error != ezechiel::core::NoError)
        EXLOG(QtFatalMsg, ezechiel::drugeditor::MODELIOERROR, "Drugs list not retreived.");

    for(int i = 0; i < _drugs->size();++i) {
        _drugs->at(i)->setParent(_drugs);
    }

    _drugs->update();
}

void UndoRedoController::resetDynamicModels()
{
    //nothing to be done in here yet
}

void UndoRedoController::initProxyModels()
{

    drugListProxyModel->setDynamicSortFilter(false);
    drugListProxyModel->setSourceModel(drugListModel);

    editorListProxyModel->setDynamicSortFilter(false);
    editorListProxyModel->setSourceModel(editorListModel);

    doseListProxyModel->setDynamicSortFilter(false);
    doseListProxyModel->setSourceModel(doseListModel);


}

void UndoRedoController::initRootContext()
{

    rootContext()->setContextProperty("editorListModel",            editorListProxyModel);
    rootContext()->setContextProperty("editorModel",                editorModel);
    rootContext()->setContextProperty("editorCommentsModel",        editorCommentsModel);
    //    rootContext()->setContextProperty("drugListModel",              drugListProxyModel);
    rootContext()->setContextProperty("drugCommentsModel",          drugCommentsModel);
    rootContext()->setContextProperty("domainListModel",            domainListModel);
    rootContext()->setContextProperty("studyListModel",             studyListModel);
    rootContext()->setContextProperty("doseListModel",              doseListProxyModel);
    rootContext()->setContextProperty("doseCommentsModel",          doseCommentsModel);

    rootContext()->setContextProperty("undoRedoController",             this);


    setSource(QUrl("qrc:/MainWindow.qml"));
}

void UndoRedoController::initQmlViews()
{
    mainView = rootObject();
    Q_ASSERT(mainView);

    flowView = rootObject()->findChild<QObject *>("flowView");
    Q_ASSERT(flowView);

    drugsView = rootObject()->findChild<QObject *>("drugsView");
    Q_ASSERT(drugsView);

    drugCommentsView = rootObject()->findChild<QObject *>("drugCommentsView");
    Q_ASSERT(drugCommentsView);

    editorView = rootObject()->findChild<QObject *>("editorView");
    Q_ASSERT(editorView);

    editorCommentsView = rootObject()->findChild<QObject *>("editorCommentsView");
    Q_ASSERT(editorCommentsView);

    dosageView = rootObject()->findChild<QObject *>("doseView");
    Q_ASSERT(dosageView);

    dosageCommentsView = rootObject()->findChild<QObject *>("doseCommentsView");
    Q_ASSERT(dosageCommentsView);

    targetView = rootObject()->findChild<QObject *>("targetsView");
    Q_ASSERT(targetView);

    conversionView = rootObject()->findChild<QObject *>("conversionView");
    Q_ASSERT(conversionView);

    parameterView = rootObject()->findChild<QObject *>("parameterView");
    Q_ASSERT(parameterView);

    covariateView = rootObject()->findChild<QObject *>("covariateView");
    Q_ASSERT(covariateView);
}

void UndoRedoController::initConnexions()
{

    ///QStrings below are formated in the following fashion: "name-stuff-moreStuff-etc"
    connect(drugsView,      SIGNAL(mainIndexChanged(int)),  this,   SLOT(drugIndexChanged(int)));
    connect(editorView,     SIGNAL(mainIndexChanged(int)),  this,   SLOT(editorIndexChanged(int)));
    connect(dosageView,       SIGNAL(mainIndexChanged(int)),  this,   SLOT(dosageIndexChanged(int)));
    connect(targetView,     SIGNAL(mainIndexChanged(int)), this,    SLOT(targetIndexChanged(int)));

    connect(flowView,       SIGNAL(currentIndexChanged(QString, int)),  this,   SLOT(currentIndexChanged(QString,int)));

    connect(drugsView,      SIGNAL(currentIndexChanged(QString, int)), this, SLOT(currentIndexChanged(QString, int)));
    connect(editorView,     SIGNAL(currentIndexChanged(QString, int)), this, SLOT(currentIndexChanged(QString, int)));
//    connect(editorView,     SIGNAL(currentIndexChanged(QString, int)), this, SLOT(editorIndexChanged(QString, int)));
    connect(dosageView,     SIGNAL(currentIndexChanged(QString, int)), this, SLOT(currentIndexChanged(QString, int)));
    connect(targetView,     SIGNAL(currentIndexChanged(QString, int)), this,    SLOT(currentIndexChanged(QString,int)));
    connect(conversionView, SIGNAL(currentIndexChanged(QString, int)), this,    SLOT(currentIndexChanged(QString,int)));
    connect(parameterView,  SIGNAL(currentIndexChanged(QString, int)),  this,   SLOT(currentIndexChanged(QString,int)));
    connect(covariateView,  SIGNAL(currentIndexChanged(QString, int)),  this,   SLOT(currentIndexChanged(QString,int)));

    connect(drugsView,      SIGNAL(entityChanged(QString,int)), this,   SLOT(drugEntityChanged(QString,int)));
    connect(editorView,     SIGNAL(entityChanged(QString, int)), this,  SLOT(editorEntityChanged(QString, int)));
    connect(dosageView,       SIGNAL(entityChanged(QString, int)), this,  SLOT(dosageEntityChanged(QString, int)));
    connect(targetView,     SIGNAL(entityChanged(QString,int)), this,   SLOT(targetEntityChanged(QString,int)));
    connect(conversionView, SIGNAL(entityChanged(QString,int)),this,    SLOT(conversionEntityChanged(QString,int)));
    connect(parameterView,  SIGNAL(entityChanged(QString,int)),this,    SLOT(parameterEntityChanged(QString,int)));
    connect(covariateView,  SIGNAL(entityChanged(QString,int)),this,    SLOT(covariateEntityChanged(QString,int)));

    connect(drugsView,      SIGNAL(textChanged(QString,QVariant)), this,SLOT(drugDataChanged(QString,QVariant)));
    connect(editorView,     SIGNAL(textChanged(QString,QVariant)), this,SLOT(editorDataChanged(QString, QVariant)));
    connect(dosageView,       SIGNAL(textChanged(QString,QVariant)), this,SLOT(dosageDataChanged(QString, QVariant)));
    connect(targetView,     SIGNAL(textChanged(QString,QVariant)),  this,SLOT(targetDataChanged(QString,QVariant)));
    connect(conversionView, SIGNAL(textChanged(QString,QVariant)),  this,SLOT(conversionDataChanged(QString,QVariant)));

    connect(covariateView,  SIGNAL(textChanged(QString,QVariant)),  this,SLOT(covariateDataChanged(QString,QVariant)));
}

void UndoRedoController::refreshCurrentView()
{
    int currentViewIndex = flowView->property("currentIndex").toInt();

    QObject *editorButton = mainView->findChild<QObject *>("editorButton");
    Q_ASSERT(editorButton);

    QObject *drugButton = mainView->findChild<QObject *>("drugButton");
    Q_ASSERT(drugButton);
    if (currentViewIndex == drugButton->property("index").toInt()) {

    } else if (currentViewIndex == editorButton->property("index").toInt()) {
        editorDataChanged();
    }
}

void UndoRedoController::undoPushator(QUndoCommand *cmd)
{
    undoRedoStack->push(cmd);
}


void UndoRedoController::undo()
{
    if(!undoRedoStack->canUndo())
        return;

    undoRedoStack->undo();
    refreshCurrentView();
}

void UndoRedoController::redo()
{

    if(!undoRedoStack->canRedo())
        return;

    undoRedoStack->redo();
    refreshCurrentView();
}

void UndoRedoController::currentIndexChanged(QString view, int index)
{

    QObject *neoView = mainView->findChild<QObject *>(view.split('-').at(0));
    Q_ASSERT(neoView);

    undoRedoStack->push(new ChangeViewIndexCommand(neoView,index));

}


void UndoRedoController::drugIndexChanged(int index)
{
    if(index != -1){


        _currentDrug = _drugs->at(index);
        Q_ASSERT(_currentDrug);

        _drugGeneralComments = (_currentDrug->getGeneralCommentsTranslationMap());

        //        Q_ASSERT(_currentDrug->getMetaData()->isValid());
        _editors->getList().clear();
        if (_currentDrug->getMetaData()->getEditors()->size() != 0)
            _editors->append( _currentDrug->getMetaData()->getEditors()->at(0));
        editorListModel->setModelData(&_editors->getList());

        _doses->getList() = _currentDrug->getDoses()->mid(0);
        _dosesComments = _currentDrug->getDoses()->getComments();

        _intervals->getList() = _currentDrug->getIntervals()->mid(0);
        _infusions->getList() = _currentDrug->getInfusions()->mid(0);

        _targets = _currentDrug->getTargets();

        _adme = _currentDrug->getAdme();
        _admeComments = _adme->getComments();
        _halflife = _currentDrug->getHalflife();
        _halflifeComments = _halflife->getComments();
        _errorModel = _currentDrug->getErrorModel();
        _errorModelComments = _errorModel->getComments();
        _parameters = _currentDrug->getParameters()->getParameters();
        _covariates = _currentDrug->getCovariates();



        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(covariateView,"reset"));


        emit drugGeneralCommentsChanged(_drugGeneralComments);
        emit dosesChanged(_doses);
        emit dosesCommentsChanged(_dosesComments);

        emit targetsChanged(_targets);

        emit admeChanged(_adme);
        emit admeCommentsChanged(_admeComments);
        emit halflifeChanged(_halflife);
        emit halflifeCommentsChanged(_halflifeComments);
        emit errorModelChanged(_errorModel);
        emit errorModelCommentsChanged(_errorModelComments);
        emit parametersChanged(_parameters);
        emit covariatesChanged(_covariates);

    }
}

void UndoRedoController::editorIndexChanged(int index)
{
    ezechiel::core::Editor *editor;

    if(index < 0 || index >= _currentDrug->getMetaData()->getEditors()->size()) {
        return;
    } else {
        editor =  _currentDrug->getMetaData()->getEditors()->at(index);
        editor->setParent(this);
        editorModel->setModelData(editor);
        _editorComments = editor->getComments();
        emit editorCommentsChanged(_editorComments);
    }
}

void UndoRedoController::dosageIndexChanged(int index)
{
    //TODO
}

void UndoRedoController::targetIndexChanged(int index)
{
    if(index == -1) return;
    ezechiel::core::Target *target = _targets->at(index);

    _targetComments = target->getComments();

    emit targetCommentsChanged(_targetComments);
}



void UndoRedoController::currentDrugChanged(QString view, int index)
{
    undoRedoStack->push(new ChangeViewIndexCommand(drugsView,index));
}

void UndoRedoController::currentEditorChanged(QString view, int index)
{
    undoRedoStack->push(new ChangeViewIndexCommand(editorView,index));
}

void UndoRedoController::currentDosageChanged(QString view, int index)
{
    undoRedoStack->push(new ChangeViewIndexCommand(dosageView,index));
}




/// #### Data changes ####

void UndoRedoController::drugDataChanged(QString propertyName, QVariant newText)
{
    if (propertyName.isEmpty())
        return;

    if ( propertyName.contains("comment-") ) {
        ezechiel::core::TranslatableString *comments = _drugGeneralComments;//_currentDrug->getGeneralCommentsTranslationMap();

        QString reference = propertyName.split('-').at(2);
        bool isLangChanging = (propertyName.split('-').at(1) == "lang");

        if( (isLangChanging && (newText == comments->key(reference))) ||
                (!isLangChanging && (newText == comments->value(reference))))
            return;

        undoPushator(new EditCommentCommand(comments, newText.toString(), reference, &UndoRedoController::drugGeneralCommentsChanged,this,isLangChanging));

    } else {

    }

    drugDataChanged();

}

void UndoRedoController::editorDataChanged(QString propertyName, QVariant newValue)
{
    if(propertyName.isEmpty())
        return;

    ezechiel::core::Editor *editor = editorModel->getModelData();
    Q_ASSERT(editor);


    if(propertyName.contains("comment-")){

        ezechiel::core::TranslatableString *comments = editor->getComments();
        QString reference = propertyName.split('-').at(2);
        bool  isLangChanging = (propertyName.split('-').at(1) == "lang");

        if( (isLangChanging && (newValue == comments->key(reference))) ||
                (!isLangChanging && (newValue == comments->value(reference))))
            return;

        undoPushator(new EditCommentCommand(comments,newValue.toString(),reference,&UndoRedoController::editorCommentsChanged,this,isLangChanging));

    } else {
        if(editor->property(qPrintable(propertyName)) == newValue)
            return;

        undoPushator(new EditTextCommand(editor,propertyName,newValue));
    }

    editorDataChanged();
}

void UndoRedoController::dosageDataChanged(QString propertyName, QVariant newText) //TODO
{
    if(propertyName.isEmpty()) return;

    if(propertyName.contains("comment-")) {
        QString reference = propertyName.split('-').at(2);
        bool  isLangChanging = (propertyName.split('-').at(1) == "lang");

        if( (isLangChanging && (newText == _dosesComments->key(reference))) ||
                (!isLangChanging && (newText== _dosesComments->value(reference))))
            return;

        undoPushator(new EditCommentCommand(_dosesComments,newText.toString(),reference,&UndoRedoController::dosesCommentsChanged,this,isLangChanging));


    } else if (propertyName.contains("Doses-")) {

        int index = propertyName.split('-').at(1).toInt();

        ezechiel::core::ValidDose * dose = _doses->at(index);

        ezechiel::core::IdentifiableAmount* amount = dose->getQuantity();
        undoPushator(new EditObjCommand(amount,"dbvalue",newText));//TODO send signal through command

    } else if (propertyName.contains("Intervals-")) {
        ezechiel::core::IdentifiableAmount* amount = _intervals->at(propertyName.split('-').at(1).toInt())->getQuantity();
        undoPushator(new EditObjCommand(amount,"dbvalue",newText));
    } else if (propertyName.contains("Infusions-")) {

        undoPushator(new EditObjCommand(_infusions->at(propertyName.split('-').at(1).toInt())->getQuantity(),"dbvalue",newText));

    }
}

void UndoRedoController::targetDataChanged(QString propertyName, QVariant newText)
{
    QStringList propList = propertyName.split('-');
    QString role = propList.at(0);
    int index = propList.at(1).toInt();
    QString type = propList.at(2);
    if(role == "comment") {

        undoPushator(new EditCommentCommand(_targetComments,newText.toString(),type,&UndoRedoController::targetCommentsChanged,this,propList.at(1) == "lang"));
    } else {
        ezechiel::core::Target *t = _targets->at(index);
        //    if(!_currentDrug->getTargets()->getList().contains(t)) {
        //        t->setType(type);
        //        _currentDrug->getTargets()->append(t);
        //    }
        ezechiel::core::OperableAmount *amount;
        if(role == "cmin") {
            amount = t->getCmin();
        } else if(role == "cmax") {
            amount = t->getCmax();
        } else if(role == "cbest") {
            amount = t->getCbest();
        } else if(role == "tmin") {
            amount = t->getTmin();
        } else if(role == "tmax") {
            amount = t->getTmax();
        } else if(role == "tbest") {
            amount = t->getTbest();
        } else {
            return;
        }
        undoPushator(new EditObjCommand(amount,"dbvalue",newText/*,&UndoRedoController::targetsChanged,this*/));
    }
}

void UndoRedoController::conversionDataChanged(QString propertyName, QVariant newText)
{
    QStringList propList = propertyName.split('-');
    QString type = propList.at(0);
    QString role = propList.at(1);
    if(type == "comment") {
        ezechiel::core::TranslatableString *comment;
        void (UndoRedoController::*signal)(ezechiel::core::TranslatableString*);
        type = conversionView->property("conversionSelected").toString();

        if(type == "ADME") {
            comment = _admeComments;
            signal = &UndoRedoController::admeCommentsChanged;
        } else if(type == "Halflife") {
            comment = _halflifeComments;
            signal = &UndoRedoController::halflifeCommentsChanged;
        } else if (type == "Error_Model") {
            comment = _errorModelComments;
            signal = &UndoRedoController::errorModelCommentsChanged;
        } else {
            return;
        }
        undoPushator(new EditCommentCommand(comment,newText.toString(),propList.at(2),signal,this,role == "lang"));
    } else {
        QObject *obj;
        if(type == "ADME") {
            obj = _adme;
        } else if(type == "Halflife"){
            obj = _halflife;
        } else if(type == "Error_Model") {
            obj = _errorModel;
        } else {
            return;
        }

        undoPushator(new EditObjCommand(obj,role,newText));

    }
}



void UndoRedoController::covariateDataChanged(QString propertyName, QVariant newText)
{

    QStringList propList = propertyName.split('-');
    QString name = propList.at(0);

    ezechiel::core::DrugVariate *covariate;


    //TODO check if data is the same and do nothing

    if(name == "trans") {

        QString type = propList.at(1);
        QString lang = propList.at(2);
        QString role = propList.at(3);

        int index = covariateView->property("index").toInt();

        covariate = _covariates->at(index);

        ezechiel::core::TranslatableString *trans;
        void (UndoRedoController::*signal)();



        signal = &UndoRedoController::covariatesSignal;//no proper signals for those translatableStrings. EditCommentCommand doesn't accept generic signals yet

        if( type == "name" ) {
            trans = covariate->getVisualNameTranslation();
        } else if ( type == "desc" ) {
            trans = covariate->getDescriptionTranslation();
        } else if (type == "comment") {
            trans = covariate->getComments();
        }

        undoPushator(new EditCommentCommand(trans,newText.toString(),role,signal,this,lang == "lang"));
//        emit covariatesChanged(_covariates);

    } else {

        int index = propList.at(1).toInt();
        covariate = _covariates->at(index);
        if (name == "quantity") {
            undoPushator(new EditObjCommand(covariate->getQuantity(),"dbvalue",newText));
        } else if (name == "unitString" || name == "name") {
            undoPushator(new EditObjCommand(covariate,name,newText));
        }
    }
}

/// #### Data refresh ####

void UndoRedoController::drugDataChanged()
{
    //    drugListModel->updateRow(drugListProxyModel->mapToSource(drugListProxyModel->index(drugsView->property("index").toInt(), 0)).row());
    drugCommentsModel->updateRow(drugCommentsView->property("index").toInt());
}

void UndoRedoController::editorDataChanged()
{
    editorListModel->updateRow(editorListProxyModel->mapToSource(editorListProxyModel->index(editorView->property("index").toInt(), 0)).row());
    editorCommentsModel->updateRow(editorCommentsView->property("index").toInt());

}

void UndoRedoController::dosageDataChanged()
{
    //TODO
}



/// #### entity changes ####

void UndoRedoController::drugEntityChanged(QString listViewName, int index)
{
    if(listViewName.contains("comment")) {

        undoPushator(new AddCommentCommand(_drugGeneralComments,index,&UndoRedoController::drugGeneralCommentsChanged,this));

    } else {

        QObject* drugListView = editorView->findChild<QObject *>("drugListView");
        Q_ASSERT(drugListView);

        if (index == -1) {

            undoPushator(new AddCommand(drugListModel,drugListView));

        } else if (index >= 0 && index < drugListModel->rowCount()){

        }
    }
}

void UndoRedoController::editorEntityChanged(QString listViewName, int index)
{

    if(listViewName.contains("comment")) {

        undoPushator(new AddCommentCommand(_editorComments,index,&UndoRedoController::editorCommentsChanged,this));

    } else {

        QObject* editorListView = editorView->findChild<QObject *>("editorListView");
        Q_ASSERT(editorListView);


        if(index == -1) {

            undoPushator(new AddCommand(editorListModel,editorListView));

        } else if (index >= 0 && index <editorListProxyModel->rowCount()) {

            int editorsIndex = editorListProxyModel->mapToSource(editorListProxyModel->index(index,0)).row();

            undoPushator(new RemoveCommand(editorListModel,editorListView,editorsIndex));
        }
    }
}

void UndoRedoController::dosageEntityChanged(QString listViewName, int index)
{
    if (listViewName.contains("comment")) {
        undoPushator(new AddCommentCommand(_dosesComments,index,&UndoRedoController::dosesCommentsChanged,this));
    } else if (listViewName == "Doses") {

        undoRedoStack->beginMacro("Doses change");
        undoPushator(new RemoveObjCommand(_doses,index));
        undoPushator(new ChangeViewIndexCommand(mainView->findChild<QObject *>("dosageDisplayView"),(index==-1 ? _doses->size() - 1: index-1)));
        undoRedoStack->endMacro();

    } else if ( listViewName == "Intervals") {

        undoRedoStack->beginMacro("Intakes change");
        undoPushator(new RemoveObjCommand(_intervals,index));
        undoPushator(new ChangeViewIndexCommand(mainView->findChild<QObject *>("dosageDisplayView"),(index==-1 ? _intervals->size() -1 : index-1)));
        undoRedoStack->endMacro();

    } else if ( listViewName == "Infusions") {

        undoRedoStack->beginMacro("Infusion change");
        undoPushator(new RemoveObjCommand(_infusions,index));
        undoPushator(new ChangeViewIndexCommand(mainView->findChild<QObject *>("dosageDisplayView"),(index==-1 ? _infusions->size() -1: index-1)));
        undoRedoStack->endMacro();

    }
}

void UndoRedoController::targetEntityChanged(QString listViewName, int index)
{
    QString type = listViewName.split("-").at(0);
    if (type == "comment") {
        undoPushator(new AddCommentCommand(_targetComments,index,&UndoRedoController::targetCommentsChanged,this));


    } else {
        QString position = listViewName.split("-").at(1);
        if(position == "") {

            undoRedoStack->beginMacro("target entity changed");
                undoPushator(new RemoveObjCommand(_targets,index));
            if ( index == -1 ) {
                undoPushator(new EditObjCommand(_targets->at(_targets->size() - 1),
                                                "type",ezechiel::core::Target::_typeMap.value(type)));
            }
            undoPushator(new ChangeViewIndexCommand(mainView->findChild<QObject *>("targetDisplayPanel"),(index== -1 ? _targets->size() - 1 : index - 1)));
            undoRedoStack->endMacro();
        }
    }
}

void UndoRedoController::conversionEntityChanged(QString listViewName, int index)
{
    QStringList propList = listViewName.split('-');
    QString role = propList.at(0);
    if (role == "comment") {
        ezechiel::core::TranslatableString *comment;
        void (UndoRedoController::*signal)(ezechiel::core::TranslatableString*);
        QString type = conversionView->property("conversionSelected").toString();

        if(type == "ADME") {
            comment = _admeComments;
            signal = &UndoRedoController::admeCommentsChanged;
        } else if(type == "Halflife") {
            comment = _halflifeComments;
            signal = &UndoRedoController::halflifeCommentsChanged;
        } else if (type == "Error_Model") {
            comment = _errorModelComments;
            signal = &UndoRedoController::errorModelCommentsChanged;
        } else {
            return;
        }
        undoPushator(new AddCommentCommand(comment,index,signal,this));
    }

}

void UndoRedoController::parameterEntityChanged(QString listViewName, int index)
{
    QStringList propList = listViewName.split('-');
    QString role = propList.at(0);

    QObject* view;

    if(role == "parameter") {

        view = parameterView->findChild<QObject*>("parameterSelectView");
        Q_ASSERT(view);

        undoRedoStack->beginMacro("parameters change");
        undoPushator(new RemoveObjCommand(_parameters,index));
        undoPushator(new ChangeViewIndexCommand(view,(index==-1 ? _covariates->size() - 1: index-1)));
        undoRedoStack->endMacro();
    } else if(role == "comment") {
        //TODO
    }
}


void UndoRedoController::covariateEntityChanged(QString listViewName, int index)
{

    QStringList propList = listViewName.split('-');
    QString role = propList.at(0);

    if(role == "trans") {

        ezechiel::core::TranslatableString *trans;
        void (UndoRedoController::*signal)(ezechiel::core::TranslatableString*);
        QString type = propList.at(1);
        int covariateIndex = covariateView->property("index").toInt();
        ezechiel::core::DrugVariate *covariate = _covariates->at(covariateIndex);
        if(type == "name") {
            trans = covariate->getVisualNameTranslation();
        } else if (type == "desc") {
            trans = covariate->getDescriptionTranslation();
        } else if (type == "comment") {
            trans = covariate->getComments();
        } else {
            return;
        }
        undoPushator(new AddCommentCommand(trans,index,nullptr ,nullptr));
    } else if (role == "covariate") {


        undoRedoStack->beginMacro("covariates change");
        undoPushator(new RemoveObjCommand(_covariates,index));

        undoPushator(new ChangeViewIndexCommand(mainView->findChild<QObject *>("covariateDisplayView"),(index==-1 ? _covariates->size() - 1: index-1)));
        undoRedoStack->endMacro();


    }



}


