#include <QTest>

#include <Spix/QtQmlBot.h>

#include "guiutils/src/mainwindowcontroller.h"

class QQuickWindow;

struct PatientData {
    QString firstName;
    QString lastName;
    QDate birthDate;
    int gender;
    QString identifier;
    QString stayNumber;
    QString titlePhy;
    QString firstNamePhy;
    QString lastNamePhy;
};


struct DosageData {
    double dosage = 5555;
    double interval = 2400;
    double infusion = 2468;
    bool steadyState = true;
    QDateTime dateTimeDos1 = QDateTime::currentDateTime();
    QDateTime dateTimeDos2 = QDateTime::currentDateTime().addDays(7);
};

struct CovariatesData {
    QString sex;
    double weight;
    double scc;
    QDateTime dateTimeCovar;

};

struct MeasureData {
    QString name;
    double value;
    QDateTime dateTimeMeas;
};

struct TargetData {                 // JRT 23.02.2022
    int targetType;
    double cMinInput;
    double cBestInput;
    double cMaxInput;
    double tMinInput;
    double tBestInput;
    double tMaxInput;
    double micInput;
};

struct AdjustmentsData {
    QDateTime dateTimeAdj;
    bool loadingDose;
    bool restPeriod;
    int suggestAdjNum;
};

struct ValidationData {
    QDateTime dateTimeVal;
    QString expectedness;
    QString suitability;
    QString prediction;
    QString remonitoring;
    QString warning;
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

    void fillInValidationData(struct ValidationData);
    void saveValidationComment(QString commentType, int globalSpecificIndex);
    void deleteValidationComment(QString commentType, int globalSpecifixIndex, int commentIndex);
    void validateInterpretation();


    void printReport(QString reportFileName);

    int getNbProposedAdjustments();

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
