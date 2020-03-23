#ifndef STARTUPSCREEN_H
#define STARTUPSCREEN_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>

class StartupScreen : public QDialog
{
    Q_OBJECT
public:
    explicit StartupScreen(QWidget *parent = nullptr);

protected:
    QTextEdit *text;
    QPushButton *buttonYes;
    QPushButton *buttonNo;
signals:

public slots:

};

#endif // STARTUPSCREEN_H
