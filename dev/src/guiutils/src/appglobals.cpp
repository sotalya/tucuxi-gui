#include "appglobals.h"

AppGlobals* AppGlobals::m_instance = nullptr;

AppGlobals::AppGlobals()
{}

const QString AppGlobals::getListFile(){
    return m_listFile;
}

const QString AppGlobals::getRequestFile(){
    return m_requestFile;
}

void AppGlobals::setListFile(QString _listFile){
    m_listFile = _listFile;
}

void AppGlobals::setRequestFile(QString _requestFile){
    m_requestFile = _requestFile;
}

