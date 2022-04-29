//@@license@@

#include "meldworkerthread.h"

#include <QFile>
#include <QProcess>
#include <QTemporaryDir>
#include <QTextStream>

MeldWorkerThread::MeldWorkerThread(const QString &received, const QString &expected, QObject *parent) :
    QThread(parent),
    _received(received),
    _expected(expected)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

MeldWorkerThread::~MeldWorkerThread()
{

}

void MeldWorkerThread::run()
{
    QTemporaryDir dir;
    if (!dir.isValid())
        qFatal(qPrintable(tr("An unexpected error occured while creating a temporary directory, aborting...")));

    QFile receivedOutput(dir.path() + "/" + "received.xml");
    QFile expectedOutput(dir.path() + "/" + "expected.xml");

    if (!receivedOutput.open(QIODevice::WriteOnly | QIODevice::Text))
        qFatal(qPrintable(tr("An unexpected error occured while opening file '%1', aborting...").arg(receivedOutput.fileName())));
    if (!expectedOutput.open(QIODevice::WriteOnly | QIODevice::Text))
        qFatal(qPrintable(tr("An unexpected error occured while opening file '%1', aborting...").arg(expectedOutput.fileName())));

    QTextStream receivedStream(&receivedOutput);
    receivedStream << _received;
    receivedStream.flush();

    QTextStream expectedStream(&expectedOutput);
    expectedStream << _expected;
    expectedStream.flush();

    QProcess process;
    process.start("meld", QStringList() << receivedOutput.fileName() << expectedOutput.fileName());
    process.waitForFinished(-1);
}
