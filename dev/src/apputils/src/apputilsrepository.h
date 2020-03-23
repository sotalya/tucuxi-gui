#ifndef APPUTILSREPOSITORY_H
#define APPUTILSREPOSITORY_H

#include "core/interfaces/abstractrepository.h"
#include "core/utils/errorenums.h"
//#include "apputils/src/user.h"
class User;

#define APPUTILSREPO (dynamic_cast<AppUtilsRepository*>(ABSTRACTREPO))

namespace ezechiel {
namespace core {
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
class AppUtilsRepository : public ezechiel::core::AbstractRepository
{
public:

    virtual bool connect (ezechiel::core::DBAccount& account) = 0;
    virtual bool disconnect () = 0;
    virtual void needs (bool &url, bool &user, bool &password) = 0;
    virtual bool tryRequest(const ezechiel::core::Response&, const QString&, QString&) = 0;

    virtual ezechiel::core::Response getUser(int, User&) = 0;
    virtual ezechiel::core::Response getUserKey(const QString&, QString&) = 0;
    virtual ezechiel::core::Response setUserKey(QString, const QString&) = 0;
    virtual ezechiel::core::Response getUser(const QString&, User&) = 0;
    virtual ezechiel::core::Response getUsersList(QList<User>&) = 0;
    virtual ezechiel::core::Response setUser(const User&) = 0;
    virtual ezechiel::core::Response deleteUser(QString) = 0;

    virtual ezechiel::core::Response isEncrypted(const bool&) = 0;
    virtual ezechiel::core::Response setEncrypted(const bool&) = 0;
    virtual ezechiel::core::Response encryptDatabase(const bool&) = 0;

    virtual ezechiel::core::Response getDrugFromId(const int&, ezechiel::core::DrugModel* ) = 0;
    virtual ezechiel::core::Response setDrug(ezechiel::core::DrugModel*) = 0;
    virtual ezechiel::core::Response getDrugsList(QList<ezechiel::core::DrugModel*>&) = 0;
    virtual ezechiel::core::Response deleteDrug(ezechiel::core::DrugModel*) = 0;
    virtual ezechiel::core::Response getDrugOfName(const QString&, ezechiel::core::DrugModel*&) = 0;
    virtual ezechiel::core::Response getActiveSubstanceOfId(const QString&, ezechiel::core::ActiveSubstance*&) = 0;
    virtual ezechiel::core::Response getDrugOfDrugId(const QString&, ezechiel::core::DrugModel*&) = 0;

    virtual ezechiel::core::Response setValidDoses(const int&, ezechiel::core::ValidDoses*) = 0;
    virtual ezechiel::core::Response setValidIntervals(const int&, ezechiel::core::ValidIntervals*) = 0;
    virtual ezechiel::core::Response setValidInfusions(const int&, ezechiel::core::ValidInfusions*) = 0;

    virtual ezechiel::core::Response getDrugVariateFromId(const int&, ezechiel::core::DrugVariate*) = 0;
    virtual ezechiel::core::Response setDrugVariate(ezechiel::core::DrugVariate*) = 0;
    virtual ezechiel::core::Response setDrugVariate(const int&, ezechiel::core::DrugVariate*) = 0;
    virtual ezechiel::core::Response getDrugVariatesList(QList<ezechiel::core::DrugVariate*>&) = 0;
    virtual ezechiel::core::Response getDrugVariatesByDate(const int&, QList<ezechiel::core::DrugVariate*>&) = 0;
    virtual ezechiel::core::Response deleteDrugVariate(ezechiel::core::DrugVariate*) = 0;
    virtual ezechiel::core::Response deleteDrugVariate(const int&) = 0;

    virtual ezechiel::core::Descriptor getDescriptor() = 0;
    virtual QString errorString() = 0;
};

Q_DECLARE_INTERFACE(AppUtilsRepository, "ch.heig-vd.ezechiel.AppUtilsInterface/0.1")

#endif // APPUTILSREPOSITORY_H
