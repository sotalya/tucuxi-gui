//@@license@@

#include "processingresponse.h"

#include <iostream>
#include "dataset.h"

using namespace ezechiel::GuiCore;

ProcessingResponse::ProcessingResponse()
{
}


QDataStream &operator>>(QDataStream &stream, Unit & unit)
{
    QString u;
    stream >> u;
    unit.fromString(u);
}

QDataStream &operator<<(QDataStream &stream, const Unit & unit)
{
    stream << unit.toString();
}

QDataStream &operator>>(QDataStream &stream, CycleData & cycle)
{
    uint size;
    Unit unit;
    CycleData::CycleType type;
    QDateTime startTime;
    QDateTime endTime;
    QDateTime peakTime;
    double peak;

    stream >> size;
    stream >> unit;
    quint32 t;
    stream >> t;
    type = static_cast<CycleData::CycleType>(t);
    stream >> startTime;
    stream >> endTime;
    stream >> peakTime;
    stream >> peak;
    cycle = CycleData(size, unit);
    cycle.setTimes(startTime, endTime, type);
    cycle.setPeak(peak, peakTime);
    for(int i=0;i<size;i++) {
        double value;
        stream >> value;
        cycle.set(i,value);
    }
}

QDataStream &operator<<(QDataStream &stream, const CycleData & cycle)
{
    stream << cycle.size();
    stream << cycle.unit();
    stream << static_cast<quint32>(cycle.type());
    stream << cycle.startTime();
    stream << cycle.endTime();
    stream << cycle.peakTime();
    stream << cycle.peak();
    for(int i=0;i<cycle.size();i++)
        stream << cycle.at(i);
}

QDataStream &operator>>(QDataStream &stream, DataSet & dataset)
{
    int nbCycles;
    stream >> nbCycles;
    for(int i=0;i<nbCycles;i++) {
        CycleData cycle;
        stream >> cycle;
        dataset.addCycleData(cycle);
    }
}


QDataStream &operator<<(QDataStream &stream, const DataSet & dataset)
{
    int size = dataset.nbCycles();
    QList<CycleData> cycles = dataset.allCycles();
    stream << size;
    for(int i=0;i<size;i++) {
        CycleData cycle = dataset.cycleData(i);
        stream << dataset.cycleData(i);
    }
}

QDataStream &operator>>(QDataStream &stream, ProcessingResponse & resp)
{
    stream >> resp.error;
    stream >> resp.fakeData;
//    std::cout << "Error: " << resp.error << std::endl;
//    std::cout << "Data: " << resp.fakeData << std::endl;

    quint32 size;
    stream >> size;
    resp.data.resize(size);
    for(int i=0;i<size;i++)
        stream >> resp.data[i];
    stream >> resp.dataset;
  //  stream >> resp.data;
}

QDataStream &operator<<(QDataStream &stream, const ProcessingResponse & resp)
{
    stream << resp.error;
    stream << resp.fakeData;
    // The copy of data could be optimized
    stream << static_cast<quint32>(resp.data.size());
    for(int i=0;i<resp.data.size();i++)
        stream << resp.data[i];
    stream << resp.dataset;
}



/*
 * typedef double cxn_type_t, cxn_type_I_t, cxn_type_IO_t, cxn_type_O_t;
typedef std::vector<cxn_type_t> comps_Concentration;
typedef std::vector<cxn_type_t> cxn_list_t, cxn_list_I_t, cxn_list_IO_t, cxn_list_O_t;
*/
