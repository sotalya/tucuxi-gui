//@@license@@

#include "acklistrequest.h"

const char *const AckListRequest::_PATH = "query";

AckListRequest::AckListRequest() :
    MirthRequest()
{
    updatePath(_PATH);
}

AckListRequest::~AckListRequest()
{

}
