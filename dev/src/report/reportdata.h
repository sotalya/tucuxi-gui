//@@license@@

#ifndef REPORTDATA_H
#define REPORTDATA_H

#include <QObject>
#include <QDateTime>
#include <QStringList>
#include <QHash>
#include <QImage>

class ReportData : public QObject
{
    Q_OBJECT

    //Database columns
    Q_PROPERTY(int id READ reportId WRITE setReportId RESET unsetReportId)
    Q_PROPERTY(QString template READ templateName WRITE setTemplateName)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString reference READ reference WRITE setReference)
    Q_PROPERTY(QDateTime creation READ creationDate WRITE setCreationDate)
    Q_PROPERTY(QDateTime publication READ publicationDate WRITE setPublicationDate)
    Q_PROPERTY(int patientId READ patientId WRITE setPatientId RESET unsetPatientId)
    Q_PROPERTY(int analystId READ analystId WRITE setAnalystId RESET unsetAnalystId)
    Q_PROPERTY(int recipientId READ recipientId WRITE setRecipientId RESET unsetRecipientId)
    Q_PROPERTY(int instituteId READ instituteId WRITE setInstituteId RESET unsetInstituteId)
    Q_PROPERTY(int treatmentId READ treatmentId WRITE setTreatmentId RESET unsetTreatmentId)

public:
    ReportData(QObject *parent = 0);
    virtual ~ReportData();

    //Members accessors
    QString templateName() const;
    void setTemplateName(const QString &name);

    QString language() const;
    void setLanguage(const QString &language);

    QString title() const;
    void setTitle(const QString &title);

    QString reference() const;
    void setReference(const QString &reference);

    QDateTime creationDate() const;
    void setCreationDate(const QDateTime &date);

    QDateTime publicationDate() const;
    void setPublicationDate(const QDateTime &date);

    int reportId() const;
    void setReportId(int id);
    void unsetReportId();

    int patientId() const;
    void setPatientId(int id);
    void unsetPatientId();

    int analystId() const;
    void setAnalystId(int id);
    void unsetAnalystId();

    int recipientId() const;
    void setRecipientId(int id);
    void unsetRecipientId();

    int instituteId() const;
    void setInstituteId(int id);
    void unsetInstituteId();

    int treatmentId() const;
    void setTreatmentId(int id);
    void unsetTreatmentId();

    //Sections accessors (JS accessible)
    Q_INVOKABLE QStringList hiddenSections() const;
    Q_INVOKABLE void setHiddenSections(const QStringList &sections);
    Q_INVOKABLE void hideSection(const QString &section);
    Q_INVOKABLE void showSection(const QString &section);
    Q_INVOKABLE bool containsSection(const QString &section) const;

    //Values accessors (JS accessible)
    Q_INVOKABLE QHash<QString, QString> values() const;
    Q_INVOKABLE void setValues(const QHash<QString, QString> &values);
    Q_INVOKABLE void setValue(const QString &id, const QString &value);
    Q_INVOKABLE void unsetValue(const QString &id);
    Q_INVOKABLE QString value(const QString &id) const;
    Q_INVOKABLE bool containsValue(const QString &id) const;

    //Images accessors (JS accessible)
    Q_INVOKABLE QHash<QString, QImage> images() const;
    Q_INVOKABLE void setImages(const QHash<QString, QImage> &images);
    Q_INVOKABLE void setImage(const QString &id, const QImage &image);
    Q_INVOKABLE void unsetImage(const QString &id);
    Q_INVOKABLE QImage image(const QString &id) const;
    Q_INVOKABLE bool containsImage(const QString &id) const;

signals:
    void sectionHidden(const QString &section);
    void sectionShown(const QString &section);
    void sectionsReset();

    void valueChanged(const QString &id, const QString &value);
    void valuesReset();

    void imageChanged(const QString &id, const QImage &image);
    void imagesReset();

private:
    int _reportId;
    QString _templateName;
    QStringList _hiddenSections;
    QHash<QString, QString> _data;
    QHash<QString, QImage> _images;
};

#endif // REPORTDATA_H
