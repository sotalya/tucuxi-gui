//@@license@@

#ifndef STUDYLISTMODEL_H
#define STUDYLISTMODEL_H

#include "abstractentitylistmodel.h"

#include "core/utils/ezutils.h"

class StudyListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum StudyRoles {
        IdRole,
        NameRole,
        DescriptionRole
    };
    Q_ENUM(StudyRoles)

    explicit StudyListModel(QObject *parent = 0);
    explicit StudyListModel(QUndoStack *undoStack, QObject *parent = 0);
    virtual ~StudyListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

public slots:
    void setModelData(const QList<ezechiel::core::Descriptor> &studies);

private:
    void init();

    QList<ezechiel::core::Descriptor> _studies;
    QHash<int, QByteArray> _roleNames;
};

#endif // STUDYLISTMODEL_H
