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


#include "startupwindow.h"

#include <QQmlContext>
#include <QQuickItem>

using namespace Tucuxi::Gui::GuiUtils;

StartupWindow::StartupWindow(QWidget *parent) :
    QQuickWidget(parent)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowModality(Qt::WindowModal);

    rootContext()->setContextProperty("window", this);
    //setSource(QUrl("qrc:/windows/StartupWindow.qml"));
    setSource(QUrl("qrc:/windows/StartupScreen.qml"));

    connect(rootObject(), SIGNAL(optionSelected(int)), this, SLOT(close()));
    connect(rootObject(), SIGNAL(optionSelected(int)), this, SIGNAL(optionSelected(int)));
}

StartupWindow::~StartupWindow()
{

}

void StartupWindow::setWidth(int width)
{
    setMinimumWidth(width);
}

void StartupWindow::setHeight(int height)
{
    setMinimumHeight(height);
}
