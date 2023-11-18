//@@license@@

#ifndef FLATREQUESTSCLIENTPROCESSING_H
#define FLATREQUESTSCLIENTPROCESSING_H

#include "admin/src/requestsclient.h"
#include <QDomDocument>

namespace Tucuxi {
namespace Gui {
namespace FlatRequest {

class FlatRequestsClientProcessing : public Tucuxi::Gui::Admin::RequestsClient
{
    Q_OBJECT

public:
    FlatRequestsClientProcessing(QObject *parent);

protected:

    int analyzeList(const QString &xmlList, QString &controlId);

    int analyzeRequest(const QString &xmlRequest);

};

class SimpleBuilder : QObject
{
    Q_OBJECT

public:

    Tucuxi::Gui::Admin::InterpretationRequest* buildRequest(const QString &xmlRequest);

};


}
}
}

#endif // FLATREQUESTSCLIENTPROCESSING_H
