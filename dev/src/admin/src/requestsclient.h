/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
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


#ifndef REQUESTSCLIENT_H
#define REQUESTSCLIENT_H

#include <QObject>
#include <QDateTime>

namespace Tucuxi {
namespace Gui {
namespace Admin {

class InterpretationRequest;
class PartialRequest;

/**
 * @brief The RequestsClient class
 * This abstract class exposes the methods and signals required to get data
 * about pending requests. It shall be subclassed.
 * A DemoClient could generated fake patients, while a REST client could
 * connect to a REST server (Mirth, for instance).
 * Asking for a list or for a request is done via a call to a method,
 * and signals are used to send back the data. This way, the methods are
 * non-blocking and fit well with a client-server implementation.
 */
class RequestsClient : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief RequestsClient Empty constructor
     * @param parent The parent object
     */
    explicit RequestsClient(QObject *parent = nullptr);

    /**
     * @brief ~RequestsClient Empty destructor
     */
    virtual ~RequestsClient() Q_DECL_OVERRIDE;

public slots:

    /**
     * @brief queryList Asks for a list of queries
     * @param from From this date
     * @param to Up to this date
     * @param state State of the requests ("true": open, "false": closed)
     * This function shall be called whenever a list of pending requests is
     * required. Subclasses should implement this function.
     * Typically, for a REST connection, this function should connect
     * to the server and send a packet with the required information.
     * This function is non-blocking, the list being retrieved through the
     * emission of the requestListReady() signal.
     */
    virtual void queryList(QDateTime from = QDateTime::currentDateTime().addYears(-10),
                           QDateTime to = QDateTime::currentDateTime().addYears(10),
                           bool state = true) = 0;

    /**
     * @brief queryRequest Asks for a pending request
     * @param requestId Request ID, corresponding to an existing request
     * @param patientId Patient ID, for double checking the request ID
     * @param drugId Drug ID, for double checking the request ID
     * This function shall be called whenever a pending request is
     * required. Subclasses should implement this function.
     * Typically, for a REST connection, this function should connect
     * to the server and send a packet with the required information.
     * This function is non-blocking, the request being retrieved through the
     * emission of the requestReady() signal.
     */
    virtual void queryRequest(const QString &requestId, const QString &patientId, const QString &drugId) = 0;

signals:

    /**
     * @brief requestReady Signal emitted when a request has been received
     * @param ir An interpretation request object containing all data about the request
     * This signal shall be emitted when a request is retrieved by the client and
     * another object should intercept the signal. Typically in the GUI when the user
     * clicks on a pending request in the list, the server is asked to supply all the
     * requests details. When everything has been received, the signal is emitted.
     */
    void requestReady(InterpretationRequest* ir);

    /**
     * @brief requestListReady Signal emitted when the pending requests list is ready
     * @param requests The list of partial requests available
     * This signal shall be emitted when a pending request list is retrieved by the client
     * and another object should intercept the signal. In the GUI a controller shall
     * wait for this signal to then update the model used by the requests list view.
     */
    void requestListReady(QList<PartialRequest*> requests);

};

}
}
}

#endif // REQUESTSCLIENT_H
