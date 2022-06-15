//@@license@@

#include "querylistrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

const char *const QueryListRequest::_PATH = "query";

QueryListRequest::QueryListRequest() :
    MirthRequest()
{
    updatePath(_PATH);
}

QueryListRequest::~QueryListRequest()
{

}

}
}
}
