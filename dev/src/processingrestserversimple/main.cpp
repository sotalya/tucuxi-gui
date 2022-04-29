//@@license@@

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QDebug>


#include "httplistener.h"
#include "processingrestserversimple/network/requestmapper.h"
#include "processingrestserversimple/utils/settings.h"


void parseOptions();

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("processing-rest-server");
    QCoreApplication::setApplicationVersion("1.0.0");

    parseOptions();

    QSettings *listenerSettings = new QSettings(QCoreApplication::applicationDirPath() + "/etc/webapp.ini",
                                                QSettings::IniFormat,
                                                &app);
    listenerSettings->beginGroup("listener");

    new HttpListener(listenerSettings, new RequestMapper(&app), &app);

    return app.exec();
}

void parseOptions()
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Processing REST Server for TUCUXI");

    const QCommandLineOption helpOption    = parser.addHelpOption();
    const QCommandLineOption versionOption = parser.addVersionOption();
    const QCommandLineOption sourceOption(QStringList() << "s" << "source-directory",
                                    QCoreApplication::translate("main", "The source directory from which the answers are retreived."),
                                    "source",
                                    QCoreApplication::applicationDirPath() + "/example/reply_request.xml");
    parser.addOption(sourceOption);

    parser.parse(QCoreApplication::arguments());
    if (parser.isSet(helpOption))
        parser.showHelp();
    if (parser.isSet(versionOption))
        parser.showVersion();

    const QString source = parser.value(sourceOption);
    Settings::getInstance().set("source", source);
    qDebug().nospace().noquote() << "Main: source directory='" << source << "'";
}
