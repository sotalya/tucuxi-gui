#ifndef ACKREQUESTREQUEST_H
#define ACKREQUESTREQUEST_H

#include "rest/network/mirthrequest.h"

class AckRequestRequest : public MirthRequest
{

public:
    AckRequestRequest();
    virtual ~AckRequestRequest();

private:
    static const char *const _PATH;
};

#endif // ACKREQUESTREQUEST_H
