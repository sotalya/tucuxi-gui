//@@license@@

#ifndef PROCESSINGRESPONSE_H
#define PROCESSINGRESPONSE_H

#include <QDataStream>
#include "ezmathtypes.h"
#include "dataset.h"

class ProcessingResponse
{
public:
    ProcessingResponse();
    // All info about any kind of response
    int error;
    int fakeData;
    //DataSet data[];


    ezechiel::math::cxn_list_t data;
    ezechiel::core::DataSet dataset;

};

QDataStream &operator>>(QDataStream &, ProcessingResponse & );
QDataStream &operator<<(QDataStream &, const ProcessingResponse & );


#endif // PROCESSINGRESPONSE_H
