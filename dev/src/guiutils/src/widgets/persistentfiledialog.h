/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef PERSISTENTFILEDIALOG_H
#define PERSISTENTFILEDIALOG_H

#include <QFileDialog>

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

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

}
}
}

#endif // PERSISTENTFILEDIALOG_H
