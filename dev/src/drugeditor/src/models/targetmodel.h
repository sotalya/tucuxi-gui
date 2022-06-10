//@@license@@

#ifndef TARGETMODEL_H
#define TARGETMODEL_H

#include "abstractentitymodel.h"
#include "core/dal/drug/target.h"

class TargetModel : public AbstractEntityModel
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

    explicit TargetModel(QObject *parent = 0);
    explicit TargetModel(QUndoStack *undoStack, QObject *parent = 0);
    virtual ~TargetModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(Tucuxi::Gui::Core::Target* target);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    void init();

    Tucuxi::Gui::Core::Target* _target;
    QHash<int, QByteArray> _roleNames;
};

#endif // TARGETMODEL_H
