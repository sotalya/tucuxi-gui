//@@license@@

#ifndef DOSAGELISTMODEL_H
#define DOSAGELISTMODEL_H

#include "abstractentitylistmodel.h"

#include "core/dal/drug/doses.h"
#include "core/dal/drug/drug.h"

class IntervalListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum DoseRoles {
        IntervalDefaultRole,
        IntervalUnitRole,
        IntervalAnyRole,
        IntervalRole,
    };

    Q_ENUM(DoseRoles)

    explicit IntervalListModel(QObject *parent = nullptr);
    explicit IntervalListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~IntervalListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

public slots:
    void setModelData( ezechiel::GuiCore::ValidIntervals *intervals);
    void setModelData(ezechiel::GuiCore::DrugModel* drug);

private:
//    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
//    virtual void insertRowsImpl(int at, int count, const ezechiel::GuiCore::SharedEntitySet &entities = ezechiel::GuiCore::SharedEntitySet());
//    virtual ezechiel::GuiCore::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    ezechiel::GuiCore::ValidIntervals *_intervals;

    QHash<int, QByteArray> _roleNames;
};

#endif // DOSAGELISTMODEL_H
