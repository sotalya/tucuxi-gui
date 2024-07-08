//@@license@@

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "LoginDialog.h"

using namespace Tucuxi::Gui::FlatRequest;

LoginDialog::LoginDialog(QWidget *parent): QDialog(parent)
{
    // Create the username label and edit
    QLabel *usernameLabel = new QLabel("Username:");
    usernameEdit = new QLineEdit;

    // Create the password label and edit
    QLabel *passwordLabel = new QLabel("Password:");
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);

    // Create the OK and Cancel buttons
    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancel");

    // Create the layout and add the widgets
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameEdit);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordEdit);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // Set the layout and connect signals and slots
    setLayout(mainLayout);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString LoginDialog::getUsername() const
{
    return usernameEdit->text();
}

QString LoginDialog::getPassword() const
{
    return passwordEdit->text();
}
