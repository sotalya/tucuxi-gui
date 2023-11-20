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

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class PartialRequestListModel;
class SortFilterEntityListProxyModel;

class RequestsController : public QObject
{
    Q_OBJECT

public:
    explicit RequestsController(QObject *parent = nullptr);
    virtual ~RequestsController() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual QAbstractItemModel *model();

    void setClient(Tucuxi::Gui::Admin::RequestsClient *client);

    int nbRequests() const;
    const Tucuxi::Gui::Admin::PartialRequest *getPartialRequest(int id);


private:
    Tucuxi::Gui::Admin::RequestsClient *client;

    PartialRequestListModel *_requestModel;
    SortFilterEntityListProxyModel *_proxyModel;

    void updateModel();

private slots:
    void processListReady(QList<Tucuxi::Gui::Admin::PartialRequest *> list);

public slots:

    virtual void queryList(QDateTime from = QDateTime::currentDateTime().addYears(-10),
                           QDateTime to = QDateTime::currentDateTime().addYears(10),
                           bool state = true);
    virtual void queryRequest(const QString &requestId, const QString &patientId, const QString &drugId);

    void queryRequestById(int id);

signals:
    void requestReady(Tucuxi::Gui::Admin::InterpretationRequest* ir);

    void queryRequestByIdSignal(int id);

};

}
}
}

#endif // REQUESTCONTROLLER_H

