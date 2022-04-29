//@@license@@

#ifndef EDITORMODEL_H
#define EDITORMODEL_H

#include "abstractentitymodel.h"
#include "core/dal/drug/editors.h"

class EditorModel : public AbstractEntityModel
{
        Q_OBJECT

public:
    enum EditorRoles{
        NameRole,
        InstitutionRole,
        EmailRole,
        DateRole,
        CommentKeysRole,
        CommentValuesRole
    };
    Q_ENUM(EditorRoles)

    explicit EditorModel(QObject *parent = nullptr);
    explicit EditorModel(QUndoStack *undoStack,QObject *parent = nullptr);
    virtual ~EditorModel() Q_DECL_OVERRIDE;


    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

public slots:
    void setModelData(ezechiel::core::Editor *editor);
    ezechiel::core::Editor* getModelData();
private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    void init();

    ezechiel::core::Editor *_editor;
    QHash<int, QByteArray> _roleNames;



};

#endif // EDITORMODEL_H
