//@@license@@

#ifndef REVERSEMODEL_H
#define REVERSEMODEL_H

#include "abstractentitymodel.h"
#include "QDateTime"
#include "core/dal/predictionresult.h"

class ReverseModel : public AbstractEntityModel
{
    Q_OBJECT

public:
    enum ReverseRoles {
        Prediction2Role
    };
    Q_ENUM(ReverseRoles)

    explicit ReverseModel(QObject *parent = nullptr);
    explicit ReverseModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~ReverseModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool clearModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(Tucuxi::GuiCore::PredictionResult*);

private:
    void init();

    Tucuxi::GuiCore::PredictionResult* _pred;
    QHash<int, QByteArray> _roleNames;
};

#endif // REVERSEMODEL_H
