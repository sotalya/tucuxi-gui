#include "validatedrugdialog.h"
#include "ui_validatedrugdialog.h"
#include "core/core.h"
#include "persistentfiledialog.h"

#include "apputils/src/appcore.h"

#include "tucucore/drugmodelchecker.h"
#include "tucucore/drugmodelimport.h"
#include "tucucore/pkmodel.h"

#include <QStandardPaths>
#include <QSettings>

//The shared strings
const char *const ValidateDrugDialog::_OK               =  QT_TR_NOOP_UTF8("OK!");
const char *const ValidateDrugDialog::_FAILED           =  QT_TR_NOOP_UTF8("Failed.");

//The scan strings
const char *const ValidateDrugDialog::_SCAN_INIT        =  QT_TR_NOOP_UTF8("Step 1: Verifying the drug version, structure and header...");
const char *const ValidateDrugDialog::_SCAN_ERROR       =  QT_TR_NOOP_UTF8("\nThe parser returned the following error:");

//The build strings
const char *const ValidateDrugDialog::_BUILD_INIT       =  QT_TR_NOOP_UTF8("Step 2: Extracting the data and building the drug...");
const char *const ValidateDrugDialog::_BUILD_ERROR      =  QT_TR_NOOP_UTF8("\nThe builder returned the following error:");

//The validation strings
const char *const ValidateDrugDialog::_VALIDATION_INIT  =  QT_TR_NOOP_UTF8("Step 3: Validating the data and checking the model...");
const char *const ValidateDrugDialog::_VALIDATION_ERROR =  QT_TR_NOOP_UTF8("\nThe validator returned the following error:");

//The label strings
const char *const ValidateDrugDialog::_SUCCESS_MESSAGE  =  QT_TR_NOOP_UTF8("The drug is valid!");
const char *const ValidateDrugDialog::_WARNING_MESSAGE  =  QT_TR_NOOP_UTF8("The drug file is well formed but contains incomplete or unknown data.");
const char *const ValidateDrugDialog::_ERROR_MESSAGE    =  QT_TR_NOOP_UTF8("The drug is invalid.");

//The label strings
const char *const ValidateDrugDialog::_SUCCESS_COLOR    =  "color: darkGreen";
const char *const ValidateDrugDialog::_WARNING_COLOR    =  "color: darkOrange";
const char *const ValidateDrugDialog::_ERROR_COLOR      =  "color: darkRed";

//Constructor
ValidateDrugDialog::ValidateDrugDialog(QWidget *parent) :
    QDialog(parent), _ui(new Ui::ValidateDrugDialog), _fileDialog(nullptr), _success_icon(":icons/checked.png"), _warning_icon(":icons/warning.png"), _error_icon(":icons/error.png")
{
    //Setup the GUI
    _ui->setupUi(this);

    //Setup the file dialog
    QSettings settings;
    QString path = settings.value("validatedrugdialog/path", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
    _fileDialog = new PersistentFileDialog(this, tr("Select a drug"), path, tr("Tucuxi Drug Description files (*.tdd)"));
    _fileDialog->setFileMode(QFileDialog::ExistingFile);

    //Set the dialog title
    setWindowTitle(tr("Validate a drug"));

    //Establish the connexions
    connect(_ui->fileWidget, SIGNAL(textChanged(QString)), this, SLOT(clearErrors()));
    connect(_ui->fileButton, SIGNAL(pressed()), this, SLOT(browseFiles()));
    connect(_ui->validateButton, SIGNAL(pressed()), this, SLOT(clearErrors()));
    connect(_ui->validateButton, SIGNAL(pressed()), this, SLOT(validateDrug()));
}

//Destructor
ValidateDrugDialog::~ValidateDrugDialog()
{
    delete _ui;
}

//Clear the GUI
int ValidateDrugDialog::exec()
{
    //Clear the previous errors
    clearErrors();

    //Call the parent implementation
    return QDialog::exec();
}


//Sets the error messages
void ValidateDrugDialog::setErrorMessages(const char *errorInit, const char *errorMsg, const char *labelMsg, const char *labelColor, const QPixmap &icon)
{
    //Format the error message
    QString managerErrorMsg = QString::fromStdString(_errorMessage);
    if (managerErrorMsg[0].isUpper())
        managerErrorMsg[0] = managerErrorMsg[0].toLower();

    //Set the error widget text
    _ui->errorWidget->appendPlainText(tr(errorInit) + " " + tr(_FAILED));
    _ui->errorWidget->appendPlainText(tr(errorMsg)  + " " + managerErrorMsg);

    //Set the validation label
    _ui->validateLabel->setText(tr(labelMsg));
    _ui->validateLabel->setStyleSheet(labelColor);

    //Set the validation icon
    _ui->validateIcon->setPixmap(icon.scaledToHeight(_ui->validateLabel->size().height(), Qt::SmoothTransformation));
}

//Browses the files
void ValidateDrugDialog::browseFiles()
{
    //Open the file dialog
    if (!_fileDialog->exec())
        return;

    //Get the file path list
    QStringList filePathList = _fileDialog->selectedFiles();

    if (filePathList.size() != 1)
        return;

    //Get the file path
    QString filePath = filePathList.first();

    if (filePath.isEmpty())
        return;


    QSettings settings;
    settings.setValue("validatedrugdialog/path", QFileInfo(filePath).absoluteDir().path());


    //Set it in line edit
    _ui->fileWidget->setText(filePath);

    validateDrug();
}

//Validates the drug
void ValidateDrugDialog::validateDrug()
{
    QString filePath = _ui->fileWidget->text();

    _errorMessage = "";

    //Check if a file is selected
    if (filePath.isEmpty()) {
        clearErrors();
        _ui->errorWidget->setPlainText(tr("Please select a file."));
        return;
    }

    //Check if the file exits
    if (!QFile::exists(filePath)) {
        clearErrors();
        _ui->errorWidget->setPlainText(tr("The file '%1' does not exist.").arg(filePath));
        return;
    }

    //Scan the drug

    Tucuxi::Core::DrugModel *dModel;

    Tucuxi::Core::DrugModelImport importer;
    if (importer.importFromFile(dModel, filePath.toStdString()) != Tucuxi::Common::IImport::Status::Ok) {
        _errorMessage = "Can not import the drug file.\n\n" + importer.getErrorMessage();
        setErrorMessages(_VALIDATION_INIT, _VALIDATION_ERROR, _ERROR_MESSAGE, _WARNING_COLOR, _error_icon);
        return;
    }

    _ui->errorWidget->appendPlainText(tr(_SCAN_INIT) + " " + tr(_OK));
    _ui->errorWidget->appendPlainText(tr(_BUILD_INIT) + " " + tr(_OK));


    Tucuxi::Core::DrugModelChecker checker;

    Tucuxi::Core::PkModelCollection pkCollection;

    if (!defaultPopulate(pkCollection)) {
        _errorMessage = "Could not populate the Pk models collection. No model will be available";
        setErrorMessages(_VALIDATION_INIT, _VALIDATION_ERROR, _WARNING_MESSAGE, _WARNING_COLOR, _warning_icon);
        return;
    }

    Tucuxi::Core::DrugModelChecker::CheckerResult_t checkerResult = checker.checkDrugModel(dModel, &pkCollection);
    if (!checkerResult.m_ok) {
        _errorMessage = checkerResult.m_errorMessage;
        setErrorMessages(_VALIDATION_INIT, _VALIDATION_ERROR, _WARNING_MESSAGE, _WARNING_COLOR, _warning_icon);
        return;
    }

    _ui->errorWidget->appendPlainText(tr(_VALIDATION_INIT) + " " + tr(_OK));

    //Set the result label
    _ui->validateLabel->setText(tr(_SUCCESS_MESSAGE));
    _ui->validateLabel->setStyleSheet(_SUCCESS_COLOR);

    //Set the validation icon
    _ui->validateIcon->setPixmap(_success_icon.scaledToHeight(_ui->validateLabel->size().height(), Qt::SmoothTransformation));
}

//Clears the errors
void ValidateDrugDialog::clearErrors()
{
    _ui->validateLabel->clear();
    _ui->validateIcon->clear();
    _ui->errorWidget->clear();
}
