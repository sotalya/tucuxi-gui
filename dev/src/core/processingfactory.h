#ifndef PROCESSINGFACTORY_H
#define PROCESSINGFACTORY_H

#include "core/interfaces/processinginterface.h"

class ProcessingFactory
{
public:
    static ezechiel::core::ProcessingInterface* createProcessing();
    static ezechiel::core::ProcessingInterface* createProcessing(std::string what);

public:
    ProcessingFactory();
};

#endif // PROCESSINGFACTORY_H
