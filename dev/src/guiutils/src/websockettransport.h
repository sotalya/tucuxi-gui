//@@license@@

#ifndef WEBSOCKETTRANSPORT_H
#define WEBSOCKETTRANSPORT_H

#include <QObject>
#include <QtWebSockets/QtWebSockets>
#include <QtWebChannel/QWebChannelAbstractTransport>

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class WebSocketTransport : public QWebChannelAbstractTransport
{
    Q_OBJECT
public:
    explicit WebSocketTransport(QWebSocket *socket);
    virtual ~WebSocketTransport();

    void sendMessage(const QJsonObject &message) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void textMessageReceived(const QString &message);

private:
    QWebSocket *m_socket;
};

}
}
}

#endif // WEBSOCKETTRANSPORT_H
