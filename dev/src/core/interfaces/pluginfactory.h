//@@license@@

#ifndef PLUGINFACTORY_H
#define PLUGINFACTORY_H

#include <QObject>
#include "core/core.h"
#include "plugin.h"
//#include "percentileengine.h"
//#include "reverseengine.h"
//#include "model.h"
//#include "postengine.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


//class Factory : public QObject
//{

//public:

//    virtual void init(Core *core) = 0;

////    virtual int priority(const QString &id) const = 0;

//    virtual PluginBase::PluginType ptype() = 0;

//    virtual PluginBase* newPlugin(QString& id) = 0;

////    DescriptorBase* m_desc;

//    virtual Descriptor descriptor () = 0;
////    DescriptorBase descriptor () {
////        return *m_desc;
////    }
//};
//Q_DECLARE_INTERFACE(Tucuxi::Factory, "ch.heig-vd.ezechiel.Factory")


//template<class T>
//class PluginFactory : public Factory
//{
//public:

////    typedef T plugintype;
//    PluginFactory() {
////        m_desc = &desc;
//    }

////    virtual DescriptorBase* descriptor () = 0;
////    virtual Descriptor<PluginFactory<T>> descriptor () = 0;

////    virtual Plugin* newPlugin() = 0;
//    virtual T* newPlugin (QString& id) = 0;

//    Descriptor desc;
//};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // PLUGINFACTORY_H
