#ifndef APPUTILSREPOSITORY_H
#define APPUTILSREPOSITORY_H

#include "core/interfaces/abstractrepository.h"
#include "core/utils/errorenums.h"
//#include "apputils/src/user.h"
class User;

#define APPUTILSREPO (dynamic_cast<AppUtilsRepository*>(ABSTRACTREPO))

namespace ezechiel {
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
class AppUtilsRepository : public ezechiel::GuiCore::AbstractRepository
{
public:

    virtual bool connect (ezechiel::GuiCore::DBAccount& account) = 0;
    virtual bool disconnect () = 0;
    virtual void needs (bool &url, bool &user, bool &password) = 0;
    virtual bool tryRequest(const ezechiel::GuiCore::Response&, const QString&, QString&) = 0;

    virtual ezechiel::GuiCore::Response getUser(int, User&) = 0;
    virtual ezechiel::GuiCore::Response getUserKey(const QString&, QString&) = 0;
    virtual ezechiel::GuiCore::Response setUserKey(QString, const QString&) = 0;
    virtual ezechiel::GuiCore::Response getUser(const QString&, User&) = 0;
    virtual ezechiel::GuiCore::Response getUsersList(QList<User>&) = 0;
    virtual ezechiel::GuiCore::Response setUser(const User&) = 0;
    virtual ezechiel::GuiCore::Response deleteUser(QString) = 0;

    virtual ezechiel::GuiCore::Response isEncrypted(const bool&) = 0;
    virtual ezechiel::GuiCore::Response setEncrypted(const bool&) = 0;
    virtual ezechiel::GuiCore::Response encryptDatabase(const bool&) = 0;

    virtual ezechiel::GuiCore::Response getDrugFromId(const int&, ezechiel::GuiCore::DrugModel* ) = 0;
    virtual ezechiel::GuiCore::Response setDrug(ezechiel::GuiCore::DrugModel*) = 0;
    virtual ezechiel::GuiCore::Response getDrugsList(QList<ezechiel::GuiCore::DrugModel*>&) = 0;
    virtual ezechiel::GuiCore::Response deleteDrug(ezechiel::GuiCore::DrugModel*) = 0;
    virtual ezechiel::GuiCore::Response getDrugOfName(const QString&, ezechiel::GuiCore::DrugModel*&) = 0;
    virtual ezechiel::GuiCore::Response getActiveSubstanceOfId(const QString&, ezechiel::GuiCore::ActiveSubstance*&) = 0;
    virtual ezechiel::GuiCore::Response getDrugOfDrugId(const QString&, ezechiel::GuiCore::DrugModel*&) = 0;

    virtual ezechiel::GuiCore::Response setValidDoses(const int&, ezechiel::GuiCore::ValidDoses*) = 0;
    virtual ezechiel::GuiCore::Response setValidIntervals(const int&, ezechiel::GuiCore::ValidIntervals*) = 0;
    virtual ezechiel::GuiCore::Response setValidInfusions(const int&, ezechiel::GuiCore::ValidInfusions*) = 0;

    virtual ezechiel::GuiCore::Response getDrugVariateFromId(const int&, ezechiel::GuiCore::DrugVariate*) = 0;
    virtual ezechiel::GuiCore::Response setDrugVariate(ezechiel::GuiCore::DrugVariate*) = 0;
    virtual ezechiel::GuiCore::Response setDrugVariate(const int&, ezechiel::GuiCore::DrugVariate*) = 0;
    virtual ezechiel::GuiCore::Response getDrugVariatesList(QList<ezechiel::GuiCore::DrugVariate*>&) = 0;
    virtual ezechiel::GuiCore::Response getDrugVariatesByDate(const int&, QList<ezechiel::GuiCore::DrugVariate*>&) = 0;
    virtual ezechiel::GuiCore::Response deleteDrugVariate(ezechiel::GuiCore::DrugVariate*) = 0;
    virtual ezechiel::GuiCore::Response deleteDrugVariate(const int&) = 0;

    virtual ezechiel::GuiCore::Descriptor getDescriptor() = 0;
    virtual QString errorString() = 0;
};

Q_DECLARE_INTERFACE(AppUtilsRepository, "ch.heig-vd.ezechiel.AppUtilsInterface/0.1")

#endif // APPUTILSREPOSITORY_H
