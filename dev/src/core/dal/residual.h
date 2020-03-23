#ifndef RESIDUAL_H
#define RESIDUAL_H

#include <QObject>
#include <QVector>

namespace ezechiel {
namespace core {


//class Residual : public QObject
//{
//    Q_OBJECT
//public:
//    explicit Residual(QObject *parent = nullptr);

//signals:

//public slots:
//};

class Residual {

public:
    //! Create a residual with value
    explicit Residual(double value);
    //! Create a residual with default value 0.0
    Residual();
    //! The residual value
    double value;
};

//! \brief Define a vector of Residual
/** \ingroup utils
  The vector is automatically set with the needed size if trying to acces
  unavailable positions. New positions are set with the default residual value
  */
class Residuals {
public:
    Residuals();
    //! Create sized vector
    explicit Residuals(int size);
    //! Return the value at i, create new positions if needed.
    const Residual &at(int i);
    //! Provide acces to value at i, create new positions if needed.
    Residual &operator[](int i);
    //! Return the size of the vector
    int size() const;
    //! Clear all the values
    void clear();

private:
    QVector<Residual> residuals;
};

} // namespace core
} // namespace ezechiel


#endif // RESIDUAL_H
