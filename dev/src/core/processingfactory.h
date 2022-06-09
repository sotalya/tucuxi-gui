//@@license@@

#ifndef PROCESSINGFACTORY_H
#define PROCESSINGFACTORY_H

#include "core/interfaces/processinginterface.h"

class ProcessingFactory
{
public:
    static ezechiel::GuiCore::ProcessingInterface* createProcessing();
    static ezechiel::GuiCore::ProcessingInterface* createProcessing(std::string what);

public:
    ProcessingFactory();
};

#endif // PROCESSINGFACTORY_H
