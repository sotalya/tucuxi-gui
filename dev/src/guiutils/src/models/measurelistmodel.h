#ifndef MEASURELISTMODEL_H
#define MEASURELISTMODEL_H

#include "abstractentitylistmodel.h"

#include "core/dal/drug/drug.h"

#include "admin/src/dal/measure.h"
#include "core/dal/coremeasure.h"

class MeasureListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum MeasureRoles {
        SampleIdRole,
        ValueRole,
        UnitRole,
        SampleDateRole,
        ArrivalDateRole
    };
    Q_ENUM(MeasureRoles)

    explicit MeasureListModel(QObject *parent = nullptr);
    explicit MeasureListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~MeasureListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE void setDefaultUnit(QString unitname);

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(QList<ezechiel::core::CoreMeasure *> *measures);
    void setModelData(ezechiel::core::DrugModel* drug);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual void insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities = ezechiel::core::SharedEntitySet());
    virtual ezechiel::core::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    QList<ezechiel::core::CoreMeasure *> *_measures;
    ezechiel::core::Unit _defaultUnit;
    ezechiel::core::DrugModel* _drug;
    QHash<int, QByteArray> _roleNames;
};

#endif // MEASURELISTMODEL_H
