#ifndef APPGLOBALS_H
#define APPGLOBALS_H

#include <QObject>

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

private:
    Q_OBJECT
    AppGlobals();
    static AppGlobals* m_instance;

    QString m_listFile;
    QString m_requestFile;
};

#endif // APPGLOBALS_H
