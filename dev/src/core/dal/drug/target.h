//@@license@@

#ifndef TARGET_H
#define TARGET_H

#include "core/dal/entity.h"
#include "core/dal/amount.h"
#include "core/dal/drug/operableamount.h"
#include "core/utils/ezutils.h"

Q_MOC_INCLUDE("core/dal/drug/translatablestring.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


//class OperableAmount;
class TranslatableString;

class TargetMethod : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(TargetMethod)
    Q_PROPERTY(TargetType type READ getTargetType WRITE setTargetType NOTIFY typeChanged)

    Q_PROPERTY(QString label READ getLabel WRITE setLabel NOTIFY labelChanged) \
    Q_PROPERTY(int value READ getValue WRITE setValue NOTIFY valueChanged) \
    public: \
        enum TargetType {
        ResidualTarget = 0,  //! Targeted residual concentration, or targetted concentration at the end of a complete cycle.
        PeakTarget,          //! Targeted peak concentration. WARNING: It does not correspond to the maximum of a cycle, but the value at a specific time after the intake.
        MeanTarget,          //! Targeted mean concentration, currently unused.
        AUCTarget,           //! Targeted AUC concentration, currently unused.
        AUC24Target,           //! Targeted AUC concentration, currently unused.
        CumulativeAUCTarget, //! Targeted cumulative AUC concentration.
        AUCDividedByMicTarget, //! Soon to be supported target
        AUC24DividedByMicTarget, //! Soon to be supported target
        AUCOverMicTarget, //! Soon to be supported target
        AUC24OverMicTarget, //! Soon to be supported target
        PeakDividedByMicTarget, //! Soon to be supported target
        TimeOverMicTarget, //! Soon to be supported target
        ResidualDividedByMicTarget, //! Targeted residual concentration divided by MIC
        FractionTimeOverMicTarget, //! Fraction of time spent over the MIC [0,1]
        UnknownTarget        //! Unknown or invalid target.
        };
        Q_ENUM(TargetType)

    Q_INVOKABLE TargetMethod(AbstractRepository *repository, QObject *parent = nullptr)
        : Entity(repository, parent),
        _type( ResidualTarget)
        {
        }

    Q_INVOKABLE TargetMethod(AbstractRepository *repository, QObject *parent, TargetMethod* admin)
        : Entity(repository, parent),
        _type(admin->getTargetType())
        {
        }

        QString getLabel () const {
            return QMetaEnum::fromType<TargetType>().key(_type);
        }
        void setLabel(QString label) {
            QMetaEnum menum = QMetaEnum::fromType<TargetType>();
            _type = static_cast<TargetType>(menum.keyToValue(&label.toStdString()[0]));
            emit valueChanged(menum.keyToValue(menum.key(_type)));
            emit labelChanged(menum.key(_type));
        }

        int getValue () const {
            QMetaEnum menum = QMetaEnum::fromType<TargetType>();
            return menum.keyToValue(menum.key(_type));
        }

        void setValue(int value) {
            _type = static_cast<TargetType>(value);
            QMetaEnum menum = QMetaEnum::fromType<TargetType>();
            emit valueChanged(menum.keyToValue(menum.key(_type)));
            emit labelChanged(menum.key(_type));
        }

        void setTargetType(TargetType type) { _type = type; emit typeChanged(_type); }
        TargetType getTargetType() const { return _type; }
    Q_SIGNAL void labelChanged (QString value); \
    Q_SIGNAL void valueChanged (int value); \
    Q_SIGNAL void typeChanged (TargetType type); \

private:
    TargetType _type;
};

QML_POINTERLIST_CLASS_DECL(TargetMethodList, TargetMethod)

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::TargetMethod*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::TargetMethod*>)

namespace Tucuxi {
namespace Gui {
namespace Core {


class Target : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Target)

    AUTO_PROPERTY_DECL(TargetMethod*, type, Type)

    //! Minimum concentration value
    AUTO_PROPERTY_DECL(OperableAmount*, cmin, Cmin)

    //! Maximum concentration value
    AUTO_PROPERTY_DECL(OperableAmount*, cmax, Cmax)

    //! Optimal concentration value
    AUTO_PROPERTY_DECL(OperableAmount*, cbest, Cbest)

    //! Minimum time value
    AUTO_PROPERTY_DECL(OperableAmount*, tmin, Tmin)

    //! Maximum time value
    AUTO_PROPERTY_DECL(OperableAmount*, tmax, Tmax)

    //! Best time value
    AUTO_PROPERTY_DECL(OperableAmount*, tbest, Tbest)

    //! Minimum inhibitory concentration
    AUTO_PROPERTY_DECL(OperableAmount*, mic, Mic)

    AUTO_PROPERTY_DECL(TranslatableString*, comments, Comments)

///used with commented Q_PROPERTYs above
//    public:
//    //cUnit
//    QString getCUnit() { return _cmin->unit().toString();}
//    void setCUnit(QString unit) { _cmin->setUnit(Unit(unit)); _cmax->setUnit(Unit(unit)); _cbest->setUnit(Unit(unit)); emit cUnitChanged(unit); }
//    //tUnit
//    QString getTUnit() { return _tmin->unit().toString(); }
//    void setTUnit(QString unit) { _tmin->setUnit(Unit(unit)); _tmax->setUnit(Unit(unit)); _tbest->setUnit(Unit(unit)); emit tUnitChanged(unit); }

//signals:
//    void cUnitChanged(QString);
//    void tUnitChanged(QString);

protected:

    Q_INVOKABLE explicit Target(AbstractRepository *repository, QObject *parent = nullptr);

public:

//    enum TargetType {
//        ResidualTarget = 1, //! Targeted residual concentration, or targetted concentration at the end of a complete cycle.
//        PeakTarget,     //! Targeted peak concentration. WARNING: It does not correspond to the maximum of a cycle, but the value at a specific time after the intake.
//        MeanTarget,     //! Targeted mean concentration, currently unused.
//        UnknownTarget   //! Unknown or invalid target.
//    };
//    Q_ENUMS(TargetType)

//    TargetMethod* getType() const { return _type; }
//    void setType(TargetType tartype) { _type = tartype; emit typeChanged(tartype);}

    void copyFrom(Target *other);
//    bool isValid();

    //! Translate a target type into a translated string
//    static QString name (TargetType type);

static const QMap<QString, TargetMethod::TargetType> _typeMap;
static QMap<QString, TargetMethod::TargetType> createMap() {
    QMap<QString, TargetMethod::TargetType> amap;
    amap["Peak"] = TargetMethod::PeakTarget;
    amap["Residual"] = TargetMethod::ResidualTarget;
    amap["Mean"] = TargetMethod::MeanTarget;
    amap["AUC"] = TargetMethod::AUCTarget;
    amap["AUC24"] = TargetMethod::AUC24Target;
    amap["CumulativeAUC"] = TargetMethod::CumulativeAUCTarget;
    amap["AUCDividedByMic"] = TargetMethod::AUCDividedByMicTarget;
    amap["AUC24DividedByMic"] = TargetMethod::AUC24DividedByMicTarget;
    amap["AUCOverMicTarget"] = TargetMethod::AUCOverMicTarget;
    amap["AUC24OverMicTarget"] = TargetMethod::AUC24OverMicTarget;
    amap["PeakDividedByMic"] = TargetMethod::PeakDividedByMicTarget;
    amap["ResidualDividedByMic"] = TargetMethod::ResidualDividedByMicTarget;
    amap["TimeOverMic"] = TargetMethod::TimeOverMicTarget;
    amap["FractionTimeOverMic"] = TargetMethod::FractionTimeOverMicTarget;
    return amap;
}

//signals:
//    void typeChanged(TargetType);
//private:
//    TargetType _type;
};



QML_POINTERLIST_CLASS_DECL(TargetList, Target)

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Target*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Target*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::TargetList*)

#endif // TARGET_H
