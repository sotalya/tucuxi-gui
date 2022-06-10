//@@license@@

#ifndef INSTITUTELISTMODEL_H
#define INSTITUTELISTMODEL_H

#include "abstractentitylistmodel.h"
#include "admin/src/dal/institute.h"

class InstituteListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum InstituteRoles {
        NameRole,
        ExternalIdRole,
        LogoRole,
        AddressRole,
        PostcodeRole,
        CityRole,
        StateRole,
        CountryRole
    };
    Q_ENUM(InstituteRoles)

    explicit InstituteListModel(QObject *parent = nullptr);
    explicit InstituteListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~InstituteListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(QList<SharedInstitute> *institutes);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual void insertRowsImpl(int at, int count, const Tucuxi::GuiCore::SharedEntitySet &entities = Tucuxi::GuiCore::SharedEntitySet());
    virtual Tucuxi::GuiCore::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    QList<SharedInstitute> *_institutes;
    QHash<int, QByteArray> _roleNames;
};

#endif // INSTITUTELISTMODEL_H
