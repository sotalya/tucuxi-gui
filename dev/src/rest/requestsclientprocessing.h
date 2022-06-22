//@@license@@

#ifndef REQUESTSCLIENTPROCESSING_H
#define REQUESTSCLIENTPROCESSING_H

#include "admin/src/requestsclient.h"
#include <QDomDocument>

namespace Tucuxi {
namespace Gui {
namespace Rest {

class RequestsClientProcessing : public Tucuxi::Gui::Admin::RequestsClient
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

    Tucuxi::Gui::Admin::InterpretationRequest* buildRequest(const QString &xmlRequest);

};


}
}
}

#endif // REQUESTSCLIENTPROCESSING_H
