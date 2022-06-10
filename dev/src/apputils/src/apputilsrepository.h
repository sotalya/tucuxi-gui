#ifndef APPUTILSREPOSITORY_H
#define APPUTILSREPOSITORY_H

#include "core/interfaces/abstractrepository.h"
#include "core/utils/errorenums.h"
//#include "apputils/src/user.h"
class User;

#define APPUTILSREPO (dynamic_cast<AppUtilsRepository*>(ABSTRACTREPO))

namespace Tucuxi {
namespace Gui {
namespace Core {

    class DrugModel;
    class DBAccount;
    class ValidDoses;
    class ValidIntervals;
    class ValidInfusions;
    class DrugVariate;
    class Descriptor;
    class ActiveSubstance;
}
}
}
class AppUtilsRepository : public Tucuxi::Gui::Core::AbstractRepository
{
public:

    virtual bool connect (Tucuxi::Gui::Core::DBAccount& account) = 0;
    virtual bool disconnect () = 0;
    virtual void needs (bool &url, bool &user, bool &password) = 0;
    virtual bool tryRequest(const Tucuxi::Gui::Core::Response&, const QString&, QString&) = 0;

    virtual Tucuxi::Gui::Core::Response getUser(int, User&) = 0;
    virtual Tucuxi::Gui::Core::Response getUserKey(const QString&, QString&) = 0;
    virtual Tucuxi::Gui::Core::Response setUserKey(QString, const QString&) = 0;
    virtual Tucuxi::Gui::Core::Response getUser(const QString&, User&) = 0;
    virtual Tucuxi::Gui::Core::Response getUsersList(QList<User>&) = 0;
    virtual Tucuxi::Gui::Core::Response setUser(const User&) = 0;
    virtual Tucuxi::Gui::Core::Response deleteUser(QString) = 0;

    virtual Tucuxi::Gui::Core::Response isEncrypted(const bool&) = 0;
    virtual Tucuxi::Gui::Core::Response setEncrypted(const bool&) = 0;
    virtual Tucuxi::Gui::Core::Response encryptDatabase(const bool&) = 0;

    virtual Tucuxi::Gui::Core::Response getDrugFromId(const int&, Tucuxi::Gui::Core::DrugModel* ) = 0;
    virtual Tucuxi::Gui::Core::Response setDrug(Tucuxi::Gui::Core::DrugModel*) = 0;
    virtual Tucuxi::Gui::Core::Response getDrugsList(QList<Tucuxi::Gui::Core::DrugModel*>&) = 0;
    virtual Tucuxi::Gui::Core::Response deleteDrug(Tucuxi::Gui::Core::DrugModel*) = 0;
    virtual Tucuxi::Gui::Core::Response getDrugOfName(const QString&, Tucuxi::Gui::Core::DrugModel*&) = 0;
    virtual Tucuxi::Gui::Core::Response getActiveSubstanceOfId(const QString&, Tucuxi::Gui::Core::ActiveSubstance*&) = 0;
    virtual Tucuxi::Gui::Core::Response getDrugOfDrugId(const QString&, Tucuxi::Gui::Core::DrugModel*&) = 0;

    virtual Tucuxi::Gui::Core::Response setValidDoses(const int&, Tucuxi::Gui::Core::ValidDoses*) = 0;
    virtual Tucuxi::Gui::Core::Response setValidIntervals(const int&, Tucuxi::Gui::Core::ValidIntervals*) = 0;
    virtual Tucuxi::Gui::Core::Response setValidInfusions(const int&, Tucuxi::Gui::Core::ValidInfusions*) = 0;

    virtual Tucuxi::Gui::Core::Response getDrugVariateFromId(const int&, Tucuxi::Gui::Core::DrugVariate*) = 0;
    virtual Tucuxi::Gui::Core::Response setDrugVariate(Tucuxi::Gui::Core::DrugVariate*) = 0;
    virtual Tucuxi::Gui::Core::Response setDrugVariate(const int&, Tucuxi::Gui::Core::DrugVariate*) = 0;
    virtual Tucuxi::Gui::Core::Response getDrugVariatesList(QList<Tucuxi::Gui::Core::DrugVariate*>&) = 0;
    virtual Tucuxi::Gui::Core::Response getDrugVariatesByDate(const int&, QList<Tucuxi::Gui::Core::DrugVariate*>&) = 0;
    virtual Tucuxi::Gui::Core::Response deleteDrugVariate(Tucuxi::Gui::Core::DrugVariate*) = 0;
    virtual Tucuxi::Gui::Core::Response deleteDrugVariate(const int&) = 0;

    virtual Tucuxi::Gui::Core::Descriptor getDescriptor() = 0;
    virtual QString errorString() = 0;
};

Q_DECLARE_INTERFACE(AppUtilsRepository, "ch.heig-vd.ezechiel.AppUtilsInterface/0.1")

#endif // APPUTILSREPOSITORY_H
