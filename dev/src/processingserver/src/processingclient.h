#ifndef PROCESSINGCLIENT_H
#define PROCESSINGCLIENT_H

#include <QObject>
#include <QTcpServer>

class ProcessingClient : public QObject
{
    Q_OBJECT
public:
    explicit ProcessingClient(QObject *parent = 0);
    virtual ~ProcessingClient();
    QTcpSocket* m_socket;

signals:
    void disconnected(ProcessingClient *client);

public slots:
    void looseConnection();

};

#endif // PROCESSINGCLIENT_H
