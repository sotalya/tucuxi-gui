//@@license@@

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QNetworkReply;
class QProgressBar;
class AbstractTestWidget;

#include <QMainWindow>
#include <QSslError>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    static const char *const _REQUEST_WIDGET_BUTTON_TEXT;
    static const char *const _NOTIFY_WIDGET_BUTTON_TEXT;

    void initTestWidgets();

    Ui::MainWindow *_ui;
    AbstractTestWidget *_listWidget;
    AbstractTestWidget *_requestWidget;
    AbstractTestWidget *_notifyWidget;

private slots:
    void testStarted(AbstractTestWidget *);
    void testStopped(AbstractTestWidget *);
    void errors(const QString &);
    void sslErros(QNetworkReply *, QList<QSslError>);
    void reset();
    void requestSelected(const QString &requestId);
};

#endif // MAINWINDOW_H
