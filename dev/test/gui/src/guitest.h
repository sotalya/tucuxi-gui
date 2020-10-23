#include <QTest>

#include <Spix/QtQmlBot.h>

#include "guiutils/src/mainwindowcontroller.h"

class QQuickWindow;


class SpixGTest : public spix::TestServer {
public:
    SpixGTest(MainWindowController *mainWindowController, QQuickWindow *window, int argc, char* argv[]);
    int testResult();

    QQuickWindow* m_window;
    MainWindowController *m_mainWindowController;

    bool synchronize();

    void waitPeriod(unsigned int nbPeriods = 1)
    {
        wait(m_period * nbPeriods);
    }

    void startNewPatient();

protected:
    int m_argc;
    char** m_argv;
    std::atomic<int> m_result {0};

    std::chrono::milliseconds m_period{std::chrono::milliseconds(500)};



    void executeTest() override;
};


#define CHECKSPIXERRORS \
{auto spixErrors = srv->getErrors(); \
EXPECT_EQ(spixErrors.size(), 0); \
for (const auto &spixError : spixErrors) { \
    EXPECT_EQ(spixError, ""); \
}} \
