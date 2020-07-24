#ifndef APPMODE_H
#define APPMODE_H

#include <QObject>

class AppMode : public QObject
{
private:
    Q_OBJECT
    static AppMode* m_instance;
    AppMode();

public:
    static AppMode* getInstance(){
        if (!m_instance){
            m_instance = new AppMode();
        }
        return m_instance;
    }
    Q_INVOKABLE bool isDemo();
    Q_INVOKABLE bool isEducational();
    Q_INVOKABLE bool isInstitutional();
    Q_INVOKABLE bool isPractice();

};

#endif // APPMODE_H
