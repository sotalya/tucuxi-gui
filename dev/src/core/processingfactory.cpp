//@@license@@

#include "processingfactory.h"

#ifdef COMPILE_WITH_TUCUCORE
#include "processingtucucore/src/processingtucucore.h"
#ifdef COMPILE_WITH_COMPARATOR
#include "processingcomparator/src/processingcomparator.h"
#endif // COMPILE_WITH_COMPARATOR
#else
#include "processing/src/processingmath.h"
#endif // COMPILE_WITH_TUCUCORE

ProcessingFactory::ProcessingFactory()
{

}

Tucuxi::GuiCore::ProcessingInterface *ProcessingFactory::createProcessing()
{
#ifdef COMPILE_WITH_TUCUCORE
#ifdef COMPILE_WITH_COMPARATOR
    return new ProcessingComparator();
#else
    return new ProcessingTucucore();
#endif // COMPILE_WITH_COMPARATOR
#else
    return new ProcessingMath();
#endif // COMPILE_WITH_TUCUCORE
}


Tucuxi::GuiCore::ProcessingInterface *ProcessingFactory::createProcessing(std::string what)
{
    if (what == "comparator") {
#ifdef COMPILE_WITH_TUCUCORE
#ifdef COMPILE_WITH_COMPARATOR
        return new ProcessingComparator();
#else
        return nullptr;
#endif // COMPILE_WITH_COMPARATOR
#else
        return nullptr;
#endif // COMPILE_WITH_TUCUCORE
    }
    else if (what == "tucucore") {
#ifdef COMPILE_WITH_TUCUCORE
        return new ProcessingTucucore();
#else
        return nullptr;
#endif // COMPILE_WITH_TUCUCORE
    }
    else if (what == "ezechiel") {
        return nullptr;
//        return new ProcessingMath();
    }
}
