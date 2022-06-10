//@@license@@

#ifndef DATASET_H
#define DATASET_H

#include "core/utils/units.h"

#include <QDateTime>
#include <QVector>


namespace Tucuxi {
namespace Gui {
namespace Core {



/** Reunion of some data with a unit.
 * \ingroup utils
 * A DataSet contains a set of data in the form of an array of double along with an unit.
 * The data is implicitly shared, meaning that the data is not copied when the object is copied or assigned.
 * Instead, the reference on the shared data used.
 * The data is deleted when the last object using the shared data is removed.
 */
class CycleData {
public:

    //! The type of this cycle data.
    enum CycleType {
        START,          //! The cycle data is the first part of a cycle.
        END,            //! The cycle data is the last part of a cycle.
        INTERMEDIATE,   //! The cycle data is an intermediate part of a cycle.
        COMPLETE        //! The cycle data is a complete cycle.
    };

    CycleData ();
    CycleData (const CycleData& other);
    CycleData (uint size, Unit unit);
    ~CycleData ();
    double* data () const;
    uint size () const;
    Unit unit() const;
    CycleType type() const;
    QDateTime startTime() const;
    QDateTime endTime() const;
    QDateTime peakTime() const;
    double peak() const;
    double at (const uint pos) const;
    void set (const uint pos, const double value);
    void setTimes(const QDateTime startTime, const QDateTime endTime, const CycleType type = COMPLETE);
    void setType(const CycleType type);
    void setPeak(double peak, QDateTime peakTime);
    void setUnit(const Unit &unit);
    void convert (Unit unit);
    bool read (const QString &file);
    bool write (const QString &file) const;
    double& operator[] (const uint &pos);
    CycleData& operator= (const CycleData& other);
    CycleData operator+ (const CycleData& other);
private:
    double *_data;
    uint _size;
    Unit _unit;
    CycleType _type;
    QDateTime _startTime;
    QDateTime _endTime;
    QDateTime _peakTime;
    double _peak;
//    void detach ();
};


/** Reunion of some data with a unit.
 * \ingroup utils
 * A DataSet contains a set of data in the form of an array of double along with an unit.
 * The data is implicitly shared, meaning that the data is not copied when the object is copied or assigned.
 * Instead, the reference on the shared data used.
 * The data is deleted when the last object using the shared data is removed.
 */
class DataSet {
public:
    DataSet ();
    DataSet(Unit unit);
    DataSet (uint size, Unit unit);
    ~DataSet ();
    uint size () const;
    Unit unit() const;
    QVector<double> getData() const;
    QVector<double> getTimes() const;
    QVector<int> getAbsTimes() const;
    QVector<CycleData> allCycles() const;
    void append(CycleData&);
    void append(QVector<CycleData>&);
    double at (const uint pos) const;
    CycleData cycleData(int pos) const;
    CycleData &cycleData(int pos);
    int nbCycles() const;
    void set (const uint pos, const double value);
    void setTimes(const QDateTime startTime, const QDateTime endTime, const CycleData::CycleType type = CycleData::COMPLETE);
    void setUnit(const Unit &unit);
    void convert (Unit unit);
    bool read (const QString &file);
    bool write (const QString &file) const;
    double& operator[] (const uint &pos);
    DataSet& operator= (const DataSet& other);

private:
    //each datapoint can have any number of compartments, so we use an array (a row is cxns for all times)
    QVector<double> data;
    QVector<double> times;
    QVector<int> abstimes;
    //the strides are one for every cycle
    QVector<double*> stride;
    //data is contiguous in memory so we can use an array instead of list
    QVector<CycleData> _dataList;
    uint _size;
    Unit _unit;
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // DATASET_H
