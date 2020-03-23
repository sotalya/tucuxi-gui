#ifndef DEMOREQUESTFILECLIENT_H
#define DEMOREQUESTFILECLIENT_H

#include "rest/requestsclientprocessing.h"

class DemoRequestFileClient : public RequestsClientProcessing
{
    Q_OBJECT

    public:
    explicit DemoRequestFileClient(QObject *parent = nullptr);
    virtual ~DemoRequestFileClient() Q_DECL_OVERRIDE;

    void setListFile(const QString &fileName);

    public slots:
    virtual void queryList(QDateTime from = QDateTime::currentDateTime().addYears(-10),
                           QDateTime to = QDateTime::currentDateTime().addYears(10),
                           bool state = true) Q_DECL_OVERRIDE;
    virtual void queryRequest(const QString &requestId, const QString &patientId, const QString &drugId) Q_DECL_OVERRIDE;


private:

    QString m_listFileName;

};


#endif // DEMOREQUESTFILECLIENT_H
