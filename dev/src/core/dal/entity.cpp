//@@license@@

#include "core/dal/entity.h"
//#include "core/corerepository.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



Entity::Entity(AbstractRepository *repository, QObject *parent) :
    QObject(parent),
    _id(-1),
    _isValid(false),
    _isModified(false),
    _repository(repository)
{
}

Entity::Entity(AbstractRepository *repository, const int &id, QObject *parent) :
    QObject(parent),
    _id(id),
    _isValid(false),
    _isModified(false),
    _repository(repository)
{
}

Entity::~Entity()
{

}

bool Entity::save() {
    // YTA : Remove it for now. Could be restored later, but I would prefer not to have an access through setDBObject.
    // I think a visitor pattern would better suite the needs
    /*
    Tucuxi::Gui::Core::Response r = NEWDB->setDBObject(*this, 0);
    if (r.error != NoError) {
        return false;
    }
    return true;
    */
    return false;
}

bool Entity::isModified() const
{
    if (_isModified)
        return true;
    foreach (QObject *obj,children()) {
        Entity *ent = dynamic_cast<Entity*>(obj);
        if (ent)
            if (ent->isModified())
                return true;
    }
    return false;
}

void Entity::setRecursiveModified(bool modified)
{
    _isModified = modified;
    foreach (QObject *obj,children()) {
        Entity *ent = dynamic_cast<Entity*>(obj);
        if (ent)
            ent->setRecursiveModified(modified);
    }
}

bool Entity::isValid()
{
    const QMetaObject* mobj = this->metaObject();
//    if (!isModified()) {
//        LOG(QtMsgType::QtWarningMsg, DATAMISSING, QString("Class %1 is modified, returning isValid value of %2.").arg(mobj->className()).arg(_isValid));
//            return _isValid;
//    }
    
    // This offset corresponds to:
    // 1 property of QObject
    // 2 properties of Entity
    int entityOffset = 3;
    
    // Create the vector if not yet done.
    // Could be done in the constructor instead
    if (_isValids.empty()) {
        _isValids.fill(false, mobj->propertyCount() - entityOffset);
    }

   
    int numprops = mobj->propertyCount();
    
    // Only iterate on the properties of the object, not on Entity nor QObject
    for (int i = entityOffset; i < numprops; ++i) {
        QMetaProperty prp = mobj->property(i);
        
        // If at least a property is not valid, then it's over
        if (prp.isValid() == false) {
            LOG(QtMsgType::QtWarningMsg, DATAMISSING, QString("Property %1 of class %2 is not set.").arg(i).arg(mobj->className()));
            return false;
        }
        QString pname = prp.name();
        
        // Get the QVariant corresponding to the type of the property.
        // Doing so imply that the property can be represented as a QVariant
        QVariant pval = this->property(prp.name());
        
        // Check if the property is something. A valid QVariant with data in it.
        // It implies that a pointer should not be null
        if (!pval.isValid() || pval.isNull() || pval.data() == 0) {
            LOG(QtMsgType::QtInfoMsg, OBJECTISINVALID, QString("Invalid, null, or inaccessible property: '%1 %2::%3").arg(prp.typeName(), mobj->className(), pname))
            _isValids[i-entityOffset] = false;
        } else {

            // If the property is an Entity, then apply isValid recursively
            if (pval.canConvert<Entity*>()) {
                Entity* ent = pval.value<Entity*>();
                if (!ent) {
                    _isValids[i-entityOffset] = false;
                } else {
                    _isValids[i-entityOffset] = ent->isValid();
                }
            } else {
                _isValids[i-entityOffset] = true;
            }
        }
    }
    
    // Iterate over the array to display log messages and 
    for (int i = entityOffset; i < numprops; ++i) {
        if (!_isValids[i-entityOffset]) {
            QString isvalidstr = "[ \n";
            for (int i = 0; i < _isValids.size(); ++i) {
               isvalidstr += QString("[%1 : %2, TYPE: %3]\n ")
                       .arg(mobj->property(i+entityOffset).name())
                       .arg(_isValids[i])
                       .arg(mobj->property(i+entityOffset).typeName());
            }
            isvalidstr += QString("]");
            LOG(QtMsgType::QtInfoMsg, OBJECTISINVALID,
                QString("Object of class %1 is invalid with properties: %2.")
                .arg(mobj->className())
                .arg(isvalidstr))
                    
            return false;
        }
    }

    return true;
}

void Entity::setModified(bool modified)
{
    _isModified = modified;
    if (modified) {
        emit hasBeenModified();
        if (parent()) {
            Entity *parentEntity = dynamic_cast<Entity*>(parent());
            if (parentEntity)
                parentEntity->setModified(true);
        }
    }
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
