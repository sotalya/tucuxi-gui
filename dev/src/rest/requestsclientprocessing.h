//@@license@@

#ifndef REQUESTSCLIENTPROCESSING_H
#define REQUESTSCLIENTPROCESSING_H

#include "admin/src/requestsclient.h"
#include <QDomDocument>

namespace Tucuxi {
namespace Gui {
namespace Rest {


class RequestsClientProcessing : public RequestsClient
{
public:
    RequestsClientProcessing(QObject *parent);

protected:

    int analyzeList(const QString &xmlList, QString &controlId);

    int analyzeRequest(const QString &xmlRequest);

};

class SimpleBuilder : QObject
{
public:

    InterpretationRequest* buildRequest(const QString &xmlRequest);

};


}
}
}

#endif // REQUESTSCLIENTPROCESSING_H
