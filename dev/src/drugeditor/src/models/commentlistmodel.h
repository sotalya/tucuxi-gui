//@@license@@

#ifndef COMMENTLISTMODEL_H
#define COMMENTLISTMODEL_H

#include "abstractentitylistmodel.h"
#include "core/utils/ezutils.h"
#include "core/dal/drug/translatablestring.h"



class CommentListModel : public AbstractEntityListModel
{

public:

    enum CommentRoles {
        LangRole,
        CommentRole
    };
    Q_ENUM(CommentRoles)

    explicit CommentListModel(QObject *parent = nullptr);
    explicit CommentListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~CommentListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;


public slots:
    void setModelData(Tucuxi::Gui::Core::TranslatableString *comments);


private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual void insertRowsImpl(int at, int count, const Tucuxi::Gui::Core::SharedEntitySet &entities = Tucuxi::Gui::Core::SharedEntitySet());
    virtual Tucuxi::Gui::Core::SharedEntitySet removeRowsImpl(int at, int count);

    void init();
    QString keyGen();

    Tucuxi::Gui::Core::TranslatableString *_comments;
    int keyGenAcc;
    QList<QString*> _keys;
    QHash<int,QByteArray> _roleNames;
};

#endif // COMMENTLISTMODEL_H
