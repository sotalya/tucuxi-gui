#ifndef TESTCURVES_H
#define TESTCURVES_H

#include <QMap>
#include <QMultiMap>
#include <QString>
#include <QList>
#include <QVariant>
#include <QStringList>

#include "medical.h"
#include "drugmodelengine.h"
#include "drugmodelfactory.h"
#include "core.h"
#include "ezutils.h"
#include "distribution.h"

/*! \defgroup analytic The Analytic model
  \ingroup models
   This plugin proposes an analytic approach for the drug concentration prediction.
   This implementation follows the course of Dr. Thierry Buclin about the pharmacocinetics and pharmacodynamics models.
   Some model subtypes and drug take method are defined, each of these corresponding to a specific prediction formula.
*/

//! \brief The Analytic model factory
/*! \ingroup analytic factories
    This factory uses three sub-classes to provides the 3 ways of drug intake.
  */
class TestCurves : public DrugModelFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "testcurves")
    Q_INTERFACES(DrugModelFactory)
public:

    TestCurves();

    void init(Core *core);

    QString pluginId();

    DrugModel* newModel (QString id);

    QList<ModelDescriptor> models ();

    int priority(const QString &id) const;

private:
    //Plugin ID
    QString _pluginId;

    //List of sub-model description
    QList<ModelDescriptor> _models;

    //Map of <Model ID, Model priority>
    QMap<QString, int> _priorities;
};



/** \ingroup analytic
 * \brief Mono-compartimental bolus predictor for the Analytic model
 */
class TestCurveConstant : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(TestCurveConstant)
    MODELUTILITIES(TestCurveConstant)
public:

    TestCurveConstant();

    ModelDescriptor descriptor ();        

    Version version ();

    Duration halflife ( const Duration &start,
                                const ParameterSet* &parameters,
                                const SharedDosage &dosage,
                                const Residuals &residuals);

    DataSet predict( const Duration &start,
                             const Duration &until,
                             const ParameterSet* &parameters,
                             uint points,
                             const SharedDosage &dosage,
                             Residuals &residuals,
                             bool uniform);
private:

   //Used units
   Unit _ug,_ugl;
};




/** \ingroup analytic
 * \brief Mono-compartimental bolus predictor for the Analytic model
 */
class TestCurveProportional : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(TestCurveProportional)    
    MODELUTILITIES(TestCurveProportional)
public:

    TestCurveProportional();

    ModelDescriptor descriptor();

    Version version();

    Duration halflife(const Duration &start,
                              const ParameterSet* &parameters,
                              const SharedDosage &dosage,
                              const Residuals &residuals);

    DataSet predict(const Duration &start,
                            const Duration &until,
                            const ParameterSet* &parameters,
                            uint points,
                            const SharedDosage &dosage,
                            Residuals &residuals,
                            bool uniform);

private:

   //Used units
   Unit _ug,_ugl;
};





/** \ingroup analytic
 * \brief Mono-compartimental bolus predictor for the Analytic model
 */
class TestCurveConstantSum : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(TestCurveConstantSum)
    MODELUTILITIES(TestCurveConstantSum)
public:

    TestCurveConstantSum();

    ModelDescriptor descriptor();

    Version version();

    Duration halflife(const Duration &start,
                              const ParameterSet* &parameters,
                              const SharedDosage &dosage,
                              const Residuals &residuals);

    DataSet predict(const Duration &start,
                            const Duration &until,
                            const ParameterSet* &parameters,
                            uint points,
                            const SharedDosage &dosage,
                            Residuals &residuals,
                            bool uniform);

private:

   //Used units
   Unit _ug,_ugl;
};
#endif // TESTCURVES_H
