#include "translatableplaintextedit.h"

#include <QToolButton>
#include <QPlainTextEdit>
#include <QStyle>
#include <QScrollBar>
#include <algorithm>

TranslatablePlainTextEdit::TranslatablePlainTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    _languageBtn = new QToolButton(this);
    _languageBtn->setText("en");
    _languageBtn->setCursor(Qt::ArrowCursor);
    _languageBtn->setStyleSheet("QToolButton { font-style : italic; }");

    connect(_languageBtn, SIGNAL(clicked()), this, SIGNAL(languageClicked()));
}

QString TranslatablePlainTextEdit::language() const
{
    return _languageBtn->text();
}

void TranslatablePlainTextEdit::setLanguage(const QString &lang)
{
    _languageBtn->setText(lang);
}

void TranslatablePlainTextEdit::resizeEvent(QResizeEvent *e)
{
    //Padding around the button
    int padding = 3;

    //Button new height and width
    QSize btnSize = _languageBtn->sizeHint();

    int newHeight = std::min(height() - (padding * 2), btnSize.height() - (padding * 2));
    if (newHeight < 15)
        newHeight = 15;

    int newWidth = btnSize.width();

    //Resize and move the button
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    int xPos = rect().right() - newWidth - padding - (verticalScrollBar()->isVisible() ? verticalScrollBar()->frameGeometry().width() + frameWidth : 0);
    int yPos = rect().bottom() - newHeight - padding - (horizontalScrollBar()->isVisible() ? horizontalScrollBar()->frameGeometry().height() + frameWidth : 0);

    _languageBtn->resize(newWidth, newHeight);
    _languageBtn->move(xPos, yPos);

    //Propagate the resize event
    QPlainTextEdit::resizeEvent(e);
}
