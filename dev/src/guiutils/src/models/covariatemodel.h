//@@license@@

#ifndef COVARIATEMODEL_H
#define COVARIATEMODEL_H

#include "abstractentitymodel.h"
#include "core/dal/covariate.h"

class CovariateModel : public AbstractEntityModel
{
    Q_OBJECT

public:
    enum CovariateRoles {
        NameRole,
        DescriptionRole,
        ValueRole,
        UnitRole,
        TypeRole,
        DateRole
    };
    Q_ENUM(CovariateRoles)

    explicit CovariateModel(QObject *parent = nullptr);
    explicit CovariateModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~CovariateModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(Tucuxi::GuiCore::PatientVariate* covariate);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    void init();

    Tucuxi::GuiCore::PatientVariate* _covariate;
    QHash<int, QByteArray> _roleNames;
};

#endif // COVARIATEMODEL_H
