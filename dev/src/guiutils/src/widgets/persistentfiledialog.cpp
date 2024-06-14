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


#include "persistentfiledialog.h"

using namespace Tucuxi::Gui::GuiUtils;

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
