//@@license@@

#include <QTest>

#include <Spix/QtQmlBot.h>

#include "guiutils/src/mainwindowcontroller.h"

class QQuickWindow;

struct PatientData {
    QString firstName       = "Joan";
    QString lastName        = "Of Arc";
    QDate birthDate         = QDate(1960, 11, 10);
    int gender              = 0;                    // gender : 1 = male, 0 = female
    QString identifier      = "Patient_0";
    QString stayNumber      = "Private Drive 4";
    QString address         = "Rue du Flon 1";
    QString city            = "Lausanne";
    QString state           = "VD";
    QString postcode        = "1003";
    QString country         = "Switzerland";

    QString titlePhy       = "Dr.";
    QString firstNamePhy   = "Doo";
    QString lastNamePhy    = "Little";
    QString affiliationPhy = "Aff";
    QString addressPhy     = "Rte de Cheseaux 1";
    QString cityPhy        = "Yverdon-les-Bains";
    QString statePhy       = "VD";
    QString postcodePhy    = "1400";
    QString countryPhy     = "Switzerland";
};


struct DosageData {
    double dosage           = 5555;
    double interval         = 2400;
    double infusion         = 2468;
    bool steadyState        = true;
    QDateTime dateTimeDos1  = QDateTime::currentDateTime().addDays(0);
    QDateTime dateTimeDos2  = QDateTime::currentDateTime().addDays(7);
};

struct CovariatesData {
    QString sex;
    double weight           = 5566;
    double scc              = 80;
    QDateTime dateTimeCovar = QDateTime::currentDateTime();

    double dayNightDosing                     = 100;
    double asian                              = 100;
    double nonValvularAtrialFibriliation       = 0;
    double acuteCoronarySyndrome              = 0;
    double strongModerateCytochromeInhibitors = 100;
    double dose                               = 410;
    double glomerularFiltrationRate           = 9500;

    double atazanavirWithWithoutBooster = 0;
    double darunavir                    = 1;
    double rifampicin                   = 0;
    double currentSmoking               = 1;

    double gestionalAge             = 2600;
    double heamatologicalMalignacy  = 100;
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

struct AnalystData {
    QString analystTitle       = "Dre";
    QString analystFirstName   = "Jane";
    QString analystLastName    = "Doe";
    QString analystRole        = "Analyst";
    QString analystPhoneNumber = "123456789";
    QString analystAffiliation = "AffiliationName";
    QString analystAddress     = "Rue du Bugnon 46";
    QString analystCity        = "Lausanne";
    QString analystPostcode    = "1011";
    QString analystState       = "VD";
    QString analystCountry     = "Switzerland";


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

    void addCovariatesByDrug(CovariatesData covariatesData1, int covariateType, int drugIndex);
    void fillInCovariatesDataByDrug(struct CovariatesData, int covariateType, int drugIndex);

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

    QMap<QString, QString> fillMapWithInterpretation(Interpretation *interpretation);

    void fillInAnalystData(struct AnalystData);

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
