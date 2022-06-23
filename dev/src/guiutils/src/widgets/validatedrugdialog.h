//@@license@@s

#ifndef VALIDATEDRUG_H
#define VALIDATEDRUG_H

#include <QDialog>

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class PersistentFileDialog;

namespace Ui {
    class ValidateDrugDialog;
}

/** \ingroup dialogs
 *  \brief Dialog used to validate drug XML files.
 *
 * The ValidateDrug dialog can be used to validate drug XML files using the same process the core is
 * validating existing drugs. The user can select an XML file and try to validate it. If the file is
 * valid, a label will specify it. If it isn't, an error message will be displayed in a plain text area.
 *
 * \sa DrugManager, Drug
 */
class ValidateDrugDialog : public QDialog
{
    Q_OBJECT
    
public:

    /** \brief Constructor.
     * @param parent An optional parent widget.
     */
    explicit ValidateDrugDialog(QWidget *parent = 0);

    //! Destructor.
    ~ValidateDrugDialog();
    
public slots:

    //! Reimplemented from QDialog::exec().
    /** Reimplemented from QDialog::exec().
     * This function will clear any previous error or validation messages and call the parent implementation.
     */
    virtual int exec();

private:
    //The shared strings
    static const char *const _OK;
    static const char *const _FAILED;

    //The scan strings
    static const char *const _SCAN_INIT;
    static const char *const _SCAN_ERROR;

    //The build strings
    static const char *const _BUILD_INIT;
    static const char *const _BUILD_ERROR;

    //The validation strings
    static const char *const _VALIDATION_INIT;
    static const char *const _VALIDATION_ERROR;

    //The label strings
    static const char *const _SUCCESS_MESSAGE;
    static const char *const _WARNING_MESSAGE;
    static const char *const _ERROR_MESSAGE;

    //The label colors
    static const char *const _SUCCESS_COLOR;
    static const char *const _WARNING_COLOR;
    static const char *const _ERROR_COLOR;

    //Sets the error messages
    void setErrorMessages(const char *errorInit, const char *errorMsg, const char *labelMsg, const char *labelColor, const QPixmap &icon);

    //The dialog GUI
    Ui::ValidateDrugDialog *_ui;

    //The files browser
    PersistentFileDialog *_fileDialog;

    //The validation icons
    QPixmap _success_icon;
    QPixmap _warning_icon;
    QPixmap _error_icon;

    std::string _errorMessage;

private slots:
    //Browses the files
    void browseFiles();

    //Validates the drug
    void validateDrug();

    //Clears the errors
    void clearErrors();
};

}
}
}

#endif // VALIDATEDRUG_H
