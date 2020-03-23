#ifndef ANALYTIC_H
#define ANALYTIC_H

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
class Analytic : public DrugModelFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "analytic")
    Q_INTERFACES(DrugModelFactory)
public:

    Analytic();

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

#endif // ANALYTIC_H
