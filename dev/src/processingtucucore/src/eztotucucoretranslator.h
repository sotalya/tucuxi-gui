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

namespace Tucuxi {
namespace Gui {
namespace Core {

class DrugTreatment;
class DrugModel;
class ParamTraits;
class Dosage;
}
}
}


class EzToTucucoreTranslator
{
public:
    EzToTucucoreTranslator();

    Tucuxi::Common::DateTime buildDateTime(const QDateTime &qDate);


    Tucuxi::Common::TucuUnit buildUnit(const QString &_strUnit);

    Tucuxi::Core::PredictionParameterType buildParameterType(const Tucuxi::Gui::Core::ParamTraits *traits);

    Tucuxi::Core::DosageTimeRange *buildTimeRange(const Tucuxi::Gui::Core::Dosage *_ezDosage);

    Tucuxi::Core::DrugTreatment *buildTreatment(const Tucuxi::Gui::Core::DrugTreatment *_ezTreatment, QDateTime adjTime = QDateTime());
    Tucuxi::Core::DrugModel *buildDrugModel(const Tucuxi::Gui::Core::DrugModel *_drugModel);

};



#endif // EZTOTUCUCORETRANSLATOR_H
