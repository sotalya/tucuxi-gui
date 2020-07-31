
#include <iostream>
#include <algorithm>

#include <QtWidgets>
#include <QQmlContext>
#include <QQuickItem>
#include <QPrinter>
#include <QMessageBox>
#include <QMetaProperty>
#include <QtWebSockets/QtWebSockets>
#include <QFileDialog>



#include "interpretationcontroller.h"
#include "errors_guiutils.h"



#include "core/utils/connect.h"
#include "core/corefactory.h"
#include "core/corerepository.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/prediction.h"
#include "core/dal/predictionspec.h"
#include "core/dal/dosage.h"
#include "core/dal/drug/drug.h"
#include "core/dal/coremeasure.h"
#include "core/dal/drug/target.h"
#include "core/dal/chartdata.h"
#include "core/utils/logging.h"
#include "core/settings.h"
#include "core/dal/drug/translatablestring.h"

#include "apputils/src/apputilsrepository.h"
#include "apputils/src/modelvstreatmentcompatibilitychecker.h"
#include "apputils/src/chartdataexporter.h"

#include "admin/src/adminfactory.h"
#include "admin/src/stdadminrepository.h"
#include "admin/src/dal/person.h"
#include "admin/src/dal/interpretation.h"
#include "admin/src/dal/interpretationrequest.h"
#include "admin/src/dal/interpretationanalysis.h"
#include "admin/src/dal/graphinformationselection.h"
#include "admin/src/interpretationxmlexport.h"
#include "admin/src/interpretationtorequestxmlexport.h"

#include "guiutils/src/flowcontroller.h"
#include "guiutils/src/drugtohtml.h"
#include "guiutils/src/startupwindow.h"
#include "guiutils/src/fakepatientscreator.h"
#include "guiutils/src/calculationcontroller.h"
#include "guiutils/src/multithreadedcalculationcontroller.h"
#include "guiutils/src/chartdatacontroller.h"
#include "guiutils/src/widgets/persistentfiledialog.h"
#include "guiutils/src/unavailablefunctionality.h"
#include "guiutils/src/appmode.h"

#include "measuretabcontroller.h"
#include "dosagetabcontroller.h"
#include "targettabcontroller.h"
#include "covariatetabcontroller.h"
#include "adjustmenttabcontroller.h"
#include "validationtabcontroller.h"
#include "drugtabcontroller.h"

using namespace ezechiel::core;

static const double NBCYCLES_TO_COMPUTE_PERCENTILES = 20.0;
static const double NBCYCLES_VIEWRANGE_MAX = 60.0;
static const double COMPUTATION_MARGIN_FACTOR = 0.3;
static const double NBCYCLES_AFTER_LASTDOSE = 3.0;
static const double NBCYCLES_BEFORE_NOW = 1.0;
static const double NBCYCLES_AFTER_NOW = 2.0;
static const double NBCYCLES_MAX_FOR_INITIAL_COMPUTATION = 15.0;

STD_PROPERTY_IMPL(InterpretationController, ezechiel::core::DrugModel*, currentDrugModel, CurrentDrugModel)
STD_PROPERTY_IMPL(InterpretationController, ezechiel::core::LightActiveSubstance*, currentActiveSubstance, CurrentActiveSubstance)
STD_PROPERTY_IMPL(InterpretationController, Interpretation*, interpretation, Interpretation)
STD_PROPERTY_IMPL(InterpretationController, QString, rawRequest, RawRequest)
STD_PROPERTY_IMPL(InterpretationController, QString, clinicalsHtml, ClinicalsHtml)
STD_PROPERTY_IMPL(InterpretationController, bool, isDemo, IsDemo)

InterpretationController::InterpretationController(QObject *parent) :
    QObject (parent),
    _currentDrugModel(nullptr),
    _currentActiveSubstance(nullptr),
    _interpretation(nullptr),
    _rawRequest(""),
    #ifdef CONFIG_EXTERNALREPORT
    _isDemo(false),
    #else
    _isDemo(true),
    #endif // CONFIG_DEMO
    _currentPatient(nullptr),
    root(nullptr),
    rootContext(nullptr),
    _chartscale(0),
    _defaultUnit("mg/l"),
    predictionspec(nullptr),
    _drugs(nullptr),
    _drugModelsForCurrentSubstance(nullptr),
    _activeSubstances(nullptr),
    _privateActiveSubstances(nullptr),
    _patients(nullptr),
    flowController(nullptr),
    adjustmentsView(nullptr),
    chartView(nullptr),
    covariatesView(nullptr),
    domainsView(nullptr),
    dosagesView(nullptr),
    drugsView(nullptr),
    drugVariatesView(nullptr),
    flowView(nullptr),
    measuresView(nullptr),
    patientsView(nullptr),
    studiesView(nullptr),
    targetsView(nullptr),
    validationView(nullptr),
    reportView(nullptr),
    _webchannel(nullptr),
    printer(),
    exportFileDialog()
{
    _drugs = CoreFactory::createEntity<DrugModelList>(ABSTRACTREPO, this);
    _drugModelsForCurrentSubstance = CoreFactory::createEntity<DrugModelList>(ABSTRACTREPO, this);
    _activeSubstances = CoreFactory::createEntity<LightActiveSubstanceList>(ABSTRACTREPO, this);
    _privateActiveSubstances = CoreFactory::createEntity<LightActiveSubstanceList>(ABSTRACTREPO, this);
    _patients = CoreFactory::createEntity<CorePatientList>(ABSTRACTREPO, this);

    _webchannel = new QQmlWebChannel(this);

    // It seems there is no need for this server

    /*
    QWebSocketServer* server = new QWebSocketServer(QStringLiteral("QWebChannel Standalone Example Server"), QWebSocketServer::NonSecureMode);
    if (!server->listen(QHostAddress::LocalHost, 12345)) {
        qFatal("Failed to open web socket server.");
    }

    // wrap WebSocket clients in QWebChannelAbstractTransport objects
    WebSocketClientWrapper* clientWrapper = new WebSocketClientWrapper(server);

    // setup the channel
    CONNECT(clientWrapper, &WebSocketClientWrapper::clientConnected,
                     _webchannel, &QWebChannel::connectTo);
//    QObject::CONNECT(&clientWrapper, clientConnected(WebSocketTransport*)), _webchannel, SLOT(connectTo(QWebChannelAbstractTransport*)));
*/


    flowController = new FlowController(this);

    calculationController = new MultiThreadedCalculationController(this);
//    calculationController = new ThreadedCalculationController(this);
//    calculationController = new LocalCalculationController(this);
//    calculationController = new TucuxiCalculationController(this);

    _chartDataController = new ChartDataController(this);
    _chartDataController->setCalculationController(calculationController);

    // The two following objects are invariants during the lifetime of the
    // interpretation controller
    // Actually that could be done differently in setNewInterpretation


    _graphInformationSelection = ezechiel::core::CoreFactory::createEntity<GraphInformationSelection>(REPO, this);


    measureTabController = new MeasureTabController(this);
    measureTabController->setMasterController(this);
    measureTabController->setMeasures(CoreFactory::createEntity<ezechiel::core::CoreMeasureList>(ABSTRACTREPO, this));

    dosageTabController = new DosageTabController(this);
    dosageTabController->setMasterController(this);
    dosageTabController->setDosages(CoreFactory::createEntity<ezechiel::core::DosageHistory>(ABSTRACTREPO, this));

    targetTabController = new TargetTabController(this);
    targetTabController->setMasterController(this);
    targetTabController->setTargets(CoreFactory::createEntity<ezechiel::core::TargetList>(ABSTRACTREPO, this));

    covariateTabController = new CovariateTabController(this);
    covariateTabController->setMasterController(this);
    covariateTabController->reset(CoreFactory::createEntity<ezechiel::core::DrugVariateList>(ABSTRACTREPO, this));
    covariateTabController->setPatientVariates(CoreFactory::createEntity<ezechiel::core::PatientVariateList>(ABSTRACTREPO, this));

    adjustmentTabController = new AdjustmentTabController(this);
    adjustmentTabController->setMasterController(this);
    adjustmentTabController->setAdjustments(CoreFactory::createEntity<ezechiel::core::DosageHistory>(ABSTRACTREPO, this));

    validationTabController = new ValidationTabController(this);

    drugTabController = new DrugTabController(this);
    drugTabController->setMasterController(this);


    // The following lines are necessary to avoid warnings from QML.
    // However these objects will be replaced by new ones when starting an interpretation
    // Let's create an empty interperation and assign it
    Interpretation *interpretation = CoreFactory::createEntity<Interpretation>(ADMINREPO, this);
    predictionspec = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionSpec>(REPO, this);
    setInterpretation(interpretation);
    predictionspec->setAnalysis(_interpretation->getDrugResponseAnalysis());
    chartData = _interpretation->getAnalysis()->getChartData();
    _chartDataController->chartData = chartData;
    _chartDataController->predictionspec = predictionspec;
    adjustmentTabController->setChartData(chartData);

//    _webchannel->registerObject("interpretation", _interpretation);
}

void InterpretationController::setNewInterpretation(Interpretation *interpretation, bool newInterpretation)
{
    if (_interpretation != nullptr) {
        deleteCurrentRequest();
        // TODO : To be fixed. If we want to delete it we have to take care of every call to clear() method
        // that deletes the objects of the list. For instance, patients list clear will delete the patient
        // of the interpretation, and so the interpretation will be corrupted.

       // _interpretation->deleteLater();
        //predictionspec->deleteLater();
    }

    _currentActiveSubstance = nullptr;
    _currentDrugModel = nullptr;
    _currentPatient = nullptr;

    predictionspec = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionSpec>(REPO, this);
    std::vector<double> _percv;
    _percv.insert(_percv.begin(), {5, 25, 75, 95});
    predictionspec->setPercentileList(_percv);
    predictionspec->setNbPoints(250);

    predictionspec->setAdjustmentDate(interpretation->getAdjustmentDate());
    predictionspec->setAdjustmentWithRestPeriod(interpretation->getAdjustmentWithRestPeriod());
    predictionspec->setAdjustmentWithLoadingDose(interpretation->getAdjustmentWithLoadingDose());


    setInterpretation(interpretation);


    // The prediction spec and the interpretation share the same drug response analysis
    predictionspec->setAnalysis(_interpretation->getDrugResponseAnalysis());
    chartData = _interpretation->getAnalysis()->getChartData();

    _chartDataController->chartData = chartData;
    _chartDataController->predictionspec = predictionspec;
    adjustmentTabController->setChartData(chartData);

    // Rebinds the interpretation
    rootContext->setContextProperty("interpretation", _interpretation);
    rootContext->setContextProperty("chartData", chartData);
    flowController->setInterpretation(interpretation);
    if (newInterpretation)
        flowController->startNewInterpretation();
    else
        flowController->loadInterpretation();

    ezechiel::core::DrugTreatment *newTreatment = interpretation->getDrugResponseAnalysis()->getTreatment();
    measureTabController->setMeasures(newTreatment->getMeasures());
    dosageTabController->setDosages(newTreatment->getDosages());
    targetTabController->setTargets(newTreatment->getTargets());
    covariateTabController->setPatientVariates(newTreatment->getCovariates());
    if (interpretation->getAdjustmentDateIsSet()) {
        adjustmentTabController->setInitialAdjustmentDateFromMaster(interpretation->getAdjustmentDate());
    }
    adjustmentTabController->setAdjustments(newTreatment->getAdjustments());
    if ((interpretation->getInterpretationType() == Interpretation::InterpretationType::FromRequest)
            || (!newInterpretation)) {
        covariateTabController->reset(nullptr);
        covariateTabController->updateAllActualValues();
    }

    flowController->evaluate();
    flowView->setProperty("currentIndex", QVariant::fromValue(0));

    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(drugsView,    "reset"));
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(patientsView, "reset"));
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(covariatesView, "reset"));
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(chartView, "reset"));


    if (interpretation->getInterpretationType() == Interpretation::InterpretationType::FromRequest)
    {
        InterpretationRequest *interpretationRequest = interpretation->getRequest();
        QString toDisplay;

        toDisplay = "<style> \
            .oldPM {color: #333333}\
            .spec {color: blue;}\
            .newPM {color: #333333}\
            table, td, th {border-style : solid; border-color: black;border-width:1px;} \
        </style>";

        toDisplay += "<table width=100% cellpadding=4 cellspacing=0>";
        toDisplay += "<tr><th>Clinical</th><th>Date</th><th>Text</th></tr>";
        for(int i = 0; i < interpretationRequest->getClinicals()->size(); i++) {
            Clinical *clinical = interpretationRequest->getClinicals()->at(i);
            toDisplay += "<tr><td>" + clinical->getName() + "</td><td>" + clinical->getDate().toString() + "</td><td>" + clinical->getValue() + "</td></tr>";
        }
        toDisplay += "</table>";
        setClinicalsHtml(toDisplay);

        setRawRequest(interpretationRequest->getSource());
    }

}


void InterpretationController::initAfterQmlLoading(QObject *root, QQmlContext *rootContext)
{
#ifdef CONFIG_GUITEST
    this->root = root;
#else
    this->root = dynamic_cast<QQuickItem *>(root);
#endif // CONFIG_GUITEST
    this->rootContext = rootContext;

    retrieveViewsFromQml();
    initViewConnexions();
}

void InterpretationController::bindModelsToRootContext(QQmlContext *rootContext)
{
    //Set global controllers accessible from every QML element

    rootContext->setContextProperty("interpretationController", this);
    rootContext->setContextProperty("chartData", chartData);
    rootContext->setContextProperty("interpretation", _interpretation);
    rootContext->setContextProperty("measureTabController", measureTabController);
    rootContext->setContextProperty("dosageTabController", dosageTabController);
    rootContext->setContextProperty("targetTabController", targetTabController);
    rootContext->setContextProperty("covariateTabController", covariateTabController);
    rootContext->setContextProperty("adjustmentTabController", adjustmentTabController);
    rootContext->setContextProperty("validationTabController", validationTabController);
    rootContext->setContextProperty("drugTabController", drugTabController);
	rootContext->setContextProperty("graphInformationSelection", _graphInformationSelection);
    rootContext->setContextProperty("appMode", AppMode::getInstance());

}


void InterpretationController::retrieveViewsFromQml()
{
    adjustmentsView = root->findChild<QObject *>("adjustmentsView");
    Q_ASSERT(adjustmentsView);

    chartView = root->findChild<QObject *>("chartView");
    Q_ASSERT(chartView);

    covariatesView = root->findChild<QObject *>("covariatesView");
    Q_ASSERT(covariatesView);

//    domainsView = root->findChild<QObject *>("domainsView");
//    Q_ASSERT(domainsView);

    dosagesView = root->findChild<QObject *>("dosagesView");
    Q_ASSERT(dosagesView);

    drugsView = root->findChild<QObject *>("drugsView");
    Q_ASSERT(drugsView);

    drugVariatesView = root->findChild<QObject *>("drugVariatesView");
    Q_ASSERT(drugVariatesView);

    flowView = root->findChild<QObject *>("flowView");
    Q_ASSERT(flowView);
    flowController->setFlowView(flowView);


    measuresView = root->findChild<QObject *>("measuresView");
    Q_ASSERT(measuresView);

    patientsView = root->findChild<QObject *>("patientsView");
    Q_ASSERT(patientsView);

//    studiesView = root->findChild<QObject *>("studiesView");
//    Q_ASSERT(studiesView);

    targetsView = root->findChild<QObject *>("targetsView");
    Q_ASSERT(targetsView);

    reportView = root->findChild<QObject *>("reportView");
    Q_ASSERT(reportView);

    webengineView = reportView->property("daweb").value<QObject*>();

//    channelView = root->findChild<QObject*>("channelView");
//    Q_ASSERT(channelView);

    validationView = root->findChild<QObject *>("validationView");
    Q_ASSERT(validationView);
}

void InterpretationController::initViewConnexions()
{
    CONNECT(flowView, SIGNAL(changedTab(int)), this, SLOT(currentTabChanged(int)));
    CONNECT(flowView, SIGNAL(validateTab(int)), this, SLOT(validateTab(int)));
    CONNECT(flowView, SIGNAL(toggleValidation(int)), this, SLOT(toggleValidation(int)));
    CONNECT(this, SIGNAL(chartscaleChanged(double)), this, SLOT(chartScaleChanged()));

    //Selections changes
    CONNECT(drugsView,       SIGNAL(switchDrugModel(int)), this, SLOT(switchDrugModel(int)));
    CONNECT(drugsView,       SIGNAL(switchActiveSubstance(int)), this, SLOT(switchActiveSubstance(int)));

    CONNECT(calculationController, SIGNAL(disengage()), chartView, SLOT(wait()));
    CONNECT(calculationController, SIGNAL(engage()), chartView, SLOT(resume()));

//    CONNECT(adjustmentTabController, SIGNAL(adjustmentDateChanged(QDateTime)), adjustmentsView, SLOT(setAdjustmentDate(QDateTime)));
}

QObject *InterpretationController::view() const
{
    return flowView;
}

bool InterpretationController::isFlowRequest() const
{
    return _interpretation->getInterpretationType() == Interpretation::InterpretationType::FromRequest;
}

bool InterpretationController::acceptAndQuit()
{
    // Check if we can loose the work done
    if (QMessageBox::warning(QApplication::activeWindow(),
                             tr("Be careful"),
                             tr("Are you sure you want to leave? You will loose your current treatment analysis"),
                             QMessageBox::Yes,
                             QMessageBox::No) == QMessageBox::No)
        return false;

    // Stop every calculation
    calculationController->abortAll();

    _webchannel->deregisterObject(_interpretation);

    // TODO:  Clean the interpretation
    // Currently deleting the request has an issue. To be checked on Windows.
    //deleteCurrentRequest();

    return true;

}


// This function should be called to delete the current request.
// It deletes all objects related to the request and leaves the flow in its initial state
void InterpretationController::deleteCurrentRequest()
{

    if (chartData)
        chartData->initialize();

    measureTabController->setMeasures(CoreFactory::createEntity<ezechiel::core::CoreMeasureList>(ABSTRACTREPO, this));
    dosageTabController->setDosages(CoreFactory::createEntity<ezechiel::core::DosageHistory>(ABSTRACTREPO, this));
    targetTabController->setTargets(CoreFactory::createEntity<ezechiel::core::TargetList>(ABSTRACTREPO, this));
    covariateTabController->reset(CoreFactory::createEntity<ezechiel::core::DrugVariateList>(ABSTRACTREPO, this));
    covariateTabController->setPatientVariates(CoreFactory::createEntity<ezechiel::core::PatientVariateList>(ABSTRACTREPO, this));
    adjustmentTabController->setAdjustments(CoreFactory::createEntity<ezechiel::core::DosageHistory>(ABSTRACTREPO, this));
    drugTabController->setDrugHtmlDescription(tr("No drug selected"));

}

bool activeSubstanceComparator(const ezechiel::core::LightActiveSubstance *a1,
                               const ezechiel::core::LightActiveSubstance *a2)
{
    return a1->getName()->value() < a2->getName()->value();
}

void InterpretationController::populateDrugModels()
{
    static bool populated = false;
    if (!populated) {

        if (APPUTILSREPO->getDrugsList(_drugs->getList()).error != ezechiel::core::NoError)
        EXLOG(QtFatalMsg, ezechiel::guiutils::MODELIOERROR, "Drugs list not retrieved.");

        for (int i = 0; i < _drugs->size(); ++i) {
            ezechiel::core::DrugModel *model = _drugs->at(i);
            model->setParent(_drugs);
        }
        _drugs->update();

        populated = true;
    }
}

void InterpretationController::populateMultipleActiveSubstance()
{
    populateDrugModels();

    // Be careful here. We are really deleting the objects. That is well here with LighActiveSubstances, but the real
    // active substances of the DrugModels should not be deleted
    // Actually I don't know why a simple clear() does not work sometimes...
    _activeSubstances->clearWithoutDeletion();
    _privateActiveSubstances->clearWithoutDeletion();


    // This is not a request, so we add all the active substances
    for (int i = 0; i < _drugs->size(); ++i) {
        // We first check if the an active substance with the same Id is already in the list
        bool canAdd = true;
        for(int j = 0; j < _activeSubstances->size(); j++) {
            if (_activeSubstances->at(j)->getSubstanceId() == _drugs->at(i)->getActiveSubstance()->getSubstanceId())
                canAdd = false;
        }
        // Then add
        if (canAdd) {
            LightActiveSubstance *light = CoreFactory::createEntity<LightActiveSubstance>(ABSTRACTREPO, _activeSubstances);
            light->setName(_drugs->at(i)->getActiveSubstance()->getName());
            light->setSubstanceId(_drugs->at(i)->getActiveSubstance()->getSubstanceId());
            _activeSubstances->append(light);
            _privateActiveSubstances->append(light);
        }
    }
    _activeSubstances->sort(activeSubstanceComparator);
    _privateActiveSubstances->sort(activeSubstanceComparator);
    _activeSubstances->update();
}

void InterpretationController::populateSingleActiveSubstance(const QString &activeSubstanceId)
{
    populateDrugModels();

    // Be careful here. We are really deleting the objects. That is well here with LighActiveSubstances, but the real
    // active substances of the DrugModels should not be deleted
    // Actually I don't know why a simple clear() does not work sometimes...
    _activeSubstances->clearWithoutDeletion();
    _privateActiveSubstances->clearWithoutDeletion();

    // We just add the corresponding active substance
    for (int i = 0; i < _drugs->size(); ++i) {
        if (activeSubstanceId == _drugs->at(i)->getActiveSubstance()->getSubstanceId()) {
            LightActiveSubstance *light = CoreFactory::createEntity<LightActiveSubstance>(ABSTRACTREPO, nullptr);
            light->setName(_drugs->at(i)->getActiveSubstance()->getName());
            light->setSubstanceId(_drugs->at(i)->getActiveSubstance()->getSubstanceId());
            _activeSubstances->append(light);
            LightActiveSubstance *light2 = CoreFactory::createEntity<LightActiveSubstance>(ABSTRACTREPO, nullptr);
            light2->setName(_drugs->at(i)->getActiveSubstance()->getName());
            light2->setSubstanceId(_drugs->at(i)->getActiveSubstance()->getSubstanceId());
            _privateActiveSubstances->append(light2);
            break;
        }
    }

    _activeSubstances->update();
}

void InterpretationController::startNewPatient()
{

    // Inialize the new interpretation
    Interpretation *interpretation = CoreFactory::createEntity<Interpretation>(ABSTRACTREPO, this);

    interpretation->setStartInterpretationTime(QDateTime::currentDateTime());

    // Create a new treatment and assigns it to both objects
    ezechiel::core::DrugTreatment *newTreatment = CoreFactory::createEntity<DrugTreatment>(ABSTRACTREPO, _interpretation->getDrugResponseAnalysis());

    interpretation->getDrugResponseAnalysis()->setTreatment(newTreatment);
    interpretation->getRequest()->setTreatment(newTreatment);
    interpretation->setInterpretationType(Interpretation::InterpretationType::NoRequest);


    populateMultipleActiveSubstance();
    setRawRequest("");
    FakePatientsCreator().createFakePatients(_patients);


    // Set it to the controller
    setNewInterpretation(interpretation, true);

    _interpretation->getDrugResponseAnalysis()->getTreatment()->setActiveSubstanceId("");
    _interpretation->getDrugResponseAnalysis()->setDrugModel(nullptr);
    _interpretation->getDrugResponseAnalysis()->getTreatment()->getMeasures()->clear();
    _interpretation->getDrugResponseAnalysis()->getTreatment()->getDosages()->clear();
    _interpretation->getDrugResponseAnalysis()->getTreatment()->getTargets()->clear();
    _interpretation->getDrugResponseAnalysis()->getTreatment()->getAdjustments()->clear();
    _interpretation->getAnalysis()->getChartData()->setAdjPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
    _interpretation->getDrugResponseAnalysis()->getTreatment()->setPatient(_patients->at(0));
    flowController->patientChanged(0);
    _interpretation->getAnalysis()->getChartData()->getRevPred()->getAdjustments()->clear();
    flowController->evaluate();
    drugsView->setProperty("currentIndex", QVariant::fromValue(-1));
    setCurrentPatient(dynamic_cast<Patient*>(_patients->at(0)));
}

void InterpretationController::startInterpretationRequest(InterpretationRequest *interpretationRequest)
{
    // First build the interpretation
    Interpretation *interpretation = CoreFactory::createEntity<Interpretation>(ABSTRACTREPO, this);

    interpretation->setStartInterpretationTime(QDateTime::currentDateTime());

    interpretation->setInterpretationType(Interpretation::InterpretationType::FromRequest);
    DrugTreatment* dt = interpretationRequest->getTreatment();
    interpretation->getDrugResponseAnalysis()->setTreatment(dt);
    dt->setParent(_interpretation->getAnalysis());
    interpretation->setRequest(interpretationRequest);
/*
    foreach (CoreMeasure* cm, interpretation->getDrugResponseAnalysis()->getTreatment()->getMeasures()->getList()) {
        cm->getConcentration()->setUnitstring(_defaultUnit);
    }
    foreach (Target* tar, interpretation->getDrugResponseAnalysis()->getTreatment()->getTargets()->getList()) {
        tar->getCbest()->setUnitstring(_defaultUnit);
        tar->getCmax()->setUnitstring(_defaultUnit);
        tar->getCmin()->setUnitstring(_defaultUnit);
    }
*/
    Dosage *lastDosage = interpretationRequest->getTreatment()->getDosages()->lastDosage();
    if (lastDosage != nullptr) {
        adjustmentTabController->setAdjustmentDateFromMaster(lastDosage->getEndTime().addMSecs(lastDosage->getInterval().mSecs()));
    }
    else {
        adjustmentTabController->setAdjustmentDateFromMaster(QDateTime::currentDateTime());
    }

    setNewInterpretation(interpretation, true);


    populateSingleActiveSubstance(dt->getActiveSubstanceId());

    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(patientsView, "reset"));

    _patients->clear();
    _patients->append(_interpretation->getDrugResponseAnalysis()->getTreatment()->getPatient());
    setCurrentPatient(dynamic_cast<Patient*>(_patients->at(0)));

    updateInterpretationRange(EViewRangeUpdateContext::Start);
    flowController->evaluate();
    flowView->setProperty("index", QVariant::fromValue(0));


    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(drugsView,    "reset"));

//    drugsView->property("drugListView").value<QObject*>()->setProperty("currentIndex", QVariant::fromValue(0));

    // The following lines are valid if we display all the active substances
    //find the treatment drug in the list of drugs for the gui. Set the index in the gui
//    for (int i = 0; i < _drugs->size(); ++i) {
        // TODO: Use activeSubstances instead
//        if (_drugs->at(i)->getName() == dt->getDrug()->getName())
//        drugsView->property("drugListView").value<QObject*>()->setProperty("currentIndex", QVariant::fromValue(i));
//    }
    _interpretation->getDrugResponseAnalysis()->getTreatment()->getDosages()->update();
    _interpretation->getDrugResponseAnalysis()->getTreatment()->getCovariates()->update();
    _interpretation->getDrugResponseAnalysis()->getTreatment()->getMeasures()->update();


    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(covariatesView, "reset"));
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(chartView, "reset"));


 //   CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(patientsView, "clearSelection"));

    flowView->setProperty("currentIndex", QVariant::fromValue(0));

    // No choice about the active substance. Select the only one
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(drugsView, "setCurrentActiveSubstance",
                              Q_ARG(QVariant, QVariant::fromValue(0))));

    setCurrentActiveSubstance(_activeSubstances->at(0));

    // Do selection and checks, there is no choice about the active substance in pending request mode


    ModelVsTreatmentCompatibilityChecker checker;

    _drugModelsForCurrentSubstance->clearWithoutDeletion();

    bool foundDrugModel = false;

    QList<Admin::Route> defaultRoutes;
    QList<Admin*> defaultAdmins;

    for (int i = 0; i < _drugs->size(); ++i) {
        if (_drugs->at(i)->getActiveSubstance()->getSubstanceId().compare(dt->getActiveSubstanceId()) == 0) {
            if (_drugs->at(i)->getDrugModelId() != "ch.heig-vd.ezechiel.unknown") {
                // Another check in pending request mode, as the dosage history is known before the model is choosen
                if (checker.check(_drugs->at(i), _interpretation->getDrugResponseAnalysis()->getTreatment())){
                    defaultRoutes.append(_drugs->at(i)->getAdme()->getDefaultIntake()->getRoute());
                    defaultAdmins.append(_drugs->at(i)->getAdme()->getDefaultIntake());
                    _drugModelsForCurrentSubstance->append(_drugs->at(i));
                    foundDrugModel = true;
                }
            }
        }
    }

    if (!foundDrugModel) {

        QMessageBox::critical(nullptr, tr("Error"), tr("There is no drug model compatible with the current interpretation request. Tucuxi detected this active substance Id: %1. Maybe the elimination is not compatible.").arg(dt->getActiveSubstanceId()));
        return;
    }

    if (defaultRoutes.size() != 0) {
        if (defaultRoutes.size() != defaultRoutes.count(defaultRoutes[0])) {
            // There are at least two default routes
            // We do not manage this case
        }
        else {
            for (auto dosage : _interpretation->getDrugResponseAnalysis()->getTreatment()->getDosages()->getList()) {
                if (dosage->getRoute()->getRoute() == Admin::Route::DEFAULT) {
                    Admin *admin = ezechiel::core::CoreFactory::createEntity<Admin>(APPUTILSREPO, nullptr);
                    admin->setRoute(defaultRoutes[0]);
                    admin->setDescription(defaultAdmins[0]->getDescription());
                    dosage->setRoute(admin);
                }
            }
        }
    }

    emit drugModelsChanged(_drugModelsForCurrentSubstance);


    if (_drugModelsForCurrentSubstance->size() == 1) {

        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(drugsView, "setCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(0))));
        setCurrentDrugModel(_drugModelsForCurrentSubstance->at(0));
        _interpretation->getDrugResponseAnalysis()->setDrugModel(_drugModelsForCurrentSubstance->at(0));


        TargetList* targets = _interpretation->getDrugResponseAnalysis()->getTreatment()->getTargets();
        TargetList* drugTargets            = _drugModelsForCurrentSubstance->at(0)->getTargets();

        targets->clear();
        for (int i = 0; i < drugTargets->size(); ++i) {
            ezechiel::core::Target* target = ezechiel::core::CoreFactory::createEntity<Target>(APPUTILSREPO, targets);
            target->copyFrom(drugTargets->at(i));
            targets->append(target);
        }


        flowController->drugChanged(0);
        flowController->evaluate();


        covariateTabController->reset(_drugModelsForCurrentSubstance->at(0)->getCovariates());
        updateSexAndAgeCovariates();

        drugTabController->setDrugModelInfo(_drugModelsForCurrentSubstance->at(0));


        lastDosage = interpretationRequest->getTreatment()->getDosages()->lastDosage();
        if (lastDosage != nullptr) {
            adjustmentTabController->setAdjustmentDateFromMaster(lastDosage->getEndTime().addMSecs(lastDosage->getInterval().mSecs()));
        }
        else {
            adjustmentTabController->setAdjustmentDateFromMaster(QDateTime::currentDateTime());
        }

        // Moved it after switchActiveSubstance, as this function will set the DrugModel
        adjustmentTabController->onDosageUpdated();

        _chartDataController->start();
    }
    else {
        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(drugsView, "setCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(-1))));
        drugTabController->setDrugInfo(findRealActiveSubstance( _privateActiveSubstances->at(0)));
        //drugTabController->setDrugInfo(findRealActiveSubstance( _activeSubstances->at(0)));
    }


}

void InterpretationController::loadInterpretation(Interpretation *interpretation)
{

    _chartDataController->dosageUpdated(shouldPercentilesBeComputed);
    // No need to prepare the interpretation, as we simply load it



    DrugTreatment *dt = interpretation->getDrugResponseAnalysis()->getTreatment();
    InterpretationRequest *interpretationRequest = interpretation->getRequest();
    if (interpretationRequest != nullptr)
        interpretationRequest->setTreatment(dt);

    setNewInterpretation(interpretation, false);

    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(patientsView, "reset"));

//    _patients->clear();
    _patients->clearWithoutDeletion();

    // Here we only load the specific patient.
    _patients->append(_interpretation->getDrugResponseAnalysis()->getTreatment()->getPatient());


    setCurrentPatient(dynamic_cast<Patient*>(_patients->at(0)));

    // We populate the drug list depending on the type of interpretation we are loading
    if (interpretation->getInterpretationType() == Interpretation::InterpretationType::FromRequest)
        populateSingleActiveSubstance(dt->getActiveSubstanceId());
    else
        populateMultipleActiveSubstance();



    updateInterpretationRange(EViewRangeUpdateContext::Start);
    flowController->evaluate();
    flowView->setProperty("index", QVariant::fromValue(0));

    _interpretation->getDrugResponseAnalysis()->getTreatment()->getDosages()->update();
    _interpretation->getDrugResponseAnalysis()->getTreatment()->getCovariates()->update();
    _interpretation->getDrugResponseAnalysis()->getTreatment()->getMeasures()->update();


    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(covariatesView, "reset"));
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(chartView, "reset"));

    flowView->setProperty("currentIndex", QVariant::fromValue(0));

    // Try to set the active substance and the drugmodel directly here
    {

        ezechiel::core::DrugModel *drugModel = _interpretation->getDrugResponseAnalysis()->getDrugModel();
        ezechiel::core::ActiveSubstance *activeSubstance = drugModel->getActiveSubstance();

        ezechiel::core::LightActiveSubstance *lightActiveSubstance = nullptr;

        int activeSubstanceIndex = 0;
        // Find the LightActiveSubstance
        foreach (ezechiel::core::LightActiveSubstance *substance, _activeSubstances->getList()) {
            if (substance->getSubstanceId() == activeSubstance->getSubstanceId()) {
                lightActiveSubstance = substance;
                break;
            }
            activeSubstanceIndex ++;
        }
        Q_ASSERT(lightActiveSubstance != nullptr);
        setCurrentActiveSubstance(lightActiveSubstance);

        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(drugsView, "setCurrentActiveSubstance",
                                  Q_ARG(QVariant, QVariant::fromValue(activeSubstanceIndex))));

        // Do selection and checks, there is no choice about the active substance in pending request mode

        ModelVsTreatmentCompatibilityChecker checker;

        _drugModelsForCurrentSubstance->clearWithoutDeletion();

        int drugModelIndex = 0;
        int currentIndex = 0;
        for (int i = 0; i < _drugs->size(); ++i) {
            if (_drugs->at(i)->getActiveSubstance()->getSubstanceId().compare(activeSubstance->getSubstanceId()) == 0) {
                if (_drugs->at(i)->getDrugModelId() != "ch.heig-vd.ezechiel.unknown") {
                    // Another check in pending request mode, as the dosage history is known before the model is choosen
                    if (checker.check(_drugs->at(i), _interpretation->getDrugResponseAnalysis()->getTreatment())) {
                        _drugModelsForCurrentSubstance->append(_drugs->at(i));
                        if (_drugs->at(i)->getDrugModelId() == drugModel->getDrugModelId()) {
                            drugModelIndex = currentIndex;
                        }
                        currentIndex ++;
                    }
                }
            }
        }

        emit drugModelsChanged(_drugModelsForCurrentSubstance);

        setCurrentDrugModel(drugModel);

        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(drugsView, "setCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(drugModelIndex))));

        drugTabController->setDrugModelInfo(drugModel);

        {
                covariateTabController->reset(drugModel->getCovariates());

                // TODO: To be checked: Do we really need that?
       //         dosagesView->setProperty("routes", QVariant::fromValue(drug->getAdme()->getIntakes()));
       //         adjustmentsView->setProperty("routes", QVariant::fromValue(drug->getAdme()->getIntakes()));

       //         adjustmentTabController->setAdjustmentDateFromMaster(QDateTime::currentDateTime());
        }

        flowController->drugChanged(currentIndex);
        flowController->evaluate();

    }


    _chartDataController->start();
}


InterpretationController::~InterpretationController()
{
    if (calculationController != nullptr)
        delete calculationController;
}


void InterpretationController::setReportTabShow(QVariant vecin) {
    QObject* canvas = chartView->findChild<QObject*>("canvas");
    canvas->setProperty("reportshow", vecin);
    canvas->setProperty("show", vecin);
}

// TODO : This method should not be here
void InterpretationController::resetReqState() {
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(root, "resetReqState"));
}

QVariant InterpretationController::getGraphDrugData() {
    if (!_interpretation) {return QVariant::fromValue(false);}
    if (!_interpretation->getDrugResponseAnalysis()) {return QVariant::fromValue(false);}
    ezechiel::core::DrugModel *drug = _interpretation->getDrugResponseAnalysis()->getDrugModel();
    if (drug) {
        QString desc;
        desc += "Drug name: " + drug->getName()->value() + "|| ";
        desc += "ATC: " + drug->getActiveSubstance()->getAtc().at(0) + "|| ";
        if (!drug->getDescriptionsTranslationMap()->isEmpty())
            desc += "Description: " + drug->getDescriptions() + "||";
        return QVariant::fromValue(desc);
        _chartDataController->dosageUpdated(shouldPercentilesBeComputed);
    }
    return QVariant::fromValue(false);
}


bool InterpretationController::exportReport(Report *report)
{
    Q_UNUSED(report);
//    ReportExportToolFactory factory;

//    // ToDo-MVC ///////////////////////////////////////////////////////////////
//    // Replace the exportFileDialog and QMessageBox with QML components
//    // ////////////////////////////////////////////////////////////////////////

//    EXLOG(QtDebugMsg, NOEZERROR, "Exporting report");
//    if (exportFileDialog == 0) {
//        EXLOG(QtDebugMsg, NOEZERROR, "Making file dialog");
//        exportFileDialog.reset(new PersistentFileDialog);
//        exportFileDialog->setAcceptMode(QFileDialog::AcceptSave);
//        exportFileDialog->setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
//        exportFileDialog->setWindowTitle(tr("Export Report"));
//        exportFileDialog->setNameFilters(factory.supportedFilesTypes());
//    }
//    if (exportFileDialog->exec() == QFileDialog::Rejected)
//        return false;

//    QStringList selectedFiles = exportFileDialog->selectedFiles();
//    if (selectedFiles.size() != 1) {
//        QMessageBox::warning(0, tr("Invalid file"), tr("Please select the file format and a name for your file."));
//        return false;
//    }

//    QScopedPointer<ReportExportTool> exportTool(factory.createFromFileType(exportFileDialog->selectedNameFilter()));
//    if (exportTool.isNull()) {
//        QMessageBox::warning(0, tr("Invalid file type"), tr("The report cannot be exported to the specified file format '%1'.").arg(exportFileDialog->selectedNameFilter()));
//        return false;
//    }

//    QString selectedFile = selectedFiles.first();
//    if (!selectedFile.endsWith(exportTool->extension()))
//        selectedFile.append(exportTool->extension());

//    exportTool->setFilePath(selectedFile);
//    exportTool->setReport(report);
//    if (!exportTool->exportReport()) {
//        QMessageBox::warning(0, tr("Exportation failed"), tr("An error occured while exporting the report to the file '%1'. Please make sure you have the appropriate rights and try again.").arg(selectedFile));
//        return false;
//    }
    return true;
}

//TODO: doesnt work! Not implicitly convertable to QImage. image is invalid here
void InterpretationController::publishReport(QImage image) {
    Q_UNUSED(image);
//    QScopedPointer<ReportBuilder> builder(new NewReportBuilder());

//    EXLOG(QtMsgType::QtDebugMsg, NOEZERROR, "received image from chart")
//    QList<CoreMeasure *> measures = _interpretation->getDrugResponseAnalysis()->getTreatment()->getMeasures()->getList();
//    if (measures.empty()) {
//       EXLOG(QtMsgType::QtDebugMsg, NOEZERROR, "No measures so no report")
//       return;
//    }

//    Measure * measure = static_cast<Measure*>(measures.last());
//    InterpretationBuilderModule ibuilda(interpretation, predictionspec, measure, this);
//    builder->setModule(ibuilda.metaObject()->className(), &ibuilda);
//    QScopedPointer<Report> report(builder->build());
//    report->data()->setImage("chart.image", image);
//    exportReport(report.data());
}

bool InterpretationController::printReport(Report *report)
{
    Q_UNUSED(report);
    Q_UNIMPLEMENTED();

    // ToDo-Report ////////////////////////////////////////////////////////////
    // Print the report (cf. reportpanel.cpp on master)
    // ////////////////////////////////////////////////////////////////////////

    return false;
}

bool InterpretationController::sendReport(Report *report)
{
    Q_UNUSED(report);
    Q_UNIMPLEMENTED();

    // ToDo-Rest //////////////////////////////////////////////////////////////
    // Send the notification message to mirth along with the report
    // ////////////////////////////////////////////////////////////////////////

    return false;
}





void InterpretationController::currentDomainChanged(int index)
{
    // ToDo-MVC ///////////////////////////////////////////////////////////////
    // Update the study
    // ////////////////////////////////////////////////////////////////////////

    Q_UNUSED(index);
}

QVariantHash InterpretationController::hashToJS(QVariant qlobj) {
    Q_UNUSED(qlobj);
   QVariantHash ret;
//   QObject* qobj = qlobj.value<QObject*>();
//   QList<QObject*> qobjs = qobj->property("objlist").value<QList<QObject*>>();
   for (int i = 0; i < _drugs->size(); ++i) {
       QString lala = QString("drugs%1").arg(i);
       QObject * laobj = _drugs->objat(i);
       ret.insert(lala, QVariant::fromValue(laobj));
   }
   return ret;
}

void InterpretationController::adjustmentDateUpdated()
{
    if (updateInterpretationRange(EViewRangeUpdateContext::AdjustmentDate)) {
        flowController->evaluate();

        _interpretation->getDrugResponseAnalysis()->getTreatment()->getAdjustments()->clear();
        _interpretation->getAnalysis()->getChartData()->setRevPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
        _interpretation->getAnalysis()->getChartData()->setAdjPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));

        _chartDataController->adjustmentSettingsUpdated();
    }
}

void InterpretationController::adjustmentSettingsUpdated()
{
    if (updateInterpretationRange(EViewRangeUpdateContext::AdjustmentDate)) {
        flowController->evaluate();

        _interpretation->getDrugResponseAnalysis()->getTreatment()->getAdjustments()->clear();
        _interpretation->getAnalysis()->getChartData()->setRevPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
        _interpretation->getAnalysis()->getChartData()->setAdjPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));

        _chartDataController->adjustmentSettingsUpdated();
    }
}

void InterpretationController::adjustmentUpdated()
{
    updateInterpretationRange(EViewRangeUpdateContext::Adjustment);
    flowController->evaluate();
    _chartDataController->adjustmentUpdated();
}

void InterpretationController::dosageUpdated(bool clearAdjustment)
{
    updateInterpretationRange(EViewRangeUpdateContext::Dosage);
    flowController->evaluate();
    if (clearAdjustment) {
        if (_interpretation->getDrugResponseAnalysis()->getTreatment()->getDosages()->size() == 0) {
            _interpretation->getDrugResponseAnalysis()->getTreatment()->getMeasures()->clear();
            _chartDataController->measureUpdated(shouldPercentilesBeComputed);
        }
        clearAdjustments();
        adjustmentTabController->onDosageUpdated();
    }
    _chartDataController->dosageUpdated(shouldPercentilesBeComputed);
}

void InterpretationController::chartScaleChanged()
{
    dosageUpdated(false);
}


void InterpretationController::evaluateFlow()
{
    flowController->evaluate();
}

void InterpretationController::clearAdjustments()
{
    _interpretation->getDrugResponseAnalysis()->getTreatment()->getAdjustments()->clear();
    _interpretation->getAnalysis()->getChartData()->setRevPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
    _interpretation->getAnalysis()->getChartData()->setAdjPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
    adjustmentTabController->reset();
}

void InterpretationController::measureUpdated()
{
    clearAdjustments();
    flowController->invalidateInterpretation();
    flowController->evaluate();
    _chartDataController->measureUpdated(shouldPercentilesBeComputed);
}

void InterpretationController::targetUpdated()
{
    clearAdjustments();
    flowController->invalidateInterpretation();
    flowController->evaluate();
    _chartDataController->targetUpdated();
}

void InterpretationController::covariateUpdated()
{
    clearAdjustments();
    flowController->invalidateInterpretation();
    flowController->evaluate();
    _chartDataController->covariateUpdated(shouldPercentilesBeComputed);
}


ezechiel::core::ActiveSubstance *InterpretationController::findRealActiveSubstance(const LightActiveSubstance *activeSubstance)
{
    for (int i = 0; i < _drugs->size(); ++i) {
        if (_drugs->at(i)->getActiveSubstance()->getSubstanceId().compare(activeSubstance->getSubstanceId()) == 0) {
                return _drugs->at(i)->getActiveSubstance();
        }
    }
    // That should never happen, as the active substance argument should be valid
    Q_ASSERT(false);
    return nullptr;
}

void InterpretationController::switchActiveSubstance(int index)
{
    ezechiel::core::LightActiveSubstance* activeSubstance = index == -1 ? nullptr : _activeSubstances->at(index);

    // If the new active substance is actually the current one, then no need to update anything
    if (_currentActiveSubstance == activeSubstance)
        return;

    if (activeSubstance == nullptr) {
        return;
    }

    // We are to this point only for an interpretation without a request, as with a request there is
    // a unique active substance


    setCurrentActiveSubstance(activeSubstance);
//    _currentActiveSubstance = activeSubstance;

    QString newId = activeSubstance->getSubstanceId();


    if (_interpretation->getInterpretationType() == Interpretation::InterpretationType::NoRequest)
    {
        _interpretation->getDrugResponseAnalysis()->getTreatment()->setActiveSubstanceId(newId);
        _drugModelsForCurrentSubstance->clearWithoutDeletion();

        for (int i = 0; i < _drugs->size(); ++i) {
            if (_drugs->at(i)->getActiveSubstance()->getSubstanceId().compare(newId) == 0) {
                    _drugModelsForCurrentSubstance->append(_drugs->at(i));
            }
        }

        emit drugModelsChanged(_drugModelsForCurrentSubstance);

        if ((_drugModelsForCurrentSubstance->size() == 1)) {
            // If a single drug model available, then automatically select it
            switchDrugModel(0);
        }
        else {
            CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(drugsView, "clearDrugModelSelection"))
                    _interpretation->getDrugResponseAnalysis()->setDrugModel(nullptr);
                    _interpretation->getDrugResponseAnalysis()->getTreatment()->getMeasures()->clear();
                    _interpretation->getDrugResponseAnalysis()->getTreatment()->getDosages()->clear();
                    _interpretation->getDrugResponseAnalysis()->getTreatment()->getTargets()->clear();
                    _interpretation->getDrugResponseAnalysis()->getTreatment()->getAdjustments()->clear();
                    _interpretation->getAnalysis()->getChartData()->setAdjPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
                    _interpretation->getDrugResponseAnalysis()->getTreatment()->setPatient(_patients->at(0));
                    _interpretation->getAnalysis()->getChartData()->getRevPred()->getAdjustments()->clear();

                    flowController->evaluate();

                    // Just display the info about the active substance
            drugTabController->setDrugInfo(findRealActiveSubstance(activeSubstance));
        }

    }

}

//void InterpretationController::currentDrugChanged(int index)
void InterpretationController::switchDrugModel(int index)
{
    ezechiel::core::DrugModel* drug = ((index == -1) || (_drugModelsForCurrentSubstance == nullptr)) ? nullptr : _drugModelsForCurrentSubstance->at(index);//(drugListProxyModel->mapToSource(drugListProxyModel->index(index, 0)).row());


    // If the new active substance is actually the current one, then no need to update anything
    if (_currentDrugModel == drug)
        return;
    // TODO I don't know why but the previous test could fail even with identical models.
    // We have to figure out why there are two pointers...
    if (_currentDrugModel != nullptr) {
        if (_currentDrugModel->getDrugModelId() == drug->getDrugModelId()) {
            return;
        }
    }

    if (drug == nullptr) {
        return;
    }

    setCurrentDrugModel(drug);

    drugTabController->setDrugModelInfo(drug);

    if (_interpretation->getInterpretationType() == Interpretation::InterpretationType::NoRequest) {
        // TODO: This call makes trouble with the REST flow.
        DrugTreatment* treatment = _interpretation->getDrugResponseAnalysis()->getTreatment();
        _interpretation->getDrugResponseAnalysis()->setDrugModel(drug);
        if (_drugs->size() != 1) {
            treatment->getMeasures()->clear();
            treatment->getDosages()->clear();
            treatment->getCovariates()->clear();
            treatment->getAdjustments()->clear();
            _interpretation->getAnalysis()->getChartData()->setAdjPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
            _interpretation->getDrugResponseAnalysis()->setDrugModel(drug);

            /**************************************
             * TODO : This is wrong. It should act on the treatment targets by copying them
             **************************************/
            dosageTabController->reset();
            covariateTabController->reset(drug->getCovariates());
            adjustmentTabController->reset();

            updateSexAndAgeCovariates();

            _interpretation->getAnalysis()->getChartData()->getRevPred()->getAdjustments()->clear();
            _interpretation->getAnalysis()->getChartData()->getPopPred()->getPredictive()->getPredictionData()->getPoints()->clear();
            _interpretation->getAnalysis()->getChartData()->getPopPred()->getPredictive()->getPercentileDataList()->clear();
            _interpretation->getAnalysis()->getChartData()->getAprPred()->getPredictive()->getPredictionData()->getPoints()->clear();
            _interpretation->getAnalysis()->getChartData()->getAprPred()->getPredictive()->getPercentileDataList()->clear();
            _interpretation->getAnalysis()->getChartData()->getApoPred()->getPredictive()->getPredictionData()->getPoints()->clear();
            _interpretation->getAnalysis()->getChartData()->getApoPred()->getPredictive()->getPercentileDataList()->clear();
    //        if (interpretation->getAnalysis()->getChartData()->getRevPred()) {
    //            interpretation->getAnalysis()->getChartData()->getRevPred()->deleteLater();
    //        }
    //        if (interpretation->getAnalysis()->getChartData()->getPopPred()) {
    //            interpretation->getAnalysis()->getChartData()->getPopPred()->deleteLater();
    //        }
    //        if (interpretation->getAnalysis()->getChartData()->getApoPred()) {
    //            interpretation->getAnalysis()->getChartData()->getApoPred()->deleteLater();
    //        }
    //        if (interpretation->getAnalysis()->getChartData()->getAprPred()) {
    //            interpretation->getAnalysis()->getChartData()->getAprPred()->deleteLater();
    //        }
        }

        // ToDo-MVC ///////////////////////////////////////////////////////////////
        // Refactor to get a list of domains for the current drug, and a list of
        // studies for the current domain, and update InterpretationController
        // ////////////////////////////////////////////////////////////////////////

        if (drug) {
            TargetList* targets = _interpretation->getDrugResponseAnalysis()->getTreatment()->getTargets();
            TargetList* drugTargets            = drug->getTargets();

            targets->clear();
            for (int i = 0; i < drugTargets->size(); ++i) {
                ezechiel::core::Target* target = ezechiel::core::CoreFactory::createEntity<Target>(APPUTILSREPO, targets);
                target->copyFrom(drugTargets->at(i));
                targets->append(target);
            }

            // TODO: To be checked: Do we really need that?
            dosagesView->setProperty("routes", QVariant::fromValue(drug->getAdme()->getIntakes()));
            adjustmentsView->setProperty("routes", QVariant::fromValue(drug->getAdme()->getIntakes()));

            adjustmentTabController->setAdjustmentDateFromMaster(QDateTime::currentDateTime());
        }

        flowController->drugChanged(index);
        flowController->evaluate();

        _chartDataController->dosageUpdated(shouldPercentilesBeComputed, false);

        // To start a first dose proposition
        adjustmentSettingsUpdated();
    }
    else {
        // We are dealing with a request
        Q_ASSERT(drug);

        _interpretation->getDrugResponseAnalysis()->setDrugModel(drug);
        TargetList* targets = _interpretation->getDrugResponseAnalysis()->getTreatment()->getTargets();
        TargetList* drugTargets            = drug->getTargets();

        targets->clear();
        for (int i = 0; i < drugTargets->size(); ++i) {
            ezechiel::core::Target* target = ezechiel::core::CoreFactory::createEntity<Target>(APPUTILSREPO, targets);
            target->copyFrom(drugTargets->at(i));
            targets->append(target);
        }

        covariateTabController->reset(drug->getCovariates());

        updateSexAndAgeCovariates();

        // TODO: To be checked: Do we really need that?
        dosagesView->setProperty("routes", QVariant::fromValue(drug->getAdme()->getIntakes()));
        adjustmentsView->setProperty("routes", QVariant::fromValue(drug->getAdme()->getIntakes()));

        Dosage *lastDosage = nullptr;
        if (_interpretation->getDrugResponseAnalysis()->getTreatment() != nullptr) {
            if (_interpretation->getDrugResponseAnalysis()->getTreatment()->getDosages() != nullptr) {
                lastDosage = _interpretation->getDrugResponseAnalysis()->getTreatment()->getDosages()->lastDosage();
            }
        }
        if (lastDosage != nullptr) {
            adjustmentTabController->setAdjustmentDateFromMaster(lastDosage->getEndTime().addMSecs(lastDosage->getInterval().mSecs()));
        }
        else {
            adjustmentTabController->setAdjustmentDateFromMaster(QDateTime::currentDateTime());
        }

        flowController->drugChanged(index);
        flowController->evaluate();

        _chartDataController->dosageUpdated(shouldPercentilesBeComputed, true);
    }

}


void InterpretationController::currentStudyChanged(int index)
{
    // Not used now. There is a one-to-one relationship between the study and the domain

    Q_UNUSED(index);
}

void InterpretationController::goToNextEvent()
{
    QList<QDateTime> eventList = buildEventsList();
    QDateTime eventDate;
    for (int i = 0; i < eventList.size(); i++) {
        if (eventList[i] > _maxX) {
            eventDate = eventList[i];
            break;
        }
    }
    if (eventDate.isValid()) {
        setDateViewRange(eventDate);
    }
}

void InterpretationController::goToPreviousEvent()
{
    QList<QDateTime> eventList = buildEventsList();
    QDateTime eventDate;
    for (int i = eventList.size() - 1; i >= 0; i--) {
        if (eventList[i] < _minX) {
            eventDate = eventList[i];
            break;
        }
    }
    if (eventDate.isValid()) {
        setDateViewRange(eventDate);
    }
}

QList<QDateTime> InterpretationController::buildEventsList()
{
    // We build the list of all events: dosage changes, covariate changes, measures, adjustment time
    QList<QDateTime> list;
    DrugTreatment *drugTreatment = _interpretation->getDrugResponseAnalysis()->getTreatment();
    if (drugTreatment == nullptr) {
        return list;
    }
    for (auto dosage : drugTreatment->getDosages()->getList()) {
        list.append(dosage->getApplied());
    }
    for (auto dosage : drugTreatment->getDosages()->getList()) {
        list.append(dosage->getEndTime());
    }
    for (auto covariate : drugTreatment->getCovariates()->getList()) {
        list.append(covariate->getDate());
    }
    for (auto measure : drugTreatment->getMeasures()->getList()) {
        list.append(measure->getMoment());
    }
    if (adjustmentTabController->getAdjustmentDate().isValid()) {
        list.append(adjustmentTabController->getAdjustmentDate());
    }
    std::sort(list.begin(), list.end());
    return list;
}

Q_INVOKABLE void InterpretationController::resetViewRange()
{
    updateInterpretationRange(EViewRangeUpdateContext::Start);
    flowController->evaluate();
    _chartDataController->dosageUpdated(shouldPercentilesBeComputed);
}

void InterpretationController::shiftViewRange(double dX)
{
    setViewRange(_minX.addSecs(dX), _maxX.addSecs(dX));
}

void InterpretationController::zoomViewRange(double f)
{
    int d = _minX.secsTo(_maxX) * f;
    setViewRange(_minX, _minX.addSecs(d));
}

void InterpretationController::setDateViewRange(const QDateTime &date)
{
    qint64 currentRange = _minX.secsTo(_maxX);
    QDateTime dMin = date.addSecs(- currentRange / 2);
    QDateTime dMax = date.addSecs(currentRange / 2);
    setViewRange(dMin, dMax);
}

void InterpretationController::setViewRange(const QDateTime &minX, const QDateTime &maxX)
{
    _userRequestedMinX = minX;
    _userRequestedMaxX = maxX;

    // Do not allow too small ranges. At least one hour
    if (minX.secsTo(maxX) < 3600)
        _userRequestedMaxX = minX.addSecs(3600);

//    std::cout << "User view range resize from ";
//    std::cout << minX.toLocalTime().toString().toLatin1().data();
//    std::cout << " to ";
//    std::cout << maxX.toLocalTime().toString().toLatin1().data();
//    std::cout << std::endl;

    if (updateInterpretationRange(EViewRangeUpdateContext::UserResize)) {
        flowController->evaluate();
        _chartDataController->dosageUpdated(shouldPercentilesBeComputed);
    }
    else {
        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(chartView, "requestPaint"));
    }
}

bool InterpretationController::updateInterpretationRange(EViewRangeUpdateContext context)
{
    Q_ASSERT(predictionspec);
    Q_ASSERT(_interpretation->getDrugResponseAnalysis()->getTreatment());

    QDateTime now = QDateTime::currentDateTime();
    QDateTime dateOfLastDose;
    QDateTime dateOfFirstDose;
    QDateTime dateOfLastMeasure;
    QDateTime dateOfAdjustment;
    double lastDoseInterval = 0.0;
    bool isAtSteadyState = false;
    bool hasNoEnd = false;
    bool hasDosage = false;
    QDateTime currentStartDate = predictionspec->getStartDate();
    QDateTime currentEndDate = predictionspec->getEndDate();    
    QDateTime startDate;
    QDateTime endDate;

    // 1. Get relevant dates for computing the start and end dates

    // Get dates of first and last doses
    const QList<Dosage*> &dosages = _interpretation->getDrugResponseAnalysis()->getTreatment()->getDosages()->getList();    
    foreach (Dosage *dosage, dosages) {
        if (dateOfFirstDose.isNull() || dateOfFirstDose > dosage->getApplied()) {
            dateOfFirstDose = dosage->getApplied();
        }
        if (dateOfLastDose.isNull() || dateOfLastDose < dosage->getEndTime()) {
            dateOfLastDose = dosage->getEndTime();
            lastDoseInterval = dosage->getInterval().toHours();
            isAtSteadyState = dosage->getIsAtSteadyState();
            hasNoEnd = !dosage->getHasEndDate();
        }
    }
    hasDosage = dosages.size() > 0;

    // Get date of last measure
    const QList<CoreMeasure*> &measures = _interpretation->getDrugResponseAnalysis()->getTreatment()->getMeasures()->getList();
    foreach (CoreMeasure *measure, measures) {
        if (dateOfLastMeasure.isNull() || dateOfLastMeasure < measure->getMoment()) {
            dateOfLastMeasure = measure->getMoment();
        }
    }

    // Get date of adjustment
    dateOfAdjustment = adjustmentTabController->getAdjustmentDate();

    // 2. Handle the different contexts
    switch (context) {
        case EViewRangeUpdateContext::UserResize:
        {
            // Limit range computation to avoid memory problems
            if (_userRequestedMinX.daysTo(_userRequestedMaxX) > NBCYCLES_VIEWRANGE_MAX*lastDoseInterval/24.0) {
                return false;
            }

            bool bShowPercBefore = shouldPercentilesBeComputed;
            _minX = _userRequestedMinX;
            _maxX = _userRequestedMaxX;
            shouldPercentilesBeComputed = ((_minX.daysTo(_maxX) < NBCYCLES_TO_COMPUTE_PERCENTILES*lastDoseInterval/24.0));
            bool bShowPercAfter = shouldPercentilesBeComputed;
            if (!bShowPercBefore && bShowPercAfter) {
                // Force recomputation!
                break;
            }

            bool isOlderDataMissing = _minX < currentStartDate && (currentStartDate > (hasDosage ? dateOfFirstDose : dateOfAdjustment));
            bool isNewerDataMissing = _maxX > currentEndDate;

            // Do nothing if we already have all the requested data
            if (!isOlderDataMissing && !isNewerDataMissing) {
                return false;
            }
            break;
        }        
        case EViewRangeUpdateContext::AdjustmentDate:
        {
            // Recompute the view range only if the adjustment date is not part of it
            if (dateOfAdjustment >= _minX && dateOfAdjustment <= _maxX) {
                break;
            }
            // Intentional fall through...
        }
        case EViewRangeUpdateContext::Start:
        case EViewRangeUpdateContext::Dosage:
        {
            // Check if we have at lease one dosage
            if (hasDosage) {
                _minX = dateOfFirstDose;
                _maxX = dateOfLastDose;
                if (measures.length() > 0 && dateOfLastMeasure > dateOfLastDose) {
                    _maxX = dateOfLastMeasure;
                }
                _maxX = _maxX.addSecs(NBCYCLES_AFTER_LASTDOSE*lastDoseInterval*3600);

                if (isAtSteadyState) {
                    _minX = now.addSecs(-NBCYCLES_BEFORE_NOW*lastDoseInterval*3600);
                    _maxX = now.addSecs(NBCYCLES_AFTER_NOW*lastDoseInterval*3600);
                }
                // Limit range to avoid too long computations
                if (_minX.secsTo(_maxX) > NBCYCLES_MAX_FOR_INITIAL_COMPUTATION*lastDoseInterval*3600) {
                    _minX = _maxX.addSecs(-NBCYCLES_MAX_FOR_INITIAL_COMPUTATION*lastDoseInterval*3600);
                }
            }
            else {
                // If no dosages, use the adjustment date
                _minX = dateOfAdjustment;
                _maxX = _minX.addDays(NBCYCLES_AFTER_LASTDOSE);
            }
            break;
        }
        case EViewRangeUpdateContext::Adjustment:
            break;
        default:
            break;
    }

    // 3. Compute more than what will be displayed so the user can zoom and
    // translate without launching a new computation each time
    double nSecs = _minX.secsTo(_maxX);
    double margin = nSecs*COMPUTATION_MARGIN_FACTOR/2.0;
    startDate = _minX.addSecs(-margin);
    endDate = _maxX.addSecs(margin);

    shouldPercentilesBeComputed = ((_minX.daysTo(_maxX) < NBCYCLES_TO_COMPUTE_PERCENTILES*lastDoseInterval/24.0));

    // 4. Launch computation of new curves
    predictionspec->setStartDate(startDate);
    predictionspec->setEndDate(endDate);
    flowController->invalidateInterpretation();

    std::cout << "Launching computations (";
    switch (context) {
        case EViewRangeUpdateContext::Start: std::cout << "Start"; break;
        case EViewRangeUpdateContext::Dosage: std::cout << "Dosage"; break;
        case EViewRangeUpdateContext::AdjustmentDate: std::cout << "AdjustmentDate"; break;
        case EViewRangeUpdateContext::Adjustment: std::cout << "Adjustment"; break;
        case EViewRangeUpdateContext::UserResize: std::cout << "UserResize"; break;
    }
    std::cout << ") from ";
    std::cout << startDate.toLocalTime().toString().toLatin1().data();
    std::cout << " to ";
    std::cout << endDate.toLocalTime().toString().toLatin1().data();
    std::cout << std::endl;

    return true;
}

void InterpretationController::validateTab(int step)
{
    flowController->validateTab(StepType::convert(step));
}

void InterpretationController::toggleValidation(int step)
{
    flowController->toggleValidation(StepType::convert(step));
}

Patient * InterpretationController::getCurrentPatient() const
{
    return _currentPatient;
}

void InterpretationController::setCurrentPatient(Patient *patient)
{
    if (_currentPatient != patient) {
        _currentPatient = patient;
        emit newCurrentPatient();
    }
}

void InterpretationController::currentTabChanged(int index)
{
    if (StepType::isValid(index))
    {
        _graphInformationSelection->setCurrentTab(StepType::convert(index));
        _chartDataController->setCurrentTab(StepType::convert(index));
    }
}

PredictionSpec *InterpretationController::getPredictionSpec() const
{
    return predictionspec;
}

QDateTime InterpretationController::getLastDataTime(Interpretation *interpretation) const
{
    QDateTime lastTime;
    DrugTreatment *treatment = interpretation->getDrugResponseAnalysis()->getTreatment();
    foreach (CoreMeasure * measure, treatment->getMeasures()->getList()) {
        if ((!lastTime.isValid()) || (measure->getMoment() > lastTime)) {
            lastTime = measure->getMoment();
        }
    }
    foreach (PatientVariate * covariate, treatment->getCovariates()->getList()) {
        if ((!lastTime.isValid()) || (covariate->getDate() > lastTime)) {
            lastTime = covariate->getDate();
        }
    }

    return lastTime;

}

void InterpretationController::validateInterpretation(bool isValid)
{
    if (isValid) {
        _interpretation->setValidateInterpretationTime(QDateTime::currentDateTime());
        Duration processingTime(0, 0, _interpretation->getStartInterpretationTime().secsTo(_interpretation->getValidateInterpretationTime()));
        Duration processingTimeFromData(0, 0, getLastDataTime(_interpretation).secsTo(_interpretation->getValidateInterpretationTime()));
#ifndef CONFIG_DEMO
        QMessageBox messageBox;
        messageBox.setWindowTitle("Processing time");
        messageBox.setText(QString("Interpretation total processing time from last data was : <br/><b>%1</b> "
                                                          "<br/><br/>Processing time from starting the interpretation was :  <br/><b>%2</b>").arg(processingTimeFromData.toLongString()).arg(processingTime.toLongString()));
        messageBox.setTextFormat(Qt::TextFormat::RichText);
        messageBox.exec();
//        QMessageBox::information(0, "Processing time", QString("Interpretation total processing time from last data was : \n<b>%1</b> "
//                                                               "\n\nProcessing time from starting the interpretation was :  \n<b>%2</b>").arg(processingTimeFromData.toLongString()).arg(processingTime.toLongString()));
#endif // CONFIG_DEMO

        flowController->validateInterpretation();

        // We got an issue with this predictionData
        //_webchannel->registerObject("some", _interpretation->getAnalysis()->getChartData()->getAdjPred()->getPredictive()->getPredictionData());

        emit interpretationValidated();
    }
    else {
        flowController->invalidateInterpretation();
    }
}

bool InterpretationController::isInterpretationValid()
{
    return flowController->isInterpretationValidated();
}

void InterpretationController::updateSexAndAgeCovariates()
{
    Person::GenderType gender = _currentPatient->person()->gender();
    double sex = 1.0;
    if (gender == Person::GenderType::Female)
        sex = 0.0;

    covariateTabController->setSinglePatientVariate("sex", sex);

    QDate birthday = _currentPatient->person()->birthday();
    QDateTime realBirthdate(birthday);
    covariateTabController->setBirthdate(realBirthdate);
//    covariateTabController->setSinglePatientVariate("birthdate", realBirthdate.toMSecsSinceEpoch() / (qint64)1000);

    // Trigger a new calculation
    covariateUpdated();
}

/***********************************************************************
 * TODO: Shall we really start an adjustment calculation when current time changes?
 * I guess not
 ***********************************************************************/
//void InterpretationController::currentTimeChanged(QDateTime time) {
//    QDateTime stime = predictionspec->getAdjustmentDate();//adjustmentSettingsModel->data(adjustmentSettingsModel->index(0,0),AdjustmentSettingsModel::DateRole).toDateTime();
//    if (stime < time && (stime.date() < time.date() || stime.time().hour() != stime.time().hour() || stime.time().minute() != time.time().minute())) {
//        _adjustmentDate = time;
//        initiateUpdateRevPred();
//    }
//}


void InterpretationController::saveInterpretation()
{
#ifndef CONFIG_DEMO
    QString dirPath;

    // Settings to keep track of the previous path
    dirPath = SETTINGS.get(ezechiel::core::Module::GUI,"interpretationDirPath", "").toString();

//    Patient* currentPatient = getCurrentPatient();

//    dirPath +=

    QString fileName = QFileDialog::getSaveFileName(QApplication::activeWindow(), tr("Save Request File"),
                                                    dirPath,
                                                    tr("Tucuxi Interpretation File (*.tui)"));
    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".tui"))
        fileName += ".tui";

    dirPath = QFileInfo(fileName).absoluteDir().absolutePath();
    SETTINGS.set(ezechiel::core::Module::GUI,"interpretationDirPath", dirPath);

//    InterpretationToRequestXmlExport exporter;
    InterpretationXmlExport exporter;
    QString xml = exporter.toXml(getInterpretation());
    QFile interpretationFile(fileName);
    if (!interpretationFile.open(QFile::WriteOnly)) {
        exit(0);
    }
    QTextStream out(&interpretationFile);
    // Necessary for Windows, else wrong codec selected
    out.setCodec("UTF-8");
    out << xml;
    interpretationFile.close();

#else
    UnavailableFunctionality::displayMessage();
#endif // CONFIG_DEMO
}


QString InterpretationController::handleFileChosen(const QString &urlString) {
    const QUrl url(urlString);
    QString fileName;
    if (url.isLocalFile()) {
        fileName = (QDir::toNativeSeparators(url.toLocalFile()));
    } else {
        fileName = (urlString);
    }
    if (!fileName.endsWith(".pdf")) {
        fileName += ".pdf";
    }
    return fileName;
}

void InterpretationController::saveStatistics()
{

    QString dirPath;

    // Settings to keep track of the previous path
    dirPath = SETTINGS.get(ezechiel::core::Module::GUI,"statisticsDirPath", "").toString();

    QString fileName = QFileDialog::getSaveFileName(QApplication::activeWindow(), tr("Save Stats File"),
                                                    dirPath,
                                                    tr("CSV (*.csv)"));
    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".csv"))
        fileName += ".csv";

    dirPath = QFileInfo(fileName).absoluteDir().absolutePath();
    SETTINGS.set(ezechiel::core::Module::GUI,"statisticsDirPath", dirPath);

    ChartDataExporter exporter;
    QString content = exporter.exportData(_chartDataController->chartData, EXPORT_ALL);
    QFile statisticsFile(fileName);
    if (!statisticsFile.open(QFile::WriteOnly)) {
        exit(0);
    }
    QTextStream out(&statisticsFile);
    out << content;
    statisticsFile.close();
}

void InterpretationController::showStatistics(int what)
{
    ChartDataHtmlExporter exporter;
    QString content = exporter.exportData(_chartDataController->chartData, what);
    QMessageBox::information(nullptr,"Current statistics", content);

}

void InterpretationController::addPatient()
{
#ifndef CONFIG_DEMO

    Patient* patient = AdminFactory::createEntity<Patient>(ABSTRACTREPO, _patients);
    //static_cast<Patient*>(patient)->externalId(QString("P%1").arg(i + 1, 3, 10, QChar('0')));
    //static_cast<Patient*>(patient)->stayNumber(QString("S%1").arg(i + 1, 3, 10, QChar('0')));
    //static_cast<Patient*>(patient)->statOk(i % 2);
    //static_cast<Patient*>(patient)->person()->firstname(names[i]);
    //static_cast<Patient*>(patient)->person()->name(QString("Doe"));
    patient->person()->gender(Person::GenderType(1));
    patient->person()->birthday(QDate(1960, 1, 1));
    //static_cast<Patient*>(patient)->person()->location()->address(QString("Avenue de Lausanne 1"));
    //static_cast<Patient*>(patient)->person()->location()->postcode(QString("1000"));
    //static_cast<Patient*>(patient)->person()->location()->city("Genève");
    //static_cast<Patient*>(patient)->person()->location()->state("GE");
    //static_cast<Patient*>(patient)->person()->location()->country("Switzerland");

    _patients->append(patient);
    emit patientsChanged(_patients);
#else
    UnavailableFunctionality::displayMessage();
#endif // CONFIG_DEMO
}

void InterpretationController::removePatient(int index)
{
#ifndef CONFIG_DEMO
    if (index < 0) {
        return;
    }
    if (index >= _patients->size()) {
        return;
    }

    if ((_interpretation != nullptr) && (_interpretation->getDrugResponseAnalysis()->getTreatment()->getPatient() == _patients->at(index)))
    {
        // Check if we can loose the work done
        if (QMessageBox::warning(QApplication::activeWindow(),
                                 tr("Be careful"),
                                 tr("Are you sure you want to remove the patient? You will loose your current treatment analysis"),
                                 QMessageBox::Yes,
                                 QMessageBox::No) == QMessageBox::No) {
            return;
        }

        // Stop every calculation
        calculationController->abortAll();

        setCurrentPatient(nullptr);

        // TODO:  Clean the interpretation
        deleteCurrentRequest();


        // Inialize the new interpretation
        Interpretation *interpretation = CoreFactory::createEntity<Interpretation>(ABSTRACTREPO, this);

        interpretation->setStartInterpretationTime(QDateTime::currentDateTime());

        // Create a new treatment and assigns it to both objects
        ezechiel::core::DrugTreatment *newTreatment = CoreFactory::createEntity<DrugTreatment>(ABSTRACTREPO, _interpretation->getDrugResponseAnalysis());

        interpretation->getDrugResponseAnalysis()->setTreatment(newTreatment);
        interpretation->getRequest()->setTreatment(newTreatment);
        interpretation->setInterpretationType(Interpretation::InterpretationType::NoRequest);


        populateMultipleActiveSubstance();
        setRawRequest("");


        // Set it to the controller
        setNewInterpretation(interpretation, true);

    }
    _patients->remove(index);
    emit patientsChanged(_patients);
#else
    UnavailableFunctionality::displayMessage();
#endif // CONFIG_DEMO
}

bool InterpretationController::isTesting()
{
#ifdef CONFIG_GUITEST
    return true;
#else
    return false;
#endif // CONFIG_GUITEST
}

QString InterpretationController::getReportFileName()
{
    return "TestReport.pdf";
}
