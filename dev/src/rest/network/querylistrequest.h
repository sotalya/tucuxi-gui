//@@license@@

#ifndef QUERYLISTREQUEST_H
#define QUERYLISTREQUEST_H

#include "rest/network/mirthrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class QueryListRequest : public MirthRequest
{

public:
    QueryListRequest();
    virtual ~QueryListRequest();

private:
    static const char *const _PATH;
};

}
}
}


#endif // QUERYLISTREQUEST_H
