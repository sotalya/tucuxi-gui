//@@license@@

#ifndef NOTIFICATIONREQUEST_H
#define NOTIFICATIONREQUEST_H

#include "rest/network/mirthrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class NotificationRequest : public MirthRequest
{

public:
    NotificationRequest();
    virtual ~NotificationRequest();

private:
    static const char *const _PATH;
};

}
}
}

#endif // NOTIFICATIONREQUEST_H
