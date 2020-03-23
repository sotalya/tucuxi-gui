#include "translatablelineedit.h"

#include <QToolButton>
#include <QLineEdit>
#include <QStyle>

TranslatableLineEdit::TranslatableLineEdit(QWidget *parent) : QLineEdit(parent)
{
    _languageBtn = new QToolButton(this);
    _languageBtn->setText("en");
    _languageBtn->setCursor(Qt::ArrowCursor);
    _languageBtn->setStyleSheet("QToolButton { font-style : italic; }");

    connect(_languageBtn, SIGNAL(clicked()), this, SIGNAL(languageClicked()));
}

QString TranslatableLineEdit::language() const
{
    return _languageBtn->text();
}

void TranslatableLineEdit::setLanguage(const QString &lang)
{
    _languageBtn->setText(lang);
}

void TranslatableLineEdit::resizeEvent(QResizeEvent *e)
{
    //Padding around the button
    int padding = 3;

    //Button new height and width
    int newHeight = height() - (padding * 2);
    if (newHeight < 15)
        newHeight = 15;

    int newWidth = _languageBtn->sizeHint().width();

    //Resize and move the button
    _languageBtn->resize(newWidth, newHeight);
    _languageBtn->move(rect().right() - newWidth - padding, rect().top() + padding);

    //Adapt the widget right padding
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    setStyleSheet(QString("QLineEdit { padding-right: %1px; }").arg(newWidth + frameWidth + padding));

    //Propagate the resize event
    QLineEdit::resizeEvent(e);
}
