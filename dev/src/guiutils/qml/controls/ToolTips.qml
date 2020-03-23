pragma Singleton

import QtQuick 2.5

QtObject {

    property QtObject applicationBar: QtObject {
        readonly property string save:    CONFIG_DEMO ? qsTr("Save the current clinical interpretation: Not available in the demo version.")
                                                      : qsTr("Save the current clinical interpretation.")
        readonly property string home: qsTr("Start a new clinical interpretation")
        readonly property string pending: qsTr("Get the list of pending requests (concentration results to interpret)")
        readonly property string quit: qsTr("Quit Tucuxi")
    }
    property QtObject flowButtons: QtObject {
        readonly property string patient: qsTr("This tab is to enter information about the patient,<br/>
                                such as birthdate and gender that may be used as pharmacokinetic covariates,<br/>
                                or administrative data needed to attach the report to the medical record.")

        readonly property string drug: qsTr("This tab is to select the medicinal drug analyzed<br/>
                                and the model to support the clinical interpretation of the result. <br/>
                                For a given drug, several models might be available, corresponding to various domains of application.<br/>
                                The appropriate model can be chosen according to the reference of its publication.")

        readonly property string dosage: qsTr("This tab is to enter the current dosage and the dosage history. <br/>
                                Use the <img src=\"qrc:/icons/buttons/add.png\" height=\"22\" width=\"22\"/> button to introduce a new dosage specification.<br/>
                                The first dosage can be considered either at steady state or not. Recent dosage changes can be specified by adding new dosages.<br/>
                                <i>Population percentiles</i> are then computed, which show the range of likely concentration profiles<br/>
                                predicted under the defined dosage history in a typical patient.")

        readonly property string covariates: qsTr("This tab is to specify the patient's individual covariates. <br/>
                               Depending on the model selected, various covariates are assumed to influence the pharmacokinetic parameters.<br/>
                               The <i>default</i> column contains default covariate values, and the <i>current</i> column contains values specified at present.<br/>
                               Note that age is automatically calculated from the birthdate and cannot be modified here.<br/>
                               Sex is as well derived from the information entered in the patient panel.<br/>
                               Covariate values can change over time time if specified sequentially at different dates.<br/>
                               <i>Individual prior percentiles</i> will be computed, showing the predicted concentration profile taking into account the covariate values.")

        readonly property string measures: qsTr("This tab is to enter one or several concentration measurement results.<br/>
                                Use the <img src=\"qrc:/icons/buttons/add.png\" height=\"22\" width=\"22\"/> button to add a new measurement result. There is no limitation in the number of measurements.<br/>
                                <i> Individual posterior percentiles</i> will then indicate the most likely prediction range expected for further measurements in this specific patient.")

        readonly property string targets: qsTr("This tab is to specify concentration targets for the dosage adjustment engine to suggest a dose adaptation.<br/>
                               Multiple targets can be defined, in which case all of them must be satisfied simultaneously by the adjustment proposed.<br/>
                               Targets have default population values that can be overridden to specify individual concentration levels aimed to in this patient.<br/>
                               It is possible to enter manually a dosage adjustment different from the adaptations automatically proposed.<br/>
                               The starting time of the dosage adjustment can be changed as well, which will trigger a new proposition.")

        readonly property string adjustment: qsTr("This tab shows one or several adjustments proposed, which best fit the targets previously defined.<br/>
                               Clicking on a given adjustment will show its corresponding prediction curve.<br/>
                               Clicking on the column entitled <i>i</i> will show the target values.<br/>
                               The adjustment to apply to the patient's dosage can be selected by pressing on the <img src=\"qrc:/icons/buttons/check.png\" height=\"22\" width=\"22\"/> button.<br/>
                               This adjustment can still be edited and further modified at the bottom of the panel, while observing the resulting prediction of the concentration curve")

        readonly property string validation: qsTr("The validation tab allows to add text comments that are to be written onto the report.<br/>
                               Predictons and percentiles to represent on the report can also be selected here.<br/>
                               Once the choice is made, pressing the <i>Validate</i> button will open the final report panel.")

        readonly property string report: qsTr("This tab is to show the report for proofreading, and to export it into a PDF file.<br>
                               Modifications of the report just require jumping back to any previous panel to edit the corresponding information,<br/>
                               and to validate the revised report again in this tab.")
    }


    property QtObject patientTab: QtObject {
        readonly property string patientList: qsTr("Select here the patient for whom concentration results are to be clinically interpreted.<br/>
                               In particular, enter the birthdate and gender in this panel for using age and sex as pharmacokinetic covariates.")
        readonly property string birthdate: qsTr("The birthdate is needed to compute the age (or the postnatal age) required in certain pharmacokinetic models.")
        readonly property string gender : qsTr("The gender is needed by certain pharmacokinetic models that include sex as covariate.<br/>
                               It can be entered or modified only here.")
    }

    property QtObject drugTab: QtObject {
        readonly property string activeSubstanceTitle : qsTr("Choose the active substance of the monitored drug")
        readonly property string domainTitle : qsTr("The <i>domain</i> indicate the type of patients for whom the model is appropriate.<br/>
                                 If more than one model is available, choose the one that best corresponds to the patient.")
        readonly property string studyTitle : qsTr("The <i>study</i> indicates the source publication from which the model has been derived.<br/>
                                 If more than one model is available, choose the one that best corresponds to the patient.")
    }

    property QtObject dosageTab: QtObject {
        readonly property string add :      qsTr("Specify a new dosage.")
        readonly property string edit :      qsTr("Edit this dosage specification.")
        readonly property string remove :      qsTr("Remove this dosage specification.")
        readonly property string dosageHistoryTitle :   qsTr("This list shows the dosage history.<br/>
                                 A new dosage can be added by clicking on the <img src=\"qrc:/icons/buttons/add.png\" height=\"22\" width=\"22\"/> button.")
        readonly property string from :      qsTr("Starting date for this dosage")
        readonly property string to :      qsTr("Ending date for this dosage")
        readonly property string dose :      qsTr("Unitary dose (with its unit)")
        readonly property string interval :      qsTr("Dosing interval (time between two doses)")
        readonly property string infusion :      qsTr("If the dose is given by intravenous infusion,<br/> enter here the infusion duration")
    }

    property QtObject dosageDialog: QtObject {
        readonly property string steadyState: qsTr("Indicate here whether the dosage starts at a specific time or is considered at steady state (introduced long ago).")
        readonly property string dose : qsTr("Unitary dose (with its unit)")
        readonly property string interval : qsTr("Dosing interval (time between two doses)")
        readonly property string route : qsTr("Route of administration (bolus intravenous injection, intravenous infusion or extravascular administration")
        readonly property string infusion : qsTr("Infusion duration, for intravenous infusions")
        readonly property string date : qsTr("Date of last dose administration")
        readonly property string from :      qsTr("Starting date for this dosage")
        readonly property string to :      qsTr("Ending date for this dosage")
    }

    property QtObject covariateTab: QtObject {
        readonly property string drugVariateList : qsTr("This list contains the covariates used by the drug pharmacokinetic model.<br/>
                                 Default values might be kept if the corresponding information about the patient is missing.<br/>")
        readonly property string patientVariateList : qsTr("This list shows the modifications of specific covariates over time.<br/>
                                 Multiple modifications can be introduced, each of them applying since the date entered.<br/>
                                 For certain covariates, levels will be automatically interpolated between two modifications.")
        readonly property string name : qsTr("Name of the covariate")
        readonly property string defaultValue : qsTr("Default value of the covariate, as defined in the drug model.")
        readonly property string currentValue : qsTr("The current value reflects the covariate level from the indicated time onwards.<br/>
                                 Previous values can be entered or modified as well for the covariates.")
        readonly property string value : qsTr("Value of the covariate at the specified date")
        readonly property string date : qsTr("Date at which the covariate value is observed.<br/>
                                 Depending on the drug model, either the value is applied constantly from that specific time onwards,<br/>
                                 or levels are interpolated linearly between pairs of observed value.<br/>
                                 If only a single value is given for a covariate, then it is applied throughout time.")
        readonly property string add : qsTr("Add a patient value for the selected covariate.")
    }

    property QtObject covariateDialog: QtObject {
        readonly property string type:  qsTr("Type of covariate")
        readonly property string value : qsTr("Value of the covariate")
        readonly property string date : qsTr("Correspnding date of observation for the covariate")
    }

    property QtObject measureTab: QtObject {
        readonly property string listTitle : qsTr("List of concentration measurement values")
        readonly property string id : qsTr("Id of the sample (not used in computation)")
        readonly property string value : qsTr("Measured concentration value")
        readonly property string sampleDate : qsTr("Date of sampling")
        readonly property string arrivalDate : qsTr("Date sample entry in the lab (not used in computation)")
        readonly property string add : qsTr("Add a new concentration measurement value")
        readonly property string modify : qsTr("Modify the selected concentration value")
        readonly property string remove : qsTr("Remove the selected concentration measurement")
    }

    property QtObject measureDialog: QtObject {
        readonly property string sampleId: qsTr("Id of the sample (not used in computation)")
        readonly property string value : qsTr("Measured concentration value ")
        readonly property string date : qsTr("Date of sampling")
        readonly property string arrivalDate : qsTr("Date sample entry in the lab (not used in computation)")
    }

    property QtObject targetTab: QtObject {
        readonly property string listTitle : qsTr("List of targets used for dosage adjustment.<br/>
                                 Initial default target values are defined in the drug model.<br/>
                                 These targets can be modified, removed or complemented if the patient requires different values.")
        readonly property string type : qsTr("Type of target, defined as a pharmacokinetic quantity")
        readonly property string cMin : qsTr("Minimum acceptable value for the target, if it exists")
        readonly property string cBest : qsTr("Optimal value for the target.<br/> The adjustment score will be maximized according to closeness to this value.")
        readonly property string cMax : qsTr("Maximum acceptable value for the target, if it exists")
        readonly property string add : qsTr("Add another target. Be aware that all targets will have to be met simultaneously<br/>
                                 to propose an adjustment. Overconstraint may end up with no propositions at all.")
        readonly property string edit : qsTr("Edit the selected target")
        readonly property string remove : qsTr("Remove the selected target")
    }

    property QtObject targetDialog: QtObject {
        readonly property string targetType: qsTr("Type of target, defined as a pharmacokinetic quantity ")
        readonly property string concentrations : qsTr("Concentrations")
        readonly property string cMin : qsTr("Minimum acceptable value for the target")
        readonly property string cMax : qsTr("Maximum acceptable value for the target")
        readonly property string cBest : qsTr("Optimal value for the target")
        readonly property string times : qsTr("Times")
        readonly property string tMin : qsTr("Minimum acceptable value for the target ")
        readonly property string tMax : qsTr("Maximum acceptable value for the target ")
        readonly property string tBest : qsTr("Optimal value for the target")
    }

    property QtObject adjustmentTab: QtObject {
        readonly property string select :      qsTr("Select the proposed adjustment and apply it in the patient")
        readonly property string add :      qsTr("Add a manual adjustment")
        readonly property string edit :      qsTr("Modify the selected adjustment")
        readonly property string remove :      qsTr("Remove the selected adjustment")
        readonly property string suggestedListTitle :  qsTr("List of suggested dosage adjustments.<br/>
                                                             Every adjustment fits within the targets limits.<br/>
                                                             The adjustments are sorted according to closeness to the targets.")
        readonly property string from :      qsTr("Starting date for the adjusted dosage")
        readonly property string to :      qsTr("Ending date of for the adjusted dosage")
        readonly property string dose :      qsTr("Unitary dose for the adjusted dosage (with its unit)")
        readonly property string interval :      qsTr("Dosing interval for the adjusted dosage")
        readonly property string infusion :      qsTr("Infusion duration, for intravenous infusions")
        readonly property string info :      qsTr("Passing the cursor over this field will show the targets values and their score.")
        readonly property string type :      qsTr("Adjustment selected among suggestions or edited manually")
        readonly property string adjustmentTime :      qsTr("Time at which the dosage adjustment starts")
        readonly property string adjustmentListTitle :      qsTr("List of selected or edited adjustments")
        readonly property string withLoadingDose : qsTr("Shall Tucuxi try to add a loading dose?")
        readonly property string withRestPeriod : qsTr("Shall Tucuxi try to add a rest period?")
    }

    property QtObject adjustmentDialog: QtObject {
        readonly property string from :      qsTr("Starting date for the adjusted dosage")
        readonly property string to :      qsTr("Ending date of for the adjusted dosage")
        readonly property string route : qsTr("Route of administration (bolus intravenous injection, intravenous infusion or extravascular administration")
        readonly property string dose :      qsTr("Unitary dose for the adjusted dosage (with its unit)")
        readonly property string interval :      qsTr("Dosing interval for the adjusted dosage")
        readonly property string infusion :      qsTr("Infusion duration, for intravenous infusions")
    }

    property QtObject validationTab: QtObject {
        readonly property string parametersListTitle :      qsTr("Population, prior and posterior values of each pharmacokinetic parameter")
        readonly property string populationParameters :      qsTr("<i>Population</i>  parameter values for a typical patient")
        readonly property string aprioriParameters :      qsTr("<i>A priori</i> parameter values computed based on the patient's individual covariates")
        readonly property string aposterioriParameters :      qsTr("Bayesian maximum likelihood <i>a posteriori</i> parameter values, <br/>
                                 computed based on both the patient's covariates and his/her measured concentrations")
        readonly property string validate :      qsTr("Press on this button to validate the clinical interpretation")
        readonly property string expectedness : qsTr("Comment about the <i>expectedness</i> of observed concentration measurements <br/>
                                 with regard to the current drug dosage and patient's characteristics (refer to <i>a priori</i> concentration percentiles)")
        readonly property string suitability : qsTr("Comment about the <i>suitability</i> of concentration exposure under the current dosage <br/>
                                 with respect to the defined targets (compare <i>a posteriori</i> concentration curves with therapeutic targets)")
        readonly property string prediction : qsTr("Comment about the proposed dosage adjustment and its predicted effect on target attainment <br/>
                                 compare <i>a posteriori</i> concentration curves after adjustment with therapeutic targets)")
        readonly property string remonitoring : qsTr("Comment about suggestions to repeat monitoring")
        readonly property string warning : qsTr("Express any warning that applies to this interpretation")
    }

    property QtObject startupScreen: QtObject {
        readonly property string newPatient :      CONFIG_DEMO ? "<b>Standard</b> : Add a new patient and start another clinical interpretation.<br/>
                                                                  <b>Demo</b> : Perform a clinical interpretation for one of the existing patients.<br/>
                                                                                 To toggle the tooltips on/off, press CTRL+SHIFT+T."
                                                                  : "Add a new patient and start a clinical interpretation.<br/>
                                                                                 To toggle the tooltips on/off, press CTRL+SHIFT+T."
        readonly property string load :      CONFIG_DEMO ? "<b>Standard</b> : Load a previously saved clinical interpretation.<br/>
                                                            <b>Demo</b> : Not available.<br/>
                                                                          To toggle the tooltips on/off, press CTRL+SHIFT+T."
                                                             : "Load a previously saved clinical interpretation.<br/>
                                                                To toggle the tooltips on/off, press CTRL+SHIFT+T."
        readonly property string pending :    CONFIG_DEMO ? "<b>Standard</b> : Get the list of pending requests from the institutional database.<br/>
                                                             <b>Demo</b> : Play with a virtual list of pending requests.<br/>
                                                                To toggle the tooltips on/off, press CTRL+SHIFT+T."
                                                              : "Get the list of pending requests from the institutional database.<br/>
                                                                To toggle the tooltips on/off, press CTRL+SHIFT+T."

        readonly property string quit :      qsTr("Quit Tucuxi")

    }

    property QtObject chart: QtObject {
        readonly property string popVisible : qsTr("Display the <i>population</i> prediction for a typical patient")
        readonly property string popPercVisible : qsTr("Display the <i>population</i> percentiles for typical patients")
        readonly property string aprioriVisible : qsTr("Display the <I>a priori</i> prediction for the current patient")
        readonly property string aprioriPercVisible : qsTr("Display the <I>a priori</i> percentiles for patients similar to the current one")
        readonly property string apostVisible : qsTr("Display the <i>a posteriori</i> prediction for the current patient")
        readonly property string apostPercVisible : qsTr("Display the <i>a posteriori</i> percentiles for further measurements in this patient")
        readonly property string adjVisible : qsTr("Display i>a posteriori</i> predictions resulting from dosage adjustments")
        readonly property string revVisible : qsTr("Highlight the i>a posteriori</i> prediction resulting from the suggested dosage adjustment")
    }

    property QtObject uncastedList: QtObject {
        readonly property string listTitle: qsTr("List of information that could not be casted from the data sent by the institutional database.<br/>
                                                  Modify the ambiguous values above and check the validity of all data shown. Once this is done, <br/>
                                                  press on the check button <img src=\"qrc:/icons/buttons/check.png\" height=\"22\" width=\"22\"/> to launch Tucuxi.")
        readonly property string field: qsTr("Field containing an ambiguous value")
        readonly property string value: qsTr("Value (string) found in the pending request")
        readonly property string comment: qsTr("comment from Tucuxi about how the value was handled")
        readonly property string check: qsTr("Press here to let Tucuxi know that you handled this ambiguous value")
    }

}
