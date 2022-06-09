//@@license@@

#ifndef HALFLIFE_H
#define HALFLIFE_H

#include "core/utils/units.h"
#include "core/utils/ezutils.h"
#include "core/dal/entity.h"
#include "core/utils/duration.h"

namespace ezechiel {
namespace GuiCore {

class TranslatableString;

class Halflife : public Entity
{
    Q_OBJECT

    Q_PROPERTY(QString unitString READ getUnitString WRITE setUnitString NOTIFY unitStringChanged)


    ENTITY_UTILS(Halflife)

    AUTO_PROPERTY_DECL(Unit, unit, Unit)
    AUTO_PROPERTY_DECL(double, value, Value)
    AUTO_PROPERTY_DECL(int, multiplier, Multiplier)
//    AUTO_PROPERTY_DECL(CommentSet, commentSet, CommentSet)

    AUTO_PROPERTY_DECL(TranslatableString*, comments, Comments)

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

protected:

    Q_INVOKABLE explicit Halflife(AbstractRepository *repository, QObject *parent = nullptr)
    : Entity(repository, parent), _unit(), _value(-1.0), _multiplier(-1), _comments(0) {}
    Q_INVOKABLE Halflife(AbstractRepository *repository, QObject *parent, const Unit &unit, double value, int multiplier)
     : Entity(repository,parent), _unit(unit), _value(value), _multiplier(multiplier) {}

public:
    bool isValid() const {return _unit.isValid() && _value > 0.0 && _multiplier > 0;}

    Duration halfLife() const {
        // ToDo ///////////////////////////////////////////////////////////////////////////////////////
        // Convert the duration using the half-life's unit.
        // ////////////////////////////////////////////////////////////////////////////////////////////
        return Duration(_value);
    }

    QString getUnitString() {return _unit.name();}
    void setUnitString(QString name) { _unit.fromString("["+name+"]");}

signals:
    void unitStringChanged(QString);

};

} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::GuiCore::Halflife*)
//Q_DECLARE_OPAQUE_POINTER(ezechiel::GuiCore::Halflife*)
Q_DECLARE_METATYPE(QList<ezechiel::GuiCore::Halflife*>)

#endif // HALFLIFE_H
