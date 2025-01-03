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


#include "requestscontroller.h"


#include "core/utils/connect.h"
#include "admin/src/requestsclient.h"
#include "models/partialrequestlistmodel.h"
#include "models/sortfilterentitylistproxymodel.h"

#include "core/dal/drug/drug.h"

using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::GuiUtils;

RequestsController::RequestsController(QObject *parent) :
    QObject(parent),
    client(0),
    //_requestModel(new PartialRequestListModel(this)),
    _requestModel(nullptr),
    _proxyModel(new SortFilterEntityListProxyModel(this))
{
    _requestModel = PartialRequestListModel::currentModel();
    _proxyModel->setSourceModel(_requestModel);
    CONNECT(this, &RequestsController::queryRequestByIdSignal, this, &RequestsController::queryRequestById);
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
    _requestModel = PartialRequestListModel::currentModel();
    _requestModel->setModelData(list);
}

void RequestsController::queryList(QDateTime from,
                       QDateTime to,
                       bool state)
{
    Q_ASSERT(client);

    _requestModel = PartialRequestListModel::currentModel();
    _requestModel->clearModel();

    client->queryList(from, to, state);
}


QAbstractItemModel *RequestsController::model()
{
    _requestModel = PartialRequestListModel::currentModel();
    _proxyModel->setSourceModel(_requestModel);
    return _proxyModel;
}

void RequestsController::queryRequest(const QString &requestId, const QString &patientId, const QString &drugId)
{
    Q_ASSERT(client);

    client->queryRequest(requestId, patientId, drugId);
}

const PartialRequest *RequestsController::getPartialRequest(int id)
{
    _requestModel = PartialRequestListModel::currentModel();
    return _requestModel->requests().at(id);
}

void RequestsController::queryRequestById(int id)
{
    auto request = _requestModel->requests().at(id);
    client->queryRequest(request->requestId(), request->patient()->externalId(), request->drug()->getSubstanceId());
}

int RequestsController::nbRequests() const
{
    return _requestModel->requests().size();
}
