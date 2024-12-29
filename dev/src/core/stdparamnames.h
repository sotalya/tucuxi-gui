/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
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


#ifndef STDPARAMNAMES_H
#define STDPARAMNAMES_H

#include <QObject>


namespace Tucuxi {
namespace Gui {
namespace Core {



class AbstractStdParamNameHandler : public QObject
{
    Q_OBJECT

public:
    AbstractStdParamNameHandler(){};
    virtual QString getName(const QString &id) = 0;
    virtual QString getDesc(const QString &id) = 0;
};


/*
 * Example of how to declare a new name handler that can be added with
 * StdParamNames::addStdParamNameHandler(handle):
 * This can be useful when designing a new prediction plugin that should
 * not impact the main development trunk.
 * It allows to generate the report with a language different from the one
 * used by the software.
 *
class MyParam : public AbstractStdParamNameHandler
{
public :
    MyParam(){};
    QString getName(const QString &id)
    {
        if (id == "Q5")
            return tr("Q5 name");
        if (id == "Q6")
            return tr("Q6 name");
        return "";

    }

    QString getDesc(const QString &id)
    {
        if (id == "Q5")
            return tr("Q5 desc");
        if (id == "Q6")
            return tr("Q6 desc");
        return "";
    }

};
*/


class StdParamNames : QObject
{
    Q_OBJECT

public:
    StdParamNames();

    static QList<AbstractStdParamNameHandler*> handlersList;

    static void addStdParamNameHandler(AbstractStdParamNameHandler *handler);

    static QString getName(const QString &id);
    static QString getDesc(const QString &id);

    static QString getNameCL();
    static QString getDescCL();
    static QString getNameV();
    static QString getDescV();
    static QString getNameKA();
    static QString getDescKA();
    static QString getNameF();
    static QString getDescF();
    static QString getNameV1();
    static QString getDescV1();
    static QString getNameV2();
    static QString getDescV2();
    static QString getNameV3();
    static QString getDescV3();
    static QString getNameQ();
    static QString getDescQ();
    static QString getNameQ2();
    static QString getDescQ2();
    static QString getNameQ3();
    static QString getDescQ3();
    static QString getNameVM();
    static QString getDescVM();
    static QString getNameKM();
    static QString getDescKM();

    static QString getNameK12();
    static QString getDescK12();
    static QString getNameK21();
    static QString getDescK21();
    static QString getDescK13();
    static QString getNameK13();
    static QString getDescK31();
    static QString getNameK31();
    static QString getNameKe();
    static QString getDescKe();
    static QString getNameGA1();
    static QString getDescGA1();
    static QString getNameGB1();
    static QString getDescGB1();
    static QString getNameGA2();
    static QString getDescGA2();
    static QString getNameGB2();
    static QString getDescGB2();
    static QString getNameA();
    static QString getDescA();
    static QString getNameR();
    static QString getDescR();
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // STDPARAMNAMES_H
