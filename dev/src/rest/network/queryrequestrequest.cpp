//@@license@@

#include "queryrequestrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

const char *const QueryRequestRequest::_PATH = "query";

QueryRequestRequest::QueryRequestRequest() :
    MirthRequest()
{
    updatePath(_PATH);
}

QueryRequestRequest::~QueryRequestRequest()
{

}

}
}
}
