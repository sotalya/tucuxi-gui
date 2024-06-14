/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef RESIDUAL_H
#define RESIDUAL_H

#include <QObject>
#include <QVector>

namespace Tucuxi {
namespace Gui {
namespace Core {



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

} // namespace Core
} // namespace Gui
} // namespace Tucuxi


#endif // RESIDUAL_H
