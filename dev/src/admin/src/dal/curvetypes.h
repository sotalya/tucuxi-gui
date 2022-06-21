#ifndef CURVETYPES_H
#define CURVETYPES_H

#include <QObject>

namespace Tucuxi {
namespace Gui {
namespace Admin {

class CurveType : public QObject
{
    Q_OBJECT
public:
    enum Enum
    {
        PopulationPrediction = 0,
        PopulationPercentiles,
        APrioriPrediction,
        AprioriPercentiles,
        APosterioriPrediction,
        APosterioriPercentiles,
        Measure,
        Target,
        PossibleAdjustments,
        SelectedAdjustments
    };
    Q_ENUM(CurveType::Enum)

    static const int first = Enum::PopulationPrediction;
    static const int last = Enum::SelectedAdjustments;
    static const int size = last + 1;
    inline static Enum convert(int value) { return static_cast<Enum>(value); }
    inline static bool isValid(int value) { return (value >= 0 && value<size); }
};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::CurveType::Enum)

#endif // CURVETYPES_H
