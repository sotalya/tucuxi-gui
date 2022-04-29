//@@license@@

#ifndef CLIMANAGER_H
#define CLIMANAGER_H

//namespace ezechiel { namespace core {class DBAccount;}}
#include "core/core.h"

#include <QApplication>
#include <QCommandLineParser>

namespace ezechiel {
namespace cliutils {

enum CommandLineParseResult {
    CommandLineOk,
    CommandLineError,
    CommandLineHelpRequested
};

struct CliCommand
{
    CliCommand() {}
    CliCommand(QStringList args) {
        if (args.size() > 1) {
                actionName = args[0];
                args.pop_front();
                actionArgs = args;
        }
    }

    int verbocity;
    int color;
    QString logfile;
    QString language;
    QString dburl;
    QString actionName;
    QList<QString> actionArgs;
};

class CliManager : public QObject
{
    Q_OBJECT
public:
    explicit CliManager(QObject* parent = 0);
    ~CliManager();
    void initializeCore();
    void parseRestOptions();
    //added a (insecure but so is existing!) override to the requirement to login. This is
    //so that a submodule can run commands as if in a script (after running itself.
    //For an inpersistent datastore, in order to test, we need some hack to gain persistence
    //or run import + execute all in one cli process
    CommandLineParseResult parseCommandLine(QCommandLineParser &, CliCommand *);
public slots:
    int execute(QCoreApplication &app, int argc, char *argv[]);
    static bool selectAction(CliCommand* query, bool);
signals:
    void finished();
    void start();
private:
    QCoreApplication *m_app;
//    void connect(ezechiel::core::DBAccount &account);
    void setLanguage(CliCommand*);
};
}
}

#endif // CLIMANAGER_H
