//@@license@@

#include <QTest>

#include <Spix/QtQmlBot.h>

#include "guiutils/src/mainwindowcontroller.h"

class QQuickWindow;

struct PatientData {
    QString firstName       = "Joan";
    QString lastName        = "Of Arc";
    QDate birthDate         = QDate(1934, 11, 10);
    int gender              = 0;                    // gender : 1 = male, 0 = female
    QString identifier      = "Patient_0";
    QString stayNumber      = "Private Drive 4";
    QString titlePhy        = "Dr.";
    QString firstNamePhy    = "Doo";
    QString lastNamePhy     = "Little";
};


struct DosageData {
    double dosage           = 5555;
    double interval         = 2400;
    double infusion         = 6543;
    bool steadyState        = true;
    QDateTime dateTimeDos1  = QDateTime::currentDateTime().addDays(0);
    QDateTime dateTimeDos2  = QDateTime::currentDateTime().addDays(7);
};

struct CovariatesData {
    QString sex;
    double weight           = 67.87;
    double scc              = 80;
    QDateTime dateTimeCovar = QDateTime::currentDateTime();
};

struct MeasureData {
    QString name = "Sample_a_124";
    double value = 1589;
    QDateTime dateTimeMeas = QDateTime::currentDateTime();
};

struct TargetData {
    int targetType          = 0;
    double cMinInput        = 3000;
    double cBestInput       = 3500;
    double cMaxInput        = 4000;
    double tMinInput;
    double tBestInput;
    double tMaxInput;
    double micInput;
};

struct AdjustmentsData {
    QDateTime dateTimeAdj   = QDateTime::currentDateTime().addDays(2);
    bool loadingDose        = true;
    bool restPeriod         = false;
    int suggestAdjNum       = 1;
    int dose                = 666;
    int interval            = 36;
};

struct ValidationData {
    QDateTime dateTimeVal   = QDateTime::currentDateTime().addDays(7);
    QString expectedness    = "Full rehabilitation";
    QString suitability     = "Any";
    QString prediction      = "Sunny but windy this afternoon";
    QString remonitoring    = "None";
    QString warning         = "Don't invest in Bitcoin";
};



class SpixGTest : public spix::TestServer {
public:
    SpixGTest(MainWindowController *mainWindowController, QQuickWindow *window, int argc, char* argv[]);
    int testResult();

    QQuickWindow* m_window;
    MainWindowController *m_mainWindowController;

    QObject *getObjectByName(QObject *root, std::string name);

    bool synchronize();

    void waitPeriod(unsigned int nbPeriods = 1)
    {
        wait(m_period * nbPeriods);
        synchronize();
    }

    void waitForSync();

    int getCurrentTabIndex();

    void startNewPatient();

    void saveIntepretation(QString saveName);
    void loadIntepretation(QString loadName);

    void findObjectAndSetValue(QString objectName, int inputProperty);

    void selectDrugInList(int drugIndex, int modelIndex);

    void fillInPatientData(struct PatientData);

    void addDosage(struct DosageData);
    void editDosage(struct DosageData, int editIndex);
    QVariant getSteadyStateDosage();
    void setSteadyStateDosage(bool value);
    void fillInDosageData(struct DosageData);

    void addCovariates(struct CovariatesData, int covariateType);
    void editCovariates(struct CovariatesData, int covariateType, int editIndex);
    void fillInCovariatesData(struct CovariatesData, int covariateType);

    void addMeasure(struct MeasureData);
    void editMeasure(struct MeasureData, int editIndex);
    void fillInMeasureData(struct MeasureData);

    void addTarget(struct TargetData);
    void editTarget(struct TargetData, int editIndex);
    void fillInTargetData(struct TargetData);
    void removeFromList(std::string removeFrom, int removeIndex);

    void addAdjustments(struct AdjustmentsData);
    void selectAdjustments(int editIndex);
    void editAdjustments(AdjustmentsData adjustmentsData1, int editIndex);


    void fillInValidationData(struct ValidationData);
    void saveValidationComment(QString commentType, int globalSpecificIndex);
    void deleteValidationComment(QString commentType, int globalSpecifixIndex, int commentIndex);
    void validateInterpretation();


    void printReport(QString reportFileName);

    int getNbProposedAdjustments();

signals:
    void loadInterpretation(QString);

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
