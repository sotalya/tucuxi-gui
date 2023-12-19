//@@license@@

#ifndef GUITEST_H
#define GUITEST_H

#include <QTest>

#include <Spix/QtQmlBot.h>

#include "guiutils/src/mainwindowcontroller.h"
#include "defaultdrugconfig.h"

class QQuickWindow;

enum tabIndex{
    patientTab = 0,
    drugsTab = 1,
    dosagesTab = 2,
    covariatesTab = 3,
    measuresTab = 4,
    targetsTab = 5,
    adjustementsTab = 6,
    validationTab = 7,
    reportsTab = 8
};

class SpixGTest : public spix::TestServer {
public:
    SpixGTest(Tucuxi::Gui::GuiUtils::MainWindowController *mainWindowController, QQuickWindow *window, int argc, char* argv[]);
    int testResult();

    QQuickWindow* m_window;
    Tucuxi::Gui::GuiUtils::MainWindowController *m_mainWindowController;

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

    void findObjectAndSetValue(QString objectName, int inputProperty);

    void findDateObjectAndSetValue(QString dateObjectName, QString timeObjectName, QDateTime date);

    void findEntityTextFieldAndSetValue(QString objectName, QString textString);

    void findEntityTextValueFieldAndSetValue(QString objectName, double value);

    void mouseClickIfPathOk(std::string clickPath);

    void selectDrugInList(QString drugName, int modelIndex);

    void fillInPatientData(struct PatientData);

    void addDosage(struct DosageData);
    void editDosage(struct DosageData, int editIndex);
    QVariant getSteadyStateDosage();
    void setSteadyStateDosage(bool value);
    void fillInDosageData(struct DosageData);

    void addCovariates(struct CovariatesData, int covariateType);
    void editCovariates(struct CovariatesData, int covariateType, int editIndex);
    void fillInCovariatesData(struct CovariatesData, int covariateType);

    void addCovariatesByDrug(CovariatesData covariatesData1, int covariateType, QString drugName);
    void fillInCovariatesDataByDrug(struct CovariatesData, int covariateType, QString drugName);

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

    bool addGlobalSentencePalette(QString sentence, QString shortcutModifier, QString shortcutKey, QString section);
    bool addSpecificSentencePalette(QString sentence, QString shortcutModifier, QString shortcutKey, QString section);
    void removeAllSentences();

    void fillInValidationData(struct ValidationData);
    void saveValidationComment(QString commentType, int globalSpecificIndex);
    void deleteValidationComment(QString commentType, int globalSpecifixIndex, int commentIndex);
    void validateInterpretation();


    void printReport(QString reportFileName);

    int getNbProposedAdjustments();

    QMap<QString, QString> fillMapWithInterpretation(Tucuxi::Gui::Admin::Interpretation *interpretation);

    void fillInAnalystData(struct AnalystData);

signals:
    void loadInterpretation(QString);
    void extSetView(QDateTime, QDateTime);

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

#endif //GUITEST_H
