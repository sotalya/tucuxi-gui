#ifndef MELDWORKERTHREAD_H
#define MELDWORKERTHREAD_H

#include <QThread>

class MeldWorkerThread : public QThread
{
    Q_OBJECT

public:
    MeldWorkerThread(const QString &received, const QString &expected, QObject *parent = nullptr);
    virtual ~MeldWorkerThread() Q_DECL_OVERRIDE;

    void run() Q_DECL_OVERRIDE;

private:
    QString _received;
    QString _expected;
};

#endif // MELDWORKERTHREAD_H
