//@@license@@

#ifndef PROCESSINGSERVER_H
#define PROCESSINGSERVER_H

#include <QObject>

#include "processingmath.h"
#include "processingclient.h"

class ProcessingServer : public QObject
{
    Q_OBJECT
public:
    explicit ProcessingServer(QObject *parent = 0);
    virtual ~ProcessingServer();

    void initialize();

    int readAndExecuteCommand(QTcpSocket *socket,QDataStream &stream,ProcessingClient *client);

    QTcpServer m_server;
    QList<ProcessingClient *> m_clients;
    ProcessingMath *m_math;

    int m_port;

    void getCommand(QDataStream &stream, ProcessingClient *client);
    void returnVersion(QDataStream &stream, ProcessingClient *client);

signals:

public slots:
    void Connection();
    virtual void dataReady();
    void looseConnection(ProcessingClient *client);

};

#endif // PROCESSINGSERVER_H
