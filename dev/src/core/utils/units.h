//@@license@@

#ifndef UNITS_H
#define UNITS_H

#include <QString>
#include <QChar>
#include <QMetaType>
#include <QStringList>
#include <QHash>

#include "tucucommon/unit.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



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

    /** \brief Returns the unit validity state.
     * @return True if the unit is valid.
     */
    bool isValid() const;

    //! Simple comparison.
    bool operator== (const Unit & other) const;

    //! Simple comparison.
    bool operator!= (const Unit & other) const;

    //! Returns the core TucuUnit object
    /**
     * It simply returns the TucuUnit object that can be used for translations,
     * for instante
     * @return The TucuUnit object representing the unit.
     */
    Tucuxi::Common::TucuUnit tucuUnit() const;

    //! Converts a value from a unit to another one
    /**
     * @param value Value to be converted
     * @param initialUnit Unit of the value to be converted
     * @param finalUnit Unit into which the return value shall be
     * @return The value in final unit
     *
     * The initialUnit and finalUnit shall be compatible.
     */
    static double convertToUnit(double value, const Unit &initialUnit, const Unit &finalUnit);

    //! Checks if two units are compatible
    /**
     * @param unit1 First unit
     * @param unit2 Second unit
     * @return true if the units are compatible, false else
     */
    static bool isCompatible(const Unit &unit1, const Unit &unit2);

private:

    Tucuxi::Common::TucuUnit _unit;

};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi


Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Unit)

#endif // UNITS_H
