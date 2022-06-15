//@@license@@

#ifndef QUERYREQUESTREQUEST_H
#define QUERYREQUESTREQUEST_H

#include "rest/network/mirthrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class QueryRequestRequest : public MirthRequest
{

public:
    QueryRequestRequest();
    virtual ~QueryRequestRequest();

private:
    static const char *const _PATH;
};

}
}
}

#endif // QUERYREQUESTREQUEST_H
