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

#ifndef APPMODE_H
#define APPMODE_H

#include <QObject>

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class AppMode : public QObject
{
private:
    Q_OBJECT
    static AppMode* m_instance;
    AppMode();

public:
    static AppMode* getInstance(){
        if (m_instance == nullptr){
            m_instance = new AppMode();
        }
        return m_instance;
    }
    Q_INVOKABLE bool isDemo();
    Q_INVOKABLE bool isEducational();
    Q_INVOKABLE bool isInstitutional();
    Q_INVOKABLE bool isPractice();

};

}
}
}

#endif // APPMODE_H
