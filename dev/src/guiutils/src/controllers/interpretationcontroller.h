//@@license@@

#ifndef INTERPRETATIONCONTROLER_H
#define INTERPRETATIONCONTROLER_H


#include <QQuickView>
#include <QList>
#include <QDateTime>
//#include <QWebEngineView>
#include <QWebChannel>
#include <QtWebSockets/QtWebSockets>
#include <QtWebSockets/QWebSocketServer>
#include <QQmlWebChannel>

#include "core/utils/autoproperty.h"

#include "guiutils/src/startupwindow.h"
#include "guiutils/src/websocketclientwrapper.h"
#include "guiutils/src/websockettransport.h"
#include "guiutils/src/calculationcontroller.h"
#include "admin/src/dal/sentencepalette.h"

class CalculationController;
class ChartDataController;

class Report;
class QPrinter;
class PersistentFileDialog;

class InterpretationRequest;
class Interpretation;
class Patient;
class GraphInformationSelection;

class FlowController;

class MeasureTabController;
class DosageTabController;
class TargetTabController;
class CovariateTabController;
class AdjustmentTabController;
class DrugTabController;
class ValidationTabController;

namespace ezechiel {
namespace core {
class PredictionResult;
class ChartData;
class PredictionSpec;
class DrugModel;
class DrugModelList;
class Dosage;
class CorePatient;
class CorePatientList;
class TargetList;
class ActiveSubstanceList;
class LightActiveSubstanceList;
class LightActiveSubstance;
class ActiveSubstance;
class AdminList;
class DosageHistory;
}
}


enum class EViewRangeUpdateContext {
    Start,
    Dosage,
    AdjustmentDate,
    Adjustment,
    UserResize
};


class InterpretationController : public QObject
{
    Q_OBJECT


    /**
      The following properties expose various aspects of an interpretation to the different
      QML tabs. They should be set up when starting a flow, either as a pending request
      or as a new interpretation.
      Exposing this through this properties decouples the interpretation object tree from
      the views, which is a good design practice. Doing so, all tabs can access the properties
      through the interpretationController object.
      We have to be sure to set all these properties when starting a new interpretation,
      and to set them to 0 when an interpretaion ends.
      The Interpretation has to ensure these properties are always valid.
      */

    //! This property is used by PatientTab.qml
    //! TODO It should deal with patients, not core patients
    Q_PROPERTY(ezechiel::core::CorePatientList* patients READ getPatients NOTIFY patientsChanged)

    //! This property is used by DrugTab.qml
    Q_PROPERTY(ezechiel::core::DrugModelList* drugs READ getDrugs NOTIFY drugsChanged)

    //! This property is used by DrugTab.qml
    Q_PROPERTY(ezechiel::core::LightActiveSubstanceList* activeSubstances READ getActiveSubstances NOTIFY activeSubstancesChanged)


    //! This property is used by DrugTab.qml
    Q_PROPERTY(ezechiel::core::DrugModelList* drugModels READ getDrugModels NOTIFY drugModelsChanged)

    //! This property exposes the current patient.
    //! It is used by the FlowInformationPanel to display information about the current patient
    //! Compared to the previous properties, it is modified by the PatientTab
    Q_PROPERTY(Patient* currentPatient READ getCurrentPatient WRITE setCurrentPatient NOTIFY newCurrentPatient)


    //! This property exposes the current drugModel.
    STD_PROPERTY_DECL(ezechiel::core::DrugModel*, currentDrugModel, CurrentDrugModel)

    //! This property exposes the current active substance.
    //! It is used by the FlowInformationPanel to display information about the current active substance
    //! Compared to the previous properties, it is modified by the DrugTab
    STD_PROPERTY_DECL(ezechiel::core::LightActiveSubstance*, currentActiveSubstance, CurrentActiveSubstance)

    //! This property exposes the current interpretation.
    //! It is used by various tabs
    STD_PROPERTY_DECL(Interpretation*, interpretation, Interpretation)

    //! This property is used by RawRequest.qml
    //! Only used in request mode
    STD_PROPERTY_DECL(QString, rawRequest, RawRequest)

    //! This property is used by RawRequest.qml
    //! Only used in request mode
    STD_PROPERTY_DECL(QString, clinicalsHtml, ClinicalsHtml)

    //! This property is used by ValidationTab.qml and ReportTab.xml
    //! Only used in request mode
    Q_PROPERTY(bool isInterpretationValidated READ isInterpretationValid WRITE validateInterpretation NOTIFY interpretationValidated)

    Q_PROPERTY(QString defaultUnit READ getDefaultUnit WRITE setDefaultUnit NOTIFY defaultUnitChanged)
    Q_PROPERTY(double chartscale READ getChartscale WRITE setChartscale NOTIFY chartscaleChanged)
    Q_PROPERTY(QQmlWebChannel* webchannel READ getWebchannel NOTIFY webchannelChanged)


//    Q_PROPERTY(std::vector<double> percv READ getPoppv WRITE setPoppv NOTIFY percvChanged)
//    Q_PROPERTY(std::vector<double> aprpv READ getAprpv WRITE setAprpv NOTIFY aprpvChanged)
//    Q_PROPERTY(std::vector<double> apopv READ getApopv WRITE setApopv NOTIFY apopvChanged)

    STD_PROPERTY_DECL(bool, isDemo, IsDemo)

    //! For testing purpose.
    //! Allows to set the name of the report form the GUI tests, and to retrieve it from QML
    STD_PROPERTY_DECL(QString, reportFileName, ReportFileName);

public:

    // Get the flow view
    QObject *view() const;

    // ToDo-MVC ///////////////////////////////////////////////////////////////
    // Probably better in a report controler, along with all the report stuff
    // ////////////////////////////////////////////////////////////////////////

    QQmlWebChannel* getWebchannel() { return static_cast<QQmlWebChannel*>(_webchannel);}

    enum PublicationType {
        Pdf,
        Printer,
        Remote
    };
    Q_ENUM(PublicationType)

    explicit InterpretationController(QObject *parent = nullptr);
    virtual ~InterpretationController() Q_DECL_OVERRIDE;

    void initAfterQmlLoading(QObject *root, QQmlContext *rootContext);

    Patient* _currentPatient;

    void setCurrentPatient(Patient *patient);
    Patient * getCurrentPatient() const;

    ezechiel::core::DrugModelList* getDrugs() { return _drugs; }
    ezechiel::core::DrugModelList* getDrugModels() { return _drugModelsForCurrentSubstance; }
    ezechiel::core::LightActiveSubstanceList* getActiveSubstances() { return _activeSubstances; }
    ezechiel::core::CorePatientList* getPatients() { return _patients; }

//    std::vector<double> getAprpv() { return _aprpv; }
//    std::vector<double> setAprpv(std::vector<double> vec) { _aprpv = vec; emit aprpvChanged(_aprpv); }

//    std::vector<double> getApopv() { return _apopv; }
//    std::vector<double> setApopv(std::vector<double> vec) { _apopv = vec; emit apopvChanged(_apopv); }

    QString getDefaultUnit() { return _defaultUnit; }
    void setDefaultUnit(QString unit) {_defaultUnit = unit; emit defaultUnitChanged(unit);}

    double getChartscale() { return _chartscale; }
    void setChartscale(const double scale) {
        _chartscale = scale;
        emit chartscaleChanged(_chartscale); }

    Q_INVOKABLE QVariantHash hashToJS(QVariant);

    Q_INVOKABLE double getViewRangeMin() { return _minX.toMSecsSinceEpoch() / 1000; }
    Q_INVOKABLE double getViewRangeMax() { return _maxX.toMSecsSinceEpoch() / 1000; }
    Q_INVOKABLE void resetViewRange();
    Q_INVOKABLE void shiftViewRange(double dX);
    Q_INVOKABLE void zoomViewRange(double f);
    void setDateViewRange(const QDateTime &date);
    Q_INVOKABLE void setViewRange(const QDateTime &minX, const QDateTime &maxX);
    Q_INVOKABLE void goToNextEvent();
    Q_INVOKABLE void goToPreviousEvent();
    QList<QDateTime> buildEventsList();

    Q_INVOKABLE void updateSexAndAgeCovariates();

    void deleteCurrentRequest();

    Q_INVOKABLE void addPatient();
    Q_INVOKABLE void removePatient(int index);

    // For testing purposes
    Q_INVOKABLE bool isTesting();

    // Translate a Url into a local file string
    Q_INVOKABLE QString handleFileChosen(const QString &urlString);


    //Q_INVOKABLE void setExtCurrentActiveType(int index);        // JRT 24.02.2022
    ///
    /// \brief saveStatistics
    /// Saves statistics in a file
    void saveStatistics();

    ///
    /// \brief showStatistics
    /// \param what Defines what has to be shown
    /// Show statistics in a dialog
    void showStatistics(int what);

    bool associateFormulationToRoute(ezechiel::core::DosageHistory *dosageHistory, ezechiel::core::AdminList *adminList);

    Q_INVOKABLE QByteArray interpretationToJson();

    ///
    /// \brief saveGraph
    /// Opens a dialog to select a file, and then saves the current graph in it
    ///
    void saveGraph();

signals:
    void webchannelChanged(QQmlWebChannel&);
    void drugsChanged(ezechiel::core::DrugModelList*);
    void drugModelsChanged(ezechiel::core::DrugModelList*);
    void activeSubstancesChanged(ezechiel::core::LightActiveSubstanceList*);
    void patientsChanged(ezechiel::core::CorePatientList*);
    void defaultUnitChanged(QString);
    void chartscaleChanged(double);

    void newCurrentPatient();

    void interpretationValidated();

public slots:

    void goToSaveInterpretation();
    void saveInterpretation(QString);

    void publishReport(QImage);
    void resetReqState();
//    QVariant getGraphDrugData();
    void setReportTabShow(QVariant);

    void currentTabChanged(int index);
//    void currentTimeChanged(QDateTime time);
    void currentDomainChanged(int index);

    /**
     * @brief switchActiveSubstance
     * @param index
     * @param loading This boolean should be true when an interpretation starts from a pending request,
     * or when an interpretation or a request file is loaded
     */
    void switchActiveSubstance(int index);
    void switchDrugModel(int index);
    void currentStudyChanged(int index);


public:

    /**
     * @brief acceptAndQuit Check if the flow can be deleted to start a new flow
     * @return true if the user validated the fact that the flow data can be discarded, false else
     * If the user accepts to quit the current flow, then all data are discarded.
     */
    bool acceptAndQuit();

    /**
     * @brief startInterpretationRequest Starts the flow with an existing interpretation request
     * @param interpretation
     * This function is one of the two to start an interpretation flow, the other one
     * being startNewPatient().
     */
    void startInterpretationRequest(InterpretationRequest *_interpretation);

    /**
     * @brief startNewPatient Starts the flow with a new patient
     * This function is one of the two to start an interpretation flow, the other one
     * being startInterpretationRequest().
     */
    void startNewPatient();

    /**
     * @brief loadInterpretation Starts the flow with an existing interpretation
     * @param interpretation
     * This function is one of the three to start an interpretation flow.
     */
    void loadInterpretation(Interpretation *interpretation);
    void extSetViewRange();
//    void extSetViewRange();

    ezechiel::core::PredictionSpec *getPredictionSpec() const;


    void validateInterpretation(bool isValid);
    bool isInterpretationValid();

    ///
    /// \brief setNewInterpretation
    /// \param interpretation
    /// \param newInterpretation If true it is a new interpretation, if false, we are loading an existing one
    ///
    void setNewInterpretation(Interpretation *interpretation, bool newInterpretation);

public:

    /**
     * @brief bindModelsToRootContext Binds the models to the root context
     * @param rootContext The root context of the main QQuickView
     * This method should be called before loading the QML files, else a couple
     * of warnings appear
     */
    void bindModelsToRootContext(QQmlContext *rootContext);

public slots:
    bool isFlowRequest() const;

private:

    void retrieveViewsFromQml();
    void initViewConnexions();
    void clearAdjustments();

    ezechiel::core::ActiveSubstance *findRealActiveSubstance(const ezechiel::core::LightActiveSubstance *activeSubstance);

#ifdef CONFIG_GUITEST
    QObject *root;
#else
    QQuickItem *root;
#endif // CONFIG_GUITEST

    QQmlContext *rootContext;

    /**
     * @brief populateDrugModels populates the list of drug models available in the software
     * This Function actually does the initialization only once. The drug models should not change
     */
    void populateDrugModels();

    /**
     * @brief populateSingleActiveSubstance populates the list of available active substances and drug models
     * @param activeSubstanceId The Id of the current active substance.
     * It populates with a single active substance corresponding to the one specified.
     */
    void populateSingleActiveSubstance(const QString &activeSubstanceId);

    /**
     * @brief populateMultipleActiveSubstance populates the list of available active substances and drug models
     * It adds all available active substances.
     */
    void populateMultipleActiveSubstance();

    /**
     * @brief getLastDataTime get the time of last data recording
     * @param interpretation
     * @return The time of last data recording
     * This function will look at measures, dosage history, and covariates to find the
     * last one recorded. It allows to evaluate the full process time.
     */
    QDateTime getLastDataTime(Interpretation *interpretation) const;

    double _chartscale;
    QString _defaultUnit;

//    ezechiel::core::ChartData* chartData;     // JRT

    ezechiel::core::PredictionSpec* predictionspec;
    ezechiel::core::DrugModelList* _drugs;
    ezechiel::core::DrugModelList* _drugModelsForCurrentSubstance;
    ezechiel::core::LightActiveSubstanceList* _activeSubstances;
    ezechiel::core::LightActiveSubstanceList* _privateActiveSubstances;
    ezechiel::core::CorePatientList* _patients;

    GraphInformationSelection *_graphInformationSelection;

    SentencesPalettes *_sentencesPalettes;

    FlowController* flowController;
    CalculationController* calculationController;

//    ChartDataController *_chartDataController;        // JRT

//    QObject *chartView;
    QObject *domainsView;
    QObject *dosagesView;
    QObject *drugVariatesView;
    // Added drugsView & targetsView as public for testing purposes
    public:

    ChartDataController *_chartDataController;        // JRT
    ezechiel::core::ChartData* chartData;             // JRT

    QObject *patientsView;
    QObject *drugsView;
    QObject *targetDialog;
    QObject *adjustmentsView;
    QObject *covariatesView;
    QObject *covariateDialog;
    QObject *validationView;
    QObject *sentencePaletteDialog;
    QObject *chartView;


public: // For RandomTest
    QObject *flowView;
private:
    QObject *measuresView;
    QObject *targetsView;
    QObject *studiesView;

    QObject *reportView;
    QObject *webengineView;
    QQmlWebChannel *_webchannel;

    MeasureTabController *measureTabController;
    DosageTabController *dosageTabController;
    TargetTabController *targetTabController;
    CovariateTabController *covariateTabController;
    AdjustmentTabController *adjustmentTabController;
    ValidationTabController *validationTabController;

    DrugTabController *drugTabController;

    QScopedPointer<QPrinter> printer;
    QScopedPointer<PersistentFileDialog> exportFileDialog;

    QDateTime _minX;
    QDateTime _maxX;
    QDateTime _userRequestedMinX;
    QDateTime _userRequestedMaxX;
    bool shouldPercentilesBeComputed; // True if the view range is not too large

public slots:

    void validateTab(int step);
    void toggleValidation(int step);
    void dosageUpdated(bool clearAdjustment = true);
    void chartScaleChanged();
    void measureUpdated();
    void evaluateFlow();
    void targetUpdated();
    void covariateUpdated();
    void adjustmentDateUpdated();
    void adjustmentSettingsUpdated();
    void adjustmentUpdated();
//    void extSetViewRange();
    bool exportReport(Report *report);
    bool printReport(Report *report);
    bool sendReport(Report *report);

    bool updateInterpretationRange(EViewRangeUpdateContext context);

};

#endif // INTERPRETATIONCONTROLER_H
