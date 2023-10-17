//@@license@@

#include "sortfilterentitylistproxymodel.h"
#include <QAbstractTableModel>

#include <QDateTime>
#include <QQmlEngine>
#include <QRegularExpression>

using namespace Tucuxi::Gui::GuiUtils;

ProxyModelFilter::ProxyModelFilter(QObject *parent) :
    QObject(parent),
    _role(-1),
    _value(QRegularExpression()),
    _operation(Equal)
{

}

ProxyModelFilter::ProxyModelFilter(int role, QVariant value, ProxyModelFilter::Operation operation, QObject *parent) :
    QObject(parent),
    _role(role),
    _value(value),
    _operation(operation)
{

}

ProxyModelFilter::~ProxyModelFilter()
{

}

int ProxyModelFilter::role() const
{
    return _role;
}

void ProxyModelFilter::setRole(int role)
{
    if (_role == role)
        return;

    _role = role;
    emit roleChanged(role);
}
QVariant ProxyModelFilter::value() const
{
    return _value;
}

void ProxyModelFilter::setValue(const QVariant &value)
{
    if (_value == value)
        return;

    _value = value;
    emit valueChanged(value);
}

ProxyModelFilter::Operation ProxyModelFilter::operation() const
{
    return _operation;
}

void ProxyModelFilter::setOperation(Operation operation)
{
    if (_operation == operation)
        return;

    _operation = operation;
    emit operationChanged(operation);
}

//void ProxyModelFilter::setFixedString(const QString &pattern, Qt::CaseSensitivity cs)
//{
//    setValue(QRegExp(pattern, cs, QRegExp::FixedString));
//}

void ProxyModelFilter::setRegExp(const QString &pattern, Qt::CaseSensitivity cs)
{
    QRegularExpression reg;
    reg.setPattern(pattern);
    if (cs == Qt::CaseInsensitive)
        reg.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    setValue(reg);
}

//void ProxyModelFilter::setWildcard(const QString &pattern, Qt::CaseSensitivity cs)
//{
//    setValue(QRegExp(pattern, cs, QRegExp::Wildcard));
//}

bool ProxyModelFilter::accept(const QModelIndex &index) const
{
    if (!index.isValid())
        return true;
    if (!value().isValid())
        return true;

    QVariant data = index.data(role());

    if (value().type() == QMetaType::QRegularExpression)
        return data.toString().contains(value().toRegularExpression());
    if (value().type() == QVariant::DateTime && value().toDateTime().isValid())
        return compare(data.toDateTime(), value().toDateTime());
    if (value().type() == QVariant::Double || value().type() == QVariant::Int)
        return compare(data.toDouble(), value().toDouble());

    return true;
}

SortFilterEntityListProxyModel::SortFilterEntityListProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    _filters()
{
}

SortFilterEntityListProxyModel::~SortFilterEntityListProxyModel()
{

}

void SortFilterEntityListProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    if (sourceModel != nullptr){
        Q_ASSERT(dynamic_cast<QAbstractTableModel *>(sourceModel));
        QSortFilterProxyModel::setSourceModel(sourceModel);
    }
}

void SortFilterEntityListProxyModel::sort(int role, Qt::SortOrder order)
{
    QSortFilterProxyModel::setSortRole(role);
    QSortFilterProxyModel::sort(0, order);
}

bool SortFilterEntityListProxyModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return QSortFilterProxyModel::insertRows(row, count, parent);
}

bool SortFilterEntityListProxyModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return QSortFilterProxyModel::removeRows(row, count, parent);
}

bool SortFilterEntityListProxyModel::appendRows(int count, const QModelIndex &parent)
{
    return sourceModel()->insertRows(sourceModel()->rowCount(), count, parent);
}

bool SortFilterEntityListProxyModel::prependRows(int count, const QModelIndex &parent)
{
    return sourceModel()->insertRows(0, count, parent);
}

void SortFilterEntityListProxyModel::removeFilter(int role)
{
    QList<ProxyModelFilter *> removed = _filters.values(role);
    if (removed.isEmpty())
        return;

    _filters.remove(role);

    for (int i = 0; i < removed.count(); ++i) {
        disconnect(removed.at(i), SIGNAL(operationChanged(Operation)), this, SLOT(filterChanged()));
        disconnect(removed.at(i), SIGNAL(roleChanged(int)), this, SLOT(filterChanged()));
        disconnect(removed.at(i), SIGNAL(valueChanged(QVariant)), this, SLOT(filterChanged()));
    }

    invalidateFilter();
}

void SortFilterEntityListProxyModel::setFilter(ProxyModelFilter *filter)
{
    if (_filters.values(filter->role()).contains(filter))
        return;

    connect(filter, SIGNAL(operationChanged(Operation)), this, SLOT(filterChanged()));
    connect(filter, SIGNAL(roleChanged(int)), this, SLOT(filterChanged()));
    connect(filter, SIGNAL(valueChanged(QVariant)), this, SLOT(filterChanged()));

    _filters.insert(filter->role(), filter);
    invalidateFilter();
}

bool SortFilterEntityListProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    Q_ASSERT(!source_parent.isValid());
    Q_ASSERT(sourceModel()->columnCount() == 1);

    QModelIndex index = sourceModel()->index(source_row, 0);
    Q_ASSERT(index.isValid());

    QList<ProxyModelFilter *> filters = _filters.values();

    for (int i = 0; i < filters.count(); ++i)
        if (!filters.at(i)->accept(index))
            return false;

    return true;
}

void SortFilterEntityListProxyModel::filterChanged()
{
    invalidateFilter();
}
