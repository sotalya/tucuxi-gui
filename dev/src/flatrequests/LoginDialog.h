#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>
#include <QLineEdit>

namespace Tucuxi {
namespace Gui {
namespace FlatRequest {

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);

    QString getUsername() const;
    QString getPassword() const;

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
};

}
}
}

#endif // LOGINDIALOG_H
