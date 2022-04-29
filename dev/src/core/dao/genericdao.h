//@@license@@

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
