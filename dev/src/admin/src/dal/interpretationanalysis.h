#ifndef INTERPRETATIONANALYSIS_H
#define INTERPRETATIONANALYSIS_H

#include "core/dal/entity.h"
#include "core/dal/chartdata.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

class InterpretationAnalysis : public Tucuxi::Gui::Core::Entity
{

    Q_OBJECT
    ADMIN_ENTITY_UTILS(InterpretationAnalysis)

    //! the interpretation of the normality of the result by the analyst
    AUTO_PROPERTY_DECL(QString, expectedness, Expectedness)

    //! the interpretation of the appropriateness of drug exposure by the analyst
    AUTO_PROPERTY_DECL(QString, suitability, Suitability)

    AUTO_PROPERTY_DECL(Tucuxi::Gui::Core::ChartData*, chartData, ChartData)

    //! the recommendation of dosage adjustment by the analyst
    AUTO_PROPERTY_DECL(QString, prediction, Prediction)

    //! the recommendation for future monitoring by the analyst
    AUTO_PROPERTY_DECL(QString, remonitoring, Remonitoring)

    //! Some cautionary statement by the analyst
    AUTO_PROPERTY_DECL(QString, warning, Warning)

    AUTO_PROPERTY_DECL(QDateTime, nextControl, NextControl)

    AUTO_PROPERTY_DECL(QString, dosageDescription, DosageDescription)
protected:

    Q_INVOKABLE InterpretationAnalysis(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);

public:

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::InterpretationAnalysis*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::InterpretationAnalysis*>)

#endif // INTERPRETATIONANALYSIS_H

