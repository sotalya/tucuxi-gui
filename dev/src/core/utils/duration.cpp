//@@license@@

#include "core/utils/duration.h"

#include <QDate>
#include <QRegExp>
#include <QStringList>
#include <QObject>
#include <QDebug>

#include <math.h>

//#include "core_errors.h"
//#include "coremessagehandler.h"
#include "core/core.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


// Initiate the duration with 0
Duration::Duration()
   : duration(0)
{
}

// Use the other functions to create the duration
Duration::Duration (qint64 h, qint64 m, qint64 s, qint64 ms)
   : duration(0)
{
   if (h != 0)
      this->addHours(h);
   if (m != 0)
      this->addMinutes(m);
   if (s != 0)
      this->addSecs(s);
   if (ms != 0)
      this->addMSecs(ms);
}

// Use formString()
Duration::Duration (QString str)
   : duration(0)
{
   this->fromString(str);
}

/* Remove the white space
   Analyse the string, add the value to the duration
   */
bool Duration::fromString(QString str)
{
   //Remove spaces
   QString s = str.simplified();
   //Create the regex
   QRegExp reg("^([0-9]+)h?([0-9]*)$");
   //Check the string
   if ( ! reg.exactMatch(s)) {
      qDebug() << "Matched="+QString::number(reg.matchedLength())+" in "+str.toLatin1();
      return false;
   }
   //Clear ourselves
   this->clear();
   //Retrieve the numbers
   reg.indexIn(s);
   QStringList list = reg.capturedTexts();
   //Get the hours
   this->addHours(list.at(1).toInt());
   //If the minutes are here, get them
   if (list.size() > 2)
      this->addMinutes(list.at(2).toInt());
   //All green
   return true;
}

// Simply create a string in the form hh'h'mm
QString Duration::toString() const
{
   int minutes = this->minutes() % 60;
   //Add a leading 0 when the minutes are under 10
   QString filler;
   if (abs(minutes) < 10)
      filler = "0";
   return QString::number(this->hours()) + 'h' + filler + QString::number(minutes);
}


// Simply create a string in the form hh'h'mm
QString Duration::toLongString() const
{
    int seconds = this->seconds() % 60;
    int minutes = this->minutes() % 60;
    int hours = this->hours() % 24;
    int days = this->days();
    if (days > 0) {
    return QString::number(days) + " days " + QString::number(hours) + 'h'
            + QString::number(minutes) + 'm' + QString::number(seconds) + 's';
    }
    else{
        return QString::number(hours) + 'h'
                + QString::number(minutes) + 'm' + QString::number(seconds) + 's';
    }
}

// Return true if duration = 0
bool Duration::isEmpty() const
{
    return (duration == 0);
}

//Checks if the duration is negative
bool Duration::isNegative() const
{
    return duration < 0;
}

// Add these hours, use the other functions
Duration &Duration::addHours(qint64 h)
{
   return this->addMinutes(h*60);
}

// Add these minutes, use the other functions
Duration &Duration::addMinutes(qint64 m)
{
   return this->addSecs(m*60);
}

// Directly add the seconds to duration
Duration &Duration::addSecs(qint64 s)
{
   duration += s*1000;
   return *this;
}

// Directly add the milliseconds to duration
Duration &Duration::addMSecs(qint64 ms)
{
   duration += ms;
   return *this;
}

// Calculate from the millisecs
double Duration::toHours() const
{
   return double(duration) / 1000.0 / 3600.0;
}

// Calculate from the millisecs
double Duration::toMinutes() const
{
   return double(duration) / 1000.0 / 60.0;
}

// Calculate from the millisecs
double Duration::toSeconds() const
{
    return double(duration) / 1000.0;
}

// Return the number of days, using the number of hours
qint64 Duration::days() const
{
    return this->hours()/24;
}

// Return the number of hours, using the number of minutes
qint64 Duration::hours() const
{
   return this->minutes()/60;
}

// Return the number of minutes using the number of seconds
qint64 Duration::minutes() const
{
   return this->seconds()/60;
}

// Directly return the number of seconds
qint64 Duration::seconds() const
{
   return duration/1000;
}

// Simply return duration
qint64 Duration::mSecs() const
{
   return duration;
}

qint64 Duration::yearsBetween(const QDate &start, const QDate &end)
{
    int years = end.year() - start.year();

    if (end.month() > start.month())
        return years;

    if (end.month() == start.month()) {
        if (end.day() >= start.day())
            return years;
    }

    return years - 1;
}

Duration Duration::operator +(const Duration &d) const
{
    return Duration(0,0,0,duration + d.mSecs());
}

void Duration::operator +=(const Duration &d)
{
    duration += d.duration;
}

QString Duration::operator +(const QString &s) const
{
    return this->toString() + s;
}

Duration Duration::operator *(int v) const
{
    Duration d;
    d.duration = duration * v;
    return d;
}

Duration Duration::operator *(double v) const
{
    Duration d;
    d.duration = duration * v;
    return d;
}

Duration Duration::operator *(const Duration &other) const
{
    Duration d;
    d.duration = duration * other.duration;
    return d;
}

Duration Duration::operator /(int v) const
{
    Duration d;
    d.duration = duration / v;
    return d;
}

Duration Duration::operator /(double v) const
{
    Duration d;
    d.duration = duration / v;
    return d;
}

Duration Duration::operator /(const Duration &other) const
{
    Duration d;
    d.duration = duration / other.duration;
    return d;
}

Duration Duration::operator %(const Duration &other) const
{
    qint64 r = duration % other.duration;
    return Duration(0, 0, 0, r);
}

Duration Duration::operator -(const Duration &d) const
{
   qint64 r = duration - d.mSecs();
   return Duration(0,0,0,r);
}

Duration &Duration::operator*=(int rhs)
{
    duration *= rhs;
    return *this;
}

Duration &Duration::operator*=(double rhs)
{
    duration *= rhs;
    return *this;
}

void Duration::operator -=(const Duration &d)
{
    duration -= d.duration;
}

bool Duration::operator <(const Duration &d) const
{
   return (duration < d.mSecs());
}

bool Duration::operator >(const Duration &d) const
{
   return (duration > d.mSecs());
}

bool Duration::operator >=(const Duration &d) const
{
   return (duration >= d.mSecs());
}

bool Duration::operator <=(const Duration &d) const
{
   return (duration <= d.mSecs());
}

bool Duration::operator ==(const Duration &d) const
{
   return (duration == d.mSecs());
}

bool Duration::operator !=(const Duration &d) const
{
   return (duration != d.mSecs());
}

void Duration::clear()
{
   duration = 0;
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
