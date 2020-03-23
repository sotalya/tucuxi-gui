#include "reportsqlitedao.h"
#include "core.h"
#include "reportrepository.h"
#include "report.h"
#include "reportdata.h"
#include "reportfactory.h"
#include "patient.h"
#include "stdadminrepository.h"
#include "corerepository.h"

using namespace ezechiel::core;

Report *ReportSQLiteDAO::findById(int id)
{
    Response response;

    QScopedPointer<ReportData> data(new ReportData());

    response = REPORTREPO->getReport(id, *data.data());
    if (response.error == NotFound) {
        setError(tr("The report with SQL ID '%1' could not be found.").arg(id));
        return 0;
    }
    if (response.error != NoError) {
        setError(tr("An unexpected error occurred while retrieving the report with ID '%1'.\n\n%2").arg(QString::number(id), response.message));
        return 0;
    }

    enforceFKConstraint(data.data());

    ReportFactory factory;

    return factory.create(data.take());
}

Report *ReportSQLiteDAO::findByValue(const QString &field, const QVariant &value)
{
    if (ReportData::staticMetaObject.indexOfProperty(field.toStdString().c_str()) == -1) {
        setError(tr("The field '%1' is not a property of the reports."));
        return 0;
    }

    Response response;

    QScopedPointer<ReportData> data(new ReportData());

    response = REPORTREPO->getReport(field, value, *data.data());
    if (response.error == NotFound) {
        setError(response.message);
        return 0;
    }
    if (response.error != NoError) {
        setError(tr("An unexpected error occurred while retrieving the report with the field '%1' set to '%2'.\n\n%3").arg(field, value.toString(), response.message));
        return 0;
    }

    enforceFKConstraint(data.data());

    ReportFactory factory;

    return factory.create(data.take());
}

QList<Report *> ReportSQLiteDAO::findAll()
{
    Response response;

    QList<ReportData *> dataList;
    if ((response = REPORTREPO->getReportsList(dataList)).error != NoError) {
        setError(tr("An unexpected error occurred while retrieving the reports list.\n\n%1").arg(response.message));
        return QList<Report *>();
    }

    ReportFactory factory;
    QList<Report *> reportList;

    for (int i = 0; i < dataList.size(); ++i)
        reportList.append(factory.create(dataList.at(i)));

    return reportList;
}

bool ReportSQLiteDAO::save(Report *report)
{
    Response response;

    enforceFKConstraint(report->data());

    if ((response = REPORTREPO->setReport(*report->data())).error != NoError) {
        setError(tr("An unexpected error occurred while saving the report with ID '%1'.\n\n%2").arg(QString::number(report->reportId()), response.message));
        return false;
    }

    return true;
}

bool ReportSQLiteDAO::remove(const Report *report)
{
    Response response;

    if ((response = REPORTREPO->deleteReport(*report->data())).error != NoError) {
        setError(tr("An unexpected error occurred while deleting the report with ID '%1'.\n\n%2").arg(QString::number(report->reportId()), response.message));
        return false;
    }

    return true;
}

/* Ensures that the FK are valid.
 * Ensures that the FK are valid when saving or loading a report from the database. It is necessary
 * to ensure that the element corresponding to a foreign key still exists in the database when
 * loading or saving the report. If not done, it would lead to foreign key constraint violation.
 * @param data The report's data.
 */
void ReportSQLiteDAO::enforceFKConstraint(ReportData *data)
{
    //Clear the patient ID if invalid or nonexistent
    SharedPatient patient;
//    if (ADMINREPO->getPatient(data->patientId(), patient).error != NoError)
    if (ADMINREPO->getPatientFromId(data->patientId(), patient).error != NoError)
        data->unsetPatientId();

    //Clear the analyst ID if invalid or nonexistent
    SharedPractician analyst;
//    if (REPO->getPractician(data->analystId(), analyst).error != NoError)
    if (ADMINREPO->getPracticianFromId(data->analystId(), analyst).error != NoError)
        data->unsetAnalystId();

    //Clear the recipient ID if invalid or nonexistent
    SharedPractician recipient;
//    if (REPO->getPractician(data->recipientId(), recipient).error != NoError)
    if (ADMINREPO->getPracticianFromId(data->recipientId(), recipient).error != NoError)
        data->unsetRecipientId();

    //Clear the institute ID if invalid or nonexistent
    SharedInstitute institute;
//    if (REPO->getInstitute(data->instituteId(), institute).error != NoError)
    if (ADMINREPO->getInstituteFromId(data->instituteId(), institute).error != NoError)
        data->unsetInstituteId();

    //Clear the treatment ID if invalid or nonexistent
    Prediction* curve;
    if (REPO->getPredictionFromId(data->treatmentId(), curve).error != NoError)
        data->unsetTreatmentId();
}
