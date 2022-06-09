#ifndef VALIDATIONSTATUS_H
#define VALIDATIONSTATUS_H

#include <QVector>

#include "core/dal/entity.h"
#include "steptypes.h"


class ValidationStatus : public ezechiel::GuiCore::Entity
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(ValidationStatus)
    Q_PROPERTY(bool enable READ getEnable WRITE setEnable NOTIFY enableChanged)
    Q_PROPERTY(StatusType status READ getStatus WRITE setStatus NOTIFY statusChanged)
//    Q_PROPERTY(ValidationStatusType validationStatus READ getValidationStatus WRITE setValidationStatus NOTIFY validationStatusChanged)

public:
    Q_INVOKABLE ValidationStatus(ezechiel::GuiCore::AbstractRepository *repository = nullptr, QObject *parent = nullptr);

    enum StatusType { Invalid = 0,
                      Valid,
                      FullyValidated
    };
    Q_ENUM(StatusType)

    enum DataStatusType {
        InvalidData = 0,
        ValidData
    };

    enum ValidationStatusType {
        UnValidated = 0,
        Validated
    };
    /// \todo remove the next 4 functions
    StatusType getStatus() { return StatusType::Invalid;}
    void setStatus(StatusType type){}

    bool getEnable() { return true; }
    void setEnable(bool type) {}

    Q_INVOKABLE StatusType getStatus(StepType::Enum step) const;
//    Q_INVOKABLE void setStatus(StepType step, StatusType type);

    Q_INVOKABLE DataStatusType getDataStatus(StepType::Enum step) const;
    Q_INVOKABLE ValidationStatusType getValidationStatus(StepType::Enum step) const;
    Q_INVOKABLE void setDataStatus(StepType::Enum step, DataStatusType type);
    Q_INVOKABLE void setValidationStatus(StepType::Enum step, ValidationStatusType type);

    void validate(StepType::Enum step);
    void toggleValidation(StepType::Enum step);

    Q_INVOKABLE bool getEnable(StepType::Enum step) const;
    Q_INVOKABLE void setEnable(StepType::Enum step, bool type);

    void copyFrom(ValidationStatus *other);

    bool operator==(ValidationStatus& other);

signals:
    void statusChanged(StatusType);
    void enableChanged(bool);
    void validationStatusChanged(ValidationStatusType);
private:
    QVector<DataStatusType> _dataStatus;
    QVector<ValidationStatusType> _validationStatus;
    QVector<bool> _enable;
};

Q_DECLARE_METATYPE(ValidationStatus*)
Q_DECLARE_METATYPE(QList<ValidationStatus*>)

#endif // VALIDATIONSTATUS_H
