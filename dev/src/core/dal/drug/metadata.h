//@@license@@

#ifndef METADATA_H
#define METADATA_H

#include "core/dal/entity.h"

namespace ezechiel {
namespace GuiCore {

class EditorList;
class ReferenceList;

class MetaData : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(MetaData)

    //first editor is always the creator
    AUTO_PROPERTY_DECL(EditorList*,editors,Editors)

    AUTO_PROPERTY_DECL(ReferenceList*,references,References)


    protected :
        Q_INVOKABLE MetaData(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent), _editors(0), _references(0) {}
    Q_INVOKABLE MetaData(AbstractRepository *repository, QObject * parent, EditorList *editors, ReferenceList *references)
        : Entity(repository, parent), _editors(editors), _references(references) {}


};


} //namespace core
} //namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::GuiCore::MetaData*)
Q_DECLARE_METATYPE(QList<ezechiel::GuiCore::MetaData*>)

#endif // METADATA_H
