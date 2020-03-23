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
namespace core {
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


    Tucuxi::Core::Unit buildUnit(const QString &_strUnit);

    Tucuxi::Core::PredictionParameterType buildParameterType(const ezechiel::core::ParamTraits *traits);

    Tucuxi::Core::DosageTimeRange *buildTimeRange(const ezechiel::core::Dosage *_ezDosage);

    Tucuxi::Core::DrugTreatment *buildTreatment(const ezechiel::core::DrugTreatment *_ezTreatment, QDateTime adjTime = QDateTime());
    Tucuxi::Core::DrugModel *buildDrugModel(const ezechiel::core::DrugModel *_drugModel);

};

#endif // EZTOTUCUCORETRANSLATOR_H
