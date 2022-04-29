//@@license@@

#ifndef TARGETLISTMODEL_H
#define TARGETLISTMODEL_H

#include "abstractentitylistmodel.h"

#include "core/dal/drug/target.h"
#include "core/dal/drug/drug.h"

class TargetListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum TargetRoles {
        TypeRole,
        CMinValueRole,
        CMinUnitRole,
        CMaxValueRole,
        CMaxUnitRole,
        CBestValueRole,
        CBestUnitRole,
        TMinValueRole,
        TMinUnitRole,
        TMaxValueRole,
        TMaxUnitRole,
        TBestValueRole,
        TBestUnitRole
    };
    Q_ENUM(TargetRoles)

    explicit TargetListModel(QObject *parent = nullptr);
    explicit TargetListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~TargetListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE void setDefaultUnit(QString unitname);

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(QList<ezechiel::core::Target*> *targets);
    void setModelData(ezechiel::core::DrugModel* drug);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual void insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities = ezechiel::core::SharedEntitySet());
    virtual ezechiel::core::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    QList<ezechiel::core::Target*> *_targets;
    ezechiel::core::DrugModel* _drug;
    ezechiel::core::Unit _defaultCUnit;
    QHash<int, QByteArray> _roleNames;
};

#endif // TARGETLISTMODEL_H
