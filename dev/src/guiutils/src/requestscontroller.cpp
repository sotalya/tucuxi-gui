//@@license@@

#include "requestscontroller.h"


#include "core/utils/connect.h"
#include "admin/src/requestsclient.h"
#include "models/partialrequestlistmodel.h"
#include "models/sortfilterentitylistproxymodel.h"

using namespace Tucuxi::Gui::Admin;

RequestsController::RequestsController(QObject *parent) :
    QObject(parent),
    client(0),
    _requestModel(new PartialRequestListModel(this)),
    _proxyModel(new SortFilterEntityListProxyModel(this))
{
    _proxyModel->setSourceModel(_requestModel);
}

RequestsController::~RequestsController()
{

}

void RequestsController::setClient(RequestsClient *client)
{
    this->client = client;

//    CONNECT(client, SIGNAL(requestListReady(QList<PartialRequest*>)), this, SLOT(processListReady(QList<PartialRequest*>)));
    CONNECT(client, &RequestsClient::requestListReady, this, &RequestsController::processListReady);

    CONNECT(client, SIGNAL(requestReady(InterpretationRequest*)), this, SIGNAL(requestReady(InterpretationRequest*)));
}

void RequestsController::processListReady(QList<PartialRequest*> list)
{
    _requestModel->setModelData(list);
}

void RequestsController::queryList(QDateTime from,
                       QDateTime to,
                       bool state)
{
    Q_ASSERT(client);

    _requestModel->clearModel();

    client->queryList(from, to, state);
}


QAbstractItemModel *RequestsController::model() const
{
    return _proxyModel;
}

void RequestsController::queryRequest(const QString &requestId, const QString &patientId, const QString &drugId)
{
    Q_ASSERT(client);

    client->queryRequest(requestId, patientId, drugId);
}

