//@@license@@

#include <QDateTime>
#include <QDataStream>

#include "tcpclient.h"
#include "tcpcommands.h"

TcpClient::TcpClient()
{
    socket = 0;
}

bool TcpClient::initialize()
{
    socket = new QTcpSocket(this);
    int timeout = 5000; // 5 seconds
    socket->connectToHost("localhost",DEFAULTPORT);
    if (socket->waitForConnected(timeout))
    {
        return true;
    }
    return false;
}

void TcpClient::close()
{
    socket->disconnectFromHost();
    socket->deleteLater();
}

void TcpClient::sendMeasure(const QString &firstName,
                            const QString &lastName,
                            const QString &medicId,
                            const QDateTime t,
                            qreal concentration)
{
    PREPAREBLOCK(COMMAND_NEWMEASURE);
    out << firstName;
    out << lastName;
    out << medicId;
    out << t;
    out << concentration;
    ENDBLOCK;
    SENDBLOCK(socket);
}

void TcpClient::sendDose(const QString &firstName,
                         const QString &lastName,
                         const QString &medicId,
                         const QDateTime t,
                         qreal dose,
                         qreal interval)
{
    PREPAREBLOCK(COMMAND_NEWDOSE);
    out << firstName;
    out << lastName;
    out << medicId;
    out << t;
    out << dose;
    out << interval;
    ENDBLOCK;
    SENDBLOCK(socket);
}
