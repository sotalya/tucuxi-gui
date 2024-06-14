/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef FLOWCONTROLLER_H
#define FLOWCONTROLLER_H

#include <QObject>

#include "admin/src/dal/validationstatus.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

class Interpretation;

}
}
}

typedef Tucuxi::Gui::Admin::ValidationStatus::StatusType StatusType;
typedef Tucuxi::Gui::Admin::ValidationStatus::DataStatusType DataStatusType;
typedef Tucuxi::Gui::Admin::ValidationStatus::ValidationStatusType ValidationStatusType;

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class InterpretationController;

class FlowController : public QObject
{
    Q_OBJECT
public:
    explicit FlowController(QObject *parent = nullptr);

    virtual ~FlowController();
    void setInterpretation(Tucuxi::Gui::Admin::Interpretation *interpretation);

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

    void validateTab(Tucuxi::Gui::Admin::StepType::Enum tabNumber);
    void toggleValidation(Tucuxi::Gui::Admin::StepType::Enum tabNumber);
    void invalidateTab(Tucuxi::Gui::Admin::StepType::Enum tabNumber);
    void evaluate();

protected:
    void propagateEnable(bool s, Tucuxi::Gui::Admin::StepType::Enum from);
    void propagateStatus(DataStatusType s, Tucuxi::Gui::Admin::StepType::Enum from);
    void propagateUnvalidate(Tucuxi::Gui::Admin::StepType::Enum from);

    void generateEnables();
    void generateStatuses();

    void start();
    void end();

    Tucuxi::Gui::Admin::Interpretation *interpretation;
    Tucuxi::Gui::Admin::ValidationStatus *currentStatus;
    Tucuxi::Gui::Admin::ValidationStatus * oldStatus;

    Tucuxi::Gui::Admin::StepType::Enum validating;
    Tucuxi::Gui::Admin::StepType::Enum invalidating;

    QObject *flowView;

    bool _isInterpretationValidated;

    InterpretationController *interpretationController;

};

}
}
}

#endif // FLOWCONTROLLER_H
