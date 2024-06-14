/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "websockettransport.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#include <QtWebSockets/QWebSocket>

/**
    \brief QWebChannelAbstractSocket implementation that uses a QWebSocket internally.

    The transport delegates all messages received over the QWebSocket over its
    textMessageReceived signal. Analogously, all calls to sendTextMessage will
    be send over the QWebSocket to the remote client.
*/

/**
    Construct the transport object and wrap the given socket.

    The socket is also set as the parent of the transport object.
*/

using namespace Tucuxi::Gui::GuiUtils;

WebSocketTransport::WebSocketTransport(QWebSocket *socket)
: QWebChannelAbstractTransport(socket)
, m_socket(socket)
{
    connect(socket, &QWebSocket::textMessageReceived,
            this, &WebSocketTransport::textMessageReceived);
    connect(socket, &QWebSocket::disconnected,
            this, &WebSocketTransport::deleteLater);
}

/**
    Destroys the WebSocketTransport.
*/
WebSocketTransport::~WebSocketTransport()
{
    m_socket->deleteLater();
}

/**
    Serialize the JSON message and send it as a text message via the WebSocket to the client.
*/
void WebSocketTransport::sendMessage(const QJsonObject &message)
{
    QJsonDocument doc(message);
    m_socket->sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
}

/**
    Deserialize the stringified JSON messageData and emit messageReceived.
*/
void WebSocketTransport::textMessageReceived(const QString &messageData)
{
    QJsonParseError error;
    QJsonDocument message = QJsonDocument::fromJson(messageData.toUtf8(), &error);
    if (error.error) {
        qWarning() << "Failed to parse text message as JSON object:" << messageData
                   << "Error is:" << error.errorString();
        return;
    } else if (!message.isObject()) {
        qWarning() << "Received JSON message that is not an object: " << messageData;
        return;
    }
    emit messageReceived(message.object(), this);
}
