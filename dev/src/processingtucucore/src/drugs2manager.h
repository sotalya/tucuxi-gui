#ifndef DRUGS2MANAGER_H
#define DRUGS2MANAGER_H

#include <vector>
#include <string>

#include <QDir>

namespace ezechiel {
namespace core {

class DrugModel;

}
}

namespace Tucuxi {
namespace Core {

class DrugModel;

}
}

class Drugs2Manager
{
public:

    static Drugs2Manager *getInstance();

    void buildAllDrugModels(std::string dirName);

    const std::vector<Tucuxi::Core::DrugModel *> & getAllTucucoreDrugModels() const;

    const std::vector<ezechiel::core::DrugModel *> & getAllEzechielDrugModels() const;

    Tucuxi::Core::DrugModel *getTucucoreById(std::string id) const;


protected:
    Drugs2Manager();

    std::vector<Tucuxi::Core::DrugModel *> m_tucuxiDrugModels;

    std::map<std::string, Tucuxi::Core::DrugModel *> m_tucuxiDrugModelsByIds;

    std::vector<ezechiel::core::DrugModel *> m_ezechielDrugModels;

    void scanDirectory(const QDir &directory);

    Tucuxi::Core::DrugModel* scanDrug(const QString & fileName);

#ifdef DRUGMODELTESTS
    void addDrugModelTests();
#endif // DRUGMODELTESTS

};

#endif // DRUGS2MANAGER_H
