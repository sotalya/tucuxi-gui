//@@license@@

#ifndef EDITORLISTMODEL_H
#define EDITORLISTMODEL_H

#include "abstractentitylistmodel.h"
#include "core/dal/drug/editors.h"

class EditorListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum EditorRoles{
        NameRole,
        InstitutionRole,
        EmailRole,
        DateRole,
        CommentListRole
    };
    Q_ENUM(EditorRoles)

    explicit EditorListModel(QObject *parent = nullptr);
    explicit EditorListModel(QUndoStack *undoStack,QObject *parent = nullptr);
    virtual ~EditorListModel() Q_DECL_OVERRIDE;


    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

public slots:
    void setModelData(QList<ezechiel::core::Editor*> *editor);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual void insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities = ezechiel::core::SharedEntitySet());
    virtual ezechiel::core::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    QList<ezechiel::core::Editor*> *_editors;
    QHash<int, QByteArray> _roleNames;


};

#endif // EDITORLISTMODEL_H
