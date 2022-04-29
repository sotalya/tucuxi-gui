//@@license@@

#ifndef PREDICTIONACTION_H
#define PREDICTIONACTION_H

//#include "core/interfaces/genericdatabase.h"
#include "core/utils/errorenums.h"
#include "action.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/core.h"
#include "core/dal/prediction.h"
#include "core/interfaces/optionable.h"
#include "core/utils/dataset.h"

//class Optionable;

/** Prediction related action.
 * \ingroup cli actions
 *
 * Everything to create and manage curves, as well as calculating points.
 *
 * This action can :
 * \li create, list and remove curves
 * \li edit some curve data
 * \li manage the curve dosages
 * \li calculate some points and percentiles
 * \li show a graphical view of the curve
 * \li compare a set of points to a file and show their difference
 */
namespace ezechiel {
namespace cliutils {
using namespace core;

class PredictionAction : public Action
{
    Q_DECLARE_TR_FUNCTIONS(PredictionAction)

public:
    PredictionAction();

    static QString name () { return "prediction"; }

    QString help ();

    bool run (const QStringList &args);

private:
    //Error messages
    static const char *const _INVALID_COMMAND;

    //Shared messages
    static const char *const _ENGINE_OPTIONS_DISPLAY;
    static const char *const _MO;
    static const char *const _PE;
    static const char *const _PO;
    static const char *const _REE;

    //The commands keywords
    static const char *const _CMD_LIST;
    static const char *const _CMD_OPTION;
    static const char *const _CMD_MO_ABRV;
    static const char *const _CMD_PE_ABRV;
    static const char *const _CMD_PO_ABRV;
    static const char *const _CMD_RE_ABRV;
    static const char *const _CMD_CURVE_CONTINUOUS;
    static const char *const _CMD_CURVE_STEADYSTATE;
    static const char *const _CMD_PARAM_TYPICAL;
    static const char *const _CMD_PARAM_APRIORI;
    static const char *const _CMD_PARAM_APOSTERIORI;

    //Prediction
    bool create (const QStringList &args);
    bool list ();
    bool remove (ident id);

    //Data
    bool curveType(Prediction* curve, const QStringList &args);
    bool type (Prediction* curve, const QStringList &args);
    bool parameters (Prediction* curve, const QStringList &args);

    //Options
    bool option(Prediction* curve, QStringList &args);
    bool optionList(Prediction* curve, QStringList &args);
    bool optionManage(Prediction* curve, QStringList &args, Optionable *engine);
    void optionMultipleDisplay(const Optionable *engine);
    void optionSingleDisplay(const Option &option);

    //Dosages
    bool dosage (Prediction* curve, QStringList &args);
    bool dosageList (Prediction* curve);
    bool dosageAdd (Prediction* curve, QStringList &args);
    bool dosageSet (Prediction* curve, ident id, QStringList &args);
    bool dosageRemove (ident id);

    //Calculations
    bool point (Prediction* curve, const QStringList &args);
    bool points (Prediction* curve, QStringList &args);
    bool percentile (Prediction* curve, const QStringList &args);
    bool reverse (Prediction* curve, const QStringList &args);
//    bool chart (SharedPrediction curve, const QStringList &args);

    //Type <-> String conversion
    QString typeToString (ParameterType type) const;
    ParameterType stringToType (const QString &str, bool &ok);
    ParameterType parameterType(const QString &typeStr);

    //Compare a given dataset with a file
    bool compare (DataSet data, QString arg);

};

} //namespace cliutils
} //namespace ezechiel
#endif // PREDICTIONACTION_H
