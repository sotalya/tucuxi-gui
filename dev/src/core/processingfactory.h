//@@license@@

#ifndef PROCESSINGFACTORY_H
#define PROCESSINGFACTORY_H

#include "core/interfaces/processinginterface.h"

class ProcessingFactory
{
public:
    static Tucuxi::Gui::Core::ProcessingInterface* createProcessing();
    static Tucuxi::Gui::Core::ProcessingInterface* createProcessing(std::string what);

public:
    ProcessingFactory();
};

#endif // PROCESSINGFACTORY_H
