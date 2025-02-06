/*
 * Tucuxi - Tucuxi-gui software.
 * This software is able to perform prediction of drug concentration in blood
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV.
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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

    void setFlatFile(QString _flatFile);
    void setFlatImport(bool enable);

    void setGroupIntake(bool enable);

    const QString getListFile();

    const QString getRequestFile();

    const QString getFlatFile();
    bool getFlatImport();

    bool getGroupIntake();

    Q_INVOKABLE Tucuxi::Gui::Admin::Practician *getAnalyst();

    Q_INVOKABLE void saveAnalystSettings();

    Q_INVOKABLE bool showProcessingTime();
    Q_INVOKABLE void setShowProcessingTime(bool show);

    Q_INVOKABLE bool autoCalculation();
    Q_INVOKABLE void setAutoCalculation(bool enable);

    Q_INVOKABLE bool percentileCalculation();
    Q_INVOKABLE void setPercentileCalculation(bool enable);

    Q_INVOKABLE QString loadCDSSReportPath();
    Q_INVOKABLE void saveCDSSReportPath();
    Q_INVOKABLE void updateCDSSReportPath(QString path);

    bool cdssOnly();
    void setCdssOnly(bool cdssOnly);
    Q_PROPERTY(bool cdssOnly READ cdssOnly WRITE setCdssOnly NOTIFY cdssOnlyChanged);

signals:
    void cdssOnlyChanged();

private:
    Q_OBJECT
    AppGlobals();

    // Declared private as it is automatically called when we get the analyst the first time
    void loadAnalystSettings();
    QString getDefaultPath();

    static AppGlobals* m_instance;

    QString m_cdssReportPath;
    QString m_listFile;
    QString m_requestFile;
    QString m_flatFile;

    Tucuxi::Gui::Admin::Practician *m_analyst;

    bool m_showProcessingTime;
    bool m_autoCalculation;
    bool m_percentileCalculation;

    bool m_groupIntake;
    bool m_flatImport;
    bool m_cdssOnly;
};

}
}
}


#endif // APPGLOBALS_H
