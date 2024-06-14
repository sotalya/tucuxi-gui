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


#include "core/utils/version.h"

#include <QRegularExpression>
#include <QStringList>

namespace Tucuxi {
namespace Gui {
namespace Core {



//Reset the value
Version::Version()
{
   this->reset();
}

Version::~Version ()
{

}

//Use the fromString() function
Version::Version (QString str) {
   this->fromString(str);
}

//Defaults to -1.0.0
void Version::reset()
{
   _versions[0] = -1;
   for (int i=1; i<LEVELS; i++)
      _versions[i] = 0;
}

//------------ SETTERS --------------

//Parse using a regex, if it fails, reset the value
bool Version::fromString(QString str)
{
   //Reset the version by default
   this->reset();

   //Remove spaces
   QString s = str.simplified();

   //Create the regex
   QRegularExpression reg ("^([0-9]+)\\.([0-9]+)\\.?([0-9]*)$");

   //Check the string
   if ( ! reg.match(s).hasMatch())
      return false;

   //Retrieve the numbers
   QRegularExpressionMatch match = reg.match(s);
   int cnt = -1; //Avoid the first string (entire match)
   foreach(QString value, match.capturedTexts()) {
      if (cnt >= 0)
         _versions[cnt] = value.toUInt();
      cnt++;
   }

   //All green
   return true;
}

void Version::setMajor(uint v)
{
   _versions[0] = v;
}

void Version::setMinor(uint v)
{
   _versions[1] = v;
}

void Version::setDev(uint v)
{
   _versions[2] = v;
}

//------------ OPERATORS ------------

//All possible cases take one by one
bool Version::operator <(Version v) const
{
   //Last version level is used for the last decision
   for (int i=0; i<LEVELS-1; i++)
      if (_versions[i] < v._versions[i])
         return true;
      else if (_versions[i] > v._versions[i])
         return false;
   //Last decision
   return _versions[LEVELS-1] < v._versions[LEVELS-1];
}

// Bigger means not smaller and not equal
bool Version::operator >(Version v) const
{
   return !(*this < v) && (*this != v);
}

bool Version::operator ==(Version v) const
{
   bool b = true;
   for (int i=0; i<LEVELS; i++)
      b &=_versions[i] == v._versions[i];
   return b;
}

bool Version::operator !=(Version v) const
{
   return ! (*this == v);
}

//------------ GETTERS --------------

//If the version is invalid, return 0.0.0

uint Version::major()
{
   return this->isValid() ? _versions[0] : 0;
}

uint Version::minor()
{
   return this->isValid() ? _versions[1] : 0;
}

uint Version::dev()
{
   return this->isValid() ? _versions[2] : 0;
}

//Return 0.0.0 if invalid, or construct the string (with or wothout the dev part)
QString Version::toString(bool showDev) const
{
   if (! this->isValid())
      return "0.0.0";

   if (showDev)
      return QString("%1.%2.%3").arg(_versions[0]).arg(_versions[1]).arg(_versions[2]);
   else
      return QString("%1.%2").arg(_versions[0]).arg(_versions[1]);
}

//Use the major version to determine the validity
bool Version::isValid() const
{
   return (_versions[0] != -1);
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

