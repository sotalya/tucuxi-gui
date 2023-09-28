//@@license@@

#ifndef WEBSOCKETCLIENTWRAPPER_H
#define WEBSOCKETCLIENTWRAPPER_H

#include <QObject>
#include <QtWebSockets/QtWebSockets>
#include <QtWebEngineCore/QtWebEngineCoreVersion.h>
#include <QtWebChannel/QWebChannelAbstractTransport>
#include "websockettransport.h"

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class WebSocketClientWrapper : public QObject
{
    Q_OBJECT

public:
    WebSocketClientWrapper(QWebSocketServer *server, QObject *parent = nullptr);

Q_SIGNALS:
    void clientConnected(WebSocketTransport* client);

private Q_SLOTS:
    void handleNewConnection();

private:
    QWebSocketServer *m_server;
};

}
}
}

#endif // WEBSOCKETCLIENTWRAPPER_H
