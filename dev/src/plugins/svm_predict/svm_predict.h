//@@license@@

#ifndef SVMPREDICT_H
#define SVMPREDICT_H

#include <QMap>
#include <QMultiMap>
#include <QString>
#include <QList>
#include <QVariant>
#include <QStringList>

#include "medical.h"
#include "model.h"
#include "modelfactory.h"
#include "core.h"
#include "ezutils.h"
#include "distribution.h"

class SvmPredictFactory : public ModelFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "svm_predict")
    Q_INTERFACES(ModelFactory)

public:

    SvmPredictFactory();

    void init(Core *core);

    QString pluginId();

    Model* newModel (QString id);

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

class SvmPredict : public Model
{
    Q_DECLARE_TR_FUNCTIONS(SvmPredict)

public:

    SvmPredict();

    Model* clone ();

    ModelDescriptor descriptor ();

    QList<Descriptor> parametersDescription ();

    int priority() const;

    Version version ();

    QString errorString ();

    Duration halflife ( const Duration &start,
                        const ParameterSet &parameters,
                        const Dosage &dosage,
                        const Residuals &residuals);

    DataSet predict( const Duration &start,
                     const Duration &until,
                     const ParameterSet &parameters,
                     uint points,
                     const Dosage &dosage,
                     Residuals &residuals);

private:
   //The model priority level
   static const int _priority;

   //Parameter description list
   QList<Descriptor> _parameters;

   //Error message
   QString _errors;

   //Used units
   Unit _ug,_ugl;
};

#endif // SVMPREDICT_H
