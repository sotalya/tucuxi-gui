//@@license@@

#include "defaultreport.h"
#include "core.h"

#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QTranslator>
#include <QApplication>

using namespace Tucuxi::Gui::Core::Core;

DefaultReport::DefaultReport(ReportData *data, QObject *parent) :
    Report(data, parent)
{

}

void DefaultReport::finishedLoading()
{
//    if (language() == "en")
//        return;

    //Install the translator
    QTranslator translator;
    translator.load(CORE->path(Core::Translations) + "/report_" + language());
    QApplication::installTranslator(&translator);

    QWebElementCollection collection;

    //The header
    page()->mainFrame()->findAllElements("#recipient\\.fullname").first().setInnerXml(tr("Recipient Name"));
    page()->mainFrame()->findAllElements("#recipient\\.fulladdress").first().setInnerXml(tr("Recipient Address"));
    page()->mainFrame()->findAllElements("#analyst\\.fullname").first().setInnerXml(tr("Analyst Name"));
    page()->mainFrame()->findAllElements("#analyst\\.fulladdress").first().setInnerXml(tr("Analyst Address"));
    page()->mainFrame()->findAllElements("#institute\\.name").first().setInnerXml(tr("Institute Name"));
    page()->mainFrame()->findAllElements("#institute\\.fulladdress").first().setInnerXml(tr("Institute Address"));
    page()->mainFrame()->findAllElements("#team\\.name").first().setInnerXml(tr("Team Name"));
    page()->mainFrame()->findAllElements("#team\\.members").first().setInnerXml(tr("Members list"));

    collection = page()->mainFrame()->findAllElements(".translatable\\.phone");
    for (int i = 0; i < collection.count(); ++i)
        collection.at(i).setInnerXml(tr("Phone :"));
    collection = page()->mainFrame()->findAllElements(".translatable\\.fax");
    for (int i = 0; i < collection.count(); ++i)
        collection.at(i).setInnerXml(tr("Fax :"));
    collection = page()->mainFrame()->findAllElements(".translatable\\.mail");
    for (int i = 0; i < collection.count(); ++i)
        collection.at(i).setInnerXml(tr("Mail :"));
    collection = page()->mainFrame()->findAllElements(".translatable\\.website");
    for (int i = 0; i < collection.count(); ++i)
        collection.at(i).setInnerXml(tr("Website :"));

    //The date and reference
    page()->mainFrame()->findAllElements("#creation\\.display").first().setInnerXml(tr("Creation date"));
    page()->mainFrame()->findAllElements("#translatable\\.reference").first().setInnerXml(tr("Reference :"));

    //The patient section
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.title").first().setInnerXml(tr("Patient"));
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.name").first().setInnerXml(tr("Name :"));
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.birthdate").first().setInnerXml(tr("Birthdate :"));
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.age").first().setInnerXml(tr("Age :"));
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.gender").first().setInnerXml(tr("Gender :"));
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.weight").first().setInnerXml(tr("Weight :"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.indication").first().setInnerXml(tr("Indication :"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.comedication").first().setInnerXml(tr("Comedication :"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.response").first().setInnerXml(tr("Response :"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.afterEffect").first().setInnerXml(tr("Adverse effects :"));

    //The sample section
    page()->mainFrame()->findAllElements("#translatable\\.sample\\.title").first().setInnerXml(tr("Sample"));
    page()->mainFrame()->findAllElements("#translatable\\.sample\\.drug").first().setInnerXml(tr("Treatment :"));
    page()->mainFrame()->findAllElements("#translatable\\.sample\\.external").first().setInnerXml(tr("Sample ID :"));
    page()->mainFrame()->findAllElements("#translatable\\.sample\\.date\\.display").first().setInnerXml(tr("Sample date :"));
    page()->mainFrame()->findAllElements("#translatable\\.sample\\.dosage\\.date\\.display").first().setInnerXml(tr("Last dose :"));
    page()->mainFrame()->findAllElements("#translatable\\.dosage\\.display").first().setInnerXml(tr("Current posology"));
    page()->mainFrame()->findAllElements("#translatable\\.interval\\.display").first().setInnerXml(tr("Interval"));
    page()->mainFrame()->findAllElements("#translatable\\.sample\\.display").first().setInnerXml(tr("Observed concentration"));
    page()->mainFrame()->findAllElements("#translatable\\.extrapolated\\.display").first().setInnerXml(tr("Extrapolated Residual Concentration"));

    //The interpretation section
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.title").first().setInnerXml(tr("Interpretation"));
    page()->mainFrame()->findAllElements("#translatable\\.proposedPosology").first().setInnerXml(tr("Proposed posology :"));

    //The next control section
    page()->mainFrame()->findAllElements("#translatable\\.nextControl\\.title").first().setInnerXml(tr("Next control"));

    //The chart section
    page()->mainFrame()->findAllElements("#translatable\\.chart\\.title").first().setInnerXml(tr("Concentration prediction"));

    //The covariate section
    page()->mainFrame()->findAllElements("#translatable\\.covariates\\.title").first().setInnerXml(tr("Covariates"));
    page()->mainFrame()->findAllElements("#translatable\\.covariates\\.name").first().setInnerXml(tr("Covariate"));
    page()->mainFrame()->findAllElements("#translatable\\.covariates\\.value").first().setInnerXml(tr("Value"));
    page()->mainFrame()->findAllElements("#translatable\\.covariates\\.description").first().setInnerXml(tr("Description"));

    //The parameters section
    page()->mainFrame()->findAllElements("#translatable\\.parameters\\.title").first().setInnerXml(tr("Parameters"));
    page()->mainFrame()->findAllElements("#translatable\\.parameters\\.identifier").first().setInnerXml(tr("Parameter"));
    page()->mainFrame()->findAllElements("#translatable\\.parameters\\.patient").first().setInnerXml(tr("A posteriori"));
    page()->mainFrame()->findAllElements("#translatable\\.parameters\\.reference").first().setInnerXml(tr("A priori"));
    page()->mainFrame()->findAllElements("#translatable\\.parameters\\.population").first().setInnerXml(tr("Typical patient"));

    //The references section
    page()->mainFrame()->findAllElements("#translatable\\.references\\.title").first().setInnerXml(tr("References"));

    //The comments
    collection = page()->mainFrame()->findAllElements(".translatable\\.comments");
    for (int i = 0; i < collection.count(); ++i)
        collection.at(i).setInnerXml(tr("Comments :"));

    //Remove the translator
    QApplication::removeTranslator(&translator);
}
