#ifndef TRANSLATIONWIDGET_H
#define TRANSLATIONWIDGET_H

#include <QWidget>

class TranslationWidget : public QWidget
{
    Q_OBJECT

public:

    /** \brief Default constructor.
     * @param parent An optional parent widget.
     * @param f An optional Qt::WindowFlags.
     */
    TranslationWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

    //! Virtual destructor
    ~TranslationWidget() = 0;

    void clear();
    void setText(const QString &text, const QString &lang = QString());
    void displayedText(const QString &lang = QString()) const;

signals:
    void textChanged(const QString &text, const QString &lang = QString());
    void editingFinished(const QString &text, const QString &lang = QString());
    void languageChanged(const QString &lang);

protected:
    QMap<QString, QString> languages() const;

private:
    virtual void clearImpl() = 0;
    virtual void setTextImpl(const QString &text, const QString &lang = QString()) = 0;
    virtual void displayedTextImpl(const QString &lang) const = 0;

    //Maps of languages/ISO code
    static QMap<QString, QString> _languageToCode;
    static QMap<QString, QString> _codeToLanguage;

};

#endif // TRANSLATIONWIDGET_H
