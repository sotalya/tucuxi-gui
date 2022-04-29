//@@license@@

#ifndef PERCENTILETHREAD_H
#define PERCENTILETHREAD_H

#include <QThread>
#include <QSemaphore>

#include "dataset.h"

class PercentileThread : public QThread
{
    Q_OBJECT
public:
    explicit PercentileThread(QObject *parent = 0);
    ~PercentileThread();

    //! Calculate the percentile for these peoples
    void calcPercentile(DataSet **people, int nbPeople, int start, int end, double percent, DataSet *result);

    //! Wait for the thread to finish its work
    void waitForEnd();

    //! Terminates the thread if the thread is waiting for a new processing
    void askForTermination();

protected:

    virtual void run();

    QSemaphore semStart;
    QSemaphore semEnd;

    DataSet **people;
    int nbPeople;
    int starti;
    int end;
    double percent;
    DataSet *result;
    bool shouldTerminate;

    
};

#endif // PERCENTILETHREAD_H
