/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef VERSION_H
#define VERSION_H

#include <QString>

//Number of versions level
#define LEVELS 3

namespace Tucuxi {
namespace Gui {
namespace Core {



/** Handle the Tucuxi's version numbering.
 * \ingroup utils
 * Ezekiel versions are defined by a major, minor, and evelopment version.
 * For example, version 1.3.15, where 1 is the major, 3 is the minor and 15 is the development version.
 * When a version is invalid, it will return "0.0.0".
 */
class Version
{
public:
   //! Constructor (version at -1.0.0 by default, invalid).
   Version ();

   ~Version ();

   //! \brief String constructor
   /** To check if the string was parsed correctly, use the isValid() function.
     @param str String containing the version
     */
   Version (QString str);

   //! Reset the version value to -1.0.0 (invalid).
   void reset ();

   //------------ GETTERS --------------

   //! Get the major version.
   uint major ();

   //! Get the minor version.
   uint minor ();

   //! Get the development version.
   uint dev ();

   //! Retrieve the version in form of a string.
   /**
     @param showDev Show the development version if true, ommit it if false
     @return The version in the format : major.minor[.development]
     */
   QString toString (bool showDev = true) const;

   //! Is the version valid?
   bool isValid () const;

   //------------ SETTERS --------------

   //! \brief Set the version using a string
   /** When the parsing failed, the version value is resetted to -1.0.0
      @param str String containing the version
      @return True if the parsing worked, false otherwise
     */
   bool fromString (QString str);

   //! \brief Set the major version
   /** The minor and development versions are left untouched.
      @param v Major version to set
     */
   void setMajor (uint v);

   //! \brief Set the minor version
   /** The major and development versions are left untouched.
      @param v Minor version to set
     */
   void setMinor (uint v);

   //! \brief Set the development version
   /** The minor and major versions are left untouched.
      @param v Development version to set
     */
   void setDev (uint v);

   //------------ OPERATORS ------------

   bool operator< (Version) const;

   bool operator> (Version) const;

   bool operator== (Version) const;

   bool operator!= (Version) const;

private:
   //Versions (one for each level)
   int _versions[LEVELS];
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // VERSION_H
