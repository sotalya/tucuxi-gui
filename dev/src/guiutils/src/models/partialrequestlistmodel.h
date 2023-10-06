//@@license@@

#ifndef PARTIALREQUESTLISTMODEL_H
#define PARTIALREQUESTLISTMODEL_H

#include <qqml.h>
#include <QAbstractTableModel>
#include "admin/src/dal/partialrequest.h"

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class PartialRequestListModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum PartialRequestRoles {
        RequestIdRole = Qt::UserRole +1,
        RequestCpclRole,
        PatientIdRole,
        FirstNameRole,
        LastNameRole,
        InstituteRole,
        ValueRole,
        UnitRole,
        DateRole,
        DrugRole,
        DrugIdRole,
        ColorRole,
        AlarmRole
    };

    Q_ENUM(PartialRequestRoles)

    explicit PartialRequestListModel(QObject *parent = nullptr);
    virtual ~PartialRequestListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool clearModel();

    Q_INVOKABLE virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    // For GUI tests
    const QList<SharedPartialRequest>& requests() {return _requests;}

    static PartialRequestListModel* currentModel();


signals:
    void updateStatus();

public slots:
    void setModelData(const QList<SharedPartialRequest> &requests);

private:
    void init();

    QList<SharedPartialRequest> _requests;
    QHash<int, QByteArray> _roleNames;
};

}
}
}

#endif // PARTIALREQUESTLISTMODEL_H
