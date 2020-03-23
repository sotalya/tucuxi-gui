#ifndef FOCE_H
#define FOCE_H

#include <QCoreApplication>
#include <QList>
#include <QMap>
#include <QString>

#include "ezutils.h"
#include "medical.h"
#include "core.h"
#include "dataset.h"
#include "postengine.h"
#include "postenginefactory.h"
#include "drugresponseanalysis.h"
#include "drugtreatment.h"
#include "duration.h"
#include "../dal/parameters.h"
#include "units.h"
#include "version.h"
#include "FOCETaylorModel.h"

#include <QTextStream>

#define EPSILON            "1epsilon"
#define DELTA_EPSILON      "1epsilon_d"
#define PRECISION          "1precision"
#define IGNOREPARAMETERS   "2ignorepar"
#define IGNORERESIDUAL     "3ignoreres"
#define CALCULIMP          "4calcimp"
#define IGNORECORRELATIONS "5ignorecorr"
#define EXPORTSTATS        "6exportstats"
#define STATSFILE_OPTION   "7exportdir"
#define RESIDERRORIMP      "8residerrorimp"


class FOCEFactory : public PostEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "FOCE")
    Q_INTERFACES(PostEngineFactory)
public:
    void init(Core *core);

    PostEngine* newPostEngine ();

    Descriptor descriptor ();

    int priority(const QString &id) const;

private:
    //Our ID is : ch.heig-vd.ezechiel.FOCE
    Descriptor _d;

    //Map of <Engine ID, Engine priority>
    QMap<QString, int> _priorities;
};

class FOCE : public PostEngine
{
    Q_DECLARE_TR_FUNCTIONS(FOCE)
public:

    FOCE(const Descriptor &d);
    ~FOCE();
    PostEngine* clone ();
    Descriptor descriptor ();
    int priority () const;
    Version version ();
    QString errorString ();
    void setPrediction (Prediction* model);

    virtual bool calculateEta (QList<ParameterSet*> const &aprioriParams,
                               SharedMeasureSet const &measures);

    virtual ParameterSet* aPosterioriParameters (const ParameterSet*& aprioriParams);

    virtual void aPosterioriParameters (const QList<ParameterSet*>& aprioriParams,
                                        QList<ParameterSet*>& aposterioriParams);

    ParameterSet* aposteriori (ParameterSet* const &input,
                              QList<SharedMeasure> const &measures,
                              Duration const &intervalStart,
                              Duration const &intervalStop);

    double calculateGlobalLogLikelihood(QMap<QString, double>&);
    void getEtaBounds();

private:

    static const int _priority;    
    void updateEta(double deltas[], bool positive);
    QMap<ident, FOCETaylorModel> m_mvmds;
    QMap<QString, double> m_eta;
    Prediction* m_model;
    Descriptor m_d;
    QString m_errors;
    ParameterSet* m_params_ind;
    QMap<QString, double> m_eta_min;
    QMap<QString, double> m_eta_max;
    void initializeMVMDs(const QList<SharedParameterSet>&, const SharedMeasureSet);
    double minimizeGlobalLogLikelihood();
};



#endif // FOCE_H
