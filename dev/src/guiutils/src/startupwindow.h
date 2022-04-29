//@@license@@

#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QQuickWidget>

class StartupWindow : public QQuickWidget
{
    Q_OBJECT

public:
    enum Option {
        PendingRequests,
        PendingRequestGivenPatient,
        NewPatient,
        LoadRequest,
        Shutdown
    };
    Q_ENUM(Option)

    explicit StartupWindow(QWidget *parent = 0);
    virtual ~StartupWindow();

    Q_INVOKABLE void setWidth(int width);
    Q_INVOKABLE void setHeight(int height);

signals:
    void optionSelected(int option);
};

#endif // STARTUPWINDOW_H
