//@@license@@

#include "processingrequest.h"

#include "drugresponseanalysis.h"

#include <iostream>

using namespace Tucuxi::Gui::Core::Core;

ProcessingRequest::ProcessingRequest()
{
}

/*
QDataStream &operator<<(QDataStream &stream, const PostEngine & engine)
{
    stream << engine.id();
}

QDataStream &operator>>(QDataStream &stream, PostEngine & engine)
{
    int id;
    stream >> id;
    // Create the correct engine
}

QDataStream &operator<<(QDataStream &stream, const PercentileEngine & engine)
{
}

QDataStream &operator>>(QDataStream &stream, PercentileEngine & engine)
{
}

QDataStream &operator<<(QDataStream &stream, const ReverseEngine & engine)
{
}

QDataStream &operator>>(QDataStream &stream, ReverseEngine & engine)
{
}
*/

QDataStream &operator<<(QDataStream &stream, const DrugModel & model)
{

}

QDataStream &operator<<(QDataStream &stream, const PatientVariateSet & set)
{

}

QDataStream &operator<<(QDataStream &stream, const CoreMeasureSet & model)
{

}

QDataStream &operator<<(QDataStream &stream, const TargetSet & model)
{

}

QDataStream &operator<<(QDataStream &stream, const DosageHistory & model)
{

}

QDataStream &operator<<(QDataStream &stream, const DrugTreatment & treatment)
{
    stream << *treatment.drugmodel();
    int size;
    size = treatment.covariates().size();
    stream << size;
    for(int i=0;i<size;i++)
        stream << *treatment.covariates().at(i);
    stream << *treatment.measures();
    stream << *treatment.dosages();
    stream << *treatment.targets();
}

QDataStream &operator>>(QDataStream &stream, DrugTreatment & treatment)
{
    int size;
    stream >> size;

}


QDataStream &operator>>(QDataStream &stream, DrugResponseAnalysis & analysis)
{
    int postEngineId;
    int percentileEngineId;
    int reverseEngineId;
    stream >> postEngineId;
    stream >> percentileEngineId;
    stream >> reverseEngineId;
    // TODO : We need to create the various engines, based on their Ids.

    DrugTreatment *treatment = new DrugTreatment(0);
    stream >> *treatment;
}

QDataStream &operator<<(QDataStream &stream, const DrugResponseAnalysis & analysis)
{
    if (!analysis.postEngine().isNull())
        stream << analysis.postEngine()->id();
    else
        stream << static_cast<int>(0);
    if (!analysis.percentileEngine().isNull())
    stream << analysis.percentileEngine()->id();
    else
        stream << static_cast<int>(0);
    if (!analysis.reverseEngine().isNull())
        stream << analysis.reverseEngine()->id();
    else
        stream << static_cast<int>(0);
    stream << *analysis.treatment();

}


// Seems complete
QDataStream &operator>>(QDataStream &stream, PredictionSpec & req)
{
    QDateTime start;
    QDateTime end;
    bool steadyState;
    int nbPoints;
    int nbPercentiles;
    double percentile;
    stream >> start;
    stream >> end;
    stream >> nbPoints;
    stream >> nbPercentiles;
    double *percentiles = new double[nbPercentiles];
    for(int i=0;i<nbPercentiles;i++) {
        stream >> percentiles[i];
    }
    req.setPercentileList(percentiles,nbPercentiles);

    req.setStartDate(start);
    req.setEndDate(end);
    req.setNbPoints(nbPoints);

}

// Seems complete
QDataStream &operator<<(QDataStream &stream, const PredictionSpec & req)
{
    stream << req.startDate();
    stream << req.endDate();
    stream << req.nbPoints();
    std::cout << "NB points: " << req.nbPoints() << std::endl;
    stream << static_cast<int>(req.nbPercentiles());
    for(int i=0;i<req.nbPercentiles();i++)
        stream << req.percentileList()[i];
}

// Seems complete
QDataStream &operator>>(QDataStream &stream, Prediction & req)
{
    SharedPredictionSpec spec = SharedPredictionSpec(new PredictionSpec(0));
    stream >> *spec;
    req.setSpec(spec);
    quint32 curveType;
    stream >> curveType;
    req.setCurveType(static_cast<Prediction::CurveType>(curveType));
    quint32 parameterType;
    stream >> parameterType;
    req.setParamsType(static_cast<ParameterType>(parameterType));
    SharedDrugResponseAnalysis analysis = SharedDrugResponseAnalysis(new DrugResponseAnalysis(0));
    stream >> *analysis;
    req.setAnalysis(analysis);
}

// Seems complete
QDataStream &operator<<(QDataStream &stream, const Prediction & req)
{
    stream << *req.spec();
    stream << static_cast<quint32>(req.curveType());
    stream << static_cast<quint32>(req.paramsType());
    stream << *req.analysis();
}

// Seems complete
QDataStream &operator>>(QDataStream &stream, ProcessingRequest & req)
{
    SharedDrugResponseAnalysis analysis = SharedDrugResponseAnalysis(new DrugResponseAnalysis(0));
    stream >> *analysis;
    req.analysis = analysis;
}

// Seems complete
QDataStream &operator<<(QDataStream &stream, const ProcessingRequest & req)
{
    stream << *req.analysis;
}
