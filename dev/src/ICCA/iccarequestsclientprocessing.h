//@@license@@

#ifndef ICCAREQUESTSCLIENTPROCESSING_H
#define ICCAREQUESTSCLIENTPROCESSING_H

#include "admin/src/requestsclient.h"
#include <QDomDocument>

namespace Tucuxi {
namespace Gui {
namespace ICCA {

class ICCARequestsClientProcessing : public Tucuxi::Gui::Admin::RequestsClient
{
public:
    ICCARequestsClientProcessing(QObject *parent);

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

#endif // ICCAREQUESTSCLIENTPROCESSING_H
