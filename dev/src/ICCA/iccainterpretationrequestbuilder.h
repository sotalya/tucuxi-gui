#ifndef ICCAINTERPRETATIONREQUESTBUILDER_H
#define ICCAINTERPRETATIONREQUESTBUILDER_H


#include <QDomDocument>

#include "core/dal/drug/drug.h"
#include "core/dal/covariate.h"
#include "core/dal/dosage.h"
#include "admin/src/dal/institute.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/clinical.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/measure.h"
#include "admin/src/dal/interpretationrequest.h"


namespace Tucuxi {
namespace Gui {
namespace ICCA {

class ICCAInterpretationRequestBuilder
{

public:
//    explicit InterpretationRequestBuilder(const MessageContent &content);
    explicit ICCAInterpretationRequestBuilder(const QDomDocument &content);
    ~ICCAInterpretationRequestBuilder();

    Tucuxi::Gui::Admin::InterpretationRequest* buildInterpretationRequest();

private:
//    MessageContent content;
    QDomDocument content;
    QDomElement datasetNode;
};

}
}
}


#endif // ICCAINTERPRETATIONREQUESTBUILDER_H
