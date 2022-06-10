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
    void setModelData( Tucuxi::Gui::Core::ValidIntervals *intervals);
    void setModelData(Tucuxi::Gui::Core::DrugModel* drug);

private:
//    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
//    virtual void insertRowsImpl(int at, int count, const Tucuxi::Gui::Core::SharedEntitySet &entities = Tucuxi::Gui::Core::SharedEntitySet());
//    virtual Tucuxi::Gui::Core::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    Tucuxi::Gui::Core::ValidIntervals *_intervals;

    QHash<int, QByteArray> _roleNames;
};

#endif // DOSAGELISTMODEL_H
