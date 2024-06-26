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


#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QList>

#include "core/interfaces/optionable.h"
#include "core/dal/drug/parameters.h"
#include "core/dal/dosage.h"
#include "core/dal/residual.h"
#include "core/utils/ezutils.h"
#include "core/utils/version.h"
#include "core/utils/dataset.h"
#include "core/utils/duration.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



//#define MODELUTILITIES(CLASSNAME) \
//private: \
//DrugModelEngine *DrugModelEngine::doClone()\
//{ \
//    DrugModelEngine *other = new CLASSNAME(); \
//    other->copyOptions(this); \
//    return other; \
//}

/** The model plugin interface.
 * \ingroup core models
 * This interface use the Qt framework plugin system to allow an implementing class to be a plugin.
 * A model has the role of calculating an actual prediction.
 *
 * To do so, a model is given everything it needs : the start and stop time, the parameters to use, the dosage to use and the residual concentration at the start of the cycle.
 * A model should not worry about the dosage interval as the start and stop time are always inside it.
 * This also means that no multi-cycle handling is asked.
 *
 * The parameters are defined in the drug descriptor XML, but each model has its own parameters which might be shown to the user.
 * They then should be described and translated directly in the model, using Qt's tr() function.
 *
 * A model might have options, see the Optionable object.
 */
class DrugModelEngine : public Optionable
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString version READ version WRITE setVersion)
    Q_PROPERTY(QString name READ name WRITE setName)

public:

    int id() const {return _id;}
    void setId(const int &id) {_id = id;}

    QString name() const {return _name;}
    void setName(const QString &name) {_name = name;}

    QString version() {return _version.toString();}
    void setVersion(const QString &version) {_version.fromString(version);}

    //! Virtual destructor of the interface.
    virtual ~DrugModelEngine() {}

    /** Clone this model.
     * All the options must be copied as well.
     * @return A new model cloned from this one
     */

//    DrugModelEngine* clone ();

    /** Returns the model priority level.
     * When there is more than one model available for a given drug, the priority level
     * will be used to determine which one will be used as the default engine when creating
     * or editing a curve. The lower the number is, the higher the priority is.
     */
    int priority ();

protected:
    QString _errors;
    virtual DrugModelEngine* doClone() = 0;
    int _priority;

private:    
    Version _version;
    QString _name;
    int _id;
};

typedef DrugModelEngine* SharedModel;

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::DrugModelEngine*);

#endif // MODEL_H
