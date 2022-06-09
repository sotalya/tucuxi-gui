//@@license@@

#ifndef CONVERT_H
#define CONVERT_H

#include "core/utils/duration.h"

#include <QCoreApplication>
#include <QString>
#include <QVariant>

// Time format
#define TIME_FORMAT "h'h'm"
// Short time format, only the hours
#define SHORT_TIME_FORMAT "h'h'"
// Date format
#define DATE_FORMAT "dd/mm/yyyy"

namespace ezechiel {
namespace GuiCore {


//! \brief Safe conversion between QString and QVariant
/** \ingroup utils
   Permit to retrieve a QString from a QVariant, even if the QVariant type is a custom one.
   Permit to retrieve a QVariant from a QString, even if the QVariant type is a custom one.
   This object use warnings, but no errors.
  */
class Convert
{
    Q_DECLARE_TR_FUNCTIONS(Convert)
public:
   Convert();

   //! \brief Convert a QVariant to a string
   /** For the QVariant::Type types, just call QVariant.toString.
      For the custom types, use the appropriate function.
      @param type The QVariant::Type, or a custom type
      @param value The QVariant to convert
      @return A string representing the QVariant, or an empty string if something went wrong
     */
   static QString toString (int type, QVariant value);

   //! \brief Convert a QVariant to a string
   /** The same as toString(int,QVariant), but the type is automatically deduced from the QVariant.
      @param value The QVariant to convert
      @return A string representing the QVariant, or an empty string if something went wrong
     */
   static QString toString (QVariant value);

   //! \brief Convert a string to a QVariant
   /** Use the specific given type.
      If something went wrong, return an empty QVariant.
      Internally, all the custom types are handled.
      @param type The QVariant::Type, or a custom type
      @param str String to retrieve data from
      @return A valid QVariant in the specified type, or an empty QVariant if something went wrong
     */
   static QVariant fromString (int type, QString str);

private:
   //Definition of the custom type ID
   int durationType;
};

} // namespace core
} // namespace ezechiel

#endif // CONVERT_H
