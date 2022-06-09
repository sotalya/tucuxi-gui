//@@license@@

#ifndef UNITS_H
#define UNITS_H

#include <QString>
#include <QChar>
#include <QMetaType>
#include <QStringList>
#include <QHash>

namespace ezechiel {
namespace GuiCore {


//! Unique identifier for a unit
/** \ingroup utils
   This ID corresponds to the one used in the database.
  */
typedef int unit_id;

//! \def invalid_unit_id Code for an invalid unit ID
#define invalid_unit_id -1

//! Unit object
/** \ingroup utils
   The Unit object corresponds to the Units namespace, but with an identity.
   It heavily rellies on the Units namespace for it functionnalities
  */
class Unit
{

public:

    //! Create an invalid unit.
    Unit();

    //! Copy constructor.
    Unit(const Unit &other);

    /** \brief Create a unit from a string.
     * @param str String from which to create the unit.
     */
    Unit(QString str);

    /** \brief Sets the unit from a string.
     * @param str The unit as a string.
     */
    bool fromString(QString str);

    //! Convert the unit to string, surrounding it with brackets (eg. [kg]).
    /** If the unit is empty (but valid), an empty string is returned.
     * @return The unit as a string surrounded with brackets.
     */
    QString toString() const;

    /** \brief Convert the unit to string.
     * @return The unit as a string.
     */
    QString name() const;

    /** \brief Get the unit base name.
     * @return The unit base name.
     */
    QString base() const;

    /** \brief Retrieve the current multiplier.
     * @return The unit multiplier.
     */
    double multiplier() const;

    /** \brief Returns the unit validity state.
     * @return True if the unit is valid.
     */
    bool isValid() const;

    //! Retrieve the multiplier between this unit and another one, or -1.0 if it fails.
    /** The number returned is the multiplier that should should be applied to a number to convert it from this unit to the other.
     * @param other The unit to compare this unit with.
     * @return Multiplier between the two units, or -1.0 if it can't be converted.
     */
    double multiplier (const Unit &other) const;

    //! Simple comparison.
    bool operator== (const Unit & other) const;

    //! Simple comparison.
    bool operator!= (const Unit & other) const;

private:
    //ID of the unit base
    unit_id _base;

    //Multiplier from the base
    double _multiplier;
};


//! Units register
/** \ingroup utils
   This class contains some functions for the units.
   A unit beeing defined by its ID in the definition file, a name can be retrieved for it.
   In the same way, an ID can be found from a string defining the unit.
   The rest of the software do not have to be unit-aware, as the convertion between units is done here.
   The list of available units is dynamically created when a new unit is found from a string.
 */
class UnitsRegister
{

public:

    //! Initilise the multiplier and their value.
    UnitsRegister();

    //! Retrieve a unit's name from an ID.
    /** The string is searched in the database.
      @param unit Unit's id to search for
      @param multiplier The base unit's multiplier
      @return The unit's name, or an empty string if the unit ID is invalid
     */
    QString nameForUnit (unit_id unit, double multiplier);

    //! Retrieve a unit's ID from a name.
    /** The string can be given with or without the leading and trailing '[]'.
      If the base unit doesn't exists, it will be added.
      @param str The unit's name to search for
      @param base The unit's ID, or the invalid_unit_id if the given name do not corresponds to any of the database units
      @param multiplier The unit's multiplier from the base (1.0 if no multiplier)
      @return True if the string parsed correctly, false otherwise
    */
    bool unitForName(QString name, unit_id &base, double &multiplier);

private:
    //A base name for invalid units
    static const char *const _invalid_unit_name;

    //Stores the units base name, the index being their ID
    QStringList _baseNames;

    //QHash of <Multiplier letter, Multiplier value>
    QHash<QChar, double> _multipliers;
};

} // namespace core
} // namespace ezechiel


Q_DECLARE_METATYPE(ezechiel::GuiCore::Unit)

#endif // UNITS_H
