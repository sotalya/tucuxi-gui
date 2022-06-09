//@@license@@

#ifndef REFERENCES_H
#define REFERENCES_H

#include "core/dal/entity.h"

namespace ezechiel {
namespace GuiCore {

class Reference : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Reference)


    AUTO_PROPERTY_DECL(QString, text, Text)
    AUTO_PROPERTY_DECL(QString, type, Type)


    protected :
        Q_INVOKABLE Reference(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) {}
        Q_INVOKABLE Reference(AbstractRepository *repository, QObject * parent, const QString &text, const QString &type)
            : Entity(repository, parent), _text(text), _type(type){}

};

POINTERLIST_CLASS_DECL(ReferenceList, Reference)

} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::GuiCore::Reference*)
Q_DECLARE_METATYPE(QList<ezechiel::GuiCore::Reference*>)
Q_DECLARE_METATYPE(ezechiel::GuiCore::ReferenceList*)

#endif // REFERENCES_H
