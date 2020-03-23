#include "testmessagevalidator.h"
#include "restclient/utils/meldworkerthread.h"
#include "restclient/utils/settings.h"
#include "restclient/utils/utils.h"

TestMessageValidator::TestMessageValidator()
{

}

TestMessageValidator::~TestMessageValidator()
{

}

void TestMessageValidator::validate(const QString &received, const QString &expected, const QString &messageType)
{
    bool valid = received == expected;
    qMessage() << "The message '" << messageType << "' is " << (valid ? "valid!" : "invalid...") << endl;

    if (!valid && !Settings::getInstance().get("disable-meld").toBool()) {
        MeldWorkerThread *meld = new MeldWorkerThread(received, expected);
        meld->start();
    }
}
