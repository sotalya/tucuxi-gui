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


#ifndef GENERICDAO_H
#define GENERICDAO_H

#include <QVariant>
#include <QDateTime>
#include <QList>

template <typename T>
class GenericDAO
{

public:
    GenericDAO();
    virtual ~GenericDAO();

    virtual T *findById(int id) = 0;
    virtual T *findByValue(const QString &field, const QVariant &value) = 0;
    virtual QList<T *> findAll() = 0;
    virtual bool save(T *object) = 0;
    virtual bool remove(const T *object) = 0;

    QString error() const;

protected:
    void setError(const QString &error);

private:
    QString _error;
};

template <typename T>
GenericDAO<T>::GenericDAO() :
    _error()
{

}

template <typename T>
GenericDAO<T>::~GenericDAO()
{

}

template <typename T>
QString GenericDAO<T>::error() const
{
    return _error;
}

template <typename T>
void GenericDAO<T>::setError(const QString &error)
{
    _error = error;
}

#endif // GENERICDAO_H
