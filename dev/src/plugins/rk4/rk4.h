#ifndef RK4_H
#define RK4_H

#include <QMap>
#include <QMultiMap>
#include <QString>
#include <QList>
#include <QVariant>
#include <QStringList>

#include "medical.h"
#include "drugmodelfactory.h"
#include "core.h"
#include "ezutils.h"
#include "distribution.h"

/*! \defgroup rk4 The RK4 model
  \ingroup models
   Use some strange math-magic.
*/

//! \brief The RK4 model factory
/*! \ingroup rk4 factories
    This factory uses one sub-classes to provides the 3 ways of drug intake.
  */
class RK4 : public DrugModelFactory
{
   Q_OBJECT
    Q_PLUGIN_METADATA(IID "rk4")
   Q_INTERFACES(DrugModelFactory)
public:

    RK4();

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

#endif // RK4_H
