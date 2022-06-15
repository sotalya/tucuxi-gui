//@@license@@

#include "notificationrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

const char *const NotificationRequest::_PATH = "notify";

NotificationRequest::NotificationRequest() :
    MirthRequest()
{
    updatePath(_PATH);
}

NotificationRequest::~NotificationRequest()
{

}

}
}
}
