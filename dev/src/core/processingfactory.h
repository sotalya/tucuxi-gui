//@@license@@

#ifndef PROCESSINGFACTORY_H
#define PROCESSINGFACTORY_H

#include "core/interfaces/processinginterface.h"

class ProcessingFactory
{
public:
    static Tucuxi::GuiCore::ProcessingInterface* createProcessing();
    static Tucuxi::GuiCore::ProcessingInterface* createProcessing(std::string what);

public:
    ProcessingFactory();
};

#endif // PROCESSINGFACTORY_H
