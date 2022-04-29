//@@license@@

#include "FOCETaylorModel.h"
#include "distribution.h"
#include "prediction.h"

namespace ublas = boost::numeric::ublas;
typedef ublas::row_major ORI;
typedef ublas::matrix<double,ORI> boostmatrix;
typedef ublas::vector<double> boostvector;
typedef ublas::permutation_matrix<std::size_t> pmatrix;


FOCETaylorModel::FOCETaylorModel(SharedPrediction _model,
                                 const SharedParameterSet params_ind,
                                 const SharedMeasure measure,
                                 const QMap<QString, double>& eta_min,
                                 const QMap<QString, double>& eta_max)
    : m_pred(0), m_measure(0), m_measuretime(0), m_invomega(0), m_logdetomega(0), m_sigma(0), m_omega(0), m_invsigma(0), m_eta(0)
{
    m_pred = _model;
    m_measure = measure;
    m_measuretime = Duration(0,0,_model->treatment()->dosages()->firsttake().secsTo(measure->moment()));
    m_params_ind = params_ind;
    m_local_params_ind = SharedParameterSet(new ParameterSet(params_ind));
    this->m_eta_min = eta_min;
    this->m_eta_max = eta_max;
}

FOCETaylorModel::FOCETaylorModel(SharedPrediction _model,
                                 const QList<SharedParameterSet> &params_ind,
                                 const SharedMeasure measure,
                                 const Duration& measuretime)
    : m_pred(0), m_measure(0), m_measuretime(0), m_invomega(0), m_logdetomega(0), m_sigma(0), m_omega(0), m_invsigma(0), m_eta(0)
{
    m_pred = _model;
    m_measure = measure;
    m_measuretime = measuretime;
    for (QList<SharedParameterSet>::const_iterator it = params_ind.constBegin(); it != params_ind.constEnd(); ++it) {
        if (measuretime < (*it)->duration()) {
            m_params_ind = *it;
            m_local_params_ind = SharedParameterSet(new ParameterSet(*m_params_ind));
            return;
        }
    }
}

FOCETaylorModel::~FOCETaylorModel() {
    if (m_omega != 0) {
        delete(m_omega);
    }
    if (m_invomega != 0) {
        delete(m_invomega);
    }
    if (m_sigma != 0) {
        delete(m_sigma);
    }
    if (m_invsigma != 0) {
        delete(m_invsigma);
    }
    if (m_eta != 0) {
        delete(m_eta);
    }
}

double FOCETaylorModel::calculateLogLikelihood2(const QMap<QString, double>& _eta)
{
    reinitialize(_eta);
    computeMeanAndVariance(_eta);
    m_loglikelihood = log(m_variance) + pow((m_measure->amount().value() - m_mean), 2) / m_variance;
    return m_loglikelihood;
}

double FOCETaylorModel::calculateLogLikelihood(const QMap<QString, double>& _eta)
{
    reinitialize(_eta);    
//    boostmatrix invomega(omega.size1(), omega.size2());
//    QList<double> omu = {omega(0,0),omega(0,1),omega(1,0),omega(1,1)};
//    InvertMatrix(omega, invomega);
//    QList<double> om7u = {(*m_invomega)(0,0),(*m_invomega)(0,1),(*m_invomega)(1,0),(*m_invomega)(1,1)};
//!    boostmatrix hs = hessian(_eta)/2;
//    boostvector jac = jacobian(_eta);
//    QList<double> omff7u = {jac(0),jac(1)};
//    boostmatrix tmp = invomega + hs;
//    boostmatrix tmp = *m_invomega + ublas::outer_prod(jac, ublas::trans(jac)) / 4;
//!    boostmatrix tmp = *m_invomega + hs;
//!    double dettmp = det(tmp);
//    double p = phi(jac, *m_omega);
//    double om = log(det(*m_omega));
//    double et = etaOeta(_eta, *m_invomega);
//!    double et = ublas::inner_prod(ublas::prod(*m_eta, *m_invomega), *m_eta);
//    double tm = log(det(tmp));
//!    double p = phi(m_local_params_ind);
//!    m_loglikelihood = p + m_logdetomega + et + log(det(tmp));
//    m_loglikelihood = phi() + etaOeta(_eta, *m_invomega) + log(det(tmp));

//    return m_loglikelihood;
    double m_loglikelihood = phi(m_local_params_ind);
    return m_loglikelihood;
}

double FOCETaylorModel::phi(SharedParameterSet params) {

    m_pred->analysis()->setParameters(m_pred, params, TMP);
    double mean = m_pred->analysis()->point(m_pred, m_measuretime, TMP).value();
    double meanmeasurediff = m_measure->amount().value() - mean;

    if (m_sigma == 0) {
        m_sigma = new boostmatrix(1,1);
    }

    populateSigma(m_params_ind, *m_sigma, mean);

    if (m_invsigma == 0) {
        m_invsigma = new boostmatrix(m_sigma->size1(), m_sigma->size2());
    }

    InvertMatrix(*m_sigma, *m_invsigma);

    boostvector mmd(m_sigma->size1());
    for (int i = 0; i < mmd.size(); ++i) {
        mmd(i) = meanmeasurediff;
    }

    boostvector top = ublas::prod(ublas::trans(mmd),*m_invsigma);
    double exponent = ublas::inner_prod(top, mmd);
    double ret = m_sigma->size1();
    ret *= log(2 * 3.1428);
    ret += log(det(*m_sigma));
    ret += exponent;
    return ret/2;
}

double FOCETaylorModel::dphidx(double start, double increment, QString pname, SharedParameterSet params) {

    double ret = 0;
    SharedParameterSet tmp_params = SharedParameterSet(new ParameterSet(m_local_params_ind));
    double _deriv = derivative(start, increment, [&](double y) -> double
    {
        switch((*m_local_params_ind)[pname]->distribution()) {

            case distribution::LogNormal:
                (*tmp_params)[pname]->setValue((*params)[pname]->value() * exp(y));
                ret = phi(params);
                (*tmp_params)[pname]->setValue((*params)[pname]->value());
                return ret;
                break;
            case distribution::Normal:
                (*tmp_params)[pname]->setValue((*params)[pname]->value() + y);
                ret = phi(params);
                (*tmp_params)[pname]->setValue((*params)[pname]->value());
                return ret;
                break;
        }
     });
    return _deriv * increment;
}

double FOCETaylorModel::d2phidx2(QString pnamex, QString pnamey) {

    double ret = 0;
    SharedParameterSet tmp_params = SharedParameterSet(new ParameterSet(m_local_params_ind));
    double _deriv2 = derivative(0.0, 0.001, [&](double y) -> double
    {
        switch((*m_local_params_ind)[pnamex]->distribution()) {

            case distribution::LogNormal:
                (*tmp_params)[pnamex]->setValue((*m_local_params_ind)[pnamex]->value() * exp(y));
                ret = dphidx(y, 0.01, pnamey, tmp_params);
                (*tmp_params)[pnamex]->setValue((*m_local_params_ind)[pnamex]->value());
                return ret;
                break;
            case distribution::Normal:
                (*tmp_params)[pnamex]->setValue((*m_local_params_ind)[pnamex]->value() + y);
                ret = dphidx(y, 0.01, pnamey, tmp_params);
                (*tmp_params)[pnamex]->setValue((*m_local_params_ind)[pnamex]->value());
                return ret;
                break;
        }

    });
    return _deriv2;
}

double FOCETaylorModel::logprior(const QMap<QString, double>& _eta) {

    QMap<QString, double> vareta;

    foreach (QString pid, m_local_params_ind->pids()) {
        if ((*m_local_params_ind)[pid]->isVariable()) {
            vareta.insert(pid, _eta[pid]);
        }
    }

    boostvector veceta(vareta.size());
    int etacount = 0;
    for (QMap<QString, double>::const_iterator it = vareta.constBegin(); it != vareta.constEnd(); ++it) {
        veceta(etacount) = it.value();
        etacount++;
    }

    double exponent = etaOeta(_eta, *m_invomega)/2;
    double log2pi = veceta.size() * log(2 * 3.1428);
    double ret = 0.5 * (exponent + log2pi + log(det(*m_omega)));

    return ret;
}

void FOCETaylorModel::populateSigma(const SharedParameterSet params, boostmatrix& sigma, double mean) {

    if (params->globalAdditive() != 0 && params->globalProportional() != 0) {
        sigma(0,0) = mean * mean * pow(params->globalProportional(), 2) + pow(params->globalAdditive(), 2);

    }
    else {
        if (params->globalProportional() == 0) {
            sigma(0,0) = pow(params->globalAdditive(), 2);
        } else {
            sigma(0,0) = pow(params->globalProportional() * mean, 2);
        }
    }

//    int sizecount = 1;
//    if (m_params_ind->globalAdditive() != 0) {
//        sigma.resize(sizecount,sizecount);
//        sigma(sizecount -1,sizecount -1) = pow(m_params_ind->globalAdditive(), 2);
//        sizecount++;
//    }

//    if (m_params_ind->globalProportional() != 0) {
//        sigma.resize(sizecount,sizecount);
//        sigma(sizecount -1,sizecount -1) = pow(m_params_ind->globalProportional() * m_mean, 2);
//    }
}

void FOCETaylorModel::populateOmega(SharedParameterSet params, boostmatrix& omega) {

    QStringList pl = params->pids();
    QStringList plist = QStringList(pl);
    for (QStringList::iterator it = pl.begin(); it != pl.end(); ++it){
        if (!(*m_local_params_ind)[*it]->isVariable()) {
            plist.removeOne(*it);
        }
    }

    for (int etacount = 0; etacount < plist.size(); ++etacount) {

        for (int inetacount = 0; inetacount < plist.size(); ++inetacount) {

            if (etacount == inetacount) {
                double e = (*params)[plist[etacount]]->deviation() * (*params)[plist[inetacount]]->deviation();
                omega(etacount, inetacount) = e;
            }
            else if (etacount != inetacount && params->correlation(plist[etacount],plist[inetacount]) != 0) {
                double d = params->correlation(plist[etacount],plist[inetacount]) * (*params)[plist[etacount]]->deviation() * (*params)[plist[inetacount]]->deviation();
                omega(etacount, inetacount) = d;
            }
            else {
                omega(etacount, inetacount) = 0;
            }
        }
    }    
}



double FOCETaylorModel::etaOeta(const QMap<QString, double>& _eta, boostmatrix& invomega) {

    QMap<QString, double> vareta;

    foreach (QString pid, m_local_params_ind->pids()) {
        if ((*m_local_params_ind)[pid]->isVariable()) {
            vareta.insert(pid, _eta[pid]);
        }
    }

    double ret = 0;
    boostvector veceta(vareta.size());
    int etacount = 0;
    for (QMap<QString, double>::const_iterator it = vareta.constBegin(); it != vareta.constEnd(); ++it) {
        veceta(etacount) = it.value();
        etacount++;
    }

    ret = boost::numeric::ublas::inner_prod(boost::numeric::ublas::prod(veceta, invomega), veceta);
    return ret;
}

boostvector FOCETaylorModel::jacobian(const QMap<QString, double> & _eta) {

    QMap<QString, double> vareta;
    SharedParameterSet tmp_params = SharedParameterSet(new ParameterSet(m_local_params_ind));

    foreach (QString pid, m_local_params_ind->pids()) {
        if ((*m_local_params_ind)[pid]->isVariable()) {
            vareta.insert(pid, _eta[pid]);
        }
    }

    boostvector jac(vareta.size());
    int etacount = 0;
    for (QMap<QString, double>::const_iterator it = vareta.constBegin(); it != vareta.constEnd(); ++it) {
        jac(etacount) = dphidx(0.0, 0.001, it.key(), tmp_params);
        etacount++;
    }
    return jac;
}

boostmatrix FOCETaylorModel::hessian(const QMap<QString, double>& _eta) {

    QMap<QString, double> vareta;

    foreach (QString pid, m_local_params_ind->pids()) {
        if ((*m_local_params_ind)[pid]->isVariable()) {
            vareta.insert(pid, _eta[pid]);
        }
    }
    boostmatrix hess = boost::numeric::ublas::zero_matrix<double>(vareta.size());

    for (int rowcount = 0; rowcount < vareta.size(); ++rowcount) {
        for (int colcount = 0; colcount < vareta.size(); ++colcount) {
            double h = d2phidx2(vareta.keys().at(rowcount), vareta.keys().at(colcount));
            hess(rowcount, colcount) = h;
        }
    }
    return hess;
}



double FOCETaylorModel::dfdx(double start, double increment, QString pname, SharedParameterSet params) {

    double ret = 0;
    double _deriv = derivative(start, increment, [&](double y) -> double
    {
        switch((*m_local_params_ind)[pname]->distribution()) {
            case distribution::DistributionType::LogNormal:
                (*params)[pname]->setValue((*m_local_params_ind)[pname]->value() * exp(y));
                m_pred->analysis()->setParameters(m_pred, params, TMP);
                ret = m_pred->analysis()->point(m_pred, m_measuretime, TMP).value();
                (*params)[pname]->setValue((*m_local_params_ind)[pname]->value());
                return ret;
                break;
            case distribution::DistributionType::Normal:
                (*params)[pname]->setValue((*m_local_params_ind)[pname]->value() + y);
                m_pred->analysis()->setParameters(m_pred, params, TMP);
                ret = m_pred->analysis()->point(m_pred, m_measuretime, TMP).value();
                (*params)[pname]->setValue((*m_local_params_ind)[pname]->value());
                return ret;
                break;
        }
     });
    return _deriv * increment;
}

double FOCETaylorModel::d2fdxdy(QString pnamex, QString pnamey) {

    double ret = 0;
    SharedParameterSet tmp_params = SharedParameterSet(new ParameterSet(m_local_params_ind));
    double _deriv2 = derivative(0.0, 0.001, [&](double x) -> double
    {
        switch((*m_local_params_ind)[pnamex]->distribution()) {

            case distribution::LogNormal:
                (*tmp_params)[pnamex]->setValue((*m_local_params_ind)[pnamex]->value() * exp(x));
                m_pred->analysis()->setParameters(m_pred, tmp_params, TMP);
                ret = dfdx(x, 0.01, pnamey, tmp_params);
                (*tmp_params)[pnamex]->setValue((*m_local_params_ind)[pnamex]->value());
                return ret;
                break;
            case distribution::DistributionType::Normal:
                (*tmp_params)[pnamex]->setValue((*m_local_params_ind)[pnamex]->value() + x);
                m_pred->analysis()->setParameters(m_pred, tmp_params, TMP);
                ret = dfdx(x, 0.01, pnamey, tmp_params);
                (*tmp_params)[pnamex]->setValue((*m_local_params_ind)[pnamex]->value());
                return ret;
                break;
        }
    });
    return _deriv2;
}

int FOCETaylorModel::getNumVarEtas(const QMap<QString, double>& _eta) {

    m_eta = new boostvector();

    int etacount = 0;
    for (QMap<QString, double>::const_iterator it = _eta.constBegin(); it != _eta.constEnd(); ++it) {
        if ((*m_local_params_ind)[it.key()]->isVariable()) {
            m_eta->resize(etacount + 1);
            (*m_eta)(etacount) = it.value();
            etacount++;
        }
    }
    return m_eta->size();
}


void FOCETaylorModel::reinitialize(const QMap<QString, double>& _eta)
{    
    double numetas = getNumVarEtas(_eta);

    if (m_invomega == 0) {
        m_omega = new boostmatrix(numetas, numetas);
        populateOmega(m_local_params_ind, *m_omega);
        m_invomega = new boostmatrix(m_omega->size1(), m_omega->size2());
        InvertMatrix(*m_omega, *m_invomega);
        m_logdetomega = log(det(*m_omega));
    }

    m_local_params_ind = SharedParameterSet(new ParameterSet(*m_params_ind));
    int count = 0;
    foreach (QString pid, m_local_params_ind->pids()) {
        if ((*m_local_params_ind)[pid]->isVariable()) {
            switch((*m_local_params_ind)[pid]->distribution()) {

                case distribution::LogNormal:
                    (*m_local_params_ind)[pid]->setValue((*m_local_params_ind)[pid]->value() * exp(_eta[pid]));
                    (*m_eta)(count) = _eta[pid];
                    count++;
                    break;

                case distribution::Normal:
                    (*m_local_params_ind)[pid]->setValue((*m_local_params_ind)[pid]->value() + _eta[pid]);
                    (*m_eta)(count) = _eta[pid];
                    count++;
                    break;
            }
        }
    }


    m_pred->analysis()->setParameters(m_pred, m_local_params_ind, TMP);
    ParameterSet ps = m_pred->treatment()->parameters(m_measuretime, TMP);
    m_mean = m_pred->analysis()->point(m_pred, m_measuretime, TMP).value();

//    if (m_sigma == 0) {
//        m_sigma = new boostmatrix();
//        populateSigma(m_local_params_ind, *m_sigma);
//        m_invsigma = new boostmatrix(m_sigma->size1(), m_sigma->size2());
//        InvertMatrix(*m_sigma, *m_invsigma);
//    }
//    m_model->setParameters(m_local_params_ind, TMP);
//    ParameterSet ps = m_model->parameters(m_measuretime, TMP);
//    m_mean = m_model->point(m_measuretime, TMP).value;
}

void FOCETaylorModel::computeMeanAndVariance(const QMap<QString, double>& _eta)
{
    m_variance = 0;
    if (m_params_ind->globalAdditive() != 0) {

        m_variance = pow(m_params_ind->globalAdditive(), 2);
    }

    if (m_params_ind->globalProportional() != 0) {

        m_variance += pow(m_params_ind->globalProportional() * m_mean, 2);
    }

    QMap<QString, double> _derivs;
    for (QMap<QString, double>::const_iterator it = _eta.constBegin();
         it != _eta.constEnd(); ++it) {

        if ((*m_params_ind)[it.key()]->deviation() != 0) {

            double ret = 0;
            SharedParameterSet tmp_params = SharedParameterSet(new ParameterSet(m_local_params_ind));
            double _deriv = derivative(0.0, 0.01, [&](double x) -> double
            {
                switch((*m_local_params_ind)[it.key()]->distribution()) {
                    case distribution::DistributionType::LogNormal:
                        (*tmp_params)[it.key()]->setValue((*m_local_params_ind)[it.key()]->value() * exp(x));
                        m_pred->analysis()->setParameters(m_pred, tmp_params, TMP);
                        ret = m_pred->analysis()->point(m_pred, m_measuretime, TMP).value();
                        (*tmp_params)[it.key()]->setValue((*m_local_params_ind)[it.key()]->value());
                        return ret;
                        break;

                    case distribution::Normal:
                        (*tmp_params)[it.key()]->setValue((*m_local_params_ind)[it.key()]->value() + x);
                        m_pred->analysis()->setParameters(m_pred, tmp_params, TMP);
                        ret = m_pred->analysis()->point(m_pred, m_measuretime, TMP).value();
                        (*tmp_params)[it.key()]->setValue((*m_local_params_ind)[it.key()]->value());
                        return ret;
                        break;
                }


            } );
                _derivs.insert(it.key(),_deriv * 0.01);
        }
    }

    for (QMap<QString, double>::iterator it = _derivs.begin();
         it != _derivs.end(); ++it) {
        m_mean -= _eta[it.key()] * it.value();
        foreach (QString pid, m_local_params_ind->pids()) {
            if (m_local_params_ind->correlation(it.key(), pid) != 0) {
                m_variance += it.value() * _derivs[pid] * m_local_params_ind->correlation(it.key(), pid) * (*m_local_params_ind)[pid]->deviation() * (*m_local_params_ind)[it.key()]->deviation();
            }
        }
        m_variance += pow(it.value() * (*m_local_params_ind)[it.key()]->deviation() , 2);
    }
}
