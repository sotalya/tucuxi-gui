/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
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


#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QQuickWidget>

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class StartupWindow : public QQuickWidget
{
    Q_OBJECT

public:
    enum Option {
        PendingRequests,
        PendingRequestGivenPatient,
        NewPatient,
        LoadRequest,
        Shutdown
    };
    Q_ENUM(Option)

    explicit StartupWindow(QWidget *parent = 0);
    virtual ~StartupWindow();

    Q_INVOKABLE void setWidth(int width);
    Q_INVOKABLE void setHeight(int height);

signals:
    void optionSelected(int option);
};

}
}
}

#endif // STARTUPWINDOW_H
