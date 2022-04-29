//@@license@@

#include "licensedialog.h"

#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

char rfc3986[256] = {0};
char html5[256] = {0};
void url_encoder_rfc_tables_init()
{
    int i;
    for (i = 0; i < 256; i++){
        rfc3986[i] = isalnum( i) || i == '~' || i == '-' || i == '.' || i == '_' ? i : 0;
        html5[i] = isalnum( i) || i == '*' || i == '-' || i == '.' || i == '_' ? i : (i == ' ') ? '+' : 0;
    }
}
char *url_encode( char *table, unsigned char *s, char *enc)
{
    for (; *s; s++){
        if (table[*s]) sprintf( enc, "%c", table[*s]);
        else sprintf( enc, "%%%02X", *s);
        while (*++enc);
    }
    return( enc);
}


LicenseDialog::LicenseDialog(const QString &_request)
{
    QTextEdit* requestTE = new QTextEdit(_request);
    requestTE->setReadOnly(true);
    requestTE->setMinimumSize(500, 50);
    requestTE->setMaximumSize(1000, 100);

    licenseTE = new QTextEdit();
    licenseTE->setMinimumSize(500, 50);
    licenseTE->setMaximumSize(1000, 100);

    QPushButton *installButton = new QPushButton(tr("Install license"));
    QPushButton *exitButton = new QPushButton(tr("Exit"));
    connect(installButton, &QAbstractButton::clicked, this, &LicenseDialog::handleInstallLicense);
    connect(exitButton, &QAbstractButton::clicked, this, &QWidget::close);

//    QString encodedRequest = QUrl::toPercentEncoding(_request);
    QString url = QString("<a href='http://license.tucuxi.ch/license.html?request=%1'>Tucuxi License Server</a>").arg(_request);

    QLabel *introLbl1 = new QLabel("\nA license is needed to run Tucuxi but no license is installed yet on this machine.");
    //introLbl->setOpenExternalLinks(true);
    //introLbl->setWordWrap(true);
    QLabel *introLbl2 = new QLabel(QString("Please follow the process below to obtain right away a free license from the %1.").arg(url));
    introLbl2->setOpenExternalLinks(true);
    QLabel *requestLbl = new QLabel("1. Note the request data");
    requestLbl->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    QLabel *getLicenseLbl = new QLabel("2. Follow the link to get a free license ");
    getLicenseLbl->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    QLabel *getLicenseUrl = new QLabel(url);
    getLicenseUrl->setOpenExternalLinks(true);
    QLabel *licenseLbl = new QLabel("3. Install the license");
    licenseLbl->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(requestLbl, 1, 1);
    gridLayout->addWidget(requestTE, 1, 2);
    gridLayout->addWidget(getLicenseLbl, 2, 1);
    gridLayout->addWidget(getLicenseUrl, 2, 2);
    gridLayout->addWidget(licenseLbl, 3, 1);
    gridLayout->addWidget(licenseTE, 3, 2);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(installButton);
    buttonLayout->addWidget(exitButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(introLbl1);
    mainLayout->addWidget(introLbl2);
    mainLayout->addLayout(gridLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("License Dialog"));
}

void LicenseDialog::handleInstallLicense()
{
    m_license = licenseTE->toPlainText();
    accept();
}
