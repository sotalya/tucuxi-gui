//@@license@@

#ifndef FLATIMPORTREQUESTSCLIENTPROCESSING_H
#define FLATIMPORTREQUESTSCLIENTPROCESSING_H

#include "admin/src/requestsclient.h"
#include <QDomDocument>

namespace Tucuxi {
namespace Gui {
namespace FlatRequest {

class FlatRequestsClientProcessing : public Tucuxi::Gui::Admin::RequestsClient
{
public:
    FlatRequestsClientProcessing(QObject *parent);

protected:

    int analyzeList(const QString &xmlList, QString &controlId);

    int analyzeRequest(const QString &xmlRequest);

};

class SimpleBuilder : QObject
{
public:

    Tucuxi::Gui::Admin::InterpretationRequest* buildRequest(const QString &xmlRequest);

};


}
}
}

#endif // FLATIMPORTREQUESTSCLIENTPROCESSING_H
