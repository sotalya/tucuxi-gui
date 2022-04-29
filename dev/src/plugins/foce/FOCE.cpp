//@@license@@

#include <FOCE.h>

#include "core.h"
#include "core_errors.h"
#include "coremessagehandler.h"
#include "prediction.h"

#include <QtPlugin>
#include <limits>
#include <cmath>

#include <boost/math/special_functions/erf.hpp>

//------------------------------------ FACTORY -------------------------------------

//Save it, and create our descriptor
void FOCEFactory::init(Core *core)
{
    //Set the core
    Core::setInstance(core);

    //Oh yes we are
    _d.id = FOCE_ID;
    _d.name = "FOCE";
    _d.description = "Approximate patient parameters using a maximum of likelihood approach.";

    //Store the priorities
    _priorities.insert(_d.id, FOCE(_d).priority());
}

//Create it with our description
PostEngine *FOCEFactory::newPostEngine()
{
    return new FOCE(_d);
}

//Created in init()
Descriptor FOCEFactory::descriptor()
{
    return _d;
}

//Returns an engine priority
int FOCEFactory::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}

//------------------------------------ POST ENGINE -------------------------------------


//The engine default priority level
const int FOCE::_priority = 10;

//Save the descriptor, setup the options
FOCE::FOCE (const Descriptor &d)
    : m_model(0), m_params_ind(0)
{
    m_d = d;

    // Gradient descent parameters
    addOption(IGNOREPARAMETERS, tr("Ignore parameters"), tr("Ignore the parameters variability during the process"),
              QVariant::Bool, QVariant(false));
    addOption(IGNORERESIDUAL, tr("Ignore residual"), tr("Ignore the residual error during the process"),
              QVariant::Bool, QVariant(false));
    addOption(IGNORECORRELATIONS, tr("Ignore correlations"), tr("Ignore the parameters correlations"),
              QVariant::Bool, QVariant(false));
    addOption(EPSILON, tr("Epsilon"), tr("Initial Epsilon for gradient steps"),
              QVariant::Double, QVariant(1.));
    addOption(DELTA_EPSILON, tr("Epsilon delta"), tr("Epsilon multiplicator (should be <= 1)"),
              QVariant::Double, QVariant(0.5));
    addOption(PRECISION, tr("Precision (xE-6)"), tr("Precision (stop searching for the minima when (X1 - X2 < precision*E-6))"),
              QVariant::Double, QVariant(10));
    addOption(CALCULIMP, tr("Calculus implementation"), tr("Defines the algo used. 0: Yann. 1: Lionel. 2:With correlation"),
              QVariant::Int, QVariant(2));

    addOption(RESIDERRORIMP, tr("Residual error implementation"), tr("Defines the algo used for calculating the residual error impact. 0: Laplace. 1: Additive"),
              QVariant::Int, QVariant(0));


    addOption(EXPORTSTATS, tr("Export computation details into files"), tr("If checked, then all the calculus points are exported to files that can be viewed in Octave/Matlab."),
              QVariant::Bool, QVariant(false));

    //Name of the directory where the files are saved
    addOption(STATSFILE_OPTION, tr("Directory name for export"), tr("All the files will be generated into this directory."),
              qMetaTypeId<DirName>(), QVariant::fromValue(DirName("")));

}

FOCE::~FOCE() {
    if (m_params_ind != 0) {
        delete(m_params_ind.data());
    }            
}

//Create and copy
PostEngine *FOCE::clone()
{
    FOCE *instance = new FOCE(m_d);
    instance->copyOptions(this);
    return instance;
}

//Given by the factory
Descriptor FOCE::descriptor ()
{
    return m_d;
}

int FOCE::priority() const
{
    return _priority;
}

//0.3 : Because
Version FOCE::version ()
{
    return Version("0.3");
}

//Mostly empty
QString FOCE::errorString()
{
    return m_errors;
}

//Simply save it
void FOCE::setPrediction(SharedPrediction model)
{
    m_model = model;
}

void FOCE::updateEta(double deltas[],bool positive)
{
    int i=0;
    for (QMap<QString, double>::iterator it = m_eta.begin();
         it != m_eta.end(); ++it) {
        if (positive)
            it.value() += deltas[i];
        else
            it.value() -= deltas[i];

        i++;
    }
}

ParameterSet* FOCE::aPosterioriParameters (const ParameterSet*& aprioriParams)
{
    ParameterSet* _returnpsetpluseta(aprioriParams);
    foreach (QString pid, aprioriParams->pids()) {
        switch((*aprioriParams)[pid]->distribution()) {
            case distribution::LogNormal:
                (*_returnpsetpluseta)[pid]->setValue((*_returnpsetpluseta)[pid]->value() * exp(m_eta[pid]));
                break;
            case distribution::Normal:
                (*_returnpsetpluseta)[pid]->setValue((*_returnpsetpluseta)[pid]->value() + m_eta[pid]);
                break;
            default:
                break;
        }

    }

    return _returnpsetpluseta;
}

ParameterSet* FOCE::aposteriori (ParameterSet* const &input,
                          QList<SharedMeasure> const &measures,
                          Duration const &intervalStart,
                          Duration const &intervalStop) {
    Q_UNUSED(input);
    Q_UNUSED(measures);
    Q_UNUSED(intervalStart);
    Q_UNUSED(intervalStop);

    return ParameterSet*(new ParameterSet(m_params_ind));
}

void FOCE::aPosterioriParameters (const QList<ParameterSet*>& aprioriParams,
                                  QList<ParameterSet*>& aposterioriParams)
{
    aposterioriParams.clear();

    QList<ParameterSet*>::const_iterator i = aprioriParams.constBegin();
    while (i != aprioriParams.constEnd()) {
        aposterioriParams.append(aPosterioriParameters(*i));
        ++i;
    }
}

bool FOCE::calculateEta (QList<ParameterSet*> const &apriori_params,
                         SharedMeasureSet const &measures) {
    if (m_params_ind != 0) {
        delete m_params_ind.data();
    }

    m_eta.clear();
    m_eta_min.clear();
    m_eta_max.clear();
    m_mvmds.clear();
    m_params_ind = ParameterSet*(new ParameterSet(apriori_params.first()));

    if (measures->isEmpty()) {
        return true;
    }

    foreach (QString pName, m_params_ind->pids()) {
        m_eta.insert(pName, 0);
    }

    getEtaBounds();

    initializeMVMDs(apriori_params, measures);

    double min = minimizeGlobalLogLikelihood();

    return true;
}

void FOCE::getEtaBounds() {

    //Set min/max of parameters
    foreach (QString pName, m_params_ind->pids()) {                  
        m_eta_min.insert(pName, sqrt(2) * (*m_params_ind)[pName]->proportional() *
                                          boost::math::erf_inv(2 * 0.001 - 1));
        m_eta_max.insert(pName, sqrt(2) * (*m_params_ind)[pName]->proportional() *
                                          boost::math::erf_inv(2 * 0.999 - 1));
    }
}

void FOCE::initializeMVMDs(const QList<SharedParameterSet>& apriori_params, const SharedMeasureSet measures) {
    for (MeasureSet::const_iterator mit = measures->constBegin(); mit != measures->constEnd(); ++mit) {

        for (QList<SharedParameterSet>::const_iterator pit = apriori_params.constBegin(); pit != apriori_params.constEnd(); ++pit) {
            if (Duration(m_model->firsttake().secsTo((*mit)->moment())) > (*pit)->duration()) {

                m_mvmds.insert((*mit)->id(), FOCETaylorModel(m_model, *pit, *mit, m_eta_min, m_eta_max));
            }
        }
    }
}

double FOCE::minimizeGlobalLogLikelihood()
{

    double eps = option(EPSILON).value.toDouble();
    eps = 10;
    double deps = option(DELTA_EPSILON).value.toDouble();
    deps = 0.1;
    double precision = option(PRECISION).value.toDouble();
    precision /= 1.E11;

    if (precision == 0)
    {
        precision = 1.E-300;
    }    

    double llhood = calculateGlobalLogLikelihood(m_eta);
    double llhood_old = llhood;
    double delta_llhood_old = 0;
    double delta_llhood = 0;

    int nb_iter = 0;
    int nb_loops = 0;

    llhood_old = llhood;
    delta_llhood = llhood;
    delta_llhood_old = 0;

    QMap<QString, double> tmp_eta;

    QMap<QString, double> inc_eta = m_eta;
    foreach (QString pName, m_params_ind->pids()) {
        inc_eta[pName] = 0;
    }

    do {
        nb_loops++;        
        nb_iter = 0;
        do {
            nb_iter++;
            foreach (QString pName, m_params_ind->pids()) {
                tmp_eta.insert(pName, m_eta[pName]);
            }


            foreach (QString pName, m_params_ind->pids()) {
                if ((*m_params_ind)[pName]->deviation() != 0) {
                    double _deriv = derivative(m_eta[pName], 0.001, [&](double x)
                    {
                        tmp_eta[pName] = x;
                        double _ret = calculateGlobalLogLikelihood(tmp_eta);
                        tmp_eta[pName] = m_eta[pName];
                        return _ret;
                    } );                    
                    inc_eta[pName] = - _deriv * 0.001 * eps;
                }
            }

            do {
                foreach (QString pName, m_params_ind->pids()) {
                    m_eta[pName] += inc_eta[pName];

                    if (m_eta[pName] < m_eta_min[pName]) {
                        m_eta[pName] = m_eta_min[pName];
                    }
                    if (m_eta[pName] > m_eta_max[pName]) {
                        m_eta[pName] = m_eta_max[pName];
                    }
                }

                llhood = calculateGlobalLogLikelihood(m_eta);
                delta_llhood_old = delta_llhood;
                delta_llhood = llhood_old - llhood;
                llhood_old = llhood;

                foreach (QString pName, m_params_ind->pids()) {
                    if ((delta_llhood) < 0.00001) {
                        m_eta[pName] -= inc_eta[pName];
                    }

                    if (m_eta[pName] < m_eta_min[pName]) {
                        m_eta[pName] = m_eta_min[pName];
                    }
                    if (m_eta[pName] > m_eta_max[pName]) {
                        m_eta[pName] = m_eta_max[pName];
                    }
                }

//                foreach (QString pName, m_params_ind->pids()) {
//                    inc_eta[pName] = 0;
//                }

            } while (delta_llhood > 0.00001 && llhood > 0);

        } while((nb_iter<100) && (fabs(delta_llhood_old) > precision) && llhood > 0);
        eps *= deps;
    } while((eps > precision)&&(nb_loops<1000));

    return llhood;
}

double FOCE::calculateGlobalLogLikelihood(QMap<QString, double>& _eta) {
    double gll = 0;
    QMap<ident, FOCETaylorModel>::iterator it = m_mvmds.begin();
    for (; it != m_mvmds.end(); ++it) {
        gll += it.value().calculateLogLikelihood(_eta);
    } --it;
    gll += it->logprior(_eta);
//    gll /= m_mvmds.size();
    return gll;
}

