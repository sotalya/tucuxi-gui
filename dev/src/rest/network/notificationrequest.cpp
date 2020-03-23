#include "notificationrequest.h"

const char *const NotificationRequest::_PATH = "notify";

NotificationRequest::NotificationRequest() :
    MirthRequest()
{
    updatePath(_PATH);
}

NotificationRequest::~NotificationRequest()
{

}
