//@@license@@

#ifndef COVARIATELISTMODEL_H
#define COVARIATELISTMODEL_H

#include "abstractentitylistmodel.h"

#include "core/dal/covariate.h"
#include "core/dal/drug/drugvariate.h"

class CovariateListModel : public AbstractEntityListModel
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

    explicit CovariateListModel(QObject *parent = nullptr);
    explicit CovariateListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~CovariateListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(Tucuxi::GuiCore::PatientVariateList *covariates);
    void setModelData(Tucuxi::GuiCore::DrugVariate* drugVariate);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual void insertRowsImpl(int at, int count, const Tucuxi::GuiCore::SharedEntitySet &entities = Tucuxi::GuiCore::SharedEntitySet());
    virtual Tucuxi::GuiCore::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    Tucuxi::GuiCore::PatientVariateList *_covariates;
    Tucuxi::GuiCore::DrugVariate* _drugVariate;
    QHash<int, QByteArray> _roleNames;
};

#endif // COVARIATELISTMODEL_H
