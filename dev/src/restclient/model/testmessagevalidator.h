//@@license@@

#ifndef TESTMESSAGEVALIDATOR_H
#define TESTMESSAGEVALIDATOR_H

#include <QString>

class TestMessageValidator
{

public:
    TestMessageValidator();
    ~TestMessageValidator();

    void validate(const QString &received, const QString &expected, const QString &messageType);
};

#endif // TESTMESSAGEVALIDATOR_H
