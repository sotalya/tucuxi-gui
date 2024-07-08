//@@license@@

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
