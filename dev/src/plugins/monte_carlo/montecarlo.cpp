#include "montecarlo.h"

#include <QFile>
#include <QTextStream>
#include <QtPlugin>
#include <QThread>

#include "coremessagehandler.h"
#include "core_errors.h"
#include "units.h"
#include "curvegeneratorthread.h"
#include "percentilethread.h"

#include <math.h>

#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/lagged_fibonacci.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/math/distributions/normal.hpp>

#include "cholesky.hpp"

typedef boost::numeric::ublas::row_major ORI;
typedef double DBL;

//------------------------------------ FACTORY -------------------------------------

//Save it, and create our descriptor
void MonteCarloFactory::init(Core *core)
{
    //Set the core
    Core::setInstance(core);

    //Oh yes we are
    _d.id = MONTE_CARLO_ID;
    _d.name = "Monte Carlo";
    _d.description = "Prediction engine using montecarlo to get percentiles.";

    //Store the priorities
    _priorities.insert(_d.id, MonteCarlo(_d).priority());
}

//Create it with our description
PercentileEngine *MonteCarloFactory::newPercentileEngine()
{
    return new MonteCarlo(_d);
}

//Created in init()
Descriptor MonteCarloFactory::descriptor()
{
    return _d;
}

//Returns an engine priority
int MonteCarloFactory::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}

//------------------------------------ PERCENTILE ENGINE -------------------------------------

// Options OID definition
#define NBPEOPLE_OPTION "1nbpeople"
#define NBTHREADS_OPTION "2nbthreads"
#define CORRELATION_OPTION "3correlation"
#define EXPORTPEOPLE_OPTION "4exportpeople"
#define PEOPLEFILE_OPTION "5peoplefile"

//The engine default priority level
const int MonteCarlo::_priority = 50;

//Save the descriptor, setup the options
MonteCarlo::MonteCarlo (const Descriptor &d) :
    _model(0)
{
    _d = d;

    //Determine the best number of thread to be used, or use 1 if not available
    idealThreadsNb = QThread::idealThreadCount();
    idealThreadsNb = idealThreadsNb == -1 ? 1 : idealThreadsNb;

    //Number of generated people for the montecarlo
    addOption(NBPEOPLE_OPTION, tr("Number of generated people"), tr("The montecarlo works by creating a certain number of patients, calculates the a prediction for each one of them and sorting the result. The number of generated people can be set here, the bigger this value is the better the results will be, but the longer the montecarlo will take."),
              QVariant::Int, QVariant(1000));

    //Number of threads to use
    addOption(NBTHREADS_OPTION, tr("Number of threads to use"), tr("The montecarlo can use multiple threads to speed up the calulation, you should put here the same number as your computer CPU cores. Using -1 means that the best number of threads will be used."),
              QVariant::Int, QVariant(idealThreadsNb));

    //Shall we use correlation of parameters
    addOption(CORRELATION_OPTION, tr("Use correlation of parameters"), tr("If checked, then the population is generated from the correlation matrix, else only the variances are used."),
              QVariant::Bool, QVariant(true));

    //Shall we export the generated people into a file
    addOption(EXPORTPEOPLE_OPTION, tr("Export people into a file"), tr("If checked, then all the points of the generated people are saved into the file selected."),
              QVariant::Bool, QVariant(false));

    //Name of the file in which the people points are saved
    addOption(PEOPLEFILE_OPTION, tr("Filename for export"), tr("If the people points are exported, this is the name of the file."),
              qMetaTypeId<FileName>(), QVariant::fromValue(FileName("","*.csv")));

}

//Create and copy
PercentileEngine *MonteCarlo::clone()
{
    MonteCarlo *instance = new MonteCarlo(_d);
    instance->copyOptions(this);
    return instance;
}

//Given by the factory
Descriptor MonteCarlo::descriptor ()
{
    return _d;
}

int MonteCarlo::priority() const
{
    return _priority;
}

//Never seen this changed
Version MonteCarlo::version ()
{
    return Version("0.1");
}

//Mostly empty
QString MonteCarlo::errorString()
{
    return _errors;
}

//Simply save it
void MonteCarlo::setAnalysis(SharedDrugResponseAnalysis model)
{
    _model = model;
}

SharedDrugResponseAnalysis MonteCarlo::analysis()
{
    return _model;
}

boost::numeric::ublas::matrix<DBL, ORI> MonteCarlo::createCorrMatrix(const QList<ParameterSet*> &parameters){

    // Calculate the size of the correlation matrix
    // We create the smallest correlation matrix possible.
    // It will only contain the the parameters that do not have a 0 standard deviation
    int corrMatrixSize=0;
    foreach(QString pName, parameters.first()->pids()) {
        SharedParameter param = parameters.first()->get(pName);
        if ((param->proportional()!=0.0) || (param->standard()!=0.0)) {
            corrMatrixSize++;
        }
    }

    // We build the correlation matrix
    boost::numeric::ublas::matrix<DBL, ORI> cor_matrix = boost::numeric::ublas::identity_matrix<DBL>(corrMatrixSize);

    // Populate the correlation matrix
    int param_index=0;
    foreach(QString pName, parameters.first()->pids()) {
        SharedParameter param = parameters.first()->get(pName);
        if ((param->proportional()!=0.0) || (param->standard()!=0.0)) {

            int param_index2=0;
            foreach(QString pName2, parameters.first()->pids()) {
                SharedParameter param2 = parameters.first()->get(pName2);
                if (parameters.first()->correlation(pName,pName2)!=0.0) {
                    cor_matrix(param_index,param_index2)=parameters.first()->correlation(pName,pName2);
                }
                if ((param2->proportional()!=0.0) || (param2->standard()!=0.0))
                    param_index2++;
            }
            param_index++;
        }
    }
    return cor_matrix;
}


//Yann's magic again!
QMap<int,DataSet> MonteCarlo::percentiles (const QList<int> &values, const Duration &start, const Duration &stop, uint points, const QList<ParameterSet*> &parameters, bool uniform)
{
    Q_UNUSED(uniform);

    //Fetching the options
    int nbPeople = option(NBPEOPLE_OPTION).value.toInt();
    int nbThreads = option(NBTHREADS_OPTION).value.toInt();
    bool exportPeople = option(EXPORTPEOPLE_OPTION).value.toBool();
    FileName peopleFile = option(PEOPLEFILE_OPTION).value.value<FileName>();

    //For the number of threads, if it equals to 0, use the ideal value
    if (nbThreads == 0)
        nbThreads = idealThreadsNb;

    //Fetch the base parameters
    ParameterSet* firstParameterSet = *parameters.constBegin();

    //Static stuff, shared by the threads
    static DataSet **people=0;
    static uint last_nbPeople;

    boost::numeric::ublas::matrix<DBL, ORI> cor_matrix = createCorrMatrix(parameters);
    boost::numeric::ublas::matrix<DBL, ORI> L (cor_matrix.size1(), cor_matrix.size1());
    cholesky_decompose(cor_matrix,L);

    {
        //Remove the old DataSet
        if (people!=0)
        {
            //DataSet do not like to be deleted directly with delete[], God only knows why
            for (uint i=0; i<last_nbPeople; i++)
                delete people[i];
            delete[] people;
        }
        last_nbPeople = nbPeople;

        //One DataSet per people
        people = new DataSet*[nbPeople];        

        CurveGeneratorThread *threads[nbThreads];
        for(int i=0; i<nbThreads; i++){
            threads[i]=new CurveGeneratorThread();
            threads[i]->start();
        }

        int pplPerThread = int(nbPeople / nbThreads);
        //Start the calculation on each thread
        for(int i=0; i<nbThreads; i++)
            threads[i]->generateCurves(parameters, people, _model, pplPerThread, points, i * pplPerThread, i * pplPerThread + pplPerThread, start, stop, L);
        //Wait for the threads to finish
        for(int i=0; i<nbThreads; i++)
            threads[i]->waitForEnd();

        //Destroy the threads
        for(int i=0;i<nbThreads;i++) {
            threads[i]->askForTermination();
            threads[i]->wait();
            delete threads[i];
        }

    }

    // Potential export of all points into a .csv file.
    // This file can be useful for Matlab/Octave further analysis
    if (exportPeople) {
        QFile file(peopleFile);
        if (file.open(QFile::WriteOnly))
        {
            QTextStream out(&file);
            for(int peo=0;peo<nbPeople;peo++) {
                for(unsigned int po=0;po<points;po++) {
                    out << people[peo]->at(po) << "\t";
                }
                out << "\n";
            }
            file.close();
        }
        else
            WARNING("The file for exporting people points could not be opened");
    }


    QMap<int, DataSet> results;


    //Create the threads and start them
    PercentileThread *threads[nbThreads];
    for(int i=0; i<nbThreads; i++){
        threads[i]=new PercentileThread();
        threads[i]->start();
    }


    //For each percentile value to calculate
    foreach (int percentile, values) {

        DataSet result(people[0]->unit());
        foreach (CycleData cycle, people[0]->allCycles()) {
            CycleData newCycle(cycle.size(), cycle.unit());
            newCycle.setTimes(cycle.startTime(), cycle.endTime(), cycle.type());
            result.addCycleData(newCycle);
        }

        //Start the calculation on each thread
        for(int i=0; i<nbThreads; i++)
            threads[i]->calcPercentile(people,nbPeople,i*points/nbThreads,(i+1)*points/nbThreads-1,percentile,&result);

        //Wait for the threads to finish
        for(int i=0; i<nbThreads; i++)
            threads[i]->waitForEnd();

        //Save the result
        results.insert(percentile, result);
    }

    //Destroy the threads
    for(int i=0;i<nbThreads;i++) {
        threads[i]->askForTermination();
        threads[i]->wait();
        delete threads[i];
    }

    return results;
}

