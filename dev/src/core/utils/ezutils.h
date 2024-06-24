//@@license@@

#ifndef UTILS_H
#define UTILS_H

#include <QCoreApplication>
#include <QString>
#include <QVariant>
#include <QMetaType>

#include "dbobjects.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



class Unit;

//! \brief Reunion of a name and a description
/** \ingroup utils
  Usually translated in multiple languages, this structure offers a name and a description in an unique variable.
  */
class Descriptor {
    Q_DECLARE_TR_FUNCTIONS(Descriptor)
public:
    Descriptor();    
    Descriptor(QString id, QString name, QString description) : name(name), description(description), id(id) {}
    //! A name
    QString name;
    //! A description corresponding to the name
    QString description;
    //! An ID
    QString id;

    //! Return a string representation of this object
    QString toString ();

    QString getName () const;
    QString getDescription () const;

    //! Returns true if the ID, name and descriptions are not empty.
    bool isValid() const;

    /** Compare two descriptor.
     * Only the non-empty fields are compared.
     * If all the fields are empty in one but not on both, returns false.
     */
    bool operator== (const Descriptor& other) const;
};

/** Descriptor for a model.
 * \ingroup utils
 * In addition to the Descriptor fields, the intake is saved.
 */
class ModelDescriptor : public Descriptor
{
public:
    ModelDescriptor() { }
    ModelDescriptor (QString id, QString modelId, QString name, QString description, QString intake);

    //! The model Id. Different comeths can share the same model ID, for instance the
    //! implementation of linear 1 comp rk4 and analytic
    QString modelId;

    //! The model's intake.
    QString intake;

    //! Same as Descriptor::toString, with the intake appended.
    QString toString();

    //! Same as Descriptor.
    bool operator== (const ModelDescriptor& other) const;
};

// YTA: Hack
typedef int ident;

//! \brief An option for the model
/** \ingroup utils
  Models have options.
  These options can be of multiple types and so need to be stored in a QVariant.
  They have a unique identifier used only inside the model and a descriptor allowing their name and a short description to be used.
  A flag is present to determine if this option is a read-only or writable.
  */
class Option {
public:
    Option ();
    Option (Descriptor infos, int type, QVariant value, QString oid, QString group, bool readonly);
    //! The option's name and description.
    Descriptor infos;
    //! The type of the option, using QVariant::Invalid means that this option is a group descriptor.
    QMetaType::Type type;
    //! The option value.
    QVariant value;
    //! The identifier in the model (not the whole software).
    QString oid;
    //! Group in which reside this option (oid of the option defining the group).
    QString group;
    //! A read-only flag, primary useful for the GUI.
    bool readonly;
    //! The option database ID.
    ident sqlId;
};

//! \brief FileName type used for the options
/** \ingroup utils
  This class is used to represent an option that can be cast to a QVariant.
  While a QString could represent a file name, this class embeds the notion
  of extension (*.csv for instance) and allow the OptionsWidget to display
  a browse button that allows the user to select a file.
  */
class FileName : public QString
{
    public:
        FileName();
        FileName(const QString &name);
        FileName(const QString &name,const QString &ext);

        /** extension of the files to be selected
          It can be for instance *.csv , or *.csv,*.xls,*.txt
          */
        QString ext;
};

//! \brief DirName type used for the options
/** \ingroup utils
  This class is used to represent an option that can be cast to a QVariant.
  While a QString could represent a direcotry name, this class allows the
  OptionsWidget to display a browse button that allows the user to select a
  directory.
  */
class DirName : public QString
{
    public:
        DirName();
        DirName(const QString &name);
};



} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::DirName);
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ModelDescriptor)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::FileName);
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Descriptor)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Option)

#endif // UTILS_H
