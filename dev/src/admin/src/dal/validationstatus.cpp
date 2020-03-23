#include "admin/src/dal/validationstatus.h"

ValidationStatus::ValidationStatus(ezechiel::core::AbstractRepository *repository, QObject *parent) :
    ezechiel::core::Entity(repository, parent)
{
    _dataStatus.fill(DataStatusType::InvalidData, StepType::size);
    _validationStatus.fill(ValidationStatusType::UnValidated, StepType::size);
    _enable.fill(false, StepType::size);
    _enable[StepType::Patient] = true;
}


ValidationStatus::StatusType ValidationStatus::getStatus(StepType::Enum step) const
{
    if (_validationStatus[step] == ValidationStatusType::Validated)
        return StatusType::FullyValidated;
    if (_dataStatus[step] == DataStatusType::InvalidData)
        return StatusType::Invalid;
    return StatusType::Valid;
/*
    if (_dataStatus[step] == DataStatusType::InvalidData)
        return StatusType::Invalid;
    if (_validationStatus[step] == ValidationStatusType::UnValidated)
        return StatusType::Valid;
    return StatusType::FullyValidated;
    */
}

/*
#include <iostream>

void ValidationStatus::setStatus(StepType step, StatusType type)
{
    std::cerr << "Error";
}
*/

void ValidationStatus::setDataStatus(StepType::Enum step, DataStatusType type)
{
    if (_dataStatus[step] == type)
        return;

    // If we have invalid data, then the validation status is reset
    switch (type) {
    case DataStatusType::InvalidData : {
        _dataStatus[step] = DataStatusType::InvalidData;
        _validationStatus[step] = ValidationStatusType::UnValidated;
    } break;
    case DataStatusType::ValidData : {
        _dataStatus[step] = DataStatusType::ValidData;
    }
    }
}

void ValidationStatus::setValidationStatus(StepType::Enum step, ValidationStatusType type)
{
    _validationStatus[step] = type;
}

void ValidationStatus::validate(StepType::Enum step)
{
    // Depending on the politics we could restrict validation when data is valid
//    if (_dataStatus[step] == DataStatusType::ValidData)
        _validationStatus[step] = ValidationStatusType::Validated;
}

void ValidationStatus::toggleValidation(StepType::Enum step)
{
    if (_validationStatus[step] == ValidationStatusType::Validated)
        _validationStatus[step] = ValidationStatusType::UnValidated;
    else
        _validationStatus[step] = ValidationStatusType::Validated;
}

ValidationStatus::DataStatusType ValidationStatus::getDataStatus(StepType::Enum step) const
{
    return _dataStatus[step];
}

ValidationStatus::ValidationStatusType ValidationStatus::getValidationStatus(StepType::Enum step) const
{
    return _validationStatus[step];
}

bool ValidationStatus::getEnable(StepType::Enum step) const
{
    return _enable[step];
}

void ValidationStatus::setEnable(StepType::Enum step, bool type)
{
    _enable[step] = type;
}

void ValidationStatus::copyFrom(ValidationStatus *other)
{
    for (int i=StepType::first; i<=StepType::last; i++) {
        StepType::Enum stepType = StepType::convert(i);
        _dataStatus[i] = other->getDataStatus(stepType);
        _validationStatus[i] = other->getValidationStatus(stepType);
        _enable[i] = other->getEnable(stepType);
    }
}

bool ValidationStatus::operator==(ValidationStatus& other)
{
    for (int i=StepType::first; i<=StepType::last; i++) {
        StepType::Enum stepType = StepType::convert(i);
        if (getStatus(stepType) != other.getStatus(stepType)) { return false; }
        if (_enable[i] != other.getEnable(stepType)) { return false; }
    }
}
