#ifndef PERSISTENTFILEDIALOG_H
#define PERSISTENTFILEDIALOG_H

#include <QFileDialog>

/** \ingroup dialogs
 *  \brief File dialog which remembers the previously selected files and directories.
 *
 * This dialog is a standard QFileDialog except it will remember the previously selected files and
 * directories, and will restore them every time the dialog is executed.
 *
 * \sa QFileDialog
 */
class PersistentFileDialog : public QFileDialog
{
    Q_OBJECT

public:
    //! Constructor.
    PersistentFileDialog(QWidget *parent, Qt::WindowFlags flags);

    //! Constructor.
    PersistentFileDialog(QWidget *parent = 0, const QString &caption = QString(), const QString &directory = QString(), const QString &filter = QString());

public slots:

    //! Reimplemented from QDialog::exec().
    /** Reimplemented from QDialog::exec().
     * This function will restore the current directory and file, if any, and call the parent
     * implementation.
     * @return The dialog result code.
     */
    virtual int exec();

protected slots:
    //Save the current directory
    void setCurrentDirectory(const QString &directory);

    //Save the current file
    void setCurrentFile(const QString &file);

private:
    //Establish the internal connections
    void trackChanges();

    //The current directory and file
    QString _currentDirectory;
    QString _currentFile;

};

#endif // PERSISTENTFILEDIALOG_H
