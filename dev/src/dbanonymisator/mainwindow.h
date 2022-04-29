//@@license@@

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void browseClicked();
    void evaluateClicked();
    void anonymizeClicked();
    void evaluationFinished(bool result);
    void anonymisationStatus(bool result);
private:
    Ui::MainWindow *ui;
    QPushButton* evaluateButton;
    QPushButton* anonymizeButton;
};

#endif // MAINWINDOW_H
