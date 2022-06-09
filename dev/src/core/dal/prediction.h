//@@license@@

#ifndef PREDICTION_H
#define PREDICTION_H

#include <QObject>

#include "core/dal/entity.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"
#include "core/dal/drug/parameters.h"

namespace ezechiel {
namespace GuiCore {

class DrugResponseAnalysis;
class DrugTreatment;
class PredictionSpec;

enum CurveType {
    Continous,  //! A continuous curve is a curve that starts with an intial dosage (the first take) and an initial concentration of 0.
    Partial     //! A partial curve is a curve that starts directly at steady state given an initial dosage and observation date or measure.
};

class Prediction : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Prediction)

    AUTO_PROPERTY_DECL(QString, name, Name)
    AUTO_PROPERTY_DECL(QString, comment, Comment)
    AUTO_PROPERTY_DECL(CurveType, curveType, CurveType)
    AUTO_PROPERTY_DECL(ParameterType, paramsType, ParamsType)
    AUTO_PROPERTY_DECL(DrugResponseAnalysis*, analysis, Analysis)

    AUTO_PROPERTY_DECL(PredictionSpec*, spec, Spec)

//    Q_ENUMS(CurveType)


protected:


    Q_INVOKABLE Prediction(AbstractRepository *repository = 0, QObject* parent = 0);
    Q_INVOKABLE Prediction(AbstractRepository *repository, QObject* parent, DrugResponseAnalysis*);

public:

//    QDateTime firsttake() const;
    void setFirsttake(const QDateTime &firsttake);

    DrugTreatment *getTreatment() const;

signals:

private:

};

} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::GuiCore::Prediction*)
Q_DECLARE_METATYPE(QList<ezechiel::GuiCore::Prediction*>)

#endif // PREDICTION_H
