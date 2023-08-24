//@@license@@s

#ifndef FLATIMPORTREQUESTFILECLIENT_H
#define FLATIMPORTREQUESTFILECLIENT_H

#include "flatimportrequestsclientprocessing.h"

namespace Tucuxi {
namespace Gui {
namespace FlatRequest {

class FlatRequestFileClient : public Tucuxi::Gui::FlatRequest::FlatRequestsClientProcessing
{
    Q_OBJECT

    public:
    explicit FlatRequestFileClient(QObject *parent = nullptr);
    virtual ~FlatRequestFileClient() Q_DECL_OVERRIDE;

    void setListFile(const QString &fileName);

    public slots:
    virtual void queryList(QDateTime from = QDateTime::currentDateTime().addYears(-10),
                           QDateTime to = QDateTime::currentDateTime().addYears(10),
                           bool state = true) Q_DECL_OVERRIDE;
    virtual void queryRequest(const QString &requestId, const QString &patientId, const QString &drugId) Q_DECL_OVERRIDE;


private:

    QString m_listFileName;

};

}
}
}

#endif // FLATIMPORTREQUESTFILECLIENT_H
