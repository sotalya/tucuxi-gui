//@@license@@

#include "ackrequestrequest.h"

const char *const AckRequestRequest::_PATH = "query";

AckRequestRequest::AckRequestRequest() :
    MirthRequest()
{
    updatePath(_PATH);
}

AckRequestRequest::~AckRequestRequest()
{

}
