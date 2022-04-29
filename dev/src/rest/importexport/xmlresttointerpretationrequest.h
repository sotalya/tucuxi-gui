//@@license@@

#ifndef XMLTOINTERPRETATIONREQUEST_H
#define XMLTOINTERPRETATIONREQUEST_H

#include <QXmlStreamReader>

#include "admin/src/dal/interpretationrequest.h"


class XmlRestToInterpretationRequest
{
public:
    XmlRestToInterpretationRequest();

    InterpretationRequest *interpretationRequestFromXml(const QByteArray &data);

protected:
    QXmlStreamReader reader;
};

#endif // XMLTOINTERPRETATIONREQUEST_H
