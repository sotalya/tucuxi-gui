#ifndef AMOUNT_H
#define AMOUNT_H

#include "core/utils/units.h"

namespace ezechiel {
namespace core {


class Amount /*: public QObject*/
{

public:
//    explicit Amount(QObject *parent = nullptr);
    Amount();
    Amount(Unit unit);
    Amount(double value, Unit unit);
    Amount(const QString &str);

    int id() const {return _id;}
    void setId(int id) {_id = id;}

    //! Amount, following the unit.
    double value() const {return _value;}
    void setValue(double value) {_value = value;}

    //! Amount unit, this ID must correspond to the list in the database.
    const Unit unit() const {return _unit;}
    void setUnit(Unit unit) {_unit = unit;}

    //! Convert the amount unit.
    void convert(const Unit &unit);
    //! Returns true if the amount is valid.
    bool isValid();

    //! Convert using the unit multiplier in another unit and return the resulting value (do not change the internal value).
    double valueIn (const Unit &unit) const;

    //! Return the value with its amount.
    QString toString () const;

    //! Return the value with its amount, using the given precision
    QString toString(int precision);

    /** Fill this amount using this string.
      It can parse values of the format : '100mg' or '100[mg]', and ignore spaces.
      */
    bool fromString (const QString &str);

    //! Check the value AND unit, if the resulting number is the same but the unit is not, return false (1 [mg] != 1000 [ug])
    bool operator== ( const Amount & other ) const;
    bool operator!= ( const Amount & other ) const;

    //! Basic operations : convert the type @{
    Amount operator+ (const Amount &) const;
    void operator+= (const Amount &);
    QString operator+ (const QString &) const;
    Amount operator* (double) const;
    Amount operator* (const Amount &) const;
    Amount operator/ (double) const;
    Amount operator/ (const Amount &) const;
    Amount operator- (const Amount &) const;
    void operator-= (const Amount &);
    bool operator< (const Amount &) const;
    bool operator> (const Amount &) const;
    bool operator>= (const Amount &) const;
    bool operator<= (const Amount &) const;
    //! }@
private:
    int _id;
    double _value;

protected:
    Unit _unit;
};

} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::core::Amount)
Q_DECLARE_METATYPE(QList<ezechiel::core::Amount*>)
#endif // AMOUNT_H
