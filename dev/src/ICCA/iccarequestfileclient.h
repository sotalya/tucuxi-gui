//@@license@@s

#ifndef ICCAREQUESTFILECLIENT_H
#define ICCAREQUESTFILECLIENT_H

#include "iccarequestsclientprocessing.h"

namespace Tucuxi {
namespace Gui {
namespace ICCA {

class ICCARequestFileClient : public Tucuxi::Gui::ICCA::ICCARequestsClientProcessing
{
    Q_OBJECT

    public:
    explicit ICCARequestFileClient(QObject *parent = nullptr);
    virtual ~ICCARequestFileClient() Q_DECL_OVERRIDE;

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

#endif // ICCAREQUESTFILECLIENT_H