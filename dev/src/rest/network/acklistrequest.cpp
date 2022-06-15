//@@license@@

#include "acklistrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

const char *const AckListRequest::_PATH = "query";

AckListRequest::AckListRequest() :
    MirthRequest()
{
    updatePath(_PATH);
}

AckListRequest::~AckListRequest()
{

}

}
}
}
