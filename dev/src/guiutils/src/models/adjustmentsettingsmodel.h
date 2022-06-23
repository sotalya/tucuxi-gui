//@@license@@

#ifndef ADJUSTMENTSETTINGSMODEL_H
#define ADJUSTMENTSETTINGSMODEL_H

#include "abstractentitymodel.h"

#include <QDateTime>

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class AdjustmentSettingsModel : public AbstractEntityModel
{
    Q_OBJECT

public:
    enum AdjustmentSettingsRoles {
        DateRole,
        ValidRole
    };
    Q_ENUM(AdjustmentSettingsRoles)

    explicit AdjustmentSettingsModel(QObject *parent = nullptr);
    explicit AdjustmentSettingsModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~AdjustmentSettingsModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool clearModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:
    void updateStatus();
    void dateChanged(QDateTime);

public slots:
    void setModelData(QDateTime);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    void init();

    QDateTime _date;
    bool _valid;
    QHash<int, QByteArray> _roleNames;
};

}
}
}

#endif // ADJUSTMENTSETTINGSMODEL_H
