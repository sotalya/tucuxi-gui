//@@license@@

#ifndef ACKLISTREQUEST_H
#define ACKLISTREQUEST_H

#include "rest/network/mirthrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class AckListRequest : public MirthRequest
{

public:
    AckListRequest();
    virtual ~AckListRequest();

private:
    static const char *const _PATH;
};

}
}
}

#endif // ACKLISTREQUEST_H
