#include "querylistrequest.h"

const char *const QueryListRequest::_PATH = "query";

QueryListRequest::QueryListRequest() :
    MirthRequest()
{
    updatePath(_PATH);
}

QueryListRequest::~QueryListRequest()
{

}
