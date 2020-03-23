#ifndef DURATIONSPINBOX_H
#define DURATIONSPINBOX_H

#include <QDoubleSpinBox>

class DurationSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    
public:
    explicit DurationSpinBox(QWidget *parent = 0);

    virtual QString textFromValue(double value) const;
    virtual double valueFromText(const QString &text) const;
    virtual QValidator::State validate(QString &input, int &pos) const;

    static QString toString(double value);
};

#endif // DURATIONSPINBOX_H
