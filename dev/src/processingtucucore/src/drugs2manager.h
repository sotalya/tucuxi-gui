//@@license@@

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
