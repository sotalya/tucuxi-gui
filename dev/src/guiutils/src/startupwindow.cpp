//@@license@@

#include "startupwindow.h"

#include <QQmlContext>
#include <QQuickItem>

using namespace Tucuxi::Gui::GuiUtils;

StartupWindow::StartupWindow(QWidget *parent) :
    QQuickWidget(parent)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowModality(Qt::WindowModal);

    rootContext()->setContextProperty("window", this);
    //setSource(QUrl("qrc:/windows/StartupWindow.qml"));
    setSource(QUrl("qrc:/windows/StartupScreen.qml"));

    connect(rootObject(), SIGNAL(optionSelected(int)), this, SLOT(close()));
    connect(rootObject(), SIGNAL(optionSelected(int)), this, SIGNAL(optionSelected(int)));
}

StartupWindow::~StartupWindow()
{

}

void StartupWindow::setWidth(int width)
{
    setMinimumWidth(width);
}

void StartupWindow::setHeight(int height)
{
    setMinimumHeight(height);
}
