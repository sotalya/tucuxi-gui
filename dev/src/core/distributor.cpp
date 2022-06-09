//@@license@@

#include "distributor.h"
#include "core/core.h"
//#include "coremessagehandler.h"
#include "settings.h"
#include "current_version.h"

#include <QtNetwork/QNetworkRequest>
#include <QEventLoop>
#include <QDir>
#include <QProcess>
#include <QString>

#include <stdlib.h>

namespace ezechiel {
namespace GuiCore {


//The settings names
const char *const Distributor::_stgChannel = "channel";

//Creates the network manager
Distributor::Distributor(QObject *parent) :
   QObject(parent),
   downloadReply(0),
   manager(0),
   _isFetched(false),
   _channel(Stable)
{
    //Manager
    manager = new QNetworkAccessManager(this);
    //Reset the values
    this->reset();

    //Setup our config on load the channel
    this->setChannel(SETTINGS.get(Module::Core, metaObject()->className(), _stgChannel, QVariant(this->descriptor(RELEASE_TYPE).id)).toString());
}

//No magic, remove the objects
Distributor::~Distributor()
{
   if (downloadReply) delete downloadReply;
   if (manager) delete manager;
}

// On error, clear and set the _errors message
void Distributor::networkError(QNetworkReply::NetworkError e)
{
   _errors.clear();
   _errors << tr("A network error has occured (error %1)").arg(e);
}

//Write each chunk of data into the installer file
void Distributor::downloadReadyRead()
{
   //In case of error, emit the downloadFinished() signal and quit
   if (downloadReply->error() != QNetworkReply::NoError) {
      _errors.clear();
      _errors << tr("Error while reading the downloaded data :");
      _errors << downloadReply->errorString();
      emit downloadFinished(false);
      return;
   }

   //Write what's downloaded in the file
   QByteArray data;
   for (;;) {
      data = downloadReply->read(MAX_CHUNK);
      //When there is no more data to write, exit
      if (data.isEmpty())
         break;
      else
         installer.write(data);
   }

   //If the whole file is downloaded, close it and send the downloadFinished() signal
   if (downloadReply->isFinished()) {
      installer.close();
      emit downloadFinished(true);
   }
}

//------------------ ACTIONS ---------------------

//Find the last version available, and fetch its change log and the installer URL
bool Distributor::fetch()
{
   //Reset the old data
   this->reset();

   //Create the base URL
   QString base = QString("%1%2/%3/%4/%5").arg(PROTOCOL).arg(SERVER).arg(PATH).arg(ARCH).arg(this->descriptor(_channel).id);

   //Fetch the last version available
   QString last(this->get(base + "/" + LAST_FILE));
   _lastVersion.fromString(last);
   qDebug() << QString("Last available version from %1: %2").arg(base,last);

   //If the version is invalid, quit
   if (!_lastVersion.isValid()) {
      _errors << tr("The last available version given is invalid : %1.").arg(last);
      return false;
   }

   //Re-define the base
   base.append('/' + _lastVersion.toString());

   //Fetch the changes
   _changeLog.append(QString(get(base + "/" + CHANGES_FILE)).split('\n'));
   //Remove the last line if empty
   if (_changeLog.last().isEmpty())
      _changeLog.removeLast();

   //If the changes are empty, quit
   if (_changeLog.isEmpty()) {
      _errors << tr("The change log is empty");
      return false;
   }

   //Fetch the installer URL
   _webLink = QString(get(base + "/" + URL_FILE)).remove("\n");
   //Create the local installer NAME (not file) in the temporary directory
   _localLink = QDir().absoluteFilePath(QString("%1/ezechiel/%2_%3").arg(QDir::tempPath(), _lastVersion.toString(), QFileInfo(_webLink.path()).fileName()));
   _localLink.remove("\n"); //I don't know why, but on windows, a return carrier appears, and crashes the software
   _localLink.simplified();

   //If the link is invalid, quit
   if (!_webLink.isValid()) {
      _errors << tr("The given installer link is invalid : %1.").arg(_webLink.toString());
      return false;
   }

   //We are fetched, and we have to check the last date
   _isFetched = true;
   _lastCheck = QDateTime::currentDateTime();

   //Ok, this time, everything went fine
   return true;
}

//Start the installer using a QProcess
bool Distributor::run(bool blocking)
{
   QProcess p;

   //Set the installer as executable
   QFile(_localLink).setPermissions(QFile::ExeOwner);

   //Add the silent option and the install directory
   QString exec = _localLink + " /S /D=" + CORE->path(Core::Executable);

   qDebug() << "Running "+ exec;

   //On blocking, use exec and return true if the process returns 0
   bool ok;
   if (blocking)
      ok = (p.execute(exec) == 0);
   else { //Non-blocking, start in another thread
      ok = p.startDetached(exec);
   }

   //Log the error
   if (!ok) {
      _errors.clear();
      _errors << tr("Unable to launch the installer executable");
      _errors << exec;
      _errors << p.errorString();
   }

   return true;
}

//Start the installer download
void Distributor::download()
{
   //Create the request
   QNetworkRequest request(_webLink);

   qDebug() << QString("Downloading %1 in %2").arg(_webLink.toString(),_localLink);

   //Create and set the path
   QDir::temp().mkpath("ezechiel");
   installer.setFileName(_localLink);

   //Remove the file - download a fresh one
   installer.remove();

   //Open the installer file
   if ( ! installer.open(QIODevice::WriteOnly)) {
      //On error, emit the downloadFinished() signal and quit
      _errors.clear();
      _errors << tr("Error while opening the file %1").arg(installer.fileName());
      _errors << installer.errorString();
      emit downloadFinished(false);
      return;
   }

   //Launch the request
   if (downloadReply) delete downloadReply;
   downloadReply = manager->get(request);
   connect(downloadReply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(networkError(QNetworkReply::NetworkError)));
   connect(downloadReply, SIGNAL(readyRead()),
            this, SLOT(downloadReadyRead()));
   connect(downloadReply, SIGNAL(finished()),
            this, SLOT(downloadReadyRead()));
   connect(downloadReply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(downloadProgressed(qint64,qint64)));

}

//------------------ GETTERS ---------------------

// COnstruct the base address
QString Distributor::website()
{
   return QString(PROTOCOL) + SERVER;
}

//Static descriptors, one for each channel
Descriptor Distributor::descriptor(Distributor::ChannelType channel)
{
   switch (channel)
   {
   case Dev:
      return Descriptor("dev",tr("Development"),tr("Development channel, the latest non-tested version."));
   case Beta:
      return Descriptor("beta",tr("Beta"),tr("Beta release channel, tested but not field tested."));
   case Stable:
      return Descriptor("stable",tr("Stable"),tr("Stable release channel, field tested."));
   case Test:
      return Descriptor("test",tr("Test"),tr("Test channel, should not be shown. DO NOT USE!!!"));
   default:
      return Descriptor("invalid",tr("Invalid"),tr("Invalid channel"));
   }
}

Distributor::ChannelType Distributor::channel()
{
   return _channel;
}

//If not fetched, error
QStringList Distributor::changeLog()
{
   if (_isFetched)
      return _changeLog;
   else {
      _errors.clear();
      _errors << tr("Change log requested but not fetched");
      return QStringList();
   }
}

//If not fetched, error
QString Distributor::installerPath()
{
   if (_isFetched)
      return _localLink;
   else {
      _errors.clear();
      _errors << tr("Installer path requested but not fetched");
      return QString();
   }
}

//If not fetched, error
Version Distributor::lastVersion()
{
   if (_isFetched)
      return _lastVersion;
   else {
      _errors.clear();
      _errors << tr("Last version requested but not fetched");
      return Version();
   }
}

//Use the current Tucuxi's version (according we are fecthed)
bool Distributor::isNewer()
{
   return (_lastVersion > CORE->version(Core::Tucuxi)) && _isFetched;
}

bool Distributor::isFetched()
{
   return _isFetched;
}

QStringList Distributor::errors()
{
   return _errors;
}

QDateTime Distributor::lastCheck()
{
   return _lastCheck;
}

//--------------------- SETTERS

//If the new and old channels are differents, reset the values
void Distributor::setChannel(Distributor::ChannelType channel)
{
   if (channel != _channel)
      this->reset();

   _channel = channel;
}

//Try each possible value
void Distributor::setChannel(const QString &id)
{
    if (id == "dev")
        _channel = Dev;
    else if (id == "beta")
        _channel = Beta;
    else if (id == "stable")
        _channel = Stable;
    else if (id == "test")
        _channel = Test;
    //Else, ignore the given value
}

//--------------------- PRIVATE

QByteArray Distributor::get(const QUrl url)
{
   //Create the request
   QNetworkRequest request(url);

   //Launch the request
   QNetworkReply *reply = manager->get(request);
   connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(networkError(QNetworkReply::NetworkError)));

   //Wait for the reply
   QEventLoop loop;
   QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
   loop.exec();

   //In case of error, return an empty byte stream
   if (reply->error() != QNetworkReply::NoError)
      return QByteArray();

   //Else, return the reply's content
   return reply->readAll();
}

void Distributor::reset()
{
   _lastVersion.reset();
   _changeLog.clear();
   _webLink.clear();
   _errors.clear();
   _localLink.clear();
}

//Calculate the percent, and return the result
void Distributor::downloadProgressed(qint64 received, qint64 total)
{
   if (total > 0)
      emit progress((received*100)/total);
}

} // namespace core
} // namespace ezechiel

