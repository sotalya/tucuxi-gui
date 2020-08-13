#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QMutex>
#include <QObject>

namespace ezechiel {
namespace core {


/** \ingroup core
 *  \defgroup settings Settings
 *
 * The settings are used to store users preferences and non-critical configuration. This includes,
 * for example, the windows geometry and position, or the displaying of the chart's grid. In order
 * to achieve this, EzeCHieL offers a singleton that inherits from QSettings and offers helper
 * functions to set each setting's specific key depending on the module and class from where they
 * are set and retrieved.
 *
 * As such, each class that needs to use the settings system will have to choose a name for each
 * setting, and then access the singleton in order to set and retrieve the setting. The name must
 * be unique amongst the class or the module, depending on which helper function is used to store
 * and retrieve the parameter. The singleton can be retrieved either from the core or the settings
 * class itself.
 *
 * As the settings class is using QSettings mechanisms in the background, it is, in theory, possible
 * to use a QSettings instance directly, as explained in the Qt documentation. It is nonetheless
 * discouraged because the singleton uses a QSettings instance that doesn't use the default
 * mechanisms. In this regard, please always use the settings through the provided singleton.
 *
 * \sa Settings, Module, QSettings
 */

//! This enum represents a list of the different EzeCHieL's modules.
enum class Module {
    Chart,      //! The chart module.
    CLI,        //! The CLI module.
    Core,       //! The core module.
    Database,   //! The database module.
    GUI,        //! The GUI module.
    Plugin,     //! The plugin module.
    Report      //! The report module.
};

/** \ingroup settings
 *  \brief Entry class for managing settings.
 *
 * The settings class is the entry point for every class that needs to set and retrieve settings.
 * It inherits from QSettings and offers helper functions to construct a setting's key depending on
 * the module, and optionally the class, from which the setting is used. This system allows each
 * class that needs to use the settings mechanism to only care for choosing a setting's name that
 * is unique only in the class itself, or the whole module, depending on which helper function is
 * used.
 *
 * The settings class is implemented using the singleton pattern. The singleton itself can be
 * recovered either from the core or the class itself. The enum Module is used to specify the
 * module from which the functions are called. It is also possible to use QSettings functions
 * directly, as long as they are called from the singleton itself.
 *
 * The settings class's helper functions are thread-safe.
 *
 * \sa Module, QSettings
 */
class Settings : public QSettings
{

public:

    /** \brief Return the settings singleton.
     * @return The settings singleton.
     */
    static Settings& getInstance();

    /** \brief Clears all the settings of the given module.
     * @param module The module.
     */
    void clearModule(Module module);

    /** \brief Clears all the settings of the given module's class.
     * @param module The module.
     * @param className The class's name.
     */
    void clearClass(Module module, const QString &className);

    //! Clears the setting of the given module and name.
    /** Clears the setting of the given module and name. This is a helper function that will use
     * QSettings::remove() with a key that is constructed by concatenating the module's name and
     * the setting's name.
     * @param module The module.
     * @param name The setting's name.
     */
    void clearSetting(Module module, const QString &name);

    //! Clears the setting of the given module, class and name.
    /** Clears the setting of the given module, class and name. This is a helper function that
     * will use QSettings::remove() with a key that is constructed by concatenating the module's
     * name, the class's name and the setting's name.
     * @param module The module.
     * @param className The class's name.
     * @param name The setting's name.
     */
    void clearSetting(Module module, const QString &className, const QString &name);

    //! Returns the setting's value for the given module and name.
    /** Returns the setting's value for the given module and name. This is a helper function that
     * will use QSettings::value() with a key that is constructed by concatenating the module's
     * name and the setting's name.
     * @param module The module.
     * @param name The setting's name.
     * @param defaultValue An optional default value for the setting.
     * @return The setting's value.
     */
    QVariant get(Module module, const QString &name, const QVariant &defaultValue = QVariant());

    //! Returns the setting's value for the given module, class and name.
    /** Returns the setting's value for the given module, class and name. This is a helper
     * function that will use QSettings::value() with a key that is constructed by concatenating
     * the module's name, the class's name and the setting's name.
     * @param module The module.
     * @param className The class's name.
     * @param name The setting's name.
     * @param defaultValue An optional default value for the setting.
     * @return The setting's value.
     */
    QVariant get(Module module, const QString &className, const QString &name, const QVariant &defaultValue = QVariant());

    //! Returns the setting's value for the given module, class and name.
    /** Returns the setting's value for the given module, class and name. This is a helper
     * function that will use QSettings::value() with a key that is constructed by concatenating
     * the module's name, the class's name and the setting's name.
     * @param module The module.
     * @param className The class's name.
     * @param name The setting's name.
     * @param defaultValue An optional default value for the setting.
     * @return The setting's value.
     */
    QVariant get(Module module, const QString &className, const char *name, const QVariant &defaultValue = QVariant());

    //! Returns the settings values for the given module and names.
    /** Returns the settings values for the given module and names. This is a helper function that
     * will use QSettings::value() with a set of keys that are constructed by concatenating the
     * module's name and the settings names. The settings map passed to the function can be used to
     * provide a default value for each setting. If no default value is needed for a setting,
     * simply provide a default constructed QVariant for this setting.
     * @param module The module.
     * @param settings The list of settings names and values.
     * @return A map of the settings name and value.
     */
    QMap<QString, QVariant> getSettings(Module module, const QMap<QString, QVariant> &settings);

    //! Returns the settings values for the given module, class and names.
    /** Returns the settings values for the given module, class and names. This is a helper
     * function that will use QSettings::value() with a set of keys that are constructed by
     * concatenating the module's name, the class's name and the settings names. The settings map
     * passed to the function can be used to provide a default value for each setting. If no
     * default value is needed for a setting, simply provide a default constructed QVariant for
     * this setting.
     * @param module The module.
     * @param className The class's name.
     * @param settings The list of settings names and values.
     * @return A map of the settings name and value.
     */
    QMap<QString, QVariant> getSettings(Module module, const QString &className, const QMap<QString, QVariant> &settings);

    //! Sets the setting's value of the given module and name.
    /** Sets the setting's value of the given module and name. This is a helper function that will
     * use QSettings::setValue() with a key that is constructed by concatenating the module's name
     * and the setting's name.
     * @param module The module.
     * @param name The setting's name.
     * @param value The setting's value.
     */
    void set(Module module, const QString &name, const QVariant &value);

    //! Sets the setting's value of the given module, class and name.
    /** Sets the setting's value of the given module, class and name. This is a helper function
     * that will use QSettings::setValue() with a key that is constructed by concatenating the
     * module's name, the class's name and the setting's name.
     * @param module The module.
     * @param className The class's name.
     * @param name The setting's name.
     * @param value The setting's value.
     */
    void set(Module module, const QString &className, const QString &name, const QVariant &value);

    //! Sets the settings values of the given module and names.
    /** Sets the settings values of the given module and names. This is a helper function that will
     * use QSettings::setValue() with a set of keys that are constructed by concatenating the
     * module's name and the settings names.
     * @param module The module.
     * @param settings The list of settings names and values.
     */
    void setSettings(Module module, const QMap<QString, QVariant> &settings);

    //! Sets the settings values of the given module, class and names.
    /** Sets the settings values of the given module, class and names. This is a helper function
     * that will use QSettings::setValue() with a set of keys that are constructed by concatenating
     * the module's name, the class's name and the settings names.
     * @param module The module.
     * @param className The class's name.
     * @param settings The list of settings names and values.
     */
    void setSettings(Module module, const QString &className, const QMap<QString, QVariant> &settings);

private:
    //Constructor
    Settings(QObject *parent = nullptr);

    //Destructor
    ~Settings();

    //Copy Constructor
    Settings(const Settings &);

    //Assignment operator
    Settings &operator=(const Settings &);

    //The class's mutex
    QMutex _mutex;

    //The modules names
    QMap<Module, QString> _modules;
};

//class ParameterGraphSettings : public QObject
//{
//    Q_OBJECT

//public:
//    ParameterGraphSettings();

//    bool showPrediction;
//    bool showPerc5_95;
//    bool showPerc10_90;
//    bool showPerc25_75;
//    bool showPerc50;
//};

//class SingleGraphSettings : public QObject
//{
//    Q_OBJECT

//public:
//    SingleGraphSettings();

//    bool showTarget;
//    bool showCovariatesChange;
//    ParameterGraphSettings populationCurves;
//    ParameterGraphSettings aprioriCurves;
//    ParameterGraphSettings aposterioriCurves;
//    ParameterGraphSettings adjustmentCurves;
//};

//class GraphDisplaySettings : public QObject
//{
//    Q_OBJECT

//public:
//    GraphDisplaySettings();

//    SingleGraphSettings tabSettings[8];
//};



} // namespace core
} // namespace ezechiel

#endif // SETTINGS_H
