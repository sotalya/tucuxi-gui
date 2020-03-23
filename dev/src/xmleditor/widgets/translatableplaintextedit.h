#ifndef TRANSLATABLEPLAINTEXTEDIT_H
#define TRANSLATABLEPLAINTEXTEDIT_H

#include <QPlainTextEdit>

//! QPlainTextEdit that contains a language button
class TranslatablePlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:

    //! Constructor
    explicit TranslatablePlainTextEdit(QWidget *parent = 0);

    /** \brief Get the current language
     * \return The text of language button
     */
    QString language() const;

public slots:

    /** \brief Set the language button text
     * \param lang The new language button text
     */
    void setLanguage(const QString &lang);

signals:

    //! Signal emitted when the language button is being clicked
    void languageClicked();

protected:

    //! Called when the widget is resized
    void resizeEvent(QResizeEvent *e);

private:
    class QToolButton *_languageBtn;
};

#endif // TRANSLATABLEPLAINTEXTEDIT_H
