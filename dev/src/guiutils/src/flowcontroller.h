//@@license@@

#ifndef FLOWCONTROLLER_H
#define FLOWCONTROLLER_H

#include <QObject>

#include "admin/src/dal/validationstatus.h"

class Interpretation;
class InterpretationController;

typedef ValidationStatus::StatusType StatusType;
typedef ValidationStatus::DataStatusType DataStatusType;
typedef ValidationStatus::ValidationStatusType ValidationStatusType;

class FlowController : public QObject
{
    Q_OBJECT
public:
    explicit FlowController(QObject *parent = nullptr);

    virtual ~FlowController();
    void setInterpretation(Interpretation *interpretation);

    void patientChanged(int index);
    void drugChanged(int index);

    void setFlowView(QObject *flowView);
    void initialize();

    /**
     * @brief startNewInterpretation To be called when a new interpretation starts
     *
     * Used for requests and no-requests interpretations
     */
    void startNewInterpretation();

    /**
     * @brief loadInterpretation To be called when an interpretation is loaded
     *
     * Has to be called after the _interpretation is set in the InterpretationController
     */
    void loadInterpretation();

    void validateInterpretation();
    void invalidateInterpretation();
    bool isInterpretationValidated() { return _isInterpretationValidated; }

public slots:

    void validateTab(StepType::Enum tabNumber);
    void toggleValidation(StepType::Enum tabNumber);
    void invalidateTab(StepType::Enum tabNumber);
    void evaluate();

protected:
    void propagateEnable(bool s, StepType::Enum from);
    void propagateStatus(DataStatusType s, StepType::Enum from);
    void propagateUnvalidate(StepType::Enum from);

    void generateEnables();
    void generateStatuses();

    void start();
    void end();

    Interpretation *interpretation;
    ValidationStatus *currentStatus;
    ValidationStatus * oldStatus;

    StepType::Enum validating;
    StepType::Enum invalidating;

    QObject *flowView;

    bool _isInterpretationValidated;

    InterpretationController *interpretationController;
};

#endif // FLOWCONTROLLER_H
