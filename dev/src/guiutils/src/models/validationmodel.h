//@@license@@

#ifndef VALIDATIONMODEL_H
#define VALIDATIONMODEL_H

#include "abstractentitymodel.h"
#include "admin/src/dal/interpretationanalysis.h"

class ValidationModel: public AbstractEntityModel
{
    Q_OBJECT

public:
    enum ValidationRoles {
        ExpectednessRole,
        SuitabilityRole,
        PredictionRole,
        RemonitoringRole,
        WarningRole
    };
    Q_ENUM(ValidationRoles)

    explicit ValidationModel(QObject *parent = nullptr);
    explicit ValidationModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~ValidationModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(Tucuxi::Gui::Admin::InterpretationAnalysis *analysis);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    void init();

    Tucuxi::Gui::Admin::InterpretationAnalysis* _analysis;
    QHash<int, QByteArray> _roleNames;
};

#endif // DOSAGEMODEL_H
