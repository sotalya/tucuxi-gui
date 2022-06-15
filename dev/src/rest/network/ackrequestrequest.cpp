//@@license@@

#include "ackrequestrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

const char *const AckRequestRequest::_PATH = "query";

AckRequestRequest::AckRequestRequest() :
    MirthRequest()
{
    updatePath(_PATH);
}

AckRequestRequest::~AckRequestRequest()
{

}

}
}
}
