//@@license@@

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QMetaType>
#include <QString>

#include "core/utils/ezutils.h"
#include "core/utils/distribution.h"
#include "core/utils/units.h"
#include "core/dal/entity.h"
#include "core/utils/duration.h"
#include "core/dal/drug/operableamount.h"

namespace ezechiel {
namespace core {

class Bsv;
class Operation;
class ErrorModel;
class OperationList;
class TranslatableString;

//! \brief The different type of parameters
/** \ingroup utils
   A Percentile Engine and a predictor must be able to predict a concentration using different sets of parameters.
   For example, the population parameters use only default value that have no link with the patient, while the current parameters use user-entered values.
  */
enum ParameterType {
    //! Population or default parameters.
    POPULATION = 0,
    //! Parameters adapted to the patient using its covariates.
    APRIORI,
    //! Parameters adapted to the patient using its measures.
    APOSTERIORI,
    //! Current parameters, entered or modified by the user.
    MANUAL,
    //! Temporary parameters, can be used for calculation, but should not be read.
    TMP,
    //! Last one, not used as a parameter type, have to be the last declared.
    LAST_PT
};

//! \brief Only used by the PredictionSpec exporter
QString toString(ParameterType t);

//! \brief Only used by the PredictionSpec exporter
ParameterType fromString(QString s);

//! \brief Define what a parameter is
/** \ingroup utils
  A parameter is one of the possible options types given by the model's Model::getOptions() function.
  A parameter have a identifier used exclusively in the model, a value and a variability related to this value.
  The variability distribution type is used by the PercentileEngine in order to define the distribution in the variability.
  \sa Distribution
  */
class Parameter : public Entity {
    Q_OBJECT

    ENTITY_UTILS(Parameter)

    AUTO_PROPERTY_DECL(QString, name, Name)
    AUTO_PROPERTY_DECL(OperableAmount*, quantity, Quantity)
    AUTO_PROPERTY_DECL(Bsv*, bsv, Bsv)
    //! Minimal value of this parameter
    AUTO_PROPERTY_DECL(double, min, Min)
    //! Maximal value of this parameter
    AUTO_PROPERTY_DECL(double, max, Max)
    //! Step between each value (for the UI)
    AUTO_PROPERTY_DECL(double, step, Step)

    AUTO_PROPERTY_DECL(TranslatableString*, comments, Comments)

    // YTA: Not used yet, but I guess we should go this way
    AUTO_PROPERTY_DECL(OperationList*, operations, Operations)

protected:

    Q_INVOKABLE Parameter(AbstractRepository *repository, QObject* parent = 0);
    Q_INVOKABLE Parameter(AbstractRepository *repository, QObject* parent, QString pid,
              double value,
              double standard = 0.0,
              double proportional = 0.0,
              double min = 0.0,
              double max = 0.0,
              double step = 0.0,
              Unit unit = Unit());
    Parameter(AbstractRepository *repository, QObject* parent, Parameter* _p);

public:

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

    //! To print.
    QString toString () const;

    //! Indicates if the parameter is variable or not
    bool isVariable() const;

    /** Returns the variance of the parameter
     * @return the variance of the parameter
     * Be careful with this function. If the standard deviation is
     * different than 0, then the variability is considered as
     * standard. If not, then the proportional variability is
     * computed and returned.
     */
    double variance() const;

    /** Returns the deviation of the parameter
     * @return the deviation of the parameter
     * Be careful with this function. If the standard deviation is
     * different than 0, then the variability is considered as
     * standard, and the standard deviation is returned. If not,
     * then the proportional deviation is returned.
     */
    double deviation() const;

};

typedef Parameter* SharedParameter;

QML_POINTERLIST_CLASS_DECL(Parameters, Parameter)
// YTA: To be discussed with Rob: Is the type of parameters still relevant? I guess not

/** A set of parameters.
 * \ingroup utils
 * This object can be seen like a list of parameters, but whith some parameter-specific functions and data, like for example the global additive and proportional variablities, or the covariances.
 */
class ParameterSet: public Entity
{
    Q_OBJECT

    ENTITY_UTILS(ParameterSet)
    AUTO_PROPERTY_DECL(QDateTime, time, Time)
    AUTO_PROPERTY_DECL(Parameters*, parameters, Parameters)

public:

    //! Create a parameter set and define its type and model.
    ParameterSet(ParameterType type = POPULATION, const QString &model = "");

    ParameterSet(AbstractRepository *repository, QObject* parent);
    //! Create a parameter set from another parameter set.
    ParameterSet(AbstractRepository *repository, QObject* parent, ParameterSet* other);

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

    //! Parameter type of this set.
    ParameterType type () const;

    //! Set the parameter type of this set.
    void setType (ParameterType type);

    //! List of contained parameters ID.
    QStringList pids () const;

    //! Indicate if the parameter that corresponds to this ID exists in the set.
    bool contains(const QString &pid) const;

    //! Get the parameter that corresponds to this ID.
    SharedParameter get(const QString &pid) const;

    //! Set this parameter if it exists, add it otherwise
    void set(const SharedParameter &parameter);

    //! Copy the other set parameters into ourselves (remove the old ones)
    void copy (const ParameterSet* other);

    //! Remove all the parameters in this set, but keep the parameter type.
    void clear ();

    Duration duration() const {return _duration;}
    void setDuration(Duration duration) {_duration = duration;}

    //! Number of parameters contained within.
    int size () const;

    /** Set this correlation if it exists, add it otherwise
     * @param param1 The first parameter ID.
     * @param param2 The second parameter ID.
     * @param value The correlation value.
     * @return True if successfully added, otherwise false.
     */
    bool setCorrelation(QString param1, QString param2, const double &value);

    /** Get the value of the specified correlation, or 0 if it doesn't exist.
     * @param param1 The first parameter ID.
     * @param param2 The second parameter ID.
     * @return The corresponding correlation value, or 0 if it doesn't exist.
     */
    double correlation(QString param1, QString param2) const;

    bool setCorrelationComments(QString param1, QString param2, TranslatableString *comments);

    TranslatableString* correlationComments(QString param1, QString param2) const;

    /**
     * @brief ParameterSet::correlationsKeys returns the correlations' keys AKA the concatenation of `param1 "__" param2`
     * @return every correlations' keys
     */
    QStringList correlationsKeys() const;

    void copyCorrelationsFrom(ParameterSet &source);


/*
    //! Set the global additive and proportional variablities.
    void setGlobal (double additive, double proportional);

    //! The global additive variability.
    double globalAdditive () const;

    //! The global proportional variability.
    double globalProportional () const;
*/
    //! Get this parameter set model ID
    QString model() const;

    //! Set this parameter set model ID
    void setModel (const QString &model);

    //! Access the parameter through its ID.
    SharedParameter& operator[] (const QString &pid);

    //! Access the constant parameter through its ID.
    const SharedParameter operator[] (const QString &pid) const;

private:
    //The parameters type, eg. a posteriori
    ParameterType _type;

    Duration _duration;

    //The paramaters list
    QMap<QString,SharedParameter> _parametersmap;

    //The correlations list
    QMap<QString,double> _correlations;

    //The correlations comments list
    QMap<QString, TranslatableString*> _correlationsComments;

    //The global additive and proportional variablities
//    ErrorModel *_errorModel;

    //The model's id
    QString _model;
};

} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::core::ParameterType)
Q_DECLARE_METATYPE(QList<ezechiel::core::ParameterType*>)
Q_DECLARE_METATYPE(ezechiel::core::Parameter*)
Q_DECLARE_METATYPE(QList<ezechiel::core::Parameter*>)
Q_DECLARE_METATYPE(ezechiel::core::Parameters*);
Q_DECLARE_METATYPE(ezechiel::core::ParameterSet*)

#endif // PARAMETERS_H
