//@@license@@

#ifndef DRUGLISTMODEL_H
#define DRUGLISTMODEL_H

#include "abstractentitylistmodel.h"

#include "core/dal/drug/drug.h"

class DrugListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum DrugRoles {
        NameRole,
        DescriptionRole,
        StudynameRole,
        AtcRole,
        DomainnameRole,
        IndicationRole,
        PopulationRole
    };
    Q_ENUM(DrugRoles)

    explicit DrugListModel(QObject *parent = nullptr);
    explicit DrugListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~DrugListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(QList<ezechiel::GuiCore::DrugModel*> *drugs);

private:
    void init();

    QList<ezechiel::GuiCore::DrugModel*> *_drugs;
    QHash<int, QByteArray> _roleNames;
};

#endif // DRUGLISTMODEL_H
