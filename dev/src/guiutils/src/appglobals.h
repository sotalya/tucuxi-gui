//@@license@@

#ifndef APPGLOBALS_H
#define APPGLOBALS_H

#include <QObject>

class Practician;

class AppGlobals : public QObject
{    
public:
    static AppGlobals* getInstance(){
        if (m_instance == nullptr){
            m_instance = new AppGlobals;
        }
        return m_instance;
    }

    void setListFile(QString _listFile);

    void setRequestFile(QString _requestFile);

    const QString getListFile();

    const QString getRequestFile();

    Q_INVOKABLE Practician *getAnalyst();

    Q_INVOKABLE void saveAnalystSettings();

    Q_INVOKABLE bool showProcessingTime();
    Q_INVOKABLE void setShowProcessingTime(bool show);

private:
    Q_OBJECT
    AppGlobals();

    // Declared private as it is automatically called when we get the analyst the first time
    void loadAnalystSettings();

    static AppGlobals* m_instance;

    QString m_listFile;
    QString m_requestFile;

    Practician *m_analyst;

    bool m_showProcessingTime;
};

#endif // APPGLOBALS_H
