//@@license@@

#ifndef FOCETAYLORMODEL_H
#define FOCETAYLORMODEL_H

#endif // FOCETAYLORMODEL_H

#include "drugresponseanalysis.h"
#include "drugtreatment.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/lu.hpp>

namespace ublas = boost::numeric::ublas;
typedef ublas::row_major ORI;
typedef ublas::matrix<double,ORI> boostmatrix;
typedef ublas::vector<double> boostvector;
typedef ublas::permutation_matrix<std::size_t> pmatrix;


// The following code was taken here:
// https://gist.github.com/lilac/2464434

/* Matrix inversion routine.
Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */
template<class T>
bool InvertMatrix (const ublas::matrix<T>& input, ublas::matrix<T>& inverse) {

    // create a working copy of the input
    ublas::matrix<T> A(input);
    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());

    // perform LU-factorization
    int res = ublas::lu_factorize(A,pm);
    if( res != 0 ) return false;

    // create intity matrix of "inverse"
    inverse.assign(ublas::identity_matrix<T>(A.size1()));

    // backsubstitute to get the inverse
    ublas::lu_substitute(A, pm, inverse);

    return true;
}

//from boost
template<typename value_type, typename function_type>
value_type derivative(const value_type x, const value_type dx, function_type func)
{
   // Compute d/dx[func(*first)] using a three-point
   // central difference rule of O(dx^6).

   const value_type dx1 = dx;
   const value_type dx2 = dx1 * 2;
   const value_type dx3 = dx1 * 3;

   const value_type m1 = (func(x + dx1) - func(x - dx1)) / 2;
   const value_type m2 = (func(x + dx2) - func(x - dx2)) / 4;
   const value_type m3 = (func(x + dx3) - func(x - dx3)) / 6;

   const value_type fifteen_m1 = 15 * m1;
   const value_type six_m2     =  6 * m2;
   const value_type ten_dx1    = 10 * dx1;

   return ((fifteen_m1 - six_m2) + m3) / ten_dx1;
}


class FOCETaylorModel
{
public:
    FOCETaylorModel(Prediction*, const QList<SharedParameterSet>&, const SharedMeasure, const Duration&);
    FOCETaylorModel(Prediction*, const SharedParameterSet, const SharedMeasure, const QMap<QString, double>&, const QMap<QString, double>&);
    ~FOCETaylorModel();
    void reinitialize(const QMap<QString, double>&);
    void computeMeanAndVariance(const QMap<QString, double>&);
    double getVariance() { return m_variance; }
    double getMean() { return m_mean; }
    void setMean(double _mean) { m_mean = _mean; }
    double calculateLogLikelihood(const QMap<QString, double>&);
    double calculateLogLikelihood2(const QMap<QString, double>&);
    double dfdx(double, double, QString, SharedParameterSet);
    double etaOeta(const QMap<QString, double>&, boostmatrix&);
    double phi(SharedParameterSet);
    double d2phidx2(QString, QString);
    double dphidx(double, double, QString, SharedParameterSet);
    void populateOmega(SharedParameterSet, boostmatrix&);
    void populateSigma(const SharedParameterSet, boostmatrix&, double);
    boostmatrix hessian(const QMap<QString, double>&);    
    boostvector jacobian(const QMap<QString, double>&);
    int getNumVarEtas(const QMap<QString, double>&);
    double d2fdxdy(QString, QString);
    double getLogLikelihood();
    double logprior(const QMap<QString, double>&);

    // The following code was taken here:
    // http://stackoverflow.com/questions/1419681/boost-library-how-to-get-determinant-from-lu-factorize

    template <class T>
    T det(const ublas::matrix<T>& mat)
    {
        ublas::matrix<T> A(mat);
        pmatrix pm(A.size1());
        int res = ublas::lu_factorize(A, pm);
        T determinant = 1;

        for(int i = 0; i < A.size1(); i++) {
            determinant *= (pm(i) == i ? 1 : -1) * A(i,i);
        }
        return determinant;
    }

private:
    Prediction* m_pred;
    double m_mean;
    double m_variance;
    double m_loglikelihood;
    SharedMeasure m_measure;
    Duration m_measuretime;
    SharedParameterSet m_params_ind;
    SharedParameterSet m_local_params_ind;
    QMap<QString, double> m_eta_min;
    QMap<QString, double> m_eta_max;
    boostmatrix* m_invomega;
    boostmatrix* m_omega;
    boostmatrix* m_sigma;
    boostmatrix* m_invsigma;
    boostvector* m_eta;
    double m_logdetomega;
};
