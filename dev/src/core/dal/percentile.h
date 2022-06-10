//@@license@@

#ifndef PERCENTILE_H
#define PERCENTILE_H

#include <QObject>
#include "core/dal/drug/parameters.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



//class Percentile : public QObject
//{
//    Q_OBJECT
//public:
//    explicit Percentile(QObject *parent = nullptr);

//signals:

//public slots:
//};

class Percentile
{

public:
    Percentile(int low, int high);

    int low() const;
    int high() const;

    void setLow(int value);
    void setHigh(int value);

private:
    //The percentile lower bound
    int _low;

    //The percentile higher bound
    int _high;
};

typedef Percentile* SharedPercentile;
/** A percentiles list of a given type.
 * \ingroup utils
 */
class PercentileList : public QList<Percentile>
{

public:
    PercentileList();
    PercentileList(const QList<Percentile> &other);
    PercentileList(const PercentileList &other);
    explicit PercentileList(ParameterType type);

    ParameterType type() const;
    void setType(ParameterType type);

private:
    //The percentiles type
    ParameterType _type;
};


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // PERCENTILE_H
