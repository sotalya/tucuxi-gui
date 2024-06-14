/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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
