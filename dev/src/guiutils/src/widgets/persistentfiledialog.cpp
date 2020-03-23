#include "persistentfiledialog.h"

PersistentFileDialog::PersistentFileDialog(QWidget *parent, Qt::WindowFlags flags) : QFileDialog(parent, flags)
{
    trackChanges();
}

PersistentFileDialog::PersistentFileDialog(QWidget *parent, const QString &caption, const QString &directory, const QString &filter) :
    QFileDialog(parent, caption, directory, filter)
{
    trackChanges();
}

int PersistentFileDialog::exec()
{
    //Set the current directory and file
    if (!_currentDirectory.isEmpty())
        setDirectory(_currentDirectory);
    if (!_currentFile.isEmpty())
        selectFile(_currentFile);

    //Call the parent implementation
    return QFileDialog::exec();
}

void PersistentFileDialog::setCurrentDirectory(const QString &directory)
{
    _currentDirectory = directory;
}

void PersistentFileDialog::setCurrentFile(const QString &file)
{
    _currentFile = file;
}

void PersistentFileDialog::trackChanges()
{
    connect(this, SIGNAL(directoryEntered(QString)), this, SLOT(setCurrentDirectory(QString)));
    connect(this, SIGNAL(fileSelected(QString)), this, SLOT(setCurrentFile(QString)));
}
