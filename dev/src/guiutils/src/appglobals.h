//@@license@@

#ifndef APPGLOBALS_H
#define APPGLOBALS_H

#include <QObject>

namespace Tucuxi {
namespace Gui {
namespace Admin {
class Practician;
}
}
}

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

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

    void setIccaFile(QString _iccaFile);
    void setIccaImport(bool enable);

    void setGroupIntake(bool enable);

    const QString getListFile();

    const QString getRequestFile();

    const QString getIccaFile();
    bool getIccaImport();

    bool getGroupIntake();

    Q_INVOKABLE Tucuxi::Gui::Admin::Practician *getAnalyst();

    Q_INVOKABLE void saveAnalystSettings();

    Q_INVOKABLE bool showProcessingTime();
    Q_INVOKABLE void setShowProcessingTime(bool show);

    Q_INVOKABLE bool autoCalculation();
    Q_INVOKABLE void setAutoCalculation(bool enable);

    Q_INVOKABLE bool percentileCalculation();
    Q_INVOKABLE void setPercentileCalculation(bool enable);

private:
    Q_OBJECT
    AppGlobals();

    // Declared private as it is automatically called when we get the analyst the first time
    void loadAnalystSettings();

    static AppGlobals* m_instance;

    QString m_listFile;
    QString m_requestFile;
    QString m_iccaFile;

    Tucuxi::Gui::Admin::Practician *m_analyst;

    bool m_showProcessingTime;
    bool m_autoCalculation;
    bool m_percentileCalculation;

    bool m_groupIntake;
    bool m_iccaImport;
};

}
}
}


#endif // APPGLOBALS_H
