#include "durationspinbox.h"

#include <limits>

DurationSpinBox::DurationSpinBox(QWidget *parent) : QDoubleSpinBox(parent)
{
    setSingleStep(0.25);
    setRange(0.0, (double)std::numeric_limits<int>::max());
}

QString DurationSpinBox::textFromValue(double value) const
{
    return toString(value);
}

double DurationSpinBox::valueFromText(const QString &text) const
{
    if (!text.contains('h'))
        return text.toDouble();

    double hours = text.section('h', 0, 0).toDouble();
    double minutes = text.section('h', 1, 1).toDouble();

    return hours + (minutes / 60.0);
}

QValidator::State DurationSpinBox::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);

    QRegExp reg("^([0-9]*)(h([0-5][0-9]|[0-5]?))?$");

    double minutes, hours;

    if (!input.contains('h')) {
        hours = input.toDouble();
        minutes = 0.0;
    } else {
        hours = input.section('h', 0, 0).toDouble();
        minutes = input.section('h', 1, 1).toDouble();
    }

    double duration = hours + (minutes / 60.0);

    if (reg.exactMatch(input) && minimum() <= duration && maximum() >= duration)
        return QValidator::Acceptable;
    else if (reg.exactMatch(input) && maximum() >= duration)
        return QValidator::Intermediate;
    else
        return QValidator::Invalid;
}

QString DurationSpinBox::toString(double value)
{
    int hours = (int)value;
    int minutes = (int)(value * 60) % 60;

    if (minutes == 0)
        return QString::number(hours) + "h00";
    if (minutes < 10)
        return QString::number(hours) + "h0" + QString::number(minutes);

    return QString::number(hours) + "h" + QString::number(minutes);
}
