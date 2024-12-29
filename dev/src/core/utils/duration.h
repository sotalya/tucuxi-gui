/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
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


#ifndef DURATION_H
#define DURATION_H

#include <QString>
#include <QMetaType>

namespace Tucuxi {
namespace Gui {
namespace Core {


/** Simple class that handle duration.
 * \ingroup utils
 * This class is able to load a duration from a string, give some data about it and then re-convert it to a string.
 * Created because QTime is not able to handle more than 24h, and QDateTime is not fitted for this purpose.
 * Duration use the milliseconds as its base of time, and cannot go under 0;

 * \section format From and to string formatting
 * Duration is able to read a time from a string.
 * A simple number is considered as an hour, as is a number followed by an 'h' character.
 * A number after a 'h' character is considered as a number of minutes.
 */
class Duration
{
public:
   //! Creates an empty duration (duration of 0)
   Duration();

   //! \brief Construct a duration from a number of hours, minutes, seconds and milliseconds
   /** The milliseconds can possibly be added.
     @param h Number of hours to add
     @param m Number of minutes to add
     @param s Number of seconds to add
     @param ms Number of milliseconds to add
     */
   Duration (qint64 h, qint64 m=0, qint64 s=0, qint64 ms=0);

   //! \brief Construct a duration from a string
   /** Intenally uses the fromString() function.
      @param str The string containing the duration
      \sa fromString
   */
   Duration (QString str);

   //! \brief Read a duration from a string
   /** The string is read and analyzed using regex.
      Any trailing whitespace will be ignored.
      If a number out of range is given or if the syntax is incorrect, a DATAERROR will be thrown.
      @param str The string containing the duration
      @return True if the string format was correct, false otherwise
      \sa toString
    */
   bool fromString (QString str);

   //! \brief Create a string from a duration
   /** If the number of minutes is equal to 0, will use the short form : 13h.
      Else, the full form will be used : 13h45.
      The time under the minutes are ignored.
      @return The string representing the duration
      \sa fromString
    */
   QString toString () const;

   //! \brief Create a string from a duration
   /**
      The full form will be used : 0 days, 13h45m12s.
      The time under the seconds are ignored.
      @return The string representing the duration
      \sa fromString
    */
   QString toLongString () const;

   //! \brief Is the duration empty?
   /** If the duration has not been set or if the duration equals to 0, returns true.
      @return True if the duration is empty
     */
   bool isEmpty () const;

   /** \brief Checks if the duration is negative.
    * @return Returns true if the duration is negative, otherwise returns false.
    */
   bool isNegative() const;

   //! \brief Add this number of hours
   /** @param h The hours to add
    * @return This duration.
    */
   Duration &addHours (qint64 h);

   //! \brief Add this number of minutes
   /** @param m The minutes to add
    * @return This duration.
    */
   Duration &addMinutes (qint64 m);

   //! \brief Add this number of seconds
   /** @param s The seconds to add
    * @return This duration.
    */
   Duration &addSecs (qint64 s);

   //! \brief Add this number of milliseconds
   /** @param ms The milliseconds to add
    * @return This duration.
    */
   Duration &addMSecs (qint64 ms);

   //! \brief Return the total number of hours in a double
   /** Return the total amount of hours, anything under the hours being in the decimal.
     @return The hours in this duration
    */
   double toHours () const;

   //! \brief Return the total number of minutes in a double
   /** Return the total amount of hours, anything under the hours being in the decimal.
     @return The minutes for this duration
    */
   Q_INVOKABLE double toMinutes () const;

   //! \brief Return the total number of seconds in a double
   /** Return the total amount of hours, anything under the hours being in the decimal.
     @return The seconds for this duration
    */
   double toSeconds () const;

   //! \brief Return the number of days
   /** Truncate any time under the day
     @return The number of days in this duration
    */
   qint64 days () const;

   //! \brief Return the number of hours
   /** Truncate any time under the hour
     @return The number of hours in this duration
    */
   qint64 hours () const;

   //! \brief Return the number of minutes
   /** Return the number of minutes in this duration, including the hours.
     @return The total number of minutes for this duration
    */
   qint64 minutes () const;

   //! \brief Return the number of seconds
   /** Return the number of seconds in this duration, including the hours and the minutes.
     @return The total number of seconds for this duration
    */
   qint64 seconds () const;

   //! \brief Return the number of milliseconds
   /** Return the number of milliseconds in this duration, including the hours, the minutes and the seconds.
     @return The total number of minutes for this duration
    */
   qint64 mSecs () const;

   //! \brief Reset the duration to 0h00
   void clear ();

   //! \brief Compute the number of years, takign into account leap years.
   /** Returns the number of years between end and start, taking into account leap years.
     @return the number of years between start and end.
    */
   static qint64 yearsBetween(const QDate &start, const QDate &end);

   //! \brief Add two duration
   Duration operator+ (const Duration &) const;

   //! \brief Add the two and assign them
   void operator+= (const Duration &);

   //! Add a duration to a string
   /** Internally the toString() function will be used.
     */
   QString operator+ (const QString &) const;

   //! \brief Multiply the duration by this value
   Duration operator* (int) const;

   //! \brief Multiply the duration by this value
   Duration operator* (double) const;

   //! \brief Multiply the duration by this duration
   Duration operator* (const Duration &) const;

   //! \brief Divide the duration by this value
   Duration operator/ (int) const;

   //! \brief Divide the duration by this value
   Duration operator/ (double) const;

   //! \brief Divide the duration by this duration
   Duration operator/ (const Duration &) const;

   //! \brief Obtain the remainder of dividing the duration by this duration
   Duration operator% (const Duration &) const;

   //! \brief Substract two duration
   /** If the duration to substract is smaller than the duration substracted, the resulting duration will be negative.
     */
   Duration operator- (const Duration &) const;

   /** \brief Multiplication compound assignment overload.
    * @param rhs Right-hand side integer.
    */
   Duration &operator*=(int rhs);

   /** \brief Multiplication compound assignment overload.
    * @param rhs Right-hand side double.
    */
   Duration &operator*=(double rhs);

   //! \brief Substract the two and assign them
   void operator-= (const Duration &);

   //! \brief Is the duration smaller?
   bool operator< (const Duration &) const;

   //! \brief Is the duration bigger?
   bool operator> (const Duration &) const;

   //! \brief Is the duration bigger or equal?
   bool operator>= (const Duration &) const;

   //! \brief Is the duration smaller or equal?
   bool operator<= (const Duration &) const;

   //! \brief Are these durations equals?
   bool operator== (const Duration &) const;

   //! \brief Are these durations not equals?
   bool operator!= (const Duration &) const;

private:
   //! Duration in [ms]
   qint64 duration;
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

//Declare this type to Qt
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Duration)

#endif // DURATION_H
