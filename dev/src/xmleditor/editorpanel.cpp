#include "editorpanel.h"
#include "xmleditormessagehandler.h"
#include "drugxmldescriptor.h"
#include "ui_editorpanel.h"
#include "core_errors.h"

//Constructor
EditorPanel::EditorPanel(QWidget *parent) : EzPanel(parent), _ui(new Ui::EditorPanel), _sharedMenu(0)
{
    _ui->setupUi(this);

    //Set the window title for standalone mode
    setWindowTitle("Ezechiel Drug Editor");

    //Initialize the usual GUI objects
    createActions();
    createMenus();
    createToolbar();

    //Initialize the logic and model objects
    initDrugList();
    initDrugView();

    //Set the splitter stretch factor
    _ui->splitter->setStretchFactor(0, 2);
    _ui->splitter->setStretchFactor(1, 4);

    //Set the main layout stretch factor
    _ui->mainLayout->setStretchFactor(_ui->splitter, 7);
    _ui->mainLayout->setStretchFactor(_ui->stackedWidgets, 8);

    //Set up the signals
    connect(_ui->drugView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(drugSelected(const QModelIndex &)));
    connect(_ui->drugView, SIGNAL(clicked(const QModelIndex &)), _ui->stackedWidgets, SLOT(setEmptyItemWidget()));
    connect(_domSelection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(itemSelected(QModelIndex)));
}

//Destructor
EditorPanel::~EditorPanel()
{
    delete _ui;
}

//Returns the panel menus
QMenu *EditorPanel::menus()
{
    return _sharedMenu;
}

//Returns the panel icon
QIcon EditorPanel::icon()
{
    return QIcon(":/icons/edit.png");
}

//Returns the panel title
QString EditorPanel::title()
{
    return tr("Editor");
}

//Returns the panel description
QString EditorPanel::description()
{
    return tr("The editor allows to create and edit new drugs XML files which can be used by Ezechiel");
}

//Sets up the editor actions
void EditorPanel::createActions()
{

}

//Sets up the editor menus
void EditorPanel::createMenus()
{
    _sharedMenu = new QMenu(tr("editor"), this);
}

//Sets up the editor toolbar
void EditorPanel::createToolbar()
{

}

//Initializes the editor drugs list
void EditorPanel::initDrugList()
{
    //Populate the drugs model
    foreach (DrugXmlDescriptor *drug, _engine.drugs()) {

        //Set up the drug item
        QStandardItem *item = new QStandardItem(drug->name());
        item->setData(drug->id(), Qt::UserRole);
        item->setData(drug->shortDesc(), Qt::ToolTipRole);
        item->setEditable(false);

        //Add it to the model
        _drugModel.appendRow(item);
    }

    //Sort the elements
    _drugModel.sort(0);

    //Set the drugs list model
    _ui->drugView->setModel(&_drugModel);
}

//Initializes the editor drug view
void EditorPanel::initDrugView()
{
    _ui->domView->setModel(&_domModel);
    _domSelection = _ui->domView->selectionModel();
}

//Manages a drug selection change
void EditorPanel::drugSelected(const QModelIndex &index)
{
    //Check if the index is valid
    if (!index.isValid()) {

        //Reset the DOM model
        _domModel.clear();

        //Do nothing else
        return;
    }

    //Get the corresponding drug ID
    QString drugId = _drugModel.item(index.row())->data(Qt::UserRole).toString();

    //Build the corresponding XLM tree
    DrugXmlTree drugTree(_engine.drug(drugId));

    // ToDo /////////////////////////////////////////////
    // Print a dialog, remove the drug from the list and
    // remove the corresponding drug XML descriptor
    /////////////////////////////////////////////////////

    //Initialite the XLM tree
    if (!drugTree.initialize())
        ERROR(XMLERROR, tr("Error upon initialization of the drug ID '%1' XML tree").arg(drugId));

    //Set the tree in the DOM model
    _domModel.setDocument(drugTree.tree());
}


//Manages an item selection change of the current drug
void EditorPanel::itemSelected(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    _ui->stackedWidgets->setCurrentItemWidget(static_cast<DomItem *>(index.internalPointer()));
}
