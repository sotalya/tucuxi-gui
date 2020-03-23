#include "percentilethread.h"

PercentileThread::PercentileThread(QObject *parent) :
    QThread(parent),
    people(0),
    nbPeople(0)
{
}

PercentileThread::~PercentileThread()
{
    if (people!=0)
        delete[] people;
}

void PercentileThread::calcPercentile(DataSet **people,int nbPeople,int start,int end,double percent,DataSet *result)
{
    if (this->nbPeople!=nbPeople)
    {
        if (this->people!=0)
            delete[] this->people;
        this->people=new DataSet*[nbPeople];
    }
    //       memcpy(people,this->people,nbPeople*sizeof(DataSet *));
    for(int i=0;i<nbPeople;i++)
        this->people[i]=people[i];
    this->nbPeople=nbPeople;
    this->starti=start;
    this->end=end;
    this->percent=percent;
    this->result=result;
    this->shouldTerminate=false;
    semStart.release();
}

void PercentileThread::waitForEnd() {
    semEnd.acquire();
}

void PercentileThread::askForTermination() {
    shouldTerminate=true;
    semStart.release();
}

void PercentileThread::run()
{
    while(1) {
        semStart.acquire();
        if (shouldTerminate)
        {

            return;
        }

        int index;
        index=((double)nbPeople)*percent/100.0;

        DataSet interResult(end-starti+1,result->unit());

        int maxIndex;
        if (percent>50) {
            for(int i=starti;i<=end;i++) {
                do {
                    maxIndex=0;
                    for(int j=0;j<nbPeople-1;j++) {
                        if (people[j]->at(i) > people[j+1]->at(i)) {
                            maxIndex=j+1;
                            DataSet *tmp=people[j];
                            people[j]=people[j+1];
                            people[j+1]=tmp;
                        }
                    }
                } while (maxIndex>index);
                interResult[i-starti]=people[index]->at(i);
                //(*result)[i]=people[index]->at(i);
            }
        } else {
            int minIndex;
            for(int i=starti;i<=end;i++) {
                do {
                    minIndex=nbPeople;
                    for(int j=nbPeople-2;j>=0;j--) {
                        if (people[j]->at(i) > people[j+1]->at(i)) {
                            minIndex=j;
                            DataSet *tmp=people[j];
                            people[j]=people[j+1];
                            people[j+1]=tmp;
                        }
                    }
                } while (minIndex<index);
                interResult[i-starti]=people[index]->at(i);
                //(*result)[i]=people[index]->at(i);
            }
        }
        for(int i=starti;i<=end;i++)
            (*result)[i]=interResult[i-starti];
        semEnd.release();
    }
}

