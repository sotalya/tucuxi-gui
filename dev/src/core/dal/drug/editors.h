#ifndef EDITOR_H
#define EDITOR_H

#include "core/dal/entity.h"
#include "core/utils/ezutils.h"

#include "core/dal/drug/translatablestring.h"

namespace ezechiel {
namespace core {

class Editor : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Editor)

    AUTO_PROPERTY_DECL(QString,name,Name)
    AUTO_PROPERTY_DECL(QString, institution,Institution)
    AUTO_PROPERTY_DECL(QString, email, Email)
    AUTO_PROPERTY_DECL(QDateTime,date,Date)

    AUTO_PROPERTY_DECL(TranslatableString*, comments,Comments)


    protected :

    Q_INVOKABLE Editor(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent), _comments(nullptr) {}
    Q_INVOKABLE Editor(AbstractRepository *repository, QObject * parent, const QString &name, const QString &institution, const QString &email, const QDateTime &date,TranslatableString *comments)
        : Entity(repository, parent), _name(name), _institution(institution), _email(email), _date(date), _comments(comments) {}



};
/*
class EditorSet : public QList<Editor*>
{

    ENTITY_UTILS(EditorSet)

    EditorSet(AbstractRepository *repository,QObject* parent = nullptr){ }

    virtual ~EditorSet();



};
*/
QML_POINTERLIST_CLASS_DECL(EditorList,Editor)
} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::core::Editor*)
Q_DECLARE_METATYPE(QList<ezechiel::core::Editor*>)
// Q_DECLARE_METATYPE(ezechiel::core::EditorSet*)
Q_DECLARE_METATYPE(ezechiel::core::EditorList*)

#endif // EDITOR_H
