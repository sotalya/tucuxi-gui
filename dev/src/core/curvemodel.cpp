//@@license@@

#include "curvemodel.h"

#include "drugmodelengine.h"
#include "percentileengine.h"
#include "postengine.h"
#include "reverseengine.h"
#include "database.h"
#include "core.h"
#include "core_errors.h"
#include "coremessagehandler.h"
#include "pluginmanager.h"
#include "drugmanager.h"
#include "drug.h"
#include "settings.h"
#include "definitionmanager.h"
#include "amount.h"

#include <QDebug>

#include <cmath>

namespace Tucuxi {
namespace GuiCore {


#define PNA_MAX 100

//The settings names
const char *const CurveModel::_stgSteadyStateTolerance = "steadyStateTolerance";
const char *const CurveModel::_stgSteadyStateDuration  = "steadyStateDuration";

//Returns a translated curve type
QString CurveModel::translatedCurveType(CurveModel::CurveType type)
{
    switch (type) {
    case Continous:
        return tr("Continuous");
    case Partial:
        return tr("Steady state");
    default:
        return tr("Unknown or invalid curve type");
    }
}

//Reset the cache validity to 0
CurveModel::CurveModel(QObject *parent) :
    QObject(parent),
    _model(0),
    _po(0),
    _pe(0),
    _re(0),
    _id(invalid_ident),
    _type(Continous),
    _pType(APOSTERIORI),
    _modified(true),
    _poInvalidated(true)
{
    //Reset the cache validity (Theorically useless, as a Duration starts at 0, but I like it here)
    resetCache();

    //Our patient is invalid right now
    _patient = SharedPatient();

    // ToDo ///////////////////////////////////////////////////////////////////////////////
    // The declaration below should be global, and not be done in each curve model instance
    // ////////////////////////////////////////////////////////////////////////////////////

    //Load the steady state tolerance and duration
    _tolerance   = SETTINGS.get(Module::Core, metaObject()->className(), _stgSteadyStateTolerance, QVariant(0.01)).toDouble();
    _maxDuration = SETTINGS.get(Module::Core, metaObject()->className(), _stgSteadyStateDuration, QVariant::fromValue(Duration(365 * 24))).value<Duration>();

    //Connection managing the error
    connect(this, SIGNAL(error(QString)), this, SLOT(setError(QString)));

    //Connections managing a change of state
    connect(this, SIGNAL(changed()), this, SLOT(setModified()));
    connect(this, SIGNAL(saved()), this, SLOT(setSaved()));

    //Always emit the modified signal when changed is emitted
    connect(this, SIGNAL(changed()), this, SIGNAL(modified()));
}

QString CurveModel::errorString() const
{
    return _errors;
}

//Create a new one and copy its data, except the cache
CurveModel *CurveModel::clone() const
{
    //Create a new curve model
    CurveModel *other = new CurveModel();

    //Copy the general information
    other->_name = _name;
    other->_pType = _pType;
    other->_firstTake = _firstTake;

    //Set the model
    other->setModel(_model->clone());

    // ToDo ////////////////////////////////////////////////////////////////////////////////////////////////////
    // We cannot use the setDrug() function of curve model because it will reset the model's option. The problem
    // Is the same when loading a curve. An possible fix would be to add a parameter to the setDrug() function so
    // that it's possible to prevent it from setting the default option in the model. It could even be possible
    // to remove the "force" parameter of this function, since it should never be used anymore (to verify befor
    // making the change). If it is done this way, it would be possible to set the drug using the setDrug() fun-
    // ction instead of doing like the two lines below.
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Set the drug
    other->_drugId = _drugId;

    //Set the engines
    other->setPercentileEngine(_pe->clone());
    other->setPostEngine(_po->clone());
    other->setReverseEngine(_re->clone());

    //Set the patient
    other->setPatient(_patient);

    //Copy the dosages and the parameters
    other->_dosages = _dosages;
    for (int i=0; i < LAST_PT; i++)
        other->_parameters[i] = _parameters[i];

    //Return the clone
    return other;
}

//--------------------------------- DB STUFF -----------------------------------

//Load all our stuff from the DB
bool CurveModel::load(ident id)
{
    Descriptor descriptor;
    SharedCurve curve;

    //Get the curve
//    if (!DATABASE->tryRequest(DB->getCurve(id, curve), tr("Error while trying to fetch the curve %1").arg(id), _errors)) {
//        emit error(_errors);
//        return false;
//    }
    if (NEWDB->getCurveFromId(id, curve).error != NoError) {
        return false;
    }

    //Set the ID
    _id = id;

    //Check the drug
    if (!CORE->drugManager()->exists(curve->drugname())) {
        emit error( tr("The drug used by the saved curve cannot be found : %1").arg(curve->drugname()) );
        return false;
    }

    if (!CORE->pluginManager()->filter(CORE->drugManager()->models(curve->drugname())).contains(curve->modelname())) {
        emit error( tr("The drug used in the database (%1) is not compatible with the given model (%2)").arg(curve->drugname(), curve->modelname()));
        return false;
    }

    //Check the model and engines
    PluginManager *pl = CORE->pluginManager();

    //Model
    ModelDescriptor md;
    md.id = curve->modelname();
    if (!pl->models().contains(md)) {
        emit error( tr("The model used in the database (%1) cannot be found").arg(curve->modelname()));
        return false;
    }

    //PE
    descriptor.id = curve->percentileEnginename();
    if (!pl->percentileEngines().contains(descriptor)) {
        emit error( tr("The percentile engine used by the curve (%1) cannot be found").arg(curve->percentileEnginename()));
        return false;
    }

    //PO
    descriptor.id = curve->postEnginename();
    if (!pl->postEngines().contains(descriptor)) {
        emit error( tr("The post engine used by the curve (%1) cannot be found").arg(curve->postEnginename()));
        return false;
    }

    //RE
    descriptor.id = curve->reverseEnginename();
    if (!pl->reverseEngines().contains(descriptor)) {
        emit error( tr("The reverse engine used by the curve (%1) cannot be found").arg(curve->reverseEnginename()));
        return false;
    }

    //Set the general information
    _firstTake = curve->firsttake();
    _name      = curve->name();
    _pType     = ParameterType(curve->pType());
    _type      = CurveType(curve->cType());

    //Set the model
    if (!setModel(curve->modelname()) || !getModelOptions())
        return false;

    //Set the drug
    if (!this->setDrug(curve->drugname()))
        return false;

    //Set the PE
    if (!this->setPercentileEngine(curve->percentileEnginename()) || !getPEOptions())
        return false;

    //Set the PO
    if (!this->setPostEngine(curve->postEnginename()) || !getPOOptions())
        return false;

    //Set the RE
    if (!this->setReverseEngine(curve->reverseEnginename()) || !getREOptions())
        return false;

    //Fetch and set the patient
    if (!this->setPatient(curve->patient()))
        return false;

    //Fetch the dosages list
    QList<SharedDosage> dosages;
//    if (!DATABASE->tryRequest(DB->getDosagesByDate(_id, dosages), tr("Error while trying to fetch the curve dosages"), _errors)) {
//        emit error(_errors);
//        return false;
//    }
    Tucuxi::Response r = NEWDB->getDosagesByDate(_id,dosages);
    if (r.error != NoError && r.error != NotFound) {
        return false;
    }

    //Set the dosages map
    setDosages(dosages);

    //Update the modified status
    setSaved();

    return true;
}

//Save our data in the DB
bool CurveModel::save()
{
    //Check the PE
    if (!_pe) {
        emit error(tr("The percentile engine must be defined prior to saving"));
        return false;
    }

    //Check the PO
    if (!_po) {
        emit error(tr("The post engine must be defined prior to saving"));
        return false;
    }

    //Check the RE
    if (!_re) {
        emit error(tr("The reverse engine must be defined prior to saving"));
        return false;
    }

    //Check the model
    if (!_model) {
        emit error(tr("The model must be defined prior to saving"));
        return false;
    }

    //Check the drug
    if (_drugId.isEmpty() || !CORE->drugManager()->exists(_drugId)) {
        emit error(tr("The drug must be defined prior to saving"));
        return false;
    }

    //Check the first take
    if (!_firstTake.isValid()) {
        emit error(tr("The first drug take must be defined prior to saving"));
        return false;
    }

    //Check the patient
    if (!_patient) {
        emit error(tr("The patient must be defined prior to saving"));
        return false;
    }

    //Build the DBCurve
    SharedCurve curve = SharedCurve(new NewCurve());
    curve->setId(_id);
    curve->setPatient(_patient);
    curve->setDrugname(_drugId);
    curve->setDrug(CORE->drugManager()->drug(_drugId));
    curve->setModelname(_model->descriptor().id);
    curve->setPercentileEnginename(_pe->descriptor().id);
    curve->setPostEnginename(_po->descriptor().id);
    curve->setReverseEnginename(_re->descriptor().id);
    curve->setCType(int(_type));
    curve->setPType(int(_pType));
    curve->setName(_name);
    curve->setFirsttake(_firstTake);

    //Remove related data
    if (_id != invalid_ident) {

        //Fetch the dosage list
        QList<SharedDosage> dosages;
//        if (!DATABASE->tryRequest(DB->getDosagesByDate(_id, dosages), tr("Error while trying to fetch the curve dosages"), _errors)) {
//            emit error(_errors);
//            return false;
//        }
        Tucuxi::Response r = NEWDB->getDosagesByDate(_id,dosages);
        if (r.error != NoError && r.error != NotFound) {
            return false;
        }

        //Remove previous dosages
        foreach (SharedDosage d, dosages) {
//            if (!DATABASE->tryRequest(DB->deleteDosage(d->id()), tr("Error while trying to delete the dosage %1").arg(d->id()), _errors)) {
//                emit error(_errors);
//                return false;
//            }
            r = NEWDB->getDosagesByDate(_id,dosages);
            if (r.error != NoError && r.error != NotFound) {
                return false;
            }
        }

        //Remove previous model options
        if (!deleteModelsOptions()) {
            emit error(_errors);
            return false;
        }

        //Remove previous model options
        if (!deletePEsOptions()) {
            emit error(_errors);
            return false;
        }

        //Remove previous model options
        if (!deletePOsOptions()) {
            emit error(_errors);
            return false;
        }

        //Remove previous model options
        if (!deleteREsOptions()) {
            emit error(_errors);
            return false;
        }
    }

    //Save the curve
    Tucuxi::Response r = NEWDB->setCurve(curve);
    if (r.error != NoError) {
            return false;
    }
//    if (!DATABASE->tryRequest(DB->setCurve(curve), tr("Error while trying to save the curve"), _errors )) {
//        emit error(_errors);
//        return false;
//    }

    //Update the ID
    if (_id == invalid_ident)
        _id = curve->id();

    //Save the dosages
    foreach (SharedDosage d, _dosages.values()) {
        d->setId(-1);
        if (NEWDB->setDosage(d).error != NoError) {
                return false;
        }
//        if (!DATABASE->tryRequest(DB->setDosage(_id,d), tr("Error while trying to set the curve dosages"), _errors)) {
//            emit error(_errors);
//            return false;
//        }
    }

    //Save the model options
    if (!setModelOptions()) {
        emit error(_errors);
        return false;
    }

    //Save the model options
    if (!setPEOptions()) {
        emit error(_errors);
        return false;
    }

    //Save the model options
    if (!setPOOptions()) {
        emit error(_errors);
        return false;
    }

    //Save the model options
    if (!setREOptions()) {
        emit error(_errors);
        return false;
    }

    //Signal the curve as saved
    emit saved();

    return true;
}

ident CurveModel::id() const
{
    return _id;
}

//--------------------------------- GETTERS -----------------------------------

bool CurveModel::isModified() const
{
    return _modified;
}

bool CurveModel::isSaved() const
{
    return _id != invalid_ident;
}

QVector< QVector<double> > CurveModel::exportPoints(const Duration &from, const Duration &to, const Duration &interval, const PercentileList &percentiles, bool time)
{
    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // This function needs to be optimized. Currently, the time is always calculated, and the re-
    // sult is computed in two iterations, which could be done in only one.
    // ////////////////////////////////////////////////////////////////////////////////////////////

    //Check if the durations and the interval are valid
    if (from.isNegative() || to.isNegative() || from >= to || interval.isEmpty() || interval.isNegative())
        return QVector< QVector<double> >();

    //Get the from and to durations in order to have complete cycles
    Duration totalFrom = prevCycle(from);
    Duration totalTo   = nextCycle(to);

    //Get the posology changes occuring during the specified interval
    QList<Duration> changes;
    Duration current = totalFrom;

    changes.append(current);
    while ((current = nextDosageTime(current)) < totalTo && !current.isEmpty())
        changes.append(current);
    changes.append(totalTo);

    //Use QVectors to store the sub-datasets values and time
    QVector< QVector<double> > data(percentiles.size() ? percentiles.size() * 2 + 1 : 1);
    QVector<double>            date;

    while (changes.size() > 1) {
        QVector<double> partialData;
        QVector<double> partialDate;

        //Get the current computation interval
        Duration cFrom = changes.at(0);
        Duration cTo   = changes.at(1);

        changes.removeFirst();

        //Get the number of points of the current interval
        uint cPoints = (cTo - cFrom).mSecs() / interval.mSecs() + 1;

        //Get the number of points including duplications
        cPoints = uniformPoints(cPoints, cFrom, cTo);

        //Compute the current interval dataset
        points(cFrom, cTo, cPoints, parameterType(), SharedDosage(new Dosage()), true).getDataTimes(partialData, partialDate);

        //Add it to the complete dataset
        data[0] += partialData;
        date    += partialDate;

        //If required, compute the percentiles
        if (percentiles.size()) {

            //Get the percentiles ranges
            QList<int> pList;
            foreach (Percentile p, percentiles)
                pList << p.low() << p.high();

            //Compute the percentiles points
            QMap<int, DataSet> ps = this->percentiles(pList, cFrom, cTo, cPoints, parameterType(), true);

            //Append the points to the data lists
            QMap<int, DataSet>::const_iterator ite = ps.constBegin();
            int cpt = 1;

            while (ite != ps.constEnd()) {

                //Clear the vector for next iteration
                partialData.clear();

                //Fill the data vector
                ite.value().getData(partialData);

                //Add them to the complete dataset
                data[cpt] += partialData;

                //Increment the counter and iterator
                ite++;
                cpt++;
            }
        }
    }

    //The from and to time_t
    uint fromDate = firstTake().addMSecs(from.mSecs()).toTime_t();
    uint toDate   = firstTake().addMSecs(to.mSecs()).toTime_t();

    //The previous point time
    double previousDate = -1.0;

    //The result
    QVector< QVector<double> > result(time ? data.size() + 1 : data.size());

    //For each points
    for (int i = 0; i < date.size(); i++) {

        //Ignore out of range points
        if (date.at(i) < fromDate || date.at(i) > toDate)
            continue;

        //Ignore duplicated points
        if (previousDate != -1.0 && previousDate == date.at(i))
            continue;

        //Update the previous time
        previousDate = date.at(i);

        //Insert the time if requested
        if (time)
            result[0].append(date.at(i));

        //Insert the curve and percentiles
        for (int j = (time ? 1 : 0); j < (time ? data.size() + 1 : data.size()); j++)
            result[j].append(data.at(time ? j - 1 : j).at(i));
    }

    return result;
}

QVector< QVector<double> > CurveModel::exportPoints(const QDateTime &from, const QDateTime &to, const Duration &interval, const PercentileList &percentiles, bool time)
{
    //Check the dates
    if (!from.isValid() || !to.isValid())
        return QVector< QVector<double> >();

    return exportPoints(Duration(0, 0, firstTake().secsTo(from)), Duration(0, 0, firstTake().secsTo(to)), interval, percentiles, time);
}

QVector< QVector<double> > CurveModel::exportPoints(const Duration &from, const Duration &to, int nbPoints, const PercentileList &percentiles, bool time)
{
    //Check the points
    if (nbPoints < 1)
        return QVector< QVector<double> >();

    //Get the interval
    Duration interval(0, 0, 0, (to - from).mSecs() / (nbPoints > 1 ? nbPoints - 1 : 1));

    return exportPoints(from, to, interval, percentiles, time);
}

QVector< QVector<double> > CurveModel::exportPoints(const QDateTime &from, const QDateTime &to, int nbPoints, const PercentileList &percentiles, bool time)
{
    //Check the points
    if (nbPoints < 1)
        return QVector< QVector<double> >();

    //Check the dates
    if (!from.isValid() || !to.isValid())
        return QVector< QVector<double> >();

    //Convert the dates
    Duration dFrom(0, 0, firstTake().secsTo(from));
    Duration dTo(0, 0, firstTake().secsTo(to));

    //Get the interval
    Duration interval(0, 0, 0, (dTo - dFrom).mSecs() / (nbPoints > 1 ? nbPoints - 1 : 1));

    return exportPoints(dFrom, dTo, interval, percentiles, time);
}

DrugModel *CurveModel::model() const
{
    return _model;
}

PostEngine *CurveModel::postEngine() const
{
    return _po;
}

PercentileEngine *CurveModel::percentileEngine() const
{
    return _pe;
}

ReverseEngine *CurveModel::reverseEngine() const
{
    return _re;
}

Descriptor CurveModel::drug() const
{
    return CORE->drugManager()->drug(_drugId)->descriptor();
}

//Returns the dose unit
Unit CurveModel::doseUnit() const
{
    return CORE->drugManager()->drug(_drugId)->doses()->unit();
}

//Returns the concentration unit
Unit CurveModel::concentrationUnit() const
{
    return CORE->drugManager()->drug(_drugId)->concentrations()->unit();
}

Descriptor CurveModel::intake() const
{
    return _model ? CORE->definitionManager()->intake(_model->descriptor().intake) : Descriptor();
}

SharedPatient CurveModel::patient() const
{
    return _patient;
}

//Go through the measures and see which one fits the from and to value
QList<SharedMeasure> CurveModel::measures(const Duration &from, const Duration &to) const
{
    //Check if the range is valid
    if (from > to)
        return QList<SharedMeasure>();

    //Create and fill the list
    QList<SharedMeasure> list;

    QMap<Duration, SharedMeasure>::const_iterator i = _measures.lowerBound(from);
    while (i != _measures.constEnd()) {
        if (i.key() > to)
            break;

        list.append(i.value());
        i++;
    }

    return list;
}

//Returns the measures of this curve
QList<SharedMeasure> CurveModel::measures() const
{
    return _measures.values();
}

//Returns the previous measure
SharedMeasure CurveModel::previousMeasure(const Duration &duration) const
{
    //Check if the duration is null or negative
    if (duration.isEmpty() || duration.isNegative())
        return SharedMeasure(new Measure());

    //Get the duration of the previous dosage
    Duration previous = previousMeasureTime(duration);

    //Return the previous dosage
    return _measures.value(previous);
}

//Returns the previous measure
SharedMeasure CurveModel::previousMeasure(const QDateTime &date) const
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the previous measure, if any
    return previousMeasure(duration);
}

//Returns the previous measure time
Duration CurveModel::previousMeasureTime(const Duration &duration) const
{
    //Get the measure that comes at or before the given time
    QMap<Duration, SharedMeasure>::const_iterator i = _measures.lowerBound(duration);

    //Take the previous one if the current one is not the first
    if (i != _measures.constBegin())
        i--;

    return i.key();
}

//Returns the previous measure time
Duration CurveModel::previousMeasureTime(const QDateTime &date) const
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the previous measure change time
    return previousMeasureTime(duration);
}

//Return the list from the map values
QList<PatientVariateSet*> CurveModel::covariates() const
{
    return _covariates.values();
}

//Returns the current covariates
PatientVariateSet* CurveModel::currentCovariates(const Duration &duration) const
{
    //Check if the duration is not negative
    if (duration.isNegative())
        return PatientVariateSet*(new PatientVariateSet());

    //Return the dosage currently in use
    return _covariates.value(currentCovariatesTime(duration));
}

//Returns the current covariates
PatientVariateSet* CurveModel::currentCovariates(const QDateTime &date) const
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the previous measure change time
    return currentCovariates(duration);
}

//Returns the application time of the covariates set currently used
Duration CurveModel::currentCovariatesTime(const Duration &duration) const
{
    //Get the covariates set that comes right after the given duration
    QMap<Duration, PatientVariateSet*>::const_iterator i = _covariates.upperBound(duration);

    //Take the previous one if the current one is not the first
    if (i != _covariates.constBegin())
        i--;

    return i.key();
}

//Returns the set coming at or right before the given time
ClinicalSet CurveModel::clinicals(const Duration &time) const
{
    Q_UNUSED(time);

    //Fetch the covariate set coming right after the given time
    QMap<Duration, ClinicalSet>::const_iterator i = _clinicals.end();
    QMap<Duration, ClinicalSet>::const_iterator j = _clinicals.begin();
    ClinicalSet cs;

    while(j.key() < i.key())
    {
        foreach(SharedClinical cj, j.value())
        {
            bool NotInSet = true;
            foreach(SharedClinical c, cs)
            {
                if(c->id() == cj->id())
                {
                    cs.replace(cs.indexOf(c), cj);
                    NotInSet = false;
                    continue;
                }
            }
            if(NotInSet)
            {
                cs.append(cj);
            }
        }
        j++;
    }

    return cs;
}

//Returns the current parameters set
ParameterSet* CurveModel::parameters(const Duration &time, ParameterType type)
{
    //If the type is LAST_PT, use the curve's default
    if (type == LAST_PT)
        type = _pType;

    //If the set map is empty, return a default set
    if (_parameters[type].isEmpty())
        return ParameterSet*(new ParameterSet());

    //Update the cache to this point
    calculateParameters(type, time);

    //Fetch the last one corresponding
    QMap<Duration,ParameterSet*>::const_iterator i = _parameters[type].upperBound(time);

    //If it's not the first, take the previous
    if (i != _parameters[type].constBegin())
        i--;

    return i.value();
}

//Returns the current parameters set
ParameterSet* CurveModel::parameters(const QDateTime &date, ParameterType type)
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the previous measure change time
    return parameters(duration, type);
}

//Returns a subset of parameters sets
QMap<Duration, ParameterSet*> CurveModel::parameters(const Duration &start, const Duration &end, ParameterType type)
{
    //Check if the range is valid
    if (start > end)
        return QMap<Duration, ParameterSet*>();

    //Check if the range is positive
    if (start.isNegative() || end.isNegative())
        return QMap<Duration, ParameterSet*>();

    //Check if the type is valid
    if (type == LAST_PT)
        return QMap<Duration, ParameterSet*>();

    //Check if the parameters exist
    if (_parameters[type].isEmpty())
        return QMap<Duration, ParameterSet*>();

    QMap<Duration, ParameterSet*> output, input = _parameters[type];

    //Iterate the targeted map
    QMap<Duration, ParameterSet*>::const_iterator i = input.lowerBound(start);

    while (i != input.constEnd() && i.key() <= end) {
        output.insert(i.key(), i.value());
        i++;
    }

    //Return the subset
    return output;
}

QString CurveModel::translatedCurveType() const
{
    return translatedCurveType(curveType());
}

CurveModel::CurveType CurveModel::curveType() const
{
    return _type;
}

void CurveModel::setCurveType(const CurveModel::CurveType type)
{
    //Ignore if the type is the same
    if (_type == type)
        return;

    //Update the curve type
    _type = type;

    //For edition, invalidate the post engine
    invalidatePostEngine();

    //For edition, reset the prio and post caches
    resetCache();

    //For edition, reset the population cache
    _parametersValidUntil[POPULATION].clear();
    _cacheValidUntil[POPULATION].clear();
    _residuals[POPULATION].clear();
    _steadyStates[POPULATION].clear();
    _initialResidual[POPULATION].clear();

    //For edition, reset the MANUAL cache
    _parametersValidUntil[MANUAL].clear();
    _cacheValidUntil[MANUAL].clear();
    _residuals[MANUAL].clear();
    _steadyStates[MANUAL].clear();
    _initialResidual[MANUAL].clear();

    //For edition, reset the TMP cache
    _parametersValidUntil[TMP].clear();
    _cacheValidUntil[TMP].clear();
    _residuals[TMP].clear();
    _steadyStates[TMP].clear();
    _initialResidual[TMP].clear();

    //For edition, emit the appropriate signals
    emit curveTypeChanged(type);
    emit changed();
}

ParameterType CurveModel::parameterType() const
{
    return _pType;
}

QDateTime CurveModel::firstTake() const
{
    return _firstTake;
}

QString CurveModel::name() const
{
    return _name;
}

QList<Target*> CurveModel::targets() const
{
    return CORE->drugManager()->drug(_drugId)->targets();
}

//Return the next covariate, measure or dosage change
Duration CurveModel::nextChange(const Duration &time) const
{
    //Get the next change time of dosage, covariate and measure
    Duration nd = nextDosageTime(time);
    Duration nc = nextCovariateChange(time);
    Duration nm = nextMeasureChange(time);

    //Case with no next dosage
    if (nd.isEmpty()) {
        if (nc.isEmpty())
            return nm;
        if (nm.isEmpty())
            return nc;
        return nm < nc ? nm : nc;
    }

    //Case with no next covariate
    if (nc.isEmpty()) {
        if (nd.isEmpty())
            return nm;
        if (nm.isEmpty())
            return nd;
        return nm < nd ? nm : nd;
    }

    //Case with no next measure
    if (nm.isEmpty()) {
        if (nd.isEmpty())
            return nc;
        if (nc.isEmpty())
            return nd;
        return nc < nd ? nc : nd;
    }

    //Case with no empty duration
    Duration dc = nd < nc ? nd : nc;
    return dc < nm ? dc : nm;
}

//UpperBound on the covariates
Duration CurveModel::nextCovariateChange(const Duration &time, bool current) const
{
    if (current && _covariates.contains(time))
        return time;

    QMap<Duration, PatientVariateSet*>::const_iterator i = _covariates.upperBound(time);
    if (i == _covariates.constEnd())
        return Duration();
    else
        return i.key();
}

Duration CurveModel::nextMeasureChange(const Duration &time, bool current) const
{
    if (current && _measures.contains(time))
        return time;

    QMap<Duration, SharedMeasure>::const_iterator i = _measures.upperBound(time);
    if (i == _measures.constEnd())
        return Duration();
    else
        return i.key();
}

//Returns the time of the next day
Duration CurveModel::nextDayTime(const Duration &time) const
{
    int days = time.toHours() / 24;
    return Duration((++days) * 24);
}

//Returns the time of the current day
Duration CurveModel::currentDayTime(const Duration &time) const
{
    int days = time.toHours() / 24;
    return Duration(days * 24);
}

//Find the previous change, in both the covariates and the dosages
Duration CurveModel::previousChange(const Duration &time) const
{
    //Fetch the corresponding duration
    Duration dos = currentDosageTime(time);
    Duration cov = prevCovariateChange(time);

    //If one of them is empty, return the other
    if (dos.isEmpty())
        return cov;
    else if (cov.isEmpty())
        return dos;

    //Else, return the smaller of the two
    return dos < cov ? dos : cov;
}

//Find the previous covariate change
Duration CurveModel::prevCovariateChange(const Duration &time) const
{
    //Find the covariate that comes right after or at the given time
    QMap<Duration, PatientVariateSet*>::const_iterator previous = _covariates.upperBound(time);

    //Get the previous covariate, if it does exist
    if (previous != _covariates.constBegin())
        previous--;

    //Return its duration from the first take
    return previous.key();
}

//Returns the time of the last significant change
Duration CurveModel::lastChangeTime() const
{
    Duration lastCovariate = lastCovariateTime();
    Duration lastDosage = lastDosageTime();

    return lastDosage < lastCovariate ? lastCovariate : lastDosage;
}

//Returns the time of the last covariate change
Duration CurveModel::lastCovariateTime() const
{
    //Check if there is at least one covariate
    if (_covariates.isEmpty())
        return Duration();

    return _covariates.keys().last();
}

Duration CurveModel::lastMeasureTime() const
{
    //Check if there is at least one measure
    if (_measures.isEmpty())
        return Duration();

    return _measures.keys().last();
}

//Returns the time of the next cycle begining
Duration CurveModel::nextCycle(const Duration &time) const
{
    //Temporary set the result to the provided time
    Duration result = time;

    //Get the current and next dosages' times
    Duration currDosage = currentDosageTime(time);
    Duration nextDosage = nextDosageTime(time);

    //Get the current dosage interval
    Duration currInterval = dosage(currDosage)->interval();

    //Get the duration from the current dosage time to the given time
    Duration fromLastDosage = time - currDosage;

    //Get the duration from the given time to the end of the next cycle
    Duration remainder = fromLastDosage % currInterval;

    //If the remainder is not null, add it to the result
    if (!remainder.isEmpty())
        result += currInterval - remainder;

    //If there is a next dosage that comes before the result, update it
    if (!nextDosage.isEmpty() && result > nextDosage)
        result = nextDosage;

    //Return the result
    return result;
}

Duration CurveModel::nextCycle(const QDateTime &date) const
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the next cycle time
    return nextCycle(duration);
}

//Returns the time of the previous cycle begining
Duration CurveModel::prevCycle(const Duration &time, const SharedDosage &dosage) const
{
    //Temporary set the result to the provided time
    Duration result = time;

    //Get the current dosage time
    Duration currDosage = currentDosageTime(time);

    //Get the current dosage interval
    Duration currInterval = this->dosage(currDosage)->interval();

    //Update the data if a temporary dosage is given
    if (dosage->isValid()) {
        Duration tmpDosage(0, 0, _firstTake.secsTo(dosage->applied()));

        if (tmpDosage > currDosage) {
            currDosage = tmpDosage;
            currInterval = dosage->interval();
        }
    }

    //Get the duration from the current dosage time to the given time
    Duration fromLastDosage = time - currDosage;

    //Get the duration from the given time to the end of the previous cycle
    Duration remainder = fromLastDosage % currInterval;

    //If the remainder is not null, substract it from the result
    if (!remainder.isEmpty())
        result -= remainder;

    //Return the result
    return result;
}

Duration CurveModel::prevCycle(const QDateTime &date, const SharedDosage &dosage) const
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the previous cycle time
    return prevCycle(duration, dosage);
}

//--------------------------------- DOSAGES -------------------------------------

//Adds or replaces a dosage
/*bool CurveModel::setDosage(const SharedDosage &dosage)
{
    //Check if the dosage is valid
    if (!dosage.isValid())
        return false;

    //Get the duration from first take
    Duration applied(0, 0, 0, _firstTake.msecsTo(dosage.applied));

    //Add or replace the dosage
    _dosages.insert(applied, dosage);

    //Reset the caches
    resetCache(applied);

    //Send the appropriate signals
    emit dosagesModified();
    emit changed();

    return true;
}

//Removes a dosage
bool CurveModel::removeDosage(const SharedDosage &dosage)
{
    //Check if the dosage is valid
    if (!dosage.isValid())
        return false;

    //Check if the dosage exists
    SharedDosage found = this->dosage(dosage.applied);

    if (!found.isValid() || found != dosage)
        return false;

    //Get the duration from the first take
    Duration applied(0, 0, 0, _firstTake.msecsTo(dosage.applied));

    //Remove the dosage
    _dosages.remove(applied);

    //Reset the caches
    resetCache(applied);

    //Send the appropriate signals
    emit dosagesModified();
	emit changed();

    return true;
}*/

//Checks if the two dosages are identical
bool CurveModel::isIdentical(const SharedDosage &first, const SharedDosage &second)
{
    //Return false if one the dosages is invalid
    if (!first->isValid() || !second->isValid())
        return false;

    //Return false if the two dosages are equal
    if (first == second)
        return false;

    //Return false if the first dosage occures in the middle of a cycle
    int tDiff = (int) second->applied().toTime_t() - (int) first->applied().toTime_t();

    if (tDiff % (int) first->interval().toSeconds())
        return false;

    //Return true if the next dosage is similar to the given dosage
    return (first->dose() == second->dose()) && (first->interval() == second->interval()) && (first->tinf() == second->tinf());
}

//Add a new dosage
bool CurveModel::addDosage(const SharedDosage &dosage)
{
    //Check if the dosage is valid
    if (!dosage->isValid())
        return false;

    //Check if the dosage is a duplicate
    if (isDuplicate(dosage))
        return true;

    //Updates the first take if needed
    if (dosage->applied() < _firstTake)
        setFirstTake(dosage->applied());

    //Get the duration from first take
    Duration applied(0, 0, 0, _firstTake.msecsTo(dosage->applied()));

    //Remove the current dosage if any
    if (_dosages.value(applied) && _dosages.value(applied)->isValid())
        _dosages.remove(applied);

    //Remove the next if identical
    if (isNextIdentical(dosage))
        _dosages.remove(nextDosageTime(dosage->applied()));

    //Insert if the previous is different
    if (!isPreviousIdentical(dosage))
        _dosages.insert(applied, dosage);

    //Invalidate the post engine
    invalidatePostEngine();

    //Reset the caches
    resetCache(applied);

    //Send the appropriate signals
    emit dosagesModified();
    emit changed();

    return true;
}

//Removes a dosage
bool CurveModel::removeDosage(const SharedDosage &dosage)
{
    //Check if the dosage is valid
    if (!dosage->isValid())
        return false;

    //Check if there is enough dosages
    if (_dosages.size() == 1)
        return false;

    //Check if the dosage exists
    SharedDosage found = this->dosage(dosage->applied());

    if (!found->isValid() || found != dosage)
        return false;

    //Get the duration from first take
    Duration applied(0, 0, 0, _firstTake.msecsTo(dosage->applied()));

    //Remove the dosage from the map
    _dosages.remove(applied);

    //Remove any duplicate
    SharedDosage prev = previousDosage(dosage->applied());
    SharedDosage next = nextDosage(dosage->applied());

    if (prev->isValid() && next->isValid() && isPreviousIdentical(next))
        _dosages.remove(nextDosageTime(dosage->applied()));

    //Check if the removed dosage is the first take
    if (dosage->applied() == _firstTake)
        setFirstTake(next->applied());

    //Invalidate the post engine
    invalidatePostEngine();

    //Reset the caches
    resetCache(applied);

    //Emit the signals
    emit dosagesModified();
    emit changed();

    return true;
}

//Modifies a dosage
bool CurveModel::modifyDosage(const SharedDosage &initial, const SharedDosage &modified)
{
    //Check if the dosages are valid
    if (!initial->isValid() || !modified->isValid())
        return false;

    //Check if the initial exists
    SharedDosage found = this->dosage(initial->applied());

    if (!found->isValid() || found != initial)
        return false;

    //Check if the dosages are equal
    if (initial == modified)
        return true;

    //Check if the dates are equals
    if (initial->applied() == modified->applied())
        return addDosage(modified);

    //Update the dosages map
    _dosages.remove(Duration(0, 0, 0, _firstTake.msecsTo(initial->applied())));
    _dosages.insert(Duration(0, 0, 0, _firstTake.msecsTo(modified->applied())), modified);

    //Set the dosages
    return setDosages(_dosages.values());
}

//Sets the dosages of this curve
bool CurveModel::setDosages(const QList<SharedDosage> &dosages)
{
    QMap<QDateTime, SharedDosage> ordered;

    //Check if the list is empty
    if (!dosages.size())
        return false;

    //Order the dosages in a map
    for (int i = 0; i < dosages.size(); i++) {
        SharedDosage current = dosages.at(i);

        //Abord if an invalid or duplicated is found
        if (!current->isValid())
            return false;
        if (ordered.contains(current->applied()))
            return false;

        //Insert the dosage in the ordered map
        ordered.insert(current->applied(), current);
    }

    //Get the new first take
    QMap<QDateTime, SharedDosage>::const_iterator i = ordered.constBegin();
    QMap<Duration, SharedDosage> newMap;
    QDateTime firstTake = i.key();

    //Build the new dosages map
    while (i != ordered.constEnd()) {
        Duration duration(0, 0, 0, _firstTake.msecsTo(i.key()));
        newMap.insert(duration, (i++).value());
    }

    //Apply the new dosages map
    _dosages = newMap;

    //Remove any identical dosage
    QMap<Duration, SharedDosage>::iterator j = ++_dosages.begin();

    while (j != _dosages.end()) {
        if (isIdentical((j - 1).value(), j.value()))
            _dosages.remove((j++).key());
        else
            j++;
    }

    //Invalidate the post engine
    invalidatePostEngine();

    //Reset the whole cache
    resetCache();

    //Check if the first take changed
    if (_firstTake != firstTake)
        setFirstTake(firstTake);

    //Emit the appropriate signals
    emit dosagesModified();
    emit changed();

    return true;
}

//Returns the curve dosages
QList<SharedDosage> CurveModel::dosages() const
{
    return _dosages.values();
}

//Returns the dosage at the given time
SharedDosage CurveModel::dosage(const Duration &duration) const
{
    return _dosages.value(duration);
}

//Returns the dosage at the given date
SharedDosage CurveModel::dosage(const QDateTime &date) const
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the dosage at the exact duration
    return dosage(duration);
}

//Returns the dosage currently used at the given duration
SharedDosage CurveModel::currentDosage(const Duration &duration) const
{
    //Check if the duration is not negative
    if (duration.isNegative())
        return SharedDosage(new Dosage());

    //Return the dosage currently in use
    return dosage(currentDosageTime(duration));
}

//Returns the dosage currently used at the given date
SharedDosage CurveModel::currentDosage(const QDateTime &date) const
{
    //Check if the date isn't before the first take
    if (date < firstTake())
        return SharedDosage(new Dosage());

    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the dosage currently in use
    return currentDosage(duration);
}

//Returns the time of the dosage currently used at the given date
Duration CurveModel::currentDosageTime(const Duration &duration) const
{
    //Get the dosage that comes right after the given duration
    QMap<Duration,SharedDosage>::const_iterator i = _dosages.upperBound(duration);

    //Take the previous one if the current one is not the first
    if (i != _dosages.constBegin())
        i--;

    return i.key();
}

//Returns the time of the dosage currently used at the given date
Duration CurveModel::currentDosageTime(const QDateTime &date) const
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the current dosage application time
    return currentDosageTime(duration);
}

//Returns the next dosage
SharedDosage CurveModel::nextDosage(const Duration &duration) const
{
    //Get the duration of the next dosage
    Duration next = nextDosageTime(duration);

    //If empty, return an invalid dosage
    if (next.isEmpty())
        return SharedDosage(new Dosage());

    return dosage(next);
}

//Returns the next dosage
SharedDosage CurveModel::nextDosage(const QDateTime &date) const
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the next dosage, if any
    return nextDosage(duration);
}

//Returns the next dosage change time
Duration CurveModel::nextDosageTime(const Duration &duration) const
{
    //Get the dosage that comes after the given time
    QMap<Duration, SharedDosage>::const_iterator i = _dosages.upperBound(duration);

    //Return an empty duration if there is no next dosage
    if (i == _dosages.constEnd())
        return Duration();

    return i.key();
}

//Returns the next dosage change time
Duration CurveModel::nextDosageTime(const QDateTime &date) const
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the next dosage change time
    return nextDosageTime(duration);
}

//Returns the previous dosage
SharedDosage CurveModel::previousDosage(const Duration &duration) const
{
    //Check if the duration is null or negative
    if (duration.isEmpty() || duration.isNegative())
        return SharedDosage(new Dosage());

    //Get the duration of the previous dosage
    Duration previous = previousDosageTime(duration);

    //Return the previous dosage
    return dosage(previous);
}

//Returns the previous dosage
SharedDosage CurveModel::previousDosage(const QDateTime &date) const
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the previous dosage, if any
    return previousDosage(duration);
}

//Returns the previous dosage change time
Duration CurveModel::previousDosageTime(const Duration &duration) const
{
    //Get the dosage that comes at or before the given time
    QMap<Duration, SharedDosage>::const_iterator i = _dosages.lowerBound(duration);

    //Take the previous one if the current one is not the first
    if (i != _dosages.constBegin())
        i--;

    return i.key();
}

//Returns the previous dosage change time
Duration CurveModel::previousDosageTime(const QDateTime &date) const
{
    //Get the duration from the first take
    Duration duration(0, 0, 0, firstTake().msecsTo(date));

    //Return the previous dosage change time
    return previousDosageTime(duration);
}

//Returns the last dosage
SharedDosage CurveModel::lastDosage() const
{
    return _dosages.value(lastDosageTime());
}

//Returns the last dosage change time
Duration CurveModel::lastDosageTime() const
{
    //Check if there is at least one dosage
    if (_dosages.isEmpty())
        return Duration();

    return _dosages.keys().last();
}

//Checks if the dosage is a duplicate
bool CurveModel::isDuplicate(const SharedDosage &dosage) const
{
    //Check if there is a dosage at the same date
    SharedDosage current = this->dosage(dosage->applied());

    //Return true if it is valid and equal
    return current && current->isValid() && current == dosage;
}

//Checks if the dosage is a duplicate of the next dosage
bool CurveModel::isNextIdentical(const SharedDosage &dosage) const
{
    return isIdentical(dosage, nextDosage(dosage->applied()));
}

//Checks if the dosage is a duplicate of the previous dosage
bool CurveModel::isPreviousIdentical(const SharedDosage &dosage) const
{
    return isIdentical(previousDosage(dosage->applied()), dosage);
}

//--------------------------------- CALCULATIONS -------------------------------

//Calculate a serie of points using the model and the cache
DataSet CurveModel::points(const Duration &start, const Duration &stop, uint points, ParameterType type, const SharedDosage &dosage, bool uniform)
{
    //Check if at least one point is asked
    if (points == 0) {
        emit error(tr("Cannot extract data for 0 point"));
        return DataSet();
    }

    //Check if the patient is set
    if (!_patient) {
        emit error(tr("The curve's patient is missing"));
        return DataSet();
    }

    //Check if the model is set
    if (!checkObjects(MODEL)) {
        emit error(tr("The curve's model is missing"));
        return DataSet();
    }

    //Check if the first take is set
    if (_dosages.isEmpty()) {
        emit error(tr("The curve's first take is missing"));
        return DataSet();
    }

    //If a dosage is given, add it temporarly
    SharedDosage tmpDosage;
    Duration applied;
    if (dosage->isValid()) {
        //Duration after the first drug take
        applied.addSecs(_firstTake.secsTo(dosage->applied()));

        //If another dosage was here, save it
        tmpDosage = _dosages.value(applied);

        //Add it (temporary)
        _dosages.insert(applied,dosage);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //ToDo: A better solution would be to reset all the caches when calling the resetCache(time) function, but:
        // 1. The manual, tmp and population parameters must not be erased in the resetCache(type, time) function
        // 2. The setParameters() function need to remove the TMP or MANUAL parameters itself, since it won't be
        //    done in resetCache(type, time) anymore. There might be other functions doing so, need to be checked
        // 3. In the covariatesChanged() function, we need to call the resetCache(type, time) for both APRIORI and
        //    APOSTERIORI types, since we only need to reset those two types of caches in case of a covariate change
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /* Reset the cache. If the type is POPULATION or MANUAL, we need to make sure the residuals and the cacheValidUntil
         * parameter are reseted to the specified time. For this, we need to specify the type, because the resetCache(time)
         * function only reset the APRIORI and APOSTERIORI function. We also need to make sure the time isn't empty (0), or
         * the POPULATION/MANUAL parameters will be lost. */
        if (!applied.isEmpty() && (type == POPULATION || type == MANUAL))
            resetCache(type, applied);
        else
            resetCache(applied);
    }

    //Verify and update the parameters cache (until the stop)
    if (_parametersValidUntil[type] < stop)
        calculateParameters(type, stop);

    //Verify and update the residual cache (Until the start)
    if (_cacheValidUntil[type] < start)
        calculateCache(type, start);

    //Use the calculate function, not in cache mode you idiot!
    DataSet result = calculate(start, stop, points, type, false, dosage, uniform);

    //If a dosage was given, remove it
    if (dosage->isValid()) {
        //If another one was there, replace it
        if (tmpDosage->isValid())
            _dosages.insert(applied,tmpDosage);
        //Else, remove it
        else
            _dosages.remove(applied);

        //Reset the cache (like we did above)
        if (!applied.isEmpty() && (type == POPULATION || type == MANUAL))
            resetCache(type, applied);
        else
            resetCache(applied);

        //Reset the default parameters
        if (applied.isEmpty())
            calculateParameters(type, stop);
    }

    return result;
}

//Helper function using the points() function with only one value
Amount CurveModel::point(const Duration &moment, ParameterType type, const SharedDosage &dosage)
{
    //Define a start point just before the stop point
    Duration start;

    /* In order to deal with changes in covariates in the middle of a cycle, we need to set a range that matches the begining
     * of a cycle. If we don't (ie. we set the start a few milliseconds before the given moment), the resulting point might
     * take into account paramaters changes occuring in the current cycle, while the curve prediction doesn't. That is why
     * we set the start duration to the begining of the current cycle (or previous one if moment matches the begining of
     * a cycle), unless the moment is null, in which case we need to set the start a few milliseconds before the moment. */
    if (moment.isEmpty())
        start = moment - Duration(0, 0, 0, 1);
    else
        start = prevCycle(moment - Duration(0, 0, 0, 1), dosage);

    //Predict for 1 point
    DataSet result = points(start,moment,1,type,dosage);

    //Create an amount from the dataset
    return Amount(result.at(0),result.unit());
}

//Returns the peak amount for the
Amount CurveModel::peak(Duration &peakTime, const Duration &start, const Duration &stop, uint points, ParameterType type)
{
    //Check if the interval is valid
    if (start >= stop)
        return Amount();

    //Check if the number of points is valid
    if (points < 10)
        return Amount();

    //Get the corresponding dataset
    DataSet data = this->points(start, stop, points, type);

    if (data.size() == 0)
        return Amount();

    //Find the peak in the dataset
    double current, previous = data.at(0), peak = -1.0;
    uint i;

    for (i = 1; i < data.size(); i++) {
        current = data.at(i);
        if (current < previous) {
            peak = previous;
            break;
        }
        previous = current;
    }

    if (peak == -1.0)
        return Amount();

    //Set the peak time
    if (i > 0)
        i--;

    qint64 timeUnit = Duration(stop - start).mSecs() / --points;
    peakTime = Duration(0, 0, 0, timeUnit * i);

    //Return the peak
    return Amount(data.at(i), data.unit());
}

//Call the percentile engine's function
QMap<int, DataSet> CurveModel::percentiles(const QList<int> &values, const Duration &start, const Duration &stop, uint points, ParameterType type, bool uniform)
{
    if (!checkObjects(PE))
        return QMap<int, DataSet>();

    return _pe->percentiles(values, start, stop, points, _parameters[type], uniform);
}

//Call the reverse engine's function
QMap<int, SharedDosage> CurveModel::reverseDosages(const Duration &start, ParameterType type, int nbIntakes, const QList<Target*> targets)
{
    //Check the engine presence
    if (!checkObjects(REVERSE))
        return QMap<int, SharedDosage>();

    //We need the cache until this point
    calculateParameters(type, start);
    calculateCache(type, start);

    //Get the durations lists
    QList<Duration> intervals = CORE->drugManager()->drug(_drugId)->intervals()->durations();
    QList<Duration> infusions = CORE->drugManager()->drug(_drugId)->infusions()->durations();

    //Do the reverse calculation
    return _re->reverseDosages(dosageAtTime(start),
                               start,
                               targets,
                               intervals.isEmpty() ? CORE->definitionManager()->intervals() : intervals,
                               CORE->definitionManager()->isInfusion(intake().id) ? infusions : QList<Duration>(),
                               CORE->drugManager()->drug(_drugId)->doses()->amounts(),
                               type,
                               nbIntakes);
}

//Ask the model using our parameters & dosage for this time
Duration CurveModel::halfLife(const Duration &start, ParameterType type, const SharedDosage &dosage)
{
    //Check on the model
    if (!checkObjects(MODEL))
        return Duration();

    //We need the cache until this point
    calculateParameters(type,start);
    calculateCache(type,start);

    //Fetch the last residual time
    Duration lastResidualTime = lastResidual(start,type);

    //Determine the interval local start time from the given start and the preceding residual
    return _model->halflife( start-lastResidualTime, //Local start time
                             parametersAtTime(start,type), //Parameter set
                             dosageAtTime(start,dosage), //Dosage
                             _residuals[type].value(lastResidualTime)); //Residual for the cycle start
}

//Ask the model using our parameters & dosage for this time
Duration CurveModel::steadyState(const Duration &givenStart, ParameterType type, const SharedDosage &dosage)
{
    //Check if the model is valid
    if (!checkObjects(MODEL))
        return Duration();

    //Get the last change time
    Duration lastChange = lastChangeTime();

    //Get the search start time
    Duration start = givenStart.isNegative() ? lastChange : givenStart;

    //Update the cache up to the start time
    calculateParameters(type, start);
    calculateCache(type, start);

    //Special case without using the cache for reveser engines
    if (dosage->isValid()) {
        double upperMargin, lowerMargin, previousResidual, residual = -1.0;
        Duration currentTime = start;

        do {

            //Cancel the search after maxDuration is reached
            if (currentTime > start + _maxDuration)
                return Duration();

            //Get the current residual and update the previous
            currentTime += dosage->interval();
            previousResidual = residual;
            residual = this->points(start, currentTime, 1, type, dosage).at(0);

            //Calculate the current margin for steady state check
            upperMargin = previousResidual + previousResidual * _tolerance;
            lowerMargin = previousResidual - previousResidual * _tolerance;

        } while (residual > upperMargin || residual < lowerMargin);

        //Return the stead state time
        return currentTime;
    }

    //The current time
    Duration currentTime = start;

    //Search for steady state
    while (_steadyStates[type].isEmpty() || _steadyStates[type].last() <= start) {

        //Update the cache up to the current time
        calculateParameters(type, currentTime);
        calculateCache(type, currentTime);

        //Update the current time
        currentTime += dosageAtTime(currentTime,dosage)->interval();

        //Cancel the search after maxDuration is reached
        if (currentTime > start + _maxDuration)
            return Duration();
    }

    //Return the correct steady state time
    foreach (Duration steadyState, _steadyStates[type])
        if (steadyState > start)
            return steadyState;

    return Duration();
}

//--------------------------------- SETTERS -----------------------------------

//Save it, load the drug model and reset the cache validity
bool CurveModel::setDrug(const QString &drug)
{
    //Check if the model is available
    if (!_model) {
        emit error(tr("You need to set a model before setting the drug"));
        return false;
    }

    //Check if the current drug is different
    if (_drugId == drug) {
        qDebug() << "The new drug, '" + drug + "', is the same as the current one, ignoring...";
        return true;
    }

    //Check if the given drug exists
    if (!CORE->drugManager()->exists(drug)) {
        emit error(tr("Impossible to read the drug descriptor for %1, ignoring the change").arg(drug));
        return false;
    }

    //Set the drug
    _drugId = drug;

    //Reset the caches
    resetCache();
    resetCache(MANUAL);

    //Reset the PO state
    _poInvalidated = true;

    //Set the default population params
    ParameterSet* defaultParams = ParameterSet*(CORE->drugManager()->drug(_drugId)->parameters());
    _parameters[POPULATION].clear();
    _parameters[POPULATION].insert(Duration(0), defaultParams);

    //Set the default manual params
    defaultParams->setType(MANUAL);
    _parameters[MANUAL].clear();
    _parameters[MANUAL].insert(Duration(0), defaultParams);

    //Reset the default covariates
    covariatesChanged();

    //Emit the associated signals
    emit drugChanged(drug);
    emit changed();

    return true;
}

bool CurveModel::setPatient(SharedPatient patient)
{
    _patient = patient;
    return true;
}

//Load the patient from the database
bool CurveModel::setPatient(ident patient)
{
    if (_patient)
        if (_patient->id() == patient)
            return true;

    SharedPatient p;
//    Response r = DB->getPatient(patient,p);
    Tucuxi::Response r = NEWDB->getPatientFromId(patient, p);
    if (r.error != NoError) {
//        emit error(tr("Failed to fetch the patient %1 from the database %2.\nError :\n%3")
//                   .arg(patient).arg(r.responder).arg(CORE->database()->errorString(r.error)));
        return false;
    }

    //Save it
    _patient = p;

    //Reset all
    resetCache();
    _covariates.clear();
    _measures.clear();

    //Fetch its covariates/measures
    covariatesChanged();
    measuresChanged();
    clinicalsChanged();

    //Emit the associated signals
    emit patientChanged(patient);
    emit changed();

    return true;
}

// ToDo /////////////////////////////////////////////////////////////////////////
// The first take date should be the applied date of the first dosage, and not a
// separate date that can be changed without any associated dosage. The covariates,
// measures, etc. recomputation done in the setFirttake() function should be done
// when necessary when adding a dosage or changing the date of a dosage, when re-
// levant. To achieve this, simply change the visibility of setFirstTake to private
// and call it from the add/edit/removeDosage functions. It could even be in the
// dosages manager (if the idea of having covariates/measures/dosages/... managers
// used by curve model, to reduce it size and separate the responsability in a bet-
// ter way). This should be done when refactoring the core, taking into account the
// necessity to be able to compute a curve at steady state directly, and possibly
// only for one cycle. Finally, we also need to update all the dosages duration if
// there is any. But again, it would certainly be easier to just use a date instead
// of a duration (that goes for every data in the curve, that is the covariates map,
// the measures maps, the caches variables, etc).
// //////////////////////////////////////////////////////////////////////////////

//Save it and emit its signal
//The covariates and measures needs to be reloaded
void CurveModel::setFirstTake(const QDateTime &firstTake)
{
    //Ignore if identical
    if (!firstTake.isValid() || _firstTake == firstTake)
        return;

	//Update the dosages map
    if (_firstTake.isValid() && !_dosages.isEmpty())
        updateDosages(firstTake);

    //Save the new first take
    _firstTake = firstTake;

    //Refresh the curve data
    measuresChanged();
    covariatesChanged();
    clinicalsChanged();

    //Emit the appropriate signals
    emit firstTakeModified(firstTake);
    emit changed();
}

//Load the model and reset the cache
bool CurveModel::setModel(const QString &model)
{
    //Check if the current model is different
    if (_model && _model->descriptor().id == model) {
        qDebug() << "The new model, '" + model + "', is the same as the current one, ignoring...";
        return true;
    }

    //Create an instance of the new model
    DrugModel *m = CORE->pluginManager()->newDrugModelEngine(model);
    if (!m) {
        emit error(tr("Unable to load the model %1").arg(model));
        return false;
    }

    //Reset the cache
    resetCache();

    //Reset the PO state
    _poInvalidated = true;

    //Set or replace the model
    if (_model)
        delete _model;
    _model = m;

    //Emit the appropriate signals
    emit modelChanged(model);
    emit changed();

    return true;
}

//Reset the cache only if the models are not the same
void CurveModel::setModel(DrugModel *model)
{
    // ToDo //////////////////////////////////////////////////////////////////////
    // Possible memory leak below, check and delete the model if already existing.
    // Another issue is with the model comparison, it will always be false since
    // the models and engines are cloned, and therefore do not point to the same
    // location. We need to implement an Model::equal() and Engine::equal() func-
    // tions to compare model and engine, taking into account there options values.
    // ///////////////////////////////////////////////////////////////////////////

    //Ignore if identical
    if (_model == model)
        return;

    //Update the model
    _model = model;
    resetCache();

    //Reset the PO state
    _poInvalidated = true;

    //Emit the associated signals
    emit modelChanged(model->descriptor().id);
    emit changed();
}

//Simply load it
bool CurveModel::setPercentileEngine(const QString &pe)
{
    //Check if the current percentile engine is different
    if (_pe && _pe->descriptor().id == pe) {
        qDebug() << "The new percentile engine, '" + pe + "', is the same as the current one, ignoring...";
        return true;
    }

    //Create an instance of the new percentile engine
    PercentileEngine *p = CORE->pluginManager()->newPercentileEngine(pe);
    if (!p) {
        emit error(tr("Unable to load the percentile engine %1").arg(pe));
        return false;
    }

    //Set or replace the percentile engine
    if (_pe)
        delete _pe;
    _pe = p;

    //Set the curve model in the engine
    _pe->setCurveModel(this);

    //Emit the appropriate signals
    emit percentileEngineChanged(pe);
    emit changed();

    return true;
}

void CurveModel::setPercentileEngine(PercentileEngine *engine)
{
    // ToDo///////////////////////////////////////////////////////////////////////
    // Possible memory leak below, check and delete the pe if already existing
    // ///////////////////////////////////////////////////////////////////////////

    //Ignore if identical
    if (_pe == engine)
        return;

    //Update the engine
    _pe = engine;
    _pe->setCurveModel(this);

    //Emit the appropriate signals
    emit percentileEngineChanged(engine->descriptor().id);
    emit changed();
}

//Load it and reset the a posteriori parameters validity
bool CurveModel::setPostEngine(const QString &po)
{
    //Check if the current post engine is different
    if (_po && _po->descriptor().id == po) {
        qDebug() << "The new post engine, '" + po + "', is the same as the current one, ignoring...";
        return true;
    }

    //Create an instance of the new post engine
    PostEngine *p = CORE->pluginManager()->newPostEngine(po);
    if (!p) {
        emit error(tr("Unable to load the post engine %1").arg(po));
        return false;
    }

    //Invalidate the post engine
    invalidatePostEngine();

    //Set or replace the post engine
    if (_po)
        delete _po;
    _po = p;

    //Set the curve model in the engine
    _po->setCurveModel(this);

    //Emit the appropriate signals
    emit postEngineChanged(po);
    emit changed();

    return true;
}

void CurveModel::setPostEngine(PostEngine *engine)
{
    // ToDo///////////////////////////////////////////////////////////////////////
    // Possible memory leak below, check and delete the pe if already existing
    // ///////////////////////////////////////////////////////////////////////////

    //Ignore if identical
    if (_po == engine)
        return;

    //Update the engine
    _po = engine;
    _po->setCurveModel(this);

    //Invalidate the post engine
    invalidatePostEngine();

    //Emit the appropriate signals
    emit postEngineChanged(engine->descriptor().id);
    emit changed();
}

//Load it
bool CurveModel::setReverseEngine(const QString &re)
{
    //Check if the current reverse engine is different
    if (_re && _re->descriptor().id == re) {
        qDebug() << "The new reverse engine, '" + re + "', is the same as the current one, ignoring...";
        return true;
    }

    //Create an instance of the new reverse engine
    ReverseEngine *r = CORE->pluginManager()->newReverseEngine(re);
    if (!r) {
        emit error(tr("Unable to load the reverse engine %1").arg(re));
        return false;
    }

    //Set or replace the reverse engine
    if (_re)
        delete _re;
    _re = r;

    //Set the curve model in the engine
    _re->setCurveModel(this);

    //Emit the appropriate signals
    emit reverseEngineChanged(re);
    emit changed();

    return true;
}

void CurveModel::setReverseEngine(ReverseEngine *engine)
{
    // ToDo///////////////////////////////////////////////////////////////////////
    // Possible memory leak below, check and delete the pe if already existing
    // ///////////////////////////////////////////////////////////////////////////

    //Ignore if identical
    if (_re == engine)
        return;

    //Update the engine
    _re = engine;
    _re->setCurveModel(this);

    //Emit the appropriate signals
    emit reverseEngineChanged(engine->descriptor().id);
    emit changed();
}

//Save them and clear the wanted parameters cache
void CurveModel::setParameters(const QMap<Duration, ParameterSet*> &parameters, ParameterType type)
{
    //Only accept manual and tmp parameters
    if (type != TMP && type != MANUAL) {
        WARNING(tr("Trying to set parameters of type %1, only accepting types %2 and %3, ignoring").arg(type).arg(MANUAL).arg(TMP));
        return;
    }

    //Clear cache
    resetCache(type);

    //Save
    _parameters[type] = parameters;
}

//Save them and clear the wanted parameters cache
void CurveModel::setParameters(ParameterSet* parameters, ParameterType type, Duration start)
{
    //Only accept manual and tmp parameters
    if (type != TMP && type != MANUAL) {
        WARNING(tr("Trying to set parameters of type %1, only accepting types %2 and %3, ignoring").arg(type).arg(MANUAL).arg(TMP));
        return;
    }

    //Clear cache
    resetCache(type, start);

    //Save
    _parameters[type].insert(Duration(0),parameters);
}

void CurveModel::setName(const QString &name)
{
    //Ignore if identical
    if (_name == name)
        return;

    //Update the name
    _name = name;

    //Emit the appropriate signals
    emit nameChanged(name);
    emit changed();
}

void CurveModel::setParameterType(ParameterType pType)
{
    //Ignore if the type is the same
    if (_pType == pType)
        return;

    //Update the parameters type
    _pType = pType;

    //Emit the appropriate signals
    emit parameterTypeChanged(pType);
    emit changed();
}

//--------------------------------- EVENTS ------------------------------------

//Reload the covariates from the database, and find out where the differences are
void CurveModel::covariatesChanged()
{
    //If the patient is not set, send a message and do nothing
    if (!_patient) {
        qDebug() << "Patient not set, no covariate update";
        return;
    }

    //Get the covariates from the database
    QList<SharedPatientVariate> list;
//    Response r = CORE->db()->getCovariatesByDate(_patient->id(),list);
    Tucuxi::Response r = NEWDB->getPatientVariatesByDate(_patient->id(),list);
    if (r.error != NotFound && r.error != NoError) {
        emit error(tr("Failed to fetch the patient's covariate"));
//        emit error(tr("Failed to fetch the patient's covariate (%1) from the database %2.\nError :\n%3")
//                   .arg(_patient->id()).arg(r.responder).arg(CORE->database()->errorString(r.error)));
        return;
    }

    //Will hold all the updated covariates sets and their associated durations
    QMap<Duration,PatientVariateSet*> map;

    //Fetch the drug's default covariates set from the drug model
    PatientVariateSet* covs = SharedPatientVariateSet(new PatientVariateSet(CORE->drugManager()->drug(_drugId)->covariates()));
//    CovariateSet* oldCovs = SharedPatientVariateSet(new CovariateSet());

    /* Build the covariates sets' map. The map will always have at least one covariates set, which is the one
     * containing the drug's default covariates and/or the patient's covariates with a date equal or lesser
     * than the first take. The map will hold a covariates set for each covariate after this date (unless there
     * is two or more covariates at the same date, in which case they will all be contained into the same set). */
    foreach (SharedPatientVariate c,list) {

        //A covariate must be corresponding to one of the drug's default covariate to be taken into account
        int place = covs->find(c->name());        

        //If it does correspond to this drug's covariate
        if (place == -1) {
            place = covs->find(c->name().toLower());
        }
        if (place != -1) {

            //Store the current covariates set            
            SharedPatientVariateSet oldCovs = SharedPatientVariateSet(new PatientVariateSet(covs));

            covs->at(place)->setValue(c->value());
            covs->at(place)->setDate(c->date());

            PatientVariateSet* covs2 = SharedPatientVariateSet(new PatientVariateSet(covs));
            //Replace the old covariate by the new one
//            covs->replace(place, c);

            //Calculate the time after the first drug take
            qint64 t = _firstTake.secsTo(c->date());

            //If the time is at or above 0, meaning we are in the analysis range
            if (t >= 0) {

                //If the map was empty, insert the previous set at the first position in the map
                if (map.isEmpty())                    
                     map.insert(Duration(0), oldCovs);

                //Insert the new set in the map at the corresponding duration
                map.insert(Duration(0, 0, t), covs2);
            }
        }
    }

    //If at the end, the map is empty (no covariates or corrsponding covariates), add the last ones
    if (map.isEmpty())
         map.insert(Duration(0),covs);

    // ToDo //////////////////////////////////////////////////////////////////////////////////////////////
    // The cache reset could be optimized with the following modification to the loop below:
    //    while (m != currentValues.constEnd() && p != _measures.constEnd()) {
    //        if (m.value() != p.value()) {
    //            resetTime = m.key() < p.key() ? m.key() : p.key();
    //            break;
    //        }
    //    etc...
    // The problem is that the cache will not delete the parameters at time 'resetTime', due to the current
    // implementation of the cache system. Therefore, we need to reset it to the time of the last match.
    // Might want to see that if the core is not refactored entirely. Also note the problem is similar
    // with the measuresChange function if we need to reimplement a reset from the last measure change.
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    //Update the APRIORI and APOSTERIORI caches
    Duration resetTime;
    QMap<Duration, PatientVariateSet*>::const_iterator m = map.constBegin();
    QMap<Duration, PatientVariateSet*>::const_iterator c = _covariates.constBegin();

    while (m != map.constEnd() && c != _covariates.constEnd()) {
        if (m.value().data() != c.value().data())
            break;
        resetTime = m.key();

        m++;
        c++;
    }

    //Invalidate the post engine
    if (resetTime < lastMeasureTime())
        invalidatePostEngine();

    resetCache(resetTime);

    //Set the new covariates
    _covariates = map;

    //Get the patient birthdate
    Duration daysFromBirthToFirstTake = Duration(_patient->person()->birthday().daysTo(firstTake().date()) * 24);

    //Reset the cache if it changed
    if (_daysFromBirthToFirstTake != daysFromBirthToFirstTake)
        resetCache();

    //Store the birthdate duration
    _daysFromBirthToFirstTake = daysFromBirthToFirstTake;

    //Set as modified
    emit covariatesModified();
    emit modified();
}

void CurveModel::clinicalsChanged()
{
    //If the patient is not set, send a message and do nothing
    if (!_patient) {
        qDebug() << "Patient not set, no clinical update";
        return;
    }

    //Get the covariates from the database
    QList<SharedClinical> list;
//    Response r = CORE->db()->getClinicalsByDate(_patient->id(),list);
    Tucuxi::Response r = NEWDB->getClinicalsByDate(_patient->id(),list);
    if (r.error != NoError && r.error != NotFound) {
        emit error(tr("Failed to fetch the patient's clinical"));
//        emit error(tr("Failed to fetch the patient's clinical (%1) from the database %2.\nError :\n%3")
//                   .arg(_patient->id()).arg(r.responder).arg(CORE->database()->errorString(r.error)));
        return;
    }

    ClinicalSet s;
    //Will hold all the updated clinicals sets and their associated durations
    QMap<Duration,ClinicalSet> map;

    //qDebug() << "list length : " << QString::number(list.length());

    foreach(SharedClinical c, list)
    {
        qint64 t = _firstTake.secsTo(c->date());

        QMap<Duration, ClinicalSet>::iterator place = map.find(t);
        if(place != map.end())
        {
            ClinicalSet cs = map.value(t);
            cs.append(c);
            map.insert(Duration(0,0,t), cs);
        }
        else
        {
            ClinicalSet cs;
            cs.append(c);
            map.insert(Duration(0,0,t), cs);
        }
    }

    //Copy the new covariates map
    _clinicals = map;

    //Send the appropriate signals
    emit clinicalsModified();
}

//Updates the measure and APOSTERIORI cache
void CurveModel::measuresChanged()
{
    //Check if the patient is valid
    if (!_patient) {
        qDebug() << "Patient not set, no measure update";
        return;
    }

    //Get the measures from the database
    QList<SharedMeasure> measuresList;
//    Response r = CORE->db()->getMeasuresByDrug(_patient->id(), _drugId, measuresList);
    Tucuxi::Response r = NEWDB->getMeasuresFromPatientAndDrug(_patient->id(), _drugId, measuresList);
    if (r.error != NoError && r.error != NotFound) {
        emit error(tr("Could not retrieve patient."));
//        emit error(tr("Could not retrieve patient ID '%1' measures from the database %2.\nError :\n%3").arg(_patient->id()).arg(r.responder).arg(CORE->database()->errorString(r.error)));
        return;
    }

    //Build the new measures map
    QMap<Duration, SharedMeasure> currentValues;

    // ToDo ////////////////////////////////////////////////////////////////////
    // The measures loaded from the database are converted to the default unit
    // directly below. It should be done only before calculation, and in a local
    // variable in order to preserve the unit chosen by the user.
    // /////////////////////////////////////////////////////////////////////////

    foreach (SharedMeasure m, measuresList) {
        if (m->moment() < _firstTake)
            continue;
        if (m->amount().unit() != concentrationUnit())
            m->convert(concentrationUnit());
        currentValues.insert(Duration(0, 0, _firstTake.secsTo(m->moment())), m);
    }

    //Invalidate the post engine
    invalidatePostEngine();

    //Set the new measures
    _measures = currentValues;

    //Set as modified
    emit measuresModified();
    emit modified();
}

//-----------------------------------------------------------------------------
//--------------------------------- PRIVATE -----------------------------------
//-----------------------------------------------------------------------------

//--------------------------------- CALCULATIONS ------------------------------
#include <iostream>

/* Calculates a prediction for a given interval, and update the residual and steady state caches. Every "time"
 * used is this function are relative to the curve's first take, and stored/manipulated as Duration objects.
 * Please note that the interval is specified by a global start time (outStart) and a global stop time (stop).
 * It is really the key function of this application, as pretty much any model and plugin will use it. */
DataSet CurveModel::calculate(const Duration &outStart, const Duration &stop, uint points, ParameterType type, bool cacheMode, const SharedDosage &dosage, bool uniform)
{
    //WARNING(QString("Calculate in %1 mode from %2 to %3 (%4 points)").arg(cacheMode?"cache":"normal").arg(outStart.toString(),stop.toString()).arg(points));

    /* Used to know if we need to calculate the residual points for the cache system. It will be the case for
     * every predictions except for the (non-cached) reverse engines predictions which will be using a specific dosage.
     * Please note that in that case, the cacheValidUntil parameter, the steady state and the residuals won't be updated
     * at all. Finally, the cacheMode parameter is true only when calculating the cache using the calculateCache() fun-
     * ction, which means that start and oldTime always match the beginning of a cycle. */
    bool doResidual = cacheMode || dosage->dose().value() == -1.0;

    // ToDo /////////////////////////////////////////////////////////////////////////////////////////
    // It is strange that we get the last residual time using the residuals table directly instead of
    // the cachevaliduntil value, which might expire sooner than the residuals themselves. This append
    // when the cache is not reseted using the resetcache() functions, but the curve is recalculated
    // for a duration smaller than it was previously calculated: in that case, the cachevalid_until
    // var is set to the new (shorted) duration, but the residuals still contain the old values.
    // It should not introduce any bug, but it's quite illogical: in those cases, the start time is
    // equal to the stop time, and therefore we do not enter the main loop of the function, and set
    // the cachevalid_until value to zero. It should only occure when calling calculateCache().
    /////////////////////////////////////////////////////////////////////////////////////////////////

    /* Find out the time of the last residual point in the cache. It will be different whether we are in cache
     * mode or not:
     * 1. In cache mode, we use the global stop time so that we get the latest available residual point for the
     *    given interval, which will prevent us from recalculating the previous residual points.
     * 2. In prediction mode, we use the global start time so that we get the time of the residual point
     *    that matches exactly the begining of the first cycle of the wanted prediction.
     * Please see the residual and firstTime variables' comments below for additional information. */
    Duration lastResidualTime = lastResidual(cacheMode ? stop : outStart, type);

    //WARNING(QString("Fetch %1").arg(lastResidualTime.toString()));

    //In cache mode, use the last residual time as start time, else use the global start
    Duration start = cacheMode ? lastResidualTime : outStart;

    // ToDo /////////////////////////////////////////////////////////////////////////////////////////
    // Possible bug with the residual value below if _residuals[type] is empty. It will still have a 0
    // value, but this could be a problem if the prediction start time doesn't match the start of the
    // curve (or in other words the first take time). Shouldn't occur if the cache is handled correctly.
    /////////////////////////////////////////////////////////////////////////////////////////////////

    /* Get the value of the initial residual point, given the lastResidualTime. This point will actually be used as
     * the initial point for our prediction if not in cache mode. In addition, please take note of the following:
     * 1. A residual point will ALWAYS match the begining of a cycle (which does not necessarily corresponds
     *    to the end of the previous cycle in the case of a dosage change in the middle of a cycle for exemple)
     * 2. Therefore, in the first iteration, it is important to understand that this initial residual point corresponds
     *    exactly to the first point of the first cycle of the prediction. That being said, the prediction itself
     *    might not start exactly at the begining of this first cycle, since the global start time might be somewhere
     *    in the middle of that cycle. This is explained in the firtTime variable's comments.
     * 3. Finally, after the first interation, this variable will be used to hold the current iteration residual point. */
    if (lastResidualTime.isEmpty())
        calculateInitialResidual(type, uniform);

    Residuals residual = _residuals[type].value(lastResidualTime, _initialResidual[type]);

    //Will be used as the previous iteration residual point
    Residuals previousResidual;

    //Will be used to store the initial residual point so that we can use it after the loop if needed
    Residuals oldResidual;
    oldResidual[0] = Residual(-1.0);

    //Used to store the global prediction, will be returned by the function
    DataSet result;

    //Set the prediction's total duration
    Duration duration = stop - start;

    //Will be used as the current iteration stop time
    Duration crtTime = start;

    //Will be used as the current iteration start time
    Duration oldTime;

    //Will be used as the previous iteration start time
    Duration oldOldTime;

    //Will be used as the next dosage application time
    Duration nextChangeTime;

    //Will be used as the current iteration dosage
    SharedDosage crtDosage = SharedDosage(new Dosage());

    //Will be used to accumulate the points remainder
    double rest = 0.0;

    //int cnt = 0;

    /* This variable will hold the difference between the initial residual point's time and the prediction's
     * global start time, if any. It is used to know if the prediction begins exactly on a cycle begining or not.
     * In other words, this variable is used when we need to calculate the first iteration's prediction when the
     * prediction doesn't match the begining of a cycle. Please note that in cache mode, this duration will always
     * be empty, so it's really only usefull for a real prediction. */
    Duration firstTime = start - lastResidualTime;

    /* If there is more than one point to calculate, and if the prediction starts exactly at the begining of the
     * first cycle, make room for the initial residual point that will be added as the first point of the prediction
     * (after the end of the loop). The nbPoints and */
    int nbPoints = points;

    if (points > 1 && firstTime.isEmpty()) {
        nbPoints--;             //Make room for the initial residual point
        oldResidual = residual; //Save the initial residual point for after the loop

        // ToDo /////////////////////////////////////////////////////////////////////////////////////////
        // Send an error if the initial residual point is negative, to remove when more testing is done
        /////////////////////////////////////////////////////////////////////////////////////////////////

        if (oldResidual.at(0).value < 0)
            ERROR(DATAERROR, tr("Negative initial residual point"));
    }

    //Calculate the entire prediction, which means all the points from the global start to the global stop
    while (crtTime < stop) {

        /* Set the previous iteration start time to the previous iteration current start and only used to
         * calculate the the steadstate (see the ToDo remark before the steadystate condition below) */
        oldOldTime = oldTime;

        //Set the current iteration start time to the previous iteration stop time
        oldTime = crtTime;

        //Get the current dosage (unless making a prediction for a reverse engine)
        crtDosage = dosageAtTime(oldTime, dosage);

        //Get the next dosage change time, and keep it for calculating the steady states and residuals
        nextChangeTime = nextDosageTime(oldTime);

        //Temporary set the current stop time as the next dosage change time
        crtTime = nextChangeTime;

        /* If there is no next change (which means the nextChangeTime was empty), or if the next change time is
         * after the prediction global stop time, (temporary) set the current stop time to the global stop time. */
        if (crtTime.isEmpty() || crtTime > stop)
            crtTime = stop;

        /* Set the current stop time if it's after the next cycle begining. This is done because at each iteration, the model
         * cannot calculate a prediction that is longer than a cycle: it must be either exactly the same duration of a cycle,
         * or either shorter. */
        if (firstTime.isEmpty()) {

            /* If the firsTime duration is empty, it means we are not in the first iteration, or that the first iteration's
             * prediction corresponds to the begining of a cycle. We therefore set the current stop time to the end of the
             * current cycle/start of the next one, only if the current stop time is after the begining of the next cycle. */
            if (crtTime - oldTime > crtDosage->interval())
                crtTime = oldTime + crtDosage->interval();
        } else {

            /* If the firstTime duration is not empty, it means that we are in the first iteration and that the prediction doesn't start
             * at the begining of a cycle. We therefore set the current stop time at the initial residual time plus the current dosage interval,
             * which corresponds to the start of the next cycle. */
            if (crtTime - lastResidualTime > crtDosage->interval())
                crtTime = lastResidualTime + crtDosage->interval();
        }

        //If the current stop time is after the gloabl stop time, set it to the global stop time
        if (crtTime > stop)
            crtTime = stop;

        //Calculate the theorical number of points for this iteration
        double dp = ((crtTime - oldTime).toSeconds() / duration.toSeconds()) * nbPoints;

        //Get the real number of point for this iteration
        int crtPoints = cacheMode ? 1 : floor(dp);

        //Save the remainder (decimal points) which will be used in the next iteration
        rest = dp - crtPoints + rest;

        //If the rest is bigger or almost equal to a point, add it to this iteration
        if (rest >= 0.99999 && ! cacheMode) {
            crtPoints++;
            rest -= 1.0; //Take away the virtual point from the remainder
        }

        //Save this iteration's initial residual point for later
        previousResidual = residual;

        //WARNING(QString("From %1 to %2 in %3 points").arg(oldTime.toString(),crtTime.toString()).arg(crtPoints));

        //Convert the current dosage unit to match the concentration default unit
        crtDosage->convert(concentrationUnit());

        /* Ask the model to calculate the points (make a predicition) for this iteration. The interval cannot
         * be longer than a cycle (which corresponds to the current dosage interval). The first argument corresponds
         * to the duration between the begining of the cycle and the begining of the prediction, if it does not correpond.
         * Again, this can only occurs during the first iteration. The second parameter correspond to the prediction duration,
         * taking into account the first argument (that is, the cyle duration minus firstime, unless the crtTime doesn't match
         * doesn't match the end of the current cycle). Then, the parametersAtTime() function must return the parameters
         * set exactly at or before the current cycle, which is why we call the prevCycle function in case of oldTime not
         * matching the begining of the current cycle. Finally, the last argument is the residual point which corresponds to
         * the first point of this cycle's prediction, whether the prediction start or not at the begining of the cycle. */
        DataSet data = _model->predict(firstTime,
                                       crtTime - oldTime,
                                       parametersAtTime(prevCycle(oldTime), type),
                                       qMax(crtPoints, 1),     //Calculate at least 1 point
                                       crtDosage,
                                       residual,
                                       uniform);


        //Check if the residual is valid
        if (std::isnan(residual.at(0).value))
            ERROR(DATAERROR, tr("The last residual returned by the model>'s prediction is not a valid number"));
        if (std::isinf(residual.at(0).value))
            ERROR(DATAERROR, tr("The last residual returned by the model's prediction has an infinite value"));

        //Start and stop dates
        QDateTime startDate = this->firstTake().addSecs(oldTime.toSeconds());
        QDateTime stopDate = this->firstTake().addSecs(crtTime.toSeconds());

        // ToDo /////////////////////////////////////////////////////////////////////////////////////////
        // Another very ugly way to deal with a problem. To understand what is going on below, you need
        // to take a look in the infusion models. When firsttime < Tinfusion < firsttime + crtTime -
        // oldtime, or in other words the infusion time occures in the current prediction, we will create
        // two cycles instead of one. But since we can't set the time in the predict function unless we
        // pass the date and some other stuff, we need to set the time for the two cycles in the returned
        // cycle (yea, so logical) right after the call to predict. This is what is done below.
        // ///////////////////////////////////////////////////////////////////////////////////////////////

        //Set the start and end date for the result
        if (CORE->definitionManager()->isInfusion(model()->descriptor().intake) && data.allCycles().size() == 2) {

            //In case of an infusion, compute the infusion date
            QDateTime infuDate = startDate.addSecs(crtDosage->tinf().toSeconds() - firstTime.toSeconds());

            //Set the start and end date for both internal cycles
            data.cycleData(0).setTimes(startDate, infuDate, CycleData::START);
            data.cycleData(1).setTimes(infuDate,  stopDate, CycleData::END);

            //If room was made for the initial residual point, or if partial cycle
            if (((oldTime == start) && (points > 1) && (firstTime.isEmpty())) || (!firstTime.isEmpty()))
                data.cycleData(0).setType(CycleData::INTERMEDIATE);
        } else {

            //In the other cases, simply set the start and stop dates
            data.setTimes(startDate, stopDate);

            //If room was made for the initial residual point, or if partial cycle
            if (((oldTime == start) && (points > 1) && (firstTime.isEmpty())) || (!firstTime.isEmpty()))
                data.cycleData(0).setType(CycleData::END);
        }

        //Set the resulting dataset (unless there was nothing to calculate for this iteration)
        if (crtPoints != 0) {

            //If the global result is empty or if we are in cache mode, set it, otherwise combine it
            if (cacheMode || result.size() == 0)
                result = data;
            else
                result = result + data;
        }

        // ToDo /////////////////////////////////////////////////////////////////////////////////////////
        // Possible bug with the if condition below. See the special case in the comments below, might
        // be possible to avoid it by first checking if the current stop time is different than stop, and
        // if and only if it's not (crtTime == stop), then we check if the current stop time corresponds
        // to the end of a cycle (the last part of the condition, oldT + crtD == crtT).
        /////////////////////////////////////////////////////////////////////////////////////////////////

        /* Save the residual point if the cache needs to be calculated, and calculate the steady state, if:
         * 1. We're not making a (non-cached) prediction for a reverse engine (which means doResidual is true)
         * 2. The current stop time is different from the global stop time (which could be before the end of
              the current cycle), or the current stop time corresponds to the begining of the next cycle
         * This last condition means we are sure that the current stop time corresponds to the begining of the
         * next cycle (but not necessarily the end of the current one if there was a dosage change in the middle
         * of it). We can therefore add this iteration prediction's last point to the residuals cache.
         * There is a special case when we are in the first iteration with a prevision that doesn't correspond
         * to the begining of a cycle, but ends at the end of the same cyle: in this case we do not save the residual. */
        if (doResidual && (crtTime != stop || oldTime + crtDosage->interval() == crtTime) ) {

            //Save the residual at the current stop time, which correspond to the begining of the next cycle
            _residuals[type].insert(crtTime, residual);

            /* We then check if we are in a steady state. For this, we check if the new residual point is almost
             * identical to the previous one, given a tolerance margin. If it is, we first check if we can add
             * the steady state to the cache, and then eventually set the next residual points if we are in cache mode. */

            double upperMargin = previousResidual.at(0).value + previousResidual.at(0).value * _tolerance;
            double lowerMargin = previousResidual.at(0).value - previousResidual.at(0).value * _tolerance;

            if (residual.at(0).value <= upperMargin && residual.at(0).value >= lowerMargin) {
                //Get the next dosage change that comes after the previous iteration start time
                //Duration previousChangeTime = nextDosageChange(oldOldTime);

                // ToDo /////////////////////////////////////////////////////////////////////////////////////////
                // The conditions below about the previous dosage change are quite strange, it means that if there
                // is a previous steady state in the cache, and then a dosage change, we only set the new steady
                // state if that dosage change occured during the current cycle or the previous one. I do not see
                // the reason why... Mabye to prevent to add the same steady state at each cycle prediction, but
                // there is still the possible bug explained here, that is the new steady state is never found as
                // a new dosage will make the test above fail (previous residual won't be close to the current one).
                // And when it finally matches, the previous change time is empty (unless there is a dosage change
                // occuring later on, making the current steady state obsolete and useless), making the test fails.
                // Other exceptions: when the steady state list is empty (most likely only when creating a new curve).
                // or when calculating one cycle when already in steady state (when calling peak from TargetWidget,
                // oldOldTime is null, and a new steady state is added right after the one existing).
                //
                // Also note that what is explained above only occures when doing a prediction for a curve (in cache
                // mode or not). There is other cases like for exemple:
                // 1. Calling the steadyState function with an invalid dosage (typically called by the targetWidget,
                // the dosageDialog and the DosageWizard) means that oldOldTime will "always" (not sure) be 0, as we
                // only do one iteration, leading to a default initialized oldOldTime duration (0). In this case, we
                // are sure to add the steady state if it is searched at or after the last dosage time.
                // 2. Calling the steadyState function by a prediction engine, using a temporary dosage. In this
                // case, doResidual is false and oldOldTime is never used: the cache and steady state list are not
                // updated.
                //
                // There might be some situation different than the one described above, but difficult to list all
                // of them. In any case, this mecanism is deficient.
                /////////////////////////////////////////////////////////////////////////////////////////////////

                /* Add the steady state to the cache only if there is no previous steady state, or if:
                 * 1. the previous steady states comes before the current stop time (should always be true is the cache
                 *    was reseted correctly before making this prediction), and
                 * 2. the previous dosage change comes before or at the current stop time, and
                 * 3. the previous dosage change occured after the previous iteration start time */
                //if (_steadyStates.isEmpty() || (_steadyStates.last() < crtTime && crtTime >= previousChangeTime && !previousChangeTime.isEmpty()))
                //    _steadyStates.append(crtTime);

                // ToDo /////////////////////////////////////////////////////////////////////////////////////////
                // Below a very ugly way to fix the issue described above. This allow to get the correct steady
                // state when a covariate is modified. But we still have the issue of adding multiple and un-
                // necessary steady-states. Will have to get fixed when refactoring the curve model.
                /////////////////////////////////////////////////////////////////////////////////////////////////

                Duration nextCovariate = nextCovariateChange(crtTime);              // The next covariate change is taken from the end of the current cycle
                Duration nextDosage = nextDosageTime(oldTime);                    // The next dosage change is taken from the start of the current cycle
                Duration prevCovariate = nextCovariateChange(oldTime);              // This is used to check if there was a covariate change during the current cycle

                if (crtTime > nextDosage &&                                         // Be sure there is no further dosage changes
                    crtTime > nextCovariate &&                                      // Be sure there is no further covaraite changes
                    !(oldTime < prevCovariate && prevCovariate <= crtTime) &&       // Manage the case when a covariate is change during the current cycle
                    !_steadyStates[type].contains(crtTime) &&                             // Do not add duplicates, as it can and will happen
                    (_steadyStates[type].isEmpty() || _steadyStates[type].last() < crtTime))    // Do not add intermediate steady states when possible
                    _steadyStates[type].append(crtTime);

                //Will be used as the next covariate/parameter change time (in this case, oldTime always matches the beginning of a cycle)
                Duration nextCovariateTime = nextCovariateChange(oldTime);

                /* If we are in cache mode, and if:
                 * 1. the end of the next iteration cycle comes before the global stop time, and
                 * 2. there is no dosage change in the next cycles, or that next change doesn't occur in the next cycle,
                 * 3. there is no covariate change in the next cycles, or that next change doesn't occur in the next cycle,
                 * then we optimize the cache calculation by just setting the next cycles residuals points to the actual
                 * residual points. It makes sense because we are in steady state, so the residual will not change unless
                 * there is a change in the dosages or in the covariates. We also need to update the current stop time
                 * accordingly. */
                while (cacheMode
                       && (crtTime + crtDosage->interval()) < stop
                       && (nextChangeTime.isEmpty() || (crtTime + crtDosage->interval()) < nextChangeTime)
                       && (nextCovariateTime.isEmpty() || (crtTime + crtDosage->interval()) < nextCovariateTime)) {

                    //Update the next stop time
                    crtTime += crtDosage->interval();

                    //Set the next residual point in the cache
                    _residuals[type].insert(crtTime,residual);
                }
            }
        }

        /* Clear the firstTime since every iterations except the first one will correspond to the begining
         * of a new cycle. */
        firstTime.clear();
    }

    //Once the prediction is done, we need to set the caches time's validity and set the last residual point
    if (doResidual) {
        if (crtTime == nextChangeTime || (crtTime-oldTime) == crtDosage->interval()) {

            // ToDo /////////////////////////////////////////////////////////////////////////////////////////
            // Redisual might be empty at this point. It should only occure when calling steady-state when
            // creating a new curve, and when that curve has a measure that comes before the first take. It
            // is because the _cacheValidUntil is negative, due to the measure updating it with a negative
            // value: the first updateCache() call in steadyState() will call calculate() with 0-0 range,
            // leading to an empty residual. Should not occure beside this exception, so a correct way to fix
            // this might be to prevent a negative cacheValidUntil value and an ERROR() below. For the time
            // being, only a WARNING is displayed as preventing a negative value might introduce a bug. Also
            // remove the Q_ASSERT below when we're sure it can't happen in any other case.
            /////////////////////////////////////////////////////////////////////////////////////////////////

            if (!residual.size()) {
                Q_ASSERT(start.mSecs() == 0);
                WARNING(tr("Empty residual vector added to the residuals at time '%1' with cacheValidUntil value of '%2' (should be ignored if time is 0 and if 'cacheValidUntil' is negative)").arg(crtTime.toString(), _cacheValidUntil[type].toString()));
            }

            /* If the current stop time corresponds exactly to the begining of the next cycle, then add the last point
             * of the prediction to the residual cache, and set the caches time validity to the current stop time. */
            _residuals[type].insert(crtTime,residual);
            _cacheValidUntil[type] = crtTime;

        } else

            /* Else just update the caches time validity to the current start time, which corresponds to the begining
             * of the previous cycle. */
            _cacheValidUntil[type] = oldTime;
    }

    //If a room was made for the initial residual point, add it as the first point of the prediction
    if (points > 1 && oldResidual.at(0).value >= 0) {
        DataSet tmp(1, result.unit());
        QDateTime startDate = this->firstTake().addSecs(start.toSeconds());
        tmp.setTimes(startDate, startDate, CycleData::START);
        tmp[0] = oldResidual.at(0).value;
        result = tmp + result;
    }

    //Set the data unit to the drug's concentration default unit
    result.setUnit(concentrationUnit());

    //Finally return the prediction in the form of a single data set
    return result;
}

//Use calculate in cache mode
void CurveModel::calculateCache(ParameterType type, const Duration &until)
{
    //If the wanted time is before the current cache state, do nothing
    if (until <= _cacheValidUntil[type])
        return;

    //Cache mode
    calculate(Duration(), until, 1, type, true);
}

//Calculate the initial concentration of the curve
void CurveModel::calculateInitialResidual(const ParameterType &type, bool uniform)
{
    //Check if the curve is partial
    if (curveType() == Continous)
        return;

    //Check if the residual is empty
    if (_initialResidual[type].size())
        return;

    //The error messages first part
    QString failed = tr("Could not compute the initial concentration, ");

    //Check if the model is valid
    if (!_model)
        ERROR(NOMODEL, failed + tr("the model is missing"));

    //Check if the first dose exists
    if (!_dosages.firstKey().isEmpty())
        ERROR(DATAMISSING, failed + tr("the first dosage is missing"));

    //Get the first dosage
    SharedDosage initial = dosageAtTime(Duration());
    initial->convert(concentrationUnit());

    //Get the half-life
    Duration halflife = CORE->drugManager()->drug(_drugId)->halflife()->halfLife();

    //Get the multiplier
    int multiplier = CORE->drugManager()->drug(_drugId)->halflife()->multiplier();

    //Update the parameters
    calculateParameters(type, initial->interval());

    //Get the steady state
    Residuals residual;

    DataSet result = _model->steadystate(Duration(), initial->interval(), parametersAtTime(Duration(), type), initial, halflife, multiplier, 1, residual, uniform);

    if (!result.size())
        ERROR(DATAERROR, failed + tr("the steady state value could not be found"));
    if (std::isnan(residual.at(0).value))
        ERROR(DATAERROR, failed + tr("the residual is not a valid number"));
    if (std::isinf(residual.at(0).value))
        ERROR(DATAERROR, failed + tr("the residual has an infinite value"));

    //Set the residual
    _initialResidual[type] = residual;
}

// ToDo //////////////////////////////////////////////////////////////////////////////////////////////
// The algorithm for computing the parameters could be significantly simplified if we had an isValid()
// function for the durations. Currently, an invalid duration is a null duration, which means that we
// cannot know if nextCovariateChange() and nextMeasureChange() return an empty duration because there
// is no next change (invalid duration), or because the next change is at the firsttake (null duration).
// If it is decided to change this, an invalid duration should also be considered null, but a null du-
// ration should be considered as valid.
// Also, the cache should be revamped. Currently we have to deal with two differents cases:
// 1. The cache is empty (parametersValidUntil at duration 0), and we have to recalculate the para-
//    meters including the 0 duration.
// 2. The cache is not emtpy (parametersValudUntil at duration x), and we don't have to recalculate the
//    the parameters exactly at x, which differs from the case above.
// This is due to the way the cache is reset, and when we reset it when a covariate and measure change.
//////////////////////////////////////////////////////////////////////////////////////////////////////

//Updates the parameters
void CurveModel::calculateParameters(ParameterType type, const Duration &until)
{
    //Check if the type is relevant
    if (type != APRIORI && type != APOSTERIORI)
        return;

    //Check if the cache is up to date
    if (until <= _parametersValidUntil[type])
        return;

    //If the cache is not empty, check if we need to calculate anything
    if (!_parametersValidUntil[type].isEmpty()) {
        Duration nextCovariate = nextCovariateChange(_parametersValidUntil[type]);
        Duration nextDay = nextDayTime(_parametersValidUntil[type]);

        //Ignore if there is no change between the cache and the next covariate change
        if ((nextCovariate.isEmpty() && (!withPna() || (nextDay > until))) && type == APRIORI) {
            _parametersValidUntil[APRIORI] = until;
            return;
        }

        //Ignore if there is no change between the cache and the next measure and covariate change
        if ((nextCovariate.isEmpty() && (!withPna() || (nextDay > until))) && type == APOSTERIORI) {
            _parametersValidUntil[APRIORI]     = until;
            _parametersValidUntil[APOSTERIORI] = until;
            return;
        }
    }

    //Extend the APRIORI duration if eta needs to be calculated
    Duration apriori;
    if (type == APOSTERIORI && _poInvalidated)
        apriori = qMax(until, lastMeasureTime());

    //Update the APRIORI parameters for the appropriate duration
    calculateAprioriParam(apriori.isEmpty() ? until : apriori);

    //Update the APOSTERIORI parameters and the post engine eta
    if (type == APOSTERIORI) {

        //Update the post engine eta if necessary
        if (_poInvalidated) {
            _po->calculateEta(parameters(Duration(), lastMeasureTime(), APRIORI), _measures.values());
            _poInvalidated = false;
        }

        //Update the APOSTERIORI parameters
        _po->aPosterioriParameters(_parameters[APRIORI], _parameters[APOSTERIORI]);

        //Set the APOSTERIORI parameters type
        QMap<Duration, ParameterSet*>::iterator i;
        for (i = _parameters[APOSTERIORI].begin(); i != _parameters[APOSTERIORI].end(); i++)
            i.value()->setType(APOSTERIORI);
    }

    //Update the parameters validity
    switch (type) {
    case APOSTERIORI:
        _parametersValidUntil[APOSTERIORI] = until;
    case APRIORI:
        if (until > _parametersValidUntil[APRIORI])
            _parametersValidUntil[APRIORI] = apriori.isEmpty() ? until : apriori;
        break;
    default:
        ERROR(DATAERROR, tr("Invalid curve type"));
    }
}

//Return true if the current model uses the PNA
bool CurveModel::withPna()
{
    QString modelId = _model->descriptor().id;
    if (_lastModelThatAskedForPna == modelId)
        return _lastAnsweredWithPna;

    return _lastAnsweredWithPna = CORE->drugManager()->drug(_drugId)->withPna();
}

//Updates the APRIORI parameters
void CurveModel::calculateAprioriParam(const Duration &until)
{
    //The next covariate change time
    Duration nextCovariate;

    //The next day change time
    Duration nextDay;

    //The more covariates flag
    bool moreCovariates = true;

    //The more PNA changes flag
    bool morePNA;

    //If the cache is not empty, get the next covariate change time relative to the cache
    if (!_parametersValidUntil[APRIORI].isEmpty()) {
        nextCovariate = nextCovariateChange(_parametersValidUntil[APRIORI]);

        //Stop if there is no next covariates and no PNA
        if (nextCovariate.isEmpty() && !withPna())
            return;

        //Continue with the PNA if there is no next covariates
        if (nextCovariate.isEmpty())
            moreCovariates = false;

        //Set the next day if the model has a PNA covariate
        if (withPna())
            nextDay = nextDayTime(_parametersValidUntil[APRIORI]);
    }

    //Calculation for a model without PNA
    if (!withPna()) {
        while (nextCovariate <= until) {

            //Insert the parameters set for the current covariate
            _parameters[APRIORI].insert(nextCovariate, CORE->drugManager()->drug(_drugId)->apriori(_covariates.value(nextCovariate)));

            //Search for the next covariate change time, if any
            if ((nextCovariate = nextCovariateChange(nextCovariate)).isEmpty())
                break;
        }

        //Calculation done for a model without PNA
        return;
    }

    //Check if there is more PNA changes
    morePNA = isPnaValid(nextDay);

    //Calculation for a model with PNA
    while ((moreCovariates && nextCovariate <= until) || (morePNA && nextDay <= until)) {

        //Case with PNA changes only
        if (!moreCovariates) {

            //Insert the parameters set for the current PNA
            _parameters[APRIORI].insert(nextDay, CORE->drugManager()->drug(_drugId)->apriori(currentCovariates(nextDay), _daysFromBirthToFirstTake + nextDay));

            //Set the next day time
            nextDay.addHours(24);

        //Case with covariates changes only
        } else if (!morePNA) {

            //Insert the parameters set for the current covariates
            _parameters[APRIORI].insert(nextCovariate, CORE->drugManager()->drug(_drugId)->apriori(_covariates.value(nextCovariate), _daysFromBirthToFirstTake + currentDayTime(nextCovariate)));

            //Set the next covariates change time
            nextCovariate = nextCovariateChange(nextCovariate);

        //Case with both covariates and PNA changes
        } else {

            //Get the next change time
            Duration nextChange = nextCovariate < nextDay ? nextCovariate : nextDay;

            //Insert the parameters set for the current change
            _parameters[APRIORI].insert(nextChange, CORE->drugManager()->drug(_drugId)->apriori(currentCovariates(nextChange), _daysFromBirthToFirstTake + currentDayTime(nextChange)));

            //Set the next change time according to the most recent change
            if (nextCovariate < nextDay)        //Set the next covariates change time
                nextCovariate = nextCovariateChange(nextCovariate);
            else if (nextDay < nextCovariate)   //Set the next day time
                nextDay.addHours(24);
            else {                              //Set both next day and covariates changes
                nextCovariate = nextCovariateChange(nextCovariate);
                nextDay.addHours(24);
            }
        }

        //Check if there is more changes
        if (moreCovariates && nextCovariate.isEmpty())
            moreCovariates = false;
        if (morePNA && !isPnaValid(nextDay))
            morePNA = false;
    }
}

// ToDo //////////////////////////////////////////////////////////////////////////////////////////////
// Check with the CHUV if the way the APOSTERIORI parameters are calculated correctly. More precisely,
// make sure the range passed to the post-engine, currently not used by the bayesian engine, is the
// correct one.
//////////////////////////////////////////////////////////////////////////////////////////////////////

//Updates the APOSTERIORI parameters
void CurveModel::calculateAposterioriParam(const Duration &until)
{
    bool calculateAPosteriori = false;

    //Get the list of the measures tied to this curve
    QList<SharedMeasure> measures = _measures.values();

    //Get the time from which the paramaters need to be calculated
    Duration start = _parametersValidUntil[APOSTERIORI] < _parametersValidUntil[APRIORI] ? _parametersValidUntil[APOSTERIORI] : _parametersValidUntil[APRIORI];

    //Set the iterator at the start of the map or the next element
    QMap<Duration, ParameterSet*>::const_iterator i;
    if (start.isEmpty())
        i = _parameters[APRIORI].constBegin();
    else
        i = _parameters[APRIORI].upperBound(start);

    //Iterate through the a priori parameters sets
    while (i != _parameters[APRIORI].constEnd() && i.key() <= until) {

        //Simply copy the APRIORI param if there is no measures
        if (measures.isEmpty()) {
            ParameterSet* postParams = SharedParameterSet(new ParameterSet(i.value()));
            postParams->setType(APOSTERIORI);
            _parameters[APOSTERIORI].insert(i.key(), postParams);
        }

        //If there is, query the post engine for the APOSTERIORI params
        else {
            if (!calculateAPosteriori) {
                emit startHugeCalculation();
                calculateAPosteriori = true;
            }
            ParameterSet* postParams = _po->aposteriori(i.value(), measures, Duration(), i.key());
            postParams->setType(APOSTERIORI);
            _parameters[APOSTERIORI].insert(i.key(), postParams);
        }

        //Move to the next APRIORI parameter
        i++;
    }

    //Terminate the huge calculation operation
    if (calculateAPosteriori)
        emit endHugeCalculation();
}

//--------------------------------- ENGINE HELPERS ----------------------------

//Load the model options
bool CurveModel::getModelOptions()
{
    QList<Option> options;

    //Get the options from the database
    if (!DATABASE->tryRequest(DB->getModelOptions(_model->descriptor().id, _id, options), tr("Could not load options for model ID '%1' and curve ID '%2'").arg(_model->descriptor().id, QString::number(_id)), _errors))
        return false;

    //Set the model options
    foreach (Option option, options) {

        //Check if exists
        if (!_model->optionExists(option.oid)) {
            _errors = tr("The option '%1' is not compatible with the model '%2'").arg(option.oid, _model->descriptor().id);
            return false;
        }

        //Set the value
        if (!_model->setOptionValue(option.oid, option.value)) {
            _errors = tr("Failed to set the value '%1' of option '%2' in the model '%3'").arg(option.value.toString(), option.oid, _model->descriptor().id);
            return false;
        }

        //Set the database ID
        if (option.sqlId == invalid_ident || !_model->setOptionDbId(option.oid, option.sqlId)) {
            _errors = tr("Failed to set the database ID '%1' for option '%2' in the model '%3'").arg(QString::number(option.sqlId), option.oid, _model->descriptor().id);
            return false;
        }
    }

    return true;
}

//Save the model options
bool CurveModel::setModelOptions()
{
    foreach (Option option, _model->getOptions()) {

        //Save the current option
        if (!DATABASE->tryRequest(DB->setModelOption(_model->descriptor().id, _id, option), tr("Could not save option ID '%1' for model ID '%2' and curve ID '%3'").arg(QString::number(option.sqlId), _model->descriptor().id, QString::number(_id)), _errors))
            return false;

        //Set the option database ID
        if (!_model->setOptionDbId(option.oid, option.sqlId)) {
            _errors = tr("Failed to set the database ID '%1' for option '%2' in model '%3' of curve ID '%4'").arg(QString::number(option.sqlId), option.oid, _model->descriptor().id, QString::number(_id));
            return false;
        }
    }

    return true;
}

//Delete the models options
bool CurveModel::deleteModelsOptions()
{
    //Delete all the options associated with this curve
    if (!DATABASE->tryRequest(DB->deleteModelsOptions(_id), tr("Error while trying to delete the curve's models options"), _errors))
        return false;

    //Reset the current model options database ID
    foreach (Option option, _model->getOptions()) {
        if (!_model->setOptionDbId(option.oid, invalid_ident)) {
            _errors = tr("Failed to reset the database ID of option '%1' in model '%2' of curve ID '%3'").arg(option.oid, _model->descriptor().id, QString::number(_id));
            return false;
        }
    }

    return true;
}

bool CurveModel::getPEOptions()
{
    QList<Option> options;

    //Get the options from the database
    if (!DATABASE->tryRequest(DB->getPEOptions(_pe->descriptor().id, _id, options), tr("Could not load options for percentile engine ID '%1' and curve ID '%2'").arg(_pe->descriptor().id, QString::number(_id)), _errors))
        return false;

    //Set the percentiles options
    foreach (Option option, options) {

        //Check if exists
        if (!_pe->optionExists(option.oid)) {
            _errors = tr("The option '%1' is not compatible with the percentile engine '%2'").arg(option.oid, _pe->descriptor().id);
            return false;
        }

        //Set the value
        if (!_pe->setOptionValue(option.oid, option.value)) {
            _errors = tr("Failed to set the value '%1' of option '%2' in the percentile engine '%3'").arg(option.value.toString(), option.oid, _pe->descriptor().id);
            return false;
        }

        //Set the database ID
        if (option.sqlId == invalid_ident || !_pe->setOptionDbId(option.oid, option.sqlId)) {
            _errors = tr("Failed to set the database ID '%1' for option '%2' in the percentile engine '%3'").arg(QString::number(option.sqlId), option.oid, _pe->descriptor().id);
            return false;
        }
    }

    return true;
}

bool CurveModel::setPEOptions()
{
    foreach (Option option, _pe->getOptions()) {

        //Save the current option
        if (!DATABASE->tryRequest(DB->setPEOption(_pe->descriptor().id, _id, option), tr("Could not save option ID '%1' for percentile engine ID '%2' and curve ID '%3'").arg(QString::number(option.sqlId), _pe->descriptor().id, QString::number(_id)), _errors))
            return false;

        //Set the option database ID
        if (!_pe->setOptionDbId(option.oid, option.sqlId)) {
            _errors = tr("Failed to set the database ID '%1' for option '%2' in percentile engine '%3' of curve ID '%4'").arg(QString::number(option.sqlId), option.oid, _pe->descriptor().id, QString::number(_id));
            return false;
        }
    }

    return true;
}

bool CurveModel::deletePEsOptions()
{
    //Delete all the options associated with this curve
    if (!DATABASE->tryRequest(DB->deletePEsOptions(_id), tr("Error while trying to delete the curve's percentiles engines options"), _errors))
        return false;

    //Reset the current percentiles engine options' database ID
    foreach (Option option, _pe->getOptions()) {
        if (!_pe->setOptionDbId(option.oid, invalid_ident)) {
            _errors = tr("Failed to reset the database ID of option '%1' in percentile engine '%2' of curve ID '%3'").arg(option.oid, _pe->descriptor().id, QString::number(_id));
            return false;
        }
    }

    return true;
}

bool CurveModel::getPOOptions()
{
    QList<Option> options;

    //Get the options from the database
    if (!DATABASE->tryRequest(DB->getPOOptions(_po->descriptor().id, _id, options), tr("Could not load options for post engine ID '%1' and curve ID '%2'").arg(_po->descriptor().id, QString::number(_id)), _errors))
        return false;

    //Set the post engine options
    foreach (Option option, options) {

        //Check if exists
        if (!_po->optionExists(option.oid)) {
            _errors = tr("The option '%1' is not compatible with the post engine '%2'").arg(option.oid, _po->descriptor().id);
            return false;
        }

        //Set the value
        if (!_po->setOptionValue(option.oid, option.value)) {
            _errors = tr("Failed to set the value '%1' of option '%2' in the post engine '%3'").arg(option.value.toString(), option.oid, _po->descriptor().id);
            return false;
        }

        //Set the database ID
        if (option.sqlId == invalid_ident || !_po->setOptionDbId(option.oid, option.sqlId)) {
            _errors = tr("Failed to set the database ID '%1' for option '%2' in the post engine '%3'").arg(QString::number(option.sqlId), option.oid, _po->descriptor().id);
            return false;
        }
    }

    return true;
}

bool CurveModel::setPOOptions()
{
    foreach (Option option, _po->getOptions()) {

        //Save the current option
        if (!DATABASE->tryRequest(DB->setPOOption(_po->descriptor().id, _id, option), tr("Could not save option ID '%1' for post engine ID '%2' and curve ID '%3'").arg(QString::number(option.sqlId), _po->descriptor().id, QString::number(_id)), _errors))
            return false;

        //Set the option database ID
        if (!_po->setOptionDbId(option.oid, option.sqlId)) {
            _errors = tr("Failed to set the database ID '%1' for option '%2' in post engine '%3' of curve ID '%4'").arg(QString::number(option.sqlId), option.oid, _po->descriptor().id, QString::number(_id));
            return false;
        }
    }

    return true;
}

bool CurveModel::deletePOsOptions()
{
    //Delete all the options associated with this curve
    if (!DATABASE->tryRequest(DB->deletePOsOptions(_id), tr("Error while trying to delete the curve's post engines options"), _errors))
        return false;

    //Reset the current post engine options' database ID
    foreach (Option option, _po->getOptions()) {
        if (!_po->setOptionDbId(option.oid, invalid_ident)) {
            _errors = tr("Failed to reset the database ID of option '%1' in post engine '%2' of curve ID '%3'").arg(option.oid, _po->descriptor().id, QString::number(_id));
            return false;
        }
    }

    return true;
}

bool CurveModel::getREOptions()
{
    QList<Option> options;

    //Get the options from the database
    if (!DATABASE->tryRequest(DB->getREOptions(_re->descriptor().id, _id, options), tr("Could not load options for reverse engine ID '%1' and curve ID '%2'").arg(_re->descriptor().id, QString::number(_id)), _errors))
        return false;

    //Set the reverse engine options
    foreach (Option option, options) {

        //Check if exists
        if (!_re->optionExists(option.oid)) {
            _errors = tr("The option '%1' is not compatible with the reverse engine '%2'").arg(option.oid, _re->descriptor().id);
            return false;
        }

        //Set the value
        if (!_re->setOptionValue(option.oid, option.value)) {
            _errors = tr("Failed to set the value '%1' of option '%2' in the reverse engine '%3'").arg(option.value.toString(), option.oid, _re->descriptor().id);
            return false;
        }

        //Set the database ID
        if (option.sqlId == invalid_ident || !_re->setOptionDbId(option.oid, option.sqlId)) {
            _errors = tr("Failed to set the database ID '%1' for option '%2' in the reverse engine '%3'").arg(QString::number(option.sqlId), option.oid, _re->descriptor().id);
            return false;
        }
    }

    return true;
}

bool CurveModel::setREOptions()
{
    foreach (Option option, _re->getOptions()) {

        //Save the current option
        if (!DATABASE->tryRequest(DB->setREOption(_re->descriptor().id, _id, option), tr("Could not save option ID '%1' for reverse engine ID '%2' and curve ID '%3'").arg(QString::number(option.sqlId), _re->descriptor().id, QString::number(_id)), _errors))
            return false;

        //Set the option database ID
        if (!_re->setOptionDbId(option.oid, option.sqlId)) {
            _errors = tr("Failed to set the database ID '%1' for option '%2' in reverse engine '%3' of curve ID '%4'").arg(QString::number(option.sqlId), option.oid, _re->descriptor().id, QString::number(_id));
            return false;
        }
    }

    return true;
}

bool CurveModel::deleteREsOptions()
{
    //Delete all the options associated with this curve
    if (!DATABASE->tryRequest(DB->deleteREsOptions(_id), tr("Error while trying to delete the curve's reverse engines options"), _errors))
        return false;

    //Reset the current reverse engine options' database ID
    foreach (Option option, _re->getOptions()) {
        if (!_re->setOptionDbId(option.oid, invalid_ident)) {
            _errors = tr("Failed to reset the database ID of option '%1' in reverse engine '%2' of curve ID '%3'").arg(option.oid, _re->descriptor().id, QString::number(_id));
            return false;
        }
    }

    return true;
}

//--------------------------------- UTILITIES ---------------------------------

//Previous on the upperBound on the parameters
ParameterSet* CurveModel::parametersAtTime(const Duration &time, ParameterType type) const
{
    QMap<Duration,ParameterSet*>::const_iterator i = _parameters[type].upperBound(time);
    //If it's not the first item, take the preceding item
    if (i != _parameters[type].constBegin())
        i--;
    return i.value();
}

//Previous on the upperBound on the dosages, or the given dosage
SharedDosage CurveModel::dosageAtTime(const Duration &time, const SharedDosage defaultDosage) const
{
    //If the default dosage's dose is not -1.0, use it
    if (defaultDosage->dose().value() != -1.0)
        return defaultDosage;

    // ToDo /////////////////////////////////////////////////////////////////////////////
    // Below, if the time is negative, we return a fake dosage with absurd values, apart
    // from the unit which correspond to the one (doses default) fixed in the rest of the
    // classes. It is done to manage the case where the time is before the firsttake. The
    // problem is that it could have occured in the point() function before it was fixed.
    // It is therefore possible that there is other cases with a negative duration that is
    // not before the firsttake: it is a bug that need to be fixed during the refactoring.
    //////////////////////////////////////////////////////////////////////////////////////

    //If the time is negative, return a dosage of 0 and a big interval (pre-curve points)
    if (time.mSecs() < 0) {
        SharedDosage d = SharedDosage(new Dosage());
        d->setApplied(_firstTake.addSecs(time.seconds()));
        d->setDose(Amount(0, doseUnit()));
        d->setInterval(Duration(9999999));
        d->setTinf(Duration(0,0,1));
        return d;
    }

    //Fetch it
    QMap<Duration,SharedDosage>::const_iterator i = _dosages.upperBound(time);
    //If it's not the first item, take the preceding item
    if (i != _dosages.constBegin())
        i--;
    return i.value();
}

//Updates the dosages map
void CurveModel::updateDosages(const QDateTime &newDate)
{
    QMap<Duration, SharedDosage> dosages;

    //Compute the difference between the two dates
    Duration difference(0, 0, 0, newDate.msecsTo(_firstTake));

    //Update each duration and insert in a new map
    QMap<Duration, SharedDosage>::const_iterator i;

    for (i = _dosages.constBegin(); i != _dosages.constEnd(); i++) {

        //Update the duration and verify it
        Duration updated = i.key() + difference;

        if (updated.mSecs() < 0)
            ERROR(DATAERROR, tr("A dosage with an applied date prior to the first take was detected"));

        //Insert it in the new dosages map
        dosages.insert(updated, i.value());
    }

    //Replace the old dosages map with the new one
    _dosages = dosages;
}

//Previous on the upperBound on the residuals
Duration CurveModel::lastResidual(const Duration &time, ParameterType type) const
{
    //If the residuals is empty for this type, return an empty duration
    if (_residuals[type].isEmpty())
        return Duration();

    //Fetch it
    QMap<Duration,Residuals>::const_iterator i = _residuals[type].upperBound(time);

    //If 'i' is the first, return 0, else, use the preceding value
    if (i == _residuals[type].constBegin())
        return Duration(0);
    else
        i--;
    return i.key();
}

//Previous on the upperBound on the covariates
Duration CurveModel::lastCovariates(const Duration &time) const
{
    QMap<Duration,PatientVariateSet*>::const_iterator i = _covariates.upperBound(time);
    //If it's not the first item, take the preceding item
    if (i != _covariates.constBegin())
        i--;
    return i.key();
}

// ToDo //////////////////////////////////////////////////////////////////////////////////////////////
// One of the issues with the cache is that we do not remove the residuals and parameters at the given
// time. It can have an issue when we change the date of a covariate or measure and set it later: the
// old associated parameter will not be removed, and will corrupt the prediction. This is why we have,
// in measuresChange() and covariatesChange(), to always pass a reset time before the actual change,
// leading to unecessary calcuation in some cases.
// We might want to try to remove the parameter and residual at the given time and optimize the loops
// in measuresChange() and covariatesChange() (see the first one for a heads up), but we need to check
// if this would have an impact on the residuals and dosages change.
//////////////////////////////////////////////////////////////////////////////////////////////////////

//Use the other resetCache, resetting the APRIORI and APOSTERIORI stuff
void CurveModel::resetCache(const Duration &time)
{
    resetCache(APRIORI, time);
    resetCache(APOSTERIORI, time);
}

//Reset the validity and erase the data
void CurveModel::resetCache(ParameterType type, const Duration &time)
{
    //Reset to the lower value
    _parametersValidUntil[type] = qMin(time.isNegative() ? Duration() : time, _parametersValidUntil[type]);
    _cacheValidUntil[type] = qMin(time.isNegative() ? Duration() : time, _cacheValidUntil[type]);

    //Little optimisation : if no time is given (=0), clear the objects
    if (time.isEmpty() || time.isNegative()) {
        _residuals[type].clear();
        _parameters[type].clear();
        _steadyStates[type].clear();
        _initialResidual[type].clear();
        return;
    }

    //Remove the data from the upperBound to the last one
    QMap<Duration,Residuals>::iterator ri = _residuals[type].upperBound(time);
    while (ri != _residuals[type].constEnd())
        _residuals[type].erase(ri++); //Okay, that's an ugly way to gain one line

    //The same for the parameters
    QMap<Duration,ParameterSet*>::iterator pi = _parameters[type].upperBound(time);
    while (pi != _parameters[type].constEnd())
        _parameters[type].erase(pi++); //Okay, that's an ugly way to gain one line

    //Remove each steady state until it's under the given duration (they should be in order)
    while (!_steadyStates[type].isEmpty() && _steadyStates[type].last() >= time)
        _steadyStates[type].removeLast();
}

//Check each given element, and throw the corresponding error
bool CurveModel::checkObjects(ObjectType toCheck)
{
    QString str;

    //Model, to be checked for any of the others...
    if (!_model)
        str += tr("The model\n");

    if (toCheck == REVERSE && !_re)
        str += tr("The reverse engine\n");

    if (toCheck == PO && !_po)
        str += tr("The post engine\n");

    if (toCheck == PE && !_pe)
        str += tr("The percentile engine\n");

    //If something went wrong, build the message
    bool ok = str.isEmpty();
    if (!ok)
        emit error(tr("These required objects needs to be set but are not for this operation :\n%2").arg(str));

    return ok;
}

//Checks if the PNA has reached the maximum
bool CurveModel::isPnaValid(const Duration &dayToTest)
{
    return (dayToTest + _daysFromBirthToFirstTake).toHours() / 24 <= PNA_MAX;
}

//Invalidate the post engine's current eta
void CurveModel::invalidatePostEngine()
{
    _poInvalidated = true;
    resetCache(APOSTERIORI);
}

//Calculates the total number of points to have uniform delta for the points exportation
uint CurveModel::uniformPoints(uint points, const Duration &start, const Duration &stop) const
{
    //Move the current to the initial cycle
    Duration current = start;

    //Increment the total for initial cycle
    uint total = points + 1;

    //Add one point per cycle remaining, minus one
    while ((current = nextCycle(current.addMSecs(1))) < stop)
        total++;

    //Return total point for a uniform delta
    return total;
}

//Save the value
void CurveModel::setError(QString str)
{
    _errors = str;
}

void CurveModel::setModified()
{
    _modified = true;
}

void CurveModel::setSaved()
{
    _modified = false;
}

} // namespace core
} // namespace Tucuxi

