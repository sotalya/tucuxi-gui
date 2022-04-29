//@@license@@

#ifndef REQUESTSCLIENTPROCESSING_H
#define REQUESTSCLIENTPROCESSING_H

#include "admin/src/requestsclient.h"
#include <QDomDocument>

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

#endif // REQUESTSCLIENTPROCESSING_H
