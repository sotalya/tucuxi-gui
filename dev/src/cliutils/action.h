//@@license@@

#ifndef ACTION_H
#define ACTION_H

/** \defgroup actions Possible actions
  \ingroup cli
  All the possible actions, implementing the Action interface, are listed here.
  \sa Action
  */

#include <QString>
#include <QList>
#include <QObject>

#include "cli_errors.h"

//! \brief The actions interface
/** \ingroup cli
  All actions have the same inputs and the same output, an interface has been so defined to simplify the work of the main function.
  As input, the command line parameters following the action are given.
  As output, an error code is asked (if everything went fine, the Error::NOEZERRORS code has to be returned) and possibly an error message, if something went wrong.
  */
namespace ezechiel {
namespace cliutils {

class Action : public QObject
{
    Q_OBJECT

public:

    //! Return the username command parameter.
    /** Return the username command parameter that must to retrieve the username.
     * @return The username command parameter.
     */
    static QString usernameParam();

    //! Return the password command parameter.
    /** Return the password command parameter that must to retrieve the password.
     * @return The password command parameter.
     */
    static QString passwordParam();

    //! Return the date/time format.
    /** Return the date/time format that must be used by the user for the input.
     * @return The date/time format.
     */
    static QString dateTimeFormat();

    //! Return the XML date/time format.
    /** Return the XML date/time format used to parse EzeCHieL XML files.
     * @return The XML date/time format.
     */
    static QString xmlDateTimeFormat();

    //! Return the action name.
    /** Return the action name that must be used to execute this action, or any of this action sub-
     * commands. A static function of the same name must be implemented in each Action subclasses.
     * @return The action name.
     */
    static QString name();

    //! Return the action help.
    /** Return the action help of this action, listing the possible sub-commands and their effect.
     * A static function of the same name must be implemented in each Action subclasses.
     * @return The action help.
     */
    virtual QString help();

    //! Destructor.
    virtual ~Action() = 0;

    //! Run the action
    /** Run the action accordingly to the arguments passed to the function. The arguments does not
     * include the action name itself.
     * @param params The arguments list.
     * @return True if the operation was successfull.
     */
    virtual bool run (const QStringList &args) = 0;

    //! Return the last error.
    /** Return the last error message, or an empty string if noone occured.
     * @return The last error message.
     */
    virtual QString error() const;

protected:
    //The commands parameters
    static const char *const _CMD_PARAM_USERNAME;
    static const char *const _CMD_PARAM_PASSWORD;

    //The error string
    QString _errors;

    //The dates and times formats
    static const char *const _dateTimeFormat;
    static const char *const _xmlDateTimeFormat;

};
} //namespace cliutils
} //namespace ezechiel
#endif // ACTION_H
