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


#ifndef DRUGS2MANAGER_H
#define DRUGS2MANAGER_H

#include <vector>
#include <string>

#include <QDir>

#ifdef CONFIG_SIGN
#include "tucusign/signvalidator.h"
#include "tucusign/signparser.h"
#endif // CONFIG_SIGN

namespace Tucuxi {
namespace Gui {
namespace Core {


class DrugModel;

}
}
}

namespace Tucuxi {
namespace Core {

class DrugModel;

}
}

namespace Tucuxi {
namespace Gui {
namespace Processing {

class Drugs2Manager
{
public:

    static Drugs2Manager *getInstance();

    void buildAllDrugModels(std::string dirName);

    const std::vector<Tucuxi::Core::DrugModel *> & getAllTucucoreDrugModels() const;

    const std::vector<Tucuxi::Gui::Core::DrugModel *> & getAllGuiDrugModels() const;

    Tucuxi::Core::DrugModel *getTucucoreById(std::string id) const;

#ifdef CONFIG_SIGN
    Tucuxi::Sign::Signer /*bool*/ checkSign(std::string fileName);
    Tucuxi::Sign::Signer /*bool*/ checkSign(const Tucuxi::Gui::Core::DrugModel* drugModel);
#endif // CONFIG_SIGN

    Drugs2Manager();

protected:
//    Drugs2Manager();

    std::vector<Tucuxi::Core::DrugModel *> m_tucuxiDrugModels;

    std::map<std::string, Tucuxi::Core::DrugModel *> m_tucuxiDrugModelsByIds;

    std::vector<Tucuxi::Gui::Core::DrugModel *> m_guiDrugModels;

    void scanDirectory(const QDir &directory);

    Tucuxi::Core::DrugModel* scanDrug(const QString & fileName);

#ifdef DRUGMODELTESTS
    void addDrugModelTests();
#endif // DRUGMODELTESTS

};

}
}
}

#endif // DRUGS2MANAGER_H
