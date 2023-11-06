//@@license@@

#ifndef TQFLOGGER_H
#define TQFLOGGER_H

#include <QMutex>
#include <QString>

#include "tucucore/computingrequestxmlexport.h"


/**
 * This class enables logging all the ComputingRequest in files.
 * It can be enabled, and if it is, each request will be logged
 * in a file starting with current the date and time.
 * If a file already exist with that name, a counter Id is added
 * at the end of the file name.
*/
class TqfLogger
{
public:

    ///
    /// \brief Returns the only instance of TqfLogger
    /// \return The only instance of TqfLogger
    ///
    static TqfLogger* getInstance();

    ///
    /// \brief sets the path into which files shall be created
    /// \param path The path
    /// The folder shall exist, Tucuxi will not create it
    ///
    void setPath(const QString &path);

    ///
    /// \brief Enables or not the logging
    /// \param enable boolean to enable or not logging
    ///
    void setEnable(bool enable);

    ///
    /// \brief logs the ComputingRequest in a file
    /// \param _request The ComputingRequest to log
    /// This function will create a file with a name embedding the
    /// current date and time, and a potential Id in case other
    /// files with the same date/time exist.
    ///
    void log(const Tucuxi::Core::ComputingRequest& _request);

private:

    /// Singleton pattern, so a private constructor
    TqfLogger() = default;

    /// Enables or not the logging
    bool enabled{false};

    /// Path of the folder in which the files are added
    QString path{"./log"};

    /// Path of the folder in which the files are added, followed by "/"
    QString currentPath{"./log/"};

    /// A mutex to protect the log() function
    QMutex mutex;
};

#endif // TQFLOGGER_H
