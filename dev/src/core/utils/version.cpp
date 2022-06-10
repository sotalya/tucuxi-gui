//@@license@@

#include "core/utils/version.h"

#include <QRegExp>
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
   QRegExp reg ("^([0-9]+)\\.([0-9]+)\\.?([0-9]*)$");

   //Check the string
   if ( ! reg.exactMatch(s))
      return false;

   //Retrieve the numbers
   reg.indexIn(s);
   int cnt = -1; //Avoid the first string (entire match)
   foreach(QString value, reg.capturedTexts()) {
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

