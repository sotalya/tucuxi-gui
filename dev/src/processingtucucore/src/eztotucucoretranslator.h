//@@license@@

#ifndef EZTOTUCUCORETRANSLATOR_H
#define EZTOTUCUCORETRANSLATOR_H

#include <QDateTime>
#include <QString>

#include "core/interfaces/processingtraits.h"

#include "tucucommon/datetime.h"

#include "tucucore/drugdefinitions.h"
#include "tucucore/computingservice/computingtrait.h"

namespace Tucuxi {
namespace Core {
class DrugTreatment;
class DrugModel;
class DosageTimeRange;
}
}

namespace ezechiel {
namespace GuiCore {
class DrugTreatment;
class DrugModel;
class ParamTraits;
class Dosage;
}
}

class EzToTucucoreTranslator
{
public:
    EzToTucucoreTranslator();

    Tucuxi::Common::DateTime buildDateTime(const QDateTime &qDate);


    Tucuxi::Common::TucuUnit buildUnit(const QString &_strUnit);

    Tucuxi::Core::PredictionParameterType buildParameterType(const ezechiel::GuiCore::ParamTraits *traits);

    Tucuxi::Core::DosageTimeRange *buildTimeRange(const ezechiel::GuiCore::Dosage *_ezDosage);

    Tucuxi::Core::DrugTreatment *buildTreatment(const ezechiel::GuiCore::DrugTreatment *_ezTreatment, QDateTime adjTime = QDateTime());
    Tucuxi::Core::DrugModel *buildDrugModel(const ezechiel::GuiCore::DrugModel *_drugModel);

};

#endif // EZTOTUCUCORETRANSLATOR_H
