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

#ifndef RESTREQUESTSCLIENT_H
#define RESTREQUESTSCLIENT_H


#include <QObject>
#include <QString>
#include <QDomDocument>

#include "requestsclientprocessing.h"
class QNetworkReply;

namespace Tucuxi {
namespace Gui {
namespace Rest {

class QAbstractItemModel;
class PartialRequestListModel;
class SortFilterEntityListProxyModel;
class MirthRequest;
class QueryListMessageBuilder;
class QueryRequestMessageBuilder;
class ReplyListMessageBuilder;
class ReplyRequestMessageBuilder;
class AckListMessageBuilder;
class AckRequestMessageBuilder;

class RestLogger;


class RestRequestsClient : public RequestsClientProcessing
{
    Q_OBJECT

public:
    explicit RestRequestsClient(QObject *parent = nullptr);
    virtual ~RestRequestsClient() Q_DECL_OVERRIDE;

public slots:
    void queryList(QDateTime from = QDateTime::currentDateTime().addYears(-10),
                   QDateTime to = QDateTime::currentDateTime().addYears(10),
                   bool state = true) Q_DECL_OVERRIDE;
    void queryRequest(const QString &requestId, const QString &patientId, const QString &drugId) Q_DECL_OVERRIDE;


private:
    QueryListMessageBuilder *_queryListBuilder;
    QueryRequestMessageBuilder *_queryRequestBuilder;
    ReplyListMessageBuilder *_replyListBuilder;
    ReplyRequestMessageBuilder *_replyRequestBuilder;
    AckListMessageBuilder *_ackListBuilder;
    AckRequestMessageBuilder *_ackRequestBuilder;

    QScopedPointer<MirthRequest> _queryList;
    QScopedPointer<MirthRequest> _queryRequest;
    QScopedPointer<MirthRequest> _ackList;
    QScopedPointer<MirthRequest> _ackRequest;

    RestLogger *_restLogger;

    void ackList(const QString &replyControlId, const QString &ackCode, const QString &ackMessage = QString());
    void ackRequest(const QString &replyControlId, const QString &ackCode, const QString &ackMessage = QString());

signals:
    void queryListFinished();
    void queryRequestFinished();
    void ackListFinished();
    void ackRequestFinished();

private slots:
    void processQueryList(QNetworkReply*);
    void processQueryRequest(QNetworkReply*);
    void processAckList(QNetworkReply*);
    void processAckRequest(QNetworkReply*);
};

}
}
}

#endif // RESTREQUESTSCLIENT_H
