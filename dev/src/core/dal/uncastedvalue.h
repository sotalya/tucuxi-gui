//@@license@@

#ifndef UNCASTEDVALUE_H
#define UNCASTEDVALUE_H


#include "core/dal/entity.h"


namespace ezechiel {
namespace core {

enum class UncastedStatus
{
    //! No way to cast the value correctly
    Uncasted = 0,
    //! The software could cast it, but it requires a human check
    PartiallyCasted,
    //! A human acknowledged the uncasted value
    Acknowledged,
    //! The value could actually be casted
    Casted
};

/*
 * This class is meant to embed information received from a server and that can not
 * be casted to a know variable. For instance if a dosage is given in a strange
 * manner and can not fill a Dosage object, then some fields could be displayed
 * in the GUI so that the user could manually set correct values.
 * A prediction could then be done only if every list of UncastedValue are empty.
 */
class UncastedValue : public ezechiel::core::Entity
{
    Q_OBJECT
    ENTITY_UTILS(UncastedValue)

    //! xml field that could not be casted
    AUTO_PROPERTY_DECL(QString, field, Field)

    //! text of the field
    AUTO_PROPERTY_DECL(QString, text, Text)

    //! Comment potentially added by the software when trying to cast
    AUTO_PROPERTY_DECL(QString, comment, Comment)

    //! Status of the uncast
    AUTO_PROPERTY_DECL(UncastedStatus, status, Status)

    AUTO_PROPERTY_DECL(bool, validated, Validated)


    Q_PROPERTY(bool ispartiallycasted READ isPartiallyCasted NOTIFY isPartiallyCastedChanged)


    Q_SIGNAL void isPartiallyCastedChanged(bool);

    bool isPartiallyCasted() { return _status == UncastedStatus::PartiallyCasted;}

protected:

    Q_INVOKABLE UncastedValue(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr);

public:
    bool isValid() Q_DECL_OVERRIDE;

};

QML_POINTERLIST_CLASS_DECL(UncastedValueList, UncastedValue)

} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::core::UncastedValue*)
Q_DECLARE_METATYPE(QList<ezechiel::core::UncastedValue*>)
Q_DECLARE_METATYPE(ezechiel::core::UncastedValueList*)

#endif // UNCASTEDVALUE_H
