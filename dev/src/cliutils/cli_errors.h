//@@license@@

#ifndef CLI_ERRORS_H
#define CLI_ERRORS_H

//Used by cmake when pre-processing this file
#ifndef PREPROC_ONLY
#include <QString>
#include <QObject>
#endif

// Enum
#undef DECLARE_CLI_ERROR
#define DECLARE_CLI_ERROR( x,i ) x = i,
/*! \ingroup errors
  Errors for the CLI.
  */
enum CliError {
  #include "cli_errors.cpp"
   //! Last possible CLI error (not to be used)
   CLILASTERROR = 150
};

//! \brief Display informations about a CLI error
/*! \ingroup cli
   Exact replica of CoreErrorInfo.
   \sa CoreErrorInfo
  */
class CliErrorInfo
{
public:
#undef DECLARE_CLI_ERROR
#define DECLARE_CLI_ERROR( x,i ) case i : return #x;
   //! \sa CoreErrorInfo::name
   static QString name (CliError d)
   {
      switch (d) {
         #include "cli_errors.cpp"
         default: return "UNKNOWNERROR";
      }
   }

#undef DECLARE_CLI_ERROR
#define DECLARE_CLI_ERROR( x,i ) case i : return QObject::tr(#x);
   //! \sa CoreErrorInfo::desc
   static QString desc (CliError d)
   {
      switch (d) {
         #include "cli_errors.cpp"
         default: return QObject::tr("UNKNOWNERROR");
      }
   }
};


#endif // CLI_ERRORS_H
