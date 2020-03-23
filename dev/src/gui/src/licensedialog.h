
#include <QObject>
#include <QDialog>

class QTextEdit;

class LicenseDialog : public QDialog
{
    Q_OBJECT

public:
    LicenseDialog(const QString &_request);

    std::string getLicense() { return m_license.toLatin1().data(); }

public slots:
    void handleInstallLicense();

private:
    QTextEdit *licenseTE;
    QString m_license;
};
