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

#ifndef FLATREQUESTFILECLIENT_H
#define FLATREQUESTFILECLIENT_H

#include "flatrequestsclientprocessing.h"

namespace Tucuxi {
namespace Gui {
namespace FlatRequest {

class FlatRequestFileClient : public Tucuxi::Gui::FlatRequest::FlatRequestsClientProcessing
{
    Q_OBJECT

    public:
    explicit FlatRequestFileClient(QObject *parent = nullptr);
    ~FlatRequestFileClient() Q_DECL_OVERRIDE;

    void setListFile(const QString &fileName);

    public slots:
    void queryList(QDateTime from = QDateTime::currentDateTime().addYears(-10),
                           QDateTime to = QDateTime::currentDateTime().addYears(10),
                           bool state = true) Q_DECL_OVERRIDE;
    void queryRequest(const QString &requestId, const QString &patientId, const QString &drugId) Q_DECL_OVERRIDE;


private:

    QString m_listFileName;

    void constructFileFromDB();

};

}
}
}

#endif // FLATREQUESTFILECLIENT_H
