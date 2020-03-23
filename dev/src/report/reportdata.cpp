#include "reportdata.h"
#include "identifiers.h"

ReportData::ReportData(QObject *parent) : QObject(parent),
    _reportId(-1),
    _templateName(),
    _hiddenSections(),
    _data(),
    _images()
{

}

ReportData::~ReportData()
{

}

QString ReportData::templateName() const
{
    return _templateName;
}

void ReportData::setTemplateName(const QString &name)
{
    _templateName = name;
}

QString ReportData::language() const
{
    return value(ID::Language);
}

void ReportData::setLanguage(const QString &language)
{
    setValue(ID::Language, language);
}

QString ReportData::title() const
{
    return value(ID::Title);
}

void ReportData::setTitle(const QString &title)
{
    setValue(ID::Title, title);
}

QString ReportData::reference() const
{
    return value(ID::Reference);
}

void ReportData::setReference(const QString &reference)
{
    setValue(ID::Reference, reference);
}

QDateTime ReportData::creationDate() const
{
    return QDateTime::fromString(value(ID::Creation), Qt::ISODate);
}

void ReportData::setCreationDate(const QDateTime &date)
{
    setValue(ID::Creation, date.toString(Qt::ISODate));
}

QDateTime ReportData::publicationDate() const
{
    return QDateTime::fromString(value(ID::Publication), Qt::ISODate);
}

void ReportData::setPublicationDate(const QDateTime &date)
{
    setValue(ID::Publication, date.toString(Qt::ISODate));
}

int ReportData::reportId() const
{
    return _reportId;
}

void ReportData::setReportId(int id)
{
    id < 0 ? _reportId = -1 : _reportId = id;
}

void ReportData::unsetReportId()
{
    _reportId = -1;
}

int ReportData::patientId() const
{
    return containsValue(ID::Patient::Id) ? value(ID::Patient::Id).toInt() : -1;
}

void ReportData::setPatientId(int id)
{
    id < 0 ? unsetValue(ID::Patient::Id) : setValue(ID::Patient::Id, QString::number(id));
}

void ReportData::unsetPatientId()
{
    unsetValue(ID::Patient::Id);
}

int ReportData::analystId() const
{
    return containsValue(ID::Analyst::Id) ? value(ID::Analyst::Id).toInt() : -1;
}

void ReportData::setAnalystId(int id)
{
    id < 0 ? unsetValue(ID::Analyst::Id) : setValue(ID::Analyst::Id, QString::number(id));
}

void ReportData::unsetAnalystId()
{
    unsetValue(ID::Analyst::Id);
}

int ReportData::recipientId() const
{
    return containsValue(ID::Recipient::Id) ? value(ID::Recipient::Id).toInt() : -1;
}

void ReportData::setRecipientId(int id)
{
    id < 0 ? unsetValue(ID::Recipient::Id) : setValue(ID::Recipient::Id, QString::number(id));
}

void ReportData::unsetRecipientId()
{
    unsetValue(ID::Recipient::Id);
}

int ReportData::instituteId() const
{
    return containsValue(ID::Institute::Id) ? value(ID::Institute::Id).toInt() : -1;
}

void ReportData::setInstituteId(int id)
{
    id < 0 ? unsetValue(ID::Institute::Id) : setValue(ID::Institute::Id, QString::number(id));
}

void ReportData::unsetInstituteId()
{
    unsetValue(ID::Institute::Id);
}

int ReportData::treatmentId() const
{
    return containsValue(ID::Treatment::Id) ? value(ID::Treatment::Id).toInt() : -1;
}

void ReportData::setTreatmentId(int id)
{
    id < 0 ? unsetValue(ID::Treatment::Id) : setValue(ID::Treatment::Id, QString::number(id));
}

void ReportData::unsetTreatmentId()
{
    unsetValue(ID::Treatment::Id);
}

QStringList ReportData::hiddenSections() const
{
    return _hiddenSections;
}

void ReportData::setHiddenSections(const QStringList &sections)
{
    _hiddenSections = sections;
    emit sectionsReset();;
}

void ReportData::hideSection(const QString &section)
{
    if (_hiddenSections.contains(section))
        return;

    _hiddenSections.append(section);
    emit sectionHidden(section);
}

void ReportData::showSection(const QString &section)
{
    if (!_hiddenSections.contains(section))
        return;

    _hiddenSections.removeAll(section);
    emit sectionShown(section);
}

bool ReportData::containsSection(const QString &section) const
{
    return !_hiddenSections.contains(section);
}

QHash<QString, QString> ReportData::values() const
{
    return _data;
}

void ReportData::setValues(const QHash<QString, QString> &values)
{
    _data = values;
    emit valuesReset();
}

void ReportData::setValue(const QString &id, const QString &value)
{
    if (_data.value(id) == value)
        return;

    _data.insert(id, value);
    emit valueChanged(id, value);
}

void ReportData::unsetValue(const QString &id)
{
    if (!_data.contains(id))
        return;

    _data.remove(id);
    emit valueChanged(id, "");
}

QString ReportData::value(const QString &id) const
{
    return _data.value(id);
}

bool ReportData::containsValue(const QString &id) const
{
    return _data.contains(id);
}

QHash<QString, QImage> ReportData::images() const
{
    return _images;
}

void ReportData::setImages(const QHash<QString, QImage> &images)
{
    _images = images;
    emit imagesReset();
}

void ReportData::setImage(const QString &id, const QImage &image)
{
    if (_images.value(id) == image)
        return;

    _images.insert(id, image);
    emit imageChanged(id, image);
}

void ReportData::unsetImage(const QString &id)
{
    if (!_images.contains(id))
        return;

    _images.remove(id);
    emit imageChanged(id, QImage());
}

QImage ReportData::image(const QString &id) const
{
    return _images.value(id);
}

bool ReportData::containsImage(const QString &id) const
{
    return _images.contains(id);
}
