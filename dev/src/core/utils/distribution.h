//@@license@@

#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <QList>
#include <QString>
#include <QObject>

namespace Tucuxi {
namespace Gui {
namespace Core {


class Descriptor;

/** Distribution type.
 * \ingroup utils
 * This class holds the different distribution types as well as their description.
 * A distribution is applied on the variability of a parameter, and is used mainly in PercentileEngine as well as in PostEngine.
 * No mathematical operations are done here.
 */
namespace distribution
{

   //! Enumeration of the possible distribution types.
   enum DistributionType {
      //! Uniform distribution
      Uniform,
      /** Log normal distribution
        This is the default
        */
      LogNormal,
      //! Normal distribution
      Normal,
      /** Invalid distribution.
        This value is used when parsing the XML or the command line, users of the distributions (such as PercentileEngine and PostEngine) should take this in account.
        */
      Invalid
   };

   //! \brief List of possible distributions
   /** List of textual values of the possible distributions.
      A description is given for each one.
     */
   QList<Descriptor> descriptions();

   //! \brief Retrieve a distribution type from a string
   /** If the given string does not corresponds to any of the available distribution type, the Invalid value is returned
     @param str String to analyse
     @return The corresponding DistributionType, or Invalid if not found
     */
   DistributionType fromString (QString str);

   //! \brief Return the string representation of this distribution type
   /** The value returned can be understanded by the fromString() function.
     @param type Distribution type of which the string is asked
     */
   QString toString (DistributionType type);

}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::distribution::DistributionType)

#endif // DISTRIBUTION_H
