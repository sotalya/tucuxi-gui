
#include <QApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QDebug>

#include "gui/mainwindow.h"
#include "utils/settings.h"

#include "rest/network/mirthrequest.h"
#include "rest/restlogger.h"

void parseOptions();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("rest-client");
    QCoreApplication::setApplicationVersion("1.0.0");

    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(rest);

    parseOptions();

    MainWindow w;
    w.show();

    return a.exec();
}

void parseOptions()
{
    QCommandLineParser parser;
    parser.setApplicationDescription("REST Test Client for EzeCHieL");

    const QCommandLineOption basePathOption(QStringList() << "b" << "base-path",
                                    QCoreApplication::translate("main", "The base path of the server."),
                                    "path",
                                    "/services");
    const QCommandLineOption disableMeldOption(QStringList() << "m" << "disable-meld",
                                    QCoreApplication::translate("main", "The host name of the server"));
    const QCommandLineOption portOption(QStringList() << "p" << "port",
                                    QCoreApplication::translate("main", "The port number of the server."),
                                    "port",
                                    QString::number(8889));
    const QCommandLineOption schemeOption(QStringList() << "s" << "scheme",
                                    QCoreApplication::translate("main", "The scheme of the server."),
                                    "scheme",
                                    "https");
    const QCommandLineOption hostOption(QStringList() << "t" << "host",
                                    QCoreApplication::translate("main", "The host name of the server"),
                                    "host",
                                    "ezecs1");
    const QCommandLineOption logRestOption(QStringList() << "l" << "logrest",
                                    QCoreApplication::translate("main", "Set the path in which the REST logs will be stored."),
                                    "logrest",
                                    QApplication::applicationDirPath());

    parser.addOption(basePathOption);
    parser.addOption(disableMeldOption);
    const QCommandLineOption helpOption = parser.addHelpOption();
    parser.addOption(hostOption);
    parser.addOption(portOption);
    parser.addOption(schemeOption);
    parser.addOption(logRestOption);
    const QCommandLineOption versionOption = parser.addVersionOption();

    parser.parse(QCoreApplication::arguments());
    if (parser.isSet(helpOption))
        parser.showHelp();
    if (parser.isSet(versionOption))
        parser.showVersion();

    MirthRequest::updateDefaultBasePath(parser.value(basePathOption));
    MirthRequest::updateDefaultPort(parser.value(portOption).toInt());
    MirthRequest::updateDefaultScheme(parser.value(schemeOption));
    MirthRequest::updateDefaultHost(parser.value(hostOption));
    RestLogger::getInstance()->setPath(parser.value(logRestOption));

    Settings::getInstance().set("disable-meld", parser.isSet(disableMeldOption));
}
