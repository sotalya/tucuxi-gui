//@@license@@

#ifndef PROCESSINGREQUEST_H
#define PROCESSINGREQUEST_H

#include <vector>
#include <QDateTime>
#include <QDataStream>
#include "prediction.h"

class ProcessingRequest
{
public:
    ProcessingRequest();

    ezechiel::GuiCore::SharedDrugResponseAnalysis analysis;
};

QDataStream &operator>>(QDataStream &, ProcessingRequest &);
QDataStream &operator<<(QDataStream &, const ProcessingRequest &);

#endif // PROCESSINGREQUEST_H
