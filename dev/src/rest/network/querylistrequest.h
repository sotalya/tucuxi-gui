//@@license@@

#ifndef QUERYLISTREQUEST_H
#define QUERYLISTREQUEST_H

#include "rest/network/mirthrequest.h"

class QueryListRequest : public MirthRequest
{

public:
    QueryListRequest();
    virtual ~QueryListRequest();

private:
    static const char *const _PATH;
};

#endif // QUERYLISTREQUEST_H
