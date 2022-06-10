//@@license@@

#ifndef LISTACTION_H
#define LISTACTION_H

#include <QCoreApplication>

#include "action.h"
#include "cli_errors.h"

//! \brief The 'list' corresponding action
/** \ingroup actions
  This action permit to retrieve data from the database.
  If a specific item is wanted, its informations will be showed, othewise a list of possible item will be printed.
  */
namespace Tucuxi {
namespace cliutils {
class ListAction : public Action
{
    Q_DECLARE_TR_FUNCTIONS(ListAction)
public:

    /** \brief Returns the action name.
     * @return The action name.
     */
    static QString name();

    /** \brief Returns the action help.
     * @return The action help.
     */
    QString help();

    //! Run the action.
    /** Run the action given the arguments. If an error occurs, an error message is set in the error
     * string.
     * @arg args The list of arguments.
     * @return True if the action was run without any error.
     */
    bool run(const QStringList &args);

private:
    //Lists the engines options
    void listOptions(const QString &arg);

};

} //namespace cliutils
} //namespace Tucuxi
#endif // LISTACTION_H
