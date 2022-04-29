//@@license@@

#ifndef USERACTION_H
#define USERACTION_H

#include "action.h"

#include <QCoreApplication>

namespace ezechiel {
namespace cliutils {
class UserAction : public Action
{
    Q_DECLARE_TR_FUNCTIONS(UserAction)

public:

    //! The action static getters.
    static QString name();
    QString help();

    //! Constructor.
    UserAction();

    //! The action run function.
    virtual bool run(const QStringList &args);

private:
    //The error messages
    static const char *const _INVALID_SUBCOMMAND;
    static const char *const _INVALID_ARGUMENTS;
    static const char *const _INVALID_ARGUMENT;
    static const char *const _TOO_FEW_ARGUMENTS;
    static const char *const _TOO_MANY_ARGUMENTS;
    static const char *const _USER_CREATION_FAILED;
    static const char *const _ADMINISTRATOR_RIGHTS;

    //The shared messages
    static const char *const _DATABASE_STATUS;

    //The action commands
    static const char *const _ACTION_COMMAND;
    static const char *const _CMD_CREATE_FIRST;
    static const char *const _CMD_CREATE;
    static const char *const _CMD_PASSWORD;
    static const char *const _CMD_ENCRYPTED;
    static const char *const _CMD_ENCRYPT;
    static const char *const _CMD_DECRYPT;

    //The commands parameters
    static const char *const _CMD_PARAM_USER;
    static const char *const _CMD_PARAM_ADMIN;

    //The subcommands functions
    bool createFirst(QStringList &args);
    bool createUser(QStringList &args);
    bool changePassword(QStringList &args, const QString &password);
    bool checkEncryption(QStringList &args);
    bool encryptDatabase(QStringList &args, bool encrypt);
};

} //namespace cliutils
} //namespace ezechiel
#endif // USERACTION_H
