#ifndef TRANSLATABLELINEEDIT_H
#define TRANSLATABLELINEEDIT_H

#include <QLineEdit>

//! QLineEdit that contains a language button
class TranslatableLineEdit : public QLineEdit
{
    Q_OBJECT

public:

    //! Constructor
    explicit TranslatableLineEdit(QWidget *parent = 0);

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

#endif // TRANSLATABLELINEEDIT_H
