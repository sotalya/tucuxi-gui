#ifndef APPUTILSREPOSITORY_H
#define APPUTILSREPOSITORY_H

#include "core/interfaces/abstractrepository.h"
#include "core/utils/errorenums.h"
//#include "apputils/src/user.h"
class User;

#define APPUTILSREPO (dynamic_cast<AppUtilsRepository*>(ABSTRACTREPO))

namespace Tucuxi {
namespace GuiCore {
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
class AppUtilsRepository : public Tucuxi::GuiCore::AbstractRepository
{
public:

    virtual bool connect (Tucuxi::GuiCore::DBAccount& account) = 0;
    virtual bool disconnect () = 0;
    virtual void needs (bool &url, bool &user, bool &password) = 0;
    virtual bool tryRequest(const Tucuxi::GuiCore::Response&, const QString&, QString&) = 0;

    virtual Tucuxi::GuiCore::Response getUser(int, User&) = 0;
    virtual Tucuxi::GuiCore::Response getUserKey(const QString&, QString&) = 0;
    virtual Tucuxi::GuiCore::Response setUserKey(QString, const QString&) = 0;
    virtual Tucuxi::GuiCore::Response getUser(const QString&, User&) = 0;
    virtual Tucuxi::GuiCore::Response getUsersList(QList<User>&) = 0;
    virtual Tucuxi::GuiCore::Response setUser(const User&) = 0;
    virtual Tucuxi::GuiCore::Response deleteUser(QString) = 0;

    virtual Tucuxi::GuiCore::Response isEncrypted(const bool&) = 0;
    virtual Tucuxi::GuiCore::Response setEncrypted(const bool&) = 0;
    virtual Tucuxi::GuiCore::Response encryptDatabase(const bool&) = 0;

    virtual Tucuxi::GuiCore::Response getDrugFromId(const int&, Tucuxi::GuiCore::DrugModel* ) = 0;
    virtual Tucuxi::GuiCore::Response setDrug(Tucuxi::GuiCore::DrugModel*) = 0;
    virtual Tucuxi::GuiCore::Response getDrugsList(QList<Tucuxi::GuiCore::DrugModel*>&) = 0;
    virtual Tucuxi::GuiCore::Response deleteDrug(Tucuxi::GuiCore::DrugModel*) = 0;
    virtual Tucuxi::GuiCore::Response getDrugOfName(const QString&, Tucuxi::GuiCore::DrugModel*&) = 0;
    virtual Tucuxi::GuiCore::Response getActiveSubstanceOfId(const QString&, Tucuxi::GuiCore::ActiveSubstance*&) = 0;
    virtual Tucuxi::GuiCore::Response getDrugOfDrugId(const QString&, Tucuxi::GuiCore::DrugModel*&) = 0;

    virtual Tucuxi::GuiCore::Response setValidDoses(const int&, Tucuxi::GuiCore::ValidDoses*) = 0;
    virtual Tucuxi::GuiCore::Response setValidIntervals(const int&, Tucuxi::GuiCore::ValidIntervals*) = 0;
    virtual Tucuxi::GuiCore::Response setValidInfusions(const int&, Tucuxi::GuiCore::ValidInfusions*) = 0;

    virtual Tucuxi::GuiCore::Response getDrugVariateFromId(const int&, Tucuxi::GuiCore::DrugVariate*) = 0;
    virtual Tucuxi::GuiCore::Response setDrugVariate(Tucuxi::GuiCore::DrugVariate*) = 0;
    virtual Tucuxi::GuiCore::Response setDrugVariate(const int&, Tucuxi::GuiCore::DrugVariate*) = 0;
    virtual Tucuxi::GuiCore::Response getDrugVariatesList(QList<Tucuxi::GuiCore::DrugVariate*>&) = 0;
    virtual Tucuxi::GuiCore::Response getDrugVariatesByDate(const int&, QList<Tucuxi::GuiCore::DrugVariate*>&) = 0;
    virtual Tucuxi::GuiCore::Response deleteDrugVariate(Tucuxi::GuiCore::DrugVariate*) = 0;
    virtual Tucuxi::GuiCore::Response deleteDrugVariate(const int&) = 0;

    virtual Tucuxi::GuiCore::Descriptor getDescriptor() = 0;
    virtual QString errorString() = 0;
};

Q_DECLARE_INTERFACE(AppUtilsRepository, "ch.heig-vd.ezechiel.AppUtilsInterface/0.1")

#endif // APPUTILSREPOSITORY_H
