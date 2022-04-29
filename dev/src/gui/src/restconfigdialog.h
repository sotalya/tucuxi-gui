//@@license@@

#ifndef RESTCONFIGDIALOG_H
#define RESTCONFIGDIALOG_H

// ToDo-REST //////////////////////////////////////////////////////////////////
// Move in GUI-Utils and use QQuickView instead of QDialog
// ////////////////////////////////////////////////////////////////////////////

#include <QDialog>

namespace Ui {
    class RestConfigDialog;
}

class RestConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RestConfigDialog(QWidget *parent = 0);
    virtual ~RestConfigDialog() Q_DECL_OVERRIDE;

    QString getIp() const;
    QString getPort() const;
    QString getScheme() const;

private:
    Ui::RestConfigDialog *ui;
};

#endif // RESTCONFIGDIALOG_H
