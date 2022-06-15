//@@license@@s

#ifndef ACKREQUESTREQUEST_H
#define ACKREQUESTREQUEST_H

#include "rest/network/mirthrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class AckRequestRequest : public MirthRequest
{

public:
    AckRequestRequest();
    virtual ~AckRequestRequest();

private:
    static const char *const _PATH;
};

}
}
}

#endif // ACKREQUESTREQUEST_H
