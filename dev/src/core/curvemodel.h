/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef CURVEMODEL_H
#define CURVEMODEL_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QString>
#include <QFlags>

#include "dbobjects.h"
#include "ezutils.h"
#include "units.h"
#include "../dal/percentile.h"
#include "../dal/dosage.h"
#include "../dal/measure.h"
#include "../dal/target.h"
#include "duration.h"
#include "dataset.h"
#include "../dal/covariate.h"
#include "../dal/clinical.h"
#include "../dal/parameters.h"
#include "../dal/residual.h"
#include "../dal/patient.h"
#include "drugmodel.h"


namespace Tucuxi {
namespace Gui {
namespace Core {

class DrugModelEngine;
class PercentileEngine;
class PostEngine;
class ReverseEngine;

/** Curve model and predictor.
 * \ingroup core
 *
 * The CurveModel is a super-predictor, able to handle all the time-based events to produce a valid set of points.
 * It really is an interface to a number of other Core object which it uses to work :
 *
 * \li The \ref pluginsmanager object for model/engines loading
 * \li The \ref DrugModel object to load the model part of the drug XML
 * \li The \ref DrugInfo object to load the possible list
 *
 * \section Usage
 * In order to be able to produce a set of points, some stuff must be set in order :
 * \li Create the CurveModel object (really???)
 * \li Set a model
 * \li Set a drug
 * \li Set the engines
 * \li Set a valid patient
 * \li Set a first dosage and the first drug take
 *
 * You are then ready to do a prediction/percentile/reverse prediction.
 *
 * \section Prediction
 * A model is able to predict a set of points between two dates, but using only the parameters it's given.
 * These parameters changes in time, when a covariate is changed.
 * The posology (dosage/interval) changes also in time, and is also a parameter for the models.
 * From this we can see that an object able to take in account all the posologies and covariates and create a real set of points between any dates.
 *
 * The residual value at the end of each cycle is calculated and stored for a futur use.
 * The model is given the current parameter, posology and residual value.
 * From this a part of the final set of points can be calculated.
 *
 * \section Percentiles
 * In addition of a simple prediction, the CurveModel object is able to create a list of percentile.
 * In the background, the \ref peengines itself might be using the CurveModel to produce some prediction for its internal calculations.
 *
 * \section Parameters
 * Parameters changes in time when the covariates are changed.
 * This fact is true only for the a-priori and a-posteriori parameter, beacause a-priori parameters uses the patient's covariates and the a-posteriori parameters uses the a-priori parameters.
 * The manual and population parameters are constant in time, and are used directly.
 * In case of a change of parameter type, the whole residual and parameter cache must be re-calculated.
 * A-priori and a-posteriori parameters calculation are handled directly by this object.
 *
 * \section cm_db Database
 * The CurveModel itself is able to load and save itself in the database.
 * To achieve that, it uses some curve-specific function defined in the \ref database
 *
 * What's saved is :
 * \li The curve name
 * \li The dosages for this curve
 * \li The engines and model ID
 * \li The drug ID
 * \li The type used (even if it's not used)
 * \li The linked patient (only it's ID)
 *
 * No points or result are saved in the database right now.
 *
 * \section Optimisations
 * A lot of optimisations can be done on this object, here is a list of them :
 *
 * \li Parameter cache : the parameters are saved for each time already calculated, and re-used on the next calculation
 * \li Residual cache : the residual values (concentration at the end of a cycle) are stored, and re-used on the next calculation
 *
 * Cache optimisations are relatively complex to use, and also to keep valid. Cache values becomes invalid in a number of cases, so their use must be well thought.
 *
 * \section Tests
 * To be sure (or relatively sure) that this object works as expected, a series of automated tests are used.
 * Build using cmake, the test-curvemodel uses different technics to try the CurveModel.
 *
 * Here is a (non-complete) list of tests done :
 * \li Create some simple datasets with no time change
 * \li Add different dosages
 * \li Create dataset from 1 point to 100 points
 * \li Data-test a large number of points regrouping a lot of different situation using a matlab model
 * \li Test all parameters type in predictions (population, a-priori, a-posteriori, manual)
 *
 * All devellopers working on the CurveModel or any object it uses MUST run this test before any push.
 * Yes, you heard me Yann, RUN THEM! (BNZ)
 */
class CurveModel : public QObject
{
    Q_OBJECT

public:

    //! This enum define the possible types of curve or prediction.
    enum CurveType {
        Continous,  //! A continuous curve is a curve that starts with an intial dosage (the first take) and an initial concentration of 0.
        Partial     //! A partial curve is a curve that starts directly at steady state given an initial dosage and observation date or measure.
    };

    /** \brief Returns a translated string for a given curve type.
     * @param type The curve's type.
     * @return The translated curve's type.
     */
    static QString translatedCurveType(CurveType type);


    /** Constructor.
     * Initialise some internal object (DrugInfo, DrugModel, etc).
     */
    CurveModel(QObject *parent = 0);

    /** Error string.
     * If an error occured, the specific error string can be fetched here.
     * @return The last error string, or an empty string if none
     */
    QString errorString () const;

    /** Clone this curve.
     * All the internal data will be saved, but the big internal objects are created anew.
     * The model/engines are cloned with their options.
     * @return A clone of this curve in a new object
     */
    CurveModel* clone () const;

    //-------------- DB STUFF ----------------

    /** Load the curve from the database.
     * After being loaded, a curve is ready to be used.
     * See the \ref cm_db section for more information.
     * @param id The curve id in the database
     * @return True if the curve has been loaded correctly, false otherwise
     * \sa errorString
     */
    bool load(ident id);

    /** Save the curve in the database.
     * If the curve wasn't in the database previously, it will be added there.
     * @return True if the curve has been save correctly, false otherwise
     * \sa errorString
     */
    bool save();

    /** The curve's database ID.
     * @return The database ID assigned to this curve, or invalid_ident if not already saved
     */
    ident id () const;

    //-------------- GETTERS ----------------

    //! Has this curve beeing modified since the last save?
    bool isModified () const;

    //! Has this curve being saved to the database?
    bool isSaved () const;

    //! Export the curve points.
    /** Export the curve points for a given range and interval between points. The first vector will
     * contain the time, if requested, the second the curve points, and the following vector(s) will
     * contain the percentile(s), if requested.
     * @param from Duration relative to the first take.
     * @param to Duration relative to the first take.
     * @param interval Interval between each points.
     * @param percentiles The list of percentiles to be exported.
     * @param time True if the time should be returned, relative to the first take.
     * @return A vector containing one vector for each set of data requested, or an empty vector if
     * an error occured.
     */
    QVector< QVector<double> > exportPoints(const Duration &from, const Duration &to, const Duration &interval, const PercentileList &percentiles, bool time = true);

    //! Export the curve points.
    /** This is an overloaded function using dates instead of duration for the exportation range.
     */
    QVector< QVector<double> > exportPoints(const QDateTime &from, const QDateTime &to, const Duration &interval, const PercentileList &percentiles, bool time = true);

    //! Export the curve points.
    /** This is an overloaded function using a number of points instead of an interval between each
     * points.
     */
    QVector< QVector<double> > exportPoints(const Duration &from, const Duration &to, int nbPoints, const PercentileList &percentiles, bool time = true);

    //! Export the curve points.
    /** This is an overloaded function using dates instead of duration for the exportation range, and
     * a number of points instead of an interval between each points.
     */
    QVector< QVector<double> > exportPoints(const QDateTime &from, const QDateTime &to, int nbPoints, const PercentileList &percentiles, bool time = true);

    /** Current model.
     * @return The currently used model, or 0 if not set
     */
    DrugModel* model () const;

    /** Current post engine.
     * @return The currently used post engine, or 0 if not set
     */
    PostEngine* postEngine () const;

    /** Current percentile engine.
     * @return The currently used percentile engine, or 0 if not set
     */
    PercentileEngine* percentileEngine () const;

    /** Current reverse engine.
     * @return The currently used reverse engine, or 0 if not set
     */
    ReverseEngine* reverseEngine () const;

    /** Current drug.
     * @return The currently used drug, or an empty Descriptor if not set
     */
    Descriptor drug() const;

    //! Returns the curve's doses default unit.
    /** Returns the curve's doses default unit, or an invalid unit if a problem occured.
     * This unit corresponds to the current drug's doses default unit.
     * @return The curve's doses default unit.
     */
    Unit doseUnit() const;

    //! Returns the curve's concentrations default unit.
    /** Returns the curve's concentrations default unit, or an invalid unit if a problem
     * occured. This unit corresponds to the current drug's concentrations default unit.
     * @return The curve's concentrations default unit.
     */
    Unit concentrationUnit() const;

    /** Current intake.
     * @return The current intake descriptor, or an invalid one if none is set.
     */
    Descriptor intake() const;

    /** Current patient.
     * @return The currently used patient, or an invalid one (id = invalid_ident) if not set
     */
    SharedPatient patient() const;

    /** Returns a list of measures.
     * Returns a list of measures in the given interval.
     * @param from Start of the interval relative to the first take.
     * @param to End of the interval relative to the first take.
     * @return A list of measure contained in the specified interval.
     */
    QList<SharedMeasure> measures (const Duration &from, const Duration &to) const;

    /** Returns the list of measures.
     * A complete list of the measures of the current curve. The measures taking place before the first take are ignored.
     * @return The list of measures.
     */
    QList<SharedMeasure> measures () const;

    //! Get the previous measure.
    /** Get the measure that comes right before the given duration, or an invalid measure
     * if there is none. The duration must be relative to the first take.
     * @param duration The duration relative to the first take.
     * @return The previous measure, or an invalid measure if there is none.
     */
    SharedMeasure previousMeasure(const Duration &duration) const;

    //! Get the previous measure.
    /** Get the measure that comes right before the given date, or an invalid measure if
     * there is none.
     * @param date The date.
     * @return The previous measure, or an invalid measure if there is none.
     */
    SharedMeasure previousMeasure(const QDateTime &date) const;

    //! Returns the application time of the previous measure.
    /** This function returns the time, or duration from the first take, of the measure
     * that takes place right before the given time. If an empty or negative duration
     * is given, the duration returned will be empty, corresponding to the curve initial
     * measure. The duration must be relative to the first take.
     * @param duration The duration relative to the first take.
     * @return The duration from the first take to the previous measure change.
     */
    Duration previousMeasureTime(const Duration &duration) const;

    //! Returns the application time of the previous measure.
    /** This function returns the time, or duration from the first take, of the measure
     * that takes place right before the given date. If the given date is equal or before
     * the first take, the duration returned will be empty, corresponding to the curve
     * initial measure.
     * @param date The date.
     * @return The duration from the first take to the previous measure change.
     */
    Duration previousMeasureTime(const QDateTime &date) const;

    /** Applied covariates.
     * This list contains only the used covariates, the ones before the curve's first take are not here.
     * @return The list of covariates
     */
    QList<PatientVariateSet*> covariates () const;

    //! Returns the covariates set currently used.
    /** Returns the covariates set currently used at the given duration. The duration must
     * be relative to the first take. If the duration is negative, an invalid covariates set
     * is returned.
     * @param duration The duration relative to the first take.
     * @return The covariates set currently used.
     */
    PatientVariateSet* currentCovariates(const Duration &duration) const;

    //! Returns the covariates set currently used.
    /** Returns the covariates set currently used at the given date. If the duration is negative,
     * an invalid covariates set is returned.
     * @param date The date.
     * @return The covariates set currently used.
     */
    PatientVariateSet* currentCovariates(const QDateTime &date) const;

    //! Returns the time of the covariates set currently used.
    /** Returns the time, or duration from the first take, of the covariates set currently
     * used at the given duration. If an empty or negative duration is given, the duration
     * returned will be empty, corresponding to the curve initial covariates set. The duration
     * must be relative to the first take.
     * @param duration The duration relative to the first take.
     * @return The time of the covariates set currently in use.
     */
    Duration currentCovariatesTime(const Duration &duration) const;

    /** Set of clinicals matching a given time
     * This function returns the clinicals set coming at or right before the given time.
     * @param time The given time
     * @return The corresponding set of clinicals
     */
    ClinicalSet clinicals(const Duration &time) const;

    /** Set of parameters applied at a given time.
     * At each covariate change, a new set of parameters is calculated.
     * Time function can be used to fetch a set of parameters applied at a given time
     * @param time The wanted time
     * @param type The parameter type wanted, if not specified, uses the currently used parameter type
     * \sa parameterType setParameterType
     */
    ParameterSet* parameters(const Duration &time, ParameterType type = LAST_PT);

    /** Set of parameters applied at a given date.
     * At each covariate change, a new set of parameters is calculated.
     * Time function can be used to fetch a set of parameters applied at a given date
     * @param time The wanted time
     * @param type The parameter type wanted, if not specified, uses the currently used parameter type
     * \sa parameterType setParameterType
     */
    ParameterSet* parameters(const QDateTime &date, ParameterType type = LAST_PT);

    //! Returns a subset of parameters sets.
    /** Returns a subset of parameters sets of the given type. The subset corresponds to the
     * parameters sets of the given type in the given range (inclusive). If an error occures
     * or if the range is invalid, an empty map is returned.
     * @param start The start time relative to the first take (inclusive).
     * @param end The stop time relative to the first take (inclusive).
     * @param type The type of the parameters.
     * @return The subset of parameters sets, or an empty map if an error occured.
     */
    QMap<Duration, ParameterSet*> parameters(const Duration &start, const Duration &end, ParameterType type);

    /** \brief Returns the translated curve's type.
     * @return The translated curve type.
     */
    QString translatedCurveType() const;

    /** \brief Returns the curve's type.
     * @return The curve's type
     */
    CurveType curveType() const;

    /** \brief Set the curve's type.
     * @param curveType The curve's type.
     */
    void setCurveType(const CurveType type);

    /** Current parameter type.
     * The current parameter type is not used in the calculation, where the parameter type to use is always asked.
     * It's just a value saved in the database and used informatively.
     * @return The current parameter type
     * \sa setParameterType
     */
    ParameterType parameterType () const;

    /** First drug take.
     * @return The current first drug take if set, or an invalid date if not
     * \sa setFirstTake
     */
    QDateTime firstTake () const;

    /** The curve name.
     * This name is used nowhere, but is saved and loaded in the database.
     * @return The current curve name
     * \sa setName
     */
    QString name () const;

    /** List of target currently in use.
     * These are loaded in the drug descriptor XML, and used when asking for some propositions.
     * @return The current list of target
     */
    QList<Target*> targets () const;

    /** Time of the next change.
     * A change is a dosage or a parameter change (triggered by a covariate change).
     * @param time From where the change is searched
     * \sa previousChange
     */
    Duration nextChange (const Duration &time) const;

    /** Time of the next covariate change.
     * Time from the first take to the next covariate change that takes place right after the given time.
     * If there is no next covariate, this function returns an empty duration.
     * @param time From where the covariate change is searched
     * @param current If true, a covariate at the current time is considered as the next covariate.
     * @return The duration from the first take to the next covariate change, or an empty duration if there is none
     * \sa nextChange
     */
    Duration nextCovariateChange (const Duration &time, bool current = false) const;

    /** Time of the next measure change.
    * Time from the first take to the measure that takes place right after the given time.
    * If there is no such measure, this function returns an empty duration.
    * @param time From where the next measure is searched.
    * @param current If true, a measure at the current time is considered as the next measure.
    * @return The duration from the first take to the next measure, or an empty duration if there is none.
    * \sa nextChange
    */
    Duration nextMeasureChange (const Duration &time, bool current = false) const;

    //! Returns the time of the next day.
    /** Returns the time of the next day, relative to the first take.
     * @param time The time from where the next day is searched.
     * @return The next day time.
     */
    Duration nextDayTime(const Duration &time) const;

    //! Returns the time of the current day.
    /** Returns the time of the current day, relative to the first take.
     * @param time The time from where the current day is searched.
     * @return The current day time.
     */
    Duration currentDayTime(const Duration &time) const;

    /** Time of the previous change.
     * A change is a dosage or a parameter change (triggered by a covariate change).
     * @param time From where the change is searched (backward)
     * @return The duration from the first take to the previous change
     * \sa nextChange
     */
    Duration previousChange (const Duration &time) const;

    /** Time of the previous covariate change.
     * Time from the first take to the previous covariate change that takes place right before or at the given time.
     * @param time From where the covariate change is searched
     * @return The duration from the first take to the previous covariate change
     * \sa previousChange
     */
    Duration prevCovariateChange (const Duration &time) const;

    //! Returns the time of the last significant change.
    /** Returns the time of the last significant change made to the curve. The last change time corresponds to the
     * last covariate change time or the last dosage change time, whichever of the two is the latest. The duration
     * returned is relative to the first take.
     * @return The duration from the first take to the last change made to the curve.
     */
    Duration lastChangeTime() const;

    //! Returns the application time of the last covariate.
    /** Returns the application time, or duration from the first take, of the last covariate change.
     * @return The duration from the first take to the last covariate change.
     */
    Duration lastCovariateTime() const;

    //! Returns the application time of the last measure.
    /** Returns the application time, or duration from the first take, of the last measure.
     * @return The duration from the first take to the last measure.
     */
    Duration lastMeasureTime() const;

    /** Returns the time of the next cycle begining.
     * If the provided duration does already match the begining of a cycle, the functions returns that same duration.
     * @param time The duration from where to look for the next cycle begining.
     * @return The duration of the next cycle begining.
     */
    Duration nextCycle (const Duration &time) const;

    /** Returns the time of the next cycle begining.
     * If the provided duration does already match the begining of a cycle, the functions returns that same duration.
     * @param date The date from which to look for the next cycle begining.
     * @return The duration of the next cycle begining.
     */
    Duration nextCycle (const QDateTime &date) const;

    /** Returns the time of the previous cycle begining.
     * If the provided duration does already match the begining of a cycle, the functions returns that same duration.
     * @param time The duration from where to look for the previous cycle begining.
     * @param dosage An optional temporary dosage used by the reverse engines.
     * @return The duration of the previous cycle begining.
     */
    Duration prevCycle (const Duration &time, const SharedDosage &dosage = SharedDosage(new Dosage())) const;

    /** Returns the time of the previous cycle begining.
     * If the provided duration does already match the begining of a cycle, the functions returns that same duration.
     * @param date The date from which to look for the previous cycle begining.
     * @param dosage An optional temporary dosage used by the reverse engines.
     * @return The duration of the previous cycle begining.
     */
    Duration prevCycle (const QDateTime &date, const SharedDosage &dosage = SharedDosage(new Dosage())) const;

    /** Returns true if a postnatal age is a covariate.
     * @return True if there is a postnatal age covariate (the name being "pna")
     */
    bool withPna();

    //-------------- DOSAGES ----------------

    //! Adds or replaces a dosage.
    /** This function will add a new dosage to the curve, or replace a previous dosage. The
     * dosage must be valid or it won't be added. The residuals and paramters caches are
     * cleared after the dosage is set.
     * @param dosage The new dosage.
     * @return Returns true if the dosage was correctly set, otherwise returns false.
     */
    //bool setDosage(const SharedDosage &dosage);

    //! Removes a dosage.
    /** This function will remove an existing dosage from the curve. The dosage sent to the
     * function must be valid and match an existing dosage, otherwise the function returns
     * false. The residuals and parameters caches are cleared after the dosage is removed.
     * @param dosage The dosage to remove.
     * @return Returns true if the dosage was correctly removed, otherwise returns false.
     */
    //bool removeDosage(const SharedDosage &dosage);

    //! Checks if the first dosage is identical to the second dosage.
    /** This function checks if the first dosage is a duplicate of the second dosage. To
     * be considered as a duplicate, the two dosages must be identical, apart from the
     * applied date, and the least recent dosage's applied date must corresponds to the
     * end of a cycle of the first one. The function will return false if the two dosages
     * are identical and have the same applied date.
     * @param first The first dosage.
     * @param second The second dosage.
     * @return Returns true if the two dosages are identical.
     */
    static bool isIdentical(const SharedDosage &first, const SharedDosage &second);

    //! Add a new dosage.
    /** Add a new dosage to the curve. If the dosage is not different than the previous
     * dosage, meaning it doesn't change the curve in any way, the dosage is ignored.
     * The new dosage can be set before the current first take, in which case it will
     * update it. If the dosage applied date matches an existing dosage, it will replace
     * that dosage. Finally, if the next dosage is similar to the new dosage, it will be
     * removed.
     * @param dosage The new dosage.
     * @return True if the dosage was successfully added, and false if the dosage was
     * invalid, ignored or if an error occured.
     */
    bool addDosage(const SharedDosage &dosage);

    //! Remove a dosage.
    /** Remove an existing dosage from the curve. If the dosage is different from
     * the dosage effectively stored in the curve at the same date, not found, invalid,
     * or is the last dosage of the curve, the function will do nothing and return false.
     * The function will also remove the next dosage if it is identical to the previous
     * dosage.
     * @param dosage The dosage to be removed.
     * @return True if the dosage was successfully removed, otherwise false.
     */
    bool removeDosage(const SharedDosage &dosage);

    //! Modify a dosage.
    /** Modify an existing dosage. If the dosage specified in the initial parameter
     * is different from the dosage effectively stored in the curve at the same date,
     * not found or invalid, the function will do nothing and return false. Please
     * note that this function will call addDosage() or setDosages depending on the
     * situation to apply the modifications, and therefore follow the same logic ex-
     * plained in those two functions documentation.
     * @param initial An existing dosage.
     * @param modified The modified dosage.
     * @return True if the dosage was successfully edited, otherwise false.
     */
    bool modifyDosage(const SharedDosage &initial, const SharedDosage &modified);

    //! Sets the dosages of this curve.
    /** This function will set the dosages of this curve. The previous dosages will be
     * replaced by the dosages contained in the given list. The first take will be modifed
     * accordingly, and dosages identical to their previous dosage will be ignored. If the
     * list contains an invalid dosage, the whole operation will be aborded and the function
     * will return false.
     * @param dosages The list of dosages, including the first take.
     * @return Returns true if the dosages were successfully added to the curve, otherwise
     * returns false.
     */
    bool setDosages(const QList<SharedDosage> &dosages);

    //! Returns the dosages list.
    /** This function returns all the dosages tied to this curve.
     * @return The list of dosages.
     */
    QList<SharedDosage> dosages() const;

    //! Returns the dosage applied at the given time.
    /** This function returns the dosage applied at the given time, or an invalid dosage
     * if there is none. The duration must be relative to the first take.
     * @param duration The duration relative to the first take.
     * @return The dosage applied at the given time.
     */
    SharedDosage dosage(const Duration &duration) const;

    //! Returns the dosage applied at the given date.
    /** This function returns the dosage applied at the given date, or an invalid dosage
     * if there is none.
     * @param duration The date.
     * @return The dosage applied at the given date.
     */
    SharedDosage dosage(const QDateTime &date) const;

    //! Returns the dosage currently used at the given duration.
    /** This function returns the dosage applied at or right before the given duration.
     * It corresponds to the dosage currently in use at the given time. The duration
     * must be relative to the first take. If the duration is negative, an invalid dosage
     * is returned.
     * @param duration The duration relative to the first take.
     * @return The dosage currently in use.
     */
    SharedDosage currentDosage(const Duration &duration) const;

    //! Returns the dosage currently used at the given date.
    /** This function returns the dosage applied at or right before the given date. It
     * corresponds to the dosage currently in use at the given date. If the date comes
     * before the first take, an invalid dosage is returned.
     * @param date The date.
     * @return The dosage currently in use.
     */
    SharedDosage currentDosage(const QDateTime &date) const;

    //! Returns the application time of the dosage currently used at the given duration.
    /** This function returns the time, or duration from the first take, of the dosage
     * applied at or right before the given duration. It corresponds to the application
     * time of the dosage currently in use at the given time. If an empty or negative
     * duration is given, the duration returned will be empty, corresponding to the curve
     * initial dosage. The duration must be relative to the first take.
     * @param duration The duration relative to the first take.
     * @return The application time of the dosage currently in use.
     */
    Duration currentDosageTime(const Duration &duration) const;

    //! Returns the application time of the dosage currently used at the given date.
    /** This function returns the time, or duration from the first take, of the dosage
     * applied at or right before the given date. It corresponds to the application time
     * of the dosage currently in use at the given date. If the date comes before the
     * take,  the duration returned will be empty, corresponding to the curve initial
     * dosage.
     * @param duration The duration relative to the first take.
     * @return The dosage currently in use.
     */
    Duration currentDosageTime(const QDateTime &date) const;

    //! Get the next dosage.
    /** Get the dosage that comes right after the given duration, or an invalid dosage
     * if there is none. The duration must be relative to the first take.
     * @param duration The duration relative to the first take.
     * @return The next dosage, or an invalid dosage if there is none.
     */
    SharedDosage nextDosage(const Duration &duration) const;

    //! Get the next dosage.
    /** Get the dosage that comes right after the given date, or an invalid dosage if
     * there is none.
     * @param date The date.
     * @return The next dosage, or an invalid dosage if there is none.
     */
    SharedDosage nextDosage(const QDateTime &date) const;

    //! Returns the application time of the next dosage.
    /** This function returns the time, or duration from the first take, of the dosage
     * that takes place right after the given time. If there is no such dosage, this
     * function returns an empty duration. The duration must be relative to the first take.
     * @param duration The duration relative to the first take.
     * @return The duration from the first take to the next dosage change, or an empty
     * duration if there is none.
     */
    Duration nextDosageTime(const Duration &duration) const;

    //! Returns the application time of the next dosage.
    /** This function returns the time, or duration from the first take, of the dosage
     * that takes place right after the given date. If there is no such dosage, this
     * function returns an empty duration.
     * @param date The date.
     * @return The duration from the first take to the next dosage change, or an empty
     * duration if there is none.
     */
    Duration nextDosageTime(const QDateTime &date) const;

    //! Get the previous dosage.
    /** Get the dosage that comes right before the given duration, or an invalid dosage
     * if there is none. The duration must be relative to the first take.
     * @param duration The duration relative to the first take.
     * @return The previous dosage, or an invalid dosage if there is none.
     */
    SharedDosage previousDosage(const Duration &duration) const;

    //! Get the previous dosage.
    /** Get the dosage that comes right before the given date, or an invalid dosage if
     * there is none.
     * @param date The date.
     * @return The previous dosage, or an invalid dosage if there is none.
     */
    SharedDosage previousDosage(const QDateTime &date) const;

    //! Returns the application time of the previous dosage.
    /** This function returns the time, or duration from the first take, of the dosage
     * that takes place right before the given time. If an empty or negative duration
     * is given, the duration returned will be empty, corresponding to the curve initial
     * dosage. The duration must be relative to the first take.
     * @param duration The duration relative to the first take.
     * @return The duration from the first take to the previous dosage change.
     */
    Duration previousDosageTime(const Duration &duration) const;

    //! Returns the application time of the previous dosage.
    /** This function returns the time, or duration from the first take, of the dosage
     * that takes place right before the given date. If the given date is equal or before
     * the first take, the duration returned will be empty, corresponding to the curve
     * initial dosage.
     * @param date The date.
     * @return The duration from the first take to the previous dosage change.
     */
    Duration previousDosageTime(const QDateTime &date) const;

    //! Returns the last dosage.
    /** Returns the last dosage, or an invalid dosage if there is none.
     * @return The last dosage.
     */
    SharedDosage lastDosage() const;

    //! Returns the application time of the last dosage.
    /** Returns the application time, or duration from the first take, of the last dosage.
     * @return The duration from the first take to the last dosage change.
     */
    Duration lastDosageTime() const;

    //! Checks if the dosage is a duplicate.
    /** This function checks if the dosage passed to the function is a duplicate of an
     * existing dosage.
     * @param dosage The dosage to check.
     * @return Returns true if the dosage is a duplicate, otherwise returns false.
     */
    bool isDuplicate(const SharedDosage &dosage) const;

    //! Checks if the dosage is a duplicate of the next dosage.
    /** This function checks if the dosage passed to the function is a duplicate of the
     * next dosage. To be considered as a duplicate, the two dosages must be identical,
     * apart from the applied date, and the next dosage applied date must corresponds to
     * the end of a cycle. The function does not check if a duplicate exists at the same
     * date.
     * @param dosage The dosage to check.
     * @return Returns true if the next dosage is identical, otherwise returns false.
     */
    bool isNextIdentical(const SharedDosage &dosage) const;

    //! Checks if the dosage is a duplicate of the previous dosage.
    /** This function checks if the dosage passed to the function is a duplicate of the
     * previous dosage. To be considered as a duplicate, the two dosages must be identical,
     * apart from the applied date, and the given dosage applied date must corresponds to
     * the end of a cycle. The function does not check if a duplicate exists at the same
     * date.
     * @param dosage The dosage to check.
     * @return Returns true if the previous dosage is identical, otherwise returns false.
     */
    bool isPreviousIdentical(const SharedDosage &dosage) const;

public slots:

    //-------------- CALCULATIONS ------------

    /** Predict a set of points.
     * Some checks are done on the state of the CurveModel, and the set of points is calculated.
     * If the result is an empty DataSet, then something went wrong.
     * The parameter and residual cache will be updated.
     *
     * When asking only one point, the value corresponding to the stop time is given.
     * Else, the first value will be the one at start, and the last one the one at stop.
     *
     * @param start Start time for the prediction
     * @param stop Stop time of the prediction
     * @param points Number of points to calculate
     * @param type Parameter type to use
     * @param dosage If given, use this dosage instead of the one specified in the internal list
     * @param uniform If true, ensures that the delta between each point is uniform.
     * @return The set of points calculated, or an empty set if something went wrong
     * \sa point percentiles errorString
     */
    DataSet points ( const Duration &start,
                     const Duration &stop,
                     uint points,
                     ParameterType type,
                     const SharedDosage &dosage = SharedDosage(new Dosage()),
                     bool uniform = false);

    /** Predict an unique point.
     * Simple helper for the points() function.
     * @param moment Time where the point is calculated
     * @param type Parameter type to use
     * @param dosage If given, use this dosage instead of the one specified in the internal list
     * @return The point calculated, or an empty one if something went wrong
     * \sa points percentiles errorString
     */
    Amount point ( const Duration &moment,
                   ParameterType type,
                   const SharedDosage &dosage = SharedDosage(new Dosage()) );

    /** Returns the peak amount in a given interval.
     * @param time The time of the peak relative to the given start.
     * @param start The interval start time relative to the first take.
     * @param stop The interval stop time relative to the first take.
     * @param points Number of points to calculate in the given interval (at least 10 points)
     * @param type The parameter type to use for the calculation.
     * @return The peak, or an empty amount if something went wrong.
     */
    Amount peak ( Duration &peakTime,
                  const Duration &start,
                  const Duration &stop,
                  uint points,
                  ParameterType type );

    /** Calculate some percentile.
     * A list of percentile is used instead of a single value to optimise the background calculation for some \ref peengines
     * @param values A list of percentiles value do calculate (eg: 5, 95, 10, 90)
     * @param start Start time for the percentile
     * @param stop Stop time of the percentile
     * @param points Number of points to calculate
     * @param type Parameter type to use
     * @return The percentiles calculated in the form <percentile value, percentile points>, or an empty map if something went wrong
     * \sa points point errorString
     */
    QMap<int,DataSet> percentiles ( const QList<int> &values,
                                    const Duration &start,
                                    const Duration &stop,
                                    uint points,
                                    ParameterType type,
                                    bool uniform = false);

    /** Calculate the best dosage.
     * This function uses a \ref reengines to calculate this map of dosage.
     * The returned map corresponds to list of the best dosages found, each with an associated score.
     * The score should between 0 and 100 included.
     *
     * In the backgroud, the targets are used along with the possible intervals defined in the Definitions object and the possible dosage found in the drug description XML.
     *
     * @param start Where the dosage should change for the proposed values
     * @param type Parameter type to use
     * @param nbIntakes The number of intakes for which the target(s) must be reached. If 0 is passed, the number of intakes necessary to reach the steady-state will be used.
     * @param targets The targets to use, and if an empty list is given, the default ones
     * @return A map of the best dosages found, each with their score, or an empty map if something went wrong
     * \sa targets Definitions::intervals DrugModel::dosages
     */
    QMap<int,SharedDosage> reverseDosages (const Duration &start,
                                     ParameterType type,
                                     int nbIntakes,
                                     const QList<Target*> targets = QList<Target*>());

    /** Half-life of the drug.
     *
     * When it's half your life, half of your life is done.
     * The other half thou should start then.
     * Is the life half done or half to do?
     * Half to do says the optimist.
     * Half done says the pessimist.
     * Twice too short says the engineer.
     *
     * Thank you, I'll be here all evenings, thank you all!
     *
     * "Rise and shine, Mister Freeman, rise and... shine."
     * "Not that I wish... to imply that you have been sleeping on... the job."
     * "No one is more deserving of a rest, and all the effort in the world would have gone to waste until... well... let's just say your hour has come again."
     * "The right man in the wrong place can make all the difference... in the world."
     * "So, wake up, Mister Freeman...wake up and... smell the ashes."
     *
     * Gordon Freeman, you're the best!
     *
     * @param start From where the half-life should be searched
     * @param type Parameter type to use
     * @param dosage If given, use this dosage instead of the one specified in the internal list
     * @return The half life following the given parameters
     * \sa steadyState
     */
    Duration halfLife ( const Duration &start,
                        ParameterType type,
                        const SharedDosage &dosage = SharedDosage(new Dosage()) );

    /** Time of the steady state.
     * The steady state is the duration from the first take at which the curve is considered to be stable
     * (or in other words, when the residuals variations are negligible from cycle to cycle). If a negative
     * start time is given, the start time is set to the last dosage or covariate change.
     *
     * If, for whatever reason, the steady state cannot be found, an emtpy duration is returned.
     *
     * @param start From where the steady state should be searched.
     * @param type Parameter type to use.
     * @param dosage If given, use this dosage instead of the one specified in the internal list.
     * @return The steady state time, or an empty duration if the steady state cannot be found.
     *
     * \sa halfLife
     */
    Duration steadyState(const Duration &start, ParameterType type, const SharedDosage &dosage = SharedDosage(new Dosage()));

    //-------------- SETTERS ----------------

    /** Set the drug to use.
     * The drug will be loaded in the \ref PluginsManager
     * All its values will be applied to the current model.
     * The given drug must be compatible with the current model.
     * A model is not needed for a drug to be loaded.
     * All cache will be cleared.
     *
     * A basic optimisation is used to avoid to reload a same drug twice.
     * Use the force paramater to bypass the optimisation.
     *
     * @param drug The drug ID of the drug to load
     * @return True if everything went fine, false otherwise
     * \sa drug errorString
     */
    bool setDrug(const QString &drug);

    /** Set the patient.
     * All cache will be cleared, and the covariates/measures will be re-loaded.
     * The patient itself is loaded form the database directly.
     *
     * @param ident The patient ID (must be valid)
     * @return True if everything went fine, false otherwise
     * \sa patient
     */
    bool setPatient(ident patient);
    bool setPatient(SharedPatient patient);

    /** Set the first drug take date.
     * This value is not automatically loaded from the first dosage.
     * But maybe it should be? (BNZ)
     * All the cache will be cleared.
     *
     * @param firstTake The date of the first drug take
     * \sa firstTake
     */
    void setFirstTake (const QDateTime &firstTake);

    /** Set the model ID.
     * The model will be loaded in the \ref PluginsManager
     * All the cache will be cleared.
     *
     * @param model The model ID to load
     * @return True if everything went fine, false otherwise
     * \sa model setModel(Model) errorString
     */
    bool setModel(const QString &model);

    /** Set the model.
     * This model will be directly used.
     * All cache will be cleared.
     *
     * @param model The model to use
     * \sa model setModel(QString)
     */
    void setModel(DrugModel *model);

    /** Set the percentile engine ID.
     * The percentile engine will be loaded in the \ref PluginsManager
     *
     * @param pe The percentile engine ID to load
     * @return True if everything went fine, false otherwise
     * \sa percentileEngine setPercentileEngine(PercentileEngine) errorString
     */
    bool setPercentileEngine (const QString &pe);

    /** Set the percentile engine.
     * This percentile engine will be directly used.
     * All cache will be cleared.
     *
     * @param engine The percentile engine to use
     * \sa percentileEngine setPercentileEngine(QString)
     */
    void setPercentileEngine (PercentileEngine *engine);

    /** Set the post engine ID.
     * The post engine will be loaded in the \ref PluginsManager
     * All the cache will be cleared.
     *
     * @param model The post engine ID to load
     * @return True if everything went fine, false otherwise
     * \sa postEngine setPostEngine(PostEngine) errorString
     */
    bool setPostEngine (const QString &po);

    /** Set the post engine.
     * This post engine will be directly used.
     * All cache will be cleared.
     *
     * @param engine The post engine to use
     * \sa postEngine setPostEngine(QString)
     */
    void setPostEngine (PostEngine *engine);

    /** Set the reverse engine ID.
     * The reverse engine will be loaded in the \ref PluginsManager
     *
     * @param model The reverse engine ID to load
     * @return True if everything went fine, false otherwise
     * \sa reverseEngine setReverseEngine(ReverseEngine) errorString
     */
    bool setReverseEngine (const QString &re);

    /** Set the reverse engine.
     * This reverse engine will be directly used.
     *
     * @param engine The reverse engine to use
     * \sa reverseEngine setReverseEngine(QString)
     */
    void setReverseEngine (ReverseEngine *engine);

    /** Set the parameters.
     * This function allow to set a QMap of parameters sets and their associated duration.
     * Only the manual or temporary parameters can be set, the others (population,a-priori,a-posteriori) are calculated.
     * The cache for the corresponding type is also cleared.
     * @param parameters A QMap of parameters sets and their associated duration
     * @param type The type of these parameters
     * \sa parameters setParameters
     */
    void setParameters (const QMap<Duration, ParameterSet*> &parameters, ParameterType type);

    /** Set the parameters.
     * This function allows to set a parameter set.
     * Only the manual or temporary parameters can be set, the others (population,a-priori,a-posteriori) are calculated.
     * Single Manual and temporary parameters set are not time sensitive, and added with a null duration.
     * The cache for the corresponding type is also cleared.
     * @param parameters The set of parameters to set (Mouahaha!)
     * @param type The type of these parameters
     * @param duration The duration from which the cache will be reseted
     * \sa parameters setParameters
     */
    void setParameters (const ParameterSet* parameters, ParameterType type, Duration start = Duration());

    /** Set the curve name.
     * This value is only informative, it's not used anywhere.
     * @param name The curve new name
     * \sa name
     */
    void setName (const QString &name);

    /** Set the current parameter type.
     * This value is not used for calculation, only for... well, I didn't know where else to put it.
     * @param pType The new parameter type
     * \sa parameterType
     */
    void setParameterType (ParameterType pType);

    //-------------- EVENTS ----------------

    /** The covariates have changed.
     * As the covariates resides in the database and are not curve-dependant, they are not stored internally.
     * If they changed, this function must be called for them to be reloaded.
     * \sa measureChanged
     */
    void covariatesChanged ();

    /** The clinicals have changed.
     * As the clinicals resides in the database and are not curve-dependant, they are not stored internally.
     * If they changed, this function must be called for them to be reloaded.
     * \sa measureChanged
     */
    void clinicalsChanged ();

    /** The measures have changed.
     * As for the covariates, the measures are not in stored internally, but in the database as they are patient-dependant but not curve-dependant.
     * If they changed, this function must be called for them to be reloaded.
     * \sa covariatesChanged
     */
    void measuresChanged ();

    //-------------- TESTS ----------------

    /** Reset the cache from this point.
     * All the cache (parameters and residual) will be erased.
     * Althought this should have no consequence on the result, this function should not be used for things other than test or internal use.
     * @param time The time from which the cache must be resetted
     */
    void resetCache (const Duration &time = Duration());

signals:

    /** The current curve calculation has progressed.
     * BNZ : not implemented right now, just to make you wish it was
     * @param percent The current percent (%) of progression
     */
    void progress(int percent);

    /** The curve has been modified.
     * This signal is emitted when any data having an impact on the curve's prediction changed, whether it is
     * specific to the curve or more global like the measures or the covariates. The signal is always sent
     * with a boolean parameter set to 'true' as a facility for interfacing with GUI slots.
     */
    void modified(bool = true);

    /** The curve's data has been changed.
     * This signal is emitted when data specific to the curve has changed. It is mostly used to signal that the
     * curve can be or needs to be saved, and it also emit the modified signal. It is always sent with a boolean
     * parameter set to 'true' as a facility for interfacing with GUI slots. Finally, the modified signal is
     * always emitted when emitting the changed signal.
     */
    void changed(bool = true);

    /** The curve has been saved.
     * This signal is emitted when the curve is saved to the database. It is always sent with a boolean parameter
     * set to 'true' as a facility for interfacing with GUI slots.
     */
    void saved(bool = true);

    /** The curve's dosages have been modified.
     * Called when the patient's dosages have been fetch from the database or the first take has changed.
     */
    void dosagesModified();

    /** The patient's covariates have been modified.
     * Only the covariates used by the curve are fetch (after the first take).
     */
    void covariatesModified();

    /** The patient's measure have been reloaded.
     * The measures are loaded from the database, this signal is emitted when it's done.
     */
    void measuresModified();

    /** The patient's clinicals have been reloaded.
     * The clinicals are loaded from the database, this signal is emitted when it's done.
     */
    void clinicalsModified();

    /** The curve's type has been modified.
     * This signal is emitted when the type of the curve has been modified.
     */
    void curveTypeChanged(CurveType type);

    /** The curve's parameters type has been modified.
     * This signal is emitted when the parameters type of the curve has been modified.
     */
    void parameterTypeChanged(ParameterType type);

    /** The first take has changed.
     * @param firstTake The new first take
     * \sa setFirstTake firstTake
     */
    void firstTakeModified(QDateTime firstTake);

    /** An error occured
     * The string can also be fetch using errorString().
     * @param errorString The error explaination
     * \sa errorString
     */
    void error(QString errorString);

    /** Starts a huge calculation
     * Indicates that a calculation requiring some time is starting.
     * It will allow the display to act accordingly (update of the
     * cursor, for instance).
     */
    void startHugeCalculation();

    /** Ends a huge calculation
     * Indicates that a calculation requiring some time is finished.
     * It will allow the display to act accordingly (update of the
     * cursor, for instance).
     */
    void endHugeCalculation();

    /** The curve's name has changed.
     * This signal is emitted when the name of the curve has been modified.
     * @param name The curve's name
     */
    void nameChanged(const QString &name);

    /** The curve's patient has changed.
     * This signal is emitted when the patient associated to this curve has changed.
     * @param patientId The patient ID
     */
    void patientChanged(ident patientId);

    /** The curve's drug has changed.
     * This signal is emitted when the drug associated to this curve has changed.
     * @param drugId The drug ID
     */
    void drugChanged(const QString &drugId);

    /** The curve's model has changed.
     * This signal is emitted when the curve model has changed.
     * @param modelId The model ID
     */
    void modelChanged(const QString &modelId);

    /** The curve's percentile engine has changed.
     * This signal is emitted when the curve percentile engine has changed.
     * @param engineId The engine ID
     */
    void percentileEngineChanged(const QString &engineId);

    /** The curve's post engine has changed.
     * This signal is emitted when the curve post engine has changed.
     * @param engineId The engine ID
     */
    void postEngineChanged(const QString &engineId);

    /** The curve's reverse engine has changed.
     * This signal is emitted when the curve reverse engine has changed.
     * @param engineId The engine ID
     */
    void reverseEngineChanged(const QString &engineId);

private slots:

    //Used to fetch the emitted error
    void setError (QString str);

    //Called when the curve was modified
    void setModified();

    //Called when the curve was saved
    void setSaved();

private:
    //The settings names
    static const char *const _stgSteadyStateTolerance;
    static const char *const _stgSteadyStateDuration;

    //-------------- TYPES --------------

    enum ObjectType {
        MODEL,
        REVERSE,
        PO,
        PE
    };

    //-------------- CALCULATIONS -------------

    DataSet calculate (const Duration &start,
                       const Duration &stop,
                       uint points,
                       ParameterType type,
                       bool cacheMode = false,
                       const SharedDosage &dosage = SharedDosage(new Dosage()),
                       bool uniform = false);

    void calculateCache (ParameterType type, const Duration &until);

    void calculateInitialResidual(const ParameterType &type, bool uniform = false);

    void calculateParameters (ParameterType type, const Duration &until);
    void calculateAprioriParam(const Duration &until);
    void calculateAposterioriParam(const Duration &until);

    //-------------- ENGINE HELPERS -----------

    //Get, set and delete the model options
    bool getModelOptions();
    bool setModelOptions();
    bool deleteModelsOptions();
    bool getPEOptions();
    bool setPEOptions();
    bool deletePEsOptions();
    bool getPOOptions();
    bool setPOOptions();
    bool deletePOsOptions();
    bool getREOptions();
    bool setREOptions();
    bool deleteREsOptions();

    //-------------- UTILITIES ----------------

    ParameterSet* parametersAtTime (const Duration &time, ParameterType type) const;

    SharedDosage dosageAtTime (const Duration &time, const SharedDosage defaultDosage = SharedDosage(new Dosage())) const;

    void updateDosages(const QDateTime &newDate);

    Duration lastResidual (const Duration &time, ParameterType type) const;

    Duration lastCovariates (const Duration &time) const;

    void resetCache (ParameterType type, const Duration &time = Duration());

    bool checkObjects (ObjectType toCheck);

    bool isPnaValid(const Duration &dayToTest);

    void invalidatePostEngine();

    uint uniformPoints(uint points, const Duration &start, const Duration &stop) const;

    //-------------- DATA ---------------------

    Duration _cacheValidUntil[LAST_PT];
    Duration _parametersValidUntil[LAST_PT];

    QMap<Duration, Residuals> _residuals[LAST_PT];
    QMap<Duration, ParameterSet*> _parameters[LAST_PT];
    QMap<Duration, PatientVariateSet*> _covariates;
    QMap<Duration, ClinicalSet> _clinicals;
    QMap<Duration, SharedDosage> _dosages;
    QMap<Duration, SharedMeasure> _measures;
    QList<Duration> _steadyStates[LAST_PT];
    Residuals _initialResidual[LAST_PT];
    QDateTime _firstTake;

    //Number of days separing the first take and the birth
    Duration _daysFromBirthToFirstTake;

    //-------------- THE REST -----------------

    //The drug ID
    QString _drugId;

    //The curve properties
    DrugModel *_model;
    PercentileEngine *_pe;
    PostEngine *_po;
    ReverseEngine *_re;
    SharedPatient _patient;
    QString _name;
    ParameterType _pType;
    CurveType _type;

    //Internal stuff
    QString _errors;
    ident _id;
    bool _modified;
    bool _poInvalidated;
    
    //The steady state tolerance
	double _tolerance;

    //The steady state maximum duration
    Duration _maxDuration;

	//Last model that asked for the Pna field
    QString _lastModelThatAskedForPna;

    //Last answered value by the function withPna()
    bool _lastAnsweredWithPna;
};

} // namespace Core
{ // namespace Gui
} // namespace Tucuxi

#endif // CURVEMODEL_H
