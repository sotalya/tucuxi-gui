//@@license@@s

#ifndef REQUESTCONTROLLER_H
#define REQUESTCONTROLLER_H

#include <QObject>
#include <QAbstractItemModel>
#include <QDateTime>

namespace Tucuxi {
namespace Gui {
namespace Admin {
class RequestsClient;
class PartialRequest;
class InterpretationRequest;
}
}
}


class PartialRequestListModel;
class SortFilterEntityListProxyModel;

class RequestsController : public QObject
{
    Q_OBJECT

public:
    explicit RequestsController(QObject *parent = nullptr);
    virtual ~RequestsController() Q_DECL_OVERRIDE;

    virtual QAbstractItemModel *model() const;

    void setClient(Tucuxi::Gui::Admin::RequestsClient *client);

private:
    Tucuxi::Gui::Admin::RequestsClient *client;

    PartialRequestListModel *_requestModel;
    SortFilterEntityListProxyModel *_proxyModel;

private slots:
    void processListReady(QList<Tucuxi::Gui::Admin::PartialRequest *> list);

public slots:

    virtual void queryList(QDateTime from = QDateTime::currentDateTime().addYears(-10),
                           QDateTime to = QDateTime::currentDateTime().addYears(10),
                           bool state = true);
    virtual void queryRequest(const QString &requestId, const QString &patientId, const QString &drugId);

signals:
    void requestReady(Tucuxi::Gui::Admin::InterpretationRequest* ir);

};


#endif // REQUESTCONTROLLER_H

