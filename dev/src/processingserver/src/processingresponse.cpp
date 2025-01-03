/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "processingresponse.h"

#include <iostream>
#include "dataset.h"

using namespace Tucuxi::Gui::Core::Core;

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
