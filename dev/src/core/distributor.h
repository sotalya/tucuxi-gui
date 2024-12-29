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


#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H

#include <QObject>
#include <QStringList>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QFile>
#include <QByteArray>
#include <QDateTime>

#include "core/utils/ezutils.h"
#include "core/utils/version.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


// Static server address to use
#define SERVER "www.ezechiel.ch"

// Link to the directory containing the channels
#ifdef Q_OS_WIN32
#   define PATH "releases/windows/"
#elif defined (Q_OS_DARWIN) //#elifdef Q_OS_DARWIN
#   define PATH "releases/macos/"
#else
#   define PATH "releases/unknown/"
#endif

// Define the arch
//#ifdef __GNUC__
#   if __x86_64__ || __ppc64__
#       define ARCH "64"
#   else
#       define ARCH "32"
#   endif
//#endif


// Protocol to use to access the data
#define PROTOCOL "http://"

// Name of the file containing the last version
#define LAST_FILE "last.txt"

// Name of the file containing the changes
#define CHANGES_FILE "changes.txt"

// Name of the file containing the installer URL
#define URL_FILE "installer.txt"

// Maximum chunk of data to read (for the installer download)
#define MAX_CHUNK 1048576 // 1Mio

// Argument to give to the installer executable
#define ARGUMENTS ""


//! Tucuxi's release distribution system backend.
/** \ingroup core

   This object is able to fetch the last version of the Tucuxi's installer, and then run it.
   It provides :
   \li a way to fetch and check if the current version is the last
   \li a way to download the installator
   \li a way to run the installator

   When an operation failed (returned false), the errors() function can be fetched to find out the reason.

   The channel system enable a software to be connected to a specific channel, and the development team to provide multiple channels.
   For example, a channel can be used for the alpha release, another for the stable one, and another for the development one.
  */
class Distributor : public QObject
{
    Q_OBJECT
public:
   //! \brief List of possible channels
   enum ChannelType {
      //! Development channel, to fetch the last build version
      Dev,
      //! Stable channel, to fetch the last thorougly tested version
      Stable,
      //! Beta channel, to fetch the last tested version (but not field tested)
      Beta,
      //Last value, used for loops, do not touch
      LastChannel,
      //Test value, used only for tests, not to be used
      Test
   };

   Distributor(QObject *parent = nullptr);

   ~Distributor();

   //------------------ ACTIONS ---------------------

   //! \brief Fetch the last available version
   /** With the last version, the link to the installer and its size will be retrieved.
      Also, the list of changes is fetched and can be retrieved using the changeLog() function.
      @param channel The channel to fetch the data from
      @return True if everything went fine, false otherwise (in this case, see the errors() function)
    */
   bool fetch ();

   //! \brief Run the downloaded installer
   /** By default, the installator will be launched in another thread.
      @param blocking If true, the installer will not be launched in another thread, but in the current one
      @return True if the installator was succesfully launched, false otherwise (in this case, see the errors() function)
     */
   bool run (bool blocking = false);

   //------------------ GETTERS ---------------------

   //! \brief Retrieve the Tucuxi's website address
   QString website ();

   //! \brief Description of the channels
   /** Return the description of the given channel.
      @param channel The channel for which the description is asked
      @return The corresponding description
     */
   Descriptor descriptor (ChannelType channel);

   //! \brief Retrieve the current channel
   ChannelType channel ();

   //! \brief Retrieve the change log
   /** The change log is downloaded in the fetch() function.
      @return The change log, line by line, or an empty list on error (in this case, see the errors() function)
     */
   QStringList changeLog ();

   //! \brief Last Tucuxi available version
   /** The last version is downloaded in the fetch() function
      @return The last available Tucuxi's version, or an invalid version if an error occured (in this case, see the errors() function)
     */
   Version lastVersion ();

   //! \brief Is a new version available?
   bool isNewer ();

   //! \brief Has a fetch being done?
   bool isFetched ();

   //! \brief Retrieve the installer path
   /** This path is the installer name as it was downloaded placed on the system's temporary directory.
      @return The full installer path
     */
   QString installerPath ();

   //! \brief Date of the last check
   /** When a fetch is done, the date on which it has been done is saved.
      @return The date of the last check
     */
   QDateTime lastCheck ();

   //! \brief Retrieve the errors strings
   /** For each error, a QString is given.
      @return A list of errors that occured
     */
   QStringList errors ();


   //------------------ SETTERS ---------------------

   //! \brief Set the channel to use
   /** A channel describes what kind of Tucuxi's release to download.
      Each channel has its own directory on the server, and are described by the ChannelType enumerator.
      Calling this function will reset the fetch status if the channel is different.
      @param channel The new channel to use
     */
   void setChannel (ChannelType channel);

   /** Set the channel to use from its ID.
    * Used internally to load the value from the configuration.
    * See the other setChannel(ChannelType) function for more information.
    * @param id The channel ID to set
    * \sa setChannel(ChannelType) channel ChannelType
    */
   void setChannel (const QString &id);

signals:

   //! \brief The download is progressing
   /** Called each time the download has progressed.
      Units are in percent [%].
      @param percent Percentage of the download already done
    */
   void progress (int percent);

   //! \brief The download has finished
   /** Called when the download has finished.
      @param succeed True if the download succeed, false otherwise
    */
   void downloadFinished (bool succeed = true);

public slots:

   //! \brief Launch the download
   /** The download is just launched, this function is non-blocking.
      Use the downloadFinished() signal to know when the download finished, and in which state.
      If this function is called when no new version is available, the download won't be launched, and the downloadFinished() signal will be emitted with false.
    */
   void download ();

private slots:

   // A network error has occured, the error will be set in _errors
   void networkError (QNetworkReply::NetworkError e);

   // A new part of the downloaded file can be readed
   void downloadReadyRead ();

   // The download progressed, emit a progress() signal
   void downloadProgressed(qint64 received, qint64 total);

private:
   //The settings names
   static const char *const _stgChannel;
   // Current error list
   QStringList _errors;
   // Web link to the installer
   QUrl _webLink;
   // Local link to the installer
   QString _localLink;
   // Local installer file device
   QFile installer;
   // Last version available
   Version _lastVersion;
   // Change log
   QStringList _changeLog;
   // Last progress emitted
   int _lastProgress;
   // Has the data been fetched by fetch() ?
   bool _isFetched;
   // Current channel
   ChannelType _channel;
   // When was the last check done?
   QDateTime _lastCheck;

   //Network part
   // Download reply
   QNetworkReply *downloadReply;
   // Our network manager
   QNetworkAccessManager *manager;

   //-------- FUNCTIONS

   //Retrieve a file content synchronously
   QByteArray get (const QUrl url);

   //Reset the internal values
   void reset ();

};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // DISTRIBUTOR_H
