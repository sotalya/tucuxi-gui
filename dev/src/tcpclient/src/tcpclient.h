//@@license@@


#include <QTcpSocket>
#include <QObject>

class TcpClient : public QObject
{
    Q_OBJECT

public:
    TcpClient();
    bool initialize();
    void close();
    QTcpSocket *socket;

    void sendMeasure(const QString &firstName,
                     const QString &lastName,
                     const QString &medicId,
                     const QDateTime t,
                     qreal concentration);

    void sendDose(const QString &firstName,
                  const QString &lastName,
                  const QString &medicId,
                  const QDateTime t,
                  qreal dose,
                  qreal interval);
};
