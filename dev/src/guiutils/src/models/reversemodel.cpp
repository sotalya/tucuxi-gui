//@@license@@

#include "reversemodel.h"

ReverseModel::ReverseModel(QObject *parent) :
    AbstractEntityModel(parent),
    _roleNames()
{
    init();
}

ReverseModel::ReverseModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityModel(undoStack, parent),
    _roleNames()
{
    init();
}

ReverseModel::~ReverseModel()
{

}

bool ReverseModel::clearModel()
{
    beginResetModel();
    _pred = 0;
    endResetModel();
    emit updateStatus();
    return false;
}

QHash<int, QByteArray> ReverseModel::roleNames() const
{
    return _roleNames;
}

QVariant ReverseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    if (!_pred) {
        return QVariant();
    }

    switch (role) {
    case Prediction2Role:
        return QVariant::fromValue(_pred);
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int ReverseModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

bool ReverseModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    Q_UNIMPLEMENTED();
    return false;
}

void ReverseModel::setModelData(ezechiel::core::PredictionResult* pred)
{
//    Q_ASSERT(data.contains("timeList"));
//    Q_ASSERT(data.contains("dataList"));

//    Q_ASSERT(data.value("timeList").canConvert<QVariantList>());
//    Q_ASSERT(data.value("dataList").canConvert<QVariantList>());

    beginResetModel();
    pred->setParent(this);
    _pred = pred;
    endResetModel();
    emit updateStatus();
}

void ReverseModel::init()
{
    _roleNames[Prediction2Role] = "pred";
}
