//@@license@@

#ifndef PRACTICIANLISTMODEL_H
#define PRACTICIANLISTMODEL_H

#include "abstractentitylistmodel.h"
#include "admin/src/dal/practician.h"

class PracticianListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum PracticianRoles {
        ExternalIdRole,
        TitleRole,
        InstituteIdRole,
        FirstNameRole,
        LastNameRole,
        BirthDateRole,
        GenderRole,
        AddressRole,
        PostcodeRole,
        CityRole,
        StateRole,
        CountryRole
    };
    Q_ENUM(PracticianRoles)

    explicit PracticianListModel(QObject *parent = nullptr);
    explicit PracticianListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~PracticianListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(QList<SharedPractician> *practicians);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual void insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities = ezechiel::core::SharedEntitySet());
    virtual ezechiel::core::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    QList<SharedPractician> *_practicians;
    QHash<int, QByteArray> _roleNames;
};

#endif // PRACTICIANLISTMODEL_H
