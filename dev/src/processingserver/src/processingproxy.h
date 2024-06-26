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


#ifndef PROCESSINGPROXY_H
#define PROCESSINGPROXY_H

#include "processinginterface.h"
#include <QHostAddress>
#include <QTimer>
#include <QTcpSocket>
#include "prediction.h"
#include "processingresponse.h"
#include "processingrequest.h"

using namespace Tucuxi::Gui::Core::Core;

class ProcessingProxy : public QObject, public Tucuxi::Gui::Core::ProcessingInterface
{
    Q_OBJECT
public:
    ProcessingProxy();
    ~ProcessingProxy();

    //! Enum type describing the state of the connection with the server
    enum STATE{
        INIT,              //!< Initial state
        WAITFORCONNECTION, //!< Waiting for the connection to be established
        VERSIONERROR,      //!< The version of the server does not match the one of the client
        WAITFORDATA,       //!< Waiting for data to be sent by the server
        WAITFORCOMMAND     //!< Connection ready, a command can be sent to the server
    };

    virtual int points(const SharedDrugResponseAnalysis analysis, const PointsTraits traits, DataSet &data);

    virtual int point(const SharedDrugResponseAnalysis analysis, const PointTraits traits, Amount &amount);

    virtual int peak(const SharedDrugResponseAnalysis analysis, const PeakTraits traits, Amount &amount);

    virtual int percentiles(const SharedDrugResponseAnalysis analysis, const PercentilesTraits traits, QMap<double,DataSet> &concentrations);

//    virtual int point(const SharedPrediction prediction, const QDateTime &moment, Amount &amount);

//    virtual int peak(const SharedPrediction _pred, QDateTime &peakTime, Amount &amount);
    
    bool checkServerVersion();


    int connectToHost(QHostAddress address,int port=0);
    int connectToHostBlocked(QHostAddress address,int timeout,int port=0);

protected:

    bool process(const ProcessingRequest &request, ProcessingResponse &response);

    //! Sends a block to the server
    int sendBlock(const QByteArray &array);

    //! A timer to detect the impossibility to connect to the server
    QTimer *m_timer;
    //! The port on which the server is waiting connections
    /*! By default, it refers to sm_port, but it can be set by the function
        connectToHost().
    */
    int m_port;


    int m_responseTimeout;

    //! Current state of the proxy
    /*! It is used to know if a command can be sent, or if data should be
        retrieved from the server, for instance.
    */
    STATE m_state;

    //! IP address of the host server
    QHostAddress m_address;
    //! Socket open to exchange data with the server
    QTcpSocket *m_socket;

    void printMessage(QString message);

    int sendCommand(int command);
    int disconnectAll();

    int getResponse(ProcessingResponse &response);

signals:
    //! The connection did not succeed (timed out)
    void connectTimeout();
    //! The version of the server is not the one expected. QString to be changed to a custom type
    void versionError(QString expected,QString current);
    //! The server has been disconnected
    void disconnected();
    //! The connection is established
    void newConnection();

public slots:
    void found();
    void connected();
    void Connection();
    void looseConnection();
    void dataReady();
    void connectionTimeout();

};

#endif // PROCESSINGPROXY_H
