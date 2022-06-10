//@@license@@

#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QVariant>
#include <QMetaObject>
#include <QMetaProperty>
#include <QVector>

#include "core/interfaces/abstractrepository.h"

#include "factory.h"
#include "core/corefactory.h"
#include "core/utils/autoproperty.h"

#include "core/utils/logging.h"
#include "core/errors_core.h"

namespace Tucuxi {
namespace GuiCore {

typedef int Identifier;


class Entity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)

public:
    Entity(AbstractRepository *repository = nullptr, QObject *parent = nullptr);

    Entity(AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    virtual ~Entity() = 0;

    virtual int id() const {return _id;}
    virtual void setId(const int &id) {_id = id; emit idChanged(id);}

    virtual bool isValid();

    virtual bool save();
    bool isModified() const;

//    Q_INVOKABLE QObject* raw(QString spropname) const {
//        Entity* that = this->property(&spropname.toStdString()[0]).value<Entity*>();
//        return that.data();
//    }
//    bool save() {DB->setDBObject(*this);}

    void setRecursiveModified(bool modified = false);

protected slots:
    void setModified(bool modified = true);

signals:
    void idChanged(const int &id);
    void isValidChanged(const int &id);
    void hasBeenModified();

protected:
    int _id;
    int _isValid;
    bool _isModified;
    AbstractRepository *_repository;
    QVector<bool> _isValids;
};
typedef Entity* SharedEntity;
typedef QList<SharedEntity> SharedEntitySet;

} // namespace core
} // namespace Tucuxi

//Q_DECLARE_METATYPE(int)
Q_DECLARE_METATYPE(Tucuxi::GuiCore::Entity*)

#endif // ENTITY_H
