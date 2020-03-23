#ifndef RK4GENERICMODEL_H
#define RK4GENERICMODEL_H

#include "drugmodel.h"

//#define GETPARAMETER(param,name) {name = _parameters.get(name).value;}

#define GETPARAMETER(param,name) \
    { \
        if (!parameters->contains(name)) \
        { \
            ERROR(NOPARAMETER,"Problem with a parameter name"); \
        } \
        else \
        { \
            param = parameters->get(name)->value(); \
        } \
    }


class RK4GenericModel : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(RK4GenericModel)    

public:
    RK4GenericModel(int nbComp);

//    virtual ModelDescriptor descriptor () = 0;

    virtual Version version ();

    virtual Duration halflife (const Duration &start,
                               const ParameterSet* &parameters,
                        	   const SharedDosage &dosage,
                        	   const Residuals &residuals) = 0;

    virtual DataSet predict(const Duration &start,
                     	    const Duration &until,
                            const ParameterSet* &parameters,
                     	    uint points,
                     	    const SharedDosage &dosage,
                            Residuals &residuals,
                            bool uniform);

protected:
//RETURN AMOUNTS[g]!! SO TAKE AN AMOUNT IN INPUT (yn [g])!
    virtual void yDeriv(double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result) = 0;
    virtual QVector<double> y0();
    virtual void getParameters(const ParameterSet* &parameters) = 0;

    void rungeKutta4(double tn, const QVector<double> &yn, const SharedDosage &dosage, double h, QVector<double> &result);
    void setNbComp(int nbComp);

    //Used units
    Unit _ug,_ugl;

    //Number of compartments
    int nbComp;

    //Dosage
    double D;

    QVector<double> rk4points[4];

};

#endif // RK4GENERICMODEL_H
