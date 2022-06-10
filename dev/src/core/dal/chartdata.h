//@@license@@

#ifndef CHARTDATA_H
#define CHARTDATA_H

#include <QObject>
#include <QString>
#include "core/utils/autoproperty.h"
#include "core/dal/predictionresult.h"
#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class ChartData : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(ChartData)

    AUTO_PROPERTY_DECL(PredictionResult*, popPred, PopPred)
    AUTO_PROPERTY_DECL(PredictionResult*, aprPred, AprPred)
    AUTO_PROPERTY_DECL(PredictionResult*, apoPred, ApoPred)
    AUTO_PROPERTY_DECL(PredictionResult*, revPred, RevPred)
    AUTO_PROPERTY_DECL(PredictionResult*, adjPred, AdjPred)

public:

    void setInfo(QString key, QString value) {
        info[key] = value;
    }

    Q_INVOKABLE QString getInfo(QString key) const {
        return info[key];
    }


public:
    explicit ChartData(AbstractRepository *repository,QObject *parent = nullptr);

    void initialize();

protected:

    QMap<QString, QString> info;

};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ChartData*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::ChartData*>)

#endif // CHARTDATA_H
