#ifndef CALCULATIONREQUESTLOGGER_H
#define CALCULATIONREQUESTLOGGER_H

#include <QString>
#include <QMutex>

namespace Tucuxi {
namespace GuiCore {
class PredictionSpec;
}

}

class CalculationRequestLogger
{
public:

    static CalculationRequestLogger *getInstance();


    void log(Tucuxi::GuiCore::PredictionSpec *spec);

    void setPath(const QString &path);

    void setEnable(bool enable);

private:
    CalculationRequestLogger();

    QString path;
    QString currentPath;
    bool enabled;

    QMutex mutex;

};

#endif // CALCULATIONREQUESTLOGGER_H
