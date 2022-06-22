//@@license@@s

#ifndef REQUESTCONTROLLER_H
#define REQUESTCONTROLLER_H

#include <QObject>
#include <QAbstractItemModel>
#include <QDateTime>

class RequestsClient;
class InterpretationRequest;
class PartialRequest;
class PartialRequestListModel;
class SortFilterEntityListProxyModel;

class RequestsController : public QObject
{
    Q_OBJECT

public:
    explicit RequestsController(QObject *parent = nullptr);
    virtual ~RequestsController() Q_DECL_OVERRIDE;

    virtual QAbstractItemModel *model() const;

    void setClient(RequestsClient *client);

private:
    RequestsClient *client;

    PartialRequestListModel *_requestModel;
    SortFilterEntityListProxyModel *_proxyModel;

private slots:
    void processListReady(QList<PartialRequest *> list);

public slots:

    virtual void queryList(QDateTime from = QDateTime::currentDateTime().addYears(-10),
                           QDateTime to = QDateTime::currentDateTime().addYears(10),
                           bool state = true);
    virtual void queryRequest(const QString &requestId, const QString &patientId, const QString &drugId);

signals:
    void requestReady(InterpretationRequest* ir);

};


#endif // REQUESTCONTROLLER_H

