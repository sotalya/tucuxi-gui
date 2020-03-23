#include "compactreport.h"
#include "core.h"

#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QTranslator>
#include <QApplication>

CompactReport::CompactReport(ReportData *data, QObject *parent) :
    Report(data, parent)
{

}

void CompactReport::finishedLoading()
{
    if (language() == "en")
        return;

    //Install the translator
    QTranslator translator;
    translator.load(CORE->path(ezechiel::core::Core::Translations) + "/report_" + language());
    QApplication::installTranslator(&translator);

    QWebElementCollection collection;

    //The header
    page()->mainFrame()->findAllElements("#recipient\\.fullname").first().setInnerXml(tr("Recipient Name"));
    page()->mainFrame()->findAllElements("#recipient\\.fulladdress").first().setInnerXml(tr("Recipient Address"));
    page()->mainFrame()->findAllElements("#institute\\.name").first().setInnerXml(tr("Institute Name"));
    page()->mainFrame()->findAllElements("#institute\\.fulladdress").first().setInnerXml(tr("Institute Address"));
    page()->mainFrame()->findAllElements("#cmo\\.unit").first().setInnerXml(tr("Drugs laboratory"));
    page()->mainFrame()->findAllElements("#cmo\\.fullname").first().setInnerXml(tr("Chief Medical Officer Name"));
    page()->mainFrame()->findAllElements("#team\\.name").first().setInnerXml(tr("Therapeutic Drug Monitoring Unit"));
    page()->mainFrame()->findAllElements("#team\\.members").first().setInnerXml(tr("Members list"));
    page()->mainFrame()->findAllElements("#translatable\\.reference").first().setInnerXml(tr("Reference:"));
    page()->mainFrame()->findAllElements("#creation\\.display").first().setInnerXml(tr("Creation date"));

    //The title section
    page()->mainFrame()->findAllElements("#translatable\\.customTitle").first().setInnerXml(tr("Therapeutic Drug Monitoring:"));

    //The patient section
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.name").first().setInnerXml(tr("Patient:"));
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.birthdate").first().setInnerXml(tr("Birthdate:"));
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.gender").first().setInnerXml(tr("Sex:"));
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.age").first().setInnerXml(tr("Age:"));
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.weight").first().setInnerXml(tr("Weight:"));
    page()->mainFrame()->findAllElements("#translatable\\.patient\\.gist").first().setInnerXml(tr("GIST:"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.indication").first().setInnerXml(tr("Indication:"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.comedication").first().setInnerXml(tr("Comedication:"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.response").first().setInnerXml(tr("Response:"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.afterEffect").first().setInnerXml(tr("Adverse effects:"));

    //The sample section
    page()->mainFrame()->findAllElements("#translatable\\.sample\\.from").first().setInnerXml(tr("Sample:"));
    page()->mainFrame()->findAllElements("#translatable\\.sample\\.result").first().setInnerXml(tr("Result:"));
    page()->mainFrame()->findAllElements("#translatable\\.dosage\\.last\\.date\\.display").first().setInnerXml(tr("Last dose:"));
    page()->mainFrame()->findAllElements("#translatable\\.dosage\\.display").first().setInnerXml(tr("Dosage:"));
    page()->mainFrame()->findAllElements("#translatable\\.dosage\\.every").first().setInnerXml(tr("every"));
    page()->mainFrame()->findAllElements("#translatable\\.dosage\\.since").first().setInnerXml(tr("since"));
    page()->mainFrame()->findAllElements("#translatable\\.dosage\\.route").first().setInnerXml(tr("Route:"));

    //The prediction section
    page()->mainFrame()->findAllElements("#translatable\\.prediction\\.title").first().setInnerXml(tr("Prediction:"));
    page()->mainFrame()->findAllElements("#translatable\\.prediction\\.extrapolated").first().setInnerXml(tr("Extrapolated trough:"));
    page()->mainFrame()->findAllElements("#translatable\\.prediction\\.target\\.residual\\.best").first().setInnerXml(tr("Target trough:"));
    page()->mainFrame()->findAllElements("#translatable\\.parameters\\.identifier").first().setInnerXml(tr(""));
    page()->mainFrame()->findAllElements("#translatable\\.parameters\\.population").first().setInnerXml(tr("Standard"));
    page()->mainFrame()->findAllElements("#translatable\\.parameters\\.reference").first().setInnerXml(tr("A priori"));
    page()->mainFrame()->findAllElements("#translatable\\.parameters\\.patient").first().setInnerXml(tr("A posteriori"));

    //The interpretation section
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.title").first().setInnerXml(tr("Comments:"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.author").first().setInnerXml(tr("Author:"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.date").first().setInnerXml(tr("Date:"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.proposed").first().setInnerXml(tr("Dosage adjustment proposed:"));
    page()->mainFrame()->findAllElements("#translatable\\.interpretation\\.next").first().setInnerXml(tr("Next TDM suggested:"));

    //The references section
    page()->mainFrame()->findAllElements("#translatable\\.references").first().setInnerXml(tr("References:"));

    //The shared strings
    collection = page()->mainFrame()->findAllElements(".translatable\\.phone");
    for (int i = 0; i < collection.count(); ++i)
        collection.at(i).setInnerXml(tr("Phone:"));
    collection = page()->mainFrame()->findAllElements(".translatable\\.fax");
    for (int i = 0; i < collection.count(); ++i)
        collection.at(i).setInnerXml(tr("Fax:"));
    collection = page()->mainFrame()->findAllElements(".translatable\\.id\\.start");
    for (int i = 0; i < collection.count(); ++i)
        collection.at(i).setInnerXml(tr("(ID:"));
    collection = page()->mainFrame()->findAllElements(".translatable\\.id\\.stop");
    for (int i = 0; i < collection.count(); ++i)
        collection.at(i).setInnerXml(tr(")"));
    collection = page()->mainFrame()->findAllElements(".translatable\\.at");
    for (int i = 0; i < collection.count(); ++i)
        collection.at(i).setInnerXml(tr("at"));
    collection = page()->mainFrame()->findAllElements(".translatable\\.comments");
    for (int i = 0; i < collection.count(); ++i)
        collection.at(i).setInnerXml(tr("Comments:"));

    //Remove the translator
    QApplication::removeTranslator(&translator);
}
