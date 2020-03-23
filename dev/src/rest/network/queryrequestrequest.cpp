#include "queryrequestrequest.h"

const char *const QueryRequestRequest::_PATH = "query";

QueryRequestRequest::QueryRequestRequest() :
    MirthRequest()
{
    updatePath(_PATH);
}

QueryRequestRequest::~QueryRequestRequest()
{

}
