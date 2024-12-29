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

