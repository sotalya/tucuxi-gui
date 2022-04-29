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
    void evaluationFinished(bool result, QStringList patches);
    void updateStatus(bool result);
private:
    Ui::MainWindow *ui;
    QPushButton* evaluateButton;
    QPushButton* updateButton;
};

#endif // MAINWINDOW_H
