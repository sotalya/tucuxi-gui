//@@license@@

#ifndef FLATINTERPRETATIONREQUESTBUILDER_H
#define FLATINTERPRETATIONREQUESTBUILDER_H


#include <QDomDocument>

#include "core/dal/dosage.h"
#include "admin/src/dal/interpretationrequest.h"
#include "flatrequestparameters.h"


namespace Tucuxi {
namespace Gui {
namespace FlatRequest {

class FlatInterpretationRequestBuilder
{

public:
//    explicit InterpretationRequestBuilder(const MessageContent &content);
    explicit FlatInterpretationRequestBuilder(const QDomDocument &content);
    ~FlatInterpretationRequestBuilder();

    Tucuxi::Gui::Admin::InterpretationRequest* buildInterpretationRequest();

private:
//    MessageContent content;
    QDomDocument content;
    QDomElement reportNode;

    FlatRequestParameters* flatRequestParameters;

    static bool compareDosage(const Tucuxi::Gui::Core::Dosage* a, const Tucuxi::Gui::Core::Dosage* b);
    Tucuxi::Gui::Core::Duration findDuration(const QDomElement &currentElement);
    void createUncastedIntervalValue(Tucuxi::Gui::Core::Dosage* dosage, int interval_sec);
    void createUncastedDosageValue(Tucuxi::Gui::Core::Dosage *dosage, QString field, QString text, QString comment);
    void splitOverlappingDosage(Tucuxi::Gui::Core::DosageHistory* dosages);
    void setDosageEndDateInterval(Tucuxi::Gui::Core::DosageHistory* dosages);
    void groupDosage(Tucuxi::Gui::Core::DosageHistory* dosages);
};

}
}
}


#endif // FLATINTERPRETATIONREQUESTBUILDER_H
