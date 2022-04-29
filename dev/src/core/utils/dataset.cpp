//@@license@@

#include "core/utils/dataset.h"

#include <QFile>
#include <QByteArray>
#include <QList>
#include <QVector>
#include <QDebug>

#include "core/core.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"

#include <iostream>

namespace ezechiel {
namespace core {


DataSet::DataSet () :
    _size(0)
{
}

DataSet::DataSet(Unit unit) : _unit(unit), _size(0), data(0), stride(0), times(0)
{
}

// Create the array, and set the unit and size
DataSet::DataSet (uint size, Unit unit) : _unit(unit), _size(0), data(0), stride(0), times(0)

{
}

DataSet::~DataSet()
{
}

//---------------------------- GETTERS ----------------------------

uint DataSet::size() const
{
    return _size;
}

Unit DataSet::unit() const
{
    return _unit;
}

//---------------------------- SETTERS ----------------------------

void DataSet::setTimes(const QDateTime startTime, const QDateTime endTime, const CycleData::CycleType type)
{
    _dataList[0].setTimes(startTime, endTime, type);
}

void DataSet::setUnit(const Unit &unit)
{
    //Set the data set unit
    _unit = unit;

    //Set each cycle unit
    foreach (CycleData cycle, _dataList)
        cycle.setUnit(unit);
}

//---------------------------- OPERATORS --------------------------

QVector<double> DataSet::getData() const {
   return data;
}

QVector<double> DataSet::getTimes() const {
   return times;
}

QVector<int> DataSet::getAbsTimes() const {
   return abstimes;
}

CycleData DataSet::cycleData(int pos) const
{
    return _dataList[pos];
}

CycleData &DataSet::cycleData(int pos)
{
    return _dataList[pos];
}

int DataSet::nbCycles() const
{
    return _dataList.size();
}

QVector<CycleData> DataSet::allCycles() const
{
    return _dataList;
}

void DataSet::append(CycleData& cycle)
{
    if (!cycle.startTime().isValid() || !cycle.endTime().isValid()) {
        LOG(QtFatalMsg, INVALIDCYCLE, "The cycle start and/or end time is invalid");
    }
    if (cycle.size() > 0) {
        data.push_back(cycle.data()[0]);
        stride.push_back(&data.back());
        for (int i = 1; i < cycle.size(); ++i) {
            data.push_back(cycle.data()[i]);
        }
    } else {
        double * _data = new double[cycle.size()];
        for (int i = 0; i < cycle.size(); ++i) {
            data.insert(data.end(), _data[i]);
        }
    }
    double time = _dataList.empty() ? 0 : _dataList.begin()->startTime().secsTo(cycle.startTime()) / 3600.0;
    double deltatime = cycle.startTime().secsTo(cycle.endTime()) / cycle.size() / 3600.0;
    for (int i = 0; i < cycle.size(); ++i) {
        times.push_back(time);
        QDateTime start = _dataList.empty() ? cycle.startTime() : _dataList[0].startTime();
        abstimes.push_back(start.addSecs(time * 3600.0).toMSecsSinceEpoch() / 1000);
        time += deltatime;
    }

    _dataList.push_back(cycle);
}

void DataSet::append(QVector<CycleData>& cycles)
{
    for (int i = 0; i < cycles.size(); ++i) {
        append(cycles[i]);
    }

}

DataSet& DataSet::operator =(const DataSet& dset)
{
    this->data.clear();
    this->times.clear();
    this->stride.clear();
    this->_dataList.clear();

    QVector<CycleData> cycles = dset.allCycles();
    this->append(cycles);
    return *this;
}

//---------------------------- PRIVATE --------------------------

CycleData::CycleData () :
    _data(0),
    _size(0),
    _startTime(QDateTime()),
    _endTime(QDateTime()),
    _peak(0.0),
    _peakTime(QDateTime()),
    _type(COMPLETE)
{
}

// Use the = operator
CycleData::CycleData (const CycleData& other) :
    _data(0),
    _startTime(QDateTime()),
    _endTime(QDateTime()),
    _peak(0.0),
    _peakTime(QDateTime()),
    _type(COMPLETE)
{
    *this = other;
}

// Create the array, and set the unit and size
CycleData::CycleData (uint size, Unit unit) :
    _size(size),
    _unit(unit),
    _startTime(QDateTime()),
    _endTime(QDateTime()),
    _peak(0.0),
    _peakTime(QDateTime()),
    _type(COMPLETE)
{
    _data = new double[size];
}

//If not shared, delete the data
CycleData::~CycleData()
{
}

//---------------------------- GETTERS ----------------------------
double * CycleData::data() const
{
    return _data;
}

uint CycleData::size() const
{
    return _size;
}

Unit CycleData::unit() const
{
    return _unit;
}

CycleData::CycleType CycleData::type() const
{
    return _type;
}

QDateTime CycleData::startTime() const
{
    return _startTime;
}

QDateTime CycleData::endTime() const
{
    return _endTime;
}

QDateTime CycleData::peakTime() const
{
    return _peakTime;
}

double CycleData::peak() const
{
    return _peak;
}

double CycleData::at(const uint pos) const
{
    Q_ASSERT_X(pos < _size, "DataSet.at()", "Index out of range");
    return _data[pos];
}

//---------------------------- SETTERS ----------------------------
//Copy the data, and set the value
void CycleData::set(const uint pos, const double value)
{
    Q_ASSERT_X(pos < _size, "DataSet.set()", "Index out of range");
//    detach();
    _data[pos] = value;
}

void CycleData::setTimes(const QDateTime startTime, const QDateTime endTime, const CycleType type)
{
    _startTime = startTime;
    _endTime   = endTime;
    _type = type;
}

void CycleData::setType(const CycleData::CycleType type)
{
    _type = type;
}

void CycleData::setPeak(double peak, QDateTime peakTime)
{
    _peak = peak;
    _peakTime = _peakTime;
}

//---------------------------- ACTIONS ----------------------------
//Use the utility in units (data will be copied when necessary)
void CycleData::convert(Unit unit)
{
    // ToDo /////////////////////////////////////////////////////////////
    // For the moment, the data is never converted to another unit. It
    // is considered to be matching the drug concentration default unit
    // at any given time in the code. Please remove this part of the code
    // if it is no longer necessary, or verify and improve it otherwise.
    // //////////////////////////////////////////////////////////////////

    Q_UNUSED(unit);
    Q_ASSERT(false);

//    //Find out the multiplier
//    double mul = _unit.multiplier(unit);
//
//    //Do the multiplication for the whole dataset
//    for (uint i=0; i<_size; i++)
//        _data[i] *= mul;
//
//    //Isn't this the simplest part?
//    _unit = unit;
}

//Read each line and convert it to a double
bool CycleData::read(const QString &file)
{
    //Open the file
    QFile f(file);
    if (!f.open(QFile::ReadOnly))
        return false;

    //Contains the data, we need the size to create a DataSet
    QList<double> data;

    //Read each line as a number
    while (!f.atEnd()) {
        bool ok;
        //Read and convert to double
        QByteArray line = f.readLine();
        data.append(line.remove(line.size()-1,1).toDouble(&ok));
        //On error, quit
        if (!ok)
            return false;
    }
    f.close();

    //Fill the data
    for (uint i=0; i<_size; i++)
        _data[i] = data.at(i);

    return true;
}

//Write each data in the file
bool CycleData::write(const QString &file) const
{
    //Open the file
    QFile f(file);
    if (!f.open(QFile::WriteOnly))
        return false;

    //Write each number in the file
    for (uint i=0; i<_size; i++)
        f.write(QString("%1\n").arg(_data[i]).toLatin1());
    f.close();

    return true;
}

//---------------------------- OPERATORS --------------------------

//Copy the data if shared, and return the modifiable value
double & CycleData::operator [](const uint &pos)
{
    Q_ASSERT_X(pos < _size, "DataSet[]", "Index out of range");
//    detach();
    return _data[pos];
}

//Increment the data user number
CycleData & CycleData::operator =(const CycleData &other)
{
    //Auto-assignement, just quit
    if (this == &other)
        return *this;

    //Copy data
    _size = other._size;
    _data = other._data;
    _unit = other._unit;
    _startTime = other._startTime;
    _endTime = other._endTime;
    _type = other._type;

    //Return ourselves
    return *this;
}

//Create a new one combining the two
CycleData CycleData::operator +(const CycleData &other)
{
    //Create the new
    CycleData result(_size + other._size, _unit);

    //Copy the other, and convert its unit if necessary
    CycleData converted(other);
    if (other._unit != _unit)
        converted.convert(_unit);

    //Copy the first set values
    memcpy(result._data,_data,sizeof(double)*_size);
    //Copy the second set of values
    memcpy(result._data+_size,converted._data,sizeof(double)*converted._size);

    return result;
}


//---------------------------- PRIVATE --------------------------

//void CycleData::detach()
//{
//    //Copy
//    double *copy = _data;
//    _data = new double[_size];
//    for (uint i=0; i<_size; i++)
//        _data[i] = copy[i];
//}

void CycleData::setUnit(const Unit &unit)
{
    _unit = unit;
}



} // namespace core
} // namespace ezechiel

