//@@license@@

#ifndef ABSTRACTTESTWIDGET_H
#define ABSTRACTTESTWIDGET_H

class MirthRequest;
class Message;
class QNetworkReply;
class QDomDocument;
class QPushButton;
class QProgressBar;
class QPlainTextEdit;

#include <QWidget>
#include <QString>

class AbstractTestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractTestWidget(MirthRequest *request, QWidget *parent = 0);
    virtual ~AbstractTestWidget() Q_DECL_OVERRIDE = 0;

    virtual void enable();
    virtual void disable();
    virtual void reset();

    virtual void setQueryMessage(Message *message);
    virtual void setReplyMessage(Message *message);

    void setButtonText(const QString &text);
    QString buttonText();

signals:
    void started(AbstractTestWidget *);
    void stopped(AbstractTestWidget *);
    void errors(const QString &);

protected:
    virtual QPushButton *pushButton() = 0;
    virtual QProgressBar *progressBar() = 0;
    virtual QPlainTextEdit *textArea() = 0;

    void setAcknowledgement(MirthRequest *request, Message *message);

    QDomDocument buildAck(const QString &controlId, const QString &ackType, const QString &ackCode, const QString &ackMessage = QString());
    void sendAck(const QDomDocument &reply, const QString &ackType, const QString &ackCode, const QString &ackMessage = QString());

    QScopedPointer<MirthRequest> _request;
    QScopedPointer<Message> _reply;

    QScopedPointer<MirthRequest> _ackReq;
    Message *_ackMsg;

private slots:
    virtual void displayError(const QString &errorMsg);
    virtual void displaySuccess();

    void start();
    void setProgress(qint64, qint64);
    void requestFinished(QNetworkReply *);
    void ackFinished(QNetworkReply *);
};

#endif // ABSTRACTTESTWIDGET_H
