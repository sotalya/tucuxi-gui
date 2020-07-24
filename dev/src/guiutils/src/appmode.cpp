#include "appmode.h"

AppMode* AppMode::m_instance = nullptr;

AppMode::AppMode()
{}

bool AppMode::isDemo(){
#ifdef CONFIG_DEMO
    return true;
#else
    return false;
#endif
}

bool AppMode::isEducational(){
    return false;
}

bool AppMode::isInstitutional(){
    return false;
}

bool AppMode::isPractice(){
    return false;
}
