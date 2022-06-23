#ifndef STEPTYPES_H
#define STEPTYPES_H

#include <QObject>

namespace Tucuxi {
namespace Gui {
namespace Admin {

//    Q_NAMESPACE

class StepType : public QObject
{
    Q_OBJECT

public:
    enum Enum
    {
        Patient = 0,
        Drug,
        Dosage,
        Covariate,
        Measure,
        Target,
        Adjustment,
        Validation,
        Report
    };
    Q_ENUM(StepType::Enum)

    static const int first = Enum::Patient;
    static const int last = Enum::Report;
    static const int size = last + 1;
    inline static Enum convert(int value) { return static_cast<Enum>(value); }
    inline static bool isValid(int value) { return (value >= 0 && value<size); }

};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::StepType::Enum)

#endif // STEPTYPES_H
