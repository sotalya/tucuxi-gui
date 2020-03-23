#ifndef OPTIONABLE_H
#define OPTIONABLE_H

#include <QCoreApplication>
#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>

#include "core/utils/ezutils.h"

namespace ezechiel {
namespace core {

//! \brief Describes an option having option
/** \ingroup core
   The option system enables an object the have some options that can be set by the end user at runtime.
   This options are currently used in prediction engines and models.
   By implementing this class, an object describes itself as able to have some option.
   The option ID (OID) have to be unique inside of the object.
   The default implementation is doing some type checking when added, and uses the options list to fetch them.
   A child object should declares all its options using the addOption() function on its creation.
  */
class Optionable : public QObject
{
    //Q_DECLARE_TR_FUNCTIONS(Optionable)
public:

    //! Destructor.
    virtual ~Optionable();

    /** \brief Get the object's options list.
    * @return The list of options.
    */
    virtual QList<Option> getOptions () const;

    /** \brief Get the option corresponding to the given option ID.
    * @param oid The option ID.
    * @return The option.
    */
    virtual Option option(const QString &oid) const;

    /** \brief Set an option's value.
    * @param oid The option ID.
    * @param value The option value.
    * @return True if the value was successfully set.
    */
    virtual bool setOptionValue(const QString &oid, const QVariant &value);

    /** \brief Set an option's database ID.
     * @param oid The option ID.
     * @param id The option database ID.
     * @return True if the value was successfully set.
     */
    virtual bool setOptionDbId(const QString &oid, const ident id);

    //! Copy the options of another Optionable object.
    /** Copy the options of another Optionable object. Please not any previous option will be erased.
     * @param other An optionable object to copy from.
     */
    virtual void copyOptions (Optionable *other);

    /** \brief Check if the option with the given ID exists.
     * @param oid The option ID.
     * @return True if the optionable object contains an option with the given ID.
     */
    virtual bool optionExists (const QString &oid) const;

protected:

    /** \brief Add an option to the optionable object.
     * @param oid The option ID.
     * @param name The option name
     * @param desc The option description
     * @param type The option type
     * @param value The option default value
     * @param group An optional group ID.
     */
    virtual void addOption (const QString &oid, const QString &name, const QString &desc, int type, const QVariant &value, const QString &group = "");

    /** \brief Remove an option from this optionable object.
     * @param oid The option ID.
     */
    virtual void removeOption (const QString &oid);

private:
   //Map of <Option ID, Option>
   QMap<QString, Option> _options;   
};

} // namespace core
} // namespace ezechiel

#endif // OPTIONABLE_H
